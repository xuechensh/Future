#include <iostream>
#include <string>

#include "consumer.h"

int main() {
  std::string brokers = "127.0.0.1:9092";

  std::vector<std::string> topics;  // 待消费主题的集合

  topics.emplace_back("demo");

  std::string group = "consumer-group-demo";  // 消费组

  KafkaConsumer consumer(brokers, group, topics,
                         RdKafka::Topic::OFFSET_BEGINNING);

  std::string msg_str;
  do {
    msg_str = consumer.pullMessage();
    std::cout << msg_str << std::endl;
  } while (!msg_str.empty());

  RdKafka::wait_destroyed(5000);

  return 0;
}
