#! /bin/sh

curl -sSL https://get.docker.com | sh
apt-get update
apt-get install oracle-java8-installer oracle-java8-set-default scala docker-ce nodejs -y
export SBT_OPTS="${SBT_OPTS} -Dsbt.jse.engineType=Node -Dsbt.jse.command=$(which nodejs)"
systemctl enable docker
groupadd docker
usermod -aG docker $USER
systemctl start docker
docker pull jsurf/rpi-raspbian
