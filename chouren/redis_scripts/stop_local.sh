#!/bin/bash
 
DIR=$(cd "$(dirname "$0")";pwd)
. $DIR/base_info.sh



for pid in `ps -ef | grep redis-5.0.14 | awk '{print $2}'`
do	echo "start stop redis sever!"
	kill $pid
done
