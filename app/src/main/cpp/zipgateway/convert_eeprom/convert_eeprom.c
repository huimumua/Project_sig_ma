#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

/*------------------------------------------------------*/
/*      Taken from Resource Directory                   */
/*------------------------------------------------------*/
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
//typedef unsigned long uint32_t; //djnakata

typedef union uip_ip6addr_t {
  uint8_t  u8[16];         /* Initializer, must come first!!! */
  uint16_t u16[8];
} uip_ip6addr_t ;

#define ZW_MAX_NODES        232
#define RD_SMALLOC_SIZE 0x5E00

#define PREALLOCATED_VIRTUAL_NODE_COUNT 4
#define MAX_IP_ASSOCIATIONS (200 + PREALLOCATED_VIRTUAL_NODE_COUNT)

typedef uint8_t nodeid_t;
enum ASSOC_TYPE {temporary_assoc, permanent_assoc, local_assoc, proxy_assoc};
 
struct IP_association {
  void *next;
  nodeid_t virtual_id;
  enum ASSOC_TYPE type; /* unsolicited or association */
  uip_ip6addr_t resource_ip; /*Association Destination IP */
  uint8_t resource_endpoint;  /* From the IP_Association command. Association destination endpoint */
  uint16_t resource_port;
  uint8_t virtual_endpoint;   /* From the ZIP_Command command */
  uint8_t grouping;
  uint8_t han_nodeid; /* Association Source node ID*/
  uint8_t han_endpoint; /* Association Source endpoint*/
  uint8_t was_dtls;
  uint8_t mark_removal;
}; // __attribute__((packed));   /* Packed because we copy byte-for-byte from mem to eeprom */

#define ASSOCIATION_TABLE_EEPROM_SIZE (sizeof(uint16_t) + MAX_IP_ASSOCIATIONS * sizeof(struct IP_association))
/* General layout of the datastore. This is the place to add more data. */
typedef struct rd_eeprom_static_hdr {
  uint32_t magic;
  uint32_t homeID;
  uint8_t  nodeID;
  uint32_t flags;
  uint16_t node_ptrs[ZW_MAX_NODES];
  uint8_t  smalloc_space[RD_SMALLOC_SIZE];
  uint8_t temp_assoc_virtual_nodeid_count;
  nodeid_t temp_assoc_virtual_nodeids[PREALLOCATED_VIRTUAL_NODE_COUNT];
  uint16_t association_table_length;
  uint8_t association_table[ASSOCIATION_TABLE_EEPROM_SIZE];
} rd_eeprom_static_hdr_t;

typedef enum {
  MODE_PROBING,
  MODE_NONLISTENING,
  MODE_ALWAYSLISTENING,
  MODE_FREQUENTLYLISTENING,
  MODE_MAILBOX,
} rd_node_mode_t;

typedef enum {
  //STATUS_ADDING,
  STATUS_CREATED,
  //STATUS_PROBE_PROTOCOL_INFO,
  STATUS_PROBE_NODE_INFO,
  STATUS_PROBE_PRODUCT_ID,
  STATUS_ENUMERATE_ENDPOINTS,
  STATUS_SET_WAKE_UP_INTERVAL,
  STATUS_ASSIGN_RETURN_ROUTE,
  STATUS_PROBE_WAKE_UP_INTERVAL,
  STATUS_PROBE_ENDPOINTS,
  STATUS_MDNS_PROBE,
  STATUS_MDNS_EP_PROBE,
  STATUS_DONE,
  STATUS_PROBE_FAIL,
  STATUS_FAILING,
} rd_node_state_t;

typedef enum {
    EP_STATE_PROBE_INFO,
    EP_STATE_PROBE_SEC_INFO,
    EP_STATE_PROBE_ZWAVE_PLUS,
    EP_STATE_MDNS_PROBE,
    EP_STATE_MDNS_PROBE_IN_PROGRESS,
    EP_STATE_PROBE_DONE,
    EP_STATE_PROBE_FAIL
} rd_ep_state_t;

typedef void ** list_t;

#define LIST_CONCAT2(s1, s2) s1##s2
#define LIST_CONCAT(s1, s2) LIST_CONCAT2(s1, s2)
#define LIST_STRUCT(name) \
         void *LIST_CONCAT(name,_list); \
         list_t name
typedef struct rd_node_database_entry_new {

  uint32_t wakeUp_interval;
  uint32_t lastAwake;
  uint32_t lastUpdate;

  uip_ip6addr_t ipv6_address;

  uint8_t nodeid;
  uint8_t security_flags;
  /*uint32_t homeID;*/

  rd_node_mode_t mode;
  rd_node_state_t state;

  uint16_t manufacturerID;
  uint16_t productType;
  uint16_t productID;

  uint8_t nodeType; // Is this a controller, routing slave ... etc

  uint8_t refCnt;
  uint8_t nEndpoints;
  uint8_t nAggEndpoints;

  LIST_STRUCT(endpoints);

  uint8_t nodeNameLen;
  char* nodename;
} rd_node_database_entry_new_t;

