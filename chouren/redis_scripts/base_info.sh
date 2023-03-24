#!/bin/bash

#The absolute path of the directory where the shell script is located
DIR=$(cd "$(dirname "$0")";pwd)

#ssh port
SSH_PORT=22
# Get IP Address of the  local server
LOCAL_IP=`LC_ALL=C ifconfig -a|grep inet|grep -v 127.0.0.1|grep -v inet6|awk '{print $2}'|tr -d "addr:"`

#The redis binary home directory
REDIS_HOME="/usr/local/redis-5.0.14"

#The redis software dir
REDIS_SRC_DIR=/home/redis/redis-5.0.14

#config file ,pid file, logs,dump file
DATA_DIR=/data12/redis-cluster/data

#start port and Number of nodes to create. 
START_PORT=6000
NUM_OF_NODES=2

#host file
HOST_FILE=$DIR/hosts.txt

#The redis password
AUTH="123456"

#ssh 
SSH="ssh -p $SSH_PORT"
SCP="scp -P $SSH_PORT"
