# Env
**在docker-compose.yml文件所在的目录下，运行以下命令启动Redis主从节点和多个哨兵**
启动脚本在 docker-install.sh 中，不能直接运行，需要解析redis-master的ip
bash docker-rm.sh

# CodeDemo
npm install ioredis
node redis-sentinel.js
