# Env

启动脚本：bash docker-install.sh
删除容器：bash docker-rm.sh

部署一主二从三哨兵，要有先后顺序，先启动主节点，然后从节点，然后哨兵节点

为什么三个哨兵节点：
哨兵节点也需要备份，同时要做选举

# CodeDemo

npm install ioredis
node redis-sentinel.js

框架要连接哨兵节点，同时要指定主节点名称，同时要指定规则（master || slave），和分开连接类似，但不用指定不同IP。

ioredis也没有看门狗机制，加锁逻辑（NX PX） -- 加锁成功启动看门狗（独立进程或者定期任务调用续期接口，需要判断value值是否是自己的，是自己的才能延期，判断逻辑用lua语言） -- 解锁逻辑（判断value是否是自己的，是才删除锁，解锁判断逻辑也用lua）。
