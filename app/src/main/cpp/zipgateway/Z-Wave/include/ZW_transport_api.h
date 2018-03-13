/****************************************************************************
 *
 * Copyright (c) 2001-2013
 * Sigma Designs, Inc.
 * All Rights Reserved
 *
 *---------------------------------------------------------------------------
 *
 * Description: Z-Wave Transport Application layer interface
 *
 * Author:   Ivar Jeppesen
 *
 * Last Changed By:  $Author: jbu $
 * Revision:         $Revision: 29557 $
 * Last Changed:     $Date: 2014-09-30 15:25:45 +0200 (Tue, 30 Sep 2014) $
 *
 ****************************************************************************/
/**
 * \file ZW_transport_api.h
 * \brief Z-Wave Transport Application layer interface.
 *
 * The Z Wave transport layer controls transfer of data between Z Wave nodes
 * including retransmission, frame check and acknowledgement. The Z Wave
 * transport interface includes functions for transfer of data to other Z Wave
 * nodes. Application data received from other nodes is handed over to the
 * application via the \ref ApplicationCommandHandler function. The ZW_MAX_NODES
 * define defines the maximum of nodes possible in a Z Wave network.
 */
#ifndef _ZW_TRANSPORT_API_H_
#define _ZW_TRANSPORT_API_H_

/****************************************************************************/
/*                              INCLUDE FILES                               */
/****************************************************************************/

/****************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                       */
/****************************************************************************/

/* Max number of nodes in a Z-wave system */
#define ZW_MAX_NODES        232

/************************************************************/
/* Node Information frame*/
/************************************************************/
#define NODEPARM_MAX  35   /* max. number of parameters */


/* Transmit frame option flags */
#define TRANSMIT_OPTION_ACK           0x01    /* request acknowledge from destination node */
#define TRANSMIT_OPTION_LOW_POWER     0x02    /* transmit at low output power level (1/3 of normal RF range)*/
#ifdef ZW_SLAVE
#define TRANSMIT_OPTION_RETURN_ROUTE  0x04    /* request transmission via return route */
#endif
#define TRANSMIT_OPTION_AUTO_ROUTE    0x04    /* request retransmission via repeater nodes */
/* do not use response route - Even if available */
#define TRANSMIT_OPTION_NO_ROUTE      0x10
/* Use explore frame if needed */
#define TRANSMIT_OPTION_EXPLORE       0x20

/* Transmit frame option flag which are valid when sending explore frames  */
#define TRANSMIT_EXPLORE_OPTION_ACK         TRANSMIT_OPTION_ACK
#define TRANSMIT_EXPLORE_OPTION_LOW_POWER   TRANSMIT_OPTION_LOW_POWER

/* Received frame status flags */
/**
 *  \defgroup RECEIVE_STATUS Status codes for receiving frames.
 * \addtogroup RECEIVE_STATUS
 * @{
 */

/**
 * A response route is locked by the application
 */
#define RECEIVE_STATUS_ROUTED_BUSY    0x01
/**
 * Received at low output power level, this must
 * have the same value as TRANSMIT_OPTION_LOW_POWER
 */
#define RECEIVE_STATUS_LOW_POWER      0x02
/**
 * Mask for masking out the received frametype bits
 */
#define RECEIVE_STATUS_TYPE_MASK      0x0C
/**
 * Received frame is singlecast frame (rxOptions == xxxx00xx)
 */
#define RECEIVE_STATUS_TYPE_SINGLE    0x00
/**
 * Received frame is broadcast frame  (rxOptions == xxxx01xx)
 */
#define RECEIVE_STATUS_TYPE_BROAD     0x04
/**
 * Received frame is multicast frame (rxOptions == xxxx10xx)
 * Only TYPE_BROAD can be active at the same time as TYPE_EXPLORE
 */
#define RECEIVE_STATUS_TYPE_MULTI     0x08
/**
 * Received frame is an explore frame (rxOptions == xxx1xxxx)
 * Only TYPE_BROAD can be active at the same time as TYPE_EXPLORE
 */
#define RECEIVE_STATUS_TYPE_EXPLORE   0x10
/**
 * Received frame is not send to me (useful only in promiscuous mode)
 */
