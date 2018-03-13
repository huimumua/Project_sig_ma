/**
@file   zip_api_util.c - Z/IP High Level API utility/miscellaneous functions implementation.


@author David Chow

@version    1.0 7-6-11  Initial release

@copyright � 2014 SIGMA DESIGNS, INC. THIS IS AN UNPUBLISHED WORK PROTECTED BY SIGMA DESIGNS, INC.
AS A TRADE SECRET, AND IS NOT TO BE USED OR DISCLOSED EXCEPT AS PROVIDED Z-WAVE CONTROLLER DEVELOPMENT KIT
LIMITED LICENSE AGREEMENT. ALL RIGHTS RESERVED.

NOTICE: ALL INFORMATION CONTAINED HEREIN IS CONFIDENTIAL AND/OR PROPRIETARY TO SIGMA DESIGNS
AND MAY BE COVERED BY U.S. AND FOREIGN PATENTS, PATENTS IN PROCESS, AND ARE PROTECTED BY TRADE SECRET
OR COPYRIGHT LAW. DISSEMINATION OR REPRODUCTION OF THE SOURCE CODE CONTAINED HEREIN IS EXPRESSLY FORBIDDEN
TO ANYONE EXCEPT LICENSEES OF SIGMA DESIGNS  WHO HAVE EXECUTED A SIGMA DESIGNS' Z-WAVE CONTROLLER DEVELOPMENT KIT
LIMITED LICENSE AGREEMENT. THE COPYRIGHT NOTICE ABOVE IS NOT EVIDENCE OF ANY ACTUAL OR INTENDED PUBLICATION OF
THE SOURCE CODE. THE RECEIPT OR POSSESSION OF  THIS SOURCE CODE AND/OR RELATED INFORMATION DOES NOT CONVEY OR
IMPLY ANY RIGHTS  TO REPRODUCE, DISCLOSE OR DISTRIBUTE ITS CONTENTS, OR TO MANUFACTURE, USE, OR SELL A PRODUCT
THAT IT  MAY DESCRIBE.


THE SIGMA PROGRAM AND ANY RELATED DOCUMENTATION OR TOOLS IS PROVIDED TO COMPANY "AS IS" AND "WITH ALL FAULTS",
WITHOUT WARRANTY OF ANY KIND FROM SIGMA. COMPANY ASSUMES ALL RISKS THAT LICENSED MATERIALS ARE SUITABLE OR ACCURATE
FOR COMPANY'S NEEDS AND COMPANY'S USE OF THE SIGMA PROGRAM IS AT COMPANY'S OWN DISCRETION AND RISK. SIGMA DOES NOT
GUARANTEE THAT THE USE OF THE SIGMA PROGRAM IN A THIRD PARTY SERVICE ENVIRONMENT OR CLOUD SERVICES ENVIRONMENT WILL BE:
(A) PERFORMED ERROR-FREE OR UNINTERRUPTED; (B) THAT SIGMA WILL CORRECT ANY THIRD PARTY SERVICE ENVIRONMENT OR
CLOUD SERVICE ENVIRONMENT ERRORS; (C) THE THIRD PARTY SERVICE ENVIRONMENT OR CLOUD SERVICE ENVIRONMENT WILL
OPERATE IN COMBINATION WITH COMPANY'S CONTENT OR COMPANY APPLICATIONS THAT UTILIZE THE SIGMA PROGRAM;
(D) OR WITH ANY OTHER HARDWARE, SOFTWARE, SYSTEMS, SERVICES OR DATA NOT PROVIDED BY SIGMA. COMPANY ACKNOWLEDGES
THAT SIGMA DOES NOT CONTROL THE TRANSFER OF DATA OVER COMMUNICATIONS FACILITIES, INCLUDING THE INTERNET, AND THAT
THE SERVICES MAY BE SUBJECT TO LIMITATIONS, DELAYS, AND OTHER PROBLEMS INHERENT IN THE USE OF SUCH COMMUNICATIONS
FACILITIES. SIGMA IS NOT RESPONSIBLE FOR ANY DELAYS, DELIVERY FAILURES, OR OTHER DAMAGE RESULTING FROM SUCH ISSUES.
SIGMA IS NOT RESPONSIBLE FOR ANY ISSUES RELATED TO THE PERFORMANCE, OPERATION OR SECURITY OF THE THIRD PARTY SERVICE
ENVIRONMENT OR CLOUD SERVICES ENVIRONMENT THAT ARISE FROM COMPANY CONTENT, COMPANY APPLICATIONS OR THIRD PARTY CONTENT.
SIGMA DOES NOT MAKE ANY REPRESENTATION OR WARRANTY REGARDING THE RELIABILITY, ACCURACY, COMPLETENESS, CORRECTNESS, OR
USEFULNESS OF THIRD PARTY CONTENT OR SERVICE OR THE SIGMA PROGRAM, AND DISCLAIMS ALL LIABILITIES ARISING FROM OR RELATED
TO THE SIGMA PROGRAM OR THIRD PARTY CONTENT OR SERVICES. TO THE EXTENT NOT PROHIBITED BY LAW, THESE WARRANTIES ARE EXCLUSIVE.
SIGMA OFFERS NO WARRANTY OF NON-INFRINGEMENT, TITLE, OR QUIET ENJOYMENT. NEITHER SIGMA NOR ITS SUPPLIERS OR LICENSORS
SHALL BE LIABLE FOR ANY INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES OR LOSS (INCLUDING DAMAGES FOR LOSS OF
BUSINESS, LOSS OF PROFITS, OR THE LIKE), ARISING OUT OF THIS AGREEMENT WHETHER BASED ON BREACH OF CONTRACT,
INTELLECTUAL PROPERTY INFRINGEMENT, TORT (INCLUDING NEGLIGENCE), STRICT LIABILITY, PRODUCT LIABILITY OR OTHERWISE,
EVEN IF SIGMA OR ITS REPRESENTATIVES HAVE BEEN ADVISED OF OR OTHERWISE SHOULD KNOW ABOUT THE POSSIBILITY OF SUCH DAMAGES.
THERE ARE NO OTHER EXPRESS OR IMPLIED WARRANTIES OR CONDITIONS INCLUDING FOR SOFTWARE, HARDWARE, SYSTEMS, NETWORKS OR
ENVIRONMENTS OR FOR MERCHANTABILITY, NONINFRINGEMENT, SATISFACTORY QUALITY AND FITNESS FOR A PARTICULAR PURPOSE.

The Sigma Program  is not fault-tolerant and is not designed, manufactured or intended for use or resale as on-line control
equipment in hazardous environments requiring fail-safe performance, such as in the operation of nuclear facilities,
aircraft navigation or communication systems, air traffic control, direct life support machines, or weapons systems,
in which the failure of the Sigma Program, or Company Applications created using the Sigma Program, could lead directly
to death, personal injury, or severe physical or environmental damage ("High Risk Activities").  Sigma and its suppliers
specifically disclaim any express or implied warranty of fitness for High Risk Activities.Without limiting Sigma's obligation
of confidentiality as further described in the Z-Wave Controller Development Kit Limited License Agreement, Sigma has no
obligation to establish and maintain a data privacy and information security program with regard to Company's use of any
Third Party Service Environment or Cloud Service Environment. For the avoidance of doubt, Sigma shall not be responsible
for physical, technical, security, administrative, and/or organizational safeguards that are designed to ensure the
security and confidentiality of the Company Content or Company Application in any Third Party Service Environment or
Cloud Service Environment that Company chooses to utilize.
*/
#include <stdlib.h>
#ifndef OS_MAC_X
#include <malloc.h>
#endif
#include <memory.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../include/zip_api_pte.h"
#include "../include/zip_api_util.h"
#include "../include/zip_cmd_lst.h"

/**
@defgroup Util Utility/Miscellaneous Functions
Some utility/miscellaneous functions.
@{
*/

typedef struct
{
    uint8_t                 have_vtype;     /**< flag to indicate whether vendor specific alarm type supported*/
	uint8_t		            type_evt_cnt;   /**< number of entries in type_evt buffer*/
    if_alarm_type_evt_t     type_evt[1];    /**< alarm type-event place holder*/
} if_alarm_data_v16_t;

typedef struct
{
	uint8_t		    setp_cnt;       /**< number of supported setpoint types*/
    zwsetp_range_t  setp_cap[1];    /**< setpoint types and capabilities place holder*/
} if_thrmo_setp_cap_v16_t;

/**
zwutl_buf_init - Initialize dynamic buffer
@param[in] alloc_sz     Memory block size to allocate for initial and subsequent allocation
@return Pointer to dynamic buffer if successful; else return NULL
@post  Caller is required to call zwutl_buf_exit to free the memory allocated if this call is successful
*/
zwutil_buf_t *zwutl_buf_init(uint32_t alloc_sz)
{
    zwutil_buf_t    *dyn_buf;

    dyn_buf = (zwutil_buf_t *)calloc(1, sizeof(zwutil_buf_t));

    if (!dyn_buf)
    {
        return NULL;
    }

    dyn_buf->buf = (uint8_t *)malloc(alloc_sz);

    if (!dyn_buf->buf)
    {
        free(dyn_buf);
        return NULL;
    }

    dyn_buf->alloc_sz = dyn_buf->len = alloc_sz;

    return dyn_buf;
}


/**
zwutl_buf_exit - Clean up dynamic buffer
@param[in] buf     Dynamic buffer
@return
*/
void zwutl_buf_exit(zwutil_buf_t *buf)
{
    free(buf->buf);
    free(buf);
}


/**
zwutl_buf_get_wptr - Get buffer pointer for writing
@param[in] buf     Dynamic buffer
@param[in] size    Size to write in bytes
@return Write buffer pointer if successful; else return NULL
*/
uint8_t *zwutl_buf_get_wptr(zwutil_buf_t *buf, uint32_t size)
{
    uint8_t     *tmp_buf;
    uint32_t    add_sz;

    if (buf->status != 0)
    {
        return NULL;
    }

    if ((buf->wr_ofs + size) < buf->len)
    {   //Enough buffer
        return buf->buf + buf->wr_ofs;
    }

    //Reallocate memory
    add_sz = (size < buf->alloc_sz)? buf->alloc_sz : size;

    tmp_buf = (uint8_t *)realloc(buf->buf, buf->len + add_sz);

    if (!tmp_buf)
    {   //Error, update status
        buf->status = -1;
        return NULL;
    }

    buf->buf = tmp_buf;
    buf->len += add_sz;

    return buf->buf + buf->wr_ofs;

}


/**
zwutl_ni_file_new - Create a new node info file
@param[in]      file_name     file name of the new file
@return		Non-zero on success; otherwise NULL.
@post       Caller must close the file handler if the function returns non-zero file handler.
*/
static FILE *zwutl_ni_file_new(const char *file_name)
{
    FILE *file;

    if (!file_name)
    {
        return NULL;
    }

#ifdef USE_SAFE_VERSION
    if (fopen_s(&file, file_name, "w+b") != 0)
    {
        return NULL;
    }
#else
    file = fopen(file_name, "w+b");
    if (!file)
    {
        return NULL;
    }
#endif

    if (file)
    {   //Write the file magic identifier and version
        uint8_t     file_hdr[4] = {FILE_MAGIC_NUM_H, FILE_MAGIC_NUM_L, FILE_VER_NUM_H, FILE_VER_NUM_L};
        size_t      wr_count;

        wr_count = fwrite(file_hdr, 1, 4, file);
        if (wr_count == 4)
        {
            return file;
        }
        fclose(file);
    }
    return NULL;
}


/**
zwutl_fwrite16 - Write 16-bit data to file in big endian format
@param[in]      file     file
@param[in]      data     16-bit data to be written
@return		Non-zero on success; otherwise zero.
*/
static int zwutl_fwrite16(FILE *file, uint16_t data)
{
    //Write MSB
    if (fputc((int)(data >> 8), file) != EOF )
    {
        //Write LSB
        if (fputc((int)(data & 0xFF), file) != EOF )
        {
            return 1;
        }
    }

    return 0;
}


