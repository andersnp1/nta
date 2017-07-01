#! /bin/sh

dnf install java-1.8.0-openjdk scala docker docker-client nodejs -y
systemctl enable docker
sysyemctl start docker
groupadd docker
usermod -aG docker $USER