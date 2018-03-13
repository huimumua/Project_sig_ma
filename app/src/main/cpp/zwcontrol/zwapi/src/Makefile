################################################################################
# Makefile to make zipctl library
################################################################################

CC=gcc
AR=ar
CFLAGS=

RM := rm -rf

LIB_OBJS = \
zip_api_base.o \
zip_api_intf.o \
zip_api_network.o \
zip_api_sm.o \
zip_intf_sm.o \
zip_api_util.o \
zip_poll.o \
zip_set_poll.o \
zip_dev_poll.o \
zip_supervision.o \
zip_wkup_poll.o \
zip_node_sts.o \
zip_event_gen.o \
zip_ima.o \
cjson.o \
zip_dev_cfg.o \
zip_network_save.o

LIB_HEADERS = \
 ../include/zip_error.h ../include/zip_platform.h \
 ../include/zip_util.h ../include/zip_transport.h \
 ../include/zip_error.h ../include/zip_platform.h \
 ../include/zip_util.h ../include/zip_frame.h \
 ../include/zip_transport.h ../include/zip_session.h \
 ../include/zip_tls_server.h \
 ../include/zip_frame.h ../include/zip_application.h \
 ../include/zip_session.h ../include/zip_api_pte.h \
 ../include/zip_api.h ../include/zip_api_util.h \
 ../include/zip_poll.h ../include/zip_set_poll.h \
 ../include/zip_dev_poll.h ../include/zip_wkup_poll.h \
 ../include/zip_node_sts.h ../include/zip_intf_sm.h\
 ../include/zwave/ZW_controller_api.h \
 ../include/cjson.h ../include/zip_event_gen.h ../include/zip_cmd_lst.h \
 ../include/zip_network_save.h ../include/zip_supervision.h \
 ../include/zwave/ZW_transport_api.h ../include/zwave/ZW_classcmd.h

ifneq (,$(findstring CONFIG_DEBUG,$(CFLAGS)))
	COMPILE_OPTION = -g -O0
else
	COMPILE_OPTION = -O2
endif

NAME = libzip_ctl
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
libzip_ctl_dynamic: $(DNAME)

libzip_ctl_static: $(SNAME)

#The All target
all:
ifeq ($(SHARED),1)
	$(MAKE) libzip_ctl_dynamic
else
#Remove shared-library to prevent application to link wrongly for static build
	-@$(RM) $(DNAME)
	$(MAKE) libzip_ctl_static
endif

#Directory creation target
$(SDIR) $(DDIR):
	@echo "creating dir $@"
	@mkdir $@

#Clean up
clean:
	-$(RM) $(SNAME) $(DNAME)  $(SDIR) $(DDIR)


.PHONY: all clean libzip_ctl_dynamic libzip_ctl_static


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

# add -shared, -Wl,-z,nodefaultlib, -Wl,-soname,$@ and -Wl,-z,now to LDFLAGS
# breakdown :
#   * -shared allows dynamic libraries
#
#   * -Wl,-z,nodefaultlib: avoids potential library mismatch beween compilation and runtime environments.
#     It forces the current object (libzip_ctl) to skip looking for dependencies in /lib and /usr/lib. Instead, all libs paths have to be explicited in LD_LIBRARY_PATH instead.
#
#   * -Wl,soname,$@ : make the library iimmune against hazardous file renaming.
#     It forces presence of SONAME "libzip_ctl.so" to the .so file. If for any reason, this .so file gets renamed, runtime loader will still be able to resolve the related "libzip_ctl.so" symbols thanks to the embedded SONAME.
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


