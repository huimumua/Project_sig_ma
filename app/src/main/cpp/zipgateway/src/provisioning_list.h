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

#ifndef _PROVISIONING_LIST_H
#define _PROVISIONING_LIST_H
#include <stdint.h>
#include <stdio.h>
#include <provisioning_list_types.h>

/** \defgroup pvslist Provisioning List
 * The Provisioning List supports the Smart Start functionality in the ZIP Gateway.
 *
 * It stores all the known information about pre-configured devices based on the DSKs.
 *
 * @{
 */

/** Max size of an DSK in a provisioning list iterator.
 */
#define PROVISIONING_LIST_DSK_MAX_SIZE 16

/** The type of the provisioning list.
 */
typedef struct provision provisioning_list_t;

/** Provisioning specification of a device.
 *
 */
struct provision {
    /*@null@*//*@relnull@*/uint8_t *dsk; /**< DSK of the provisioned device. */
    uint8_t dsk_len; /**< Length of the dsk. */
    pvs_status_t status; /** Current state of this item. */
    provisioning_bootmode_t bootmode; /**< Provisioning bootmode of this item (S2 or smartstart) */
    /*@owned@*//*@null@*/struct pvs_tlv *tlv_list; /**< TLVs for this item. */
#ifdef PVS_TEST
    uint8_t num_tlvs;
#endif
};

/**
Return codes for provisioning list functions.
*/
typedef enum {
    PVS_SUCCESS = 1,
    PVS_ERROR = 0
} pvs_result_t;

/**
 @brief Provisioning List definition of Metadata TLV. It is different from the QR Code TLV defined
 in SDS13937. This is the version used in provisioning_list.c and other places
*/
struct pvs_tlv
{
    /*@owned@*//*@null@*/struct pvs_tlv *next; /**< Pointer to the next tlv */
    uint8_t type; /**< TLV type.   Can be tlv type or an unknown type.*/
    uint8_t length; /**< Length of the value buffer in bytes. */
    uint8_t *value; /**< TLV payload.  May be non-printable. */
};

/** Initialize the provisioning list.
 *
 * For storage, a non-existing file argument will be created and used.
 * If a NULL storage filename is provided, or if the provided file contains
 * illegal data, the default storage file will be used instead, to avoid
 * overwriting a file that is given by mistake.
 *
 * If the default file cannot be used, either, provisioning list will
 * not have persistent storage.
 *
 * The configuration file is intended for first-time initialization.
 * It will only be used if the neither the provided nor the default
 * storage file exists (independently of whether the storage file
 * contains valid data).  In that case, it will be used for
 * initialization if the argument is non-NULL and the file exists and
 * can be parsed.
 *
 * Note that if a valid persistent storage file cannot be created, and
 * a valid configuration file is available, the ZIP gateway will
 * revert to the initial configuration every time it starts up.
 *
 * If neither file is useable, start out with an empty provisioning list.
 *
 * \param storage_filename Name of the provisioning list storage file.
 * \param cfg_filename Name of the provisioning list configuration file.
 */
void provisioning_list_init(/*@null@*/const char* storage_filename, /*@null@*/const char* cfg_filename);

/**
Find the number of items in the provisioning list.

@return: Number of items.
*/
uint8_t provisioning_list_get_count();

/** Print one device entry to strm */
void pvs_dsk_print(FILE *strm, uint8_t dsk_len, /*@null@@*/uint8_t *dsk);
void pvs_list_print(FILE *strm);

typedef struct pvs_iter_entry
{
    uint8_t dsk_len;
    uint8_t dsk[PROVISIONING_LIST_DSK_MAX_SIZE];
} pvs_iter_entry_t;

/**
 * Provisioning list iterator object.
 *
 * Stores a snapshot of the number of entries in the provisioning list.
 * Allows a caller to access all the entries asynchronously with a next() function.
 *
 * There are no more entries when cnt == next.
 *
 * The next() function returns NULL when there are no more entries,
 * but also in case an entry has been deleted from the provisioning
 * list since the iterator was created.
 */
struct provisioning_list_iter {
    uint8_t cnt;
    uint8_t next;
    uint32_t id;
    pvs_iter_entry_t *entries;
};

typedef struct provisioning_list_iter provisioning_list_iter_t;

/**
 * Get an iterator with all the entries currently in the provisioning list.
 *
 * The iterator must be deleted with provisioning_list_iterator_delete().
 *
 * \param id A 32 bit id that the user can pick.
 * @return A pointer to a provisioning list iterator object.
 */
/*@null@*//*@out@*/provisioning_list_iter_t * provisioning_list_iterator_get(uint32_t id);

/** Delete an iterator object.
 */
void provisioning_list_iterator_delete(/*@only@*/provisioning_list_iter_t *iter);

/**
 * Find the provision for the next entry from an iterator.
 *
 * Look up the next entry in the iterator and then find the device
 * provision that corresponds to that entry.
 *
 * If the device has been deleted from the provisioning list since the
 * iterator was created`, return NULL.
 *
 * If there is no next (iter->cnt == iter->next), return NULL.
 *
 * @param iter An iterator that has been returned by provisioning_list_get_iterator().
 * @return Pointer to the device provision that corresponds to the next dsk in iterator or NULL.
 */
/*@dependent@*//*@null@*/struct provision * provisioning_list_iter_get_next(provisioning_list_iter_t *iter);

/**
 * Find an indexed dsk in an iterator.
 *
 * @param iter An iterator that has been returned by provisioning_list_get_iterator().
 * @param idx The index to look up.
 * @return Pointer to the provision in the iterator or NULL.
 */
/*@dependent@*//*@null@*/struct provision * provisioning_list_iter_get_index(provisioning_list_iter_t *iter, uint8_t idx);


