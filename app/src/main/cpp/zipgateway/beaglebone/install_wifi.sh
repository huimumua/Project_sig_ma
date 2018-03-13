#!/bin/bash

INTERFACES=/etc/network/interfaces
RADVD_CONF=/etc/radvd.conf
DHCP_HELPER_CONF=/etc/default/dhcp-helper

echo "Disable connman.service"
systemctl disable connman.service

read -p "Make sure the Beaglebone is connected to internet. Continue? [Y/N]: " internet_answer
if [ "$internet_answer" == "N" ] || [ "$internet_answer" == "n" ] ; then
    echo "exiting..."
    exit 0
fi

apt-get update
apt-get install parprouted dhcp-helper radvd libpcap0.8 libpcap-dev

read -p "What is name of the wireless interface. [Default: wlan0]: " wint
if [ -z $wint ]; then
    wint="wlan0"
fi

read -p "WiFi ESSID: " essid
if [ -z $essid ]; then
    echo "ERROR: ESSID is blank. Exiting."
fi

read -p "WiFi Password: " password 
if [ -z $password ]; then
    echo "ERROR: Wifi password is blank. Exiting."
fi

ip link show $wint > /dev/null 2>&1 
if [ "$?" == "1" ]; then
    echo "ERROR: Wireless inteface not found. Exiting"
    exit 0;
fi

grep "Configured by zip gateway" $INTERFACES && {
    echo "ERROR: $INTERFACES file has the configuration for $wint from us. Exiting"
    exit 0 
}
grep $wint $INTERFACES | grep -v '^#' && {
    echo "WARNING: $INTERFACES file already has configuration for $wint. Delete or comment the old configuration and run the script again"
    exit 0
}

echo "Backing up $INTERFACES file has $INTERFACES.backup"
cp $INTERFACES $INTERFACES.backup

	cat >> $INTERFACES <<!EOF
# Configured by zip gateway install_wifi.sh script
auto $wint 
iface $wint inet dhcp
   pre-up rfkill unblock all
   wpa-ssid "$essid"
   wpa-psk  "$password"
   post-up sysctl -w net.ipv6.conf.$wint.accept_ra=2
# END
!EOF

echo "Installing tun script for wireless configuration"
cp etc/zipgateway_wlan.tun /usr/local/etc/zipgateway.tun
sed -i "s/WLAN=wlan0/WLAN=$wint/g" /usr/local/etc/zipgateway.tun

echo "Installing init script for wireless configuration"
cp etc/init.d/zipgateway_wlan.bb /etc/init.d/zipgateway 

echo "Enabling dhcp-helper on $wint"
grep "DHCPHELPER_OPTS=\"-b $wint\"" $DHCP_HELPER_CONF && {
     echo "$DHCP_HELPER_CONF already has configuration for $wint. Not touching it"
}

sed -i '/DHCPHELPER_OPTS*/d' $DHCP_HELPER_CONF 
echo "DHCPHELPER_OPTS=\"-b $wint\"" >> $DHCP_HELPER_CONF

grep $wint $RADVD_CONF  |grep -v '^#' && {
     echo "ERROR: $RADVD_CONF file already has configuration for $wint. Please remove or comment it out"
     exit 1;
}

echo "configuraiong radvd"
cat >> $RADVD_CONF<<!EOF
interface $wint
{
  AdvSendAdvert on;
  prefix fd00:aaab::/64
  {
  };
};
!EOF

echo "Restarting radvd daemon"
/etc/init.d/radvd restart 
if [ "$?" -ne "0" ]; then
     echo "Failure in restarting radvd daemon"
     exit 1;
fi

echo "Enabling ipv6 packet forwarding"
sysctl -w net.ipv6.conf.all.forwarding=1

ifup $wint
if [ "$?" -ne "0" ]; then
     echo "ERRROR: Failure in bringing up $wint"
     echo "Try doing \"ifup $wint\""
     exit 1;
fi
gcc udp_redirect_pcap_ipv6.c -o udp_redirect_pcap_ipv6 -lpcap
cd udp-broadcast-relay/
make

echo "Configuration done"
echo "Restart the zipgateway now !!!"
