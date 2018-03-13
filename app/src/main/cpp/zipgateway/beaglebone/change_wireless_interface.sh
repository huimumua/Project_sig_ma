#!/bin/bash

if [ -z $1 ]; then
	echo "Usage: change_wireless_interface.sh <new interface name>"
	exit 1;
fi

echo "Changing the wireless interface name to $1"
wint=$1

sed -i "s/wlan0/$wint/g" /usr/local/etc/zipgateway.tun
sed -i "s/wlan0/$wint/g" /etc/radvd.conf
sed -i "s/wlan0/$wint/g" /etc/default/dhcp-helper
sed -i "s/wlan0/$wint/g" /etc/network/interfaces
