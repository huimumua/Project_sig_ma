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

#ifndef IPV46_NAT_H_
#define IPV46_NAT_H_

 /**
  * \defgroup ip46nat IPv4 to IPv6 network address translator.
  * This module maps IPv4 UDP and ICMP ping packages into their IPv6 equivalents and
  * vice versa. This is done to allow the internal IPv6 to process the IPv4
  * frames as well.
  *
  * @{
  */

/**
 * Contains information about each node
 */
typedef struct nat_table_entry {
  /** Node id which the nat table entry points to */
  u8_t nodeid;
  /** The last bits of the ipv4 address if the nat entry.
   * This is combined with the ipv4 net to form the full ipv4 address.
   */
  u16_t ip_suffix;
} nat_table_entry_t;

/**
 * Maximum number of entries in the NAT table
 */
#define MAX_NAT_ENTRIES 232

/**
 * Actual number of entries in the NAT table
 */
extern u8_t nat_table_size ;

/**
 * The NAT entry database
 */
extern nat_table_entry_t nat_table[MAX_NAT_ENTRIES];


/**
 * Generic definition of an IPv6 address
 */
typedef union ip6addr {
  u8_t  u8[16];     /* Initializer, must come first!!! */
  u16_t u16[8];
} ip6addr_t ;

/**
 * Generic definition of an IPv4 address
 */
typedef union ip4addr_t {
  u8_t  u8[4];			/* Initializer, must come first!!! */
  u16_t u16[2];
} ipv4addr_t;

typedef ipv4addr_t uip_ipv4addr_t;

/**
 * Input of the NAT interface driver. if the destination of the IP address is a NAT address, 
 * this will translate the ip4 package in  uip_buf to a ipv6 package, 
 * If the address is not a NAT'ed address this function will not change the uip_buf.
 *
 * If the destination address is the the uip_hostaddr and the destination
 * UDP port is the Z-Wave port, translation will be performed. Otherwise it will not.
 */
void ipv46nat_interface_input()CC_REENTRANT_ARG;
/**
 * Translate the the package in uip_buf from a IPv6 package to a IPv4 package if the package
 * in uip_buf is a mapped IPv4 package.
 *
 * This functions updates uip_buf and uip_len with the translated IPv4 package.
  */
u8_t ipv46nat_interface_output()CC_REENTRANT_ARG;

/**
 * Add NAT table entry. Returns 0 is the add entry fails
 */
u8_t ipv46nat_add_entry(u8_t node);


/**
 * Remove a NAT table entry, return 1 if the entry was removed.
 */
u8_t ipv46nat_del_entry(u8_t node);
/**
 * Initialize the IPv4to6 translator
 */
void ipv46nat_init();

/**
 * Get the IPv4 address of a nat table entry.
 * @param ip destination to write to
 * @param e Entry to query
 */
void ipv46nat_ipv4addr_of_entry(uip_ipv4addr_t* ip,nat_table_entry_t *e);


/**
 * Get the node id behind the ipv4 address
 * @param ip IPv4 address
 * @return nodeid
 */
u8_t ipv46_get_nat_addr(uip_ipv4addr_t *ip);
/**
 * Return true if this is an IPv4 mapeed IPv6 address
 */
u8_t is_4to6_addr(ip6addr_t* ip);
/**
 * Get the IPv4 address of a node
 * @param ip destination to write the ip to
 * @param node Id of the node to query for
 * @return TRUE if address was found
 */
u8_t ipv46nat_ipv4addr_of_node(uip_ipv4addr_t* ip,u8_t node);

/**
 * Return true if all nodes has an ip
 */
u8_t ipv46nat_all_nodes_has_ip();


/**
 * Rename a node in the nat table to a new ID
 */
void ipv6nat_rename_node(uint8_t old_id, uint8_t new_id) ;

/**
 * @}
 */

#endif /* IPV46_NAT_H_ */
