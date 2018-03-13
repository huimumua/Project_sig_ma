#!/bin/sh -e
### BEGIN INIT INFO
# Provides:          zipgateway
# Required-Start:    $local_fs
# Required-Stop:     $local_fs
# Default-Start:     2 3 4 5
# Default-Stop:      0 1 6
# Short-Description: zipgateway
### END INIT INFO

BINARY="zipgateway"
case "$1" in
start)
	echo "starting zipgateway"
	ifconfig br-lan > /dev/null 2>&1 || {
		brctl addbr br-lan
		brctl addif br-lan eth0
	}
	ifconfig eth0 up
	ifconfig br-lan up
	${BINARY} -e /etc/eeprom.dat &
	;;
stop)
	echo "stopping zipgateway"
	killall -SIGINT ${BINARY}
	;;
*)
	echo "Usage: /etc/init.d/zipgateway {start|stop}"
	exit 1
	;;
esac

exit 0

