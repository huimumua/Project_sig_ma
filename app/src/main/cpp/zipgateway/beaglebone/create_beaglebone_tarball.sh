#!/bin/bash

DIR=${1}
BUILDDIR=${2}
VMJ=${3}
VMN=${4}
echo $DIR
echo $BUILDDIR
if [ -d "${DIR}/install/beaglebone" ]; then
	echo "removing previous ${DIR}/install/beaglbone/ directory"
	rm -rf ${BUILDDIR}/install/beaglebone
fi
mkdir -p ${BUILDDIR}/install/beaglebone/
cp ${DIR}/beaglebone/install_bb.sh ${BUILDDIR}/install/beaglebone/
cp ${DIR}/beaglebone/radvd.conf ${BUILDDIR}/install/beaglebone/
cp ${DIR}/beaglebone/uninstall_bb.sh ${BUILDDIR}/install/beaglebone/
cp ${DIR}/beaglebone/fix_network.sh ${BUILDDIR}/install/beaglebone/
cp ${DIR}/beaglebone/restore_network.sh ${BUILDDIR}/install/beaglebone/
mkdir -p ${BUILDDIR}/install/beaglebone/etc/init.d/
mkdir -p ${BUILDDIR}/install/beaglebone/usr/bin/
#	./install.sh -g root -o root -m 0755 -s beaglebone/zipgateway.bb install/etc/init.d/zipgateway 
cp ${DIR}/beaglebone/zipgateway.bb ${BUILDDIR}/install/beaglebone/etc/init.d/zipgateway
cp ${DIR}/beaglebone/zipgateway_wlan.bb ${BUILDDIR}/install/beaglebone/etc/init.d/zipgateway_wlan
cp ${BUILDDIR}/zipgateway ${BUILDDIR}/install/beaglebone/usr/bin/
cp ${BUILDDIR}/udprelay/udprelay ${BUILDDIR}/install/beaglebone/usr/bin/
cp ${DIR}/files/zipgateway.cfg ${DIR}/files/zipgateway.tun ${DIR}/files/zipgateway_wlan.tun ${BUILDDIR}/install/beaglebone/etc/
cp ${DIR}/WRTpackage/files/*.pem ${BUILDDIR}/install/beaglebone/etc/
cp -r ${DIR}/beaglebone/wlan/ ${BUILDDIR}/install/beaglebone/
cp ${DIR}/beaglebone/change_wireless_interface.sh ${BUILDDIR}/install/beaglebone/
cd ${BUILDDIR}
tar -cvf zipgw_beaglebone-${VMJ}.${VMN}.tar install/beaglebone

