# MySql

## 环境准备

一主一从部署MySql，细节见：[install.sh](./install.sh)
[测试数据](https://github.com/datacharmer/test_db)
mysql -uroot -proot -t < /etc/mysql/test_db-master/employees.sql

## 常用命令

create database <数据库名>;
show databases;
drop database <数据库名>;
use <数据库名>;

show tables;

## 连接池

MySql Connector C++ 自带连接池，可以直接使用。
Python使用连接池

pip3 install cryptography
pip3 install DBUtils
pip3 install pymysql
