/* © 2014 Sigma Designs, Inc. This is an unpublished work protected by Sigma
 * Designs, Inc. as a trade secret, and is not to be used or disclosed except as
 * provided Z-Wave Controller Development Kit Limited License Agreement. All
 * rights reserved.
 *
 * Notice: All information contained herein is confidential and/or proprietary to
 * Sigma Designs and may be covered by U.S. and Foreign Patents, patents in
 * process, and are protected by trade secret or copyright law. Dissemination or
 * reproduction of the source code contained herein is expressly forbidden to
 * anyone except Licensees of Sigma Designs  who have executed a Sigma Designs’
 * Z-WAVE CONTROLLER DEVELOPMENT KIT LIMITED LICENSE AGREEMENT. The copyright
 * notice above is not evidence of any actual or intended publication of the
 * source code. THE RECEIPT OR POSSESSION OF  THIS SOURCE CODE AND/OR RELATED
 * INFORMATION DOES NOT CONVEY OR IMPLY ANY RIGHTS  TO REPRODUCE, DISCLOSE OR
 * DISTRIBUTE ITS CONTENTS, OR TO MANUFACTURE, USE, OR SELL A PRODUCT THAT IT  MAY
 * DESCRIBE.
 *
 * THE SIGMA PROGRAM AND ANY RELATED DOCUMENTATION OR TOOLS IS PROVIDED TO COMPANY
 * "AS IS" AND "WITH ALL FAULTS", WITHOUT WARRANTY OF ANY KIND FROM SIGMA. COMPANY
 * ASSUMES ALL RISKS THAT LICENSED MATERIALS ARE SUITABLE OR ACCURATE FOR
 * COMPANY’S NEEDS AND COMPANY’S USE OF THE SIGMA PROGRAM IS AT COMPANY’S
 * OWN DISCRETION AND RISK. SIGMA DOES NOT GUARANTEE THAT THE USE OF THE SIGMA
 * PROGRAM IN A THIRD PARTY SERVICE ENVIRONMENT OR CLOUD SERVICES ENVIRONMENT WILL
 * BE: (A) PERFORMED ERROR-FREE OR UNINTERRUPTED; (B) THAT SIGMA WILL CORRECT ANY
 * THIRD PARTY SERVICE ENVIRONMENT OR CLOUD SERVICE ENVIRONMENT ERRORS; (C) THE
 * THIRD PARTY SERVICE ENVIRONMENT OR CLOUD SERVICE ENVIRONMENT WILL OPERATE IN
 * COMBINATION WITH COMPANY’S CONTENT OR COMPANY APPLICATIONS THAT UTILIZE THE
 * SIGMA PROGRAM; (D) OR WITH ANY OTHER HARDWARE, SOFTWARE, SYSTEMS, SERVICES OR
 * DATA NOT PROVIDED BY SIGMA. COMPANY ACKNOWLEDGES THAT SIGMA DOES NOT CONTROL
 * THE TRANSFER OF DATA OVER COMMUNICATIONS FACILITIES, INCLUDING THE INTERNET,
 * AND THAT THE SERVICES MAY BE SUBJECT TO LIMITATIONS, DELAYS, AND OTHER PROBLEMS
 * INHERENT IN THE USE OF SUCH COMMUNICATIONS FACILITIES. SIGMA IS NOT RESPONSIBLE
 * FOR ANY DELAYS, DELIVERY FAILURES, OR OTHER DAMAGE RESULTING FROM SUCH ISSUES.
 * SIGMA IS NOT RESPONSIBLE FOR ANY ISSUES RELATED TO THE PERFORMANCE, OPERATION
 * OR SECURITY OF THE THIRD PARTY SERVICE ENVIRONMENT OR CLOUD SERVICES
 * ENVIRONMENT THAT ARISE FROM COMPANY CONTENT, COMPANY APPLICATIONS OR THIRD
 * PARTY CONTENT. SIGMA DOES NOT MAKE ANY REPRESENTATION OR WARRANTY REGARDING THE
 * RELIABILITY, ACCURACY, COMPLETENESS, CORRECTNESS, OR USEFULNESS OF THIRD PARTY
 * CONTENT OR SERVICE OR THE SIGMA PROGRAM, AND DISCLAIMS ALL LIABILITIES ARISING
 * FROM OR RELATED TO THE SIGMA PROGRAM OR THIRD PARTY CONTENT OR SERVICES. TO THE
 * EXTENT NOT PROHIBITED BY LAW, THESE WARRANTIES ARE EXCLUSIVE. SIGMA OFFERS NO
 * WARRANTY OF NON-INFRINGEMENT, TITLE, OR QUIET ENJOYMENT. NEITHER SIGMA NOR ITS
 * SUPPLIERS OR LICENSORS SHALL BE LIABLE FOR ANY INDIRECT, SPECIAL, INCIDENTAL OR
 * CONSEQUENTIAL DAMAGES OR LOSS (INCLUDING DAMAGES FOR LOSS OF BUSINESS, LOSS OF
 * PROFITS, OR THE LIKE), ARISING OUT OF THIS AGREEMENT WHETHER BASED ON BREACH OF
 * CONTRACT, INTELLECTUAL PROPERTY INFRINGEMENT, TORT (INCLUDING NEGLIGENCE),
 * STRICT LIABILITY, PRODUCT LIABILITY OR OTHERWISE, EVEN IF SIGMA OR ITS
 * REPRESENTATIVES HAVE BEEN ADVISED OF OR OTHERWISE SHOULD KNOW ABOUT THE
 * POSSIBILITY OF SUCH DAMAGES. THERE ARE NO OTHER EXPRESS OR IMPLIED WARRANTIES
 * OR CONDITIONS INCLUDING FOR SOFTWARE, HARDWARE, SYSTEMS, NETWORKS OR
 * ENVIRONMENTS OR FOR MERCHANTABILITY, NONINFRINGEMENT, SATISFACTORY QUALITY AND
 * FITNESS FOR A PARTICULAR PURPOSE.
 *
 * The Sigma Program  is not fault-tolerant and is not designed, manufactured or
 * intended for use or resale as on-line control equipment in hazardous
 * environments requiring fail-safe performance, such as in the operation of
 * nuclear facilities, aircraft navigation or communication systems, air traffic
 * control, direct life support machines, or weapons systems, in which the failure
 * of the Sigma Program, or Company Applications created using the Sigma Program,
 * could lead directly to death, personal injury, or severe physical or
 * environmental damage ("High Risk Activities").  Sigma and its suppliers
 * specifically disclaim any express or implied warranty of fitness for High Risk
 * Activities.Without limiting Sigma’s obligation of confidentiality as further
 * described in the Z-Wave Controller Development Kit Limited License Agreement,
 * Sigma has no obligation to establish and maintain a data privacy and
 * information security program with regard to Company’s use of any Third Party
 * Service Environment or Cloud Service Environment. For the avoidance of doubt,
 * Sigma shall not be responsible for physical, technical, security,
 * administrative, and/or organizational safeguards that are designed to ensure
 * the security and confidentiality of the Company Content or Company Application
 * in any Third Party Service Environment or Cloud Service Environment that
 * Company chooses to utilize.
 */