#define RECEIVE_STATUS_FOREIGN_FRAME  0x40
/**
 * Received frame is from a foreign homeid.
 *
 * Used by ApplicationControllerUpdate() with status UPDATE_STATE_INCLUDED_NODE_INFO_RECEIVED.
 * Currently undocumented in the from embedded SDK.
 */
#define RECEIVE_STATUS_FOREIGN_HOMEID  0x80

/**
 * @}
 */

/* Predefined Node ID's */
#define NODE_BROADCAST              0xFF    /* broadcast */
#define ZW_TEST_NOT_A_NODEID        0x00    /* */

/* Transmit complete codes */
#define TRANSMIT_COMPLETE_OK      0x00
#define TRANSMIT_COMPLETE_NO_ACK  0x01  /* retransmission error */
#define TRANSMIT_COMPLETE_FAIL    0x02  /* transmit error */
#define TRANSMIT_ROUTING_NOT_IDLE 0x03  /* transmit error */
#define TRANSMIT_UNINIT           0xff  /* Uninitialized */
#ifdef ZW_CONTROLLER
/* Assign route transmit complete but no routes was found */
#define TRANSMIT_COMPLETE_NOROUTE 0x04  /* no route found in assignroute */
                                        /* therefore nothing was transmitted */
#endif

/* RSSI feedback constants */
/* This is a signed 8-bit value. Note that values from 101-123 are reserved.
 * All other values are received power in dBms. */
#define RSSI_NOT_AVAILABLE 127       /* RSSI measurement not available for this transmission */
#define RSSI_HOP_UNUSED 126          /* This hop was not used during transmission */
#define RSSI_MAX_POWER_SATURATED 125 /* Receiver saturated. RSSI too high to measure precisely. */
#define RSSI_BELOW_SENSITIVITY 124   /* No signal detected. The RSSI is too low to measure precisely. */


/* ZW_REDISCOVERY_NEEDED callback values. */
/* Note that they are different from ZW_REQUEST_NETWORK_UPDATE callbacks */
#define ZW_ROUTE_LOST_FAILED      0x04  /*Node Asked wont help us*/
#define ZW_ROUTE_LOST_ACCEPT      0x05  /*Accepted to help*/


#ifdef ZW_ROUTING_DEMO
/* Max hops in route */
#define TRANSMIT_ROUTED_ATTEMPT   0x08
#endif /*ZW_ROUTING_DEMO*/

#define ZW_MAX_CACHED_RETURN_ROUTE_DESTINATIONS  5

#define ZW_RF_TEST_SIGNAL_CARRIER              0x00
#define ZW_RF_TEST_SIGNAL_CARRIER_MODULATED    0x01

/* Max hops in route */
#define MAX_REPEATERS      4

typedef struct {
  BYTE bRepeaters;         /* Repeaters in route, zero for direct range */
  /* rssi_values per hop for direct and routed frames.
   * Contains repeaters + 1 value pairs. Each pair contains an
   * incoming and an outgoing rssi value. */
  uint8_t rssi_values[MAX_REPEATERS + 1];
  uint16_t transmitTicks;
  uint8_t ackChannel;
  uint8_t lastTxChannel;
  uint8_t routeSchemeState;
  uint8_t repeaters[4];
  uint8_t routeSpeed;
  uint8_t routeTries;
  uint8_t lastFailedLinkFrom;
  uint8_t lastFailedLinkTo;
} TX_STATUS_TYPE;

typedef struct _RECEIVE_OPTIONS_TYPE {
  BYTE  rxStatus;           /* Frame header info */
  BYTE  sourceNode;         /* Command sender Node ID */
  BYTE  destNode;           /* Frame destination ID, only valid when frame is not Multicast*/
  /*enum SECURITY_SCHEME securityScheme;*/
  BYTE  sourceEndpoint;
  BYTE  destEndpoint;
} RECEIVE_OPTIONS_TYPE;

/* Transmit options for ZW_SendDataEx */
typedef struct _TRANSMIT_OPTIONS_TYPE {
  BYTE destNode; /* Destination node ID - 0xFF == all nodes */
  BYTE txOptions;
  BYTE securityOptions;
  BYTE  sourceEndpoint;
  BYTE  destEndpoint;
} TRANSMIT_OPTIONS_TYPE;

