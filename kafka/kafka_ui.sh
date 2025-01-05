#!bin/bash

docker pull hub.geekery.cn/provectuslabs/kafka-ui
docker pull hub.littlediary.cn/provectuslabs/kafka-ui
docker pull docker.rainbond.cc/provectuslabs/kafka-ui
docker pull docker.unsee.tech/provectuslabs/kafka-ui
docker pull docker.m.daocloud.io/provectuslabs/kafka-ui
docker pull hub.crdz.gq/provectuslabs/kafka-ui
docker pull docker.nastool.de/provectuslabs/kafka-ui
docker pull hub.firefly.store/provectuslabs/kafka-ui
docker pull registry.dockermirror.com/provectuslabs/kafka-ui
docker pull docker.1panelproxy.com/provectuslabs/kafka-ui
docker pull hub.rat.dev/provectuslabs/kafka-ui
docker pull docker.udayun.com/provectuslabs/kafka-ui
docker pull docker.kejilion.pro/provectuslabs/kafka-ui
docker pull dhub.kubesre.xyz/provectuslabs/kafka-ui
docker pull docker.1panel.live/provectuslabs/kafka-ui
docker pull dockerpull.org/provectuslabs/kafka-ui
docker pull docker.hlmirror.com/provectuslabs/kafka-ui
docker pull docker.imgdb.de/provectuslabs/kafka-ui

docker run -it -p 8080:8080 -e DYNAMIC_CONFIG_ENABLED=true hub.littlediary.cn/provectuslabs/kafka-ui