typedef struct rd_node_database_entry {

  uint32_t wakeUp_interval;
  uint32_t lastAwake;
  uint32_t lastUpdate;

  uip_ip6addr_t ipv6_address;

  uint8_t nodeid;
  uint8_t security_flags;
  /*uint32_t homeID;*/

  rd_node_mode_t mode;
  rd_node_state_t state;

  uint16_t manufacturerID;
  uint16_t productType;
  uint16_t productID;

  uint8_t nodeType; // Is this a controller, routing slave ... etc

  uint8_t refCnt;
  uint8_t nEndpoints;

  LIST_STRUCT(endpoints);

  uint8_t nodeNameLen;
  char* nodename;
} rd_node_database_entry_t;

typedef struct rd_ep_data_store_entry_new {
  uint8_t endpoint_info_len;
  uint8_t endpoint_name_len;
  uint8_t endpoint_loc_len; 
  uint8_t endpoint_aggr_len; 
  uint8_t endpoint_id;
  rd_ep_state_t state;
  uint16_t iconID;
} rd_ep_data_store_entry_new_t;

typedef struct rd_ep_data_store_entry {
  uint8_t endpoint_info_len;
  uint8_t endpoint_name_len;
  uint8_t endpoint_loc_len; 
  uint8_t endpoint_id;
  rd_ep_state_t state;
  uint16_t iconID;
} rd_ep_data_store_entry_t;

/*------------------------------------------------------*/

unsigned long size = 0;
unsigned char *buf;
static int f;
static int zeroes_added = 0;
const char *linux_conf_eeprom_file;

void insert_zero_in_eeprom(unsigned long offset)
{
    unsigned long s = offset;
    char c[1] = {0};

//    printf("inserting zero at addres:%lu\n", offset-0x40);
    lseek(f, offset, SEEK_SET); // seek to offset in source
    s = size - offset; // calculate size of remaining data in source
    if(read(f, buf, s)!=s) { // read remaining data of source
        perror("Read error");
    }
    lseek(f, offset, SEEK_SET); //seek dest to offset
    write(f, c, 1);  //write byte to dest
    sync();
    lseek(f, offset+1, SEEK_SET); //seek offset+1
    write(f, buf, s);  //write remaining data of source to dest
    sync();
    size++;
//    printf("size of new file is %lu\n", size);
}
void set_zero_in_eeprom(unsigned long offset)
{
    unsigned long s = offset;
    char c[1] = {0};

    //printf("Setting zero at address:%lu\n", offset-0x40);
    lseek(f, offset, SEEK_SET); // seek to offset in source
    write(f, c, 1);  //write byte to dest
    sync();
}
void
eeprom_read(unsigned long addr, unsigned char *buf, int size)
{
  lseek(f, addr, SEEK_SET);
  if(read(f, buf, size)!=size) {
    perror("Read error");
  }
}

uint16_t rd_eeprom_read(uint16_t offset,uint8_t len,void* data) {
  //DBG_PRINTF("Reading at %x\n", 0x100 + offset);
  eeprom_read(0x40 + offset,data,len);
  return len;
}

void
eeprom_write(unsigned long addr, unsigned char *buf, int size)
{
  lseek(f, addr, SEEK_SET);
  if(write(f, buf, size) != size) {
    perror("Write error");
  }

  sync();
}

uint16_t rd_eeprom_write(uint16_t offset,uint8_t len,void* data) {
  //DBG_PRINTF("Writing at %x\n", 0x100 + offset);
  if(len) {
    eeprom_write(0x40 + offset,data,len);
  }
  return len;
}

int check_structure_alignemnts()
{
    int *ptr;
    if (sizeof(ptr) != 4) {
        printf("Error: This program is developed for 32bit systems\n");
        return 1;
    }
    if ((offsetof(rd_ep_data_store_entry_t, state) != 4) || offsetof(rd_ep_data_store_entry_t, iconID) != 8) {
        printf("-------------error\n");
        return 1;
    }
    if(sizeof(rd_ep_data_store_entry_new_t) != 16) {
        printf("-------------error\n");
        return 1;
    }
    if ((offsetof(rd_ep_data_store_entry_new_t, endpoint_aggr_len) != 3)) {
        printf("-------------error\n");
        return 1;
    }
    if ((offsetof(rd_ep_data_store_entry_new_t, state) != 8) || (offsetof(rd_ep_data_store_entry_new_t, iconID) != 12)) {
        printf("-------------error2\n");
        return 1;
    }
    if (sizeof(rd_node_database_entry_new_t) != sizeof(rd_node_database_entry_t)) {
        printf("-------------error1\n");
        return 1;
    }
//    printf("offsetof(rd_ep_data_store_entry_t, state): %d\n", offsetof(rd_ep_data_store_entry_t, state));
//    printf("offsetof(rd_ep_data_store_entry_t, iconID): %d\n", offsetof(rd_ep_data_store_entry_t, iconID));
    return 0;
}