#ifndef ZW_CLASSCMD_EX_H_
#define ZW_CLASSCMD_EX_H_
#include <TYPES.H>
#include "ZW_classcmd.h"
#include "contiki-conf.h"
#include "sys/cc.h"
#include <provisioning_list_types.h>

#define ZW_FRAME(name, params) \
  typedef struct _ZW_##name##_FRAME_ { \
    BYTE      cmdClass; \
    BYTE      cmd;      \
    params              \
  } CC_ALIGN_PACK ZW_##name##_FRAME; \


#define ZW_NMFRAME_EX(name, params) \
  typedef struct _ZW_##name##_FRAME_EX_ { \
    BYTE      cmdClass; \
    BYTE      cmd;      \
    BYTE      seqNo;    \
    params              \
  } CC_ALIGN_PACK ZW_##name##_FRAME_EX; \

#define ZW_NMFRAME(name, params) \
  typedef struct _ZW_##name##_FRAME_ { \
    BYTE      cmdClass; \
    BYTE      cmd;      \
    BYTE      seqNo;    \
    params              \
  } CC_ALIGN_PACK ZW_##name##_FRAME; \


#define DEFAULT_SET_DONE    ADD_NODE_STATUS_DONE
#define DEFAULT_SET_BUSY    ADD_NODE_STATUS_FAILED