enum ZW_SENDDATA_EX_RETURN_CODES {
    ZW_TX_FAILED = 0,
    ZW_TX_IN_PROGRESS = 1,
    ZW_NO_SECURITY = 2
};


#ifdef ZW_SLAVE_ROUTING

/*============================   ZW_RouteDestinations   ======================
**    Function description
**      This contains a list of nodes that currently can be reached via
**      return routes.
**      This list should not be altered by Applications.s
**
**--------------------------------------------------------------------------*/
extern IBYTE ZW_RouteDestinations[ZW_MAX_CACHED_RETURN_ROUTE_DESTINATIONS];

#endif  /* ZW_SLAVE_ROUTING */


#ifndef ZW_CONTROLLER_BRIDGE
/*==============================   ZW_SendData   ============================
**    Transmit data buffer to a single ZW-node or all ZW-nodes (broadcast).
**
**
**    txOptions:
**          TRANSMIT_OPTION_LOW_POWER   transmit at low output power level (1/3 of
**                                      normal RF range).
**          TRANSMIT_OPTION_ACK         the multicast frame will be followed by a
**                                      singlecast frame to each of the destination nodes
**                                      and request acknowledge from each destination node.
**          TRANSMIT_OPTION_AUTO_ROUTE  request retransmission on singlecast frames
**                                      via repeater nodes (at normal output power level).
**          TRANSMIT_OPTION_EXPLORE     Use explore frame route resolution if all else fails
**
** extern BYTE             RET  FALSE if transmitter queue overflow
** ZW_SendData(
**   BYTE  nodeID,          IN  Destination node ID (0xFF == broadcast)
**   BYTE *pData,           IN  Data buffer pointer
**   BYTE  dataLength,      IN  Data buffer length
**   BYTE  txOptions,       IN  Transmit option flags
**   VOID_CALLBACKFUNC(completedFunc)( IN  Transmit completed call back function
**          BYTE txStatus));    IN Transmit status
**--------------------------------------------------------------------------*/
#define ZW_SEND_DATA(node,data,length,options,func) ZW_SendData(node,data,length,options,func)

#else

/*===============================   ZW_SendData_Bridge   =======================
**    Transmit data buffer to a single ZW-node or all ZW-nodes (broadcast).
**
**
**    txOptions:
**          TRANSMIT_OPTION_LOW_POWER   transmit at low output power level (1/3 of
**                                      normal RF range).
**          TRANSMIT_OPTION_ACK         The destination is asked to acknowledge the reception
**                                      of the frame. A maximum of 2 retries are done if no
**                                      ACK received.
**          TRANSMIT_OPTION_AUTO_ROUTE  request retransmission on singlecast frames
**                                      via repeater nodes (at normal output power level).
**          TRANSMIT_OPTION_EXPLORE     Use explore frame route resolution if all else fails
**
** extern BYTE             RET  FALSE if transmitter queue overflow
** ZW_SendData_Bridge(
**   BYTE  bSrcNodeID,      IN  Source node ID
**   BYTE  nodeID,          IN  Destination node ID (0xFF == broadcast)
**   BYTE *pData,           IN  Data buffer pointer
**   BYTE  dataLength,      IN  Data buffer length
**   BYTE  txOptions,       IN  Transmit option flags
** VOID_CALLBACKFUNC(completedFunc)(   IN  Transmit completed call back function
**        BYTE txStatus));    IN Transmit status
**--------------------------------------------------------------------------*/
/* For backward compilability */
#define ZW_SendData(node,data,length,options,func) ZW_SendData_Bridge(0xff,node,data,length,options,func)
#define ZW_SEND_DATA(node,data,length,options,func) ZW_SendData_Bridge(0xff,node,data,length,options,func)
/* Use this */
#define ZW_SEND_DATA_BRIDGE(srcnode,node,data,length,options,func) ZW_SendData_Bridge(srcnode,node,data,length,options,func)

#endif  /* ZW_CONTROLLER_BRIDGE */


#if defined(ZW_CONTROLLER) || defined(ZW_SLAVE_ROUTING)

