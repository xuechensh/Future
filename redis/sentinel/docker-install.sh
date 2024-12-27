#!/bin/bash

# 创建docker network
docker network create --driver=bridge --subnet=172.18.0.0/16 redis_sentinel

# 需要修改文件的路劲
docker run -p 6379:6379 --name redis-master -v /root/data/Future/redis/sentinel/redis-master.conf:/etc/redis/redis.conf --net redis_sentinel --ip 172.18.0.2 -d redis/redis-stack-server redis-server /etc/redis/redis.conf

# 查看docker redis-master地址
docker inspect redis-master | grep IPAddress

# 启动slave
docker run -p 6380:6379 --name redis-slave1 -v /root/data/Future/redis/sentinel/redis-slave1.conf:/etc/redis/redis.conf --net redis_sentinel --ip 172.18.0.3 -d redis/redis-stack-server redis-server /etc/redis/redis.conf

docker run -p 6381:6379 --name redis-slave2 -v /root/data/Future/redis/sentinel/redis-slave2.conf:/etc/redis/redis.conf --net redis_sentinel --ip 172.18.0.4 -d redis/redis-stack-server redis-server /etc/redis/redis.conf

# 查看主从配置
# docker exec -it redis-master bash
# redis-cli -p 6379
# info Replication

# 配置哨兵
docker run -p 26379:26379 --name redis-sentinel1 -v /root/data/Future/redis/sentinel/sentinel1.conf:/etc/redis/sentinel.conf --net redis_sentinel --ip 172.18.0.5 -d redis/redis-stack-server redis-sentinel /etc/redis/sentinel.conf

docker run -p 26380:26379 --name redis-sentinel2 -v /root/data/Future/redis/sentinel/sentinel2.conf:/etc/redis/sentinel.conf --net redis_sentinel --ip 172.18.0.6 -d redis/redis-stack-server redis-sentinel /etc/redis/sentinel.conf

docker run -p 26381:26379 --name redis-sentinel3 -v /root/data/Future/redis/sentinel/sentinel3.conf:/etc/redis/sentinel.conf --net redis_sentinel --ip 172.18.0.7 -d redis/redis-stack-server redis-sentinel /etc/redis/sentinel.conf

# 查看哨兵状态
# docker exec -it redis-sentinel1 bash
# redis-cli -p 26379
# info sentinel
