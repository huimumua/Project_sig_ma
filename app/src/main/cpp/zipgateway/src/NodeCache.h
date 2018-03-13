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

#ifndef NODECACHE_H_
#define NODECACHE_H_

#include"TYPES.H"
#include"sys/clock.h"

#ifdef __ASIX_C51__
#undef data
#define data _data
#endif
typedef enum UpdateStatus {
	UPDATE_STATUS_OK=0,
	UPDATE_STATUS_NOT_RESPONDING=1,
	UPDATE_STATUS_UNKNOWN=2,
	UPDATE_STATUS_RESOURCE_UNAVAIL=3
} UpdateStatus_t;

/**
 * Place holder for a node Info.
 * After this structure comes a list of classes.
 */

struct NodeInfo {
    BYTE      basicDeviceClass;             /**/
    BYTE      genericDeviceClass;           /**/
    BYTE      specificDeviceClass;          /**/
//    BYTE      classes[0];
};

typedef struct NodeCacheHdr
{
  DWORD homeid; //Home id of which this cache belongs.
  BYTE n_nodes; //Number of nodes in cache
} NodeCacheHdr_t;

#define NIF_MAX_SIZE 64
struct NodeCacheEntry {
  BYTE node;
  BYTE info_length;
  BYTE sec_info_length;
  clock_time_t last_update;
  BYTE flags;
  BYTE node_info[NIF_MAX_SIZE];
  BYTE sec_info[NIF_MAX_SIZE];
};


#define CACHE_DATA_OFFSET (EEOFFSET_NODE_CACHE_START + sizeof(NodeCacheHdr_t))
#define NCE_OFFSET(node) (CACHE_DATA_OFFSET + (node * sizeof(struct NodeCacheEntry)))


#define NODECACHE_EEPROM_SIZE (sizeof(NodeCacheHdr_t) + 233*sizeof(struct NodeCacheEntry))


#define NODE_FLAG_SECURITY0      0x01
#define NODE_FLAG_KNOWN_BAD     0x02
/** Node probe has not completed since node was added.
*/
#define NODE_FLAG_JUST_ADDED    0x04
#define NODE_FLAG_INFO_ONLY     0x08 /* Only probe the node info */
#define NODE_FLAG_SECURITY2_UNAUTHENTICATED 0x10
#define NODE_FLAG_SECURITY2_AUTHENTICATED   0x20
#define NODE_FLAG_SECURITY2_ACCESS          0x40
/** Whether the node was (security-)added by this GW.
 *
 * This flag must not be set if the node was added by another
 * controller.  Only relevant if NODE_FLAG_JUST_ADDED is also set.
 *
 * If this flag is set, GW already knows
 * the security keys of the node and security should not be
 * down-graded by the probe process. */
#define NODE_FLAG_ADDED_BY_ME    0x80

#define NODE_FLAGS_SECURITY2 (\
    NODE_FLAG_SECURITY2_UNAUTHENTICATED| \
    NODE_FLAG_SECURITY2_AUTHENTICATED| \
    NODE_FLAG_SECURITY2_ACCESS)

#define NODE_FLAGS_SECURITY (\
    NODE_FLAG_SECURITY0 | \
    NODE_FLAG_KNOWN_BAD| \
    NODE_FLAGS_SECURITY2)

#define CACHE_INFO_REQ 1
#define CACHE_SEC_INFO_REQ 2


#define INFINITE_AGE 0xf

#define  isNodeBad(n) ((GetCacheEntryFlag(n) & NODE_FLAG_KNOWN_BAD) !=0)
#define  isNodeSecure(n) ( (GetCacheEntryFlag(n) & (NODE_FLAG_SECURITY0 | NODE_FLAG_KNOWN_BAD)) == NODE_FLAG_SECURITY0)
int isNodeController(BYTE nodeid);

/**
 * Initializeses internal data structures.
 */
BOOL CacheInit();

/**
 * Add/Update node cache entry
 *
 * field should be of either CACHE_FIELD_INFO or CACHE_FIELD_SECURITY
 */
void UpdateCacheEntry(BYTE nodeid, BYTE* data, BYTE data_len,BYTE field) CC_REENTRANT_ARG;

/**
 * Remove cache entry from node cache.
 */
void ClearCacheEntry(BYTE nodeid) CC_REENTRANT_ARG;

/**
 * Retrieve Cache entry
 * If info is NULL only the length of the node cache entry is returned.
 *
 * @sa GetCacheSecureClasses
 * @param nodeid Node in question
 * @param maxage Maximum age of the node cache entry.
 * @param callback This function is called when the node cache entry has been retrieved.
 */
void GetCacheNodeInfo(BYTE nodeid,BYTE maxage, void (*callback)(struct NodeCacheEntry* e,UpdateStatus_t status));


int get_entry_age(struct NodeCacheEntry* e);
struct NodeCacheEntry* get_cache_entry(BYTE nodeid);
/**
 * Retrieve a list of classes which are handled securely.
 * @param[in]  nodeid Node to be queried.
 * @param[in]  maxage Maximum age of the requested entry in a log time scale.
 * The maxage is calculated as 2^age minutes, age==INFINITE_AGE is infinite.
 * If the cache entry is older than the requested age the entry an attempt is
 * made to update the cache entry.
 * @param[out] classes Destination buffer to hold the node id.
 * @param[out] age  The actual age of the returned entry, calculated as in maxage
 *
 * @return The length of the class list in bytes
 */
int GetCacheSecureClasses(BYTE nodeid,BYTE maxage, BYTE* classes,  BYTE* age);

/**
 * Retrieve Cache entry flag
 */
BYTE GetCacheEntryFlag(BYTE nodeid) CC_REENTRANT_ARG;

/**
 * Checks if a given node supports a command class non secure.
 * @param nodeid ID of the node to check
 * @param class Class code to check for. This may be an extended class.
 * @retval TRUE The node supports this class
 * @retval FALSE The node does not support this class.
 */
int SupportsCmdClass(BYTE nodeid, WORD class);

/**
 * Checks if a given node supports a command class securely.
 * \param nodeid ID of the node to check
 * \param class Class code to check for. This may be an extended class.
 * \retval TRUE The node supports this class
 * \retval FALSE The node does not support this class.
 */
int SupportsCmdClassSecure(BYTE nodeid, WORD class)CC_REENTRANT_ARG;


/**
 * Set node attribute flags
 */
BYTE SetCacheEntryFlagMasked(BYTE nodeid,BYTE value, BYTE mask)CC_REENTRANT_ARG;


/**
 * Used to indicate that node info was received or timed out from protocol side.
 */
void NodeInfoRequestDone(BYTE status);


/**
 * Used to indicate that a secure node info was received or timed out from protocol side.
 */
void NodeInfoSecureRequestDone();

/**
 * Attempt to do a live update of a node
 * @param[in] nodeID The node to update.
 */
void CacheRefreshNode(BYTE nodeID);


/**
 * Refresh all nodes, by forcing them to send a node info.
 */
void CacheRefreshAllNodes();


void request_nodeupdate(BYTE node,BOOL skip_info, void(*callback)(struct NodeCacheEntry* e,UpdateStatus_t status))CC_REENTRANT_ARG;

BOOL nodeExsists(BYTE nodeid);

/**
 * Check for updates in the node lists and do probe of new nodes.
 */
u8_t CacheProbeNewNodes();

/**
 * Get the number of pending node probes
 * @return
 */
u8_t getPendingUpdatesCount();
#endif /* NODECACHE_H_ */
