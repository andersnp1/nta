.PHONY: all host program receiver install install-arm install-x86 install-web install-worker install-worker-arm
all: host program receiver

host:
	cd host && $(MAKE)

program:
	cd program && $(MAKE)

receiver:
	cd receiver && $(MAKE)

install: install-arm

install-arm: install-web install-worker-arm

install-x86: install-web install-worker-x86 

install-web:
	unzip kuiga/kuiga-web-*.zip -d /opt/
	ln -sf /opt/kuiga-web-* /opt/kuiga-web
	cp -R kuiga/image-folder /opt/kuiga-web/
	mkdir -p /usr/lib/systemd/system/
	cp service/nta_web.service /usr/lib/systemd/system
	systemctl enable nta_web.service
	systemctl daemon-reload

install-worker: install-broadcaster
	ln -sf /opt/kuiga_worker_jar /opt/kuiga-worker
	mkdir -p /usr/lib/systemd/system/
	cp service/nta_worker.service /usr/lib/systemd/system
	systemctl enable nta_worker.service
	systemctl daemon-reload

install-worker-arm: install-worker
	unzip kuiga/kuiga_worker_jar_arm.zip -d /opt/

install-worker-x86: install-worker
	unzip kuiga/kuiga_worker_jar_x86.zip -d /opt/

install-broadcaster: host
	cp host/broadcaster /usr/bin/
	
