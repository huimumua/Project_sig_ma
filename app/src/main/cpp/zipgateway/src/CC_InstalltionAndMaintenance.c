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


/**
 * SDS12954
 *
 * Z/IP Gateway Installation and Maintenance Framework
 *
 */


#include "sys/clock.h"
#include "CC_InstalltionAndMaintenance.h"
#include "Serialapi.h"
#include "ZIP_Router.h"
#include "ResourceDirectory.h"
#include "command_handler.h"

#define ZWAVE_NODEINFO_BAUD_9600                0x08
#define ZWAVE_NODEINFO_BAUD_40000               0x10

/* Speed extension in nodeinfo reserved field */
#define ZWAVE_NODEINFO_BAUD_100K                 0x01
//#define ZWAVE_NODEINFO_BAUD_200K                 0x02 /* Tentative */


static clock_time_t transmit_start_time;

struct node_ima ima_database[ZW_MAX_NODES];

#define BIT8_TST(bit,array) ((array[(bit)>>3] >> ((bit) & 7)) & 0x1)

u16_t tApp =0;


/**
 * RSSI – The Received Signal Strength Indication for the transmitted frame. If frame is routed,
 * NOTE that this will only contain the RSSI between the Z/IP GW and the first hop
 */
u8_t ima_last_snr;

/**
 * Transmission Time (TT) – Time from SendData() return to callback is received, in ms
 */
u16_t ima_last_tt;

/**
 * Indicates if the route was changed on the last transmission
 */
u8_t ima_was_route_changed;

void get_last_tx_stats(u16_t* time, u8_t* rc, u8_t* rssi ) {
  *time = ima_last_tt;
  *rc = ima_was_route_changed;
  *rssi = ima_last_snr;
}


const struct node_ima* ima_get_entry(u8_t node) {
  return &ima_database[node-1];
}

/**
 * Called when just before senddata is issued
 */
void ima_send_data_start(u8_t node) {
  transmit_start_time = clock_time();
}


/*
 * Called when send_data is completed
 */
void ima_send_data_done(u8_t node,u8_t status) {
  struct node_ima* ima;
  struct route lwr;

  ASSERT(node>0);

  ima = &ima_database[node-1];

  ima->tc++;

  if(status == TRANSMIT_COMPLETE_NO_ACK) {
    ima_was_route_changed = FALSE;
    ima->pec++;
    return;
  }
  ima_last_tt = clock_time() - transmit_start_time;
  if(ima_last_tt > tApp) {
    ima_last_tt = ima_last_tt - tApp;
  }

  ima_last_snr = 0; /*TODO*/

  /*Time statistics */
  ima->mts += ima_last_tt;
  ima->mts2 += ima_last_tt*ima_last_tt;

  if ( ZW_GetLastWorkingRoute(node,(BYTE*)&lwr) && (
  (ima->lwr.repeaters[0] != lwr.repeaters[0]) ||
  (ima->lwr.repeaters[1] != lwr.repeaters[1]) ||
  (ima->lwr.repeaters[2] != lwr.repeaters[2]) ||
  (ima->lwr.repeaters[3] != lwr.repeaters[3]) ||
  (ima->lwr.speed != lwr.speed) )
  ) {
    ima->lwr = lwr;
    DBG_PRINTF("Route change\n");
    ima_was_route_changed =TRUE;
  } else if( (ima_last_tt  > (ima->last_tt  + 150)) && (rd_get_node_mode(node) ==MODE_ALWAYSLISTENING) ) {
    /* To be able to detect if route has gone from raoute A to B and back to A, we to a timer based calculation.
     * We only do timer based route changes if the node is allways listening node.*/
    DBG_PRINTF("Route change based on timer\n");
    ima_was_route_changed = TRUE;
  } else {
    ima_was_route_changed = FALSE;
  }

  if(ima_was_route_changed) {
    ima->rc++;
  }

  ima->last_tt = ima_last_tt;
}

void ima_reset() {
  int n;
  struct route lwr;
  memset(ima_database,0, sizeof(ima_database));

  for(n=1; n< ZW_MAX_NODES; n++) {
    if(rd_node_exists(n)) {
      if(ZW_GetLastWorkingRoute(n, (BYTE*)&lwr ) ) {
        memcpy(&ima_database[n-1].lwr,&lwr,5);
      }
    }
  }
}


