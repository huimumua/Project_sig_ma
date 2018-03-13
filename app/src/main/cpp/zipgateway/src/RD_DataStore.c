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


#include "smalloc.h"
#include "ResourceDirectory.h"
//#include "ZIP_Router.h"
#include "ZIP_Router_logging.h"
#include "Serialapi.h"
#include "dev/eeprom.h"
#include "Bridge.h"
#include "DataStore.h"
#include "ZW_typedefs.h"

#include "assert.h"

#undef DBG_PRINTF
#define DBG_PRINTF(a,...)

/* TODO-km: fix this when including ZIP_Router.h does not drag in the entire gateway. */
extern uint8_t MyNodeID;
extern uint32_t homeID;

/** Obsoleted eeprom.dat magic number.  This magic indicates that the
 * hdr struct is unversioned.  The gateway will assume v2.61 format
 * and try to convert the data. */
#define RD_MAGIC_V0 0x491F00E5

/** The eeprom.dat magic number.  This magic indicates that the hdr struct
 * includes a versioning field. */
#define RD_MAGIC_V1 0x94F100E5 

#include "mDNSService.h"
uint16_t rd_eeprom_read(uint16_t offset,uint8_t len,void* data) {
  //DBG_PRINTF("Reading at %x\n", 0x100 + offset);
  eeprom_read(0x40 + offset,data,len);
  return len;
}

uint16_t rd_eeprom_write(uint16_t offset,uint8_t len,void* data) {
  //DBG_PRINTF("Writing at %x\n", 0x100 + offset);
  if(len) {
    eeprom_write(0x40 + offset,data,len);
  }
  return len;
}

static const small_memory_device_t nvm_dev  = {
  .offset = offsetof(rd_eeprom_static_hdr_t, smalloc_space),
  .size = sizeof(((rd_eeprom_static_hdr_t*)0)->smalloc_space),
  .psize = 8,
  .read = rd_eeprom_read,
  .write = rd_eeprom_write,
};

#ifdef SMALL_MEMORY_TARGET
static char rd_membuf[0x1000];

static uint16_t rd_mem_read(uint16_t offset,uint8_t len,void* data) {
  memcpy(data,rd_membuf + offset, len);
  return len;
}


static uint16_t rd_mem_write(uint16_t offset,uint8_t len,void* data) {
  memcpy(rd_membuf + offset,data, len);
  return len;
}


const small_memory_device_t rd_mem_dev  = {
  .offset = 1,
  .size = sizeof(rd_membuf),
  .psize = 8,
  .read = rd_mem_read,
  .write = rd_mem_write,
};

void* rd_data_mem_alloc(uint8_t size) {
  uint16_t p;
  if(size ==0) {
    return 0;
  }
  p = smalloc(&rd_mem_dev,size);
  if(p) {
    return rd_membuf + p;
  } else {
    ERR_PRINTF("Out of memory\n");
    return 0;
  }
}

void rd_data_mem_free(void* p) {
  smfree(&rd_mem_dev,(char*)p - rd_membuf);
}
#else
#include <stdlib.h>
void* rd_data_mem_alloc(uint8_t size) {
  return malloc(size);
}
void rd_data_mem_free(void* p) {
  free(p);
}

#endif



void rd_store_mem_free_ep(rd_ep_database_entry_t* ep) {
  if(ep->endpoint_info) rd_data_mem_free(ep->endpoint_info);
  if(ep->endpoint_name) rd_data_mem_free(ep->endpoint_name);
  if(ep->endpoint_location) rd_data_mem_free(ep->endpoint_location);
  rd_data_mem_free(ep);
}

