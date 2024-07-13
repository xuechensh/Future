# 0、环境准备
yum install -y vim net-tools.x86_64

# 1、指定主机名
hostnamectl set-hostname k8s-master
hostnamectl set-hostname k8s-node01
hostnamectl set-hostname k8s-node02

# 2、配置名称和IP的绑定关系
vim /etc/hosts
--add at end--
192.168.56.101 k8s-master
192.168.56.102 k8s-node01
192.168.56.103 k8s-node02
--end--

# 3、安装依赖包
yum install -y conntrack ntpdate ntp ipvsadm ipset jq iptables curl sysstat libseccomp wget git

# 4、关闭swap分区
swapoff -a && sed -i '/ swap / s/^\(.*\)$/#\1/g' /etc/fstab
free -m

# 5、关闭内核参数
modprobe br_netfilter && echo "modprobe br_netfilter" >> /etc/profile && echo -e "net.bridge.bridge-nf-call-ip6tables = 1\nnet.bridge.bridge-nf-call-iptables = 1\nnet.ipv4.ip_forward = 1\n" > /etc/sysctl.d/k8s.conf && sysctl -p /etc/sysctl.d/k8s.conf

cat > /etc/sysconfig/modules/ipvs.modules <<EOF
#!/bin/bash
modprobe -- ip_vs
modprobe -- ip_vs_rr
modprobe -- ip_vs_wrr
modprobe -- ip_vs_sh
modprobe -- nf_conntrack
EOF
chmod 755 /etc/sysconfig/modules/ipvs.modules && bash /etc/sysconfig/modules/ipvs.modules && lsmod | grep ip_vs

# 6、关闭不需要的服务
systemctl stop postfix && systemctl disable postfix

# 7、设置日志方案
mkdir /var/log/journal
mkdir /etc/systemd/journald.conf.d
cat > /etc/systemd/journald.conf.d/99-prophet.conf <<EOF
[Journal]
# 持久化保存到磁盘
Storage=persistent
# 压缩历史日志
Compress=true
SyncIntervalSec=5m
RateLimitInterval=30s
RateLimitBurst=1000
# 最大占用空间10G
SystemMaxUse=10G
# 单日志最大200M
SystemMaxFileSize=200M
# 单日志保存时间2周
MaxRetentionSec=2week
# 不将日志转发到syslog
ForwardToSyslog=no
EOF

systemctl restart systemd-journald

# 8、升级系统内核到4.44
rpm --import https://www.elrepo.org/RPM-GPG-KEY-elrepo.org
rpm -Uvh http://www.elrepo.org/elrepo-release-7.0-2.el7.elrepo.noarch.rpm
yum --enablerepo="elrepo-kernel" list --showduplicates | sort -r | grep kernel-ml.x86_64
yum --enablerepo=elrepo-kernel install kernel-lt-devel kernel-lt -y
# 查看内核启动顺序
awk -F\' '$1=="menuentry " {print $2}' /etc/grub2.cfg
grub2-set-default 0
reboot
uname -r

# 9、安装docker
yum install -y yum-utils device-mapper-persistent-data lvm2
yum-config-manager \
  --add-repo \
  http://mirrors.aliyun.com/docker-ce/linux/centos/docker-ce.repo
yum update -y && yum install -y docker-ce
# 需要重新设置内核版本。。
mkdir /etc/docker
cat > /etc/docker/daemon.json <<EOF
 
{
 
  "exec-opts": ["native.cgroupdriver=systemd"],
 
  "registry-mirrors": ["https://kn0t2bca.mirror.aliyuncs.com"]
,
  "log-driver": "json-file",
  "log-opts": {
      "max-size": "100m"
  }
}
 
EOF
# 存放docker配置文件
mkdir -p /etc/systemd/system/docker.service.d
systemctl daemon-reload
systemctl restart docker
systemctl enable docker 
​