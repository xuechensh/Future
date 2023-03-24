# coding=utf-8
"""
Author: yangshangda
Date: 2022/1/6
"""
import time

from resource_management import *
from redis import *
from changeconfig import change_config_master
import os,io


class RedisMaster(Script):
    """
    RedisMaster服务管理脚本
    """
    def install(self, env):
        """
        安装redis master服务
        """
        import params
        print('Installing redis master, please waiting ...')
        if os.path.exists(params.install_root_dir):
            rm = 'rm -rf ' + params.install_root_dir
            Execute(rm, user=params.user)
        if not os.path.exists(params.install_root_dir):
            Directory(
		        create_parents=True,
                name=params.install_root_dir,
                owner=params.user,
                group=params.user_group,
            )
        setup('master')
        print('Redis master installed ok.')

    def stop(self, env, upgrade_type=None):
        """
        停止redis master服务
        """
        import params
        cmd = 'sh ' + os.path.join(params.redis_home, 'bin/stop.sh')
        Execute(cmd, user=params.user)
        time.sleep(3)
        stop_metrics()
        print('Redis master stopped!')

    def start(self, env, upgrade_type=None):
        """
        启动redis master服务
        """
        import params
        env.set_params(params)
        self.configure(env)
        cmd = 'sh ' + os.path.join(params.redis_home, 'bin/start.sh')
        Execute(cmd, user=params.user)
        # time.sleep(5)
        # start_metrics()
        print('Redis master started!')
        time.sleep(10)
        
        
        #inital  cluster 
        if params.cluster_enabled_flag is True:
            ip = os.popen("ifconfig -a | grep inet | grep -v 127.0.0.1 | grep -v inet6 | awk '{print $2}'").read().strip('\n')
            if ip == params.existing_host:
                cluster_init_log = os.path.join(params.redis_log_dir,'cluster_init.log')
                init_cluster_cmd = format("cd {params.redis_data_dir} && echo 'yes' | {params.redis_home}/bin/redis-cli -h {ip}  -p {params.redis_listen_port} -a {params.redis_requirepass} --cluster create {params.cluster_master_host_port_param} --no-auth-warning  > {cluster_init_log}")
                Execute(init_cluster_cmd, user=params.user)
                
                if os.path.exists(cluster_init_log):
                    with io.open(cluster_init_log, 'r+',  encoding='utf8') as f:
                        lines = f.readlines()
                        for line in lines:
                            if '[OK] All 16384 slots covered' in line:
                                print("集群初始化成功！")
                            if '[ERR]' in line:
                                print("集群初始化失败!")
                                                          
                        
                        
        else:
            print("没有启动模式，不能进行集群的初始化！")
            
        

    def status(self, env):
        """
        检测redis master服务状态
        :return:
        """
        import params
        status_file = os.path.join(params.redis_home, 'run/redis.pid')
        flag = 0
        if os.path.exists(status_file):
            f = open(status_file,'r')
            pid = f.read()
            pid = pid.strip()
            s = os.popen('ps -ef|grep ' + pid + ' |grep -v grep ').read().strip()
            if len(s) == 0:
                rm = 'rm '+ status_file
                Execute(rm, user=params.user)
        check_process_status(os.path.join(params.redis_home, 'run/redis.pid'))
        
        

    def configure(self, env, upgrade_type=None, config_dir=None):
        """
        修改redis master服务配置
        """
        change_config_master()
        
        
        
    def addslave(self, env):  
        import params
        # print(params.cluster_slave_host_list.split(';'))
        # cluster_slave_host_list_tmp = params.cluster_slave_host_list.split(';')
        ip = os.popen("ifconfig -a | grep inet | grep -v 127.0.0.1 | grep -v inet6 | awk '{print $2}'").read().strip('\n').encode('utf-8')
        if  params.cluster_slave_host_list !='none':
            if params.cluster_enabled_flag is True:
                    if ip == params.existing_host:
                        for index, slave_host  in enumerate(params.cluster_slave_host_list_tmp):
                            slave_host_tmp = slave_host.strip().encode("utf-8")
                            add_slave_cmd = format("cd {params.redis_data_dir} && {params.redis_home}/bin/redis-cli -h {ip}  -p {params.redis_listen_port} -a {params.redis_requirepass} --cluster add-node {slave_host_tmp}:{params.redis_listen_port} {params.existing_host}:{params.redis_listen_port} --cluster-slave  --no-auth-warning  >{params.slave_init_log}")

                            Execute(add_slave_cmd, user=params.user)
                            if os.path.exists(params.slave_init_log):
                                with io.open(params.slave_init_log, 'r+',  encoding='utf-8') as f:
                                    lines = f.readlines()
                                    for line in lines:
                                        if '[OK] New node added correctly' in line:
                                            print("节点:%s添加成功!" %slave_host_tmp)
                                        if '[ERR]' in line:
                                            print("节点:%s添加失败!" %slave_host_tmp)
                            time.sleep(30)




if __name__ == '__main__':
    RedisMaster().execute()
