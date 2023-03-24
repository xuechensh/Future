#!/bin/bash


DIR=$(cd "$(dirname "$0")";pwd)

. $DIR/base_info.sh



for HOST in `cat $HOST_FILE`
do
        echo "ssh $HOST......"
        $SSH -n -T $HOST "$DIR/stop_local.sh"
done