/**
zwutl_fread16 - Read 16-bit big endian data from file
@param[in]      file     file
@param[out]     data     16-bit data read
@return		Non-zero on success; otherwise zero.
*/
static int zwutl_fread16(FILE *file, uint16_t *data)
{
    int ch;

    //Get MSB
    if ((ch = fgetc(file)) != EOF )
    {
        *data = ch & 0x00FF;
        //Get LSB
        if ((ch = fgetc(file)) != EOF )
        {
            *data = (*data << 8) | (ch & 0x00FF);
            return 1;
        }
    }

    return 0;
}


/**
zwutl_tag_rd - Read a tag and its content
@param[in]      file     node info file handler
@param[out]     tag      tag container on success
@return		Zero on success; otherwise negative error number.
@post       Caller must free the memory allocated to tag on success
*/
static int zwutl_tag_rd(FILE *file, zwtag_p *tag)
{
    uint16_t    tag_id;
    uint16_t    len;
    uint16_t    padding;
    zwtag_p     tag_cont;

    if (zwutl_fread16(file, &tag_id) == 0)
    {
        return ZW_ERR_FILE_EOF;
    }

    if (zwutl_fread16(file, &len) == 0)
    {
        return ZW_ERR_FILE_EOF;
    }

    padding = len & 0x01;   //padding to even number of bytes

    //Allocate memory for the data
    tag_cont = (zwtag_p) calloc(1, sizeof(zwtag_t) + len + padding);

    if (!tag_cont)
    {
        return ZW_ERR_MEMORY;
    }

    if (fread(tag_cont->data, len + padding, 1, file) == 0)
    {
        free(tag_cont);
        return ZW_ERR_FILE_EOF;
    }

    tag_cont->id = tag_id;
    tag_cont->len = len;

    *tag = tag_cont;

    return ZW_ERR_NONE;
}


/**
zwutl_tag_wr - Write a tag and its content into a file
@param[in]  file     node info file handler
@param[in]  id       tag id
@param[in]  len      tag data length
@param[in]  data     tag data buffer
@return		Zero on success; otherwise negative error number.
*/
static int zwutl_tag_wr(FILE *file, uint16_t id, uint16_t len, uint8_t *data)
{

    if (zwutl_fwrite16(file, id) == 0)
    {
        return ZW_ERR_FILE_WRITE;
    }

    if (zwutl_fwrite16(file, len) == 0)
    {
        return ZW_ERR_FILE_WRITE;
    }

    if (fwrite(data, len, 1, file) != 1)
    {
        return ZW_ERR_FILE_WRITE;
    }

    //padding to even number of bytes
    if (len & 0x01)
    {
        if (fputc(0, file) == EOF)
        {
            return ZW_ERR_FILE_WRITE;
        }
    }

    return ZW_ERR_NONE;
}


/**
zwutl_node_load - Load the node sub-tags into zwnode_t structure
@param[in]	nw	        Network
@param[in]	subtag_buf	Buffer that stores the sub-tags
@param[in]	len	        length of subtag_buf
@param[in]	file_ver    opened file version
@param[out]	curr_node   the current node as specified in sub-tag ZW_SUBTAG_NODE_ID
@return         ZW_ERR_XXX.
*/
static int zwutl_node_load(zwnet_p nw, uint8_t *subtag_buf, uint16_t len, uint16_t file_ver, zwnode_p *curr_node)
{
    uint8_t     *subtag;
    uint16_t    subtag_len;
    uint16_t    ext_length_hdr;
    zwnode_p    node;

    subtag = subtag_buf;

    if (len < 3)
    {
        return ZW_ERR_FILE;
    }

    //Check that first sub-tag must be ZW_SUBTAG_NODE_ID
    if ((subtag[0] != ZW_SUBTAG_NODE_ID)
        || (subtag[1] != 1))
    {
        return ZW_ERR_FILE;
    }

    //Search for the node
    node = zwnode_find(&nw->ctl, subtag[2]);
    if (!node)
    {
        return ZW_ERR_NODE_NOT_FOUND;
    }

    node->nodeid = subtag[2];

    //Point to the next sub-tag
    subtag = subtag_buf + 3;
    len -= 3;

    while (len > (ZW_SUBTAG_ID_SIZE + ZW_SUBTAG_LEN_SIZE))
    {
        if (subtag[1] < 0x80)
        {
            subtag_len = subtag[1];
            ext_length_hdr = 0;
        }
        else
        {   //The next 2 bytes become length field
            subtag_len = subtag[2];
            subtag_len = (subtag_len << 8) | subtag[3];
            ext_length_hdr = 2;
        }


        if (len < (ZW_SUBTAG_ID_SIZE + ZW_SUBTAG_LEN_SIZE + ext_length_hdr + subtag_len))
        {
            return ZW_ERR_FILE;
        }

        switch (subtag[0])
        {
            case ZW_SUBTAG_VID:
                node->vid = ((uint16_t)subtag[2]) << 8 | subtag[3];
                break;

            case ZW_SUBTAG_VTYPE:
                node->vtype = ((uint16_t)subtag[2]) << 8 | subtag[3];
                break;

            case ZW_SUBTAG_PID:
                node->pid = ((uint16_t)subtag[2]) << 8 | subtag[3];
                break;

            case ZW_SUBTAG_MULCH_VER:
                node->mul_ch_ver = subtag[2];
                break;

            case ZW_SUBTAG_EP_CNT:
                node->num_of_ep = subtag[2];
                break;

            case ZW_SUBTAG_AGGR_EP_CNT:
                node->num_of_aggr_ep = subtag[2];
                break;

            case ZW_SUBTAG_NODE_PROPTY:
                node->propty = subtag[2];
                break;

            case ZW_SUBTAG_SLEEP_CAP:
                node->enable_cmd_q = node->sleep_cap = subtag[2];//enable command queuing if the node is able to sleep
                break;

            case ZW_SUBTAG_LISTEN:
                node->listen = subtag[2];
                break;
            case ZW_SUBTAG_SE_STATUS:
                 node->security_incl_status = subtag[2];

#ifdef CRC16_ENCAP
//          case ZW_SUBTAG_CRC16_CAP:
//              node->crc_cap = subtag[2];
//              break;
#endif

            case ZW_SUBTAG_DEV_ID_TYPE:
                node->dev_id.type = subtag[2];
                break;

            case ZW_SUBTAG_DEV_ID_FMT:
                node->dev_id.format = subtag[2];
                break;

            case ZW_SUBTAG_DEV_ID_LEN:
                node->dev_id.len = subtag[2];
                break;

            case ZW_SUBTAG_DEV_ID_DAT:
                memset(node->dev_id.dev_id, 0, MAX_DEV_ID_LEN + 1);
                if (subtag_len > MAX_DEV_ID_LEN)
                {
                    subtag_len = MAX_DEV_ID_LEN;
                }
                memcpy(node->dev_id.dev_id, subtag + ZW_SUBTAG_ID_SIZE + ZW_SUBTAG_LEN_SIZE + ext_length_hdr, subtag_len);
                break;

            case ZW_SUBTAG_LIB_TYPE:
                node->lib_type = subtag[2];
                break;

            case ZW_SUBTAG_PROT_VER:
                node->proto_ver = ((uint16_t)subtag[2]) << 8 | subtag[3];
                break;

            case ZW_SUBTAG_APPL_VER:
                node->app_ver = ((uint16_t)subtag[2]) << 8 | subtag[3];
                break;

            case ZW_SUBTAG_MUL_CMD_CAP:
                node->mul_cmd_ctl = node->mul_cmd_cap = subtag[2];    //start multi command encapsulation if the node is multi command capable
                break;

            case ZW_SUBTAG_SENSOR:
                node->sensor = subtag[2];
                break;

            case ZW_SUBTAG_CATEGORY:
                node->category = subtag[2];
                break;

            case ZW_SUBTAG_BASIC:
                node->basic = subtag[2];
                break;

            case ZW_SUBTAG_EXT_VER_CAP:
                if (subtag[2])
                {   //The node supports extended version information
                    if (file_ver <= 0x0010)
                    {
                        break;
                    }

                    node->ext_ver = (ext_ver_t *)calloc(1, sizeof(ext_ver_t) + (subtag[3]*2));
                    if (node->ext_ver)
                    {
                        node->ext_ver->fw_cnt = subtag[3];
                    }
                }
                break;

            case ZW_SUBTAG_HW_VER:
                if (node->ext_ver)
                {
                    node->ext_ver->hw_ver = subtag[2];
                }
                break;

            case ZW_SUBTAG_FW_VER:
                if (node->ext_ver)
                {
                    memcpy(node->ext_ver->fw_ver, subtag + ZW_SUBTAG_ID_SIZE + ZW_SUBTAG_LEN_SIZE + ext_length_hdr,
                           subtag_len);
                }
                break;

            case ZW_SUBTAG_S2_KEY_VALID:
                node->s2_keys_valid = subtag[2];
                break;

            case ZW_SUBTAG_S2_GRNT_KEY:
                node->s2_grnt_keys = subtag[2];
                break;

            case ZW_SUBTAG_DSK_STR:
                memcpy(node->s2_dsk, subtag + ZW_SUBTAG_ID_SIZE + ZW_SUBTAG_LEN_SIZE + ext_length_hdr, subtag_len);
                break;

            case ZW_SUBTAG_WAKEUP_INTV:
                if (file_ver <= 0x0010)
                {
                    node->wkup_intv = -1; //unknown
                    break;
                }

                node->wkup_intv = ((int32_t)subtag[2])<<24 | ((int32_t)subtag[3])<<16
                                    | ((int32_t)subtag[4])<<8 | ((int32_t)subtag[5]);
                break;


            default:
                debug_zwapi_msg(&nw->plt_ctx, "Unknown sub-tag:%02X", subtag[0]);

        }

        //Point to the next sub-tag
        subtag += (ZW_SUBTAG_ID_SIZE + ZW_SUBTAG_LEN_SIZE + ext_length_hdr + subtag_len);
        len -= (ZW_SUBTAG_ID_SIZE + ZW_SUBTAG_LEN_SIZE + ext_length_hdr + subtag_len);
    }

    if (file_ver < 0x000C)
    {   //Older file doesn't has ZW_SUBTAG_LISTEN tag.
        if (node->sensor)
        {
            node->listen = 1;
        }
        else if (node->sleep_cap)
        {
            node->listen = 0;
        }
        else
        {
            node->listen = 1;
        }
    }

    //Copy the current node pointer
    *curr_node = node;
    return ZW_ERR_NONE;
}


