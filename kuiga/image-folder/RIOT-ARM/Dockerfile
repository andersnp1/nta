FROM jsurf/rpi-raspbian

RUN apt-get update && apt-get install -y \
    libc6-dev \
    net-tools \
    netcat \
    sendip \
    curl \
    iproute2 \
&& rm -rf /var/lib/apt/lists/*

ENV WORK /root
WORKDIR $WORK

COPY start_client .
COPY client.elf .

RUN chmod +x start_client client.elf