static rd_ep_database_entry_t* rd_data_store_read_ep(uint16_t ptr, rd_node_database_entry_t* n) {
  rd_ep_data_store_entry_t e;
  rd_ep_database_entry_t* ep;

  rd_eeprom_read(ptr,sizeof(rd_ep_data_store_entry_t),&e);

  ep = rd_data_mem_alloc(sizeof(rd_ep_database_entry_t));
  if(!ep) {
    return 0;
  }
  memcpy(&ep->endpoint_info_len, &e, sizeof(rd_ep_data_store_entry_t));

  /*Setup pointers*/
  ep->endpoint_info = rd_data_mem_alloc(e.endpoint_info_len);
  ep->endpoint_name = rd_data_mem_alloc(e.endpoint_name_len);
  ep->endpoint_location = rd_data_mem_alloc(e.endpoint_loc_len);
  ep->endpoint_agg =  rd_data_mem_alloc(e.endpoint_aggr_len);

  if(
      (e.endpoint_info_len && ep->endpoint_info==0) ||
      (e.endpoint_name_len && ep->endpoint_name==0) ||
      (e.endpoint_loc_len && ep->endpoint_location==0) ||
      (e.endpoint_aggr_len && ep->endpoint_agg==0)
    ) {
    rd_store_mem_free_ep(ep);
    return 0;
  }

  ptr += sizeof(rd_ep_data_store_entry_t);
  rd_eeprom_read(ptr,ep->endpoint_info_len,ep->endpoint_info);
  ptr += ep->endpoint_info_len;
  rd_eeprom_read(ptr,ep->endpoint_name_len,ep->endpoint_name);
  ptr += ep->endpoint_name_len;
  rd_eeprom_read(ptr,ep->endpoint_loc_len,ep->endpoint_location);

  ptr += ep->endpoint_loc_len;
  rd_eeprom_read(ptr,ep->endpoint_aggr_len,ep->endpoint_agg);

  ep->node = n;
  return ep;
}
/*
 * Data store layout
 *
 * global_hdr | node_ptr | node_ptr | node_ptr | .... | node_ptr
 *
 * node_hdr | node_name | ep_ptr | ep_ptr | .... | ep_ptr
 *
 * ep_hdr | info | name | location
 *
 * */

rd_node_database_entry_t* rd_data_store_read(uint8_t nodeID) {
  rd_node_database_entry_t *r;
  rd_ep_database_entry_t *ep;
  uint16_t n_ptr,e_ptr;
  uint8_t i;

  /*Size of static content of */
  const uint16_t static_size = offsetof(rd_node_database_entry_t,nodename);

  n_ptr = 0;
  rd_eeprom_read(offsetof(rd_eeprom_static_hdr_t,node_ptrs[nodeID]),sizeof(uint16_t),&n_ptr);

  if(n_ptr==0) {
    DBG_PRINTF("Node %i node not eeprom\n",nodeID);
    return 0;
  }


  r = rd_data_mem_alloc(sizeof(rd_node_database_entry_t));
  if(r==0) {
    ERR_PRINTF("Out of memory\n");
    return 0;
  }
  memset(r,0,sizeof(rd_node_database_entry_t));

  /*Read the first part of the node entry*/
  rd_eeprom_read(n_ptr, static_size,r );

  /*Init the endpoint list */
  LIST_STRUCT_INIT(r,endpoints);

  /*Read the node name*/
  r->nodename =  rd_data_mem_alloc(r->nodeNameLen);
  rd_eeprom_read(n_ptr+static_size, r->nodeNameLen ,r->nodename );

  r->dsk =  rd_data_mem_alloc(r->dskLen);
  rd_eeprom_read(n_ptr+static_size+r->nodeNameLen, r->dskLen, r->dsk);

  n_ptr += static_size + r->nodeNameLen + r->dskLen;
  for(i=0; i < r->nEndpoints; i++) {
    rd_eeprom_read(n_ptr,sizeof(uint16_t),&e_ptr);

    ep =rd_data_store_read_ep(e_ptr,r);
    DBG_PRINTF("EP alloc %p\n",ep);
    assert(ep);
    list_add(r->endpoints,ep);
    n_ptr+=sizeof(uint16_t);
  }

  return r;
}


/**
 * Write a rd_node_database_entry_t and all its endpoints to storage.
 * This is called when a new node is added.
 */
