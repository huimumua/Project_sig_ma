#!/bin/sh


if [ -x "/etc/init.d/zipgateway" ]; then
	/etc/init.d/zipgateway stop
fi

rm /usr/local/etc/zipgateway.cfg
rm /usr/local/etc/zipgateway.tun
rm /etc/init.d/zipgateway
rm /usr/local/etc/*.pem
systemctl enable connman.service