#define RETURN_ROUTE_ASSIGN                                              0x0D
#define RETURN_ROUTE_ASSIGN_COMPLETE                                     0x0E
#define RETURN_ROUTE_DELETE                                              0x0F
#define RETURN_ROUTE_DELETE_COMPLETE                                     0x10

#define COMMAND_CLASS_ZIP_ND                                                                                     0x58

/*Last working route report */
#define ROUTE_TYPE_STATIC 0
#define ROUTE_TYPE_DYNAMIC 1


#define ZIP_VERSION_V3 3

#if 0
ZW_NMFRAME(RETURN_ROUTE_ASSIGN,
    BYTE  sourceNode;
    BYTE  destinationNode; )

ZW_NMFRAME(RETURN_ROUTE_ASSIGN_COMPLETE,
  BYTE  status;  )

ZW_NMFRAME(RETURN_ROUTE_DELETE,
  BYTE  nodeID;  )

ZW_NMFRAME(RETURN_ROUTE_DELETE_COMPLETE,
  BYTE  status;  )
#endif 


#define START_FAILED_NODE_REPLACE 0x1 //ADD_NODE_ANY CODE
#define START_FAILED_NODE_REPLACE_S2 ADD_NODE_ANY_S2 //ADD_NODE_ANY2 CODE

#define STOP_FAILED_NODE_REPLACE  0x5 //ADD_NODE_STOP CODE



/************************************************************/
/* Version Report v2 command class structs */
/************************************************************/
typedef struct _ZW_VERSION_REPORT_2ID_V2_FRAME_
{
    BYTE      cmdClass;                     /* The command class */
    BYTE      cmd;                          /* The command */
    BYTE      zWaveLibraryType;             /**/
    BYTE      zWaveProtocolVersion;         /**/
    BYTE      zWaveProtocolSubVersion;      /**/
    BYTE      firmware0Version;           /**/
    BYTE      firmware0SubVersion;        /**/
    BYTE      hardwareVersion;           /**/
    BYTE      noOfFirmwareTargets;       /**/
    BYTE      firmware1Version;           /**/
    BYTE      firmware1SubVersion;        /**/
    BYTE      firmware2Version;           /**/
    BYTE      firmware2SubVersion;        /**/
    BYTE      firmware3Version;           /**/
    BYTE      firmware3SubVersion;        /**/
} ZW_VERSION_REPORT_2ID_V2_FRAME;

typedef struct _ZW_FIRMWARE_UPDATE_ACTIVATION_STATUS_REPORT_V5_FRAME_
{
    BYTE      cmdClass;                     /* The command class */
    BYTE      cmd;                          /* The command */
    BYTE      manufacturerId1;              /* MSB */
    BYTE      manufacturerId2;              /* LSB */
    BYTE      firmwareId1;                  /* MSB */
    BYTE      firmwareId2;                  /* LSB */
    BYTE      checksum1;                    /* MSB */
    BYTE      checksum2;                    /* LSB */
    BYTE      firmwareTarget;               /**/
    BYTE      firmwareUpdateStatus;         /**/
    BYTE      hardwareVersion;              /**/
} ZW_FIRMWARE_UPDATE_ACTIVATION_STATUS_REPORT_V5_FRAME;


/************************************************************/
/* Firmware Md Report V3 command class structs */
/************************************************************/
typedef struct _ZW_FIRMWARE_MD_REPORT_2ID_V3_FRAME_
{
    BYTE      cmdClass;                     /* The command class */
    BYTE      cmd;                          /* The command */
    BYTE      manufacturerId1;              /* MSB */
    BYTE      manufacturerId2;              /* LSB */
    BYTE      firmwareId1;                  /* MSB */
    BYTE      firmwareId2;                  /* LSB */
    BYTE      checksum1;                    /* MSB */
    BYTE      checksum2;                    /* LSB */
    BYTE      fwUpgradable;
    BYTE      numberOfFwTargets;
    BYTE          maxFragmentSize1;             /* MSB of max fragment size*/
    BYTE          maxFragmentSize2;             /* LSB of max fragment size*/
    BYTE      firmwareId11;                  /* MSB */
    BYTE      firmwareId12;                  /* LSB */
    BYTE      firmwareId21;                  /* MSB */
    BYTE      firmwareId22;                  /* LSB */
    BYTE      firmwareId31;                  /* MSB */
    BYTE      firmwareId32;                  /* LSB */
} ZW_FIRMWARE_MD_REPORT_2ID_V3_FRAME;