/**
zwutl_ep_load - Load the endpoint sub-tags into zwep_t structure
@param[in]	node	    node that contains the endpoint
@param[in]	subtag_buf	Buffer that stores the sub-tags
@param[in]	len	        length of subtag_buf
@param[out]	curr_ep     the current endpoint as specified in sub-tag ZW_SUBTAG_EP_ID
@return         ZW_ERR_XXX.
*/
static int zwutl_ep_load(zwnode_p node, uint8_t *subtag_buf, uint16_t len, zwep_p *curr_ep)
{
    uint8_t     *subtag;
    uint16_t    subtag_len;
    uint16_t    ext_length_hdr;
    zwep_p      ep;

    subtag = subtag_buf;

    if (len < 3)
    {
        return ZW_ERR_FILE;
    }

    //Check that first sub-tag must be ZW_SUBTAG_EP_ID
    if ((subtag[0] != ZW_SUBTAG_EP_ID)
        || (subtag[1] != 1))
    {
        return ZW_ERR_FILE;
    }

    //Check whether this is a virtual endpoint
    if (subtag[2] == VIRTUAL_EP_ID)
    {   //End point has already existed
        ep = &node->ep;
    }
    else
    {   //Create new endpoint
        zwep_p      new_ep;

        new_ep = (zwep_p)calloc(1, sizeof(zwep_t));
        if (!new_ep)
        {
            return ZW_ERR_MEMORY;
        }
        new_ep->node = node;
        new_ep->epid = subtag[2];

        //Add to the endpoint list
        zwobj_add(&node->ep.obj.next, &new_ep->obj);

        ep = new_ep;
    }

    //Point to the next sub-tag
    subtag = subtag_buf + 3;
    len -= 3;

    while (len > (ZW_SUBTAG_ID_SIZE + ZW_SUBTAG_LEN_SIZE))
    {
        if (subtag[1] < 0x80)
        {
            subtag_len = subtag[1];
            ext_length_hdr = 0;
        }
        else
        {   //The next 2 bytes become length field
            subtag_len = subtag[2];
            subtag_len = (subtag_len << 8) | subtag[3];
            ext_length_hdr = 2;
        }

        if (len < (ZW_SUBTAG_ID_SIZE + ZW_SUBTAG_LEN_SIZE + ext_length_hdr + subtag_len))
        {
            return ZW_ERR_FILE;
        }

        switch (subtag[0])
        {
            case ZW_SUBTAG_GEN:
                ep->generic = subtag[2];
                break;

            case ZW_SUBTAG_SPEC:
                ep->specific = subtag[2];
                break;

            case ZW_SUBTAG_EP_NAME:
                memcpy(ep->name, subtag + ZW_SUBTAG_ID_SIZE + ZW_SUBTAG_LEN_SIZE + ext_length_hdr, subtag_len);
                ep->name[subtag_len] = '\0';
                break;

            case ZW_SUBTAG_EP_LOC:
                memcpy(ep->loc, subtag + ZW_SUBTAG_ID_SIZE + ZW_SUBTAG_LEN_SIZE + ext_length_hdr, subtag_len);
                ep->loc[subtag_len] = '\0';
                break;

            case ZW_SUBTAG_ZWPLUS_VER:
                ep->zwplus_info.zwplus_ver = subtag[2];
                break;

            case ZW_SUBTAG_ROLE_TYPE:
                ep->zwplus_info.role_type = subtag[2];
                break;

            case ZW_SUBTAG_NODE_TYPE:
                ep->zwplus_info.node_type = subtag[2];
                break;

            case ZW_SUBTAG_INSTR_ICON:
                ep->zwplus_info.instr_icon = ((uint16_t)subtag[2]) << 8 | subtag[3];
                break;

            case ZW_SUBTAG_USER_ICON:
                ep->zwplus_info.usr_icon = ((uint16_t)subtag[2]) << 8 | subtag[3];
                break;

            case ZW_SUBTAG_AGGR_MEMBER_CNT:
                ep->aggr_ep_cnt = subtag[2];
                break;

            case ZW_SUBTAG_AGGR_MEMBERS:
                memcpy(ep->aggr_members, subtag + ZW_SUBTAG_ID_SIZE + ZW_SUBTAG_LEN_SIZE + ext_length_hdr, subtag_len);
                break;

            default:
                debug_zwapi_msg(&node->net->plt_ctx, "Unknown sub-tag:%02X", subtag[0]);

        }

        //Point to the next sub-tag
        subtag += (ZW_SUBTAG_ID_SIZE + ZW_SUBTAG_LEN_SIZE + ext_length_hdr + subtag_len);
        len -= (ZW_SUBTAG_ID_SIZE + ZW_SUBTAG_LEN_SIZE + ext_length_hdr + subtag_len);
    }

    //Copy the current endpoint pointer
    *curr_ep = ep;
    return ZW_ERR_NONE;
}


/**
zwutil_thrmo_setp_dat_upgd - Upgrade thermostat setpoint data structure
@param[in]	intf	    Interface
@return
*/
static void zwutil_thrmo_setp_dat_upgd(zwif_p intf)
{
    if_thrmo_setp_data_t    *thrmo_setp_dat;
    if_thrmo_setp_cap_v16_t *thrmo_setp_cap;
    unsigned                i;
    uint8_t                 sup_type_cnt;

    thrmo_setp_dat = (if_thrmo_setp_data_t *)intf->data;
    sup_type_cnt = thrmo_setp_dat->setp_len;

    intf->data_item_sz = sizeof(if_thrmo_setp_cap_v16_t) + sizeof(zwsetp_range_t) * (sup_type_cnt - 1);

    //Allocate memory for data
    thrmo_setp_cap = (if_thrmo_setp_cap_v16_t *)calloc(1, intf->data_item_sz);

    if (thrmo_setp_cap)
    {
        thrmo_setp_cap->setp_cnt = sup_type_cnt;
        for (i=0; i<sup_type_cnt; i++)
        {
            thrmo_setp_cap->setp_cap[i].min.type = thrmo_setp_cap->setp_cap[i].max.type = thrmo_setp_dat->setp[i];
        }
        //Free old data structure
        free(thrmo_setp_dat);

        //Assign new data structure to interface
        intf->data = thrmo_setp_cap;
        intf->data_cnt = 1;
    }
    else
    {
        //Free old data structure
        free(thrmo_setp_dat);
        intf->data = NULL;
        intf->data_item_sz = intf->data_cnt = 0;
    }
}


/**
zwutl_cch_dat_conv - Conversion of older interface cached data to the latest format
@param[in]	intf	    interface
@param[in]	file_ver    opened device database file version
@return
*/
static void zwutl_cch_dat_conv(zwif_p intf, uint16_t file_ver)
{
    time_t  *tm_stamp_p;
    time_t  old_tm_stamp;

    switch(intf->cls)
    {
        case COMMAND_CLASS_THERMOSTAT_OPERATING_STATE:
            if (file_ver <= 0x0012)
            {
                uint8_t old_sta;

                //Old format
                tm_stamp_p = (time_t *)intf->cch_data;
                memcpy(&old_sta, tm_stamp_p + 1, sizeof(uint8_t));
                old_tm_stamp = *tm_stamp_p;
                free(intf->cch_data);

                //New format
                intf->cch_data = calloc(1, sizeof(zwthrmo_op_sta_t));
                if (intf->cch_data)
                {
                    zwthrmo_op_sta_t    *op_sta;

                    intf->cch_dat_item_sz = sizeof(zwthrmo_op_sta_t);
                    intf->cch_dat_cnt = 1;
                    //Update cache
                    op_sta = (zwthrmo_op_sta_t *)intf->cch_data;
                    op_sta->op_sta = old_sta;
                    op_sta->tmstamp = old_tm_stamp;
                }

            }
            break;

        case COMMAND_CLASS_THERMOSTAT_MODE:
            if (file_ver <= 0x0012)
            {
                uint8_t old_mode;

                //Old format
                tm_stamp_p = (time_t *)intf->cch_data;
                memcpy(&old_mode, tm_stamp_p + 1, sizeof(uint8_t));
                old_tm_stamp = *tm_stamp_p;
                free(intf->cch_data);

                //New format
                intf->cch_data = calloc(1, sizeof(zwthrmo_mode_t) + sizeof(time_t));
                if (intf->cch_data)
                {
                    zwthrmo_mode_t      *thrmo_md;

                    intf->cch_dat_item_sz = sizeof(zwthrmo_mode_t) + sizeof(time_t);
                    intf->cch_dat_cnt = 1;
                    //Update cache
                    memcpy(intf->cch_data, &old_tm_stamp, sizeof(time_t));
                    thrmo_md = (zwthrmo_mode_t *)(((time_t *)intf->cch_data) + 1);
                    thrmo_md->mode = old_mode;
                }
            }
            break;

        default:
            //Can't convert, free it
            free(intf->cch_data);
            intf->cch_dat_item_sz = intf->cch_dat_cnt = 0;
            intf->cch_data = NULL;
            break;
    }
}


