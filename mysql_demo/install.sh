#! /bin/bash

# 拉取镜像
docker pull mysql:8.0.25

# 创建MySql主从网络
docker network create --driver=bridge --subnet=172.21.0.0/16 --gateway=172.21.0.254 mysql-net

# 创建主节点
docker run -p 3307:3306 --name mysql-master \
-v /docker_data/mysql-master/log:/var/log/mysql \
-v /docker_data/mysql-master/data:/var/lib/mysql \
-v /docker_data/mysql-master/conf:/etc/mysql \
-v /docker_data/mysql-master/mysql-files:/var/lib/mysql-files \
-e MYSQL_ROOT_PASSWORD=root \
--net mysql-net \
--ip 172.21.0.1 \
-d mysql:8.0.25

# 编辑conf并重启master
vim /docker_data/mysql-master/conf/my.cnf
----------------------------------------------------------------
[mysqld]

server_id=101

binlog-ignore-db=mysql

log-bin=mall-mysql-bin

binlog_cache_size=1M

binlog_format=mixed

binlog_expire_logs_seconds=604800

slave_skip_errors=1062
---------------------------------------------------------------
docker restart mysql-master

# 配置用户
docker exec -it mysql-master /bin/bash
mysql -uroot -proot
CREATE USER 'slave'@'%' IDENTIFIED BY '123456';
ALTER USER 'slave'@'%' IDENTIFIED WITH mysql_native_password BY '123456';
GRANT REPLICATION SLAVE,REPLICATION CLIENT ON *.* TO 'slave'@'%';

# 配置从机
docker run -p 3308:3306 --name mysql-slave01 \
-v /docker_data/mysql-slave01/log:/var/log/mysql \
-v /docker_data/mysql-slave01/data:/var/lib/mysql \
-v /docker_data/mysql-slave01/conf:/etc/mysql \
-v /docker_data/mysql-slave01/mysql-files:/var/lib/mysql-files \
-e MYSQL_ROOT_PASSWORD=root \
--net mysql-net \
--ip 172.21.1.1 \
-d mysql:8.0.25

# 编辑conf并重启slave
vim /docker_data/mysql-slave01/conf/my.cnf
----------------------------------------------------------------
[mysqld]

server_id=102

binlog-ignore-db=mysql

log-bin=mall-mysql-slave1-bin

binlog_cache_size=1M

binlog_format=mixed

binlog_expire_logs_seconds=604800

slave_skip_errors=1062

relay_log=mall-mysql-relay-bin

log_slave_updates=1

read_only=1
---------------------------------------------------------------
docker restart mysql-slave01

# 查看主节点状态
mysql> show master status;
+-----------------------+----------+--------------+------------------+-------------------+
| File                  | Position | Binlog_Do_DB | Binlog_Ignore_DB | Executed_Gtid_Set |
+-----------------------+----------+--------------+------------------+-------------------+
| mall-mysql-bin.000002 |      996 |              | mysql            |                   |
+-----------------------+----------+--------------+------------------+-------------------+
1 row in set (0.01 sec)

# 进入从节点，配置从节点
change master to master_host=' 172.21.0.1', master_user='slave', master_password='123456', master_port=33076, master_log_file='mall-mysql-bin.000002', master_log_pos=996, master_connect_retry=30;
show slave status \G;

# 进入从节点，开启主从复制
start slave;
show slave status \G;
