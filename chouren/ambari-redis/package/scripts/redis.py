# coding=utf-8
"""
Description: 
Author: yangshangda
Date: 2022/1/6
"""
import codecs
import os
from resource_management import *


def setup(type=None):
    """
    通过tarball安装redis
    :param type:
    :return:
    """
    import params
    remove_cmd = 'rm -rf ' + params.redis_home
    tar_cmd = 'tar -zxf ' + os.path.join(params.redis_agent_home, 'package', params.redis_tar_file_name) + ' -C ' + params.install_root_dir
    Execute(tar_cmd, user=params.user)


# def replace_config(type=None):
#     """
#     更新redis服务配置
#     :param type:
#     :return:
#     """
#     import params
#     redis_conf_file = codecs.open(params.redis_home + "/etc/redis.conf", 'a', encoding='UTF-8')
#     if type == 'master':
#         redis_conf_file.write('bind ' + params.redis_master_address + '\n')
#         redis_conf_file.write('port ' + params.redis_listen_port + '\n')
#         if params.redis_isrequirepass == 'true':
#             redis_conf_file.write(params.redis_requirepass + '\n')
#
#     if type == 'slave':
#         if params.redis_model == 'master-slave':
#             redis_conf_file.write('bind ' + params.redis_slave_address + '\n')
#             redis_conf_file.write('port ' + params.redis_listen_port + '\n')
#             if params.redis_isrequirepass == 'true':
#                 redis_conf_file.write(params.redis_requirepass + '\n')
#             redis_conf_file.write('slaveof ' + params.redis_master_address + ' ' + params.redis_listen_port + ' \n')
#     redis_conf_file.close()

def setup_sentinel():
    import params
    remove_cmd = 'rm -rf ' + params.install_sentinel_dir
    tar_cmd = 'tar -zxf ' + os.path.join(params.redis_agent_home, 'package',
                                         params.redis_tar_file_name) + ' -C ' + params.install_sentinel_dir
    Execute(tar_cmd, user=params.user)


def setup_manager():
    import params
    tar_cmd = 'tar -zxf ' + os.path.join(params.redis_agent_home, 'package',
                                         params.redis_manager_file_name) + ' -C ' + params.install_manager_dir
    Execute(tar_cmd, user=params.user)

def start_metrics():
    """
    启动redis监控服务
    :return:
    """
    import params
    pid = os.popen("ps -ef | grep send_redis_metrics.sh | grep -v grep | awk '{print $2}'").read().strip()
    if pid == '':
        print('start redis metrics send ...')
        cmd = 'nohup sh ' + params.redis_agent_home + '/package/scripts/send_redis_metrics.sh ' + params.metric_host + \
              ' > /dev/null 2>&1 &'
        Execute(cmd)


def stop_metrics():
    """
    停止redis监控服务
    :return:
    """
    pid = os.popen("ps -ef | grep send_redis_metrics.sh | grep -v grep | awk '{print $2}'").read().strip()
    if pid != '':
        print('stop redis metrics ...')
        Execute("ps -ef | grep send_redis_metrics.sh | grep -v grep | awk '{print $2}' | xargs kill -9")
