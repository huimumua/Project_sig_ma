################################################################################
# Makefile to make zip api library
################################################################################

CC=gcc
AR=ar
CFLAGS=


RM := rm -rf

LIB_OBJS = \
zip_application.o \
zip_frame.o \
zip_platform.o \
zip_session.o \
zip_transport.o \
zip_util.o


ifneq (,$(findstring DTCP_PORTAL,$(CFLAGS)))
	LIB_OBJS += zip_tls_server.o
endif

ifneq (,$(findstring DOS_ANDROID,$(CFLAGS)))
	LIB_OBJS += zip_netutil.o
endif

# Z/IP version 2 and above
ifneq (,$(findstring DZIP_V2,$(CFLAGS)))
	LIB_OBJS += zip_dtls.o zip_mdns.o
endif

LIB_HEADERS = \
../include/zip_application.h ../include/zip_session.h \
../include/zip_frame.h ../include/zip_transport.h \
../include/zip_tls_server.h \
../include/zip_error.h ../include/zip_platform.h \
../include/zip_util.h ../include/zip_mdns.h ../include/zip_dtls.h ../include/zwave/ZW_transport_api.h \
../include/zwave/ZW_classcmd.h

ifneq (,$(findstring CONFIG_DEBUG,$(CFLAGS)))
	COMPILE_OPTION = -g -O0
else
	COMPILE_OPTION = -O2
endif

NAME = libzip_api
SNAME = $(NAME).a
DNAME = $(NAME).so
SDIR = static
SOBJ    := $(addprefix $(SDIR)/, $(LIB_OBJS))
DDIR = shared
DOBJ    := $(addprefix $(DDIR)/, $(LIB_OBJS))
ARFLAGS= -r

INCLUDE_PATH =
ifneq (,$(findstring DUSE_OPENSSL,$(CFLAGS)))
INCLUDE_PATH += -I../../openssl/install/include/openssl  -I../../openssl/install/include
endif

EXTRA_CFLAGS = -Wall -DOS_LINUX

#Targets section:
libzip_api_dynamic: $(DNAME)

libzip_api_static: $(SNAME)

#The All target
all:
ifeq ($(SHARED),1)
	make libzip_api_dynamic
else
#Remove shared-library to prevent application to link wrongly for static build
	-@$(RM) $(DNAME)
	make libzip_api_static
endif

#Directory creation target
$(SDIR) $(DDIR):
	@echo "creating dir $@"
	@mkdir $@

#Clean up
clean:
	-$(RM) $(SNAME) $(DNAME) $(SDIR) $(DDIR) 

.PHONY: all clean libzip_api_dynamic libzip_api_static


#Static build section:
$(SNAME): $(SOBJ)
	@echo 'creating static library'	
	$(AR) $(ARFLAGS) $@ $^
	@echo 'Finished creating static library'
	@echo ''
#Compile c source file in static dir
$(SDIR)/%.o: %.c $(LIB_HEADERS) | $(SDIR)
	@echo 'Compiling file (static): $<'
	$(CC) $(COMPILE_OPTION) $(INCLUDE_PATH) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ -c $<
	@echo 'Finished compiling: $<'
	@echo ''

#Dynamic build section

# add -shared, -Wl,-z,nodefaultlib,-Wl,-soname,$@ and -Wl,-z,now to LDFLAGS
# breakdown :
#   * -shared allows dynamic libraries
#
#   * -Wl,-z,nodefaultlib: avoids potential library mismatch beween compilation and runtime environments.
#     It forces the current object (libzip_api) to skip looking for dependencies in /lib and /usr/lib. Instead, all libs paths have to be explicited in LD_LIBRARY_PATH instead. 
#
#   * -Wl,soname,$@ : make the library immune against hazardous file renaming.
#     It forces presence of SONAME "libzip_api.so" to the .so file. If for any reason, this .so file gets renamed, runtime loader will still be able to resolve the related "libzip_api.so" symbols thanks to the embedded SONAME. 
#
#   * -Wl,-z,now: allows immediate resolution of U symbols for current object being loaded. If any of depending symbols is missing from the required shared libs, Linux loader will crash immediately instead of crashing later, when jumping into the missing symbol (default Linux behaviour)

$(DNAME): EXTRA_CFLAGS += -fPIC
$(DNAME): LDFLAGS += -shared -Wl,-z,nodefaultlib -Wl,-soname,$@ -Wl,-z,now

$(DNAME): $(DOBJ)
	@echo 'creating dynamic library: $@'
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@
	@echo 'Finished creating dynamic library: $@'
	@echo ''
# Compile c source file in shared dir
$(DDIR)/%.o: %.c $(LIB_HEADERS) | $(DDIR)
	@echo 'Compiling file (shared): $<'
	$(CC) $(COMPILE_OPTION) $(INCLUDE_PATH) $(CFLAGS) $(EXTRA_CFLAGS)  -o $@ -c $<
	@echo 'Finished compiling: $<'
	@echo ''

