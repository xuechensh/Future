# coding=utf-8
"""
Author: yangshangda
Date: 2022/1/10
"""
import time

from resource_management import *
from redis import *
from changeconfig import *


class RedisSentinel(Script):
    """
    RedisSentinel服务管理脚本
    """

    def install(self, env):
        """
        安装redis sentinel服务
        """
        import params
        print('Installing redis slave, please waiting ...')
        if os.path.exists(params.install_sentinel_dir):
            rm = 'rm -rf ' + params.install_sentinel_dir
            Execute(rm, user=params.user)
        if not os.path.exists(params.install_sentinel_dir):
            Directory(
		        create_parents=True,
                name=params.install_sentinel_dir,
                owner=params.user,
                group=params.user_group,
            )
        setup_sentinel()
        print('Redis sentinel installed ok')

    def stop(self, env, upgrade_type=None):
        """
        停止redis sentinel服务
        """
        import params
        cmd = 'sh ' + os.path.join(params.redis_sentinel_home, 'bin/stop_sentinel.sh')
        Execute(cmd, user=params.user)
        time.sleep(3)
        stop_metrics()
        print('Redis sentinel stopped!')

    def start(self, env, upgrade_type=None):
        """
        启动redis sentinel服务
        """
        import params
        env.set_params(params)
        self.configure(env)
        cmd = 'sh ' + os.path.join(params.redis_sentinel_home, 'bin/start_sentinel.sh')
        Execute(cmd, user=params.user)
        time.sleep(5)
        start_metrics()
        print('Redis sentinel started!')

    def status(self, env):
        """
        查看redis sentinel服务状态
        :return:
        """
        import params
        status_file = os.path.join(params.redis_sentinel_home, 'run/sentinel.pid')
        flag = 0
        if os.path.exists(status_file):
            f = open(status_file,'r')
            pid = f.read().strip()
            s = os.popen('ps -ef|grep ' + pid + ' |grep -v grep ').read().strip()
            if len(s) == 0:
                rm = 'rm '+ status_file
                Execute(rm, user=params.user)
        check_process_status(os.path.join(params.redis_sentinel_home, 'run/sentinel.pid'))

    def configure(self, env, upgrade_type=None, config_dir=None):
        """
        修改redis sentinel服务配置
        """
        change_config_sentinel()


if __name__ == '__main__':
    RedisSentinel().execute()
