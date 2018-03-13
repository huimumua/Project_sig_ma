################################################################################
# Makefile to make libraries
################################################################################

TARGET_PLATFORM =
TARGET_PRODUCT = ZIP_CLIENT
CFLAG :=

# Build for Portal
ifeq ($(TARGET_PRODUCT),PORTAL_OPENSSL)
	CFLAG += -DTCP_PORTAL -DUSE_OPENSSL
endif

# Build for CE
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

MAKE_PARAM_CFLAG = 'CFLAGS= $(CFLAG)'

# All Target
all::

all \
clean ::
	@cd lib && ${MAKE} ${MAKE_PARAM_CC} ${MAKE_PARAM_CFLAG} ${MAKE_PARAM_AR} $@
	@cd src && ${MAKE} ${MAKE_PARAM_CC} ${MAKE_PARAM_CFLAG} ${MAKE_PARAM_AR} $@

.PHONY: all clean
