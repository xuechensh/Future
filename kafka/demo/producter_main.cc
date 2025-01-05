#include <unistd.h>

#include <iostream>

#include "producter.h"

int main() {
  KafkaProducer producer("127.0.0.1:9092", "demo");

  sleep(5);

  for (int i = 0; i < 10000; i++) {
    char msg[64] = {0};
    sprintf(msg, "%s%4d", "Hello Kafka ", i);  // msg = "Hello Kafka 0001";

    char key[8] = {0};
    sprintf(key, "%d", i);  // key = "1";

    producer.pushMessage(msg, key);
  }

  RdKafka::wait_destroyed(50000);  // 等待50s，然后结束进程

  return 0;
}
