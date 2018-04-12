project(zipgateway)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wparentheses -Wno-array-bounds -D_DEFAULT_SOURCE -fPIE -fPIC")

add_definitions ( -DZIPGW -DDEBUG_S2_INCL_FSM -DDEBUG_S2_FSM)

set(CMAKE_LD_FLAGS "-fPIE -fPIC")

set(OPENSSL_LIBRARIES "ssl")
set(LibCRYPTO_LIBRARIES "crypto")

set(CMAKE_SYSTEM_NAME "Linux")
set(SYSTEM_PLATFORM "Android")

if(${SYSTEM_PLATFORM} MATCHES "Android")
add_definitions(-DANDROID_PLATFORM -DZIPGATEWAY_DEBUG)
set(LibUSB_LIBRARIES "")
set(LIBUSB_INCLUDE_DIRS "")
else()
add_definitions(-DZIPGATEWAY_DEBUG)
set(LIBUSB_INCLUDE_DIRS "../libusb/libusb")
set(LibUSB_LIBRARIES "usb")
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR})

set(OPENSSL_INCLUDE_DIRS "../openssl-1.0.2l/include")

set(ZWAVE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Z-Wave/include )
# Include lib S2
set(LIBS2 "./libs2" )
set(LIBS2_INCLUDE_DIRS "${LIBS2}/include")
set(S2_LIBRARIES s2_controller s2crypto aes)
set(TS2 "${LIBS2}/transport_service")
set(TS2_SRC ${TS2}/transport_service2.c ${TS2}/transport2_fsm.c)
add_subdirectory( ${LIBS2} libs2 )

add_subdirectory( convert_eeprom )

set(GW_SRC
src-gen/Secure_learn.c
src/Bridge.c
src/CC_FirmwareUpdate.c
src/CC_InclusionController.c
src/command_handler.c
src/CC_Gateway.c
src/CC_NetworkManagement.c
src/CC_Portal.c
src/CC_PowerLevel.c
src/CC_ManufacturerSpecific.c
src/CC_Version.c
src/CC_ZWavePlus.c
src/CC_ZIPNaming.c
src/CC_Supervision.c
src/CC_InstalltionAndMaintenance.c
src/ClassicZIPNode.c
src/DTLS_server.c
src/Mailbox.c
src/RD_DataStore.c
src/RD_internal.c
src/ResourceDirectory.c
src/ZIP_Router.c
src/ZWFirmwareUpdate.c
src/ZW_ZIPApplication.c
src/ZW_tcp_client.c
src/ZW_udp_server.c
src/crc32alg.c
src/dhcpc2.c
src/ipv46_nat.c
src/mDNSService.c
src/node_queue.c
src/serial_api_process.c
src/serialapi/Serialapi.c
src/serialapi/conhandle.c
src/smalloc.c
src/transport/Security_Scheme0.c
src/transport/ZW_PRNG.c
src/transport/ZW_SendDataAppl.c
src/transport/ZW_SendRequest.c
src/transport/rijndael-alg-fst.c
src/transport/security_layer.c
src/transport/S2_wrap.c
src/transport/s2_keystore.c
src/zwdb.c
src/zwpgrmr/crc32.c
src/zwpgrmr/linux_serial_interface.c
src/zwpgrmr/linux_usb_interface.c
src/zwpgrmr/zpg.c
src/zwpgrmr/zpgp.c
src/CommandAnalyzer.c
src-gen/get_list.c
src-gen/pvs_cfg.tab.c
src-gen/lex.yy.c
src/provisioning_list.c
src/pvs_parse_help.c
src/CC_provisioning_list.c
)


set(TS2_INCLUDE_DIRS "${LIBS2}/transport_service")
#add_subdirectory( ${TS2} transport_service ) 

set(CONTIKI contiki )

include_directories( 
${OPENSSL_INCLUDE_DIRS}
${LIBUSB_INCLUDE_DIRS}
${LIBS2_INCLUDE_DIRS}
${TS2_INCLUDE_DIRS}
Z-Wave/include 
${CONTIKI}/platform/linux 
${CONTIKI}/platform/linux/dev 
${CONTIKI}/cpu/native/. 
${CONTIKI}/cpu/native/net 
src/serialapi 
src-gen 
src/zwpgrmr 
src/transport
${CONTIKI}/core/dev 
${CONTIKI}/core/lib 
${CONTIKI}/core/net 
${CONTIKI}/core/sys 
${CONTIKI}/core 
src 
.
)


add_definitions( -DPROJECT_CONF_H=\"project-conf.h\" -DSERIAL_LOG -DSUPPORTS_MDNS 
-DSUPPORTS_MDNS -DSECURITY_SUPPORT -DCONTROLLER_CHANGE_SUPPORT -D__ROUTER_VERSION__
-DCONTIKI_TARGET_LINUX -DUIP_CONF_IPV6=1 -DAUTOSTART_ENABLE  -DPROVISIONING_LIST_SIZE=232)

set(CONTIKI_SRC
${CONTIKI}/core/dev/serial-line.c
${CONTIKI}/core/lib/crc16.c
${CONTIKI}/core/lib/list.c
${CONTIKI}/core/lib/memb.c
${CONTIKI}/core/lib/random.c
${CONTIKI}/core/lib/ringbuf.c
${CONTIKI}/core/net/resolv.c
${CONTIKI}/core/net/tcpip.c
${CONTIKI}/core/net/uip-debug.c
${CONTIKI}/core/net/uip-ds6.c
${CONTIKI}/core/net/uip-icmp6.c
${CONTIKI}/core/net/uip-nd6.c
${CONTIKI}/core/net/uip-packetqueue.c
${CONTIKI}/core/net/uip-split.c
${CONTIKI}/core/net/uip-udp-packet.c
${CONTIKI}/core/net/uip6.c
${CONTIKI}/core/net/uiplib.c
${CONTIKI}/core/sys/autostart.c
${CONTIKI}/core/sys/ctimer.c
${CONTIKI}/core/sys/etimer.c
${CONTIKI}/core/sys/process.c
${CONTIKI}/core/sys/procinit.c
${CONTIKI}/core/sys/stimer.c
${CONTIKI}/core/sys/timer.c
${CONTIKI}/platform/linux/./clock.c
${CONTIKI}/platform/linux/./contiki-main.c
${CONTIKI}/platform/linux/./parse_config.c
${CONTIKI}/core/lib/assert.c
${CONTIKI}/core/net/tcpip_ipv4.c
${CONTIKI}/core/net/uip-ipv4-split.c
${CONTIKI}/core/net/uip-ipv4-udp-packet.c
${CONTIKI}/core/net/uip.c
${CONTIKI}/core/net/uip_arp.c
${CONTIKI}/cpu/native/linux-serial.c
${CONTIKI}/platform/linux/File_eeprom.c
)

add_library(zipgateway SHARED ${GW_SRC} ${CONTIKI_SRC} ${TS2_SRC})
find_library(log-lib log)
target_link_libraries(zipgateway ${S2_LIBRARIES} ${OPENSSL_LIBRARIES} ${LibCRYPTO_LIBRARIES} ${LibUSB_LIBRARIES} usbserial-jni ${log-lib})


