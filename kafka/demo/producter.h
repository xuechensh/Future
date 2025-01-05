
#include <librdkafka/rdkafkacpp.h>

#include <string>

class KafkaProducer {
 public:
  explicit KafkaProducer(const std::string& brokers, const std::string& topic);
  ~KafkaProducer();

  void pushMessage(const std::string& msg, const std::string& key);

 protected:
  std::string m_brokers;
  std::string m_topicStr;

  RdKafka::Conf*
      m_producerConfig;  // 配置接口类，用来设置对生产者、消费者、broker的各项配置值
  RdKafka::Conf* m_topicConfig;

  RdKafka::Producer* m_producer;
  RdKafka::Topic* m_topic;

  RdKafka::DeliveryReportCb*
      m_dr_cb;  // RdKafka::DeliveryReportCb 用于在调用
                // RdKafka::Producer::produce()
                // 后返回发送结果，RdKafka::DeliveryReportCb是一个类，需要自行填充其中的回调函数及处理返回结果的方式
  RdKafka::EventCb*
      m_event_cb;  // RdKafka::EventCb
                   // 用于从librdkafka向应用程序传递errors,statistics,logs
                   // 等信息的通用接口
  RdKafka::PartitionerCb*
      m_partitioner_cb;  // Rdkafka::PartitionerCb 用于设定自定义分区器
};