/**
Add a new device specification to the provisioning list.

Status is initialized to pending.

@param dsk_len: length of dsk
@param dsk: dsk value
@param bootmode provisioning bootmode of the device.

@return Pointer to the new provision or NULL on failure.
*/
/*@dependent@*//*@null@*/struct provision * provisioning_list_dev_add(uint8_t dsk_len, uint8_t *dsk, provisioning_bootmode_t bootmode);

/**
Match a security challenge containing the DSK against the provisioning_list,
returning the matching provisioning_list item.
The matching will skip the first 2 bytes of the DSK which may be zeroed out
when including Authenticated/Access devices.

@param challenge_len: length of challenge
@param challenge: challenge value

@return pointer to matching struct provision  on success or NULL on failure
*/
/*@dependent@*//*@null@*/struct provision* provisioning_list_dev_match_challenge(uint8_t challenge_len, uint8_t *challenge);

/**
Search the provisioning_list for a DSK corresponding to a received
homeid in the provisioning list.

The homeid is a version of the DSK with three bits masked as required
by the Z-Wave HomeID selection rules.

@param homeid: HomeID

@return pointer to struct provision on success or NULL on failure
*/
/*@dependent@*//*@null@*/struct provision* provisioning_list_dev_get_homeid(uint8_t *homeid);

/**
Update a device specification in the provisioning list or add it if it does not exist.

@param dsk_len: length of dsk
@param dsk: dsk value
@param bootmode provisioning bootmode of the device.
@return Pointer to the new provision or NULL on failure.
*/
/*@dependent@*//*@null@*/struct provision * provisioning_list_dev_set(uint8_t dsk_len, uint8_t *dsk, provisioning_bootmode_t bootmode);

/**
Remove a device from the provisioning list by its DSK.

@param dsk_len: length of dsk
@param dsk: dsk value
@return PVS_SUCCESS if found, PVS_ERROR if dsk does not exist
*/
int  provisioning_list_dev_remove(uint8_t dsk_len, uint8_t *dsk);

/**
 * Look up a device in the provisioning_list by its DSK.
 *
 * @param dsk_len: length of dsk
 * @param dsk: dsk value
 *
 * @return Pointer to struct provision on success or NULL on failure
*/
/*@dependent@*//*@null@*/struct provision * provisioning_list_dev_get(uint8_t dsk_len, uint8_t *dsk);

/*
Return all the entries from provisioning_list
*/

/**
 * Find the number of items with status PVS_STATUS_PENDING.
 */
int provisioning_list_pending_count(void);

/**
 * Clear the provisioning list, removing all items.
 */
void provisioning_list_clear(void);

/** Update the bootstrapping mode of a device provision.
 *
 * @param pvs The provision to update.
 * @param bootmode The new bootstrapping mode.
 * @return PVS_SUCCESS on success, PVS_ERROR if device not found.
 */
int provisioning_list_bootmode_set(struct provision *pvs, provisioning_bootmode_t bootmode);

/** Find the value of the critical flag for a tlv type.
 *
 *  @note This function does not handle bootmode and smart-start
 *  setting (status), since these tlvs have special handling anyway.
 *
 * \param tlv_type A tlv type code.
 * \return The critical flag (0 or 1).
 */
uint8_t provisioning_list_tlv_crt_flag(pvs_tlv_type_t tlv_type);

/** Add or overwrite a tlv of a device provision.
 *
 * The value is copied to new memory.
 *
 * @param pvs The provision to update.
 * @param type TLV type.   Can be tlv type or an unknown type.
 * @param len TLV payload length in bytes.
 * @param val TLV payload.
 * @return PVS_SUCCESS on success, PVS_ERROR if no memory.
 */
int provisioning_list_tlv_set(struct provision *pvs, uint8_t type, uint8_t len, uint8_t *val);

/**
 * Find a TLV for a device in the provisioning list.
 *
 * @param pvs Pointer to the device provision where to look for the tlv.
 * \param type The type to look up.   Can be tlv type or an unknown type.
 * @return Pointer to the the TLV value or NULL if the type is not found.
 */
/*@dependent@*//*@null@*/struct pvs_tlv * provisioning_list_tlv_get(struct provision *pvs, uint8_t type);

/**
 * Find a TLV for a DSK in the provisioning list.
 *
 * \param dsk_len The length of the dsk.
 * \param dsk The dsk of the device to look up the tlv in.
 * \param type The type to look up.   Can be tlv type or an unknown type.
 * @return Pointer to the the TLV value or NULL if the DSK or the type is not found.
 */
/*@dependent@*//*@null@*/struct pvs_tlv * provisioning_list_tlv_dsk_get(uint8_t dsk_len, uint8_t *dsk, uint8_t type);


/** Remove a tlv of dsk.
 *
 * @param pvs the provision to update.
 * @param type TLV type.  Can be tlv type or an unknown type.
 * @return PVS_SUCCESS on success, PVS_ERROR if pvs is NULL or type not found.
 */
int provisioning_list_tlv_remove(struct provision *pvs, uint8_t type);

/**
 * Remove all tlvs from dsk in the provisioning list.
 *
 * @param pvs the provision to remove tlvs from.
 */
void provisioning_list_tlv_clear(struct provision *pvs);

/**
Print all the items in provisioning_list.
*/
void provisioning_list_print(void);


/** Update the status of a provision entry and persist.
 * @param pvs the provision to update
 * @param status The updated status
 * @return PVS_SUCCESS on success, PVS_ERROR if pvs is NULL.
 */
int provisioning_list_status_set(struct provision *pvs, pvs_status_t status);

/**
 * @}
 */
#endif
