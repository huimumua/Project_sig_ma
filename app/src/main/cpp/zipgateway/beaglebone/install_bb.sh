#!/bin/sh

if [ -x "/etc/init.d/zipgateway" ]; then
	/etc/init.d/zipgateway stop
	sleep 2
fi
cp usr/bin/zipgateway /usr/local/bin/
cp etc/zipgateway.tun /usr/local/etc/
cp etc/zipgateway.cfg /usr/local/etc/
cp etc/init.d/zipgateway /etc/init.d/
cp etc/*.pem /usr/local/etc
read -p "What is the serial port for zwave controller (default: /dev/ttyACM0): " controller
if [ -z $controller ]
then
	echo "Using ZipSerialAPIPortName=/dev/ttyACM0"
	sed -i "/ZipSerialAPIPortName*/d" /usr/local/etc/zipgateway.cfg
	echo ZipSerialAPIPortName=/dev/ttyACM0 >> /usr/local/etc/zipgateway.cfg
	echo "Note: If you want to change the port later change it in /usr/local/etc/zipgateway.cfg"
else
	sed -i "/ZipSerialAPIPortName*/d" /usr/local/etc/zipgateway.cfg
	echo ZipSerialAPIPortName=$controller
	echo ZipSerialAPIPortName=$controller >> /usr/local/etc/zipgateway.cfg
fi
update-rc.d zipgateway defaults 
echo "Disabling connman"
systemctl disable connman.service