/*============================   ZW_SendDataAbort   ========================
**    Abort the ongoing transmit started with ZW_SendData()
**
**    Side effects:
**
**--------------------------------------------------------------------------*/
#define ZW_SEND_DATA_ABORT() ZW_SendDataAbort()


#ifndef ZW_CONTROLLER_BRIDGE
/*===============================   ZW_SendDataMulti   ======================
**    Transmit data buffer to a list of Z-Wave Nodes (multicast frame).
**
**
**    txOptions:
**          TRANSMIT_OPTION_LOW_POWER   transmit at low output power level (1/3 of
**                                      normal RF range).
**          TRANSMIT_OPTION_ACK         the multicast frame will be followed by a
**                                      singlecast frame to each of the destination nodes
**                                      and request acknowledge from each destination node.
**          TRANSMIT_OPTION_AUTO_ROUTE  request retransmission on singlecast frames
**                                      via repeater nodes (at normal output power level).
**
** extern BYTE            RET  FALSE if transmitter queue overflow
** ZW_SendDataMulti(
**  BYTE *pNodeIDList,     IN  List of destination node ID's
**  BYTE  numberNodes,     IN  Number of Nodes
**  BYTE *pData,           IN  Data buffer pointer
**  BYTE  dataLength,      IN  Data buffer length
**  BYTE  txOptions,       IN  Transmit option flags
**  VOID_CALLBACKFUNC(completedFunc)( IN  Transmit completed call back function
**    BYTE txStatus)); IN Transmit status
**--------------------------------------------------------------------------*/
#define ZW_SEND_DATA_MULTI(nodelist,data,length,options,func) ZW_SendDataMulti(nodelist,data,length,options,func)

#else

/*=============================   ZW_SendDataMulti_Bridge   ====================
**    Transmit data buffer to a list of Z-Wave Nodes (multicast frame).
**
**
**    txOptions:
**          TRANSMIT_OPTION_LOW_POWER   transmit at low output power level (1/3 of
**                                      normal RF range).
**          TRANSMIT_OPTION_ACK         the multicast frame will be followed by a
**                                      singlecast frame to each of the destination nodes
**                                      and request acknowledge from each destination node.
**          TRANSMIT_OPTION_AUTO_ROUTE  request retransmission on singlecast frames
**                                      via repeater nodes (at normal output power level).
**          TRANSMIT_OPTION_EXPLORE     Use explore frame route resolution if all else fails
**
** extern BYTE            RET  FALSE if transmitter queue overflow
** ZW_SendDataMulti_Bridge(
**  BYTE bSrcNodeID,       IN  Source NodeID - if 0xFF then controller ID is set as source
**  BYTE *pNodeIDList,     IN  List of destination node ID's
**  BYTE  numberNodes,     IN  Number of Nodes
**  BYTE *pData,           IN  Data buffer pointer
**  BYTE  dataLength,      IN  Data buffer length
**  BYTE  txOptions,       IN  Transmit option flags
**  VOID_CALLBACKFUNC(completedFunc)( IN  Transmit completed call back function
**    BYTE txStatus)); IN Transmit status
**--------------------------------------------------------------------------*/
#define ZW_SendDataMulti(nodelist,data,length,options,func) ZW_SendDataMulti_Bridge(0xff,nodelist,data,length,options,func)
#define ZW_SEND_DATA_MULTI(nodelist,data,length,options,func) ZW_SendDataMulti_Bridge(0xff,nodelist,data,length,options,func)
#define ZW_SEND_DATA_MULTI_BRIDGE(srcId,nodelist,data,length,options,func) ZW_SendDataMulti_Bridge(srcId,nodelist,data,length,options,func)

#endif  /* !ZW_CONTROLLER_BRIDGE */

#endif  /* ZW_CONTROLLER || ZW_SLAVE_ROUTING */


/*============================   ZW_SEND_CONST =============================
**    Function description
**      If production test is enabled during start up.
**      Calling this function will transmit a constant signal until a new
**      RF function is called
**
**--------------------------------------------------------------------------*/
#define ZW_SEND_CONST() ZW_SendConst(TRUE, 1, ZW_RF_TEST_SIGNAL_CARRIER)


