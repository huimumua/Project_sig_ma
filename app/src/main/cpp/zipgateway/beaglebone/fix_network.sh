#!/bin/sh

sed -i "s/wired_interface\ =\ eth0/wired_interface\ =\ br-lan/g" /etc/wicd/manager-settings.conf
/etc/init.d/wicd restart
sed -i "s/auto_reconnect\ =\ False/auto_reconnect\ =\ True/g" /etc/wicd/manager-settings.conf
ifconfig eth0 0.0.0.0
