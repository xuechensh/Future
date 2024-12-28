#!/bin/bash

# 创建docker 网络
docker network create --driver=bridge --subnet=172.19.0.0/16 --gateway=172.19.0.254 redis_cluster

# 创建节点
docker run -p 7001:6379 --name redis-cluster-master1 -v /root/data/Future/redis/cluster/master1.conf:/etc/redis/redis.conf --net redis_cluster --ip 172.19.0.1 -d redis/redis-stack-server redis-server /etc/redis/redis.conf

docker run -p 7002:6379 --name redis-cluster-master2 -v /root/data/Future/redis/cluster/master2.conf:/etc/redis/redis.conf --net redis_cluster --ip 172.19.0.2 -d redis/redis-stack-server redis-server /etc/redis/redis.conf

docker run -p 7003:6379 --name redis-cluster-master3 -v /root/data/Future/redis/cluster/master3.conf:/etc/redis/redis.conf --net redis_cluster --ip 172.19.0.3 -d redis/redis-stack-server redis-server /etc/redis/redis.conf

docker run -p 7004:6379 --name redis-cluster-slave1 -v /root/data/Future/redis/cluster/slave1.conf:/etc/redis/redis.conf --net redis_cluster --ip 172.19.0.4 -d redis/redis-stack-server redis-server /etc/redis/redis.conf

docker run -p 7005:6379 --name redis-cluster-slave2 -v /root/data/Future/redis/cluster/slave2.conf:/etc/redis/redis.conf --net redis_cluster --ip 172.19.0.5 -d redis/redis-stack-server redis-server /etc/redis/redis.conf

docker run -p 7006:6379 --name redis-cluster-slave3 -v /root/data/Future/redis/cluster/slave3.conf:/etc/redis/redis.conf --net redis_cluster --ip 172.19.0.6 -d redis/redis-stack-server redis-server /etc/redis/redis.conf

# 查看主从状态
# docker exec -it redis-cluster-master1 bash
# redis-cli -p 6379
# info Replication

# 进入任意一个主节点分配slots
# docker exec -it redis-cluster-master1 bash
# redis-cli --cluster create 172.19.0.1:6379 172.19.0.2:6379 172.19.0.3:6379 --cluster-replicas 0

# 验证集群
# docker exec -it redis-cluster-master1 bash
# redis-cli -c -p 6379
# cluster info