/**
zwutl_if_load - Load the interface sub-tags into zwif_t structure
@param[in]	ep	        endpoint that contains the interface
@param[in]	subtag_buf	Buffer that stores the sub-tags
@param[in]	len	        length of subtag_buf
@param[in]	file_ver    opened file version
@return         ZW_ERR_XXX.
*/
static int zwutl_if_load(zwep_p ep, uint8_t *subtag_buf, uint16_t len, uint16_t file_ver)
{
    zwif_p      intf;
    uint8_t     *subtag;
    uint16_t    subtag_len;
    uint16_t    ext_length_hdr;
    uint16_t    cls;
    uint16_t    tmout;
    int16_t     idx = -1;
    uint8_t     ver;
    uint8_t     propty;
    uint8_t     current_grp_id;
	cch_1lvl_t *pCurr1lvl = NULL;

    subtag = subtag_buf;

    //Check for the total length of class, version, security level sub-tags
    if (len < (3*2 + 4))
    {
        return ZW_ERR_FILE;
    }

    //Check that first sub-tag must be ZW_SUBTAG_CLS
    if ((subtag[0] != ZW_SUBTAG_CLS)
        || (subtag[1] != 2))
    {
        return ZW_ERR_FILE;
    }

    cls = subtag[2];
    cls = (cls << 8) | subtag[3];

    //Check for duplicate interface
    if (zwif_find_cls(ep->intf, cls))
    {   //Duplicate interface, skip it
        return ZW_ERR_NONE;
    }

#ifdef CRC16_ENCAP
    //Check whether the virtual endpoint has CRC-16 Encapsulation command class
    if ((ep->epid == VIRTUAL_EP_ID) && (cls == COMMAND_CLASS_CRC_16_ENCAP))
    {
        ep->node->crc_cap = 1;
    }
#endif
    //Check whether the virtual endpoint has supervision command class
    if ((ep->epid == VIRTUAL_EP_ID) && (cls == COMMAND_CLASS_SUPERVISION))
    {
        ep->node->spv_cap = 1;
    }


    //Point to the next sub-tag
    subtag += 4;
    len -= 4;

    //Check that second sub-tag must be ZW_SUBTAG_VER
    if ((subtag[0] != ZW_SUBTAG_VER)
        || (subtag[1] != 1))
    {
        return ZW_ERR_FILE;
    }

    ver = subtag[2];

    //Point to the next sub-tag
    subtag += 3;
    len -= 3;

    //Check that second sub-tag must be ZW_SUBTAG_PROPTY
    if ((subtag[0] != ZW_SUBTAG_PROPTY)
        || (subtag[1] != 1))
    {
        return ZW_ERR_FILE;
    }

    zwnet_dev_if_propty_tmout_get(ep, cls, ver, subtag[2], &propty, &tmout);

    //Create new interface
    intf = zwif_create(cls, ver, propty);

    if (!intf)
    {
        return ZW_ERR_MEMORY;
    }

    //Update timeout value
    intf->tmout = tmout;

    //Save back link to endpoint
    intf->ep = ep;

    //Add interface to the endpoint
    if (ep->intf)
    {   //Second interface onwards
        zwobj_add(&ep->intf->obj.next, &intf->obj);
    }
    else
    {   //First interface
        ep->intf = intf;
    }

    //Point to the next sub-tag
    subtag += 3;
    len -= 3;

    //Initialize group id of AGI
    current_grp_id = 0;

    while (len > (ZW_SUBTAG_ID_SIZE + ZW_SUBTAG_LEN_SIZE))
    {
        if (subtag[1] < 0x80)
        {
            subtag_len = subtag[1];
            ext_length_hdr = 0;
        }
        else
        {   //The next 2 bytes become length field
            subtag_len = subtag[2];
            subtag_len = (subtag_len << 8) | subtag[3];
            ext_length_hdr = 2;
        }

        if (len < (ZW_SUBTAG_ID_SIZE + ZW_SUBTAG_LEN_SIZE + ext_length_hdr + subtag_len))
        {
            return ZW_ERR_FILE;
        }

        switch (subtag[0])
        {
            case ZW_SUBTAG_DAT_CNT:
                //Remove any pre-allocated memory for data item
                //NOTE: zwif_create() may have already allocated memory for default interface data value.
                if (intf->data_cnt && intf->data)
                {
                    free(intf->data);
                    intf->data = NULL;
                }
                intf->data_cnt = subtag[2];
                break;

            case ZW_SUBTAG_DAT_ITEM_SZ:
                intf->data_item_sz = subtag[2];
                break;

            case ZW_SUBTAG_DATA:
                if (file_ver <= 0x000F)
                {
                    if (intf->cls == COMMAND_CLASS_CENTRAL_SCENE)
                    {   //Ignore it as the data structure has changed
                        intf->data_cnt = 0;
                        intf->data_item_sz = 0;
                        break;
                    }
                }

                intf->data = malloc(subtag_len);

                if (intf->data)
                {
                    memcpy(intf->data, subtag + ZW_SUBTAG_ID_SIZE + ZW_SUBTAG_LEN_SIZE + ext_length_hdr,
                           subtag_len);
                }
                else
                {
                    intf->data_cnt = intf->data_item_sz = 0;
                }

                if (file_ver <= 0x000E)
                {
                    if ((intf->cls == COMMAND_CLASS_THERMOSTAT_SETPOINT)
                        && intf->data)
                    {   //Convert the old data structure to the new one
                        zwutil_thrmo_setp_dat_upgd(intf);
                    }
                }

                if (intf->cls == COMMAND_CLASS_CONFIGURATION)
                {
                    if (intf->data)
                    {
                        if (intf->data_cnt && (intf->data_item_sz == sizeof(zwcfg_info_cap_t)))
                        {
                            zwcfg_info_cap_p    cfg_info = (zwcfg_info_cap_p)intf->data;

                            if (cfg_info->cnt)
                            {   //Allocate memory for all the parameters info
                                cfg_info->info = (zwcfg_info_p)calloc(cfg_info->cnt, sizeof(zwcfg_info_t));

                                if (!cfg_info->info)
                                {   //Failed to allocate memory
                                    free(intf->data);
                                    intf->data = NULL;
                                    intf->data_cnt = intf->data_item_sz = 0;
                                }
                            }
                        }
                        else
                        {   //Invalid, the zwcfg_info_cap_t structure has been changed
                            free(intf->data);
                            intf->data = NULL;
                            intf->data_cnt = intf->data_item_sz = 0;
                        }
                    }
                }
                else if (intf->cls == COMMAND_CLASS_ALARM)
                {
                    if ((file_ver <= 0x0010) && intf->data)
                    {
                        if_alarm_data_t   *new_cap = NULL;
                        uint8_t	          data_item_sz = 0;

                        if (intf->data_cnt)
                        {
                            unsigned	        i;
                            if_alarm_data_v16_t *cap_v16 = (if_alarm_data_v16_t *)intf->data;

                            data_item_sz = sizeof(if_alarm_data_t) + ((cap_v16->type_evt_cnt - 1)*sizeof(if_alarm_type_evt_t));

                            new_cap = (if_alarm_data_t *)calloc(1, data_item_sz);

                            if (new_cap)
                            {   //Copy to new structure
                                new_cap->have_vtype = cap_v16->have_vtype;
                                new_cap->type_evt_cnt = cap_v16->type_evt_cnt;

                                //Copy supported event types
                                for (i=0; i<cap_v16->type_evt_cnt; i++)
                                {
                                    new_cap->type_evt[i] = cap_v16->type_evt[i];
                                }
                            }
                        }

                        //Free the old interface data
                        free(intf->data);
                        intf->data = NULL;
                        intf->data_cnt = intf->data_item_sz = 0;

                        if (new_cap)
                        {   //Assign new interface data
                            intf->data = new_cap;
                            intf->data_cnt = 1;
                            intf->data_item_sz = data_item_sz;
                        }
                    }
                }
                else if (intf->cls == COMMAND_CLASS_THERMOSTAT_SETPOINT)
                {
                    if ((file_ver <= 0x0010) && intf->data)
                    {
                        if_thrmo_setp_cap_t *new_cap = NULL;
                        uint8_t             data_item_sz = 0;

                        if (intf->data_cnt)
                        {
                            unsigned	            i;
                            if_thrmo_setp_cap_v16_t *cap_v16 = (if_thrmo_setp_cap_v16_t *)intf->data;

                            data_item_sz = sizeof(if_thrmo_setp_cap_t) + ((cap_v16->setp_cnt - 1)*sizeof(zwsetp_range_t));

                            new_cap = (if_thrmo_setp_cap_t *)calloc(1, data_item_sz);

                            if (new_cap)
                            {   //Copy to new structure
                                new_cap->setp_cnt = cap_v16->setp_cnt;

                                //Copy supported event types
                                for (i=0; i<cap_v16->setp_cnt; i++)
                                {
                                    new_cap->setp_cap[i] = cap_v16->setp_cap[i];
                                }
                            }
                        }

                        //Free the old interface data
                        free(intf->data);
                        intf->data = NULL;
                        intf->data_cnt = intf->data_item_sz = 0;

                        if (new_cap)
                        {   //Assign new interface data
                            intf->data = new_cap;
                            intf->data_cnt = 1;
                            intf->data_item_sz = data_item_sz;
                        }
                    }
                }

                break;

			case ZW_SUBTAG_CCH_B2LVL:
                intf->b2lvlcch = subtag[2];
                break;

            case ZW_SUBTAG_CCH_DAT_CNT:
                intf->cch_dat_cnt = subtag[2];
                break;

            case ZW_SUBTAG_CCH_DAT_ITEM_SZ:
                intf->cch_dat_item_sz = subtag[2];
                break;

			case ZW_SUBTAG_CCH_1LVL_ELE_SZ:
				{
					if(!intf->cch_data
                       && intf->cch_dat_item_sz
                       && intf->cch_dat_cnt)
					{
						intf->cch_data = calloc(intf->cch_dat_cnt, intf->cch_dat_item_sz);

						if(intf->cch_data)
							pCurr1lvl = (cch_1lvl_t *)intf->cch_data;
					}

					if(pCurr1lvl)
					{
						pCurr1lvl->cch_ele_sz = subtag[2];
						pCurr1lvl->pcch_ele = calloc(1, subtag[2]);
					}
				}
                break;

            case ZW_SUBTAG_CCH_DATA:
				//file_ver 000A (0.10) onwards added 2-level cached data structure to store array of variable-size data
				//such as alarm cc
				if(intf->b2lvlcch)
				{   //2-level cache
					if(pCurr1lvl)
					{
						memcpy(pCurr1lvl->pcch_ele, subtag + ZW_SUBTAG_ID_SIZE + ZW_SUBTAG_LEN_SIZE + ext_length_hdr,
                               subtag_len);

						pCurr1lvl++;
					}
					break;
				}
				else
				{   //1-level cache
					if (file_ver <= 0x0008)
					{
						if (intf->cls == COMMAND_CLASS_DOOR_LOCK)
						{   //Use the old data structure size but allocate the new data structure size
							intf->cch_dat_cnt = 1;
							intf->cch_dat_item_sz = sizeof(time_t) + sizeof(if_dlck_cch_data_t);//new size
							intf->cch_data = calloc(1, intf->cch_dat_item_sz);
							if (intf->cch_data)
							{
								memcpy(intf->cch_data, subtag + ZW_SUBTAG_ID_SIZE + ZW_SUBTAG_LEN_SIZE + ext_length_hdr,
									subtag_len);
							}
							break;
						}
					}

					if (file_ver <= 0x0011)
					{
						if (intf->cls == COMMAND_CLASS_BASIC
                            || intf->cls == COMMAND_CLASS_DOOR_LOCK
                            || intf->cls == COMMAND_CLASS_SWITCH_MULTILEVEL
                            || intf->cls == COMMAND_CLASS_SWITCH_BINARY)
						{   //Remove the cache as format has changed
							intf->cch_dat_item_sz = intf->cch_dat_cnt = 0;
							intf->cch_data = NULL;
							break;
						}
					}

					intf->cch_data = malloc(subtag_len);
					if (intf->cch_data)
					{
						memcpy(intf->cch_data, subtag + ZW_SUBTAG_ID_SIZE + ZW_SUBTAG_LEN_SIZE + ext_length_hdr,
							   subtag_len);

                        if (file_ver <= 0x0012)
                        {
                            if (intf->cls == COMMAND_CLASS_THERMOSTAT_OPERATING_STATE
                                || intf->cls == COMMAND_CLASS_THERMOSTAT_MODE)
                            {   //Convert to new format
                                zwutl_cch_dat_conv(intf, file_ver);
                                break;
                            }
                        }
					}
				}
                break;

            case ZW_SUBTAG_AGI_GRP_CNT:
                {
                    if_grp_info_dat_t   *grp_data;
                    uint16_t	        group_cnt;

                    if (file_ver <= 0x0010)
                    {
                        break;
                    }

                    group_cnt = subtag[2];
                    group_cnt = (group_cnt << 8) | subtag[3];

                    grp_data = (if_grp_info_dat_t *)calloc(1, sizeof(if_grp_info_dat_t) +
                                                           (sizeof(zw_grp_info_p) * group_cnt));
                    if (grp_data)
                    {
                        grp_data->group_cnt = group_cnt;
                        intf->tmp_data = grp_data;
                    }
                }
                break;

            case ZW_SUBTAG_AGI_GRP_DYNAMIC:
                if (intf->tmp_data)
                {
                    if_grp_info_dat_t   *grp_data = (if_grp_info_dat_t *)intf->tmp_data;

                    grp_data->dynamic = subtag[2];
                }
                break;

            case ZW_SUBTAG_AGI_GRP_ID:
                if (intf->tmp_data)
                {
                    if_grp_info_dat_t   *grp_data = (if_grp_info_dat_t *)intf->tmp_data;

                    if (subtag[2] <= grp_data->group_cnt)
                    {
                        current_grp_id = subtag[2];
                    }
                }
                break;

            case ZW_SUBTAG_AGI_CMD_CNT:
                if (intf->tmp_data)
                {
                    if_grp_info_dat_t   *grp_data = (if_grp_info_dat_t *)intf->tmp_data;

                    if (current_grp_id)
                    {
                        zw_grp_info_p       grp_info_ent;

                        grp_info_ent = (zw_grp_info_p)calloc(1, sizeof(zw_grp_info_t) +
                                                         (subtag[2] * sizeof(grp_cmd_ent_t)));

                        if (grp_info_ent)
                        {
                            grp_info_ent->grp_num = current_grp_id;
                            grp_info_ent->cmd_ent_cnt = subtag[2];
                            grp_data->grp_info[current_grp_id - 1] = grp_info_ent;
                            grp_data->valid_grp_cnt++;
                        }
                    }
                }
                break;

            case ZW_SUBTAG_AGI_PROFILE:
                if (intf->tmp_data)
                {
                    if_grp_info_dat_t   *grp_data = (if_grp_info_dat_t *)intf->tmp_data;

                    if (current_grp_id)
                    {
                        zw_grp_info_p       grp_info_ent;

                        grp_info_ent = grp_data->grp_info[current_grp_id - 1];

                        if (grp_info_ent)
                        {
                            grp_info_ent->profile = subtag[2];
                            grp_info_ent->profile = (grp_info_ent->profile << 8) | subtag[3];
                        }
                    }
                }
                break;

            case ZW_SUBTAG_AGI_EVENT:
                if (intf->tmp_data)
                {
                    if_grp_info_dat_t   *grp_data = (if_grp_info_dat_t *)intf->tmp_data;

                    if (current_grp_id)
                    {
                        zw_grp_info_p       grp_info_ent;

                        grp_info_ent = grp_data->grp_info[current_grp_id - 1];

                        if (grp_info_ent)
                        {
                            grp_info_ent->evt_code = subtag[2];
                            grp_info_ent->evt_code = (grp_info_ent->evt_code << 8) | subtag[3];
                        }
                    }
                }
                break;

            case ZW_SUBTAG_AGI_GRP_NAME:
                if (intf->tmp_data)
                {
                    if_grp_info_dat_t   *grp_data = (if_grp_info_dat_t *)intf->tmp_data;

                    if (current_grp_id)
                    {
                        zw_grp_info_p       grp_info_ent;

                        grp_info_ent = grp_data->grp_info[current_grp_id - 1];

                        if (grp_info_ent)
                        {
                            memcpy(grp_info_ent->name, subtag + ZW_SUBTAG_ID_SIZE + ZW_SUBTAG_LEN_SIZE, subtag_len);
                            grp_info_ent->name[subtag_len] = '\0';
                        }
                    }
                }
                break;

            case ZW_SUBTAG_AGI_CMD_LIST:
                if (intf->tmp_data)
                {
                    if_grp_info_dat_t   *grp_data = (if_grp_info_dat_t *)intf->tmp_data;

                    if (current_grp_id)
                    {
                        zw_grp_info_p       grp_info_ent;

                        grp_info_ent = grp_data->grp_info[current_grp_id - 1];

                        if (grp_info_ent)
                        {
                            memcpy(grp_info_ent->cmd_lst, subtag + ZW_SUBTAG_ID_SIZE + ZW_SUBTAG_LEN_SIZE + ext_length_hdr,
                                   subtag_len);
                        }
                    }
                }
                break;

            case ZW_SUBTAG_CFG_INFO_STRUCT:
                if (intf->data && (intf->data_item_sz == sizeof(zwcfg_info_cap_t)))
                {
                    zwcfg_info_cap_p    cfg_info = (zwcfg_info_cap_p)intf->data;

                    idx++;//Increment index to info array here as the other sub-tags (ZW_SUBTAG_CFG_NAME and ZW_SUBTAG_CFG_INFO) may not exist.

                    if ((idx < cfg_info->cnt) && (subtag_len == sizeof(zwcfg_info_t)))
                    {
                        memcpy(&cfg_info->info[idx], subtag + ZW_SUBTAG_ID_SIZE + ZW_SUBTAG_LEN_SIZE, subtag_len);
                    }
                }
                break;

            case ZW_SUBTAG_CFG_NAME:
                if (intf->data && (intf->data_item_sz == sizeof(zwcfg_info_cap_t)))
                {
                    zwcfg_info_cap_p    cfg_info = (zwcfg_info_cap_p)intf->data;

                    if (idx < cfg_info->cnt)
                    {
                        cfg_info->info[idx].name = (char *)malloc(subtag_len);
                        if (cfg_info->info[idx].name)
                        {
                            memcpy(cfg_info->info[idx].name, subtag + ZW_SUBTAG_ID_SIZE + ZW_SUBTAG_LEN_SIZE, subtag_len);
                        }
                    }
                }
                break;

            case ZW_SUBTAG_CFG_INFO:
                if (intf->data && (intf->data_item_sz == sizeof(zwcfg_info_cap_t)))
                {
                    zwcfg_info_cap_p    cfg_info = (zwcfg_info_cap_p)intf->data;

                    if (idx < cfg_info->cnt)
                    {
                        cfg_info->info[idx].info = (char *)malloc(subtag_len);
                        if (cfg_info->info[idx].info)
                        {
                            memcpy(cfg_info->info[idx].info, subtag + ZW_SUBTAG_ID_SIZE + ZW_SUBTAG_LEN_SIZE, subtag_len);
                        }
                    }
                }
                break;

            default:
                debug_zwapi_msg(&intf->ep->node->net->plt_ctx, "Unknown sub-tag:%02X", subtag[0]);

        }

        //Point to the next sub-tag
        subtag += (ZW_SUBTAG_ID_SIZE + ZW_SUBTAG_LEN_SIZE + ext_length_hdr + subtag_len);
        len -= (ZW_SUBTAG_ID_SIZE + ZW_SUBTAG_LEN_SIZE + ext_length_hdr + subtag_len);
    }

    return ZW_ERR_NONE;
}


