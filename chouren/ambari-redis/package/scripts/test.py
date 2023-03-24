# coding=utf-8
"""
Description: 
Author: yangshangda
Date: 2022/1/6
"""

import os

test_str = 'false'

if test_str:
    print 'true'


print os.path.join('/var/lib/ambari-agent/cache/stacks/MDP/1.0/services/REDIS', 'package/scripts/send_redis_metrics.sh')
