################################################################################
# Makefile to make sample applications
################################################################################

TARGET_PLATFORM = LINUX_ZIP2
TARGET_PRODUCT = ZIP_CLIENT
CFLAG :=

ifeq ($(TARGET_PLATFORM),LINUX_ZIP2)
	CFLAG += -DZIP_V2 -DUSE_OPENSSL -pthread
endif

ifeq ($(TARGET_PLATFORM),VANTAGE_ZIP2)
        MAKE_PARAM_CC = 'CC=arm-none-linux-gnueabi-gcc'
        MAKE_PARAM_AR = 'AR=arm-none-linux-gnueabi-ar'
	CFLAG += -DSUPPORT_SIMPLE_AV_CONTROL -fPIC
	CFLAG += -DZIP_V2 -DUSE_OPENSSL -pthread
endif

ifeq ($(TARGET_PLATFORM),SIGMA_ZIP2)
	MAKE_PARAM_CC = 'CC=mipsel-linux-gcc'
	MAKE_PARAM_AR = 'AR=mipsel-linux-ar'
	CFLAG += -DSUPPORT_SIMPLE_AV_CONTROL -fPIC
	CFLAG += -DZIP_V2 -DUSE_OPENSSL -pthread
endif

ifeq ($(TARGET_PLATFORM),ANDROID_ZIP2)
	MAKE_PARAM_CC = 'CC=arm-linux-androideabi-gcc'
	MAKE_PARAM_AR = 'AR=arm-linux-androideabi-ar'
	CFLAG += -DOS_ANDROID
	CFLAG += -DZIP_V2 -DUSE_OPENSSL -pthread
endif

ifeq ($(TARGET_PLATFORM),OPENWRT_ZIP2)
	MAKE_PARAM_CC = 'CC=mips-openwrt-linux-gcc'
	MAKE_PARAM_AR = 'AR=mips-openwrt-linux-ar'
	CFLAG += -DZIP_V2 -DUSE_OPENSSL -pthread
endif

ifeq ($(TARGET_PLATFORM),BEAGLEBONE_ZIP2)
	MAKE_PARAM_CC = 'CC=arm-linux-gnueabihf-gcc'
	MAKE_PARAM_AR = 'AR=arm-linux-gnueabihf-ar'
	CFLAG += -DZIP_V2 -DUSE_OPENSSL -pthread
endif

ifeq ($(TARGET_PLATFORM),RASPBERRY_ZIP2)
	MAKE_PARAM_CC = 'CC=arm-linux-gnueabihf-gcc'
	MAKE_PARAM_AR = 'AR=arm-linux-gnueabihf-ar'
	CFLAG += -DZIP_V2 -DUSE_OPENSSL -pthread
endif

ifeq ($(TARGET_PLATFORM),DLINK_ZIP2)
	MAKE_PARAM_CC = 'CC=mipsel-linux-gcc'
	MAKE_PARAM_AR = 'AR=mipsel-linux-ar'
	CFLAG += -fPIC
	CFLAG += -DZIP_V2 -DUSE_OPENSSL -pthread
endif

ifeq ($(TARGET_PLATFORM),OSX_ZIP2)
	CFLAG += -DOS_MAC_X -arch i386 -DZIP_V2 -DUSE_OPENSSL -pthread
endif

ifeq ($(DEBUG),1)
	CFLAG += -DCONFIG_DEBUG
endif

MAKE_PARAM_CFLAG = 'CFLAGS=$(CFLAG)'
MAKE_SUBDIRS_TGT = all

ifeq ($(MAKECMDGOALS),clean)
MAKE_PARAM_CFLAG =
MAKE_SUBDIRS_TGT = clean
endif

INSTDIR = install

#Sub-directories to traverse
SUBDIRS = add_node basic bin_switch gw_discovery nw_reset rm_node bin_sensor

#Binaries to install
BINARIES = add_node/add_node basic/basic bin_switch/bin_switch gw_discovery/gw_discovery nw_reset/nw_reset rm_node/rm_node bin_sensor/bin_sensor

#Default target
all::

all \
clean :: $(SUBDIRS)

$(SUBDIRS) :
	$(MAKE) -C $@ ${MAKE_PARAM_CC} ${MAKE_PARAM_CFLAG} ${MAKE_PARAM_AR} ${MAKE_SUBDIRS_TGT}

#Directory creation target
$(INSTDIR):
	@echo "creating directory $@"
	@mkdir $@

#Install binaries to install directory
install_bin : | $(INSTDIR)
	for i in $(BINARIES); do \
		cp -f $$i $(INSTDIR); \
	done

#Install binaries and configuration files to install directory
install_all : | $(INSTDIR)
	cp -f add_node/app.cfg $(INSTDIR)
	cp -f ../config/cmd_class.cfg $(INSTDIR)
	cp -f ../config/zwave_device_rec.txt $(INSTDIR)
	for i in $(BINARIES); do \
		cp -f $$i $(INSTDIR); \
	done


.PHONY: all clean $(SUBDIRS) install_bin install_all