/**
zwutl_file_add_crc - Append CRC16 checksum at the end of file
@param[in]	file	    File
@param[in]	file_len	File length
@return		Non-zero on success; otherwise zero.
*/
static int zwutl_file_add_crc(FILE *file, long file_len)
{
    int         ch;
    uint16_t    crc;
    uint8_t     tmp_data;
    uint8_t     bitmask;
    uint8_t     new_bit;

    crc = CRC_INIT;

    while (file_len-- > 0)
    {
        if ((ch = fgetc(file)) == EOF)
        {
            return 0;
        }

        tmp_data = ch;
        for (bitmask = 0x80; bitmask != 0; bitmask >>= 1)
        {
            //Align test bit with next bit of the message byte, starting with msb.
            new_bit = ((tmp_data & bitmask) != 0) ^ ((crc & 0x8000) != 0);
            crc <<= 1;
            if (new_bit)
            {
                crc ^= POLY;
            }
        }
    }
    /* According to fopen man page: It is good practice (and indeed sometimes necessary  under  Linux)  to
       put an fseek(3) or fgetpos(3) operation between write and read operations on such a stream.  This opera-
       tion may be an apparent no-op (as in fseek(..., 0L, SEEK_CUR) called for its synchronizing side effect.
       */
    fseek(file, 0L, SEEK_CUR);

    return zwutl_fwrite16(file, crc);
}


/**
zwutl_file_chk - check file integrity
@param[in]	file	    File to be checked
@return         zero if check o.k.; non-zero on error
*/
static uint16_t zwutl_file_chk(FILE *file)
{
    int         ch;
    uint16_t    crc;
    uint8_t     tmp_data;
    uint8_t     bitmask;
    uint8_t     new_bit;

    crc = CRC_INIT;

    while ((ch = fgetc(file)) != EOF )
    {
        tmp_data = ch;
        for (bitmask = 0x80; bitmask != 0; bitmask >>= 1)
        {
            //Align test bit with next bit of the message byte, starting with msb.
            new_bit = ((tmp_data & bitmask) != 0) ^ ((crc & 0x8000) != 0);
            crc <<= 1;
            if (new_bit)
            {
                crc ^= POLY;
            }
        }
    }
    return crc;
}


/**
zwutl_file_crc_get - get file CRC checksum
@param[in]	file	 File to be checked
@return         CRC checksum
*/
uint16_t zwutl_file_crc_get(FILE *file)
{
    uint16_t    crc;

    crc = zwutl_file_chk(file);

    //Rewind the file pointer
    fseek(file, 0L, SEEK_SET);

    return crc;
}


/**
zwutl_ni_load - Load node information from file
@param[in]	nw	        Network
@param[in]	ni_file	    Node information file
@return         ZW_ERR_XXX.
*/
int zwutl_ni_load(zwnet_p nw, const char *ni_file)
{
    int         result;
    int         res;
    FILE        *file;
    zwtag_p     tag;
    zwnode_p    curr_node;      //current node
    zwep_p      curr_ep;        //current endpoint
    size_t      rd_count;
    uint8_t     file_hdr[4];
    uint16_t    file_ver;
    uint16_t    curr_ver;


    //Open the node info file
    if (!ni_file)
    {
        return ZW_ERR_FILE_OPEN;
    }

#ifdef USE_SAFE_VERSION
    if (fopen_s(&file, ni_file, "r+b") != 0)
    {
        return ZW_ERR_FILE_OPEN;
    }
#else
    file = fopen(ni_file, "r+b");
    if (!file)
    {
        return ZW_ERR_FILE_OPEN;
    }
#endif

    //Verify the file magic identifier
    rd_count = fread(file_hdr, 1, 4, file);

    if ((rd_count == 4)
        && (file_hdr[0] == FILE_MAGIC_NUM_H)
        && (file_hdr[1] == FILE_MAGIC_NUM_L))
    {
        file_ver = file_hdr[2];
        file_ver = (file_ver << 8) | file_hdr[3];
    }
    else
    {
        fclose(file);
        return ZW_ERR_FILE;
    }

    //Check for file version
    curr_ver = FILE_VER_NUM_H;
    curr_ver = (curr_ver << 8) | FILE_VER_NUM_L;
    if ((file_ver == 0) || (file_ver > curr_ver))
    {
        fclose(file);
        return ZW_ERR_VERSION;
    }

    //Check CRC
    fseek(file, 0L, SEEK_SET);

    if (zwutl_file_chk(file) != 0)
    {
        fclose(file);
        return ZW_ERR_FILE;
    }

    if (fseek(file, NI_FILE_HDR_LEN, SEEK_SET) != 0)
    {
        fclose(file);
        return ZW_ERR_FILE;
    }

    //Check whether the home id is matched
    result = zwutl_tag_rd(file, &tag);
    if (result < 0)
    {
        goto l_ZWUTIL_LOAD_ERROR1;
    }

    //Check that first tag must be ZW_TAG_NW and
    //first sub-tag must be ZW_SUBTAG_HOME_ID
    if (tag->id != ZW_TAG_NW)
    {
        result = ZW_ERR_FILE;
        goto l_ZWUTIL_LOAD_ERROR2;
    }

    if ((tag->len >= (ZW_SUBTAG_ID_SIZE + ZW_SUBTAG_LEN_SIZE + 4))
        && (tag->data[0] == ZW_SUBTAG_HOME_ID)
        && (tag->data[1] == 4))
    {
        uint32_t    home_id;
        uint8_t    *ptr;

        //Point to the payload of sub-tag
        ptr = tag->data + ZW_SUBTAG_ID_SIZE + ZW_SUBTAG_LEN_SIZE;
        home_id = ((uint32_t)ptr[0])<<24 | ((uint32_t)ptr[1])<<16
                   | ((uint32_t)ptr[2])<<8 | ((uint32_t)ptr[3]);

        //Check whether home id is correct
        if (home_id != nw->homeid)
        {
            result = ZW_ERR_FILE_HOME_ID;
            goto l_ZWUTIL_LOAD_ERROR2;
        }

        //Process other network sub-tag
        tag->len -= (ZW_SUBTAG_ID_SIZE + ZW_SUBTAG_LEN_SIZE + 4);
        ptr += 4;

        if ((tag->len >= (ZW_SUBTAG_ID_SIZE + ZW_SUBTAG_LEN_SIZE + 1))
            && (ptr[0] == ZW_SUBTAG_GRP_NODE)
            && (ptr[1] == 1))
        {
            nw->stAGIData.pAGIGroupList->stNodeEpInfo.byNodeID = ptr[2];
        }
    }
    else
    {
        result = ZW_ERR_FILE;
        goto l_ZWUTIL_LOAD_ERROR2;
    }

    free(tag);

    //
    //Process other tags
    //

    curr_node = NULL;
    curr_ep = NULL;

    plt_mtx_lck(nw->mtx);
    while((result = zwutl_tag_rd(file, &tag)) == ZW_ERR_NONE)
    {
        switch (tag->id)
        {
            case ZW_TAG_NODE:
                res = zwutl_node_load(nw, tag->data, tag->len, file_ver, &curr_node);
                if (res == ZW_ERR_NODE_NOT_FOUND)
                {   //Node not found is considered o.k., this just means the controller's
                    //routing table is not up-to-date.
                    curr_node = NULL;
                    curr_ep = NULL;
                    res = ZW_ERR_NONE;
                }
                break;

            case ZW_TAG_EP:
                if (curr_node)
                {
                    res = zwutl_ep_load(curr_node, tag->data, tag->len, &curr_ep);
                }
                else
                {   //Node not found, skip the endpoint details
                    res = ZW_ERR_NONE;
                }
                break;

            case ZW_TAG_IF:
                if (curr_ep)
                {
                    res = zwutl_if_load(curr_ep, tag->data, tag->len, file_ver);
                }
                else
                {   //End point not found, skip the interface details
                    res = ZW_ERR_NONE;
                }
                break;

            default:
                res = ZW_ERR_NONE;
                debug_zwapi_msg(&nw->plt_ctx, "Unknown tag:%04X", tag->id);

        }
        if (res < 0)
        {
            result = res;
            goto l_ZWUTIL_LOAD_ERROR3;
        }
        free(tag);
    }
    plt_mtx_ulck(nw->mtx);

    if (result == ZW_ERR_FILE_EOF)
    {   //Reading at end of file is considered o.k.
        result = ZW_ERR_NONE;
    }
    fclose(file);
    return result;

l_ZWUTIL_LOAD_ERROR3:
    plt_mtx_ulck(nw->mtx);
l_ZWUTIL_LOAD_ERROR2:
    free(tag);
l_ZWUTIL_LOAD_ERROR1:
    fclose(file);
    return result;

}