typedef struct _ZW_GATEWAY_PEER_REPORT_FRAME_
{
    BYTE      cmdClass;                     /* The command class */
    BYTE      cmd;                          /* The command */
    BYTE      peerProfile;                  /* PeerProfile  */
    BYTE      actualPeers;                  /* PeerProfile  */
    BYTE      ipv6Address1;                 /* MSB */
  BYTE      ipv6Address2;
  BYTE      ipv6Address3;
  BYTE      ipv6Address4;
  BYTE      ipv6Address5;
  BYTE      ipv6Address6;
  BYTE      ipv6Address7;
  BYTE      ipv6Address8;
  BYTE      ipv6Address9;
  BYTE      ipv6Address10;
  BYTE      ipv6Address11;
  BYTE      ipv6Address12;
  BYTE      ipv6Address13;
  BYTE      ipv6Address14;
  BYTE      ipv6Address15;
  BYTE      ipv6Address16;                /* LSB */
  BYTE    port1;            /*Port MSB*/
  BYTE    port2;            /*Port LSB */
  BYTE      peerLength;                   /* Peer Length */
  BYTE      peerName[1];                  /* Peer Name */
} ZW_GATEWAY_PEER_REPORT_FRAME;

typedef struct _ZW_GATEWAY_PEER_LOCK_SET_FRAME_
{
    BYTE      cmdClass;                     /* The command class */
    BYTE      cmd;                          /* The command */
    BYTE      showEnable;                  /* Show/Enable/Disable Bitmask  */
} ZW_GATEWAY_PEER_LOCK_SET_FRAME;

typedef struct _ZW_GATEWAY_UNSOLICITED_DESTINATION_SET_FRAME_
{
    BYTE      cmdClass;                     /* The command class */
    BYTE      cmd;                          /* The command */
    BYTE    unsolicit_ipv6_addr[16];      /* Unsolicited IPv6 address */
    BYTE      unsolicitPort1;               /* Unsolicited Port Number MSB */
    BYTE      unsolicitPort2;               /* Unsolicited Port Number LSB */

} ZW_GATEWAY_UNSOLICITED_DESTINATION_SET_FRAME;

typedef struct _ZW_GATEWAY_UNSOLICITED_DESTINATION_REPORT_FRAME_
{
    BYTE      cmdClass;                     /* The command class */
    BYTE      cmd;                          /* The command */
    BYTE    unsolicit_ipv6_addr[16];      /* Unsolicited IPv6 address */
    BYTE      unsolicitPort1;               /* Unsolicited Port Number MSB */
    BYTE      unsolicitPort2;               /* Unsolicited Port Number LSB */

} ZW_GATEWAY_UNSOLICITED_DESTINATION_REPORT_FRAME;

typedef struct _ZW_GATEWAY_APP_NODE_INFO_SET_FRAME_
{
    BYTE      cmdClass;                     /* The command class */
    BYTE      cmd;                          /* The command */
    BYTE    payload[1];             /* Variable Payload */

} ZW_GATEWAY_APP_NODE_INFO_SET_FRAME;

typedef struct _ZW_GATEWAY_APP_NODE_INFO_GET_FRAME_
{
    BYTE      cmdClass;                     /* The command class */
    BYTE      cmd;                          /* The command */

} ZW_GATEWAY_APP_NODE_INFO_GET_FRAME;

typedef struct _ZW_GATEWAY_APP_NODE_INFO_REPORT_FRAME_
{
    BYTE      cmdClass;                     /* The command class */
    BYTE      cmd;                          /* The command */
    BYTE    payload[1];             /* Variable Payload */

} ZW_GATEWAY_APP_NODE_INFO_REPORT_FRAME;


typedef struct _ZW_GATEWAY_CONFIGURATION_SET_
{
    BYTE      cmdClass;                     /* The command class */
    BYTE      cmd;                          /* The command */
    BYTE      payload[1];                   /*  Start of Confiuration Parameters */
}ZW_GATEWAY_CONFIGURATION_SET;

