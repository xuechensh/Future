# coding=utf-8
"""
Author: yangshangda
Date: 2022/1/11
"""

from resource_management import *
import os
import logging

STATE_DEAD = 'DEAD'
STATE_ALIVE = 'ALIVE'
RESULT_STATE_OK = 'OK'
RESULT_STATE_CRITICAL = 'CRITICAL'

logger = logging.getLogger('ambari_alerts')


def get_tokens():
    """
    Returns a tuple of tokens in the format {{site/property}} that will be used
    to build the dictionary passed into execute
    """
    return None


def execute(configurations={}, parameters={}, host_name=None):
    """
    此处只做示例，pid文件需要通过如下方式获取
    config = Script.get_config()
    pid_file = config['configurations']...
    """
    try:
        pid_file = '/opt/mdp/1.0.0.0-1/redis-5.0.4/run/redis.pid'
        if os.path.exists(pid_file):
            result_code = RESULT_STATE_OK
            redis_status = STATE_ALIVE
        else:
            result_code = RESULT_STATE_CRITICAL
            redis_status = STATE_DEAD

        alert_label = 'Redis status is {0}'
        alert_label = alert_label.format(redis_status)
        return result_code, [alert_label]
    except Exception, ex:
        logger.error('[Alert Redis] Redis server status cannot be determined. {0}'.format(host_name))
        logger.exception(str(ex))