void rd_data_store_nvm_write(rd_node_database_entry_t *n) {
  uint16_t ptr,e_ptr;
  uint8_t i;
  rd_ep_database_entry_t* ep;

  /*Size of static content of */
  const uint16_t static_size = offsetof(rd_node_database_entry_t,nodename);

  if (n->nEndpoints != list_length(n->endpoints)) {
    ERR_PRINTF("n->nEndpoints:%d\n", n->nEndpoints);
    ERR_PRINTF("list_length(n->endpoints):%d\n", list_length(n->endpoints));
    assert(0);
    DBG_PRINTF("Correcting number of endpoints\n");
    n->nEndpoints = list_length(n->endpoints);
  }

  rd_eeprom_read(offsetof(rd_eeprom_static_hdr_t,node_ptrs[n->nodeid]),sizeof(uint16_t),&ptr);
  assert(ptr==0);

  ptr = smalloc(&nvm_dev, static_size + n->nodeNameLen + n->dskLen + n->nEndpoints*sizeof(uint16_t));
  if(ptr==0) {
    ERR_PRINTF("EEPROM is FULL\n");
    return;
  }
  rd_eeprom_write(offsetof(rd_eeprom_static_hdr_t,node_ptrs[n->nodeid]),sizeof(uint16_t),&ptr);


  ptr+= rd_eeprom_write(ptr, static_size,n);
  ptr+= rd_eeprom_write(ptr, n->nodeNameLen,n->nodename);
  ptr+= rd_eeprom_write(ptr, n->dskLen, n->dsk);

  ep = list_head(n->endpoints);

  /*Now for the endpoints*/
  for(i=0; i < n->nEndpoints; i++) {
    e_ptr = smalloc(&nvm_dev, sizeof(rd_ep_data_store_entry_t) + ep->endpoint_info_len + ep->endpoint_loc_len + ep->endpoint_name_len
        + ep->endpoint_aggr_len);
    assert(e_ptr > 0);
    /*Write the pointer to the endpoint */
    ptr+= rd_eeprom_write(ptr, sizeof(uint16_t),&e_ptr);

    DBG_PRINTF("++++++ ep %i info  len %i name %s \n",n->nodeid,  ep->endpoint_info_len, ep->endpoint_name);
    /*Write the endpoint structure */
    e_ptr+= rd_eeprom_write(e_ptr, sizeof(rd_ep_data_store_entry_t),&(ep->endpoint_info_len));
    e_ptr+= rd_eeprom_write(e_ptr, ep->endpoint_info_len,ep->endpoint_info);
    e_ptr+= rd_eeprom_write(e_ptr, ep->endpoint_name_len,ep->endpoint_name);
    e_ptr+= rd_eeprom_write(e_ptr, ep->endpoint_loc_len,ep->endpoint_location);
    e_ptr+= rd_eeprom_write(e_ptr, ep->endpoint_aggr_len,ep->endpoint_agg);

    ep = list_item_next(ep);
  }
}

void rd_data_store_update(rd_node_database_entry_t *n) {
  uint16_t ptr,e_ptr;
  uint8_t i;
  rd_ep_database_entry_t* ep;

  /*Size of static content of */
  const uint16_t static_size = offsetof(rd_node_database_entry_t,nodename);

  if (n->nEndpoints != list_length(n->endpoints)) {
    ERR_PRINTF("n->nEndpoints:%d\n", n->nEndpoints);
    ERR_PRINTF("list_length(n->endpoints):%d\n", list_length(n->endpoints));
    assert(0);
    DBG_PRINTF("Correcting number of endpoints\n");
    n->nEndpoints = list_length(n->endpoints);
  }

  rd_eeprom_read(offsetof(rd_eeprom_static_hdr_t,node_ptrs[n->nodeid]),sizeof(uint16_t),&ptr);
  if(ptr==0) return;

  ptr+= rd_eeprom_write(ptr, static_size,n);
  ptr+= n->nodeNameLen;
  ptr+= n->dskLen;

  ep = list_head(n->endpoints);


  /*Now for the endpoints*/
  for(i=0; i < n->nEndpoints; i++) {
    ptr+= rd_eeprom_read(ptr, sizeof(uint16_t),&e_ptr);
    rd_eeprom_write(e_ptr, sizeof(rd_ep_data_store_entry_t),&(ep->endpoint_info_len));
    ep = list_item_next(ep);
  }

}

void rd_data_store_mem_free(rd_node_database_entry_t *n) {
  rd_ep_database_entry_t *ep;

  while( (ep = list_pop(n->endpoints)) ) {
    DBG_PRINTF("EP free %p\n",ep);

    mdns_remove_from_answers(ep);
    rd_store_mem_free_ep(ep);
  }

  if(n->nodename) rd_data_mem_free(n->nodename);

  rd_data_mem_free(n);
}

