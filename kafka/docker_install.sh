#!/bin/sh

# docker安装
# docker pull registry.dockermirror.com/apache/kafka:3.9.0

docker network create --driver=bridge --subnet=172.20.0.0/16 --gateway=172.20.0.254 kafka_cluster

# 节点1-Controller
docker run -id --name=kafka1 \
--net kafka_cluster \
--ip 172.20.0.1 \
-p 10001:9093 \
-e LANG=C.UTF-8 \
-e KAFKA_INTER_BROKER_LISTENER_NAME=PLAINTEXT \
-e KAFKA_CONTROLLER_LISTENER_NAMES=CONTROLLER \
-e CLUSTER_ID=the-custom-id \
-e KAFKA_NODE_ID=1 \
-e KAFKA_PROCESS_ROLES=controller \
-e KAFKA_CONTROLLER_QUORUM_VOTERS="1@172.20.0.1:9093" \
-e KAFKA_LISTENERS="CONTROLLER://:9093" \
registry.dockermirror.com/apache/kafka:3.9.0

# 节点2-Broker
docker run -id --name=kafka2 \
--net kafka_cluster \
--ip 172.20.0.2 \
-p 9002:9092 \
-e LANG=C.UTF-8 \
-e KAFKA_INTER_BROKER_LISTENER_NAME=PLAINTEXT \
-e KAFKA_CONTROLLER_LISTENER_NAMES=CONTROLLER \
-e CLUSTER_ID=the-custom-id \
-e KAFKA_NODE_ID=2 \
-e KAFKA_PROCESS_ROLES=broker \
-e KAFKA_CONTROLLER_QUORUM_VOTERS="1@172.20.0.1:9093" \
-e KAFKA_LISTENERS="PLAINTEXT://:9092" \
-e KAFKA_ADVERTISED_LISTENERS="PLAINTEXT://127.0.0.1:9002" \
registry.dockermirror.com/apache/kafka:3.9.0

# 节点3-Broker
docker run -id --name=kafka3 \
--net kafka_cluster \
--ip 172.20.0.3 \
-p 9003:9092 \
-e LANG=C.UTF-8 \
-e KAFKA_INTER_BROKER_LISTENER_NAME=PLAINTEXT \
-e KAFKA_CONTROLLER_LISTENER_NAMES=CONTROLLER \
-e CLUSTER_ID=the-custom-id \
-e KAFKA_NODE_ID=3 \
-e KAFKA_PROCESS_ROLES=broker \
-e KAFKA_CONTROLLER_QUORUM_VOTERS="1@172.20.0.1:9093" \
-e KAFKA_LISTENERS="PLAINTEXT://:9092" \
-e KAFKA_ADVERTISED_LISTENERS="PLAINTEXT://127.0.0.1:9003" \
registry.dockermirror.com/apache/kafka:3.9.0

# 验证
# ./kafka-topics.sh --bootstrap-server 127.0.0.1:9002,127.0.0.1:9003 --create --topic demo --partitions 2 --replication-factor 2
# ./kafka-topics.sh --bootstrap-server 127.0.0.1:9002,127.0.0.1:9003 --list