static command_handler_codes_t ima_CommandHandler(zwave_connection_t *c, BYTE* pData, WORD bDatalen) {
  int i;
  BYTE routeType;

  switch(pData[1]) {
    case PRIORITY_ROUTE_SET:
      if(bDatalen >=8) {
      ZW_SetLastWorkingRoute(pData[2],&pData[3]);
      }
      break;
    case PRIORITY_ROUTE_GET:

      pData[1] = PRIORITY_ROUTE_REPORT;
//      pData[2] = pData[2] nodeID;
      routeType = ZW_GetPriorityRoute(pData[2],&pData[4]);
      pData[3] = routeType;
      ZW_SendDataZIP(c,pData,6+3,0);
      break;
    case STATISTICS_GET:
    {
      u8_t *p = pData;
      const struct node_ima* ima = ima_get_entry(pData[2]);

      *p++ = COMMAND_CLASS_NETWORK_MANAGEMENT_INSTALLATION_MAINTENANCE;
      *p++ = STATISTICS_REPORT;
      *p++ = pData[2]; //NodeID
      *p++ = TRANSMISSION_COUNT;
      *p++ = 1;
      *p++ = ima->tc;

      *p++ = PACKET_ERROR_COUNT;
      *p++ = 1;
      *p++ = ima->pec;

      *p++ = ROUTE_CHANGES;
      *p++ = 1;
      *p++ = ima->rc;

      *p++ = NEIGHBORS;

      {
        NODEINFO ni;
        u8_t nb[ZW_MAX_NODES / 8];
        u8_t *length;

        *p=0;
        length = p;
        p++;

        ZW_GetRoutingInfo_old(pData[2], nb, FALSE, FALSE);

        for (i = 0; i < ZW_MAX_NODES; i++)
        {
          if (rd_node_exists(i + 1) && BIT8_TST(i, nb))
          {
            *length += 2;
            ZW_GetNodeProtocolInfo(i + 1, &ni);

            *p++ = i + 1; //NodeID

#if 0
            if (ni.reserved & ZWAVE_NODEINFO_BAUD_200K)
            {
              *p |= IMA_NODE_SPEED_200;
            } else 
#endif
            *p = 0;
            if (ni.reserved & ZWAVE_NODEINFO_BAUD_100K)
            {
              /* ni.reserved & ZWAVE_NODEINFO_BAUD_100K reports 100K capability
               * in addition to whatever ni.capability reports below */
              *p |= IMA_NODE_SPEED_100;
            }
            if (ni.capability & ZWAVE_NODEINFO_BAUD_40000)
            {
              /* ni.capability sets either ZWAVE_NODEINFO_BAUD_40000 or ZWAVE_NODEINFO_BAUD_96
               * depending on the maximum below-100K speed of the node. It will not set both.
               * We know that all 40K nodes also support 9.6K, so we set that bit also */
              *p |= IMA_NODE_SPEED_40 | IMA_NODE_SPEED_96;
            }
            else if (ni.capability & ZWAVE_NODEINFO_BAUD_9600)
            {
              *p |= IMA_NODE_SPEED_96;
            }

            if ((ni.capability & NODEINFO_ROUTING_SUPPORT) && (ni.capability & NODEINFO_LISTENING_SUPPORT) ) {
              *p |= IMA_NODE_REPEATER; //Is repeater
            }
            p++;
          }
        }
      }

      *p++ = TANSMISSION_TIME_SUM;
      *p++ = 4;
      *p++ = (ima->mts>>24) & 0xFF;
      *p++ = (ima->mts>>16) & 0xFF;
      *p++ = (ima->mts>> 8) & 0xFF;
      *p++ = (ima->mts>> 0) & 0xFF;

      *p++ = TANSMISSION_TIME_SUM2;
      *p++ = 4;
      *p++ = (ima->mts2>>24) & 0xFF;
      *p++ = (ima->mts2>>16) & 0xFF;
      *p++ = (ima->mts2>> 8) & 0xFF;
      *p++ = (ima->mts2>> 0) & 0xFF;

      ZW_SendDataZIP(c,pData,(u8_t)(p - pData),0);
    }
    break;
    case RSSI_GET:
    {
      uint8_t rssi_buffer[20];
      unsigned int i=0;
      uint8_t *r;
      uint8_t num_channels; /* Number of channels used by ZW chip */
      r=rssi_buffer;

      /* Fixme: Add real background rssi measurements instead of this hardcoded response */
      r[0] = COMMAND_CLASS_NETWORK_MANAGEMENT_INSTALLATION_MAINTENANCE;
      r[1] = RSSI_REPORT;
      ZW_GetBackgroundRSSI(&r[2], &num_channels);
      if (num_channels == 2)
      {
        r[4] = 0x7F;   /* Spec SDS13784-4 requires us to insert the 3rd channel even if unavailable
                          it is a shame, since the ZIP Client could otherwise detect if this is a 2CH
                          or 3CH system. */
      }

      ZW_SendDataZIP(c, rssi_buffer, 5, 0);
    }
    break;


    case STATISTICS_CLEAR:
      ima_reset();
      break;
    default:
      return COMMAND_NOT_SUPPORTED;
      break;
  }
  return COMMAND_HANDLED;
}

void ima_init() {
  clock_time_t t0;
  //Benchmark the SerialAPI for a short frame;
  t0 = clock_time();
  ZW_Type_Library();
  tApp = clock_time()- t0;
  DBG_PRINTF("Resetting IMA\n");
  ima_reset();
}



REGISTER_HANDLER(
    ima_CommandHandler,
    ima_init,
    COMMAND_CLASS_NETWORK_MANAGEMENT_INSTALLATION_MAINTENANCE, NETWORK_MANAGEMENT_INSTALLATION_MAINTENANCE_VERSION, SECURITY_SCHEME_0);