int main(int argc, char **argv)
{
    uint16_t n_ptr, e_ptr, t_ptr;
    rd_node_database_entry_t r;
    rd_node_database_entry_t r_new;
    int i, j;
    unsigned long offset = 0;
    uint8_t c;

    if (argc < 2) {
        printf("Usage: convert_eeprom <eeprom_file_path>\n");
        printf("eeprom file in changed inline. it can not be used in ver2_25 of zipgateway again after changing \n");
        exit(1);
    }
    linux_conf_eeprom_file = argv[1];
    printf("Opening eeprom file %s\n", linux_conf_eeprom_file);
    f = open(linux_conf_eeprom_file, O_RDWR | O_CREAT, 0644);
    if(f<0) {
        fprintf(stderr, "Error opening eeprom file %s\n",linux_conf_eeprom_file);
        perror("");
        exit(1);
    }
    size = lseek(f, 0, SEEK_END);
    buf = malloc(size+64000);

    if(check_structure_alignemnts()) {
        exit(1);
    }
    
    for (i = 1; i < ZW_MAX_NODES; i++) {
        const uint16_t static_size = offsetof(rd_node_database_entry_t,nodename);
        rd_eeprom_read(offsetof(rd_eeprom_static_hdr_t,node_ptrs[i]),sizeof(uint16_t),&n_ptr);
        if(n_ptr==0) {
            continue;
        }
        printf("-----------node: %d\n", i);
        printf("n_ptr: %d\n", n_ptr );
        n_ptr += zeroes_added;
        rd_eeprom_read(n_ptr, static_size , &r);
        //insert zero
        offset = 0x40 + n_ptr + offsetof(rd_node_database_entry_t, nEndpoints)+1;
        set_zero_in_eeprom(offset);

        //Change the pointer
        rd_eeprom_write(offsetof(rd_eeprom_static_hdr_t,node_ptrs[i]),sizeof(uint16_t),&n_ptr);
        printf("new n_ptr: %d\n", (int)n_ptr);

        //Verify the inserted zero
        rd_eeprom_read(n_ptr+offsetof(rd_node_database_entry_t, nEndpoints)+1,sizeof(uint8_t),&c);
        //printf("Verifying data written at t_ptr: %x \n", c);
        if(c != 0)
        {
            printf("Verification failed\n");
        }
        
        n_ptr += static_size + r.nodeNameLen;
        for(j=0; j < r.nEndpoints; j++) {
//            printf("---------n_ptr: %d\n", n_ptr);
            //read the original pointer
            rd_eeprom_read(n_ptr, sizeof(uint16_t), &e_ptr);
            printf("old e_ptr: %d \n", e_ptr);
            // need to fast forward it by number of zeroes added so far
            e_ptr += zeroes_added;
            //insert zero 
            insert_zero_in_eeprom(0x40 + e_ptr + offsetof(rd_ep_data_store_entry_t, endpoint_loc_len)+1);
            insert_zero_in_eeprom(0x40 + e_ptr + offsetof(rd_ep_data_store_entry_t, endpoint_id)+2);
            insert_zero_in_eeprom(0x40 + e_ptr + offsetof(rd_ep_data_store_entry_t, endpoint_id)+3);
            insert_zero_in_eeprom(0x40 + e_ptr + offsetof(rd_ep_data_store_entry_t, endpoint_id)+4);
            zeroes_added+= 4;
            //change the pointer
            printf("new e_ptr: %d \n", e_ptr);
            rd_eeprom_write(n_ptr,sizeof(uint16_t),&e_ptr);
            rd_eeprom_read(n_ptr, sizeof(uint16_t), &e_ptr);
            printf("File e_ptr: %d \n", e_ptr);

            //Verify the inserted zero
            rd_eeprom_read(e_ptr+offsetof(rd_ep_data_store_entry_t, endpoint_loc_len)+1,sizeof(uint8_t),&c);
            if(c != 0)
            {
                printf("Verification failed\n");
            }
            
            rd_eeprom_read(e_ptr+offsetof(rd_ep_data_store_entry_t, endpoint_id)+2,sizeof(uint8_t),&c);
            if(c != 0)
            {
                printf("Verification failed\n");
            }
            rd_eeprom_read(e_ptr+offsetof(rd_ep_data_store_entry_t, endpoint_id)+3,sizeof(uint8_t),&c);
            if(c != 0)
            {
                printf("Verification failed\n");
            }
            rd_eeprom_read(e_ptr+offsetof(rd_ep_data_store_entry_t, endpoint_id)+4,sizeof(uint8_t),&c);
            if(c != 0)
            {
                printf("Verification failed\n");
            }
            //Get the next place for e_ptr
            n_ptr+=sizeof(uint16_t);
            printf("----------------------------------------\n");
        }
            printf("=================================================\n");
    }
    close(f);
    free(buf);
    printf("%s file in changed inline now. it can not be used in ver2_25 of zipgateway again after changing \n", linux_conf_eeprom_file);
}
