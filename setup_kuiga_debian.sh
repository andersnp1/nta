#! /bin/sh

echo "deb http://ppa.launchpad.net/webupd8team/java/ubuntu xenial main" | tee /etc/apt/sources.list.d/webupd8team-java.list
echo "deb-src http://ppa.launchpad.net/webupd8team/java/ubuntu xenial main" | tee -a /etc/apt/sources.list.d/webupd8team-java.list
apt-key adv --keyserver hkp://keyserver.ubuntu.com:80 --recv-keys EEA14886
apt-get update
apt-get install oracle-java8-installer oracle-java8-set-default scala docker-ce nodejs -y
export SBT_OPTS="${SBT_OPTS} -Dsbt.jse.engineType=Node -Dsbt.jse.command=$(which node)"
systemctl enable docker
groupadd docker
usermod -aG docker $USER
systemctl start docker