/**
zwutl_subtag_wr32 - Write a 32-bit sub-tag data into buffer
@param[in]      id          id of the sub-tag
@param[in]      data        data to be written
@param[out]     buf         buffer to be written
@return
*/
static void zwutl_subtag_wr32(uint8_t id, uint32_t data, zwutil_buf_t *buf)
{
    uint8_t     *subtag;
    uint8_t     subtag_len;

    subtag_len = (ZW_SUBTAG_ID_SIZE + ZW_SUBTAG_LEN_SIZE + 4);
    subtag = zwutl_buf_get_wptr(buf, subtag_len);
    if (subtag)
    {
        subtag[0] = id;
        subtag[1] = 4;
        subtag[2] = data >> 24;
        subtag[3] = (data >> 16) & 0xFF;
        subtag[4] = (data >> 8) & 0xFF;
        subtag[5] = data & 0xFF;

        //Adjustment
        buf->wr_ofs += subtag_len;
    }
}


/**
zwutl_subtag_wr16 - Write a 16-bit sub-tag data into buffer
@param[in]      id          id of the sub-tag
@param[in]      data        data to be written
@param[out]     buf         buffer to be written
@return
*/
static void zwutl_subtag_wr16(uint8_t id, uint16_t data, zwutil_buf_t *buf)
{
    uint8_t     *subtag;
    uint8_t     subtag_len;

    subtag_len = (ZW_SUBTAG_ID_SIZE + ZW_SUBTAG_LEN_SIZE + 2);
    subtag = zwutl_buf_get_wptr(buf, subtag_len);
    if (subtag)
    {
        subtag[0] = id;
        subtag[1] = 2;
        subtag[2] = data >> 8;
        subtag[3] = data & 0xFF;

        //Adjustment
        buf->wr_ofs += subtag_len;
    }
}


/**
zwutl_subtag_wr8 - Write a 8-bit sub-tag data into buffer
@param[in]      id          id of the sub-tag
@param[in]      data        data to be written
@param[out]     buf         buffer to be written
@return
*/
static void zwutl_subtag_wr8(uint8_t id, uint8_t data, zwutil_buf_t *buf)
{
    uint8_t     *subtag;
    uint8_t     subtag_len;

    subtag_len = (ZW_SUBTAG_ID_SIZE + ZW_SUBTAG_LEN_SIZE + 1);
    subtag = zwutl_buf_get_wptr(buf, subtag_len);
    if (subtag)
    {
        subtag[0] = id;
        subtag[1] = 1;
        subtag[2] = data;

        //Adjustment
        buf->wr_ofs += subtag_len;
    }

}


/**
zwutl_subtag_wr - Write sub-tag data into buffer
@param[in]      id          id of the sub-tag
@param[in]      data        data to be written
@param[in]      data_len    size of data
@param[out]     buf         buffer to be written
@return
*/
static void zwutl_subtag_wr(uint8_t id, uint16_t data_len, uint8_t *data, zwutil_buf_t *buf)
{
    int         ext_length_hdr; //Extended length header size
    uint8_t     *subtag;
    uint16_t    subtag_len;

    //Only write sub-tag if there is data
    if (data_len == 0)
    {
        return;
    }

    ext_length_hdr = (data_len < 0x80)? 0 : 2;
    subtag_len = (ZW_SUBTAG_ID_SIZE + ZW_SUBTAG_LEN_SIZE + ext_length_hdr + data_len);
    subtag = zwutl_buf_get_wptr(buf, subtag_len);
    if (subtag)
    {
        subtag[0] = id;
        if (ext_length_hdr == 0)
        {
            subtag[1] = (uint8_t)data_len;
        }
        else
        {   //The next 2 bytes become the length field
            subtag[1] = 0x80;//Set flag
            subtag[2] = data_len >> 8;//MSB of length
            subtag[3] = data_len & 0x00FF;//LSB of length
        }

        //Copy data
        if (data_len == 1)
        {
            subtag[2] = *data;
        }
        else
        {
            memcpy(subtag + 2 + ext_length_hdr, data, data_len);
        }

        //Adjustment
        buf->wr_ofs += subtag_len;
    }
}


/**
zwutl_ni_save - Save node information into file
@param[in]	nw	        Network
@param[in]	ni_file	    Node information file
@return         ZW_ERR_XXX.
*/
int zwutl_ni_save(zwnet_p nw, const char *ni_file)
{
    int         result;
    FILE        *file;
    zwutil_buf_t *dbuf;
    zwnode_p    curr_node;      //current node
    zwep_p      curr_ep;        //current endpoint
    zwif_p      curr_intf;      //current interface
    long        file_len;
    uint16_t    tag_id;


    //Create a node info file
    file = zwutl_ni_file_new(ni_file);
    if (!file)
    {
        return ZW_ERR_FILE_OPEN;
    }

    //Allocate memory for tag
    dbuf = zwutl_buf_init(MAX_SUBTAGS_SIZE);
    if (!dbuf)
    {
        result = ZW_ERR_MEMORY;
        goto l_ZWUTIL_SAVE_ERROR1;
    }

    //Write home id
    tag_id = ZW_TAG_NW;

    zwutl_subtag_wr32(ZW_SUBTAG_HOME_ID, nw->homeid, dbuf);

    //Write group 1 node id
    zwutl_subtag_wr8(ZW_SUBTAG_GRP_NODE, nw->stAGIData.pAGIGroupList->stNodeEpInfo.byNodeID, dbuf);

    result = zwutl_tag_wr(file, tag_id, dbuf->wr_ofs, dbuf->buf);
    if (result < 0)
    {
        goto l_ZWUTIL_SAVE_ERROR2;
    }

    //Write node information
    curr_node = &nw->ctl;

    plt_mtx_lck(nw->mtx);

    while (curr_node)
    {   //----------------------------------------
        //Write node tag
        //----------------------------------------
        tag_id = ZW_TAG_NODE;
        dbuf->wr_ofs = 0;

        zwutl_subtag_wr8(ZW_SUBTAG_NODE_ID, curr_node->nodeid, dbuf);
        zwutl_subtag_wr16(ZW_SUBTAG_VID, curr_node->vid, dbuf);
        zwutl_subtag_wr16(ZW_SUBTAG_VTYPE, curr_node->vtype, dbuf);
        zwutl_subtag_wr16(ZW_SUBTAG_PID, curr_node->pid, dbuf);
        zwutl_subtag_wr8(ZW_SUBTAG_MULCH_VER, curr_node->mul_ch_ver, dbuf);
        zwutl_subtag_wr8(ZW_SUBTAG_EP_CNT, curr_node->num_of_ep, dbuf);
        zwutl_subtag_wr8(ZW_SUBTAG_AGGR_EP_CNT, curr_node->num_of_aggr_ep, dbuf);
        zwutl_subtag_wr8(ZW_SUBTAG_NODE_PROPTY, curr_node->propty, dbuf);
        zwutl_subtag_wr8(ZW_SUBTAG_SLEEP_CAP, curr_node->sleep_cap, dbuf);
        zwutl_subtag_wr8(ZW_SUBTAG_LISTEN, curr_node->listen, dbuf);
        zwutl_subtag_wr8(ZW_SUBTAG_SE_STATUS, curr_node->security_incl_status, dbuf);
#ifdef CRC16_ENCAP
//      zwutl_subtag_wr8(ZW_SUBTAG_CRC16_CAP, curr_node->crc_cap, dbuf);
#endif
        zwutl_subtag_wr8(ZW_SUBTAG_DEV_ID_LEN, curr_node->dev_id.len, dbuf);
        if (curr_node->dev_id.len > 0)
        {
            zwutl_subtag_wr8(ZW_SUBTAG_DEV_ID_TYPE, curr_node->dev_id.type, dbuf);
            zwutl_subtag_wr8(ZW_SUBTAG_DEV_ID_FMT, curr_node->dev_id.format, dbuf);
            zwutl_subtag_wr(ZW_SUBTAG_DEV_ID_DAT, curr_node->dev_id.len, (uint8_t *)curr_node->dev_id.dev_id, dbuf);
        }
        zwutl_subtag_wr8(ZW_SUBTAG_LIB_TYPE, curr_node->lib_type, dbuf);
        zwutl_subtag_wr16(ZW_SUBTAG_PROT_VER, curr_node->proto_ver, dbuf);
        zwutl_subtag_wr16(ZW_SUBTAG_APPL_VER, curr_node->app_ver, dbuf);
        zwutl_subtag_wr8(ZW_SUBTAG_MUL_CMD_CAP, curr_node->mul_cmd_cap, dbuf);
        zwutl_subtag_wr8(ZW_SUBTAG_SENSOR, curr_node->sensor, dbuf);
        zwutl_subtag_wr8(ZW_SUBTAG_CATEGORY, curr_node->category, dbuf);
        zwutl_subtag_wr8(ZW_SUBTAG_BASIC, curr_node->basic, dbuf);
        zwutl_subtag_wr8(ZW_SUBTAG_S2_KEY_VALID, curr_node->s2_keys_valid, dbuf);
        zwutl_subtag_wr8(ZW_SUBTAG_S2_GRNT_KEY, curr_node->s2_grnt_keys, dbuf);

        //Set extended node information of nodes other than the Z/IP gateway attached controller
        if (curr_node->ext_ver && (curr_node->nodeid != nw->ctl.nodeid))
        {
            uint16_t    ver_cap_cnt;

            ver_cap_cnt = 0x0100 | curr_node->ext_ver->fw_cnt;
            zwutl_subtag_wr16(ZW_SUBTAG_EXT_VER_CAP, ver_cap_cnt, dbuf);
            zwutl_subtag_wr8(ZW_SUBTAG_HW_VER, curr_node->ext_ver->hw_ver, dbuf);
            if (curr_node->ext_ver->fw_cnt)
            {
                zwutl_subtag_wr(ZW_SUBTAG_FW_VER, curr_node->ext_ver->fw_cnt * 2, (uint8_t *)curr_node->ext_ver->fw_ver, dbuf);
            }
        }

        if (curr_node->s2_dsk[0])
        {
            zwutl_subtag_wr(ZW_SUBTAG_DSK_STR, strlen(curr_node->s2_dsk) + 1, (uint8_t *)curr_node->s2_dsk, dbuf);
        }

        zwutl_subtag_wr32(ZW_SUBTAG_WAKEUP_INTV, curr_node->wkup_intv, dbuf);

        result = zwutl_tag_wr(file, tag_id, dbuf->wr_ofs, dbuf->buf);
        if (result < 0)
        {
            goto l_ZWUTIL_SAVE_ERROR3;
        }
        //-------------------------------
        //Write endpoint tag
        //-------------------------------
        curr_ep = &curr_node->ep;

        while (curr_ep)
        {
            tag_id = ZW_TAG_EP;
            dbuf->wr_ofs = 0;

            zwutl_subtag_wr8(ZW_SUBTAG_EP_ID, curr_ep->epid, dbuf);
            zwutl_subtag_wr8(ZW_SUBTAG_GEN, curr_ep->generic, dbuf);
            zwutl_subtag_wr8(ZW_SUBTAG_SPEC, curr_ep->specific, dbuf);
            zwutl_subtag_wr(ZW_SUBTAG_EP_NAME, strlen(curr_ep->name), (uint8_t *)curr_ep->name, dbuf);
            zwutl_subtag_wr(ZW_SUBTAG_EP_LOC, strlen(curr_ep->loc), (uint8_t *)curr_ep->loc, dbuf);
            //Set ZWave+ info of endpoint other than the Z/IP gateway attached controller
            if (curr_ep->zwplus_info.zwplus_ver && (curr_ep->node->nodeid != nw->ctl.nodeid))
            {
                zwutl_subtag_wr8(ZW_SUBTAG_ZWPLUS_VER, curr_ep->zwplus_info.zwplus_ver, dbuf);
                zwutl_subtag_wr8(ZW_SUBTAG_ROLE_TYPE, curr_ep->zwplus_info.role_type, dbuf);
                zwutl_subtag_wr8(ZW_SUBTAG_NODE_TYPE, curr_ep->zwplus_info.node_type, dbuf);
                zwutl_subtag_wr16(ZW_SUBTAG_INSTR_ICON, curr_ep->zwplus_info.instr_icon, dbuf);
                zwutl_subtag_wr16(ZW_SUBTAG_USER_ICON, curr_ep->zwplus_info.usr_icon, dbuf);
            }

            if (curr_ep->aggr_ep_cnt > 0)
            {
                zwutl_subtag_wr8(ZW_SUBTAG_AGGR_MEMBER_CNT, curr_ep->aggr_ep_cnt, dbuf);
                zwutl_subtag_wr(ZW_SUBTAG_AGGR_MEMBERS, curr_ep->aggr_ep_cnt, curr_ep->aggr_members, dbuf);
            }

            result = zwutl_tag_wr(file, tag_id, dbuf->wr_ofs, dbuf->buf);
            if (result < 0)
            {
                goto l_ZWUTIL_SAVE_ERROR3;
            }
            //-------------------------------
            //Write interface tag
            //-------------------------------
            curr_intf = curr_ep->intf;

            while (curr_intf)
            {
                tag_id = ZW_TAG_IF;
                dbuf->wr_ofs = 0;

                zwutl_subtag_wr16(ZW_SUBTAG_CLS, curr_intf->cls, dbuf);
                zwutl_subtag_wr8(ZW_SUBTAG_VER, curr_intf->real_ver, dbuf);
                zwutl_subtag_wr8(ZW_SUBTAG_PROPTY, curr_intf->propty, dbuf);
                //Save interface data
                if (curr_intf->data_cnt > 0)
                {
                    zwutl_subtag_wr8(ZW_SUBTAG_DAT_CNT, curr_intf->data_cnt, dbuf);
                    zwutl_subtag_wr8(ZW_SUBTAG_DAT_ITEM_SZ, curr_intf->data_item_sz, dbuf);
                    zwutl_subtag_wr(ZW_SUBTAG_DATA, curr_intf->data_cnt * curr_intf->data_item_sz,
                                    (uint8_t *)curr_intf->data, dbuf);

                }
                //Save interface cached data
                if (curr_intf->cch_dat_cnt > 0)
                {
					zwutl_subtag_wr8(ZW_SUBTAG_CCH_B2LVL, curr_intf->b2lvlcch, dbuf);
                    zwutl_subtag_wr8(ZW_SUBTAG_CCH_DAT_CNT, curr_intf->cch_dat_cnt, dbuf);
                    zwutl_subtag_wr8(ZW_SUBTAG_CCH_DAT_ITEM_SZ, curr_intf->cch_dat_item_sz, dbuf);
                    if(curr_intf->b2lvlcch == 0)
					{
						zwutl_subtag_wr(ZW_SUBTAG_CCH_DATA, curr_intf->cch_dat_cnt * curr_intf->cch_dat_item_sz,
										(uint8_t *)curr_intf->cch_data, dbuf);
					}
					else
					{
						int i = 0;
						cch_1lvl_t *p1lvl = (cch_1lvl_t *)curr_intf->cch_data;

						for (i = 0; i < curr_intf->cch_dat_cnt; i++, p1lvl++)
						{
							if(p1lvl && p1lvl->pcch_ele)
							{
								zwutl_subtag_wr8(ZW_SUBTAG_CCH_1LVL_ELE_SZ, p1lvl->cch_ele_sz, dbuf);
								zwutl_subtag_wr(ZW_SUBTAG_CCH_DATA, p1lvl->cch_ele_sz,
                                                (uint8_t *)p1lvl->pcch_ele, dbuf);
							}
						}
					}
                }

                //
                //Command class specific handling
                //
                if ((curr_intf->cls == COMMAND_CLASS_ASSOCIATION_GRP_INFO)
                    && (curr_intf->tmp_data))
                {
                    int                 i;
                    if_grp_info_dat_t   *grp_info = (if_grp_info_dat_t *)curr_intf->tmp_data;

                    zwutl_subtag_wr16(ZW_SUBTAG_AGI_GRP_CNT, grp_info->group_cnt, dbuf);
                    zwutl_subtag_wr8(ZW_SUBTAG_AGI_GRP_DYNAMIC, grp_info->dynamic, dbuf);

                    for (i=0; i<grp_info->valid_grp_cnt; i++)
                    {
                        if (grp_info->grp_info[i])
                        {
                            zw_grp_info_p grp_ent = grp_info->grp_info[i];

                            zwutl_subtag_wr8(ZW_SUBTAG_AGI_GRP_ID, grp_ent->grp_num, dbuf);
                            zwutl_subtag_wr8(ZW_SUBTAG_AGI_CMD_CNT, grp_ent->cmd_ent_cnt, dbuf);
                            zwutl_subtag_wr16(ZW_SUBTAG_AGI_PROFILE, grp_ent->profile, dbuf);
                            zwutl_subtag_wr16(ZW_SUBTAG_AGI_EVENT, grp_ent->evt_code, dbuf);
                            zwutl_subtag_wr(ZW_SUBTAG_AGI_GRP_NAME, strlen(grp_ent->name),
                                            (uint8_t *)grp_ent->name, dbuf);
                            zwutl_subtag_wr(ZW_SUBTAG_AGI_CMD_LIST, grp_ent->cmd_ent_cnt * sizeof(grp_cmd_ent_t),
                                            (uint8_t *)grp_ent->cmd_lst, dbuf);

                        }
                    }
                }
                else if ((curr_intf->cls == COMMAND_CLASS_CONFIGURATION)
                    && (curr_intf->data_cnt))
                {
                    zwcfg_info_cap_p    cfg_info = (zwcfg_info_cap_p)curr_intf->data;
                    int                 i;

                    for (i=0; i<cfg_info->cnt; i++)
                    {
                        zwutl_subtag_wr(ZW_SUBTAG_CFG_INFO_STRUCT, sizeof(zwcfg_info_t),
                                        (uint8_t *)&cfg_info->info[i], dbuf);

                        if (cfg_info->info[i].name)
                        {
                            zwutl_subtag_wr(ZW_SUBTAG_CFG_NAME, strlen(cfg_info->info[i].name) + 1,
                                            (uint8_t *)cfg_info->info[i].name, dbuf);
                        }

                        if (cfg_info->info[i].info)
                        {
                            zwutl_subtag_wr(ZW_SUBTAG_CFG_INFO, strlen(cfg_info->info[i].info) + 1,
                                            (uint8_t *)cfg_info->info[i].info, dbuf);
                        }
                    }
                }

                result = zwutl_tag_wr(file, tag_id, dbuf->wr_ofs, dbuf->buf);
                if (result < 0)
                {
                    goto l_ZWUTIL_SAVE_ERROR3;
                }

                //Next interface
                curr_intf = (zwif_p)curr_intf->obj.next;

            }

            //Next endpoint
            curr_ep = (zwep_p)curr_ep->obj.next;
        }

        //Next node
        curr_node = (zwnode_p)curr_node->obj.next;
    }

    plt_mtx_ulck(nw->mtx);
    //Check dynamic buffer status
    if (dbuf->status != 0)
    {
        fclose(file);
        zwutl_buf_exit(dbuf);
        return ZW_ERR_MEMORY;
    }

    zwutl_buf_exit(dbuf);

    //Append CRC16 checksum
    file_len = ftell(file);
    fseek(file, 0L, SEEK_SET);
    result = zwutl_file_add_crc(file, file_len);
    fclose(file);
    return (result != 0)? ZW_ERR_NONE : ZW_ERR_FILE_WRITE;

l_ZWUTIL_SAVE_ERROR3:
    plt_mtx_ulck(nw->mtx);
l_ZWUTIL_SAVE_ERROR2:
    if (dbuf->status != 0)
    {
        result = ZW_ERR_MEMORY;
    }
    zwutl_buf_exit(dbuf);
l_ZWUTIL_SAVE_ERROR1:
    fclose(file);
    return result;

}


