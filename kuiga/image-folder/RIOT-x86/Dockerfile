FROM debian

MAINTAINER KAJ THE MEMER

RUN apt-get update && apt-get install -y \
    libc6-dev-i386 \
    net-tools \
    netcat \
    sendip \
    curl \
    vim \
&& rm -rf /var/lib/apt/lists/*

ENV WORK /root
WORKDIR $WORK

COPY start_client .
COPY client.elf .

CMD chmod +x -R *