typedef struct _ZW_GATEWAY_CONFIGURATION_STATUS_
{
    BYTE      cmdClass;                     /* The command class */
    BYTE      cmd;                          /* The command */
    BYTE      status;                       /* configuration status status*/
}ZW_GATEWAY_CONFIGURATION_STATUS;

typedef struct _ZW_GATEWAY_CONFIGURATION_GET_
{
    BYTE      cmdClass;                     /* The command class */
    BYTE      cmd;                          /* The command */
}ZW_GATEWAY_CONFIGURATION_GET;

typedef struct _ZW_GATEWAY_CONFIGURATION_REPORT_
{
    BYTE      cmdClass;                     /* The command class */
    BYTE      cmd;                          /* The command */
    BYTE      payload[1];                   /*  Start of Confiuration Parameters */
}ZW_GATEWAY_CONFIGURATION_REPORT;


typedef struct _ZW_GATEWAY_PEER_SET_FRAME_
{
    BYTE      cmdClass;                     /* The command class */
    BYTE      cmd;                          /* The command */
    BYTE      peerProfile;                 /* PeerProfile  */
    BYTE      ipv6Address1;                 /* MSB */
        BYTE      ipv6Address2;
        BYTE      ipv6Address3;
        BYTE      ipv6Address4;
        BYTE      ipv6Address5;
        BYTE      ipv6Address6;
        BYTE      ipv6Address7;
        BYTE      ipv6Address8;
        BYTE      ipv6Address9;
        BYTE      ipv6Address10;
        BYTE      ipv6Address11;
        BYTE      ipv6Address12;
        BYTE      ipv6Address13;
        BYTE      ipv6Address14;
        BYTE      ipv6Address15;
        BYTE      ipv6Address16;                /* LSB */
        BYTE      port1;                                                /*Port MSB*/
        BYTE      port2;                                                /*Port LSB */
        BYTE      peerNameLength;                  /* Peer Length */
        BYTE      peerName[1];                 /* Peer Name */
} ZW_GATEWAY_PEER_SET_FRAME;


#define ZWAVEPLUS_INFO_VERSION_V2      0x02

#define ZWAVEPLUS_VERSION				0x01

/*FROM SDS12083*/
#define ICON_TYPE_UNASSIGNED                                0x0000 // MUST NOT be used by any product
#define ICON_TYPE_GENERIC_GATEWAY 0x0500

typedef union{
  ZW_COMMON_FRAME                                       ZW_Common;
  ZW_GATEWAY_MODE_SET_FRAME  ZW_GatewayModeSet;
  ZW_GATEWAY_PEER_SET_FRAME ZW_GatewayPeerSet;
  ZW_GATEWAY_PEER_LOCK_SET_FRAME ZW_GatewayLockSet;
  ZW_GATEWAY_UNSOLICITED_DESTINATION_SET_FRAME ZW_GatewayUnsolicitDstSet;
  ZW_GATEWAY_PEER_GET_FRAME ZW_GatewayPeerGet;

  BYTE bPadding[META_DATA_MAX_DATA_SIZE];
} ZW_APPLICATION_TX_BUFFER_EX;

#define GATEWAY_UNREGISTER                                                                 0x05


#define NODE_LIST_REPORT_LATEST_LIST 0
#define NODE_LIST_REPORT_NO_GUARANTEE 1



/* MAILBOX CC */


#define MAILBOX_LAST 0x4


typedef enum {
  DISABLE_MAILBOX=0,
  ENABLE_MAILBOX_SERVICE=1,
  ENABLE_MAILBOX_PROXY_FORWARDING=2,
} mailbox_configuration_mode_t;


typedef enum {
  MAILBOX_SERVICE_SUPPORTED=0x8,
  MAILBOX_PROXY_SUPPORTED=0x10,
} mailbox_configuration_supported_mode_t;

ZW_FRAME(MAILBOX_QUEUE,
    BYTE param1;
    BYTE handle;
    BYTE mailbox_entry[1];
    );


ZW_FRAME(MAILBOX_NODE_FAILING,
    BYTE node_ip[16];;
    );