/**
zwutl_crc16_chk - CRC-CCITT (0x1D0F) calculation / check
@param[in]	crc	        CRC initialization value
@param[in]	data	    Buffer that store the byte string
@param[in]	len	        Length of byte string
@return         If data contains the 16-bit checksum field: return zero if check o.k.
                If data excludes the 16-bit checksum field: return CRC-16 checksum.
*/
uint16_t zwutl_crc16_chk(uint16_t crc, uint8_t *data, uint32_t len)
{
    uint8_t tmp_data;
    uint8_t bitmask;
    uint8_t new_bit;

    while (len--)
    {
        tmp_data = *data++;
        for (bitmask = 0x80; bitmask != 0; bitmask >>= 1)
        {
            //Align test bit with next bit of the message byte, starting with msb.
            new_bit = ((tmp_data & bitmask) != 0) ^ ((crc & 0x8000) != 0);
            crc <<= 1;
            if (new_bit)
            {
                crc ^= POLY;
            }
        }
    }
    return crc;
}


/**
zwutl_file_load - Load a data file into a buffer
@param[in]	dat_file	Data file
@param[out]	buf	        Buffer for storing the file content
@return         ZW_ERR_XXX.
@post       Caller must free the buf
*/
int zwutl_file_load(const char *dat_file, uint8_t **buf)
{
    FILE        *file;
    size_t      rd_count;
    long        file_len;

    //Open the the data file
    if (!dat_file)
    {
        return ZW_ERR_FILE_OPEN;
    }

#ifdef USE_SAFE_VERSION
    if (fopen_s(&file, dat_file, "r+b") != 0)
    {
        return ZW_ERR_FILE_OPEN;
    }
#else
    file = fopen(dat_file, "r+b");
    if (!file)
    {
        return ZW_ERR_FILE_OPEN;
    }
#endif

    fseek(file, 0L, SEEK_END);
    file_len = ftell(file);
    fseek(file, 0L, SEEK_SET);

    *buf = malloc(file_len + 1);

    if (*buf == NULL)
    {
        fclose(file);
        return ZW_ERR_MEMORY;
    }

    rd_count = fread(*buf, 1, file_len, file);

    if (rd_count != file_len)
    {
        fclose(file);
        free(*buf);
        return ZW_ERR_FILE_READ;
    }

    //Append NULL character
    (*buf)[file_len] = 0;

    fclose(file);
    return ZW_ERR_NONE;
}


/**
zwutl_file_name_get - Get file name based on home id and optionally client id
@param[in]	non_client_pfx	Non-client (i.e. use_client_id=0)file prefix
@param[in]	dir_path	Directory path of file name
@param[in]	homeid	    Home id
@param[in]	client_id	Client id
@param[out]	file_name	File name which is derived from network home id and optionally client id
@param[in]	use_client_id	Flag to indicate whether to use client_id
@return		ZW_ERR_XXX
@post   Caller must free the memory allocated to file_name if this call is successful
*/
int zwutl_file_name_get(char *non_client_pfx, char *dir_path, uint32_t homeid, uint32_t client_id,
                        char **file_name, int use_client_id)
{
    char    *file_full_path;
    char    *file_prefix;
    size_t  dir_len = 0;
    char    client_id_str[20];

    if (dir_path)
    {   //Calculate directory name length
        dir_len = strlen(dir_path);
    }

    if (use_client_id)
    {
        dir_len += 12;
        file_prefix = "cli";
#ifdef USE_SAFE_VERSION
        sprintf_s(client_id_str, 20, "_%08X.dat", client_id);
#else
        sprintf(client_id_str,"_%08X.dat", client_id);
#endif
    }
    else
    {
        file_prefix = non_client_pfx;
#ifdef USE_SAFE_VERSION
        strcpy_s(client_id_str, 20, ".dat");
#else
        strcpy(client_id_str,".dat");
#endif
    }

    //Add length for basic file name
    dir_len += (16 + strlen(file_prefix));

    //Allocate memory to store full path file name
    file_full_path = (char *)malloc(dir_len);

    if (!file_full_path)
    {
        return ZW_ERR_MEMORY;
    }

    if (dir_path)
    {   //Create full path file name
#ifdef USE_SAFE_VERSION
        sprintf_s(file_full_path, dir_len, "%s%s%08X%s", dir_path, file_prefix, homeid, client_id_str);
#else
        sprintf(file_full_path, "%s%s%08X%s", dir_path, file_prefix, homeid, client_id_str);
#endif

    }
    else
    {   //Directory was not set, use current working directory
#ifdef USE_SAFE_VERSION
        sprintf_s(file_full_path, dir_len, "%s%08X%s", file_prefix, homeid, client_id_str);
#else
        sprintf(file_full_path, "%s%08X%s", file_prefix, homeid, client_id_str);
#endif
    }

    *file_name = file_full_path;

    return ZW_ERR_NONE;
}


