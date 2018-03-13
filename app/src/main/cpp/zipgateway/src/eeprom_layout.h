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

#ifndef EEPROM_LAYOUT_H_
#define EEPROM_LAYOUT_H_


#include "NodeCache.h"
#include "zip_router_config.h"

#define ZIPMAGIC 1645985

/* ZW firmware related Error Messages */
#define FW_DOWNLOAD_SUCCESSFUL		0x0
#define FW_DOWNLOAD_READY			0x1
#define ERROR_CAN_NOT_SYNCH			0x2
#define ERROR_CHECKSUM_FAILED		0x3
#define ERROR_CHECKSUM_NOT_DONE		0x4
#define ERROR_FLASH_WRITE_FAILED	0x5
#define ERROR_NVR_LOAD_FAILED		0x6


/*ZW firmware download status messages */
#define ZW_FW_DOWNLOAD_IDLE			0x00
#define ZW_FW_UPDATE_IN_PROGRESS	0x01
#define ZW_FW_UPDATE_BACKUP_DONE	0x02
#define ZW_FW_UPDATE_AP_SET_DONE	0x03
#define ZW_FW_UPDATE_FAILED			0x04

/* Default config download status message */
#define DEFAULT_CONFIG_DOWNLOAD_IDLE			0x00
#define DEFAULT_CONFIG_UPDATE_IN_PROGRESS		0x01


#define ZW_FW_HEADER_LEN		0x08UL


#define EEOFFSET_MAGIC_START 0x0000
#define EEOFFSET_MAGIC_SIZE sizeof(zip_nvm_config_t)

#define EXT_FLASH_START_ADDR	0x000000UL

#define EXT_ZW_FW_UPGRADE_STATUS_ADDR				EXT_FLASH_START_ADDR
#define ZW_FW_UPGRADE_STATUS_LEN					0x1

#define EXT_ZW_FW_DOWNLOAD_DATA_LEN_ADDR			(EXT_ZW_FW_UPGRADE_STATUS_ADDR+ZW_FW_UPGRADE_STATUS_LEN)
#define ZW_FW_DATA_LEN					 			0x4

#define EXT_DEFAULT_CONFIG_UPGRADE_STATUS_ADDR		(EXT_ZW_FW_DOWNLOAD_DATA_LEN_ADDR+ZW_FW_DATA_LEN)
#define DEFAULT_CONFIG_UPGRADE_STATUS_LEN			0x1

#define EXT_DEFAULT_CONFIG_DATA_LEN_ADDR			(EXT_DEFAULT_CONFIG_UPGRADE_STATUS_ADDR+DEFAULT_CONFIG_UPGRADE_STATUS_LEN)
#define DEFAULT_CONFIG_DATA_LEN						0x2

#define EXT_FLASH_RESERVED_LEN		0x200

#define EEOFFSET_NODE_CACHE_START (EXT_FLASH_RESERVED_LEN)
#define EEOFFSET_NODE_CACHE_SIZE  NODECACHE_EEPROM_SIZE
#define MD5_CHECKSUM_LEN						0x10
#define MAX_ZW_FWLEN							(0x20000UL + ZW_FW_HEADER_LEN)
#define MAX_ZW_WITH_MD5_FWLEN					(0x20000UL + ZW_FW_HEADER_LEN + MD5_CHECKSUM_LEN)

//External flash partitions
#define TOTAL_FLASH_SIZE						0x80000UL

/* Last 4KB of first sector reserved for portal config and extra classes */
/* Portal config start address is 60KB */
#define EXT_PORTAL_CONFIG_PARTITION_START_ADDR		0xF000UL
#define PORTAL_CONFIG_MAGIC_LEN						4
#define EXT_PORTAL_CONFIG_START_ADDR			    (EXT_PORTAL_CONFIG_PARTITION_START_ADDR + PORTAL_CONFIG_MAGIC_LEN)
#define PORTAL_CONFIG_PARTITION_LEN					256

#define EXT_EXTRA_CLASS_CONFIG_PARTITION_START		(EXT_PORTAL_CONFIG_PARTITION_START_ADDR + PORTAL_CONFIG_PARTITION_LEN)
#define EXTRA_CLASS_CONFIG_MAGIC_LEN				4
#define EXT_EXTRA_CLASS_CONFIG_START				(EXT_EXTRA_CLASS_CONFIG_PARTITION_START + EXTRA_CLASS_CONFIG_MAGIC_LEN)


#define NIF_CACHE_LEN							0x10000UL
#define EXT_GW_SETTINGS_START_ADDR				(EXT_FLASH_START_ADDR+NIF_CACHE_LEN)
#define GW_SETTINGS_PORFILE_CONFIG_LEN			0x800  //2KB Bytes

/* Index to GW profile and certs --start */
#define EXT_GW_PROFILE_START_ADDR				(EXT_GW_SETTINGS_START_ADDR+sizeof(Gw_Config_St_t))
/* Index to GW profile and certs --end */

#define EXT_CERT_SCRATCH_PAD_ADDR		    	(EXT_GW_SETTINGS_START_ADDR+GW_SETTINGS_PORFILE_CONFIG_LEN)
#define CERT_SCRATCH_PAD_LEN					0x2000  //8KB
#define DEVICE_CONFIG_LEN						(GW_SETTINGS_PORFILE_CONFIG_LEN+CERT_SCRATCH_PAD_LEN)
#define EXT_FW_START_ADDR				    	(EXT_CERT_SCRATCH_PAD_ADDR+CERT_SCRATCH_PAD_LEN)
#define MAX_FW_LEN 								(TOTAL_FLASH_SIZE-EXT_FW_START_ADDR-DEVICE_CONFIG_LEN)
#define EXT_DEFAULT_CONFIG_START_ADDR			(EXT_FW_START_ADDR+MAX_FW_LEN)
#define DEFAULT_CONFIG_LEN						(DEVICE_CONFIG_LEN) //10KB(Profile+ Default Certs)