#ifdef ZW_SLAVE
/*============================ ZW_LOCK_RESPONSE_ROUTE ========================
**    Function description
**      This function locks and unlocks all return routes
**      IN  nodeID  != 0x00 lock route to node
**          nodeDI == 0x00 unlock entry
**    Side effects:
**
**--------------------------------------------------------------------------*/
#define ZW_LOCK_RESPONSE_ROUTE(node) ZW_LockRoute(node)

#else

/*============================ ZW_LOCK_RESPONSE_ROUTE ========================
**    Function description
**      This function locks and unlocks all Last Working Routes for purging
**      IN  bLockRoute == TRUE lock all Last Working Routes - no purging allowed
**          bLockRoute == FALSE unlock purging of Last Working Routes
**
**    Side effects:
**
**--------------------------------------------------------------------------*/
#define ZW_LOCK_RESPONSE_ROUTE(bLockRoute) ZW_LockRoute(bLockRoute)
#define ZW_LOCK_ROUTE(bLockRoute) ZW_LockRoute(bLockRoute)

#endif  /* ZW_SLAVE */


#if defined(ZW_CONTROLLER) || defined(ZW_SLAVE_ROUTING)
#ifndef ZW_CONTROLLER_BRIDGE
/*===============================   ZW_SEND_DATA_META ==========================
**    Transmit Meta data buffer to a single Z-Wave node or all Z-Wave nodes
**    Only allowed for 40kbit nodes. If a 9600 destination is selected FALSE will
**    be returned. Parameters and return values identical with ZW_SEND_DATA
**    Side effects:
**
**--------------------------------------------------------------------------*/
#define ZW_SEND_DATA_META(destId,data,len,txOptions,func) ZW_SendDataMeta(destId,data,len,txOptions,func)

#else

/*==========================   ZW_SEND_DATA_META_BRIDGE ======================
**    Transmit Meta data buffer to a single Z-Wave node or all Z-Wave nodes
**    Only allowed for 40kbit nodes. If a 9600 destination is selected FALSE will
**    be returned. Parameters and return values identical with ZW_SEND_DATA
**    Side effects:
**
**--------------------------------------------------------------------------*/
#define ZW_SendDataMeta(destId,data,len,txOptions,func) ZW_SendDataMeta_Bridge(0xff,destId,data,len,txOptions,func)
#define ZW_SEND_DATA_META(destId,data,len,txOptions,func) ZW_SendDataMeta_Bridge(0xff,destId,data,len,txOptions,func)
#define ZW_SEND_DATA_META_BRIDGE(srcId,destId,data,len,txOptions,func) ZW_SendDataMeta_Bridge(srcId,destId,data,len,txOptions,func)

#endif  /* !ZW_CONTROLLER_BRIDGE */
#endif  /* ZW_CONTROLLER || ZW_SLAVE_ROUTING */


/****************************************************************************/
/*                              EXPORTED DATA                               */
/****************************************************************************/

/****************************************************************************/
/*                           EXPORTED FUNCTIONS                             */
/****************************************************************************/

#ifdef ZW_SLAVE
/* TO#2133 fix - Keil compiler >7.50(8.xx) seems to have been changed somehow */
/* in the preprocessor part as if the 2 ZW_LockRoute definitions was ifdefed */
/* by ifdef ZW_SLAVE ... endif and ifdef ZW_CONTROLLER ... endif instead of */
/* ifdef ZW_SLAVE ... else ... endif, the Keil >7.50 reports Warning C235 */
/* on slave/slave routing/slave enhanced targets. */
/*============================   ZW_LockRoute   ==============================
**    Function description
**      This function locks and unlocks any temporary route to a specific nodeID
**    Side effects:
**
**--------------------------------------------------------------------------*/
void
ZW_LockRoute(
  BYTE bNodeID);          /* IN if nonezero lock bNodeID entry, */
                          /*    zero unlock entry */

#else  /* ZW_SLAVE */

