# coding=utf-8
"""
Description: 
Author: yangshangda
Date: 2022/1/6
"""

from re import T
from resource_management import *
import os

config = Script.get_config()
user = 'root'
user_group = 'root'
redis_version = 'redis-5.0.4'
redis_tar_file_name = redis_version + '.tar.gz'
redis_manager_file_name = 'redis-manager-2.3.2.2-RELEASE.tar'
redis_agent_home = '/var/lib/ambari-agent/cache/stacks/HDP/3.1/services/REDIS'
install_root_dir = '/opt/mdp/1.0.0.0-1/redis'
install_sentinel_dir = '/opt/mdp/1.0.0.0-1/sentinel'
install_manager_dir = '/opt/mdp/1.0.0.0-1/manager'
redis_home = os.path.join(install_root_dir, redis_version)
redis_log_dir = os.path.join(redis_home,'logs')
conf_file_dir = os.path.join(install_root_dir,redis_version,'etc')
redis_pid_file = os.path.join(install_root_dir,redis_version,'run/redis.pid')
redis_data_dir = os.path.join(install_root_dir,redis_version,'data')






# agentLevelParams
local_host_name = config['agentLevelParams']['hostname']


#redis public config
redis_listen_port = config['configurations']['redis']['redis.listen.port']
sentinel_listen_port = config['configurations']['sentinel']['sentinel.listen.port']
redis_requirepass = config['configurations']['redis']['redis.requirepass']
redis_master_auth = redis_requirepass


# redis master-slave config
redis_master_address = config['configurations']['redis']['redis.master.address']
redis_slave_address = config['configurations']['redis']['redis.slave.address']
# redis_slave_listen_port =  int(redis_listen_port) + 1

#redis sentinel
cluster_name = config['configurations']['sentinel']['sentinel.cluster.name']


# #REDIS PUBLIC Parameter
daemonize = config['configurations']['redis-public']['daemonize']
if daemonize is True:
    daemonize = 'yes'
else:
    daemonize = 'no'

timeout = config['configurations']['redis-public']['timeout']
maxclients = config['configurations']['redis-public']['maxclients']
maxmemory = config['configurations']['redis-public']['maxmemory']
maxmemoryPolicy = config['configurations']['redis-public']['maxmemory-policy']
databases = config['configurations']['redis-public']['databases']
replBacklogSize = config['configurations']['redis-public']['repl-backlog-size']
redis_cnf = config['configurations']['redis-public']['content']

rdb_enabled = config['configurations']['redis-public']['rdb-enabled']
if rdb_enabled is True:
    rdb_config = config['configurations']['redis-public']['rdb-config']
else:
    rdb_config = " "

aof_enabled =  config['configurations']['redis-public']['appendonly']
if aof_enabled is True:
    aof_enabled = 'yes'
else:
    aof_enabled = 'no'

dir = config['configurations']['redis-public']['redis_data_dir']
if dir == 'none':
    dir = redis_data_dir
else:
    dir = config['configurations']['redis-public']['redis_data_dir']

is_replDiskleySnc = config['configurations']['redis-public']['repl-diskless-sync']
if is_replDiskleySnc is True:
    is_replDiskleySnc = 'yes'
else:
    is_replDiskleySnc = 'no'
    
    
    
#Redis Cluster
cluster_enabled = config['configurations']['redis-cluster']['cluster-enabled']
cluster_master_host_list = config['configurations']['redis-cluster']['cluster-master-host-list']
cluster_slave_host_list = config['configurations']['redis-cluster']['cluster-slave-host-list']
cluster_slave_host_list_tmp = cluster_slave_host_list.split(';')
slave_init_log = os.path.join(redis_log_dir,'cluster_init_slave.log')

if cluster_master_host_list != 'none':
    cluster_master_host_port_param = ''
    cluster_master_host_list_tmp = cluster_master_host_list.split(';')
    for i,cluster_master_host in enumerate(cluster_master_host_list_tmp):
        
        if i == len(cluster_master_host_list_tmp) - 1:
            cluster_master_host_port_param = cluster_master_host_port_param + cluster_master_host.strip() + ':' + str(redis_listen_port)
        else :
            cluster_master_host_port_param = cluster_master_host_port_param + cluster_master_host.strip() + ':' + str(redis_listen_port) + ' '
        print(cluster_master_host_port_param)
        if i == 0:
           existing_host = cluster_master_host.encode("utf-8")
           
           
           
           

           
if cluster_enabled is True:
    cluster_enabled_flag = True
    cluster_enabled = 'cluster-enabled yes'
    cluster_node_timeout  = 'cluster-node-timeout ' + config['configurations']['redis-cluster']['cluster-node-timeout']
    cluster_require_full_coverage = 'cluster-require-full-coverage ' +  str(config['configurations']['redis-cluster']['cluster-require-full-coverage'] )  
    cluster_migration_barrier = 'cluster-migration-barrier ' +  config['configurations']['redis-cluster']['cluster-migration-barrier']
    if cluster_require_full_coverage is True:
        cluster_require_full_coverage = 'cluster-require-full-coverage yes'
    else:
        cluster_require_full_coverage = 'cluster-require-full-coverage no' 
else:
    cluster_enabled_flag = False
    cluster_enabled = 'cluster-enabled no'
    cluster_config_file = '# cluster-config-file nodes-6379.conf'
    cluster_node_timeout = '# cluster-node-timeout 15000'
    cluster_migration_barrier = '# cluster-migration-barrier 1'
    cluster_require_full_coverage = '# cluster-require-full-coverage yes'

    
    

    

        
                         
    


# collector host
metric_host = default('/clusterHostInfo/metrics_collector_hosts', ['localhost'])[0]

redis_sentinel_home = os.path.join(install_sentinel_dir, redis_version)
redis_manager_home = os.path.join(install_manager_dir, 'redis-manager')


# manager config
manager_port = config['configurations']['manager']['manager.port']
mysql_address = config['configurations']['manager']['mysql.address']
mysql_port = config['configurations']['manager']['mysql.port']
mysql_user = config['configurations']['manager']['mysql.user']
mysql_pass = config['configurations']['manager']['mysql.pass']
