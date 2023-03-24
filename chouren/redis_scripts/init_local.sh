#！/bin/bash



DIR=$(cd "$(dirname "$0")";pwd)
. $DIR/base_info.sh



USERNAME="redis"

#The redis password
AUTH="123456"

#The maxmeory of Redis Node
maxmemory=16M

#mkdir  data dir
if [ ! -d $DATA_DIR ]; then
        echo "create  data dir $DATA_DIR"
        mkdir -p $DATA_DIR
fi


#create user
if  not id -u ${USERNAME}  >/dev/null 2>&1 ; then
	echo "User: ${USERNAME} does not exist!"
	useradd ${USERNAME} 
fi




function init_config(){
	
	if [[ "$(sysctl -n vm.overcommit_memory)" -ne '1' ]]; then
	  echo "vm.overcommit_memory = 1" >> /etc/sysctl.conf
	fi
	if [[ "$(sysctl -n vm.swappiness)" -ne '0' ]]; then
	  echo "vm.swappiness = 0" >> /etc/sysctl.conf
	fi
	
        echo  "net.core.somaxconn = 2048" >>/etc/sysctl.conf	
	if [ -f /sys/kernel/mm/transparent_hugepage/enabled ]; then
	  echo never > /sys/kernel/mm/transparent_hugepage/enabled
	  if [[ -z "$(grep 'transparent_hugepage\/enabled' /etc/rc.local)" ]]; then
	    echo "echo never > /sys/kernel/mm/transparent_hugepage/enabled" >> /etc/rc.local
	  fi
	fi
	echo -e "\033[41;36m OK: initial config done. \033[0m"
	sysctl -p
	

}








#Create an instance directory with port
function init_node(){
        for ((i=0; i<NUM_OF_NODES; i++)); 
        do
                CUR_PORT=$((START_PORT + i))
                if [ ! -d "$DATA_DIR/$CUR_PORT" ];then
                        echo "create an instance directory with port: $CUR_PORT"
                        mkdir  -p  $DATA_DIR/$CUR_PORT
                fi

                if [ ! -f "$DATA_DIR/$CUR_PORT/redis.conf" ]; then
                        {
                                echo "bind $LOCAL_IP"
                                echo "daemonize yes" 
                                echo "port $CUR_PORT"
                                echo "dir $DATA_DIR/$CUR_PORT"
                                echo "cluster-enabled yes"
                                echo "cluster-node-timeout 5000"
                                echo "cluster-config-file nodes_$CUR_PORT.conf"
                                echo "pidfile $DATA_DIR/$CUR_PORT/redis_$CUR_PORT.pid"
                                echo "logfile $DATA_DIR/$CUR_PORT/redis_$CUR_PORT.log"
                                echo "loglevel notice"
                                echo "appendonly no"
                                echo "protected-mode no"
                                echo "requirepass $AUTH"
                                echo "masterauth $AUTH"
                                echo "maxmemory $maxmemory"
                        } >> $DATA_DIR/$CUR_PORT/redis.conf
                fi
done 
}
init_config
init_node 
chown -R ${USERNAME}:${USERNAME} $DATA_DIR
chown -R ${USERNAME}:${USERNAME} $REDIS_HOME