/*=============================   ZW_LockRoute   ============================
**    Function description
**      IF bLockRoute TRUE then any attempt to purge a LastWorkingRoute entry
**      is denied.
**
**    Side effects:
**
**
**--------------------------------------------------------------------------*/
void
ZW_LockRoute(
  BOOL bLockRoute);       /* IN TRUE lock LastWorkingRoute entry purging */
                          /*    FALSE unlock LastWorkingRoute entry purging */
#endif  /* ZW_SLAVE */


/**============================   ZW_SendConst  =============================
**    Function description
**      Start/Stop generate RF test signal in a desired channel
**      Signal can be
**                a carrier only
**                a modulated carrier
**
**      Side effects:
**-------------------------------------------------------------------------------------------------*/

void
ZW_SendConst(
              BYTE bStart,  /*IN TRUE start sending RF test signal, FALSE disable RF test signal*/
              BYTE bChNo,   /*IN channle number to send RF test signal on*/
              BYTE bSignalType ); /*IN The RF test signal type.*/

#ifndef ZW_CONTROLLER_BRIDGE
/*===============================   ZW_SendData   ===========================
**    Transmit data buffer to a single ZW-node or all ZW-nodes (broadcast).
**
**
**    txOptions:
**          TRANSMIT_OPTION_LOW_POWER     transmit at low output power level
**                                        (1/3 of normal RF range).
**          TRANSMIT_OPTION_ACK           the multicast frame will be followed
**                                        by a  singlecast frame to each of
**                                        the destination nodes
**                                        and request acknowledge from each
**                                        destination node.
**          TRANSMIT_OPTION_AUTO_ROUTE    request retransmission via repeater
**                                        nodes at normal output power level).
**          TRANSMIT_OPTION_RETURN_ROUTE  transmission via return route, if
**                                        no return route is registred then
**                                        the frame will be send as a
**                                        broadcast.
**          TRANSMIT_OPTION_EXPLORE     Use explore frame route resolution if all else fails
**
** extern BYTE            RET  FALSE if transmitter queue overflow
** ZW_SendData(
** BYTE  destNodeID,      IN  Destination node ID (0xFF == broadcast)
** BYTE *pData,           IN  Data buffer pointer
** BYTE  dataLength,      IN  Data buffer length
** BYTE  txOptions,       IN  Transmit option flags
** VOID_CALLBACKFUNC(completedFunc)( IN  Transmit completed call back function
**        BYTE txStatus));    IN Transmit status
**--------------------------------------------------------------------------*/
extern BYTE            /*RET  FALSE if transmitter busy      */
ZW_SendData(
  BYTE  destNodeID,           /*IN  Destination node ID (0xFF == broadcast) */
  BYTE *pData,                /*IN  Data buffer pointer           */
  BYTE  dataLength,           /*IN  Data buffer length            */
  BYTE  txOptions,            /*IN  Transmit option flags         */
  VOID_CALLBACKFUNC(completedFunc)(BYTE, TX_STATUS_TYPE*)); /*IN  Transmit completed call back function  */

#else

/*============================   ZW_SendData_Bridge   ========================
**    Transmit data buffer to a single ZW-node or all ZW-nodes (broadcast).
**
**
**    txOptions:
**          TRANSMIT_OPTION_LOW_POWER     transmit at low output power level
**                                        (1/3 of normal RF range).
**          TRANSMIT_OPTION_ACK           the multicast frame will be followed
**                                        by a  singlecast frame to each of
**                                        the destination nodes
**                                        and request acknowledge from each
**                                        destination node.
**          TRANSMIT_OPTION_AUTO_ROUTE    request retransmission via repeater
**                                        nodes at normal output power level).
**          TRANSMIT_OPTION_RETURN_ROUTE  transmission via return route, if
**                                        no return route is registred then
**                                        the frame will be send as a
**                                        broadcast.
**
** extern BYTE            RET FALSE if transmitter queue overflow
** ZW_SendData_Bridge(
**  BYTE  bSrcNodeID,      IN Source NodeID - if 0xFF then controller ID is set as source
**  BYTE  nodeID,          IN Destination node ID (0xFF == broadcast)
**  BYTE *pData,           IN Data buffer pointer
**  BYTE  dataLength,      IN Data buffer length
**  BYTE  txOptions,       IN Transmit option flags
**  VOID_CALLBACKFUNC(completedFunc)( IN Transmit completed call back function
**         BYTE txStatus));    IN Transmit status
**--------------------------------------------------------------------------*/
extern BYTE                   /*RET FALSE if transmitter busy      */
ZW_SendData_Bridge(
  BYTE  bSrcNodeID,           /*IN  Source NodeID - if 0xFF then controller ID is set as source */
  BYTE  nodeID,               /*IN  Destination node ID (0xFF == broadcast) */
  BYTE *pData,                /*IN  Data buffer pointer           */
  BYTE  dataLength,           /*IN  Data buffer length            */
  BYTE  txOptions,            /*IN  Transmit option flags         */
  VOID_CALLBACKFUNC(completedFunc)(BYTE, TX_STATUS_TYPE*)); /*IN  Transmit completed call back function  */

