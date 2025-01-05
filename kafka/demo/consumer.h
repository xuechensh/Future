#include <librdkafka/rdkafkacpp.h>

#include <string>
#include <vector>

class KafkaConsumer {
 public:
  explicit KafkaConsumer(const std::string& brokers, const std::string& groupID,
                         const std::vector<std::string>& topics, int partition);
  ~KafkaConsumer();

  std::string pullMessage();

 protected:
  std::string m_brokers;
  std::string m_groupId;

  // 一个消费者可以同时订阅多个主题，所有用vector
  std::vector<std::string> m_topicVector;
  int m_partition;

  RdKafka::Conf* m_config;  // GLOBAL 级别的配置（Consumer客户端级别）
  RdKafka::Conf* m_topicConfig;  // TOPIC 级别的配置

  RdKafka::KafkaConsumer* m_consumer;  // 消费者客户端实例

  RdKafka::EventCb* m_event_cb;          // Event事件回调
  RdKafka::RebalanceCb* m_rebalance_cb;  // 再均衡 回调
};
