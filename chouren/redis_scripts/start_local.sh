#!/bin/bash
 
DIR=$(cd "$(dirname "$0")";pwd)
. $DIR/base_info.sh


REDIS_SER="$REDIS_HOME/src/redis-server"

for ((i=0; i<NUM_OF_NODES; i++))
do
        CUR_PORT=$((START_PORT + i))
        echo "start $LOCAL_IP:$CUR_PORT"
        $REDIS_SER  $DATA_DIR/$CUR_PORT/redis.conf
done
