#!/bin/sh

sed -i "s/wired_interface\ =\ br-lan/wired_interface\ =\ eth0/g" /etc/wicd/manager-settings.conf
/etc/init.d/wicd restart
ifconfig br-lan 0.0.0.0
ifconfig br-lan down
brctl delbr br-lan