typedef enum {
  MAILBOX_PUSH,
  MAILBOX_POP,
  MAILBOX_WAITING,
  MAILBOX_PING,
  MAILBOX_ACK,
  MAILBOX_NAK,
  MAILBOX_QUEUE_FULL,
} mailbox_queue_mode_t;
#define MAILBOX_POP_LAST_BIT 0x8
#define FIRMWARE_UPDATE_MD_PREPARE_GET                                                0x0A
#define FIRMWARE_UPDATE_MD_PREPARE_REPORT                                             0x0B

/**
 * Ask the receiver to prepare the target firmware for retrieval
 */
/************************************************************/
/* Firmware Update Md Prepare Get V5 command class structs */
/************************************************************/
typedef struct _ZW_FIRMWARE_UPDATE_MD_PREPARE_GET_V5_FRAME_
{
    BYTE      cmdClass;                     /* The command class */
    BYTE      cmd;                          /* The command */
    BYTE      manufacturerId1;              /* MSB */
    BYTE      manufacturerId2;              /* LSB */
    BYTE      firmwareId1;                  /* MSB */
    BYTE      firmwareId2;                  /* LSB */
    BYTE      firmwareTarget;               /**/
    BYTE      fragmentSize1;                /* MSB */
    BYTE      fragmentSize2;                /* LSB */
    BYTE      hardwareVersion;              /**/
} ZW_FIRMWARE_UPDATE_MD_PREPARE_GET_V5_FRAME;

/**
 * Ask the receiver to prepare the target firmware for retrieval
 */
/************************************************************/
/* Firmware Update Md Prepare Report V5 command class structs */
/************************************************************/
typedef struct _ZW_FIRMWARE_UPDATE_MD_PREPARE_REPORT_V5_FRAME_
{
    BYTE      cmdClass;                     /* The command class */
    BYTE      cmd;                          /* The command */
    BYTE      status;                       /**/
    BYTE      firmwareChecksum1;            /* MSB */
    BYTE      firmwareChecksum2;            /* LSB */
} ZW_FIRMWARE_UPDATE_MD_PREPARE_REPORT_V5_FRAME;

#define COMMAND_CLASS_NETWORK_MANAGEMENT_INSTALLATION_MAINTENANCE 0x67


typedef enum {
  ROUTE_CHANGES=0,
  TRANSMISSION_COUNT=1,
  NEIGHBORS=2,
  PACKET_ERROR_COUNT=3,
  TANSMISSION_TIME_SUM=4,
  TANSMISSION_TIME_SUM2=5,
} statistics_tlv;

#define  IMA_NODE_SPEED_96 1
#define  IMA_NODE_SPEED_40 2
#define  IMA_NODE_SPEED_100 4

#define IMA_NODE_REPEATER 0x80

/* ZIP Packet Header Extension TLV types (also called options in spec) */
#define ZIP_PACKET_EXT_EXPECTED_DELAY   1
#define INSTALLATION_MAINTENANCE_GET    2
#define INSTALLATION_MAINTENANCE_REPORT 3
#define ENCAPSULATION_FORMAT_INFO       4
#define ZWAVE_MULTICAST_ADDRESSING      5

typedef enum {
  EFI_SEC_LEVEL_NONE =0x0,
  EFI_SEC_S0 = 0x80,
  EFI_SEC_S2_UNAUTHENTICATED = 0x01,
  EFI_SEC_S2_AUTHENTICATED = 0x02,
  EFI_SEC_S2_ACCESS = 0x4,
} efi_security_level;

#define EFI_FLAG_CRC16    1
#define EFI_FLAG_MULTICMD 2

#define COMMAND_ZIP_KEEP_ALIVE 0x3
#define ZIP_KEEP_ALIVE_ACK_REQUEST  0x80
#define ZIP_KEEP_ALIVE_ACK_RESPONSE 0x40


/***************** Network management CC v 2  ******************/
#define NETWORK_MANAGEMENT_BASIC_VERSION_V2 2
#define NETWORK_MANAGEMENT_PROXY_VERSION_V2 2
#define NETWORK_MANAGEMENT_INCLUSION_VERSION_V2 2
#define NETWORK_MANAGEMENT_INCLUSION_VERSION_V3 3

#define NODE_ADD_KEYS_REPORT 0x11
#define NODE_ADD_KEYS_SET    0x12
#define NODE_ADD_DSK_REPORT  0x13
#define NODE_ADD_DSK_SET     0x14