void rd_data_store_nvm_free(rd_node_database_entry_t *n) {
  uint16_t n_ptr,e_ptr;
  uint8_t i;

  /*Size of static content of */
  const uint16_t static_size = offsetof(rd_node_database_entry_t,nodename);

  rd_eeprom_read(offsetof(rd_eeprom_static_hdr_t,node_ptrs[n->nodeid]),sizeof(uint16_t),&n_ptr);
  if(!n_ptr) {
 //   WRN_PRINTF("Tried to free a node from NVM which does not exist\n");
    return;
  }
  /*Read the first part of the node entry*/
  smfree(&nvm_dev,n_ptr);

  n_ptr += static_size + n->nodeNameLen + n->dskLen;
  for(i=0; i < n->nEndpoints; i++) {
    n_ptr += rd_eeprom_read(n_ptr,sizeof(uint16_t),&e_ptr);
    smfree(&nvm_dev,e_ptr);
  }

  n_ptr = 0;
  rd_eeprom_write(offsetof(rd_eeprom_static_hdr_t,node_ptrs[n->nodeid]),sizeof(uint16_t),&n_ptr);
}


void data_store_init() {
  /* init: Precondition: SerialAPI up */
  char buf[offsetof(rd_eeprom_static_hdr_t,node_ptrs)];
  uint8_t i;
  uint16_t ptr,q;
  uint8_t maj = 2;
  uint8_t min = 0;
  uint8_t zero = 0;
  uint32_t magic = RD_MAGIC_V1; 
  uint16_t n_ptr;
  rd_eeprom_static_hdr_t* hdr = (rd_eeprom_static_hdr_t*)buf;
  rd_node_database_entry_t r;

  /* Following checks are done to make sure the destination system is 32bit
     as till now we had only 32 bit gateways. This works on i386 and arm. If
     somehow we try to convert eeprom file written in 32bit machine to 64 bit machine
     anyways it will be Reformatted */
  if((offsetof(rd_node_database_entry_t, dskLen) != 61) || 
      (offsetof(rd_eeprom_static_hdr_t, version_major) != 9) ||
      (offsetof(rd_eeprom_static_hdr_t, version_minor) != 10)) {

      ERR_PRINTF("Weird structure aligment?\n");
      assert(0);
  }

  rd_eeprom_read(0,sizeof(buf),buf);
  if (hdr->magic == RD_MAGIC_V0) {
    WRN_PRINTF("Found old eeprom, attempting conversion.\n");

    /* No need to insert bytes in eeprom file for version_major, version_minor as by
       structure alignment on 32 bit systems, these fields take offset where there was garbage
       so we just neeed to set those bytes at those offsets. Same for dskLen */
    rd_eeprom_write(offsetof(rd_eeprom_static_hdr_t, version_major),1,&maj); // Major Version 2
    rd_eeprom_write(offsetof(rd_eeprom_static_hdr_t, version_minor),1,&min); // Minor Version 0
    rd_eeprom_write(offsetof(rd_eeprom_static_hdr_t, magic),sizeof(magic), &magic);

    for (i = 1; i < ZW_MAX_NODES; i++) {
        rd_eeprom_read(offsetof(rd_eeprom_static_hdr_t,node_ptrs[i]),sizeof(uint16_t),&n_ptr);
        if(n_ptr==0) {
             continue;
        }
        ERR_PRINTF("Converting node: %d\n", i);
        rd_eeprom_write(offsetof(rd_node_database_entry_t, dskLen), sizeof(r.dskLen), &zero); //Write 0 (of size 1 byte) at dskLen
        /* Not writing DSK as we set dsk len to zero */
    }
  }
  rd_eeprom_read(0,sizeof(buf),buf);
  /* Note: Controllers dont necessarily change homeid after SetDefault. */
  if(hdr->magic != RD_MAGIC_V1 || hdr->homeID !=homeID || hdr->nodeID != MyNodeID)
  {
    WRN_PRINTF("Reformatting eeprom\n");
    memset(buf,0,sizeof(buf));
    hdr->magic = RD_MAGIC_V1;
    hdr->homeID = homeID;
    hdr->flags = 0;
    hdr->nodeID = MyNodeID;

    ptr = rd_eeprom_write(0,sizeof(buf),buf);
    for(i=0; i < ZW_MAX_NODES; i++) {
      q=0;
      ptr += rd_eeprom_write(ptr,sizeof(uint16_t),&q);
    }
    smformat(&nvm_dev);

    bridge_reset();
  }
#ifdef SMALL_MEMORY_TARGET
  /*Reset the mem device*/
  smformat(&rd_mem_dev);
#endif
}

/* Invalidate entire data store, so we reformat on reset. */
void rd_data_store_invalidate()
{
  const uint32_t data = 0;

  uint16_t o = offsetof(rd_eeprom_static_hdr_t, magic);
  rd_eeprom_write(o, sizeof(uint32_t), (void*)&data);
}
