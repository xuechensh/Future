# coding=utf-8
"""
Description: 
Author: yangshangda
Date: 2022/1/6
"""
from resource_management import *
import os
import io


def change_config_master():
    """
    修改redis master服务配置
    :param env:
    :return:
    """
    import params
    Directory(
        params.redis_home,
        owner=params.user,
        group=params.user_group,
        mode=0755
    )
    # File(
    #     os.path.join(params.redis_home, 'etc/redis.conf'),
    #     content=Template('master.conf.j2'),
    #     owner=params.user,
    #     group=params.user_group,
    #     mode=0644
    # )

    #判断Redis默认数据目录是否存在
    if not os.path.exists(params.redis_data_dir):
        Directory(
            name=params.redis_data_dir,
            owner=params.user,
            group=params.user_group,
            create_parents=True
        )

    #判断Redis指定数据目录是否存在
    if not os.path.exists(params.dir):
        Directory(
            name=params.dir,
            owner=params.user,
            group=params.user_group,
            create_parents=True
        )

    #判断日志目录是否存在
    if not os.path.exists(params.redis_log_dir):
        Directory(
            name=params.redis_log_dir,
            owner=params.user,
            group=params.user_group,
            create_parents=True
        )
    
    redis_cnf = InlineTemplate(params.redis_cnf)
    File(format("{conf_file_dir}/redis.conf"),
        owner=params.user,
        group=params.user_group,
        content=redis_cnf)
    
    if os.path.exists(params.conf_file_dir + "/redis.conf"):
        if params.cluster_enabled_flag is True:
            with io.open(params.conf_file_dir + '/redis.conf', mode='a+', encoding='utf-8') as f:
                f.write( u'\n')
                f.write('masterauth '  + ' ' +  params.redis_master_auth + '\n')
        




def change_config_slave():
    """
    修改redis slave服务配置
    :param env:
    :return:
    """
    import params
    # File(
    #     format(params.redis_home + '/etc/redis.conf'),
    #     content=Template('slave.conf.j2'),
    #     owner=params.user,
    #     group=params.user_group
    # )

    #判断Redis默认数据目录是否存在
    if not os.path.exists(params.redis_data_dir):
        Directory(
            name=params.redis_data_dir,
            owner=params.user,
            group=params.user_group,
            create_parents=True
        )

    #判断Redis指定数据目录是否存在
    if not os.path.exists(params.dir):
        Directory(
            name=params.dir,
            owner=params.user,
            group=params.user_group,
            create_parents=True
        )

    #判断日志目录是否存在
    if not os.path.exists(params.redis_log_dir):
        Directory(
            name=params.redis_log_dir,
            owner=params.user,
            group=params.user_group,
            create_parents=True
        )

    redis_cnf = InlineTemplate(params.redis_cnf)
    File(format("{conf_file_dir}/redis.conf"),
        owner=params.user,
        group=params.user_group,
        content=redis_cnf)
    
    if os.path.exists(params.conf_file_dir + "/redis.conf"):
        if params.cluster_enabled_flag is False:
            with io.open(params.conf_file_dir + '/redis.conf', mode='a+', encoding='utf-8') as f:
                f.write( u'\n')
                f.write('replicaof' + ' ' + params.redis_master_address + ' ' + params.redis_listen_port + '\n')
                f.write('masterauth '  + ' ' +  params.redis_master_auth)
        if params.cluster_enabled_flag is True:
            with io.open(params.conf_file_dir + '/redis.conf', mode='a+', encoding='utf-8') as f:
                    f.write( u'\n')
                    f.write('masterauth '  + ' ' +  params.redis_master_auth + '\n')
                
                
            



def change_config_sentinel():
    """
    修改redis sentinel服务配置
    :param env:
    :return:
    """
    import params
    File(
        format(params.redis_sentinel_home + '/etc/sentinel.conf'),
        content=Template('sentinel.conf.j2'),
        owner=params.user,
        group=params.user_group
    )



def change_config_manager():
    """
    修改redis manager 服务配置
    :param env:
    :return:
    """
    import params
    File(
        format(params.redis_manager_home + '/conf/application.yml'),
        content=Template('manager.yml.j2'),
        owner=params.user,
        group=params.user_group
    )
