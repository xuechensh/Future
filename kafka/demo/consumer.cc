#include "consumer.h"

#include <iostream>

class ConsumerEventCb : public RdKafka::EventCb {
 public:
  void event_cb(RdKafka::Event& event) {
    switch (event.type()) {
      case RdKafka::Event::EVENT_ERROR:
        std::cerr << "ERROR (" << RdKafka::err2str(event.err())
                  << "): " << event.str() << std::endl;
        break;
      case RdKafka::Event::EVENT_STATS:
        std::cerr << "STATS: " << event.str() << std::endl;
        break;
      case RdKafka::Event::EVENT_LOG:
        fprintf(stderr, "LOG-%i-%s: %sn", event.severity(), event.fac().c_str(),
                event.str().c_str());
        break;
      case RdKafka::Event::EVENT_THROTTLE:
        std::cerr << "THROTTLED: " << event.throttle_time() << "ms by "
                  << event.broker_name() << " id " << (int)event.broker_id()
                  << std::endl;
        break;
      default:
        std::cerr << "EVENT " << event.type() << " ("
                  << RdKafka::err2str(event.err()) << "): " << event.str()
                  << std::endl;
        break;
    }
  }
};

class ConsumerRebalanceCb : public RdKafka::RebalanceCb {
 public:
  // Kafka服务端通过
  // err参数传入再均衡的具体事件（发生前、发生后），通过partitions参数传入再均衡
  // 前/后，旧的/新的 分区信息
  void rebalance_cb(RdKafka::KafkaConsumer* consumer, RdKafka::ErrorCode err,
                    std::vector<RdKafka::TopicPartition*>& partitions) {
    std::cerr << "RebalanceCb: " << RdKafka::err2str(err) << ": ";
    printTopicPartition(partitions);

    if (err == RdKafka::ERR__ASSIGN_PARTITIONS) {
      // ERR__ASSIGN_PARTITIONS:
      // 表示“再均衡发生之后，消费者开始消费之前”，此时消费者客户端可以从broker上重新加载offset
      // 再均衡后，重新 assign() 订阅这些分区
      consumer->assign(partitions);
      partition_count = (int)partitions.size();
    } else if (err == RdKafka::ERR__REVOKE_PARTITIONS) {
      // ERR__REVOKE_PARTITIONS:
      // 表示“消费者停止消费之后，再均衡发生之前”，此时应用程序可以在这里提交
      // offset
      consumer->unassign();  // 再均衡前，unassign() 退订这些分区
      partition_count = 0;   // 退订所有分区后，清0
    } else {
      std::cerr << "Rebalancing error: " << RdKafka::err2str(err) << std::endl;
    }
  }

 private:
  static void printTopicPartition(
      const std::vector<RdKafka::TopicPartition*>& partitions) {
    // 打印出所有的主题、分区信息
    for (unsigned int i = 0; i < partitions.size(); i++) {
      std::cerr << partitions[i]->topic() << "[" << partitions[i]->partition()
                << "], ";
    }
    std::cerr << "n";
  }

 private:
  int partition_count;  // 保存consumer消费者客户端 当前订阅的分区数
};

KafkaConsumer::KafkaConsumer(const std::string& brokers,
                             const std::string& groupId,
                             const std::vector<std::string>& topics,
                             int partition) {
  m_brokers = brokers;
  m_groupId = groupId;
  m_topicVector = topics;
  m_partition = partition;

  // 创建Conf实例：
  m_config = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
  if (m_config == nullptr) {
    std::cout << "Create Rdkafka Global Conf Failed." << std::endl;
  }

  // 设置Conf的各个配置参数：
  RdKafka::Conf::ConfResult result;
  std::string error_str;

  result = m_config->set("bootstrap.servers", m_brokers, error_str);
  if (result != RdKafka::Conf::CONF_OK) {
    std::cout << "Conf set 'bootstrap.servers' failed: " << error_str
              << std::endl;
  }

  result = m_config->set("group.id", m_groupId,
                         error_str);  // 设置消费组名：group.id（string类型）
  if (result != RdKafka::Conf::CONF_OK) {
    std::cout << "Conf set 'group.id' failed: " << error_str << std::endl;
  }

  result = m_config->set("max.partition.fetch.bytes", "1024000",
                         error_str);  // 消费消息的最大大小
  if (result != RdKafka::Conf::CONF_OK) {
    std::cout << "Conf set 'max.partition.fetch.bytes' failed: " << error_str
              << std::endl;
  }

  result = m_config->set(
      "enable.partition.eof", "false",
      error_str);  // enable.partition.eof: 当消费者到达分区结尾，发送
                   // RD_KAFKA_RESP_ERR__PARTITION_EOF 事件，默认值 true
  if (result != RdKafka::Conf::CONF_OK) {
    std::cout << "Conf set 'enable.partition.eof' failed: " << error_str
              << std::endl;
  }

  m_event_cb = new ConsumerEventCb;
  result = m_config->set("event_cb", m_event_cb, error_str);
  if (result != RdKafka::Conf::CONF_OK) {
    std::cout << "Conf set 'event_cb' failed: " << error_str << std::endl;
  }

  m_rebalance_cb = new ConsumerRebalanceCb;
  result = m_config->set("rebalance_cb", m_rebalance_cb, error_str);
  if (result != RdKafka::Conf::CONF_OK) {
    std::cout << "Conf set 'rebalance_cb' failed: " << error_str << std::endl;
  }

  // 设置topic_conf的配置项
  m_topicConfig = RdKafka::Conf::create(RdKafka::Conf::CONF_TOPIC);
  if (m_topicConfig == nullptr) {
    std::cout << "Create Rdkafka Topic Conf Failed." << std::endl;
  }

  result = m_topicConfig->set("auto.offset.reset", "latest", error_str);
  if (result != RdKafka::Conf::CONF_OK) {
    std::cout << "Topic Conf set 'auto.offset.reset' failed: " << error_str
              << std::endl;
  }

  result = m_config->set("default_topic_conf", m_topicConfig, error_str);
  if (result != RdKafka::Conf::CONF_OK) {
    std::cout << "Conf set 'default_topic_conf' failed: " << error_str
              << std::endl;
  }

  // 创建消费者客户端：
  m_consumer = RdKafka::KafkaConsumer::create(m_config, error_str);
  if (m_consumer == nullptr) {
    std::cout << "Create KafkaConsumer failed: " << error_str << std::endl;
  }
  std::cout << "Create KafkaConsumer succeed, consumer name : "
            << m_consumer->name() << std::endl;

  // 订阅m_topicVector中的topic
  RdKafka::ErrorCode error_code = m_consumer->subscribe(m_topicVector);
  if (error_code != RdKafka::ErrorCode::ERR_NO_ERROR) {
    std::cerr << "Consumer subscribe topics failed: "
              << RdKafka::err2str(error_code) << std::endl;
  }
}

KafkaConsumer::~KafkaConsumer() {
  delete m_config;
  delete m_topicConfig;
  delete m_consumer;
  delete m_event_cb;
  delete m_rebalance_cb;
}

std::string KafkaConsumer::pullMessage() {
  RdKafka::Message* m_message = m_consumer->consume(5000);

  if (m_message == nullptr ||
      m_message->err() == RdKafka::ErrorCode::ERR_NO_ERROR ||
      m_message->payload() == nullptr) {
    return {};
  } else {
    return std::string(static_cast<const char*>(m_message->payload()));
  }
}
