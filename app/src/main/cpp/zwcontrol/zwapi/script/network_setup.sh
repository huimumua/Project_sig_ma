#Configure local host IPv6 address
ifconfig eth0 inet6 add 2002::15/64

#Add Z/IP router as default route
route -A inet6 add default gw 2002::32 dev eth0
