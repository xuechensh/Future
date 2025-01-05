#include "producter.h"

#include <iostream>

class ProducerDeliveryReportCb : public RdKafka::DeliveryReportCb {
 public:
  void dr_cb(RdKafka::Message& message) {
    if (message.err() != 0) {  // 发送出错
      std::cerr << "Message delivery failed: " << message.errstr() << std::endl;
    } else {  // 发送成功
      std::cerr << "Message delivered to topic: " << message.topic_name()
                << " [" << message.partition() << "] at offset "
                << message.offset() << std::endl;
    }
  }
};

class ProducerEventCb : public RdKafka::EventCb {
 public:
  void event_cb(RdKafka::Event& event) {
    switch (event.type()) {
      case RdKafka::Event::EVENT_ERROR:
        std::cout << "RdKafka::EVENT::EVENT_ERROR: "
                  << RdKafka::err2str(event.err()) << std::endl;
        break;
      case RdKafka::Event::EVENT_STATS:
        std::cout << "RdKafka::EVENT::EVENT_STATS: " << event.str()
                  << std::endl;
        break;
      case RdKafka::Event::EVENT_LOG:
        std::cout << "RdKafka::EVENT::EVENT_LOG: " << event.fac() << std::endl;
        break;
      case RdKafka::Event::EVENT_THROTTLE:
        std::cout << "RdKafka::EVENT::EVENT_THROTTLE: " << event.broker_name()
                  << std::endl;
        break;
    }
  }
};

class HashPartitionerCb : public RdKafka::PartitionerCb {
  // 自定义生产者分区器，作用就是返回一个分区id。
  // 对key计算Hash值，得到待发送的分区号（其实这跟默认的分区器计算方式是一样的）
 public:
  int32_t partitioner_cb(const RdKafka::Topic* topic, const std::string* key,
                         int32_t partition_cnt, void* msg_opaque) {
    // char msg[128] = {0};
    // sprintf(msg, "HashPartitionCb:[%s][%s][%d]", topic->name().c_str(),
    //         key->c_str(), partition_cnt);
    // std::cout << msg << std::endl;

    // 前面的操作只是为了在分区器回调中打印出一行打印
    // 分区器真正的操作是在下面generate_hash，生成一个待发送的分区ID
    return generate_hash(key->c_str(), key->size()) % partition_cnt;
  }

 private:
  static inline unsigned int generate_hash(const char* str, size_t len) {
    unsigned int hash = 5381;
    for (size_t i = 0; i < len; i++) {
      hash = ((hash << 5) + hash) + str[i];
    }
    // 返回值必须在0到partition_cnt之间。如果出错则发回PARTITION_UA（-1）
    return hash;
  }
};

KafkaProducer::KafkaProducer(const std::string& brokers,
                             const std::string& topic) {
  m_brokers = brokers;
  m_topicStr = topic;

  // 先填充构造生产者客户端的参数配置：
  m_producerConfig = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
  if (m_producerConfig == nullptr) {
    std::cout << "Create Rdkafka Global Conf Failed." << std::endl;
  }

  // 下面开始配置各种需要的配置项：
  RdKafka::Conf::ConfResult result;
  std::string error_str;
  // 设置生产者待发送服务器的地址: "ip:port" 格式
  result = m_producerConfig->set("bootstrap.servers", m_brokers, error_str);
  if (result != RdKafka::Conf::CONF_OK) {
    std::cout << "Global Conf set 'bootstrap.servers' failed: " << error_str
              << std::endl;
  }

  result = m_producerConfig->set("statistics.interval.ms", "10000", error_str);
  if (result != RdKafka::Conf::CONF_OK) {
    std::cout << "Global Conf set statistics.interval.ms failed: " << error_str
              << std::endl;
  }

  // 设置发送端发送的最大字节数，如果发送的消息过大则返回失败
  result = m_producerConfig->set("message.max.bytes", "10240000", error_str);
  if (result != RdKafka::Conf::CONF_OK) {
    std::cout << "Global Conf set 'message.max.bytes' failed: " << error_str
              << std::endl;
  }

  // 设置每个消息发送后的发送结果回调
  m_dr_cb = new ProducerDeliveryReportCb;
  result = m_producerConfig->set("dr_cb", m_dr_cb, error_str);
  if (result != RdKafka::Conf::CONF_OK) {
    std::cout << "Global Conf set dr_cb failed: " << error_str << std::endl;
  }

  // 设置ProducterEvent回调
  m_event_cb = new ProducerEventCb;
  result = m_producerConfig->set("event_cb", m_event_cb, error_str);
  if (result != RdKafka::Conf::CONF_OK) {
    std::cout << "Global Conf set event_cb failed: " << error_str << std::endl;
  }

  // 创建Producer生产者客户端
  m_producer = RdKafka::Producer::create(m_producerConfig, error_str);
  if (m_producer == nullptr) {
    std::cout << "Create Producer failed: " << error_str << std::endl;
  }

  // 创建Topic参数
  m_topicConfig = RdKafka::Conf::create(RdKafka::Conf::CONF_TOPIC);
  if (m_topicConfig == nullptr) {
    std::cout << "Create Rdkafka Topic Conf Failed." << std::endl;
  }

  // 设置自定义分区器
  m_partitioner_cb = new HashPartitionerCb;
  result = m_topicConfig->set("partitioner_cb", m_partitioner_cb, error_str);
  if (result != RdKafka::Conf::CONF_OK) {
    std::cout << "Topic Conf set partitioner_cb failed: " << error_str
              << std::endl;
  }

  // 创建Topic对象，后续produce发送消息时需要使用
  m_topic =
      RdKafka::Topic::create(m_producer, m_topicStr, m_topicConfig, error_str);
  if (m_topic == nullptr) {
    std::cout << "Create Topic failed: " << error_str << std::endl;
  }
}

void KafkaProducer::pushMessage(const std::string& msg,
                                const std::string& key) {
  int32_t len = msg.length();
  void* payload = const_cast<void*>(static_cast<const void*>(msg.data()));

  RdKafka::ErrorCode error_code = m_producer->produce(
      m_topic, RdKafka::Topic::PARTITION_UA, RdKafka::Producer::RK_MSG_COPY,
      payload, len, &key, NULL);

  // poll()参数为0意味着不阻塞；poll(0)主要是为了触发应用程序提供的回调函数
  m_producer->poll(0);

  if (error_code != RdKafka::ErrorCode::ERR_NO_ERROR) {
    std::cerr << "Produce failed: " << RdKafka::err2str(error_code)
              << std::endl;
    if (error_code == RdKafka::ErrorCode::ERR__QUEUE_FULL) {
      // 如果发送失败的原因是队列正满，则阻塞等待一段时间
      m_producer->poll(1000);
    } else if (error_code == RdKafka::ErrorCode::ERR_MSG_SIZE_TOO_LARGE) {
      // 如果发送消息过大，超过了max.size，则需要裁减后重新发送
    } else {
      std::cerr << "ERR_UNKNOWN_PARTITION or ERR_UNKNOWN_TOPIC" << std::endl;
    }
  }
}

KafkaProducer::~KafkaProducer() {
  while (m_producer->outq_len() > 0) {
    // 当 Handle->outq_len() 客户端的“出队列” 的长度大于0
    std::cerr << "Waiting for: " << m_producer->outq_len() << std::endl;
    m_producer->flush(5000);
  }

  delete m_producerConfig;
  delete m_topicConfig;
  delete m_topic;
  delete m_producer;
  delete m_dr_cb;
  delete m_event_cb;
  delete m_partitioner_cb;
}