#endif  /* !ZW_CONTROLLER_BRIDGE */

#ifdef ZW_SLAVE
/*===============================   ZW_SendDataEx   ===========================
**    Transmit data buffer to a single Z-Wave node or all Z-Wave nodes - broadcast.
**    Extended version with extra transmit options, specifically security in
**    the protocol.
**
**    Side effects:
**
**--------------------------------------------------------------------------*/
enum ZW_SENDDATA_EX_RETURN_CODES            /*RET Return code      */
ZW_SendDataEx(
  BYTE *pData,                              /* IN Data buffer pointer           */
  BYTE  dataLength,                         /* IN Data buffer length            */
  TRANSMIT_OPTIONS_TYPE *txOptEx,           /* IN Transmit options              */
  VOID_CALLBACKFUNC(completedFunc)(BYTE));  /* IN Transmit completed call back function */

/*============================   ZW_RxToTxOptions   ========================
**    Convert rx options to tx options suitable for ZW_SendDataEx.
**
**    Side effects:
**
**--------------------------------------------------------------------------*/
void ZW_RxToTxOptions(
    RECEIVE_OPTIONS_TYPE *rxopt,     /* IN  receive options to convert */
    TRANSMIT_OPTIONS_TYPE *txopt);   /* OUT converted transmit options */
#endif

#if defined(ZW_CONTROLLER) || defined(ZW_SLAVE_ROUTING)
#ifndef ZW_CONTROLLER_BRIDGE
/*===============================   ZW_SendDataMeta ==========================
**    Transmit Meta data buffer to a single Z-Wave node or all Z-Wave nodes
**    Only allowed for 40kbit nodes. If a 9600 destination is selected FALSE will
**    be returned. Parameters and return values identical with ZW_SEND_DATA
**    DOES NOT EXIST ON ZW0x0x targets
**    Side effects:
**
**--------------------------------------------------------------------------*/

BYTE ZW_SendDataMeta(  BYTE  destNodeID, /* IN Destination node ID - 0xFF == all nodes */
  BYTE *pData,      /* IN Data buffer pointer           */
  BYTE  dataLength, /* IN Data buffer length            */
  BYTE  txOptions,  /* IN Transmit option flags         */
  VOID_CALLBACKFUNC(completedFunc)(BYTE, TX_STATUS_TYPE*)); /* IN Transmit completed call back function  */

#else

/*===============================   ZW_SendDataMeta_Bridge ===================
**    Transmit Meta data buffer to a single Z-Wave node or all Z-Wave nodes
**    Only allowed for 40kbit nodes. If a 9600 destination is selected FALSE will
**    be returned. Parameters and return values identical with ZW_SEND_DATA
**    Side effects:
**
**--------------------------------------------------------------------------*/
BYTE
ZW_SendDataMeta_Bridge(
  BYTE  bSrcNodeID, /* IN  Source node ID - if 0xFF then controller ID is source */
  BYTE  destNodeID, /* IN Destination node ID - 0xFF == all nodes */
  BYTE *pData,      /* IN Data buffer pointer           */
  BYTE  dataLength, /* IN Data buffer length            */
  BYTE  txOptions,  /* IN Transmit option flags         */
  VOID_CALLBACKFUNC(completedFunc)(BYTE)); /* IN Transmit completed call back function  */

#endif  /* !ZW_CONTROLLER_BRIDGE */


