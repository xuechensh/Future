#!/bin/sh

declare -A PID_MAP
declare -A METRICS_MAP
declare -A APPID_MAP

APPID_MAP["redis"]="redis"

url=http://$1:6188/ws/v1/timeline/metrics
hostname=localhost

while [ 1 ]
do
    PID_MAP["redis"]=`ps -ef | grep redis-5.0.4 | grep -v grep | awk '{print $2}'`

    millon_time=$(( $(date +%s%N) / 1000000 ))
    for k in ${!PID_MAP[*]}
    do
        if [ -n "${PID_MAP[$k]}" ]
        then
            memStr=`ps aux |grep -v 'grep'|grep ${PID_MAP[$k]}|awk '{print $6}'`
            METRICS_MAP["cpu"]=`ps aux |grep -v 'grep'|grep ${PID_MAP[$k]}|awk '{print $3}'`
            METRICS_MAP["mem"]=`expr $((memStr)) / 1024`
            for m in ${!METRICS_MAP[*]}
            do
            json="{
                 \"metrics\": [
                 {
                 \"metricname\": \"redis.$hostname.$m\",
                 \"appid\": \"${APPID_MAP[$k]}\",
                 \"hostname\": \"localhost\",
                 \"timestamp\": ${millon_time},
                 \"starttime\": ${millon_time},
                 \"metrics\": {
                 \"${millon_time}\": ${METRICS_MAP[$m]}
                 }
                 }
                 ]
                }"
                echo $json
                curl -i -X POST -H "Content-Type: application/json" -d "${json}" ${url}
            done
        fi
    done
    sleep 30s
done
