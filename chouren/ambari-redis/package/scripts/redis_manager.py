# coding=utf-8
"""
Author: yangshangda
Date: 2022/1/10
"""
import time

from resource_management import *
from redis import *
from changeconfig import *


class RedisManager(Script):
    """
    RedisSlave服务管理脚本
    """

    def install(self, env):
        """
        安装redis slave服务
        """
        import params
        print('Installing redis manager, please waiting ...')
        if os.path.exists(params.install_manager_dir):
            rm = 'rm -rf ' + params.install_manager_dir
            Execute(rm, user=params.user)
        if not os.path.exists(params.install_manager_dir):
            Directory(
		        create_parents=True,
                name=params.install_manager_dir,
                owner=params.user,
                group=params.user_group,
            )
        setup_manager()
        print('Redis manager installed ok')

    def stop(self, env, upgrade_type=None):
        """
        停止redis manager服务
        """
        import params
        cmd = 'sh ' + os.path.join(params.redis_manager_home, 'bin/stop_manager.sh')
        Execute(cmd, user=params.user)
        time.sleep(3)
        stop_metrics()
        print('Redis manager stopped!')

    def start(self, env, upgrade_type=None):
        """
        启动redis manager服务
        """
        import params
        env.set_params(params)
        self.configure(env)
        cmd = 'sh ' + os.path.join(params.redis_manager_home, 'bin/start_manager.sh')
        Execute(cmd, user=params.user)
        time.sleep(5)
        start_metrics()
        print('Redis manager started!')

    def status(self, env):
        """
        查看redis manager服务状态
        :return:
        """
        import params
        status_file = os.path.join(params.redis_manager_home, 'run/manager.pid')
        flag = 0
        if os.path.exists(status_file):
            f = open(status_file,'r')
            pid = f.read()
            pid = pid.strip()
            s = os.popen('ps -ef|grep ' + pid + ' |grep -v grep ').read().strip()
            if len(s) == 0:
                rm = 'rm '+ status_file
                Execute(rm, user=params.user)
        check_process_status(os.path.join(params.redis_manager_home, 'run/manager.pid'))

    def configure(self, env, upgrade_type=None, config_dir=None):
        """
        修改redis slave服务配置
        """
        change_config_manager()

        import params
        XmlConfig("redis-public.xml",
            conf_dir = params.conf_file_dir,
            configurations = params.config['configurations']['redis-public'],
            configuration_attributes=params.config['configurationAttributes']['redis-public']
        )


if __name__ == '__main__':
    RedisManager().execute()