/***************** Network management CC v 3  ******************/
#define COMMAND_SMART_START_JOIN_STARTED_REPORT  0x15 /* FIXME: Get official cmd no. assigned */
#define COMMAND_SMART_START_ADV_JOIN_MODE_SET    0x16 /* FIXME: Get official cmd no. assigned */
#define COMMAND_SMART_START_ADV_JOIN_MODE_GET    0x17 /* FIXME: Get official cmd no. assigned */
#define COMMAND_SMART_START_ADV_JOIN_MODE_REPORT 0x18 /* FIXME: Get official cmd no. assigned */
#define COMMAND_INCLUDED_NIF_REPORT              0x19 /* FIXME: Get official cmd no. assigned */

#define STATUS_DSK_ADD_STARTED                       1
#define STATUS_INCLUSION_REQUEST_ALREADY_INCLUDED    2


/**************** Network Management IMA ************************/
#define RSSI_GET                                                                    0x07
#define RSSI_REPORT                                                                 0x08

/****************** Provisioning List CC v1 ******************/
#define COMMAND_CLASS_PROVISIONING_LIST  0x78
#define PROVISIONING_LIST_VERSION_V1 1

#define COMMAND_PROVISION_SET         0x01
#define COMMAND_PROVISION_DELETE      0x02
#define COMMAND_PROVISION_ITER_GET    0x03
#define COMMAND_PROVISION_ITER_REPORT 0x04
#define COMMAND_PROVISION_GET         0x05
#define COMMAND_PROVISION_REPORT      0x06

#define PROVISIONING_LIST_OPT_CRITICAL_FLAG       0x01
#define PROVISIONING_LIST_OPT_TYPE_MASK           0xFE

#define PROVISIONING_LIST_DSK_LENGTH_MASK (0x1F)

#define PROVISIONING_LIST_STATUS_ADD_STARTED 1

ZW_NMFRAME_EX(INCLUSION_REQUEST,
);

ZW_NMFRAME_EX(PROVISION_SET,
    BYTE  reserved_dsk_length;
    BYTE  dsk[32];
//  BYTE  tlv_meta_data[0];
);

ZW_NMFRAME_EX(PROVISION_DELETE,
    BYTE  reserved_dsk_length;
    BYTE  dsk[32];
);

ZW_NMFRAME_EX(PROVISION_ITER_GET,
    BYTE  remaining;
);

ZW_NMFRAME_EX(PROVISION_ITER_REPORT,
    BYTE  remaining;
    BYTE  reserved_dsk_length;
    BYTE  dsk[32];
//  BYTE  tlv_meta_data[0];
);

ZW_NMFRAME_EX(PROVISION_GET,
    BYTE  reserved_dsk_length;
    BYTE  dsk[32];
);

ZW_NMFRAME_EX(PROVISION_REPORT,
    BYTE  reserved_dsk_length;
    BYTE  dsk[32];
//  BYTE  tlv_meta_data[0];
);


/* from NM basic */
#define DSK_GET              0x8
#define DSK_RAPORT           0x9

#define NODE_ADD_KEYS_SET_EX_ACCEPT_BIT 0x01
#define NODE_ADD_KEYS_SET_EX_CSA_BIT 0x02

#define NODE_ADD_DSK_SET_EX_ACCEPT_BIT  0x80
#define NODE_ADD_DSK_REPORT_DSK_LEN_MASK 0x0F
#define NODE_ADD_DSK_SET_DSK_LEN_MASK 0x0F
#define INCLUSION_REQUEST 0x10

#define DSK_GET_ADD_MODE_BIT 1

ZW_NMFRAME_EX(NODE_ADD_KEYS_REPORT,
    uint8_t request_csa;
    uint8_t requested_keys;
    );

ZW_NMFRAME_EX(NODE_ADD_KEYS_SET,
    uint8_t reserved_accept;
    uint8_t granted_keys;
    );


ZW_NMFRAME_EX(NODE_ADD_DSK_REPORT,
    uint8_t reserved_dsk_len;
    uint8_t dsk[16];
    );

ZW_NMFRAME_EX(NODE_ADD_DSK_SET,
    uint8_t accet_reserved_dsk_len;
    uint8_t dsk[2];
    );

ZW_NMFRAME_EX(DSK_GET,
    BYTE add_mode;

    );