/*============================   ZW_SendDataAbort   ========================
**    Abort the ongoing transmit started with ZW_SendData()
**
**    Side effects:
**
**--------------------------------------------------------------------------*/
void                /*RET FALSE if transmitter busy      */
ZW_SendDataAbort(void);


#ifndef ZW_CONTROLLER_BRIDGE
/*===============================   ZW_SendDataMulti   ======================
**    Transmit data buffer to a list of Z-Wave Nodes (multicast frame).
**
**
**    txOptions:
**          TRANSMIT_OPTION_LOW_POWER   transmit at low output power level (1/3 of
**                                      normal RF range).
**          TRANSMIT_OPTION_ACK         the multicast frame will be followed by a
**                                      singlecast frame to each of the destination nodes
**                                      and request acknowledge from each destination node.
**          TRANSMIT_OPTION_AUTO_ROUTE  request retransmission on singlecast frames
**                                      via repeater nodes (at normal output power level).
**
**--------------------------------------------------------------------------*/
extern BYTE            /*RET  FALSE if transmitter busy      */
ZW_SendDataMulti(
  BYTE *pNodeIDList,          /*IN  List of destination node ID's */
  BYTE *pData,                /*IN  Data buffer pointer           */
  BYTE  dataLength,           /*IN  Data buffer length            */
  BYTE  txOptions,            /*IN  Transmit option flags         */
  VOID_CALLBACKFUNC(completedFunc)(BYTE)); /*IN  Transmit completed call back function  */

#else

/*===============================   ZW_SendDataMulti_Bridge   ================
**    Transmit data buffer to a list of Z-Wave Nodes (multicast frame).
**
**
**    txOptions:
**          TRANSMIT_OPTION_LOW_POWER   transmit at low output power level (1/3 of
**                                      normal RF range).
**          TRANSMIT_OPTION_ACK         the multicast frame will be followed by a
**                                      singlecast frame to each of the destination nodes
**                                      and request acknowledge from each destination node.
**          TRANSMIT_OPTION_AUTO_ROUTE  request retransmission on singlecast frames
**                                      via repeater nodes (at normal output power level).
**
**--------------------------------------------------------------------------*/
extern BYTE            /*RET  FALSE if transmitter busy      */
ZW_SendDataMulti_Bridge(
  BYTE bSrcNodeID,     /*IN Source nodeID - if 0xFF then controller is set as source */
  BYTE *pNodeIDList,   /*IN List of destination node ID's */
  BYTE *pData,         /*IN Data buffer pointer           */
  BYTE  dataLength,    /*IN Data buffer length            */
  BYTE  txOptions,     /*IN Transmit option flags         */
  VOID_CALLBACKFUNC(completedFunc)(BYTE)); /*IN  Transmit completed call back function  */
#endif  /* !ZW_CONTROLLER_BRIDGE */

#endif /* ZW_CONTROLLER || ZW_SLAVE_ROUTING */


#if defined(ZW_CONTROLLER) && !defined(ZW_CONTROLLER_STATIC) && !defined(ZW_CONTROLLER_BRIDGE)
/*===============================   ZW_GetNodeIDMaskList   ======================
**    Get the recieved multicast frame's destinations nodes mask list
**    Side effects:
**            The API should only be called before returning from ApplicationCommandHandler
**------------------------------------------------------------------------------------------------------*/
BYTE                            /*RET: nodeID mask offset*/
ZW_GetNodeIDMaskList(
  BYTE **pNodeIDMaskList,      /* OUT destinations nodes mask list*/
  BYTE  *pMaskListLen);        /* OUTthe destinations nodes mask list length*/

#endif

/*=======================   ZW_SetListenBeforeTalkThreshold   =================
**    Set the threshold that should be added to the standard -75dBm used
**    in JP listen before talk.
**
**    Side effects:
**
**--------------------------------------------------------------------------*/
void                              /*RET: Nothing */
ZW_SetListenBeforeTalkThreshold(
  BYTE bChannel,                  /*IN: RF channel to set the threshold for */
  BYTE bThreshold);               /*IN: Threshold to be added to RSSI limit */

#endif /* _ZW_TRANSPORT_API_H_ */

