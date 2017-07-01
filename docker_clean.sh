#! /bin/sh

echo "Stopping all docker containers"
docker stop $(docker ps -a -q)
echo "Removing all docker containers"
docker rm $(docker ps -a -q)
echo "Removing all docker images tagged with <none>"
docker rmi -f $(docker images | grep "<none>" | awk "{print \$3}")
echo "Removing the old images from the image-store"
rm -r /opt/kuiga-web-1.1-SNAPSHOT/image-store/*

echo "Done"


