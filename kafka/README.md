# Kafka

## Kafka Kraft模式部署

1. 使用Raft协议部署Kafka集群不需要ZK。
2. 参考：https://juejin.cn/post/7380421216019365897
3. 参考：https://juejin.cn/post/7187301063832109112

## C++ librdkafka编译

### 源码安装

1. wget https://github.com/confluentinc/librdkafka/archive/refs/tags/v2.6.1.tar.gz
2. tar -zxvf librdkafka-2.6.1.tar.gz
3. to do

### Yum安装

1. yum install -y librdkafka-devel

## 发送方消息必达

1. spring.kafka.producer.acks=1
   - acks = 0：生产者将不会等待来自服务器的任何确认，该记录将立即添加到套接字缓冲区并视为已发送。在这种情况下，无法保证服务器已收到记录，并且重试配置将不会生效（因为客户端通常不会知道任何故障），为每条记录返回的偏移量始终设置为-1。
   - acks = 1：这意味着leader会将记录写入其本地日志，但无需等待所有副本服务器的完全确认即可做出回应，在这种情况下，如果leader在确认记录后立即失败，但在将数据复制到所有的副本服务器之前，则记录将会丢失。
   - acks = all：这意味着leader将等待完整的同步副本集以确认记录，这保证了只要至少一个同步副本服务器仍然存活，记录就不会丢失，这是最强有力的保证，这相当于acks = -1的设置。
2. 