ZW_NMFRAME_EX(DSK_RAPORT,
    BYTE add_mode;
    uint8_t dsk[16];
    );


ZW_NMFRAME_EX(LEARN_MODE_SET_STATUS,
    BYTE      status;                       /**/
    BYTE      reserved;                     /**/
    BYTE      newNodeId;
    BYTE      granted_keys;
    BYTE      kexFailType;
    BYTE      dsk[16];
    );

ZW_NMFRAME_EX(FAILED_NODE_REPLACE_STATUS,
    BYTE      status;                       /**/
    BYTE      nodeId;                       /**/
    BYTE      grantedKeys;
    BYTE      kexFailType;
);

ZW_NMFRAME_EX(SMART_START_JOIN_STARTED_REPORT,
    uint8_t reserved_dsk_len;
    uint8_t dsk[16];
    );

ZW_NMFRAME_EX(INCLUDED_NIF_REPORT,
    uint8_t  reserved_len;
    uint8_t  dsk[16];
    );

#define FIRMWARE_UPDATE_MD_VERSION_V5 5


#define COMMAND_CLASS_INCLUSION_CONTROLLER 0x74
#define INCLUSION_CONTROLLER_INITIATE 0x1
#define INCLUSION_CONTROLLER_COMPLETE 0x2

#define STEP_ID_PROXY_INCLUSION 0x1
#define STEP_ID_S0_INCLUSION 0x2
#define STEP_ID_PROXY_REPLACE 0x3


ZW_FRAME(INCLUSION_CONTROLLER_INITIATE,
  BYTE node_id;
  BYTE step_id;
);

ZW_FRAME(INCLUSION_CONTROLLER_COMPLETE,
    BYTE step_id;
    BYTE status;
);

#define NM_MULTI_CHANNEL_END_POINT_GET             0x05
#define NM_MULTI_CHANNEL_END_POINT_REPORT          0x06
#define NM_MULTI_CHANNEL_CAPABILITY_GET            0x07
#define NM_MULTI_CHANNEL_CAPABILITY_REPORT         0x08
#define NM_MULTI_CHANNEL_AGGREGATED_MEMBERS_GET    0x09
#define NM_MULTI_CHANNEL_AGGREGATED_MEMBERS_REPORT 0x0A

ZW_NMFRAME(NM_MULTI_CHANNEL_END_POINT_GET,
    BYTE      nodeID;                       /**/
);

ZW_NMFRAME(NM_MULTI_CHANNEL_END_POINT_REPORT,
    BYTE      nodeID;                       /**/
    BYTE      reserved;                     /**/
    BYTE      individualEndPointCount;      /**/
    BYTE      aggregatedEndPointCount;      /**/
);

ZW_NMFRAME(NM_MULTI_CHANNEL_CAPABILITY_GET,
    BYTE      nodeID;                       /**/
    BYTE      endpoint;                     /**/
);

ZW_NMFRAME(NM_MULTI_CHANNEL_CAPABILITY_REPORT,
    BYTE      nodeID;                       /**/
    BYTE      commandClassLength;           /**/
    BYTE      endpoint;                     /**/
    BYTE      genericDeviceClass;           /**/
    BYTE      specificDeviceClass;          /**/
    BYTE      commandClass1;                /**/
);

ZW_NMFRAME(NM_MULTI_CHANNEL_AGGREGATED_MEMBERS_GET,
    BYTE      nodeID;                       /**/
    BYTE      aggregatedEndpoint;           /**/
);

ZW_NMFRAME(NM_MULTI_CHANNEL_AGGREGATED_MEMBERS_REPORT,
    BYTE      nodeID;                       /**/
    BYTE      aggregatedEndpoint;           /**/
    BYTE      memberCount;
    BYTE      memberEndpoint1;
);

/* NETWORK_MANAGEMENT_INSTALLATION_MANTENANCE */
#define PRIORITY_ROUTE_SET LAST_WORKING_ROUTE_SET
#define PRIORITY_ROUTE_GET LAST_WORKING_ROUTE_GET
#define PRIORITY_ROUTE_REPORT LAST_WORKING_ROUTE_REPORT

#define UPDATE_STATE_INCLUDED_NODE_INFO_RECEIVED  0x86

#endif /* ZW_CLASSCMD_EX_H_ */
