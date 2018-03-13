/* © 2017 Sigma Designs, Inc. This is an unpublished work protected by Sigma
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

#include "ResourceDirectory.h"
#include "RD_internal.h"
#include "RD_DataStore.h"
#include "TYPES.H"
#include "ZW_transport_api.h"
#include "ZIP_Router_logging.h"

#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

/* TODO-km: fix this when including ZIP_Router.h does not drag in the entire gateway. */
extern uint8_t MyNodeID;
extern uint32_t homeID;


/* This eats memory ! */
/**
 * \ingroup ZIP_Resource
 *
 * The node database.
 */
static rd_node_database_entry_t* ndb[ZW_MAX_NODES];

rd_node_database_entry_t* rd_node_entry_alloc(uint8_t nodeid)
{
   rd_node_database_entry_t* nd = rd_data_mem_alloc(sizeof(rd_node_database_entry_t));
   ndb[nodeid - 1] = nd;
   return nd;
}

rd_node_database_entry_t* rd_node_entry_import(uint8_t nodeid)
{
   ndb[nodeid-1] = rd_data_store_read(nodeid);
   return ndb[nodeid-1];
}

void rd_node_entry_free(uint8_t nodeid)
{
   rd_node_database_entry_t* nd = ndb[nodeid - 1];
   rd_data_store_nvm_free(nd);
   rd_data_store_mem_free(nd);
   ndb[nodeid - 1] = NULL;
}

rd_node_database_entry_t* rd_node_get_raw(uint8_t nodeid)
{
   return ndb[nodeid - 1];
}


void
rd_destroy()
{
  u8_t i;
  rd_node_database_entry_t *n;
  for (i = 0; i < ZW_MAX_NODES; i++)
  {
    n = ndb[i];
    if (n)
    {
      rd_data_store_mem_free(n);
      ndb[i] = 0;
    }
  }
}

u8_t rd_node_exists(u8_t node)
{
  if (node > 0 && node <= ZW_MAX_NODES)
  {
    return (ndb[node - 1] != 0);
  }
  return FALSE;
}

rd_ep_database_entry_t* rd_ep_first(uint8_t node)
{
  int i;

  if (node == 0)
  {
    for (i = 0; i < ZW_MAX_NODES; i++)
    {
      if (ndb[i])
      {
        return list_head(ndb[i]->endpoints);
      }
    }
  }
  else if (ndb[node - 1])
  {
    return list_head(ndb[node - 1]->endpoints);
  }
  return 0;
}

rd_ep_database_entry_t* rd_ep_next(uint8_t node, rd_ep_database_entry_t* ep)
{
  uint8_t i;
  rd_ep_database_entry_t* next = list_item_next(ep);

  if (next == 0 && node == 0)
  {
    for (i = ep->node->nodeid; i < ZW_MAX_NODES; i++)
    {
      if (ndb[i])
      {
        return list_head(ndb[i]->endpoints);
      }
    }
  }
  return next;
}

u8_t
rd_get_node_name(rd_node_database_entry_t* n, char* buf, u8_t size)
{
  if (n->nodeNameLen)
  {
    if (size > n->nodeNameLen)
    {
      size = n->nodeNameLen;
    }
    memcpy(buf, n->nodename, size);
    return size;
  }
  else
  {
    return snprintf(buf, size, "zw%08X%02X", UIP_HTONL(homeID), n->nodeid);
  }
}

rd_node_database_entry_t* rd_lookup_by_node_name(const char* name)
{
  uint8_t i, j;
  char buf[64];
  for (i = 0; i < ZW_MAX_NODES; i++)
  {
    if (ndb[i])
    {
      j = rd_get_node_name(ndb[i], buf, sizeof(buf));
      if (strncasecmp(buf,name, j) == 0)
      {
        return ndb[i];
      }
    }
  }
  return 0;
}


void rd_node_add_dsk(u8_t node, uint8_t dsklen, uint8_t *dsk)
{
   rd_node_database_entry_t *nd;

   if ((dsklen == 0) || (dsk == NULL)) {
      return;
   }

   nd = rd_node_get_raw(node);
   if (nd) {
      if (nd->dskLen != 0) {
         /* TODO: this is not supposed to happen - replace DSK is not supported */
         assert(nd->dskLen == 0);
         if (nd->dsk != NULL) {
            WRN_PRINTF("Replacing old dsk\n");
            /* Silently replace, for now */
            free(nd->dsk);
            nd->dskLen = 0;
         }
      }

      nd->dsk = (uint8_t*) malloc(dsklen);
      if (nd->dsk) {
         memcpy(nd->dsk, dsk, dsklen);
         nd->dskLen = dsklen;
         DBG_PRINTF("Setting dsk 0x%02x%02x%02x%02x... on node %u.\n",
                    dsk[0],dsk[1],dsk[2],dsk[3],node);
      } else {
         /* TODO: should we return an error here. */
         nd->dskLen = 0;
      }
   }
   /* TODO: should we return an error if no nd?. */
}

rd_node_database_entry_t* rd_lookup_by_dsk(uint8_t dsklen, const uint8_t* dsk)
{
   uint8_t ii;

   if (dsklen == 0) {
      return NULL;
   }

   for (ii = 0; ii < ZW_MAX_NODES; ii++)
   {
      if (ndb[ii] && (ndb[ii]->dskLen >= dsklen))
      {
         if (memcmp(ndb[ii]->dsk, dsk, dsklen) == 0)
         {
            return ndb[ii];
         }
      }
   }
   return NULL;
}

rd_node_database_entry_t* rd_get_node_dbe(uint8_t nodeid)
{
  //ASSERT(nodeid>0);
  if (nodeid == 0)
    return 0;
  if (ndb[nodeid - 1])
    ndb[nodeid - 1]->refCnt++;
  return ndb[nodeid - 1];
}