#define EXT_DEFAULT_CONFIG_SCRATCH_PAD_SART		(EXT_FW_START_ADDR)



void Ext_Eeprom_Config_Read(u32_t baddr, u32_t start,void* dst, u16_t size);
void Ext_Eeprom_Config_Write(u32_t baddr, u32_t start,void* dst, u16_t size);

#define ext_eeprom_ipconfig_get(par_name,dst) Ext_Eeprom_Config_Read(EXT_PORTAL_CONFIG_PARTITION_START_ADDR, offsetof(portal_ip_configuration_with_magic_st_t,par_name),dst,sizeof(((portal_ip_configuration_with_magic_st_t*)0)->par_name))
#define ext_eeprom_ipconfig_set(par_name,src) Ext_Eeprom_Config_Write(EXT_PORTAL_CONFIG_PARTITION_START_ADDR, offsetof(portal_ip_configuration_with_magic_st_t,par_name),src,sizeof(((portal_ip_configuration_with_magic_st_t*)0)->par_name))

#define MD5_CHECKSUM_LEN						0x10
#define MAX_ZW_FWLEN							(0x20000UL + ZW_FW_HEADER_LEN)
#define MAX_ZW_WITH_MD5_FWLEN					(0x20000UL + ZW_FW_HEADER_LEN + MD5_CHECKSUM_LEN)

//External flash partitions
#define TOTAL_FLASH_SIZE						0x80000UL

/* Last 4KB of first sector reserved for portal config and extra classes */
/* Portal config start address is 60KB */
#define EXT_PORTAL_CONFIG_PARTITION_START_ADDR		0xF000UL
#define PORTAL_CONFIG_MAGIC_LEN						4
#define EXT_PORTAL_CONFIG_START_ADDR			    (EXT_PORTAL_CONFIG_PARTITION_START_ADDR + PORTAL_CONFIG_MAGIC_LEN)
#define PORTAL_CONFIG_PARTITION_LEN					256

#define EXT_EXTRA_CLASS_CONFIG_PARTITION_START		(EXT_PORTAL_CONFIG_PARTITION_START_ADDR + PORTAL_CONFIG_PARTITION_LEN)
#define EXTRA_CLASS_CONFIG_MAGIC_LEN				4
#define EXT_EXTRA_CLASS_CONFIG_START				(EXT_EXTRA_CLASS_CONFIG_PARTITION_START + EXTRA_CLASS_CONFIG_MAGIC_LEN)


#define NIF_CACHE_LEN							0x10000UL
#define EXT_GW_SETTINGS_START_ADDR				(EXT_FLASH_START_ADDR+NIF_CACHE_LEN)
#define GW_SETTINGS_PORFILE_CONFIG_LEN			0x800  //2KB Bytes

/* Index to GW profile and certs --start */
#define EXT_GW_PROFILE_START_ADDR				(EXT_GW_SETTINGS_START_ADDR+sizeof(Gw_Config_St_t))
/* Index to GW profile and certs --end */

#define EXT_CERT_SCRATCH_PAD_ADDR		    	(EXT_GW_SETTINGS_START_ADDR+GW_SETTINGS_PORFILE_CONFIG_LEN)
#define CERT_SCRATCH_PAD_LEN					0x2000  //8KB
#define DEVICE_CONFIG_LEN						(GW_SETTINGS_PORFILE_CONFIG_LEN+CERT_SCRATCH_PAD_LEN)
#define EXT_FW_START_ADDR				    	(EXT_CERT_SCRATCH_PAD_ADDR+CERT_SCRATCH_PAD_LEN)
#define MAX_FW_LEN 								(TOTAL_FLASH_SIZE-EXT_FW_START_ADDR-DEVICE_CONFIG_LEN)
#define EXT_DEFAULT_CONFIG_START_ADDR			(EXT_FW_START_ADDR+MAX_FW_LEN)
#define DEFAULT_CONFIG_LEN						(DEVICE_CONFIG_LEN) //10KB(Profile+ Default Certs)

#define EXT_DEFAULT_CONFIG_SCRATCH_PAD_SART		(EXT_FW_START_ADDR)



void Ext_Eeprom_Config_Read(u32_t baddr, u32_t start,void* dst, u16_t size);
void Ext_Eeprom_Config_Write(u32_t baddr, u32_t start,void* dst, u16_t size);

#define ext_eeprom_ipconfig_get(par_name,dst) Ext_Eeprom_Config_Read(EXT_PORTAL_CONFIG_PARTITION_START_ADDR, offsetof(portal_ip_configuration_with_magic_st_t,par_name),dst,sizeof(((portal_ip_configuration_with_magic_st_t*)0)->par_name))
#define ext_eeprom_ipconfig_set(par_name,src) Ext_Eeprom_Config_Write(EXT_PORTAL_CONFIG_PARTITION_START_ADDR, offsetof(portal_ip_configuration_with_magic_st_t,par_name),src,sizeof(((portal_ip_configuration_with_magic_st_t*)0)->par_name))


#endif /* EEPROM_LAYOUT_H_ */