/**
zwutl_pref_set - Store network preference into persistent storage
@param[in]	net		    Network
@param[in]	buf		    Buffer that contains the network preference
@param[in]	buf_size	Buffer size in bytes
@param[in]	client_id	User-defined client id to identify the preference
@param[in]	use_client_id	Flag to indicate whether to use client_id
@return		ZW_ERR_XXX
@see		zwutl_pref_get
*/
int zwutl_pref_set(zwnet_p net, void *buf, uint16_t buf_size, uint32_t client_id, int use_client_id)
{
    FILE    *file;
    char    *file_name;
    int     res;
    size_t  wr_count;
    long    file_len;

    if (!net || !buf)
    {
        return ZW_ERR_VALUE;
    }

    res = zwutl_file_name_get("net", net->pref_dir, net->homeid, client_id, &file_name, use_client_id);

    if (res == 0)
    {
        //Open the file for writing
    #ifdef USE_SAFE_VERSION
        if (fopen_s(&file, file_name, "w+b") != 0)
        {
            free(file_name);
            return ZW_ERR_FILE_OPEN;
        }
    #else
        file = fopen(file_name, "w+b");
        if (!file)
        {
            free(file_name);
            return ZW_ERR_FILE_OPEN;
        }
    #endif
        res = ZW_ERR_FILE_WRITE;
        if (file)
        {   //Write the file magic identifier and version
            uint8_t     file_hdr[4] = {PREF_FILE_MAGIC_NUM_H, PREF_FILE_MAGIC_NUM_L, PREF_FILE_VER_NUM_H, PREF_FILE_VER_NUM_L};

            wr_count = fwrite(file_hdr, 1, 4, file);
            if (wr_count == 4)
            {
                //Write preference into file
                wr_count = fwrite(buf, 1, buf_size, file);
                if (wr_count == buf_size)
                {
                    //Append CRC16 checksum
                    file_len = ftell(file);
                    fseek(file, 0L, SEEK_SET);
                    if (zwutl_file_add_crc(file, file_len))
                    {
                        res = ZW_ERR_NONE;
                    }
                }
            }

            fclose(file);
        }
        free(file_name);
    }

    return res;
}


/**
zwutl_pref_get - Retrieve network preference from persistent storage
@param[in]	net		    Network
@param[in]	client_id	User-defined client id to identify the preference
@param[out]	buf		    Return buffer that contains the network preference
@param[out]	buf_size	Buffer size in bytes
@param[in]	use_client_id	Flag to indicate whether to use client_id
@return		ZW_ERR_XXX
@post       Caller MUST free the return buffer "buf" if the call is successful.
@see		zwutl_pref_set
*/
int zwutl_pref_get(zwnet_p net, uint32_t client_id, void **buf, uint16_t *buf_size, int use_client_id)
{
    FILE        *file;
    char        *file_name;
    int         res;
    size_t      rd_count;
    long        file_len;
    uint8_t     file_hdr[4];
    uint16_t    file_ver;
    uint16_t    curr_ver;

    if (!net)
    {
        return ZW_ERR_VALUE;
    }

    res = zwutl_file_name_get("net", net->pref_dir, net->homeid, client_id, &file_name, use_client_id);

    if (res == 0)
    {
        //Open the file for writing
    #ifdef USE_SAFE_VERSION
        if (fopen_s(&file, file_name, "r+b") != 0)
        {
            res = ZW_ERR_FILE_OPEN;
            goto l_PREF_GET_ERR1;
        }
    #else
        file = fopen(file_name, "r+b");
        if (!file)
        {
            res = ZW_ERR_FILE_OPEN;
            goto l_PREF_GET_ERR1;
        }
    #endif
        res = ZW_ERR_FILE_READ;
        if (file)
        {
            //Verify the file magic identifier
            rd_count = fread(file_hdr, 1, 4, file);

            if (rd_count != 4)
            {
                goto l_PREF_GET_ERR2;
            }

            if ((file_hdr[0] == PREF_FILE_MAGIC_NUM_H)
                && (file_hdr[1] == PREF_FILE_MAGIC_NUM_L))
            {
                file_ver = file_hdr[2];
                file_ver = (file_ver << 8) | file_hdr[3];
            }
            else
            {
                res = ZW_ERR_FILE;
                goto l_PREF_GET_ERR2;
            }

            //Check for file version
            curr_ver = PREF_FILE_VER_NUM_H;
            curr_ver = (curr_ver << 8) | PREF_FILE_VER_NUM_L;
            if ((file_ver == 0) || (file_ver > curr_ver))
            {
                res = ZW_ERR_VERSION;
                goto l_PREF_GET_ERR2;
            }

            //Check CRC
            fseek(file, 0L, SEEK_SET);

            if (zwutl_file_chk(file) != 0)
            {
                res = ZW_ERR_FILE;
                goto l_PREF_GET_ERR2;
            }

            //Read preference from file
            fseek(file, 0L, SEEK_END);
            file_len = ftell(file);

            if (fseek(file, PREF_FILE_HDR_LEN, SEEK_SET) != 0)
            {
                res = ZW_ERR_FILE;
                goto l_PREF_GET_ERR2;
            }

            //Calculate preference data length
            file_len -= (PREF_FILE_HDR_LEN + PREF_FILE_CHKSUM_LEN);

            *buf = malloc(file_len);

            if (*buf == NULL)
            {
                res = ZW_ERR_MEMORY;
                goto l_PREF_GET_ERR2;
            }

            rd_count = fread(*buf, 1, file_len, file);

            if (rd_count != file_len)
            {
                res = ZW_ERR_FILE_READ;
                goto l_PREF_GET_ERR3;
            }

            res = ZW_ERR_NONE;
            *buf_size = (uint16_t)file_len;

            fclose(file);
        }
        free(file_name);
    }
    return res;

l_PREF_GET_ERR3:
    free(*buf);
l_PREF_GET_ERR2:
    fclose(file);
l_PREF_GET_ERR1:
    free(file_name);

    return res;
}


/**
zwutil_dsk_chk - check Device Specific Key (DSK) format
@param[in]	dsk	    DSK to be checked
@return     Non-zero if check o.k.; else zero on error
*/
int zwutil_dsk_chk(char *dsk)
{
    int         i;
    int         j;
    int         state;  //state of digit. 0=unknown, 1=less than the limit 65535
    const char  dsk_max[]= { "65535" };

    //Check for the following example format:
    //34028-23669-20938-46346-33746-07431-56821-14553
    //
    //Also, the 5-digit group cannot be greater than 0xFFFF or 65535

    for (i=0; i<8; i++)
    {
        state = 0;

        for (j=0; j<5; j++)
        {
            if (!isdigit(*dsk))
            {
                return 0;
            }

            if (state == 0)
            {
                if (*dsk > dsk_max[j])
                {   //limit exceeded
                    return 0;
                }
                if (*dsk < dsk_max[j])
                {
                    state = 1;
                }
            }
            dsk++;
        }

        if (i==7)
        {   //Last 5-digit group
            return (*dsk == '\0')? 1 : 0;
        }
        else
        {
            if (*dsk != '-')
            {
                return 0;
            }
            dsk++;
        }
    }

    return 0;
}


/**
zwutil_dsk_to_str - convert Device Specific Key (DSK) to string
@param[in]	dsk	    DSK
@param[in]	key_len	DSK key length in bytes
@return     Converted string if o.k.; else return NULL
@post   Caller MUST free the return string if return value is non-NULL
*/
char *zwutil_dsk_to_str(uint8_t *dsk, uint8_t key_len)
{
    char        *dsk_str;
    char        *ret_str;
    int         i;
    uint16_t    two_bytes;

    if (key_len & 0x01)
    {   //Odd number
        return NULL;
    }

    dsk_str = (char *)malloc((key_len * 3) + 2);

    if (!dsk_str)
    {
        return NULL;
    }

    ret_str = dsk_str;

    for (i=0; i<(key_len >> 1); i++)
    {
        two_bytes = *dsk++;
        two_bytes = (two_bytes << 8) | *dsk++;

#ifdef USE_SAFE_VERSION
		sprintf_s(dsk_str, 8, "%05u-", two_bytes);
#else
		sprintf(dsk_str, "%05u-", two_bytes);
#endif
        dsk_str += 6;
    }

    //Replace the last '-' with  '\0'
    dsk_str--;
    *dsk_str = '\0';

    return ret_str;

}


/**
zwutil_str_to_dsk - convert Device Specific Key (DSK) string to binary
@param[in]	dsk	    DSK string
@param[out]	dsk_bin	Buffer to store DSK binary
@param[out]	key_len	DSK binary length
@return     Non-zero if o.k.; else zero on error
*/
int zwutil_str_to_dsk(char *dsk, uint8_t *dsk_bin, uint8_t *key_len)
{
    char        dsk_str[8];
    int         i;
    unsigned    two_bytes;
    uint8_t     key_added = 0;

    //Example: 34028-23669-20938-46346-33746-07431-56821-14553

    //Check whether the DSK string is in right format
    if (!zwutil_dsk_chk(dsk))
    {
        return 0;
    }

    for (i=0; i<8; i++)
    {
        memcpy(dsk_str, dsk, 5);
        dsk_str[5] = '\0';
#ifdef USE_SAFE_VERSION
		sscanf_s(dsk_str, "%u", &two_bytes);
#else
		sscanf(dsk_str, "%u", &two_bytes);
#endif
        dsk_bin[key_added++] = (two_bytes >> 8) & 0xFF;
        dsk_bin[key_added++] = two_bytes & 0xFF;
        dsk += 6;
    }

    *key_len = key_added;

    return 1;

}


/**
zwutil_sec_encap_fmt_get - find the highest security key from the granted keys
@param[in]	grnt_keys	    S2 granted keys, bitmask of SEC_KEY_BITMSK_XXX
@return     highest security key formatted in Z/IP header extension Encapsulation Format Info.
*/
uint16_t zwutil_sec_encap_fmt_get(uint8_t grnt_keys)
{
    int      i;
    uint16_t encap_fmt;
    uint8_t  key_mask;

    if (grnt_keys & SEC_KEY_ALL_S2)
    {
        for (key_mask=SEC_KEY_BITMSK_S2_K2, i=0; i<3; i++)
        {
            if (key_mask & grnt_keys)
            {
                break;
            }
            key_mask >>= 1;
        }
        encap_fmt = key_mask;
    }
    else
    {
        encap_fmt = SEC_KEY_BITMSK_S0;
    }

    return (encap_fmt << 8);
}


/**
zwutil_is_cmd_set - Check if the Z-wave command is type "set"
@param[in]	cls	    Z-wave command class
@param[in]	cmd	    Z-wave command
@return     non-zero if it is type "set"; else return 0
*/
int zwutil_is_cmd_set(uint16_t cls, uint16_t cmd)
{
    int i;
    int num_elem;  //Number of element in the "set" command list. Note that 2 elements = 1 entry
    int found_cls = 0;

    num_elem = sizeof(zw_set_cmd_tbl)/sizeof(zw_set_cmd_tbl[0]);

    for (i=0; i<num_elem; i+=2)
    {
        if (zw_set_cmd_tbl[i] == cls)
        {
            found_cls = 1;

            if (zw_set_cmd_tbl[i+1] == cmd)
            {   //Found match
                return 1;
            }
        }
        else
        {
            if (found_cls)
            {   //All the commands belong to command class "cls" have been check and no match found
                return 0;
            }
        }
    }

    return 0;
}


/**
@}
*/

