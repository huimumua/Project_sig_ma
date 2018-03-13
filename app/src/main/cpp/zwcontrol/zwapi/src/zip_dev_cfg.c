/**
@file   zip_dev_cfg.c - Z-wave Device Configuration functions implementation.


@author David Chow

@version    1.0 17-2-2014  Initial release

@copyright © 2014 SIGMA DESIGNS, INC. THIS IS AN UNPUBLISHED WORK PROTECTED BY SIGMA DESIGNS, INC.
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
#include <string.h>
#include "../include/zip_api_pte.h"
#include "../include/zip_api_util.h"
#include "../include/cjson.h"

static int zwdev_bin_snsr_if_ld(cJSON *json_if_arry, if_rec_t *if_rec_lst_hd, if_rec_t **if_rec);
static int zwdev_alrm_result_ld(cJSON *json_if_arry, if_rec_alarm_match_t *palrm_rec);

/**
@defgroup DevCfg Z-wave Device Configuration Functions
Some device configuration functions.
@{
*/

typedef int (*zwdev_if_parser_fn)(cJSON *json_if_rec, if_rec_t *if_rec_lst_hd, if_rec_t **if_rec);
/**<
interface parser of JSON object function pointer
@param[in]	json_if_rec	    JSON interface record
@param[in]	if_rec_lst_hd	Interface record list head
@param[out]	if_rec		    Interface record
@return		ZW_ERR_xxx
@post       Caller must free the memory allocated to if_rec if return value is ZW_ERR_NONE
*/

typedef void (*zwdev_if_free_fn)(if_rec_t *if_rec);
/**<
free interface record function pointer
@param[in]	if_rec		Interface record
@return
*/

/** Interface record and corresponding z-wave command class*/
typedef struct
{
    uint16_t    cmd_cls;
    uint8_t     if_rec_type;
} zwdev_if_rec_cls;

static zwdev_if_rec_cls cmd_cls_rec_type_tbl[]= {{COMMAND_CLASS_SENSOR_BINARY, IF_REC_TYPE_BIN_SENSOR},
    {COMMAND_CLASS_SENSOR_MULTILEVEL, IF_REC_TYPE_SENSOR},
    {COMMAND_CLASS_METER, IF_REC_TYPE_METER},
    {COMMAND_CLASS_ALARM, IF_REC_TYPE_ALARM},
    {COMMAND_CLASS_BASIC, IF_REC_TYPE_BASIC},
    {COMMAND_CLASS_SENSOR_ALARM, IF_REC_TYPE_ALRM_SNSR},
    {COMMAND_CLASS_THERMOSTAT_SETPOINT, IF_REC_TYPE_THRMO_SETP}
};

static if_rec_alarm_match_t alarm_match_rec_init = { NULL, NULL, NULL, 0, -1, -1, -1, -1, -1, -1, -1, NULL};

static if_rec_alarm_result_t alarm_result_rec_init = { -1, -1, -1, NULL, -1, NULL, -1, -1, NULL};

static if_rec_alarm_rev_match_t alarm_rev_match_rec_init = { -1, -1, -1};

static if_rec_basic_match_t  basic_match_rec_init = { NULL, NULL, -1, -1, -1};

static if_rec_alrm_snsr_match_t  alrm_snsr_match_rec_init = { NULL, NULL, -1, -1, -1, -1, -1};

static if_rec_bsnsr_match_t bin_snsr_match_rec_init = { NULL, NULL, -1, -1, -1};

static redir_rec_t   redir_rec_init = { NULL, 0, -1, 0};


/**
zwdev_alrm_if_rec_create - Create an alarm interface match record and result record and initialize
@param[in]	ppalrm_match_rec  Address of the alarm interface match record pointer
@return
*/
static void zwdev_alrm_if_rec_create(if_rec_alarm_match_t **ppalrm_match_rec)
{
    if_rec_alarm_match_t *pNewRec;

    *ppalrm_match_rec = (if_rec_alarm_match_t *)calloc(1, sizeof(if_rec_alarm_match_t));
    pNewRec = *ppalrm_match_rec;

    if (pNewRec)
    {
        *pNewRec = alarm_match_rec_init;

        pNewRec->pResult = (if_rec_alarm_result_t *)calloc(1, sizeof(if_rec_alarm_result_t));

        if (pNewRec->pResult)
        {
            *pNewRec->pResult = alarm_result_rec_init;
        }
        else
        {
            free(pNewRec);
            *ppalrm_match_rec = NULL;
            return; //out of memory
        }

        pNewRec->pRevMatch = (if_rec_alarm_rev_match_t *)calloc(1, sizeof(if_rec_alarm_rev_match_t));

        if (pNewRec->pRevMatch)
        {
            *pNewRec->pRevMatch = alarm_rev_match_rec_init;
        }
        else
        {
            free(pNewRec->pResult);
            free(pNewRec);
            *ppalrm_match_rec = NULL;
            return; //out of memory
        }
    }
}

/**
zwdev_alrm_if_rec_free - Free all alarm interface match records and result records
@param[in]	palrm_match_rec  Address of the alarm interface match record pointer
@return
*/
static void zwdev_alrm_if_rec_free(if_rec_alarm_match_t *palrm_match_rec)
{
    if_rec_alarm_match_t  *pCurrMatch_rec = palrm_match_rec, *pNextMatch_rec;
    if_rec_alarm_result_t *pResult_rec;

    while (pCurrMatch_rec != NULL)
    {
        pNextMatch_rec = pCurrMatch_rec->next;
        pResult_rec = pCurrMatch_rec->pResult;

        if (pResult_rec)
        {
            if (pResult_rec->type_name)         free(pResult_rec->type_name);
            if (pResult_rec->level_name)        free(pResult_rec->level_name);
            if (pResult_rec->pEx_evt_prm)       free(pResult_rec->pEx_evt_prm);

            free(pResult_rec);
            pResult_rec = NULL;
        }

        if (pCurrMatch_rec->pRevMatch)          free(pCurrMatch_rec->pRevMatch);

        if (pCurrMatch_rec->pEx_evt_prm)        free(pCurrMatch_rec->pEx_evt_prm);

        free(pCurrMatch_rec);

        pCurrMatch_rec = pNextMatch_rec;
    }
}

/**
zwdev_alrm_if_free - free alarm interface record
@param[in]	if_rec	Interface record
@return
*/
static void zwdev_alrm_if_free(if_rec_t *if_rec)
{
    if (if_rec->type == IF_REC_TYPE_ALARM)
    {
        zwdev_alrm_if_rec_free(if_rec->rec.alarm);
        free(if_rec);
    }
}


/**
zwdev_if_rec_find - Find interface record
@param[in]	if_rec_lst_hd	Interface record list head
@param[in]  if_rec_type     Interface record type, IF_REC_TYPE_XXX
@return		interface record if found; else return NULL
*/
static if_rec_t *zwdev_if_rec_find(if_rec_t *if_rec_lst_hd, int if_rec_type)
{
    if_rec_t *tmp_if_rec = if_rec_lst_hd;

    while (tmp_if_rec)
    {
        if (tmp_if_rec->type == if_rec_type)
        {
            return tmp_if_rec;
        }

        tmp_if_rec = tmp_if_rec->next;
    }

    return NULL;
}


/**
zwdev_if_rec_detach - Detach interface record from the linked-list
@param[in]	if_rec_lst_hd	Interface record list head
@param[in]  if_rec_type     Interface record type, IF_REC_TYPE_XXX
@return		detached interface record if success; else return NULL
*/
static if_rec_t *zwdev_if_rec_detach(if_rec_t *if_rec_lst_hd, int if_rec_type)
{
    if_rec_t *tmp_if_rec = if_rec_lst_hd;
    if_rec_t *prev_if_rec = NULL;

    while (tmp_if_rec)
    {
        if (tmp_if_rec->type == if_rec_type)
        {
            if (!prev_if_rec)
            {   //The detach interface record must not be the head
                return NULL;
            }

            prev_if_rec->next = tmp_if_rec->next;

            //Invalidate the next of the detached interface record
            tmp_if_rec->next = NULL;

            return tmp_if_rec;
        }

        prev_if_rec = tmp_if_rec;

        tmp_if_rec = tmp_if_rec->next;
    }

    return NULL;
}


/**
zwdev_alrm_match_append - Append alarm match entry at the end of linked-list
@param[in]	alarm_match_hd	    Alarm match list head
@param[in]  alarm_match_ent     Alarm match entry
@return
*/
static void zwdev_alrm_match_append(if_rec_alarm_match_t **alarm_match_hd, if_rec_alarm_match_t *alarm_match_ent)
{
    if (*alarm_match_hd == NULL)
    {   //List is empty
        *alarm_match_hd = alarm_match_ent;
    }
    else
    {   //Append at the end of linked-list
        if_rec_alarm_match_t *last_match_rec;

        last_match_rec = *alarm_match_hd;

        while (last_match_rec)
        {
            if (!last_match_rec->next)
            {
                last_match_rec->next = alarm_match_ent;
                break;
            }

            last_match_rec = last_match_rec->next;
        }
    }
}


/**
zwdev_gen_result_ld - Load generic conversion result record from JSON array
@param[in]		json_if_arry   Result record JSON array
@param[in]	    if_rec_lst_hd  Interface record list head
@param[in]		res_type       Result interface record type, IF_REC_TYPE_XXX
@param[out]	    result_rec     Result record corresponds to the res_type
@param[in,out]	pif_rec		   Interface record
@return		ZW_ERR_xxx
@post       Caller must free the memory alloted if return value is ZW_ERR_NONE
*/
static int zwdev_gen_result_ld(cJSON *json_if_arry, if_rec_t *if_rec_lst_hd, int res_type, void **result_rec, if_rec_t *pif_rec)
{
    int         res;
    if_rec_t    *res_if_rec;

    if (res_type == IF_REC_TYPE_ALARM)
    {
        if_rec_alarm_match_t    *alarm_rec = NULL;

        res_if_rec = zwdev_if_rec_find(if_rec_lst_hd, res_type);

        if (!res_if_rec)
        {   //No existing alarm interface record
            if (!pif_rec->next)
            {   //Alarm interface record has not yet been created
                res_if_rec = (if_rec_t *)calloc(1, sizeof(if_rec_t));
                if (!res_if_rec)
                {
                    return ZW_ERR_MEMORY;
                }
                res_if_rec->type = IF_REC_TYPE_ALARM;

                //Append to the match interface record
                pif_rec->next = res_if_rec;
            }
            else
            {   //Alarm interface record has been created
                res_if_rec = pif_rec->next;
            }
        }

        zwdev_alrm_if_rec_create(&alarm_rec);

        if (!alarm_rec)
            return ZW_ERR_MEMORY;

        //Fill info for the result record
        res = zwdev_alrm_result_ld(json_if_arry, alarm_rec);

        if (res != ZW_ERR_NONE)
        {
            zwdev_alrm_if_rec_free(alarm_rec);
            return res;
        }

        //Insert the alarm match record
        zwdev_alrm_match_append(&res_if_rec->rec.alarm, alarm_rec);

        //Link the Result to alarm sensor interface
        *result_rec = alarm_rec->pResult;
    }
    else
    {
        return ZW_ERR_UNSUPPORTED;
    }

    return ZW_ERR_NONE;
}


/**
zwdev_basic_if_rec_create - Create an basic interface match record and result record and initialize
@param[in]	ppbasic_match_rec  Address of the basic interface match record pointer
@return
*/
static void zwdev_basic_if_rec_create(if_rec_basic_match_t **ppbasic_match_rec)
{
    if_rec_basic_match_t *pNewRec;

    *ppbasic_match_rec = (if_rec_basic_match_t *)calloc(1, sizeof(if_rec_basic_match_t));
    pNewRec = *ppbasic_match_rec;

    if (pNewRec)
    {
        *pNewRec = basic_match_rec_init;
    }
}

/**
zwdev_basic_if_rec_free - Free all basic interface match records and result records
@param[in]	pbasic_match_rec  Address of the basic interface match record pointer
@return
*/
static void zwdev_basic_if_rec_free(if_rec_basic_match_t *pbasic_match_rec)
{
    if_rec_basic_match_t  *pCurrMatch_rec = pbasic_match_rec, *pNextMatch_rec;

    while (pCurrMatch_rec != NULL)
    {
        pNextMatch_rec = pCurrMatch_rec->next;

        free(pCurrMatch_rec);
        pCurrMatch_rec = pNextMatch_rec;
    }
}

/**
zwdev_basic_if_free - free basic interface record
@param[in]	if_rec	Interface record
@return
*/
static void zwdev_basic_if_free(if_rec_t *if_rec)
{
    if (if_rec->type == IF_REC_TYPE_BASIC)
    {
        zwdev_basic_if_rec_free(if_rec->rec.basic);

        free(if_rec);
    }
}

/**
zwdev_alrm_snsr_if_rec_create - Create an alrm_snsr interface match record and result record and initialize
@param[in]	ppalrm_snsr_match_rec  Address of the alrm_snsr interface match record pointer
@return
*/
static void zwdev_alrm_snsr_if_rec_create(if_rec_alrm_snsr_match_t **ppalrm_snsr_match_rec)
{
    if_rec_alrm_snsr_match_t *pNewRec;

    *ppalrm_snsr_match_rec = (if_rec_alrm_snsr_match_t *)calloc(1, sizeof(if_rec_alrm_snsr_match_t));
    pNewRec = *ppalrm_snsr_match_rec;

    if (pNewRec)
    {
        *pNewRec = alrm_snsr_match_rec_init;
    }
}

/**
zwdev_alrm_snsr_if_rec_free - Free all alrm_snsr interface match records and result records
@param[in]	palrm_snsr_match_rec  Address of the alrm_snsr interface match record pointer
@return
*/
static void zwdev_alrm_snsr_if_rec_free(if_rec_alrm_snsr_match_t *palrm_snsr_match_rec)
{
    if_rec_alrm_snsr_match_t  *pCurrMatch_rec = palrm_snsr_match_rec, *pNextMatch_rec;

    while (pCurrMatch_rec != NULL)
    {
        pNextMatch_rec = pCurrMatch_rec->next;

        free(pCurrMatch_rec);
        pCurrMatch_rec = pNextMatch_rec;
    }
}

/**
zwdev_alrm_snsr_if_free - free alrm_snsr interface record
@param[in]	if_rec	Interface record
@return
*/
static void zwdev_alrm_snsr_if_free(if_rec_t *if_rec)
{
    if (if_rec->type == IF_REC_TYPE_ALRM_SNSR)
    {
        zwdev_alrm_snsr_if_rec_free(if_rec->rec.alrm_snsr);

        free(if_rec);
    }
}

/**
zwdev_bin_snsr_if_match_rec_create - Create an bin_snsr interface match record and result record and initialize
@param[in]	ppbin_snsr_match_rec  Address of the bin_snsr interface match record pointer
@return
*/
static void zwdev_bin_snsr_if_match_rec_create(if_rec_bsnsr_match_t **ppbin_snsr_match_rec)
{
    if_rec_bsnsr_match_t *pNewRec;

    *ppbin_snsr_match_rec = (if_rec_bsnsr_match_t *)calloc(1, sizeof(if_rec_bsnsr_match_t));
    pNewRec = *ppbin_snsr_match_rec;

    if (pNewRec)
    {
        *pNewRec = bin_snsr_match_rec_init;
    }
}

/**
zwdev_bin_snsr_if_match_rec_free - Free all bin_snsr interface match records and result records
@param[in]	pbin_snsr_match_rec  Address of the bin_snsr interface match record pointer
@return
*/
static void zwdev_bin_snsr_if_match_rec_free(if_rec_bsnsr_match_t *pbin_snsr_match_rec)
{
    if_rec_bsnsr_match_t  *pCurrMatch_rec = pbin_snsr_match_rec, *pNextMatch_rec;

    while (pCurrMatch_rec)
    {
        pNextMatch_rec = pCurrMatch_rec->next;

        free(pCurrMatch_rec);
        pCurrMatch_rec = pNextMatch_rec;
    }
}

/**
zwdev_bin_snsr_if_rec_create - Create an bin_snsr interface record and result record and initialize
@param[in]	ppbin_snsr_rec  Address of the bin_snsr interface record pointer
@return
*/
static void zwdev_bin_snsr_if_rec_create(if_rec_bsnsr_t **ppbin_snsr_rec)
{

    *ppbin_snsr_rec = (if_rec_bsnsr_t *)calloc(1, sizeof(if_rec_bsnsr_t));
}

/**
zwdev_bin_snsr_if_rec_free - Free all bin_snsr interface records and result records
@param[in]	pbin_snsr_rec  bin_snsr interface record pointer
@return
*/
static void zwdev_bin_snsr_if_rec_free(if_rec_bsnsr_t *pbin_snsr_rec)
{
    if (pbin_snsr_rec)
    {
        zwdev_bin_snsr_if_match_rec_free(pbin_snsr_rec->rec_match);
        free(pbin_snsr_rec);
    }
}

/**
zwdev_bin_snsr_if_free - free bin_snsr interface record
@param[in]	if_rec	Interface record
@return
*/
static void zwdev_bin_snsr_if_free(if_rec_t *if_rec)
{
    if (if_rec->type == IF_REC_TYPE_BIN_SENSOR)
    {
        zwdev_bin_snsr_if_rec_free(if_rec->rec.bsnsr);

        free(if_rec);
    }
}


/**
zwdev_config_prm_rec_free - free configuration record
@param[in]	rec_hd	Record head
@return
*/
static void zwdev_config_prm_rec_free(if_rec_config_t *rec_hd)
{
    if_rec_config_t *config_rec;
    if_rec_config_t *config_nxt;

    config_rec = rec_hd;

    while (config_rec)
    {
        config_nxt = config_rec->next;

        if (config_rec->rec_type == CONFIG_REC_TYPE_INFO)
        {
            free(config_rec->rec.cfg_info.param_name);
            free(config_rec->rec.cfg_info.param_info);
        }

        free(config_rec);
        //Point to the next configuration record
        config_rec = config_nxt;
    }
}


/**
zwdev_config_prm_if_free - free configuration interface record
@param[in]	if_rec	Interface record
@return
*/
static void zwdev_config_prm_if_free(if_rec_t *if_rec)
{
    if (if_rec->type == IF_REC_TYPE_CONFIG)
    {
        zwdev_config_prm_rec_free(if_rec->rec.config);
        free(if_rec);
    }
}


/**
zwdev_thrmo_setp_if_free - free thermostat setpoint interface record
@param[in]	if_rec	Interface record
@return
*/
static void zwdev_thrmo_setp_if_free(if_rec_t *if_rec)
{
    if (if_rec->type == IF_REC_TYPE_THRMO_SETP)
    {
        free(if_rec->rec.thrmo_setp->temp_range);
        free(if_rec->rec.thrmo_setp);
        free(if_rec);
    }
}

/**
zwdev_gen_if_free - free interface record
@param[in]	if_rec	Interface record
@return
*/
static void zwdev_gen_if_free(if_rec_t *if_rec)
{
    if (if_rec->rec.grp) //Since it is union. Doesn't matter which member it uses
        free(if_rec->rec.grp);
    free(if_rec);
}


/**
zwdev_cfg_if_free - Free device specific configuration interface records
@param[in]	if_rec	    Device specific configuration interface records
@return
*/
static void zwdev_cfg_if_free(if_rec_t *if_rec)
{
    static const zwdev_if_free_fn if_rec_free[] = { NULL,
        zwdev_gen_if_free,  //group
        zwdev_config_prm_if_free,    //configuration
        zwdev_bin_snsr_if_free,  //binary sensor
        zwdev_gen_if_free,  //sensor
        zwdev_gen_if_free,  //meter
        zwdev_alrm_if_free, //alarm
        zwdev_basic_if_free,//basic
        zwdev_alrm_snsr_if_free,//alarm sensor
        zwdev_thrmo_setp_if_free //thermostat setpoint
    };
    zwdev_if_free_fn    zwdev_if_rec_free;
    if_rec_t            *next_if;

    if (!if_rec)
        return;

    do
    {
        //Save next interface record
        next_if = if_rec->next;

        if ((if_rec->type > 0) && (if_rec->type <= IF_REC_TYPE_MAX))
        {
            zwdev_if_rec_free = if_rec_free[if_rec->type];
            zwdev_if_rec_free(if_rec);
        }

        if_rec = next_if;

    }while (next_if);
}

/**
zwdev_cfg_redir_free - Free device specific command redirection records
@param[in]	redir_rec	    Device specific command redirection records
@return
*/
static void zwdev_cfg_redir_free(redir_rec_t *redir_rec)
{
    redir_rec_t *tmpredir_rec = redir_rec;

    while (tmpredir_rec != NULL)
    {
        redir_rec = redir_rec->next;

        free(tmpredir_rec);

        tmpredir_rec = redir_rec;
    }
}

/**
zwdev_cfg_ep_free - Free device specific configuration endpoint records
@param[in]	ep_rec	    Device specific configuration endpoint records
@return
*/
static void zwdev_cfg_ep_free(ep_rec_t *ep_rec)
{
    ep_rec_t *next_ep;

    if (!ep_rec)
        return;

    do
    {
        //Save next endpoint record
        next_ep = ep_rec->next;

        zwdev_cfg_if_free(ep_rec->if_rec);

        zwdev_cfg_redir_free(ep_rec->redir_rec);

        free(ep_rec);

        ep_rec = next_ep;

    }while (next_ep);
}

/**
zwdev_bin_snsr_if_parser - Interface parser of JSON object
@param[in]	json_if_rec	    JSON interface record
@param[in]	if_rec_lst_hd	Interface record list head
@param[out]	if_rec		    Interface record
@return		ZW_ERR_xxx
@post       Caller must free the memory allocated to if_rec if return value is ZW_ERR_NONE
*/
static int zwdev_bin_snsr_if_parser(cJSON *json_if_rec, if_rec_t *if_rec_lst_hd, if_rec_t **if_rec)
{
    if_rec_t        *tmp_if = NULL;
    cJSON           *if_rec_ent;
    unsigned        if_rec_ent_msk;     //Bit mask to check all mandatory entries in if_rec_ent
    int             no_evt_clr = 0;
    int             bin_snsr_cnt;
    int             res;
    uint8_t         type = 0;           //Initialize to invalid type
    uint16_t        tmout = DFLT_EVT_CLR_TIMEOUT;        //Timeout default value

#define IF_ENTRY_MSK_BIN_SNSR           0x01
#define IF_ENTRY_MSK_BIN_SNSR_EVT_CLR   0x02
#define IF_ENTRY_MSK_BIN_SNSR_MATCH     0x04

//#define IF_ENTRY_MSK_BIN_SNSR_ALL_PRESENT   (IF_ENTRY_MSK_BIN_SNSR)

    for (if_rec_ent_msk=0, if_rec_ent=json_if_rec->child;
        if_rec_ent != NULL; if_rec_ent=if_rec_ent->next)
    {
        if ((if_rec_ent->type == cJSON_String) && (!strcmp(if_rec_ent->string, "_comment")))
        {
            continue;
        }

        if (if_rec_ent->type == cJSON_Number)
        {
            if (!strcmp(if_rec_ent->string, "bin_sensor_type"))
            {
                if_rec_ent_msk |= IF_ENTRY_MSK_BIN_SNSR;
                type = (uint8_t)if_rec_ent->valueint;
            }
            else if (!strcmp(if_rec_ent->string, "no_event_clear"))
            {
                if_rec_ent_msk |= IF_ENTRY_MSK_BIN_SNSR_EVT_CLR;
                no_evt_clr = if_rec_ent->valueint;
            }
            else if (!strcmp(if_rec_ent->string, "event_clear_timeout"))
            {
                tmout = (uint16_t)if_rec_ent->valueint;
            }
        }
        else if ((if_rec_ent->type == cJSON_Array) && (!strcmp(if_rec_ent->string, "bin_sensor")))
        {
            if (if_rec_ent_msk & IF_ENTRY_MSK_BIN_SNSR_MATCH)
            {   //Skip duplicate field
                continue;
            }
            if_rec_ent_msk |= IF_ENTRY_MSK_BIN_SNSR_MATCH;

            bin_snsr_cnt = cJSON_GetArraySize(if_rec_ent);

            if (bin_snsr_cnt > 0)
            {
                res = zwdev_bin_snsr_if_ld(if_rec_ent, if_rec_lst_hd, &tmp_if);

                if (res != ZW_ERR_NONE)
                {
                    return res;
                }
            }
        }
    }

    //Check whether all the mandatory entries present
    if (!if_rec_ent_msk)
    {
        return ZW_ERR_MISSING_ENTRY;
    }

    if (!tmp_if)
    {   //Allocate memory for the interface
        tmp_if = (if_rec_t *)calloc(1, sizeof(if_rec_t));
        if (tmp_if)
        {
            zwdev_bin_snsr_if_rec_create(&tmp_if->rec.bsnsr);

            if (!tmp_if->rec.bsnsr)
            {
                free(tmp_if);
                return ZW_ERR_MEMORY;
            }
        }
        else
        {
            return ZW_ERR_MEMORY;
        }
    }

    tmp_if->type = IF_REC_TYPE_BIN_SENSOR;
    tmp_if->rec.bsnsr->type = type;
    tmp_if->propty = (no_evt_clr == 0)? IF_PROPTY_BSNSR_EVT_CLR : 0;
    if (no_evt_clr)
    {
        tmp_if->tmout = (tmout > MAX_EVT_CLR_TIMEOUT)? MAX_EVT_CLR_TIMEOUT : tmout;
    }

    *if_rec = tmp_if;

    return ZW_ERR_NONE;

}


/**
zwdev_snsr_if_parser - Interface parser of JSON object
@param[in]	json_if_rec	    JSON interface record
@param[in]	if_rec_lst_hd	Interface record list head
@param[out]	if_rec		    Interface record
@return		ZW_ERR_xxx
@post       Caller must free the memory allocated to if_rec if return value is ZW_ERR_NONE
*/
static int zwdev_snsr_if_parser(cJSON *json_if_rec, if_rec_t *if_rec_lst_hd, if_rec_t **if_rec)
{
    if_rec_t        *tmp_if;
    cJSON           *if_rec_ent;
    unsigned        if_rec_ent_msk;     //Bit mask to check all mandatory entries in if_rec_ent
    uint8_t         type = 0;
    uint8_t         unit = 0;

#define IF_ENTRY_MSK_SNSR_TYPE   0x01
#define IF_ENTRY_MSK_SNSR_UNIT   0x02

#define IF_ENTRY_MSK_SNSR_ALL_PRESENT   (IF_ENTRY_MSK_SNSR_TYPE | IF_ENTRY_MSK_SNSR_UNIT)

    for (if_rec_ent_msk=0, if_rec_ent=json_if_rec->child;
        if_rec_ent != NULL; if_rec_ent=if_rec_ent->next)
    {
        if ((if_rec_ent->type == cJSON_String) && (!strcmp(if_rec_ent->string, "_comment")))
        {
            continue;
        }

        if (if_rec_ent->type == cJSON_Number)
        {
            if (!strcmp(if_rec_ent->string, "sensor_type"))
            {
                if_rec_ent_msk |= IF_ENTRY_MSK_SNSR_TYPE;
                type = (uint8_t)if_rec_ent->valueint;
            }
            else if (!strcmp(if_rec_ent->string, "sensor_unit"))
            {
                if_rec_ent_msk |= IF_ENTRY_MSK_SNSR_UNIT;
                unit = (uint8_t)if_rec_ent->valueint;
            }
        }
    }

    //Check whether all the mandatory entries present
    if ((if_rec_ent_msk & IF_ENTRY_MSK_SNSR_ALL_PRESENT) != IF_ENTRY_MSK_SNSR_ALL_PRESENT)
    {
        return ZW_ERR_MISSING_ENTRY;
    }

    //Allocate memory for the interface
    tmp_if = (if_rec_t *)calloc(1, sizeof(if_rec_t));
    if (!tmp_if)
    {
        return ZW_ERR_MEMORY;
    }

    tmp_if->rec.snsr = (if_rec_snsr_t *)calloc(1, sizeof(if_rec_snsr_t));
    if (!tmp_if->rec.snsr)
    {
        free(tmp_if);
        return ZW_ERR_MEMORY;
    }

    tmp_if->type = IF_REC_TYPE_SENSOR;
    tmp_if->rec.snsr->type = type;
    tmp_if->rec.snsr->unit = unit;

    *if_rec = tmp_if;

    return ZW_ERR_NONE;

}


/**
zwdev_meter_if_parser - Interface parser of JSON object
@param[in]	json_if_rec	    JSON interface record
@param[in]	if_rec_lst_hd	Interface record list head
@param[out]	if_rec		    Interface record
@return		ZW_ERR_xxx
@post       Caller must free the memory allocated to if_rec if return value is ZW_ERR_NONE
*/
static int zwdev_meter_if_parser(cJSON *json_if_rec, if_rec_t *if_rec_lst_hd, if_rec_t **if_rec)
{
    if_rec_t        *tmp_if;
    cJSON           *if_rec_ent;
    unsigned        if_rec_ent_msk;     //Bit mask to check all mandatory entries in if_rec_ent
    uint8_t         type = 0;
    uint8_t         unit_supp = 0;

#define IF_ENTRY_MSK_METER_TYPE         0x01
#define IF_ENTRY_MSK_METER_UNIT_SUPP    0x02

#define IF_ENTRY_MSK_METER_ALL_PRESENT   (IF_ENTRY_MSK_METER_TYPE | IF_ENTRY_MSK_METER_UNIT_SUPP)

    for (if_rec_ent_msk=0, if_rec_ent=json_if_rec->child;
        if_rec_ent != NULL; if_rec_ent=if_rec_ent->next)
    {
        if ((if_rec_ent->type == cJSON_String) && (!strcmp(if_rec_ent->string, "_comment")))
        {
            continue;
        }

        if (if_rec_ent->type == cJSON_Number)
        {
            if (!strcmp(if_rec_ent->string, "meter_type"))
            {
                if_rec_ent_msk |= IF_ENTRY_MSK_METER_TYPE;
                type = (uint8_t)if_rec_ent->valueint;
            }
            else if (!strcmp(if_rec_ent->string, "meter_supported_units"))
            {
                if_rec_ent_msk |= IF_ENTRY_MSK_METER_UNIT_SUPP;
                unit_supp = (uint8_t)if_rec_ent->valueint;
            }
        }
    }

    //Check whether all the mandatory entries present
    if ((if_rec_ent_msk & IF_ENTRY_MSK_METER_ALL_PRESENT) != IF_ENTRY_MSK_METER_ALL_PRESENT)
    {
        return ZW_ERR_MISSING_ENTRY;
    }

    //Allocate memory for the interface
    tmp_if = (if_rec_t *)calloc(1, sizeof(if_rec_t));
    if (!tmp_if)
    {
        return ZW_ERR_MEMORY;
    }

    tmp_if->rec.meter = (if_rec_meter_t *)calloc(1, sizeof(if_rec_meter_t));
    if (!tmp_if->rec.meter)
    {
        free(tmp_if);
        return ZW_ERR_MEMORY;
    }

    tmp_if->type = IF_REC_TYPE_METER;
    tmp_if->rec.meter->type = type;
    tmp_if->rec.meter->unit_supp = unit_supp;

    *if_rec = tmp_if;

    return ZW_ERR_NONE;

}


/**
zwdev_thrmo_setp_type_ld - Load setpoint type supported temparature range records from JSON array
@param[in]	json_if_arry    JSON array
@param[out]	setp_type_rec   Setpoint type array
@return		ZW_ERR_xxx
*/
static int zwdev_thrmo_setp_type_ld(cJSON *json_if_arry, zwsetp_temp_range_t *setp_type_rec)
{
    cJSON           *if_rec_json;       //interface record
    cJSON           *if_rec_ent;        //interface record entry
    int             res;
    int             i;
    unsigned        if_rec_ent_msk;     //Bit mask to check all mandatory entries in if_rec_ent
    zwnum_type_t    temp_val;

#define IF_ENTRY_MSK_SETP_TYPE          0x01
#define IF_ENTRY_MSK_SETP_MIN_UNIT      0x02
#define IF_ENTRY_MSK_SETP_MIN_VALUE     0x04
#define IF_ENTRY_MSK_SETP_MAX_UNIT      0x08
#define IF_ENTRY_MSK_SETP_MAX_VALUE     0x10

#define IF_ENTRY_MSK_SETPOINT_MATCH_ALL_PRESENT   (IF_ENTRY_MSK_SETP_TYPE | IF_ENTRY_MSK_SETP_MIN_UNIT | IF_ENTRY_MSK_SETP_MIN_VALUE \
                                                    | IF_ENTRY_MSK_SETP_MAX_UNIT | IF_ENTRY_MSK_SETP_MAX_VALUE)

    for (if_rec_json = json_if_arry->child, i=0;
         if_rec_json; if_rec_json = if_rec_json->next, i++)
    {   //Iterate each setpoint type
        for (if_rec_ent_msk = 0, if_rec_ent = if_rec_json->child;
            if_rec_ent; if_rec_ent = if_rec_ent->next)
        {
            if (if_rec_ent->type == cJSON_String)
            {
                if (!strcmp(if_rec_ent->string, "_comment"))
                {
                    continue;
                }
                if (!strcmp(if_rec_ent->string, "min_value") && ((if_rec_ent_msk & IF_ENTRY_MSK_SETP_MIN_VALUE) == 0))
                {
                    if_rec_ent_msk |= IF_ENTRY_MSK_SETP_MIN_VALUE;

                    res = util_str_2_num_type(if_rec_ent->valuestring, 2, &temp_val);

                    if (res != 0)
                    {
                        return ZW_ERR_VALUE;
                    }

                    setp_type_rec[i].min.size = temp_val.size;
                    setp_type_rec[i].min.precision = temp_val.precision;
                    memcpy(setp_type_rec[i].min.data, temp_val.data, temp_val.size);
                }
                else if (!strcmp(if_rec_ent->string, "max_value") && ((if_rec_ent_msk & IF_ENTRY_MSK_SETP_MAX_VALUE) == 0))
                {
                    if_rec_ent_msk |= IF_ENTRY_MSK_SETP_MAX_VALUE;

                    res = util_str_2_num_type(if_rec_ent->valuestring, 2, &temp_val);

                    if (res != 0)
                    {
                        return ZW_ERR_VALUE;
                    }

                    setp_type_rec[i].max.size = temp_val.size;
                    setp_type_rec[i].max.precision = temp_val.precision;
                    memcpy(setp_type_rec[i].max.data, temp_val.data, temp_val.size);
                }
            }
            else if (if_rec_ent->type == cJSON_Number)
            {
                if (!strcmp(if_rec_ent->string, "type") && ((if_rec_ent_msk & IF_ENTRY_MSK_SETP_TYPE) == 0))
                {
                    if_rec_ent_msk |= IF_ENTRY_MSK_SETP_TYPE;
                    setp_type_rec[i].min.type = setp_type_rec[i].max.type = (uint8_t)if_rec_ent->valueint;
                }
                else if (!strcmp(if_rec_ent->string, "min_unit") && ((if_rec_ent_msk & IF_ENTRY_MSK_SETP_MIN_UNIT) == 0))
                {
                    if_rec_ent_msk |= IF_ENTRY_MSK_SETP_MIN_UNIT;
                    setp_type_rec[i].min.unit = if_rec_ent->valueint;
                }
                else if (!strcmp(if_rec_ent->string, "max_unit") && ((if_rec_ent_msk & IF_ENTRY_MSK_SETP_MAX_UNIT) == 0))
                {
                    if_rec_ent_msk |= IF_ENTRY_MSK_SETP_MAX_UNIT;
                    setp_type_rec[i].max.unit = if_rec_ent->valueint;
                }
            }
        }
        //Check whether all the mandatory entries present
        if ((if_rec_ent_msk & IF_ENTRY_MSK_SETPOINT_MATCH_ALL_PRESENT) != IF_ENTRY_MSK_SETPOINT_MATCH_ALL_PRESENT)
        {
            return ZW_ERR_MISSING_ENTRY;
        }
    }

    return ZW_ERR_NONE;
}


/**
zwdev_thrmo_setp_if_parser - Thermostat setpoint interface parser of JSON object
@param[in]	json_if_rec	    JSON interface record
@param[in]	if_rec_lst_hd	Interface record list head
@param[out]	if_rec		    Interface record
@return		ZW_ERR_xxx
@post       Caller must free the memory allocated to if_rec if return value is ZW_ERR_NONE
*/
static int zwdev_thrmo_setp_if_parser(cJSON *json_if_rec, if_rec_t *if_rec_lst_hd, if_rec_t **if_rec)
{
    if_rec_t            *tmp_if = NULL;
    cJSON               *if_rec_ent;
    zwsetp_temp_range_t *setp_type_rec;
    unsigned            if_rec_ent_msk;     //Bit mask to check all mandatory entries in if_rec_ent
    int                 thrmo_setp_type_cnt;
    int                 res;

#define IF_ENTRY_MSK_THRMO_SETPOINT     0x01

    for (if_rec_ent_msk=0, if_rec_ent=json_if_rec->child;
        if_rec_ent != NULL; if_rec_ent=if_rec_ent->next)
    {
        if ((if_rec_ent->type == cJSON_String) && (!strcmp(if_rec_ent->string, "_comment")))
        {
            continue;
        }

        if ((if_rec_ent->type == cJSON_Array) && (!strcmp(if_rec_ent->string, "thrmo_setp")))
        {
            if (if_rec_ent_msk & IF_ENTRY_MSK_THRMO_SETPOINT)
            {   //Skip duplicate field
                continue;
            }
            if_rec_ent_msk |= IF_ENTRY_MSK_THRMO_SETPOINT;

            thrmo_setp_type_cnt = cJSON_GetArraySize(if_rec_ent);

            if (thrmo_setp_type_cnt > 0)
            {
                setp_type_rec = (zwsetp_temp_range_t *)calloc(thrmo_setp_type_cnt, sizeof(zwsetp_temp_range_t));

                if (!setp_type_rec)
                {
                    return ZW_ERR_MEMORY;
                }

                //Fill info for each setpoint type supported temparature range
                res = zwdev_thrmo_setp_type_ld(if_rec_ent, setp_type_rec);

                if (res != ZW_ERR_NONE)
                {
                    free(setp_type_rec);
                    return res;
                }

                //Create interface record and assign supported temparature ranges to it
                if (!tmp_if)
                {
                    tmp_if = (if_rec_t *)calloc(1, sizeof(if_rec_t));
                    if (tmp_if)
                    {
                        tmp_if->type = IF_REC_TYPE_THRMO_SETP;
                        tmp_if->rec.thrmo_setp = (if_rec_thrmo_setp_t *)calloc(1, sizeof(if_rec_thrmo_setp_t));

                        if (tmp_if->rec.thrmo_setp)
                        {
                            tmp_if->rec.thrmo_setp->type_cnt = thrmo_setp_type_cnt;
                            tmp_if->rec.thrmo_setp->temp_range = setp_type_rec;
                            *if_rec = tmp_if;
                        }
                        else
                        {
                            free(tmp_if);
                            free(setp_type_rec);
                            return ZW_ERR_MEMORY;
                        }
                    }
                    else
                    {
                        free(setp_type_rec);
                        return ZW_ERR_MEMORY;
                    }
                }
            }
        }
    }

    //Check whether all the mandatory entries present
    if (!if_rec_ent_msk)
    {
        return ZW_ERR_MISSING_ENTRY;
    }

    return ZW_ERR_NONE;
}


/**
zwdev_alrm_match_ld - Load alarm match record from JSON array
@param[in]		json_if_arry    Alarm match record JSON array
@param[in,out]	ppalrm_rec      Address of the alarm match recorder pointer
@return		ZW_ERR_xxx
@post       Caller must free the memory alloted if return value is ZW_ERR_NONE
*/
static int zwdev_alrm_match_ld(cJSON *json_if_arry, if_rec_alarm_match_t **ppalrm_rec)
{
    cJSON       *if_rec_json;       //Alarm interface record
    cJSON       *if_rec_ent;        //Alarm interface record entry
    cJSON       *if_lvl_range_ent;  //Alarm interface record entry
    if_rec_alarm_match_t *pMatchRec = *ppalrm_rec;
    int         j;
    int         res;
    int         alrm_range_count;
    unsigned    if_rec_ent_msk;     //Bit mask to check all mandatory entries in if_rec_ent

#define IF_ENTRY_MSK_ALRM_TYPE          0x01
#define IF_ENTRY_MSK_ALRM_LEVEL         0x02
#define IF_ENTRY_MSK_ALRM_ZWTYPE        0x04
#define IF_ENTRY_MSK_ALRM_ZWEVENT       0x08
#define IF_ENTRY_MSK_ALRM_ZWPARAM       0x10

//As long as has at least one of these param
//#define IF_ENTRY_MSK_ALRM_LVL_ALL_PRESENT   (IF_ENTRY_MSK_ALRM_LVL | IF_ENTRY_MSK_ALRM_LVL_NAME)


    for (if_rec_json=json_if_arry->child; if_rec_json; if_rec_json=if_rec_json->next)
    {
        for (if_rec_ent_msk = 0, if_rec_ent = if_rec_json->child;
            if_rec_ent != NULL; if_rec_ent = if_rec_ent->next)
        {
            if ((if_rec_ent->type == cJSON_String) && (!strcmp(if_rec_ent->string, "_comment")))
            {
                continue;
            }

            if (if_rec_ent->type == cJSON_Number)
            {
                if (!strcmp(if_rec_ent->string, "alarm_type") && ((if_rec_ent_msk & IF_ENTRY_MSK_ALRM_TYPE) == 0))
                {
                    if_rec_ent_msk |= IF_ENTRY_MSK_ALRM_TYPE;
                    pMatchRec->type = (uint8_t)if_rec_ent->valueint;
                }
                //If string is 'alarm_level' and alarm_level_low/high is not set in if_rec before
                //'alarm_level_range' will also set IF_ENTRY_MSK_ALRM_LVL. If  'alarm_level_range' is used,
                //ignore 'alarm_level' if there is any.
                else if (!strcmp(if_rec_ent->string, "alarm_level") && ((if_rec_ent_msk & IF_ENTRY_MSK_ALRM_LEVEL) == 0))
                {
                    if_rec_ent_msk |= IF_ENTRY_MSK_ALRM_LEVEL;
                    pMatchRec->isRange = 0;
                    pMatchRec->level_low = (uint8_t)if_rec_ent->valueint;
                }
                else if (!strcmp(if_rec_ent->string, "zw_alarm_type") && ((if_rec_ent_msk & IF_ENTRY_MSK_ALRM_ZWTYPE) == 0))
                {
                    if_rec_ent_msk |= IF_ENTRY_MSK_ALRM_ZWTYPE;
                    pMatchRec->ex_type = (uint8_t)if_rec_ent->valueint;
                }
                else if (!strcmp(if_rec_ent->string, "zw_alarm_event") && ((if_rec_ent_msk & IF_ENTRY_MSK_ALRM_ZWEVENT) == 0))
                {
                    if_rec_ent_msk |= IF_ENTRY_MSK_ALRM_ZWEVENT;
                    pMatchRec->ex_event = (uint8_t)if_rec_ent->valueint;
                }
            }
            else if (if_rec_ent->type == cJSON_Array)
            {
                if (!strcmp(if_rec_ent->string, "alarm_level_range"))
                {
                    if_rec_ent_msk |= IF_ENTRY_MSK_ALRM_LEVEL;

                    alrm_range_count = cJSON_GetArraySize(if_rec_ent);

                    if (alrm_range_count == 2) //Min and max value
                    {
                        pMatchRec->isRange = 1;
                        if_lvl_range_ent = if_rec_ent->child;
                        for (j = 0; j < alrm_range_count; j++)
                        {
                            if (if_lvl_range_ent->type == cJSON_Number)
                            {
                                if (j == 0)
                                    pMatchRec->level_low = (uint8_t)if_lvl_range_ent->valueint;
                                else
                                    pMatchRec->level_high = (uint8_t)if_lvl_range_ent->valueint;
                            }
                            else
                            {
                                res = ZW_ERR_VALUE;
                                goto l_ALRM_LVL_LD_ERR1;
                            }

                            if_lvl_range_ent = if_lvl_range_ent->next;
                        }
                    }
                    else
                    {
                        res = ZW_ERR_VALUE;
                        goto l_ALRM_LVL_LD_ERR1;
                    }
                }
                else if (!strcmp(if_rec_ent->string, "zw_alarm_param") && ((if_rec_ent_msk & IF_ENTRY_MSK_ALRM_ZWPARAM) == 0))
                {
                    alrm_range_count = cJSON_GetArraySize(if_rec_ent);

                    if (alrm_range_count > 0) //Need to have at least 1 param
                    {
                        uint8_t ex_evt_prm[248];
                        if_rec_ent_msk |= IF_ENTRY_MSK_ALRM_ZWPARAM;

                        pMatchRec->pEx_evt_prm = calloc(1, alrm_range_count);

                        if (!pMatchRec->pEx_evt_prm)
                        {
                            res = ZW_ERR_MEMORY;
                            goto l_ALRM_LVL_LD_ERR1;
                        }

                        for (if_lvl_range_ent = if_rec_ent->child, j = 0;
                            if_lvl_range_ent != NULL && j < alrm_range_count;
                            if_lvl_range_ent = if_lvl_range_ent->next, j++)
                        {
                            if (if_lvl_range_ent->type == cJSON_Number)
                            {
                                ex_evt_prm[j] = (uint8_t)if_lvl_range_ent->valueint;
                            }
                            else
                            {
                                res = ZW_ERR_VALUE;
                                goto l_ALRM_LVL_LD_ERR1;
                            }
                        }

                        memcpy(pMatchRec->pEx_evt_prm, ex_evt_prm, j);
                        pMatchRec->ex_evt_len = j;
                    }
                }
            }
        }
        //Check whether all the mandatory entries present
        //As long as has at least one of these param
        if (if_rec_ent_msk == 0)
        {
            res = ZW_ERR_MISSING_ENTRY;
            goto l_ALRM_LVL_LD_ERR1;
        }

        pMatchRec->match_valid = 1;
        *ppalrm_rec = pMatchRec; //in case pMatchRec has been realloc
    }

    return ZW_ERR_NONE;

l_ALRM_LVL_LD_ERR1:
    zwdev_alrm_if_rec_free(pMatchRec);
    *ppalrm_rec = NULL;
    return res;
}

/**
zwdev_alrm_result_ld - Load alarm result record from JSON array
@param[in]		json_if_arry    Alarm match record JSON array
@param[in,out]	palrm_rec       Alarm match recorder pointer
@return		ZW_ERR_xxx
@post       Caller must free the memory alloted if return value is ZW_ERR_NONE
*/
static int zwdev_alrm_result_ld(cJSON *json_if_arry, if_rec_alarm_match_t *palrm_rec)
{
    if_rec_alarm_result_t *pResultRec = palrm_rec->pResult;
    cJSON       *if_rec_json;       //Alarm interface record
    cJSON       *if_rec_ent;        //Alarm interface record entry
    cJSON       *if_lvl_range_ent;  //Alarm interface record entry
    int         j;
    int         res;
    int         alrm_range_count;
    unsigned    if_rec_ent_msk;     //Bit mask to check all mandatory entries in if_rec_ent

#define IF_ENTRY_MSK_ALRM_TYPE          0x01
#define IF_ENTRY_MSK_ALRM_LEVEL         0x02
#define IF_ENTRY_MSK_ALRM_ZWTYPE        0x04
#define IF_ENTRY_MSK_ALRM_ZWEVENT       0x08
#define IF_ENTRY_MSK_ALRM_ZWPARAM       0x10
#define IF_ENTRY_MSK_ALRM_ZWPARAM_TYPE  0x20
#define IF_ENTRY_MSK_ALRM_TYPE_NAME		0x40
#define IF_ENTRY_MSK_ALRM_LEVEL_NAME	0x80

    //As long as has at least one of these param
    //#define IF_ENTRY_MSK_ALRM_LVL_ALL_PRESENT   (IF_ENTRY_MSK_ALRM_LVL | IF_ENTRY_MSK_ALRM_LVL_NAME)


    for (if_rec_json = json_if_arry->child; if_rec_json; if_rec_json = if_rec_json->next)
    {
        for (if_rec_ent_msk = 0, if_rec_ent = if_rec_json->child;
            if_rec_ent != NULL; if_rec_ent = if_rec_ent->next)
        {
            if ((if_rec_ent->type == cJSON_String) && (!strcmp(if_rec_ent->string, "_comment")))
            {
                continue;
            }

            if (if_rec_ent->type == cJSON_Number)
            {
                if (!strcmp(if_rec_ent->string, "alarm_type") && ((if_rec_ent_msk & IF_ENTRY_MSK_ALRM_TYPE) == 0))
                {
                    if_rec_ent_msk |= IF_ENTRY_MSK_ALRM_TYPE;
                    pResultRec->type = (uint8_t)if_rec_ent->valueint;
                }
                //If string is 'alarm_level' and alarm_level_low/high is not set in if_rec before
                //'alarm_level_range' will also set IF_ENTRY_MSK_ALRM_LVL. If  'alarm_level_range' is used,
                //ignore 'alarm_level' if there is any.
                else if (!strcmp(if_rec_ent->string, "alarm_level") && ((if_rec_ent_msk & IF_ENTRY_MSK_ALRM_LEVEL) == 0))
                {
                    if_rec_ent_msk |= IF_ENTRY_MSK_ALRM_LEVEL;
                    pResultRec->level = (uint8_t)if_rec_ent->valueint;
                }
                else if (!strcmp(if_rec_ent->string, "zw_alarm_type") && ((if_rec_ent_msk & IF_ENTRY_MSK_ALRM_ZWTYPE) == 0))
                {
                    if_rec_ent_msk |= IF_ENTRY_MSK_ALRM_ZWTYPE;
                    pResultRec->ex_type = (uint8_t)if_rec_ent->valueint;
                }
                else if (!strcmp(if_rec_ent->string, "zw_alarm_event") && ((if_rec_ent_msk & IF_ENTRY_MSK_ALRM_ZWEVENT) == 0))
                {
                    if_rec_ent_msk |= IF_ENTRY_MSK_ALRM_ZWEVENT;
                    pResultRec->ex_event = (uint8_t)if_rec_ent->valueint;
                }
                else if (!strcmp(if_rec_ent->string, "zw_alarm_param_type") && ((if_rec_ent_msk & IF_ENTRY_MSK_ALRM_ZWPARAM_TYPE) == 0))
                {
                    if_rec_ent_msk |= IF_ENTRY_MSK_ALRM_ZWPARAM_TYPE;
                    pResultRec->ex_evt_type = (uint8_t)if_rec_ent->valueint;
                }
            }
            else if (if_rec_ent->type == cJSON_String)
            {
                if (!strcmp(if_rec_ent->string, "alarm_type_name") && ((if_rec_ent_msk & IF_ENTRY_MSK_ALRM_TYPE_NAME) == 0))
                {
                    if_rec_ent_msk |= IF_ENTRY_MSK_ALRM_TYPE_NAME;

                    pResultRec->type_name = (char *)calloc(1, MAX_ALRM_NAME_LEN + 1);
                    if (!pResultRec->type_name)
                    {
                        res = ZW_ERR_MEMORY;
                        goto l_ALRM_LVL_LD_ERR1;
                    }

#ifdef USE_SAFE_VERSION
                    strncpy_s(pResultRec->type_name, MAX_ALRM_NAME_LEN + 1, if_rec_ent->valuestring, _TRUNCATE);
#else
                    strncpy(pResultRec->type_name, if_rec_ent->valuestring, MAX_ALRM_NAME_LEN);
#endif
                }
                else if (!strcmp(if_rec_ent->string, "alarm_level_name") && ((if_rec_ent_msk & IF_ENTRY_MSK_ALRM_LEVEL_NAME) == 0))
                {
                    if_rec_ent_msk |= IF_ENTRY_MSK_ALRM_LEVEL_NAME;

                    pResultRec->level_name = (char *)calloc(1, MAX_ALRM_LVL_NAME_LEN + 1);
                    if (!pResultRec->level_name)
                    {
                        res = ZW_ERR_MEMORY;
                        goto l_ALRM_LVL_LD_ERR1;
                    }

#ifdef USE_SAFE_VERSION
                    strncpy_s(pResultRec->level_name, MAX_ALRM_LVL_NAME_LEN + 1, if_rec_ent->valuestring, _TRUNCATE);
#else
                    strncpy(pResultRec->level_name, if_rec_ent->valuestring, MAX_ALRM_LVL_NAME_LEN);
#endif
                }
            }
            else if (if_rec_ent->type == cJSON_Array)
            {
                if (!strcmp(if_rec_ent->string, "zw_alarm_param") && ((if_rec_ent_msk & IF_ENTRY_MSK_ALRM_ZWPARAM) == 0))
                {
                    alrm_range_count = cJSON_GetArraySize(if_rec_ent);

                    if (alrm_range_count > 0) //Need to have at least 1 param
                    {
                        if_rec_ent_msk |= IF_ENTRY_MSK_ALRM_ZWPARAM;

                        if (alrm_range_count > 0) //Need to have at least 1 param
                        {
                            uint8_t ex_evt_prm[248];
                            if_rec_ent_msk |= IF_ENTRY_MSK_ALRM_ZWPARAM;

                            pResultRec->pEx_evt_prm = calloc(1, alrm_range_count);

                            if (!pResultRec->pEx_evt_prm)
                            {
                                res = ZW_ERR_MEMORY;
                                goto l_ALRM_LVL_LD_ERR1;
                            }

                            for (if_lvl_range_ent = if_rec_ent->child, j = 0;
                                if_lvl_range_ent != NULL && j < alrm_range_count;
                                if_lvl_range_ent = if_lvl_range_ent->next, j++)
                            {
                                if (if_lvl_range_ent->type == cJSON_Number)
                                {
                                    ex_evt_prm[j] = (uint8_t)if_lvl_range_ent->valueint;
                                }
                                else
                                {
                                    res = ZW_ERR_VALUE;
                                    goto l_ALRM_LVL_LD_ERR1;
                                }
                            }

                            memcpy(pResultRec->pEx_evt_prm, ex_evt_prm, j);
                            pResultRec->ex_evt_len = j;
                        }
                    }
                }
            }
        }
        //Check whether all the mandatory entries present
        //As long as has at least one of these param
        if (if_rec_ent_msk == 0)
        {
            res = ZW_ERR_MISSING_ENTRY;
            goto l_ALRM_LVL_LD_ERR1;
        }

        palrm_rec->pResult = pResultRec; //in case pMatchRec has been realloc
    }

    return ZW_ERR_NONE;

l_ALRM_LVL_LD_ERR1:
    return res;
}

/**
zwdev_alrm_reverse_match_ld - Fill in the reverse match block info
@param[in,out]	palrm_rec       Alarm match recorder pointer
*/
static void zwdev_alrm_reverse_match_ld(if_rec_alarm_match_t *palrm_rec)
{
    //Logic: OR the match block and result block.
    //If an entry in both block are -1, reverse match entry is also -1.
    //If an entry is -1 in one block but valid value in another block, reverse match entry will be valid value
    //If an entry has valid values in both block, reserver match entry will take the result block value
    if_rec_alarm_result_t *pResult = palrm_rec->pResult;
    if_rec_alarm_rev_match_t *pRevMatch = palrm_rec->pRevMatch;

    if ((palrm_rec->type) == -1 && (pResult->type) == -1)
    {
        pRevMatch->type = -1;
    }
    else if ((palrm_rec->type != -1) && (pResult->type) != -1)
    {
        pRevMatch->type = pResult->type;
    }
    else if (palrm_rec->type != -1)
    {
        pRevMatch->type = palrm_rec->type;
    }
    else
    {
        pRevMatch->type = pResult->type;
    }

    if ((palrm_rec->ex_type) == -1 && (pResult->ex_type) == -1)
    {
        pRevMatch->ex_type = -1;
    }
    else if ((palrm_rec->ex_type != -1) && (pResult->ex_type) != -1)
    {
        pRevMatch->ex_type = pResult->ex_type;
    }
    else if (palrm_rec->ex_type != -1)
    {
        pRevMatch->ex_type = palrm_rec->ex_type;
    }
    else
    {
        pRevMatch->ex_type = pResult->ex_type;
    }

    if ((palrm_rec->ex_event) == -1 && (pResult->ex_event) == -1)
    {
        pRevMatch->ex_event = -1;
    }
    else if ((palrm_rec->ex_event != -1) && (pResult->ex_event) != -1)
    {
        pRevMatch->ex_event = pResult->ex_event;
    }
    else if (palrm_rec->ex_event != -1)
    {
        pRevMatch->ex_event = palrm_rec->ex_event;
    }
    else
    {
        pRevMatch->ex_event = pResult->ex_event;
    }
}

/**
zwdev_alrm_if_ld - Load alarm interface records from JSON array
@param[in]	json_if_arry        Alarm interface JSON array
@param[out]	ppif_alrm_first_rec	Alarm interface first match record
@return		ZW_ERR_xxx
@post       Caller must call zwdev_alrm_if_free() if return value is ZW_ERR_NONE
*/
static int zwdev_alrm_if_ld(cJSON *json_if_arry, if_rec_alarm_match_t **ppif_alrm_first_rec)
{
    if_rec_alarm_match_t    *curr_alrm_rec = NULL;
    if_rec_alarm_match_t    *alarm_rec = NULL;
    cJSON                   *if_rec_json;       //Alarm interface record
    cJSON                   *if_rec_ent;        //Alarm interface record entry
    int                     res = ZW_ERR_VALUE;
    int                     alrm_rec_cnt;
    unsigned                if_rec_ent_msk;     //Bit mask to check all mandatory entries in if_rec_ent

#define IF_ENTRY_MSK_ALRM_MATCH_REC		0x01
#define IF_ENTRY_MSK_ALRM_RESULT_REC    0x02

    //Define the mandatory fields
#define IF_ENTRY_MSK_ALRM_REC_ALL_PRESENT   (IF_ENTRY_MSK_ALRM_MATCH_REC | IF_ENTRY_MSK_ALRM_RESULT_REC)

    curr_alrm_rec = NULL;

    for (if_rec_json=json_if_arry->child;
        if_rec_json; if_rec_json=if_rec_json->next)
    {
        for (if_rec_ent_msk = 0, alarm_rec = NULL, if_rec_ent = if_rec_json->child;
            if_rec_ent != NULL; if_rec_ent=if_rec_ent->next)
        {
            switch (if_rec_ent->type)
            {
                case cJSON_String:
                    if (!strcmp(if_rec_ent->string, "_comment"))
                        break;

                case cJSON_Array:
                    if (!strcmp(if_rec_ent->string, "match"))
                    {
                        if (if_rec_ent_msk & IF_ENTRY_MSK_ALRM_MATCH_REC)
                        {   //Skip duplicate field
                            break;
                        }

                        if_rec_ent_msk |= IF_ENTRY_MSK_ALRM_MATCH_REC;

                        alrm_rec_cnt = cJSON_GetArraySize(if_rec_ent);

                        if (alrm_rec_cnt != 1)  //expect only 1 match record
                            goto l_ALRM_IF_LD_ERR1;

                        if (!alarm_rec)
                        {
                            zwdev_alrm_if_rec_create(&alarm_rec);
                            if (!alarm_rec)
                                return ZW_ERR_MEMORY;
                        }

                        //Fill info for the match record
                        res = zwdev_alrm_match_ld(if_rec_ent, &alarm_rec);

                        if (res != ZW_ERR_NONE)
                        {
                            goto l_ALRM_IF_LD_ERR1;
                        }

                    }
                    else if (!strcmp(if_rec_ent->string, "result"))
                    {
                        if (if_rec_ent_msk & IF_ENTRY_MSK_ALRM_RESULT_REC)
                        {   //Skip duplicate field
                            break;
                        }

                        if_rec_ent_msk |= IF_ENTRY_MSK_ALRM_RESULT_REC;

                        alrm_rec_cnt = cJSON_GetArraySize(if_rec_ent);

                        if (alrm_rec_cnt != 1)  //expect only 1 result record
                            goto l_ALRM_IF_LD_ERR1;

                        if (!alarm_rec)
                        {
                            zwdev_alrm_if_rec_create(&alarm_rec);
                            if (!alarm_rec)
                                return ZW_ERR_MEMORY;
                        }

                        //Fill info for the result record
                        res = zwdev_alrm_result_ld(if_rec_ent, alarm_rec);

                        if (res != ZW_ERR_NONE)
                        {
                            goto l_ALRM_IF_LD_ERR1;
                        }

                    }
                    break;

            }

        }

        //Check whether all the mandatory entries present
        if ((if_rec_ent_msk & IF_ENTRY_MSK_ALRM_REC_ALL_PRESENT) != IF_ENTRY_MSK_ALRM_REC_ALL_PRESENT)
        {
            res = ZW_ERR_MISSING_ENTRY;
            goto l_ALRM_IF_LD_ERR1;
        }

        //Fill in the reverse match entry.
        zwdev_alrm_reverse_match_ld(alarm_rec);

        //Append to alarm list
        if (curr_alrm_rec)
        {
            curr_alrm_rec->next = alarm_rec;
        }
        else
        {
            *ppif_alrm_first_rec = alarm_rec;
        }
        curr_alrm_rec = alarm_rec;

    }

    return ZW_ERR_NONE;

l_ALRM_IF_LD_ERR1:
    zwdev_alrm_if_rec_free(alarm_rec);
    return res;
}


/**
zwdev_alrm_if_parser - Alarm interface parser of JSON object
@param[in]	json_if_rec	    JSON interface record
@param[in]	if_rec_lst_hd	Interface record list head
@param[out]	if_rec		    Interface record
@return		ZW_ERR_xxx
@post       Caller must free the memory allocated to if_rec if return value is ZW_ERR_NONE
*/
static int zwdev_alrm_if_parser(cJSON *json_if_rec, if_rec_t *if_rec_lst_hd, if_rec_t **if_rec)
{
    if_rec_t        *tmp_if;
    if_rec_t        *res_if_rec;
    cJSON           *if_rec_ent;        //Device alarm interface record entry
    unsigned        if_rec_ent_msk;     //Bit mask to check all mandatory entries in if_rec_ent
    int             alrm_cnt;
    int             res;
    int             no_evt_clr = 0;
    if_rec_alarm_match_t *if_alrm_first_rec = NULL;
    uint16_t        tmout = DFLT_EVT_CLR_TIMEOUT;        //Timeout default value

#define IF_ENTRY_MSK_ALRM           0x01
#define IF_ENTRY_MSK_ALRM_EVT_CLR   0x02

//#define IF_ENTRY_MSK_ALRM_ALL_PRESENT   (IF_ENTRY_MSK_ALRM)

    for (if_rec_ent_msk=0, if_rec_ent=json_if_rec->child;
        if_rec_ent != NULL; if_rec_ent=if_rec_ent->next)
    {
        if ((if_rec_ent->type == cJSON_String) && (!strcmp(if_rec_ent->string, "_comment")))
        {
            continue;
        }

        if (if_rec_ent->type == cJSON_Number)
        {
            if (!strcmp(if_rec_ent->string, "no_event_clear"))
            {
                if_rec_ent_msk |= IF_ENTRY_MSK_ALRM_EVT_CLR;
                no_evt_clr = if_rec_ent->valueint;
            }
            else if (!strcmp(if_rec_ent->string, "event_clear_timeout"))
            {
                tmout = (uint16_t)if_rec_ent->valueint;
            }
        }
        else if ((if_rec_ent->type == cJSON_Array) && (!strcmp(if_rec_ent->string, "alarm")))
        {
            if (if_rec_ent_msk & IF_ENTRY_MSK_ALRM)
            {   //Skip duplicate field
                continue;
            }
            if_rec_ent_msk |= IF_ENTRY_MSK_ALRM;

            alrm_cnt = cJSON_GetArraySize(if_rec_ent);

            if (alrm_cnt > 0)
            {
                res = zwdev_alrm_if_ld(if_rec_ent, &if_alrm_first_rec);

                if (res != ZW_ERR_NONE)
                {
                    return res;
                }
            }
        }
    }

    //Check whether all the mandatory entries present
    if (!if_rec_ent_msk)
    {
        res = ZW_ERR_MISSING_ENTRY;
        goto l_ALARM_IF_PARSER_ERR1;
    }

    //Check whether the alarm interface record has been created by other interface parser (e.g. basic, alarm sensor and binary sensor)
    res_if_rec = zwdev_if_rec_detach(if_rec_lst_hd, IF_REC_TYPE_ALARM);

    if (res_if_rec)
    {
        //Alarm interface record exists
        tmp_if = res_if_rec;

    }
    else
    {   //No existing alarm interface record
        //Allocate memory for the interface
        tmp_if = (if_rec_t *)calloc(1, sizeof(if_rec_t));
        if (!tmp_if)
        {
            res = ZW_ERR_MEMORY;
            goto l_ALARM_IF_PARSER_ERR1;
        }
    }

    tmp_if->type = IF_REC_TYPE_ALARM;
    //Insert the alarm match record
    zwdev_alrm_match_append(&tmp_if->rec.alarm, if_alrm_first_rec);

    tmp_if->propty = (no_evt_clr == 0)? IF_PROPTY_ALRM_EVT_CLR : 0;
    if (no_evt_clr)
    {
        tmp_if->tmout = (tmout > MAX_EVT_CLR_TIMEOUT)? MAX_EVT_CLR_TIMEOUT : tmout;
    }

    *if_rec = tmp_if;

    return ZW_ERR_NONE;

l_ALARM_IF_PARSER_ERR1:

    zwdev_alrm_if_rec_free(if_alrm_first_rec);
    return res;
}


/**
zwdev_config_prm_set_if_ld - Load configuration interface records from JSON array
@param[in]	json_if_arry        Configuration interface JSON array
@param[out]	if_cfg_first_rec	Configuration interface first record
@return		ZW_ERR_xxx
@post       Caller must call zwdev_cfg_if_free() if return value is ZW_ERR_NONE
*/
static int zwdev_config_prm_set_if_ld(cJSON *json_if_arry, if_rec_config_t **ppif_cfg_first_rec)
{
    if_rec_config_t     *curr_cfg_rec;
    if_rec_config_t     *config_rec;
    cJSON               *if_rec_json;       //Configuration interface record
    cJSON               *if_rec_ent;        //Configuration interface record entry
    int         res;
    unsigned    if_rec_ent_msk;     //Bit mask to check all mandatory entries in if_rec_ent
    int32_t     param_val = 0;
    uint8_t     param_num = 0;
    uint8_t     param_size = 0;

#define IF_ENTRY_MSK_CFG_PRM_NUM   0x01
#define IF_ENTRY_MSK_CFG_PRM_SIZE  0x02
#define IF_ENTRY_MSK_CFG_PRM_VAL   0x04

#define IF_ENTRY_MSK_CFG_PRM_ALL_PRESENT   (IF_ENTRY_MSK_CFG_PRM_NUM | IF_ENTRY_MSK_CFG_PRM_SIZE | IF_ENTRY_MSK_CFG_PRM_VAL)
    //Init
    curr_cfg_rec = NULL;
    //if_cfg_first_rec->next = NULL;

    for (if_rec_json=json_if_arry->child;
        if_rec_json; if_rec_json=if_rec_json->next)
    {   //Iterate each configuration parameter

        for (if_rec_ent_msk=0, if_rec_ent=if_rec_json->child;
            if_rec_ent != NULL; if_rec_ent=if_rec_ent->next)
        {
            if ((if_rec_ent->type == cJSON_String) && (!strcmp(if_rec_ent->string, "_comment")))
            {
                continue;
            }

            if (if_rec_ent->type == cJSON_Number)
            {
                if (!strcmp(if_rec_ent->string, "param_num"))
                {
                    if_rec_ent_msk |= IF_ENTRY_MSK_CFG_PRM_NUM;
                    param_num = (uint8_t)if_rec_ent->valueint;
                }
                else if (!strcmp(if_rec_ent->string, "param_size"))
                {
                    param_size = (uint8_t)if_rec_ent->valueint;
                    //Check for param size
                    if ((param_size == 1) || (param_size == 2) || (param_size == 4))
                    {
                        if_rec_ent_msk |= IF_ENTRY_MSK_CFG_PRM_SIZE;
                    }
                }
                else if (!strcmp(if_rec_ent->string, "param_val"))
                {
                    if_rec_ent_msk |= IF_ENTRY_MSK_CFG_PRM_VAL;
                    param_val = (int32_t)if_rec_ent->valueint;
                }
            }
        }

        //Check whether all the mandatory entries present
        if ((if_rec_ent_msk & IF_ENTRY_MSK_CFG_PRM_ALL_PRESENT) != IF_ENTRY_MSK_CFG_PRM_ALL_PRESENT)
        {
            res = ZW_ERR_MISSING_ENTRY;
            goto l_CFG_IF_LD_ERR1;
        }

        //Allocate memory for the configuration record
        config_rec = (if_rec_config_t *)calloc(1, sizeof(if_rec_config_t));
        if (!config_rec)
        {
            res = ZW_ERR_MEMORY;
            goto l_CFG_IF_LD_ERR1;
        }

        config_rec->rec_type = CONFIG_REC_TYPE_SET;
        config_rec->rec.cfg_set.param_num = param_num;
        config_rec->rec.cfg_set.param_size = param_size;
        config_rec->rec.cfg_set.param_val = param_val;

        //Append to configuration list
        if (curr_cfg_rec)
        {
            curr_cfg_rec->next = config_rec;
        }
        else
        {   //Copy to first record
            *ppif_cfg_first_rec = config_rec;
        }
        curr_cfg_rec = config_rec;

    }

    return ZW_ERR_NONE;

l_CFG_IF_LD_ERR1:
    REMOVE_STRUCTLIST((*ppif_cfg_first_rec), if_rec_config_t)
    return res;
}


/**
zwdev_config_prm_info_if_ld - Load configuration interface records from JSON array
@param[in]	json_if_arry        Configuration interface JSON array
@param[out]	if_cfg_first_rec	Configuration interface first record
@return		ZW_ERR_xxx
@post       Caller must call zwdev_cfg_if_free() if return value is ZW_ERR_NONE
*/
static int zwdev_config_prm_info_if_ld(cJSON *json_if_arry, if_rec_config_t **ppif_cfg_first_rec)
{
    if_rec_config_t     *curr_cfg_rec;
    if_rec_config_t     *config_rec;
    cJSON               *if_rec_json;       //Configuration interface record
    cJSON               *if_rec_ent;        //Configuration interface record entry
    int         res;
    unsigned    if_rec_ent_msk;     //Bit mask to check all mandatory entries in if_rec_ent
    char        *param_name = NULL;
    char        *param_info = NULL;
    int32_t     min_param_val = 0;
    int32_t     max_param_val = 0;
    int32_t     deflt_param_val = 0;
    uint8_t     param_num = 0;
    uint8_t     param_size = 0;

#define IF_ENTRY_MSK_CFG_PRM_NUM        0x01
#define IF_ENTRY_MSK_CFG_PRM_SIZE       0x02
#define IF_ENTRY_MSK_CFG_PRM_VAL_MIN    0x04
#define IF_ENTRY_MSK_CFG_PRM_VAL_MAX    0x08
#define IF_ENTRY_MSK_CFG_PRM_VAL_DFLT   0x10
#define IF_ENTRY_MSK_CFG_PRM_NAME       0x20
#define IF_ENTRY_MSK_CFG_PRM_INFO       0x40

#define IF_ENTRY_MSK_CFG_INFO_ALL_PRESENT  (IF_ENTRY_MSK_CFG_PRM_NUM | IF_ENTRY_MSK_CFG_PRM_SIZE | IF_ENTRY_MSK_CFG_PRM_VAL_MIN | \
                                            IF_ENTRY_MSK_CFG_PRM_VAL_MAX | IF_ENTRY_MSK_CFG_PRM_VAL_DFLT | \
                                            IF_ENTRY_MSK_CFG_PRM_NAME | IF_ENTRY_MSK_CFG_PRM_INFO)
    //Init
    curr_cfg_rec = NULL;
    //if_cfg_first_rec->next = NULL;

    for (if_rec_json=json_if_arry->child;
        if_rec_json; if_rec_json=if_rec_json->next)
    {   //Iterate each configuration parameter

        for (if_rec_ent_msk=0, param_name=NULL, param_info=NULL, if_rec_ent=if_rec_json->child;
            if_rec_ent != NULL; if_rec_ent=if_rec_ent->next)
        {
            if (if_rec_ent->type == cJSON_String)
            {
                if (!strcmp(if_rec_ent->string, "_comment"))
                {
                    continue;
                }

                if (!strcmp(if_rec_ent->string, "param_name") && ((if_rec_ent_msk & IF_ENTRY_MSK_CFG_PRM_NAME) == 0))
                {
                    if (if_rec_ent->valuestring && strlen(if_rec_ent->valuestring))
                    {
                        if_rec_ent_msk |= IF_ENTRY_MSK_CFG_PRM_NAME;

                        param_name = strdup(if_rec_ent->valuestring);
                    }
                }
                else if (!strcmp(if_rec_ent->string, "param_info") && ((if_rec_ent_msk & IF_ENTRY_MSK_CFG_PRM_INFO) == 0))
                {
                    if (if_rec_ent->valuestring && strlen(if_rec_ent->valuestring))
                    {
                        if_rec_ent_msk |= IF_ENTRY_MSK_CFG_PRM_INFO;

                        param_info = strdup(if_rec_ent->valuestring);
                    }
                }
            }
            else if (if_rec_ent->type == cJSON_Number)
            {
                if (!strcmp(if_rec_ent->string, "param_num"))
                {
                    if_rec_ent_msk |= IF_ENTRY_MSK_CFG_PRM_NUM;
                    param_num = (uint8_t)if_rec_ent->valueint;
                }
                else if (!strcmp(if_rec_ent->string, "param_size"))
                {
                    param_size = (uint8_t)if_rec_ent->valueint;
                    //Check for param size
                    if ((param_size == 1) || (param_size == 2) || (param_size == 4))
                    {
                        if_rec_ent_msk |= IF_ENTRY_MSK_CFG_PRM_SIZE;
                    }
                }
                else if (!strcmp(if_rec_ent->string, "param_min"))
                {
                    if_rec_ent_msk |= IF_ENTRY_MSK_CFG_PRM_VAL_MIN;
                    min_param_val = (int32_t)if_rec_ent->valueint;
                }
                else if (!strcmp(if_rec_ent->string, "param_max"))
                {
                    if_rec_ent_msk |= IF_ENTRY_MSK_CFG_PRM_VAL_MAX;
                    max_param_val = (int32_t)if_rec_ent->valueint;
                }
                else if (!strcmp(if_rec_ent->string, "param_deflt"))
                {
                    if_rec_ent_msk |= IF_ENTRY_MSK_CFG_PRM_VAL_DFLT;
                    deflt_param_val = (int32_t)if_rec_ent->valueint;
                }
            }
        }

        //Check whether all the mandatory entries present
        if ((if_rec_ent_msk & IF_ENTRY_MSK_CFG_INFO_ALL_PRESENT) != IF_ENTRY_MSK_CFG_INFO_ALL_PRESENT)
        {
            res = ZW_ERR_MISSING_ENTRY;
            goto l_CFG_SET_IF_LD_ERR1;
        }

        //Allocate memory for the configuration record
        config_rec = (if_rec_config_t *)calloc(1, sizeof(if_rec_config_t));
        if (!config_rec)
        {
            res = ZW_ERR_MEMORY;
            goto l_CFG_SET_IF_LD_ERR1;
        }

        config_rec->rec_type = CONFIG_REC_TYPE_INFO;
        config_rec->rec.cfg_info.param_num = param_num;
        config_rec->rec.cfg_info.param_size = param_size;
        config_rec->rec.cfg_info.param_min = min_param_val;
        config_rec->rec.cfg_info.param_max = max_param_val;
        config_rec->rec.cfg_info.param_deflt = deflt_param_val;
        config_rec->rec.cfg_info.param_name = param_name;
        config_rec->rec.cfg_info.param_info = param_info;

        //Append to configuration list
        if (curr_cfg_rec)
        {
            curr_cfg_rec->next = config_rec;
        }
        else
        {   //Copy to first record
            *ppif_cfg_first_rec = config_rec;
        }
        curr_cfg_rec = config_rec;

    }

    return ZW_ERR_NONE;

l_CFG_SET_IF_LD_ERR1:
    free(param_name);
    free(param_info);
    zwdev_config_prm_rec_free(*ppif_cfg_first_rec);
    return res;
}


/**
zwdev_config_prm_if_parser - Configuration interface parser of JSON object
@param[in]	json_if_rec	    JSON interface record
@param[in]	if_rec_lst_hd	Interface record list head
@param[out]	if_rec		    Interface record
@return		ZW_ERR_xxx
@post       Caller must free the memory allocated to if_rec if return value is ZW_ERR_NONE
*/
static int zwdev_config_prm_if_parser(cJSON *json_if_rec, if_rec_t *if_rec_lst_hd, if_rec_t **if_rec)
{
    if_rec_t        *tmp_if;
    cJSON           *if_rec_ent;        //Device configuration interface record entry
    unsigned        if_rec_ent_msk;     //Bit mask to check all mandatory entries in if_rec_ent
    int             cfg_cnt;
    int             res;
    if_rec_config_t *if_cfg_set_rec_hd = NULL;
    if_rec_config_t *if_cfg_info_rec_hd = NULL;

#define IF_ENTRY_MSK_CFG_SET   0x01
#define IF_ENTRY_MSK_CFG_INFO  0x02

    for (if_rec_ent_msk=0, if_rec_ent=json_if_rec->child;
        if_rec_ent != NULL; if_rec_ent=if_rec_ent->next)
    {
        if ((if_rec_ent->type == cJSON_String) && (!strcmp(if_rec_ent->string, "_comment")))
        {
            continue;
        }

        if (if_rec_ent->type == cJSON_Array)
        {
            if (!strcmp(if_rec_ent->string, "config"))
            {   //Configuration parameter set record
                if (if_rec_ent_msk & IF_ENTRY_MSK_CFG_SET)
                {   //Skip duplicate field
                    continue;
                }
                if_rec_ent_msk |= IF_ENTRY_MSK_CFG_SET;

                cfg_cnt = cJSON_GetArraySize(if_rec_ent);

                if (cfg_cnt > 0)
                {
                    res = zwdev_config_prm_set_if_ld(if_rec_ent, &if_cfg_set_rec_hd);

                    if (res != ZW_ERR_NONE)
                    {
                        zwdev_config_prm_rec_free(if_cfg_info_rec_hd);
                        return res;
                    }
                }
            }
            else if (!strcmp(if_rec_ent->string, "config_info"))
            {   //Configuration parameter information record
                if (if_rec_ent_msk & IF_ENTRY_MSK_CFG_INFO)
                {   //Skip duplicate field
                    continue;
                }
                if_rec_ent_msk |= IF_ENTRY_MSK_CFG_INFO;

                cfg_cnt = cJSON_GetArraySize(if_rec_ent);

                if (cfg_cnt > 0)
                {
                    res = zwdev_config_prm_info_if_ld(if_rec_ent, &if_cfg_info_rec_hd);

                    if (res != ZW_ERR_NONE)
                    {
                        zwdev_config_prm_rec_free(if_cfg_set_rec_hd);
                        return res;
                    }
                }
            }
        }
    }

    //Check for at least one entry present
    if (!if_rec_ent_msk)
    {
        res = ZW_ERR_MISSING_ENTRY;
        goto l_CFG_LD_ERR1;
    }

    //Allocate memory for the interface
    tmp_if = (if_rec_t *)calloc(1, sizeof(if_rec_t));
    if (!tmp_if)
    {
        res = ZW_ERR_MEMORY;
        goto l_CFG_LD_ERR1;
    }

    tmp_if->type = IF_REC_TYPE_CONFIG;
    tmp_if->rec.config = if_cfg_set_rec_hd;//Set record MUST always precede info record

    if (if_cfg_info_rec_hd)
    {
        if (tmp_if->rec.config)
        {   //Append to the end of record
            if_rec_config_t *last_rec;

            FIND_LASTSTRUCT(tmp_if->rec.config, last_rec);
            last_rec->next = if_cfg_info_rec_hd;
        }
        else
        {
            tmp_if->rec.config = if_cfg_info_rec_hd;
        }
    }

    *if_rec = tmp_if;

    return ZW_ERR_NONE;

l_CFG_LD_ERR1:
    zwdev_config_prm_rec_free(if_cfg_set_rec_hd);
    zwdev_config_prm_rec_free(if_cfg_info_rec_hd);
    return res;
}


/**
zwdev_grp_if_parser - Interface parser of JSON object
@param[in]	json_if_rec	    JSON interface record
@param[in]	if_rec_lst_hd	Interface record list head
@param[out]	if_rec		    Interface record
@return		ZW_ERR_xxx
@post       Caller must free the memory allocated to if_rec if return value is ZW_ERR_NONE
*/
static int zwdev_grp_if_parser(cJSON *json_if_rec, if_rec_t *if_rec_lst_hd, if_rec_t **if_rec)
{
    if_rec_t        *tmp_if;
    cJSON           *if_rec_ent;        //Device configuration interface record entry
    cJSON           *if_grp_ent;
    unsigned        if_rec_ent_msk;     //Bit mask to check all mandatory entries in if_rec_ent
    int             i;
    int             grp_cnt;
    if_rec_grp_t    grp_if_rec;

#define IF_ENTRY_MSK_GRP   0x01

#define IF_ENTRY_MSK_GRP_ALL_PRESENT   (IF_ENTRY_MSK_GRP)

    for (if_rec_ent_msk=0, if_rec_ent=json_if_rec->child;
        if_rec_ent != NULL; if_rec_ent=if_rec_ent->next)
    {
        if ((if_rec_ent->type == cJSON_String) && (!strcmp(if_rec_ent->string, "_comment")))
        {
            continue;
        }

        if (if_rec_ent->type == cJSON_Number)
        {
        }
        else if ((if_rec_ent->type == cJSON_Array) && (!strcmp(if_rec_ent->string, "grp_id")))
        {
            if (if_rec_ent_msk & IF_ENTRY_MSK_GRP)
            {   //Skip duplicate field
                continue;
            }
            if_rec_ent_msk |= IF_ENTRY_MSK_GRP;

            grp_cnt = cJSON_GetArraySize(if_rec_ent);

            if (grp_cnt > 0)
            {
                if (grp_cnt > IF_REC_MAX_GROUPS)
                {
                    grp_cnt = IF_REC_MAX_GROUPS;
                }

                if_grp_ent = if_rec_ent->child;
                for (i=0; i<grp_cnt; i++)
                {
                    if (if_grp_ent->type == cJSON_Number)
                    {
                        grp_if_rec.grp_id[i] = (uint8_t)if_grp_ent->valueint;
                    }
                    else
                    {
                        return ZW_ERR_VALUE;
                    }

                    if_grp_ent = if_grp_ent->next;
                }
                grp_if_rec.grp_cnt = grp_cnt;
            }
        }
    }

    //Check whether all the mandatory entries present
    if ((if_rec_ent_msk & IF_ENTRY_MSK_GRP_ALL_PRESENT) != IF_ENTRY_MSK_GRP_ALL_PRESENT)
    {
        return ZW_ERR_MISSING_ENTRY;
    }

    //Allocate memory for the interface
    tmp_if = (if_rec_t *)calloc(1, sizeof(if_rec_t));
    if (!tmp_if)
    {
        return ZW_ERR_MEMORY;
    }
    tmp_if->rec.grp = (if_rec_grp_t *)calloc(1, sizeof(if_rec_grp_t));
    if (!tmp_if->rec.grp)
    {
        free(tmp_if);
        return ZW_ERR_MEMORY;
    }

    tmp_if->type = IF_REC_TYPE_GROUP;
    memcpy(tmp_if->rec.grp, &grp_if_rec, sizeof(if_rec_grp_t));

    *if_rec = tmp_if;

    return ZW_ERR_NONE;

}


/**
zwdev_basic_match_ld - Load basic match record from JSON array
@param[in]		json_if_arry    Basic match record JSON array
@param[in,out]	ppbasic_rec      Address of the basic match recorder pointer
@return		ZW_ERR_xxx
@post       Caller must free the memory alloted if return value is ZW_ERR_NONE
*/
static int zwdev_basic_match_ld(cJSON *json_if_arry, if_rec_basic_match_t **ppbasic_rec)
{
    cJSON       *if_rec_json;       //Basic interface record
    cJSON       *if_rec_ent;        //Basic interface record entry
    if_rec_basic_match_t *pMatchRec = *ppbasic_rec;
    int         res;
    unsigned    if_rec_ent_msk;     //Bit mask to check all mandatory entries in if_rec_ent

#define IF_ENTRY_MSK_BASIC_COMMAND       0x01
#define IF_ENTRY_MSK_BASIC_VALUE         0x02
#define IF_ENTRY_MSK_BASIC_RESULT_TYPE   0x04

#define IF_ENTRY_MSK_BASIC_LVL_ALL_PRESENT   (IF_ENTRY_MSK_BASIC_COMMAND | IF_ENTRY_MSK_BASIC_VALUE | IF_ENTRY_MSK_BASIC_RESULT_TYPE)


    for (if_rec_json = json_if_arry->child; if_rec_json; if_rec_json = if_rec_json->next)
    {
        for (if_rec_ent_msk = 0, if_rec_ent = if_rec_json->child;
            if_rec_ent != NULL; if_rec_ent = if_rec_ent->next)
        {
            if ((if_rec_ent->type == cJSON_String) && (!strcmp(if_rec_ent->string, "_comment")))
            {
                continue;
            }

            if (if_rec_ent->type == cJSON_Number)
            {
                if (!strcmp(if_rec_ent->string, "command") && ((if_rec_ent_msk & IF_ENTRY_MSK_BASIC_COMMAND) == 0))
                {
                    if_rec_ent_msk |= IF_ENTRY_MSK_BASIC_COMMAND;
                    pMatchRec->command = (uint8_t)if_rec_ent->valueint;
                }
                //May implement range for value later
                ////If string is 'basic_level' and basic_level_low/high is not set in if_rec before
                ////'basic_level_range' will also set IF_ENTRY_MSK_BASIC_LVL. If  'basic_level_range' is used,
                ////ignore 'basic_level' if there is any.
                else if (!strcmp(if_rec_ent->string, "value") && ((if_rec_ent_msk & IF_ENTRY_MSK_BASIC_VALUE) == 0))
                {
                    if_rec_ent_msk |= IF_ENTRY_MSK_BASIC_VALUE;
                    pMatchRec->value = if_rec_ent->valueint;
                }
                else if (!strcmp(if_rec_ent->string, "result_if_type") && ((if_rec_ent_msk & IF_ENTRY_MSK_BASIC_RESULT_TYPE) == 0))
                {
                    if_rec_ent_msk |= IF_ENTRY_MSK_BASIC_RESULT_TYPE;
                    pMatchRec->resultType = (uint8_t)if_rec_ent->valueint;
                }
            }

        }
        //Check whether all the mandatory entries present
        if ((if_rec_ent_msk & IF_ENTRY_MSK_BASIC_LVL_ALL_PRESENT) != IF_ENTRY_MSK_BASIC_LVL_ALL_PRESENT)
        {
            res = ZW_ERR_MISSING_ENTRY;
            goto l_BASIC_LVL_LD_ERR1;
        }
    }

    return ZW_ERR_NONE;

l_BASIC_LVL_LD_ERR1:
    zwdev_basic_if_rec_free(pMatchRec);
    *ppbasic_rec = NULL;
    return res;
}


/**
zwdev_basic_if_ld - Load basic interface records from JSON array
@param[in]	json_if_arry        Basic interface JSON array
@param[in]	if_rec_lst_hd	    Interface record list head
@param[out]	if_rec				Interface record
@return		ZW_ERR_xxx
@post       Caller must call zwdev_basic_if_free() if return value is ZW_ERR_NONE
*/
static int zwdev_basic_if_ld(cJSON *json_if_arry, if_rec_t *if_rec_lst_hd, if_rec_t **if_rec)
{
    if_rec_t                *tmp_if = NULL;
    if_rec_basic_match_t    *curr_basic_rec = NULL;
    if_rec_basic_match_t    *basic_rec = NULL;
    cJSON                   *if_rec_json;       //Basic interface record
    cJSON                   *if_rec_ent;        //Basic interface record entry
    int                     res;
    int                     basic_rec_cnt;
    unsigned                if_rec_ent_msk;     //Bit mask to check all mandatory entries in if_rec_ent

#define IF_ENTRY_MSK_BASIC_MATCH_REC		0x01
#define IF_ENTRY_MSK_BASIC_RESULT_REC		0x02

    //Define the mandatory fields
#define IF_ENTRY_MSK_BASIC_REC_ALL_PRESENT   (IF_ENTRY_MSK_BASIC_MATCH_REC | IF_ENTRY_MSK_BASIC_RESULT_REC)

    curr_basic_rec = NULL;

    for (if_rec_json = json_if_arry->child;
        if_rec_json; if_rec_json = if_rec_json->next)
    {
        for (if_rec_ent_msk = 0, basic_rec = NULL, if_rec_ent = if_rec_json->child;
            if_rec_ent != NULL; if_rec_ent = if_rec_ent->next)
        {
            switch (if_rec_ent->type)
            {
                case cJSON_String:
                    if (!strcmp(if_rec_ent->string, "_comment"))
                        break;

                case cJSON_Array:
                    if (!strcmp(if_rec_ent->string, "match"))
                    {
                        if (if_rec_ent_msk & IF_ENTRY_MSK_BASIC_MATCH_REC)
                        {   //Skip duplicate field
                            break;
                        }

                        if_rec_ent_msk |= IF_ENTRY_MSK_BASIC_MATCH_REC;

                        basic_rec_cnt = cJSON_GetArraySize(if_rec_ent);

                        if (basic_rec_cnt != 1)  //expect only 1 match record
                        {
                            res = ZW_ERR_VALUE;
                            goto l_BASIC_IF_LD_ERR1;
                        }

                        if (!basic_rec)
                        {
                            zwdev_basic_if_rec_create(&basic_rec);
                            if (!basic_rec)
                            {
                                res = ZW_ERR_MEMORY;
                                goto l_BASIC_IF_LD_ERR1;
                            }
                        }

                        //Fill info for the match record
                        res = zwdev_basic_match_ld(if_rec_ent, &basic_rec);

                        if (res != ZW_ERR_NONE)
                        {
                            goto l_BASIC_IF_LD_ERR1;
                        }

                        //Append to basic list
                        if (curr_basic_rec)
                        {
                            curr_basic_rec->next = basic_rec;
                        }
                        else
                        {   //Copy to first record
                            //if_basic_first_rec = basic_rec;

                            //Allocate memory for the interface
                            if (tmp_if == NULL)
                            {
                                //First match record, create the interface record
                                tmp_if = (if_rec_t *)calloc(1, sizeof(if_rec_t));
                                if (!tmp_if)
                                {
                                    res = ZW_ERR_MEMORY;
                                    goto l_BASIC_IF_LD_ERR1;
                                }
                                tmp_if->type = IF_REC_TYPE_BASIC;
                            }

                            tmp_if->rec.basic = basic_rec;
                            *if_rec = tmp_if;
                        }
                        curr_basic_rec = basic_rec;
                    }
                    else if (!strcmp(if_rec_ent->string, "result"))
                    {
                        if (if_rec_ent_msk & IF_ENTRY_MSK_BASIC_RESULT_REC)
                        {   //Skip duplicate field
                            break;
                        }

                        if_rec_ent_msk |= IF_ENTRY_MSK_BASIC_RESULT_REC;

                        if ((if_rec_ent_msk & IF_ENTRY_MSK_BASIC_MATCH_REC) == 0)
                        {   //Error, match block should come before result block
                            res = ZW_ERR_ORDER;
                            goto l_BASIC_IF_LD_ERR1;
                        }

                        basic_rec_cnt = cJSON_GetArraySize(if_rec_ent);

                        if (basic_rec_cnt != 1)  //expect only 1 result record
                        {
                            res = ZW_ERR_VALUE;
                            goto l_BASIC_IF_LD_ERR1;
                        }

                        //Fill info for the result record
                        res = zwdev_gen_result_ld(if_rec_ent, if_rec_lst_hd, basic_rec->resultType, &basic_rec->pResult, *if_rec);

                        if (res != ZW_ERR_NONE)
                        {
                            goto l_BASIC_IF_LD_ERR1;
                        }

                    }
                    break;

            }

        }

        //Check whether all the mandatory entries present
        if ((if_rec_ent_msk & IF_ENTRY_MSK_BASIC_REC_ALL_PRESENT) != IF_ENTRY_MSK_BASIC_REC_ALL_PRESENT)
        {
            res = ZW_ERR_MISSING_ENTRY;
            goto l_BASIC_IF_LD_ERR1;
        }
    }

    return ZW_ERR_NONE;

l_BASIC_IF_LD_ERR1:
    if (tmp_if)
    {
        zwdev_cfg_if_free(tmp_if);
        *if_rec = NULL;
    }
    if (curr_basic_rec != basic_rec)
    {
        zwdev_basic_if_rec_free(basic_rec);
    }
    return res;
}

/**
zwdev_basic_if_parser - Basic interface parser of JSON object
@param[in]	json_if_rec	    JSON interface record
@param[in]	if_rec_lst_hd	Interface record list head
@param[out]	if_rec		    Interface record
@return		ZW_ERR_xxx
@post       Caller must free the memory allocated to if_rec if return value is ZW_ERR_NONE
*/
static int zwdev_basic_if_parser(cJSON *json_if_rec, if_rec_t *if_rec_lst_hd, if_rec_t **if_rec)
{
    cJSON           *if_rec_ent;        //Device basic interface record entry
    unsigned        if_rec_ent_msk;     //Bit mask to check all mandatory entries in if_rec_ent
    int             basic_cnt;
    int             res;

#define IF_ENTRY_MSK_BASIC   0x01

#define IF_ENTRY_MSK_BASIC_ALL_PRESENT   (IF_ENTRY_MSK_BASIC)

    for (if_rec_ent_msk = 0, if_rec_ent = json_if_rec->child;
        if_rec_ent != NULL; if_rec_ent = if_rec_ent->next)
    {
        if ((if_rec_ent->type == cJSON_String) && (!strcmp(if_rec_ent->string, "_comment")))
        {
            continue;
        }

        if (if_rec_ent->type == cJSON_Number)
        {
        }
        else if ((if_rec_ent->type == cJSON_Array) && (!strcmp(if_rec_ent->string, "basic")))
        {
            if (if_rec_ent_msk & IF_ENTRY_MSK_BASIC)
            {   //Skip duplicate field
                continue;
            }
            if_rec_ent_msk |= IF_ENTRY_MSK_BASIC;

            basic_cnt = cJSON_GetArraySize(if_rec_ent);

            if (basic_cnt > 0)
            {
                res = zwdev_basic_if_ld(if_rec_ent, if_rec_lst_hd, if_rec);

                if (res != ZW_ERR_NONE)
                {
                    return res;
                }
            }
        }
    }

    //Check whether all the mandatory entries present
    if ((if_rec_ent_msk & IF_ENTRY_MSK_BASIC_ALL_PRESENT) != IF_ENTRY_MSK_BASIC_ALL_PRESENT)
    {
        res = ZW_ERR_MISSING_ENTRY;
        goto l_BASIC_IF_PARSER_ERR1;
    }

    return ZW_ERR_NONE;

l_BASIC_IF_PARSER_ERR1:

    return res;
}

/**
zwdev_alrm_snsr_match_ld - Load alrm_snsr match record from JSON array
@param[in]		json_if_arry    Alarm sensor match record JSON array
@param[in,out]	ppalrm_snsr_rec      Address of the alrm_snsr match recorder pointer
@return		ZW_ERR_xxx
@post       Caller must free the memory alloted if return value is ZW_ERR_NONE
*/
static int zwdev_alrm_snsr_match_ld(cJSON *json_if_arry, if_rec_alrm_snsr_match_t **ppalrm_snsr_rec)
{
    cJSON       *if_rec_json;       //Alarm sensor interface record
    cJSON       *if_rec_ent;        //Alarm sensor interface record entry
    cJSON       *if_sta_range_ent;  //Alarm sensor interface record entry
    if_rec_alrm_snsr_match_t *pMatchRec = *ppalrm_snsr_rec;
    int         j;
    int         res;
    int         alrm_range_count;
    unsigned    if_rec_ent_msk;     //Bit mask to check all mandatory entries in if_rec_ent

#define IF_ENTRY_MSK_ALRM_SNSR_TYPE			0x01
#define IF_ENTRY_MSK_ALRM_SNSR_STATE        0x02
#define IF_ENTRY_MSK_ALRM_SNSR_RESULT_TYPE  0x04

#define IF_ENTRY_MSK_ALRM_SNSR_LVL_ALL_PRESENT   (IF_ENTRY_MSK_ALRM_SNSR_TYPE | IF_ENTRY_MSK_ALRM_SNSR_STATE | IF_ENTRY_MSK_ALRM_SNSR_RESULT_TYPE)


    for (if_rec_json = json_if_arry->child; if_rec_json; if_rec_json = if_rec_json->next)
    {
        for (if_rec_ent_msk = 0, if_rec_ent = if_rec_json->child;
            if_rec_ent != NULL; if_rec_ent = if_rec_ent->next)
        {
            if ((if_rec_ent->type == cJSON_String) && (!strcmp(if_rec_ent->string, "_comment")))
            {
                continue;
            }

            if (if_rec_ent->type == cJSON_Number)
            {
                if (!strcmp(if_rec_ent->string, "type") && ((if_rec_ent_msk & IF_ENTRY_MSK_ALRM_SNSR_TYPE) == 0))
                {
                    if_rec_ent_msk |= IF_ENTRY_MSK_ALRM_SNSR_TYPE;
                    pMatchRec->type = (uint8_t)if_rec_ent->valueint;
                }
                //If string is 'state' and state_low/high is not set in if_rec before
                //'state_range' will also set IF_ENTRY_MSK_ALRM_SNSR_STATE.
                else if (!strcmp(if_rec_ent->string, "state") && ((if_rec_ent_msk & IF_ENTRY_MSK_ALRM_SNSR_STATE) == 0))
                {
                    if_rec_ent_msk |= IF_ENTRY_MSK_ALRM_SNSR_STATE;
                    pMatchRec->isRange = 0;
                    pMatchRec->state_low = if_rec_ent->valueint;
                }
                else if (!strcmp(if_rec_ent->string, "result_if_type") && ((if_rec_ent_msk & IF_ENTRY_MSK_ALRM_SNSR_RESULT_TYPE) == 0))
                {
                    if_rec_ent_msk |= IF_ENTRY_MSK_ALRM_SNSR_RESULT_TYPE;
                    pMatchRec->resultType = (uint8_t)if_rec_ent->valueint;
                }
            }
            else if (if_rec_ent->type == cJSON_Array)
            {
                if (!strcmp(if_rec_ent->string, "state_range") && ((if_rec_ent_msk & IF_ENTRY_MSK_ALRM_SNSR_STATE) == 0))
                {
                    if_rec_ent_msk |= IF_ENTRY_MSK_ALRM_SNSR_STATE;

                    alrm_range_count = cJSON_GetArraySize(if_rec_ent);

                    if (alrm_range_count == 2) //Min and max value
                    {
                        pMatchRec->isRange = 1;
                        if_sta_range_ent = if_rec_ent->child;
                        for (j = 0; j < alrm_range_count; j++)
                        {
                            if (if_sta_range_ent->type == cJSON_Number)
                            {
                                if (j == 0)
                                    pMatchRec->state_low = (uint8_t)if_sta_range_ent->valueint;
                                else
                                    pMatchRec->state_high = (uint8_t)if_sta_range_ent->valueint;
                            }
                            else
                            {
                                res = ZW_ERR_VALUE;
                                goto l_ALRM_SNSR_LVL_LD_ERR1;
                            }

                            if_sta_range_ent = if_sta_range_ent->next;
                        }
                    }
                    else
                    {
                        res = ZW_ERR_VALUE;
                        goto l_ALRM_SNSR_LVL_LD_ERR1;
                    }
                }
            }
        }
        //Check whether all the mandatory entries present
        if ((if_rec_ent_msk & IF_ENTRY_MSK_ALRM_SNSR_LVL_ALL_PRESENT) != IF_ENTRY_MSK_ALRM_SNSR_LVL_ALL_PRESENT)
        {
            res = ZW_ERR_MISSING_ENTRY;
            goto l_ALRM_SNSR_LVL_LD_ERR1;
        }
    }

    return ZW_ERR_NONE;

l_ALRM_SNSR_LVL_LD_ERR1:
    zwdev_alrm_snsr_if_rec_free(pMatchRec);
    *ppalrm_snsr_rec = NULL;
    return res;
}


/**
zwdev_alrm_snsr_if_ld - Load alrm_snsr interface records from JSON array
@param[in]	json_if_arry        Alarm sensor interface JSON array
@param[in]	if_rec_lst_hd	    Interface record list head
@param[out]	if_rec				Interface record
@return		ZW_ERR_xxx
@post       Caller must call zwdev_alrm_snsr_if_free() if return value is ZW_ERR_NONE
*/
static int zwdev_alrm_snsr_if_ld(cJSON *json_if_arry, if_rec_t *if_rec_lst_hd, if_rec_t **if_rec)
{
    if_rec_t                *tmp_if = NULL;
    if_rec_alrm_snsr_match_t    *curr_alrm_snsr_rec = NULL;
    if_rec_alrm_snsr_match_t    *alrm_snsr_rec = NULL;
    cJSON                   *if_rec_json;       //Alarm sensor interface record
    cJSON                   *if_rec_ent;        //Alarm sensor interface record entry
    int                     res;
    int                     alrm_snsr_rec_cnt;
    unsigned                if_rec_ent_msk;     //Bit mask to check all mandatory entries in if_rec_ent

#define IF_ENTRY_MSK_ALRM_SNSR_MATCH_REC		0x01
#define IF_ENTRY_MSK_ALRM_SNSR_RESULT_REC		0x02

    //Define the mandatory fields
#define IF_ENTRY_MSK_ALRM_SNSR_REC_ALL_PRESENT   (IF_ENTRY_MSK_ALRM_SNSR_MATCH_REC | IF_ENTRY_MSK_ALRM_SNSR_RESULT_REC)

    for (if_rec_json = json_if_arry->child;
        if_rec_json; if_rec_json = if_rec_json->next)
    {
        for (if_rec_ent_msk = 0, alrm_snsr_rec = NULL, if_rec_ent = if_rec_json->child;
            if_rec_ent != NULL; if_rec_ent = if_rec_ent->next)
        {
            switch (if_rec_ent->type)
            {
                case cJSON_String:
                    if (!strcmp(if_rec_ent->string, "_comment"))
                        break;

                case cJSON_Array:
                    if (!strcmp(if_rec_ent->string, "match"))
                    {
                        if (if_rec_ent_msk & IF_ENTRY_MSK_ALRM_SNSR_MATCH_REC)
                        {   //Skip duplicate field
                            break;
                        }

                        if_rec_ent_msk |= IF_ENTRY_MSK_ALRM_SNSR_MATCH_REC;

                        alrm_snsr_rec_cnt = cJSON_GetArraySize(if_rec_ent);

                        if (alrm_snsr_rec_cnt != 1)  //expect only 1 match record
                        {
                            res = ZW_ERR_VALUE;
                            goto l_ALRM_SNSR_IF_LD_ERR1;
                        }

                        if (!alrm_snsr_rec)
                        {
                            zwdev_alrm_snsr_if_rec_create(&alrm_snsr_rec);
                            if (!alrm_snsr_rec)
                            {
                                res = ZW_ERR_MEMORY;
                                goto l_ALRM_SNSR_IF_LD_ERR1;
                            }
                        }

                        //Fill info for the match record
                        res = zwdev_alrm_snsr_match_ld(if_rec_ent, &alrm_snsr_rec);

                        if (res != ZW_ERR_NONE)
                        {
                            goto l_ALRM_SNSR_IF_LD_ERR1;
                        }

                        //Append to alrm_snsr list
                        if (curr_alrm_snsr_rec)
                        {
                            curr_alrm_snsr_rec->next = alrm_snsr_rec;
                        }
                        else
                        {   //Copy to first record

                            //Allocate memory for the interface
                            if (tmp_if == NULL)
                            {
                                //First match record, create the interface record
                                tmp_if = (if_rec_t *)calloc(1, sizeof(if_rec_t));
                                if (!tmp_if)
                                {
                                    res = ZW_ERR_MEMORY;
                                    goto l_ALRM_SNSR_IF_LD_ERR1;
                                }
                                tmp_if->type = IF_REC_TYPE_ALRM_SNSR;
                            }

                            tmp_if->rec.alrm_snsr = alrm_snsr_rec;
                            *if_rec = tmp_if;
                        }
                        curr_alrm_snsr_rec = alrm_snsr_rec;
                    }
                    else if (!strcmp(if_rec_ent->string, "result"))
                    {
                        if (if_rec_ent_msk & IF_ENTRY_MSK_ALRM_SNSR_RESULT_REC)
                        {   //Skip duplicate field
                            break;
                        }

                        if_rec_ent_msk |= IF_ENTRY_MSK_ALRM_SNSR_RESULT_REC;

                        if ((if_rec_ent_msk & IF_ENTRY_MSK_ALRM_SNSR_MATCH_REC) == 0)
                        {   //Error, match block should come before result block
                            res = ZW_ERR_ORDER;
                            goto l_ALRM_SNSR_IF_LD_ERR1;
                        }

                        alrm_snsr_rec_cnt = cJSON_GetArraySize(if_rec_ent);

                        if (alrm_snsr_rec_cnt != 1)  //expect only 1 result record
                        {
                            res = ZW_ERR_VALUE;
                            goto l_ALRM_SNSR_IF_LD_ERR1;
                        }

                        //Fill info for the result record
                        res = zwdev_gen_result_ld(if_rec_ent, if_rec_lst_hd, alrm_snsr_rec->resultType, &alrm_snsr_rec->pResult, *if_rec);

                        if (res != ZW_ERR_NONE)
                        {
                            goto l_ALRM_SNSR_IF_LD_ERR1;
                        }

                    }
                    break;
            }

        }
        //Check whether all the mandatory entries present
        if ((if_rec_ent_msk & IF_ENTRY_MSK_ALRM_SNSR_REC_ALL_PRESENT) != IF_ENTRY_MSK_ALRM_SNSR_REC_ALL_PRESENT)
        {
            res = ZW_ERR_MISSING_ENTRY;
            goto l_ALRM_SNSR_IF_LD_ERR1;
        }

    }

    return ZW_ERR_NONE;

l_ALRM_SNSR_IF_LD_ERR1 :
    if (tmp_if)
    {
        zwdev_cfg_if_free(tmp_if);
        *if_rec = NULL;
    }
    if (curr_alrm_snsr_rec != alrm_snsr_rec)
    {
        zwdev_alrm_snsr_if_rec_free(alrm_snsr_rec);
    }
    return res;
}

/**
zwdev_alrm_snsr_if_parser - Alarm sensor interface parser of JSON object
@param[in]	json_if_rec	    JSON interface record
@param[in]	if_rec_lst_hd	Interface record list head
@param[out]	if_rec		    Interface record
@return		ZW_ERR_xxx
@post       Caller must free the memory allocated to if_rec if return value is ZW_ERR_NONE
*/
static int zwdev_alrm_snsr_if_parser(cJSON *json_if_rec, if_rec_t *if_rec_lst_hd, if_rec_t **if_rec)
{
    cJSON           *if_rec_ent;        //Device alrm_snsr interface record entry
    if_rec_t        *tmp_if = NULL;
    unsigned        if_rec_ent_msk;     //Bit mask to check all mandatory entries in if_rec_ent
    int             alrm_snsr_cnt;
    int             res;
    int             no_evt_clr = 0;
    uint16_t        tmout = DFLT_EVT_CLR_TIMEOUT;        //Timeout default value

#define IF_ENTRY_MSK_ALRM_SNSR              0x01
#define IF_ENTRY_MSK_ALRM_SNSR_EVT_CLR      0x02

//#define IF_ENTRY_MSK_ALRM_SNSR_ALL_PRESENT   (IF_ENTRY_MSK_ALRM_SNSR)

    for (if_rec_ent_msk = 0, if_rec_ent = json_if_rec->child;
        if_rec_ent != NULL; if_rec_ent = if_rec_ent->next)
    {
        if ((if_rec_ent->type == cJSON_String) && (!strcmp(if_rec_ent->string, "_comment")))
        {
            continue;
        }

        if (if_rec_ent->type == cJSON_Number)
        {
            if (!strcmp(if_rec_ent->string, "no_event_clear"))
            {
                if_rec_ent_msk |= IF_ENTRY_MSK_ALRM_SNSR_EVT_CLR;
                no_evt_clr = if_rec_ent->valueint;
            }
            else if (!strcmp(if_rec_ent->string, "event_clear_timeout"))
            {
                tmout = (uint16_t)if_rec_ent->valueint;
            }
        }
        else if ((if_rec_ent->type == cJSON_Array) && (!strcmp(if_rec_ent->string, "alrm_snsr")))
        {
            if (if_rec_ent_msk & IF_ENTRY_MSK_ALRM_SNSR)
            {   //Skip duplicate field
                continue;
            }
            if_rec_ent_msk |= IF_ENTRY_MSK_ALRM_SNSR;

            alrm_snsr_cnt = cJSON_GetArraySize(if_rec_ent);

            if (alrm_snsr_cnt > 0)
            {
                res = zwdev_alrm_snsr_if_ld(if_rec_ent, if_rec_lst_hd, &tmp_if);

                if (res != ZW_ERR_NONE)
                {
                    return res;
                }
            }
        }
    }

    //Check whether all the mandatory entries present
    if (!if_rec_ent_msk)
    {
        return ZW_ERR_MISSING_ENTRY;
    }

    if (!tmp_if)
    {   //No alrm_snsr block, allocate memory for the interface record
        tmp_if = (if_rec_t *)calloc(1, sizeof(if_rec_t));
        if (tmp_if)
        {
            zwdev_alrm_snsr_if_rec_create(&tmp_if->rec.alrm_snsr);

            if (!tmp_if->rec.alrm_snsr)
            {
                free(tmp_if);
                return ZW_ERR_MEMORY;
            }
        }
        else
        {
            return ZW_ERR_MEMORY;
        }
    }

    tmp_if->type = IF_REC_TYPE_ALRM_SNSR;
    tmp_if->propty = (no_evt_clr == 0)? IF_PROPTY_ALRM_SNSR_EVT_CLR : 0;
    if (no_evt_clr)
    {
        tmp_if->tmout = (tmout > MAX_EVT_CLR_TIMEOUT)? MAX_EVT_CLR_TIMEOUT : tmout;
    }

    *if_rec = tmp_if;

    return ZW_ERR_NONE;
}


/**
zwdev_cfg_if_rec_append - Append interface record to the end of linked-list
@param[in]	if_rec_lst_hd	Interface record linked-list
@param[in]	if_rec_ent	    Interface record entry
@return
*/
static void zwdev_cfg_if_rec_append(if_rec_t **if_rec_lst_hd, if_rec_t *if_rec_ent)
{
    if (*if_rec_lst_hd == NULL)
    {   //List is empty
        *if_rec_lst_hd = if_rec_ent;
    }
    else
    {   //Append at the end of linked-list
        if_rec_t *last_if_rec;

        last_if_rec = *if_rec_lst_hd;

        while (last_if_rec)
        {
            if (!last_if_rec->next)
            {
                last_if_rec->next = if_rec_ent;
                break;
            }

            last_if_rec = last_if_rec->next;
        }
    }
}


/**
zwdev_cfg_if_ld - Load device specific configuration interface records from JSON array
@param[in]	json_if_arry    Device specific configuration interface JSON array
@param[out]	if_rec_lst_hd	Device specific configuration interface list head
@param[out]	err_loc	        Error location in the cfg_file if there is an error while parsing it
@return		ZW_ERR_xxx
@post       Caller must call zwdev_cfg_if_free() if return value is ZW_ERR_NONE
*/
static int zwdev_cfg_if_ld(cJSON *json_if_arry, if_rec_t **if_rec_lst_hd, dev_cfg_error_t *err_loc)
{
    static const zwdev_if_parser_fn if_parser[] =
    {   NULL,   //Order must follow the definition of IF_REC_TYPE_XXX
        zwdev_grp_if_parser,
        zwdev_config_prm_if_parser,
        zwdev_bin_snsr_if_parser,
        zwdev_snsr_if_parser,
        zwdev_meter_if_parser,
        zwdev_alrm_if_parser,
        zwdev_basic_if_parser,
        zwdev_alrm_snsr_if_parser,
        zwdev_thrmo_setp_if_parser
    };
    zwdev_if_parser_fn zwdev_if_parse;
    if_rec_t    *tmp_if;
    cJSON       *if_rec_json;       //A device configuration interface record
    cJSON       *if_rec_ent;        //Device configuration interface record entry
    int         res;
    int         if_type;
    int         if_ver;
    int         hidden;

    *if_rec_lst_hd = NULL;

    for (if_rec_json=json_if_arry->child;
        if_rec_json; if_rec_json=if_rec_json->next)
    {
        //Update error location
        err_loc->if_ent++;

        //Find the interface type
        if_rec_ent = cJSON_GetObjectItem(if_rec_json,"if_type");

        if (!if_rec_ent)
        {
            res = ZW_ERR_MISSING_ENTRY;
            goto l_DEV_CFG_IF_LD_ERR;
        }

        if (if_rec_ent->type != cJSON_Number)
        {
            res = ZW_ERR_VALUE;
            goto l_DEV_CFG_IF_LD_ERR;
        }

        if_type = if_rec_ent->valueint;

        //Find the optional user-defined version
        if_rec_ent = cJSON_GetObjectItem(if_rec_json,"user_def_version");

        if_ver = (if_rec_ent && (if_rec_ent->type == cJSON_Number))? if_rec_ent->valueint : 0;

        //Find the optional 'hidden' property
        if_rec_ent = cJSON_GetObjectItem(if_rec_json,"hidden");

        hidden = (if_rec_ent && (if_rec_ent->type == cJSON_Number))? if_rec_ent->valueint : 0;

        if ((if_type > 0) && (if_type <= IF_REC_TYPE_MAX))
        {
            zwdev_if_parse = if_parser[if_type];
            res = zwdev_if_parse(if_rec_json, *if_rec_lst_hd, &tmp_if);

            if (res == ZW_ERR_NONE)
            {
                //Assign user-defined version
                tmp_if->usr_def_ver = if_ver;

                //Assign 'hidden' property
                if (hidden)
                {
                    tmp_if->propty |= IF_PROPTY_HIDDEN_POLL;
                }

                //Append to device specific configuration interface list
                zwdev_cfg_if_rec_append(if_rec_lst_hd, tmp_if);
            }
            else
            {
                goto l_DEV_CFG_IF_LD_ERR;
            }
        }
        else
        {
            res = ZW_ERR_WRONG_IF;
            goto l_DEV_CFG_IF_LD_ERR;
        }
    }

    return ZW_ERR_NONE;

l_DEV_CFG_IF_LD_ERR:
    if (*if_rec_lst_hd)
    {
        zwdev_cfg_if_free(*if_rec_lst_hd);
        *if_rec_lst_hd = NULL;
    }
    return res;
}

/**
zwdev_cfg_redir_ld - Load device specific command redirection records from JSON array
@param[in]	json_redir_arry    Device specific command redirection JSON array
@param[out]	redir_rec_lst_hd	Device specific command redirection list head
@param[out]	err_loc	        Error location in the cfg_file if there is an error while parsing it
@return		ZW_ERR_xxx
@post       Caller must call zwdev_cfg_redir_free() if return value is ZW_ERR_NONE
*/
static int zwdev_cfg_redir_ld(cJSON *json_redir_arry, redir_rec_t **redir_rec_lst_hd, dev_cfg_error_t *err_loc)
{
    redir_rec_t    *curr_redir;
    redir_rec_t    *tmp_redir;
    cJSON       *redir_rec_json;       //A device configuration interface record
    cJSON       *redir_rec_ent;        //Device configuration interface record entry
    uint16_t    if_id;
    int         cmd;
    uint8_t     target_ep;
    unsigned    redir_rec_ent_msk;     //Bit mask to check all mandatory entries in redir_rec_ent

#define IF_ENTRY_MSK_REDIR_IFID_REC			0x01
#define IF_ENTRY_MSK_REDIR_IFCMD_REC		0x02
#define IF_ENTRY_MSK_REDIR_TARGET_EP_REC	0x04

    //Define the mandatory fields
#define IF_ENTRY_MSK_REDIR_REC_ALL_PRESENT   (IF_ENTRY_MSK_REDIR_IFID_REC | IF_ENTRY_MSK_REDIR_TARGET_EP_REC)

    curr_redir = NULL;
    *redir_rec_lst_hd = NULL;

    for (redir_rec_json = json_redir_arry->child;
        redir_rec_json; redir_rec_json = redir_rec_json->next)
    {
        if_id = 0;
        cmd = -1;
        target_ep = 0;

        for (redir_rec_ent_msk = 0, redir_rec_ent = redir_rec_json->child;
            redir_rec_ent != NULL; redir_rec_ent = redir_rec_ent->next)
        {
            switch (redir_rec_ent->type)
            {
                case cJSON_String:
                    {
                        if (!strcmp(redir_rec_ent->string, "_comment"))
                            break;
                    }
                    break;

                case cJSON_Number:
                    {
                        if (!strcmp(redir_rec_ent->string, "interface") && ((redir_rec_ent_msk & IF_ENTRY_MSK_REDIR_IFID_REC) == 0))
                        {
                            redir_rec_ent_msk |= IF_ENTRY_MSK_REDIR_IFID_REC;
                            if_id = (uint16_t)redir_rec_ent->valueint;
                        }
                        //May implement range for command later
                        else if (!strcmp(redir_rec_ent->string, "command") && ((redir_rec_ent_msk & IF_ENTRY_MSK_REDIR_IFCMD_REC) == 0))
                        {
                            redir_rec_ent_msk |= IF_ENTRY_MSK_REDIR_IFCMD_REC;
                            cmd = (int)redir_rec_ent->valueint;
                        }
                        else if (!strcmp(redir_rec_ent->string, "target_ep") && ((redir_rec_ent_msk & IF_ENTRY_MSK_REDIR_TARGET_EP_REC) == 0))
                        {
                            redir_rec_ent_msk |= IF_ENTRY_MSK_REDIR_TARGET_EP_REC;
                            target_ep = (uint8_t)redir_rec_ent->valueint;
                        }
                    }
                    break;
            }
        }

        //Check whether all the mandatory entries present
        if ((redir_rec_ent_msk & IF_ENTRY_MSK_REDIR_REC_ALL_PRESENT) != IF_ENTRY_MSK_REDIR_REC_ALL_PRESENT)
        {
            return ZW_ERR_MISSING_ENTRY;
        }

        //Allocate memory for the command redirection record
        tmp_redir = (redir_rec_t *)calloc(1, sizeof(redir_rec_t));
        if (!tmp_redir)
        {
            return ZW_ERR_MEMORY;
        }
        *tmp_redir = redir_rec_init;

        tmp_redir->if_id = if_id;
        tmp_redir->cmd = cmd;
        tmp_redir->target_ep = target_ep;

        if (*redir_rec_lst_hd == NULL)
        {
            *redir_rec_lst_hd = tmp_redir;
        }
        else
        {
            curr_redir->next = tmp_redir;
        }
        curr_redir = tmp_redir;

    }

    return ZW_ERR_NONE;
}

/**
zwdev_cfg_records_free - Free device specific configuration records
@param[in]	records	    Device specific configuration records
@param[in]	record_cnt	Number of records stored in "records" array
@return
*/
static void zwdev_cfg_records_free(dev_rec_t *records, int record_cnt)
{
    int i;

    for (i=0; i<record_cnt; i++)
    {
        if (records[i].ep_rec)
        {
            zwdev_cfg_ep_free(records[i].ep_rec);
            records[i].ep_rec = NULL;
        }
    }
}


/**
zwdev_cfg_free - Free device specific configuration records
@param[in]	records	    Device specific configuration records
@param[in]	record_cnt	Number of records stored in "records" array
@return
*/
void zwdev_cfg_free(dev_rec_t *records, int record_cnt)
{
    if (!records)
        return;

    zwdev_cfg_records_free(records, record_cnt);
    free(records);
}


/**
zwdev_cfg_ep_ld - Load device specific configuration endpoint records from JSON array
@param[in]	json_ep_rec	    Device specific configuration endpoint JSON array
@param[out]	ep_rec_lst_hd	Device specific configuration endpoint list head
@param[out]	err_loc	        Error location in the cfg_file if there is an error while parsing it
@return		ZW_ERR_xxx
@post       Caller must call zwdev_cfg_ep_free() if return value is ZW_ERR_NONE
*/
static int zwdev_cfg_ep_ld(cJSON *json_ep_rec, ep_rec_t **ep_rec_lst_hd, dev_cfg_error_t *err_loc)
{
    ep_rec_t    *curr_ep;
    ep_rec_t    *tmp_ep;
    if_rec_t    *if_rec;
    redir_rec_t *redir_rec;
    cJSON       *ep_rec;            //A device configuration endpoint record
    cJSON       *ep_rec_ent;        //Device configuration endpoint record entry
    unsigned    ep_rec_ent_msk;     //Bit mask to check all mandatory entries in ep_rec_ent
    int         intf_cnt;
    int         redir_cnt;
    int         res;
    uint8_t     epid = 0;
    uint8_t     new_intf_ver = 0;
    uint16_t    new_intf = 0;

#define EP_ENTRY_MSK_EPID               0x01
#define EP_ENTRY_MSK_INTF               0x02
#define EP_ENTRY_MSK_CREATE_INTF		0x04
#define EP_ENTRY_MSK_CREATE_INTF_VER	0x08
#define EP_ENTRY_MSK_REDIR	            0x10

#define EP_ENTRY_MSK_ALL_PRESENT   (EP_ENTRY_MSK_EPID)

    if_rec = NULL;
    redir_rec = NULL;
    curr_ep = NULL;
    *ep_rec_lst_hd = NULL;

    for (ep_rec=json_ep_rec->child;
        ep_rec; ep_rec=ep_rec->next)
    {
        //Update error location
        err_loc->ep_ent++;
        err_loc->if_ent = 0;

        epid = 0;
        new_intf_ver = 0;
        new_intf = 0;
        if_rec = NULL;
        redir_rec = NULL;

        for (ep_rec_ent_msk = 0, ep_rec_ent = ep_rec->child;
            ep_rec_ent != NULL; ep_rec_ent=ep_rec_ent->next)
        {
            if ((ep_rec_ent->type == cJSON_String) && (!strcmp(ep_rec_ent->string, "_comment")))
            {
                continue;
            }

            if (ep_rec_ent->type == cJSON_Number)
            {
                if (!strcmp(ep_rec_ent->string, "epid"))
                {
                    ep_rec_ent_msk |= EP_ENTRY_MSK_EPID;
                    epid = (uint8_t)ep_rec_ent->valueint;
                }
                else if (!strcmp(ep_rec_ent->string, "create_interface"))
                {
                    ep_rec_ent_msk |= EP_ENTRY_MSK_CREATE_INTF;
                    new_intf = (uint16_t)ep_rec_ent->valueint;
                }
                else if (!strcmp(ep_rec_ent->string, "create_interface_version"))
                {
                    ep_rec_ent_msk |= EP_ENTRY_MSK_CREATE_INTF_VER;
                    new_intf_ver = (uint8_t)ep_rec_ent->valueint;
                }
            }
            else if (ep_rec_ent->type == cJSON_Array)
            {
                if (!strcmp(ep_rec_ent->string, "interface"))
                {
                    if (ep_rec_ent_msk & EP_ENTRY_MSK_INTF)
                    {   //Skip duplicate field
                        continue;
                    }
                    ep_rec_ent_msk |= EP_ENTRY_MSK_INTF;
                    intf_cnt = cJSON_GetArraySize(ep_rec_ent);
                    if (intf_cnt > 0)
                    {
                        //Load each interface into the buffer
                        res = zwdev_cfg_if_ld(ep_rec_ent, &if_rec, err_loc);
                        if (res != ZW_ERR_NONE)
                        {
                            goto l_DEV_CFG_EP_LD_ERR;
                        }
                    }
                }
                else if (!strcmp(ep_rec_ent->string, "redirect"))
                {
                    if (ep_rec_ent_msk & EP_ENTRY_MSK_REDIR)
                    {   //Skip duplicate field
                        continue;
                    }
                    ep_rec_ent_msk |= EP_ENTRY_MSK_REDIR;
                    redir_cnt = cJSON_GetArraySize(ep_rec_ent);
                    if (redir_cnt > 0)
                    {
                        //Load each interface redirection into buffer
                        res = zwdev_cfg_redir_ld(ep_rec_ent, &redir_rec, err_loc);
                        if (res != ZW_ERR_NONE)
                        {
                            goto l_DEV_CFG_EP_LD_ERR;
                        }
                    }
                }
            }
        }
        //Check whether all the mandatory entries present
        if ((ep_rec_ent_msk & EP_ENTRY_MSK_ALL_PRESENT) != EP_ENTRY_MSK_ALL_PRESENT)
        {
            res = ZW_ERR_MISSING_ENTRY;
            //Update error location
            err_loc->if_ent = 0;
            goto l_DEV_CFG_EP_LD_ERR;
        }

        //Allocate memory for the endpoint
        tmp_ep = (ep_rec_t *)calloc(1, sizeof(ep_rec_t));
        if (!tmp_ep)
        {
            res = ZW_ERR_MEMORY;
            //Update error location
            err_loc->if_ent = 0;
            goto l_DEV_CFG_EP_LD_ERR;
        }

        //Append to device specific configuration endpoint list
        if (curr_ep)
        {
            curr_ep->next = tmp_ep;
        }
        else
        {   //Assign to list head
            *ep_rec_lst_hd = tmp_ep;
        }
        curr_ep = tmp_ep;
        curr_ep->id = epid;
        curr_ep->new_if = new_intf;
        curr_ep->new_if_ver = new_intf_ver;
        curr_ep->new_if_propty = IF_PROPTY_ALRM_EVT_CLR;
        curr_ep->redir_rec = redir_rec;
        curr_ep->if_rec = if_rec;

    }

    return ZW_ERR_NONE;

l_DEV_CFG_EP_LD_ERR:
    if (if_rec)
    {
        zwdev_cfg_if_free(if_rec);
    }
    if (redir_rec)
    {
        zwdev_cfg_redir_free(redir_rec);
    }
    if (*ep_rec_lst_hd)
    {
        zwdev_cfg_ep_free(*ep_rec_lst_hd);
        *ep_rec_lst_hd = NULL;
    }
    return res;
}


/**
zwdev_cfg_records_ld - Load device specific configurations record from JSON array
@param[in]	json_dev_rec	Device specific configurations JSON array
@param[in]	rec_cnt	        Expected record counts to be stored in dev_cfg_records
@param[out]	dev_cfg_records	Device specific configuration records sorted according to manufacturer id,
                            product type id and product id
@param[out]	err_loc	        Error location in the cfg_file if there is an error while parsing it
@return		ZW_ERR_xxx
@post       Caller must call zwdev_cfg_records_free() if return value is ZW_ERR_NONE
*/
static int zwdev_cfg_records_ld(cJSON *json_dev_rec, dev_rec_t *dev_cfg_records, int rec_cnt, dev_cfg_error_t *err_loc)
{
    cJSON   *a_dev_rec;             //A device configuration record
    cJSON   *dev_rec_ent;           //Device configuration record entry
    unsigned    dev_rec_ent_msk;    //Bit mask to check all mandatory entries in dev_rec_ent
    int     i;
    int     ep_cnt;
    int     res;

#define DEV_ENTRY_MSK_VID   0x01
#define DEV_ENTRY_MSK_PTYPE 0x02
#define DEV_ENTRY_MSK_PID   0x04
#define DEV_ENTRY_MSK_CAT   0x08
#define DEV_ENTRY_MSK_EP    0x10

#define DEV_ENTRY_MSK_ALL_PRESENT   (DEV_ENTRY_MSK_VID | DEV_ENTRY_MSK_PTYPE | DEV_ENTRY_MSK_PID | DEV_ENTRY_MSK_CAT)


    for (i=0, a_dev_rec=json_dev_rec->child;
        i<rec_cnt; i++, a_dev_rec=a_dev_rec->next)
    {
        if (!a_dev_rec)
            break;

        //Update error location
        err_loc->dev_ent++;
        err_loc->ep_ent = 0;
        err_loc->if_ent = 0;

        for (dev_rec_ent_msk=0, dev_rec_ent=a_dev_rec->child;
            dev_rec_ent != NULL; dev_rec_ent=dev_rec_ent->next)
        {
            if ((dev_rec_ent->type == cJSON_String) && (!strcmp(dev_rec_ent->string, "_comment")))
            {
                continue;
            }

            if (dev_rec_ent->type == cJSON_Number)
            {
                if (!strcmp(dev_rec_ent->string, "vid"))
                {
                    dev_rec_ent_msk |= DEV_ENTRY_MSK_VID;
                    dev_cfg_records[i].vid = (uint32_t)dev_rec_ent->valueint;
                }
                else if (!strcmp(dev_rec_ent->string, "ptype"))
                {
                    dev_rec_ent_msk |= DEV_ENTRY_MSK_PTYPE;
                    dev_cfg_records[i].ptype = (uint32_t)dev_rec_ent->valueint;
                }
                else if (!strcmp(dev_rec_ent->string, "pid"))
                {
                    dev_rec_ent_msk |= DEV_ENTRY_MSK_PID;
                    dev_cfg_records[i].pid = (uint32_t)dev_rec_ent->valueint;
                }
                else if (!strcmp(dev_rec_ent->string, "category"))
                {
                    dev_rec_ent_msk |= DEV_ENTRY_MSK_CAT;
                    dev_cfg_records[i].category = (uint32_t)dev_rec_ent->valueint;
                }
            }
            else if ((dev_rec_ent->type == cJSON_Array) && (!strcmp(dev_rec_ent->string, "ep")))
            {
                if (dev_rec_ent_msk & DEV_ENTRY_MSK_EP)
                {   //Skip duplicate field
                    continue;
                }
                dev_rec_ent_msk |= DEV_ENTRY_MSK_EP;
                ep_cnt = cJSON_GetArraySize(dev_rec_ent);
                if (ep_cnt > 0)
                {
                    //Load each endpoint into the buffer
                    res = zwdev_cfg_ep_ld(dev_rec_ent, &dev_cfg_records[i].ep_rec, err_loc);
                    if (res != ZW_ERR_NONE)
                    {
                        zwdev_cfg_records_free(dev_cfg_records, rec_cnt);
                        return res;
                    }
                }
            }
        }
        //Check whether all the mandatory entries present
        if ((dev_rec_ent_msk & DEV_ENTRY_MSK_ALL_PRESENT) != DEV_ENTRY_MSK_ALL_PRESENT)
        {
            zwdev_cfg_records_free(dev_cfg_records, rec_cnt);

            //Update error location
            err_loc->ep_ent = 0;
            err_loc->if_ent = 0;
            return ZW_ERR_MISSING_ENTRY;
        }
    }

    return ZW_ERR_NONE;
}


/**
zwdev_rec_compare - Compare two device specific configuration records
@param[in]	a	  First device specific configuration record
@param[in]	b	  Second device specific configuration record
@return		Negative if the first argument is "less" than the second, zero if they are "equal",
            and positive if the first argument is “greater”.
 */
static int zwdev_rec_compare(const void *a, const void *b)
{
    const dev_rec_t *rec_a = (const dev_rec_t *)a;
    const dev_rec_t *rec_b = (const dev_rec_t *)b;

    //Compare manufacturer id (most significance)
    if (rec_a->vid != rec_b->vid)
    {
        return(rec_a->vid > rec_b->vid)? 1 : -1;
    }
    //Compare product type id (2nd most significance)
    else if (rec_a->ptype != rec_b->ptype)
    {
        return(rec_a->ptype > rec_b->ptype)? 1 : -1;
    }
    //Compare product id (least significance)
    else if (rec_a->pid != rec_b->pid)
    {
        return(rec_a->pid > rec_b->pid)? 1 : -1;
    }
    else
    {   //Both are equal
        return 0;
    }
}


/**
zwdev_global_sett_free - Free device global settings
@param[in]	global_sett	    Device setting records
@param[in]	global_sett_cnt	Number of records stored in "global_sett" array
@return
*/
void zwdev_global_sett_free(dev_global_sett_t *global_sett, uint16_t global_sett_cnt)
{
    uint16_t    i;

    if (!global_sett)
    {
        return;
    }

    for (i=0; i<global_sett_cnt;  i++)
    {
        switch(global_sett[i].type)
        {
            case GLOB_SET_TYPE_WKUP_INTV:
            case GLOB_SET_TYPE_WUNMI_DELAY:
                break;
        }
    }

    free(global_sett);
}


/**
zwdev_global_sett_ld - Load device global settings from JSON array
@param[in]	json_dev_rec	Device global settings JSON array
@param[in, out]	rec_cnt	    Input: Record counts in json_dev_rec; Output: Number of settings in dev_global_sett
@param[out]	dev_global_sett	Device global settings
@return		ZW_ERR_xxx
@post       Caller must call zwdev_global_sett_free() if return value is ZW_ERR_NONE
*/
static int zwdev_global_sett_ld(cJSON *json_dev_rec, dev_global_sett_t *dev_global_sett, int *rec_cnt)
{
    cJSON   *dev_sett_rec;
    cJSON   *dev_sett_item;
    int     i;
    int     sett_cnt = 0;

    for (i=0, dev_sett_rec=json_dev_rec->child;
        i < *rec_cnt; i++, dev_sett_rec=dev_sett_rec->next)
    {
        if (!dev_sett_rec)
            break;

        for (dev_sett_item=dev_sett_rec->child;
            dev_sett_item; dev_sett_item=dev_sett_item->next)
        {
            if ((dev_sett_item->type == cJSON_String) && (!strcmp(dev_sett_item->string, "_comment")))
            {
                continue;
            }

            if (dev_sett_item->type == cJSON_Number)
            {
                if (!strcmp(dev_sett_item->string, "wakeup_interval"))
                {
                    dev_global_sett[sett_cnt].type = GLOB_SET_TYPE_WKUP_INTV;
                    dev_global_sett[sett_cnt++].sett.wkup_intv = (uint32_t)dev_sett_item->valueint;
                }
                else if (!strcmp(dev_sett_item->string, "wakeup_no_more_info_delay"))
                {
                    dev_global_sett[sett_cnt].type = GLOB_SET_TYPE_WUNMI_DELAY;
                    dev_global_sett[sett_cnt++].sett.wunmi_delay = (uint16_t)dev_sett_item->valueint;
                }
            }
        }
    }
    *rec_cnt = sett_cnt;
    return ZW_ERR_NONE;
}


/**
zwdev_cfg_load - Load and store device specific configurations and device global settings
@param[in]	cfg_file	    Device specific configurations file in JSON format
@param[out]	records	        Device specific configuration records sorted according to manufacturer id,
                            product type id and product id
@param[out]	record_cnt	    Number of device specific configuration records stored in "records"
@param[out]	global_sett	    Device global settings
@param[out]	global_sett_cnt	Number of entries in device global settings
@param[out]	err_loc	        Parse error location for "device_records" in JSON file
@return		ZW_ERR_xxx
@post       Caller must free the memory allocated to "records" using zwdev_cfg_free() and memory allocated to "global_sett"
            using zwdev_global_sett_free() if return value is ZW_ERR_NONE

*/
int zwdev_cfg_load(const char *cfg_file, dev_rec_t **records, uint16_t *record_cnt,
                   dev_global_sett_t **global_sett, uint16_t *global_sett_cnt, dev_cfg_error_t *err_loc)
{
    int         res;
    int         rec_cnt;    //Number of records
    uint8_t     *json_data;
    cJSON       *json;
    cJSON       *sub_item;
    cJSON       *json_dev_rec = NULL;
    cJSON       *json_global_sett = NULL;
    dev_rec_t   *dev_cfg_records = NULL;
    dev_global_sett_t *dev_global_sett = NULL;
    uint16_t    dev_cfg_cnt;

    //Clear error location
    memset(err_loc, 0, sizeof(dev_cfg_error_t));

    res = zwutl_file_load(cfg_file, &json_data);

    if (res != ZW_ERR_NONE)
    {
        return res;
    }

    json = cJSON_Parse((const char *)json_data);
    free(json_data);

    if (!json)
    {   // Failed
        return ZW_ERR_PARSE_FILE;
    }

    sub_item = json->child;
    //Search for beginning of device record
    while (sub_item)
    {
        if (sub_item->type == cJSON_Array)
        {
            if (!strcmp(sub_item->string, "device_records"))
            {
                json_dev_rec = sub_item;
            }
            else if (!strcmp(sub_item->string, "global_settings"))
            {
                json_global_sett = sub_item;
            }
        }
        sub_item = sub_item->next;
    }

    //Parse "global_settings"
    *global_sett = NULL;
    *global_sett_cnt = 0;

    if (json_global_sett)
    {
        //O.k. Allocate memory for each record
        rec_cnt = cJSON_GetArraySize(json_global_sett);
        if (rec_cnt > 0)
        {
            dev_global_sett = (dev_global_sett_t *)calloc(rec_cnt, sizeof(dev_global_sett_t));
            if (!dev_global_sett)
            {
                res = ZW_ERR_MEMORY;
                goto l_DEV_CONF_ERR1;
            }

            //Load each setting into the buffer
            res = zwdev_global_sett_ld(json_global_sett, dev_global_sett, &rec_cnt);
            if (res != ZW_ERR_NONE)
            {
                goto l_DEV_CONF_ERR2;
            }

            *global_sett = dev_global_sett;
            *global_sett_cnt = rec_cnt;
        }
    }

    //Parse "device_records"
    if (json_dev_rec)
    {
        //O.k. Allocate memory for each record
        rec_cnt = cJSON_GetArraySize(json_dev_rec);
        if (rec_cnt > 0)
        {
            dev_cfg_records = (dev_rec_t *)calloc(rec_cnt, sizeof(dev_rec_t));
            if (!dev_cfg_records)
            {
                res = ZW_ERR_MEMORY;
                goto l_DEV_CONF_ERR3;
            }
            //Save the record counts
            dev_cfg_cnt = rec_cnt;

            //Load each record into the buffer
            res = zwdev_cfg_records_ld(json_dev_rec, dev_cfg_records, rec_cnt, err_loc);
            if (res != ZW_ERR_NONE)
            {
                goto l_DEV_CONF_ERR4;
            }
            //Sort the records
            qsort (dev_cfg_records, rec_cnt, sizeof(dev_rec_t), zwdev_rec_compare);

        }
        else
        {   //No record, but still acceptable
            *records = NULL;
            *record_cnt = 0;
            res = ZW_ERR_NONE;
            goto l_DEV_CONF_ERR1;
        }
    }
    else
    {   //No "device_records"
        res = ZW_ERR_FILE;
        goto l_DEV_CONF_ERR3;
    }

    cJSON_Delete(json);
    *records = dev_cfg_records;
    *record_cnt = dev_cfg_cnt;
    return ZW_ERR_NONE;

l_DEV_CONF_ERR4:
    free(dev_cfg_records);
l_DEV_CONF_ERR3:
    if (*global_sett == dev_global_sett)
    {
        dev_global_sett = NULL;
    }
    zwdev_global_sett_free(*global_sett, *global_sett_cnt);
l_DEV_CONF_ERR2:
    free(dev_global_sett);
l_DEV_CONF_ERR1:
    cJSON_Delete(json);
    return res;
}


/**
zwdev_cfg_find - Search for a match in device specific configuration records
@param[in]	srch_key	Search key
@param[in]	records	    Device specific configuration records sorted according to manufacturer id,
                        product type id and product id
@param[in]	record_cnt	Number of records stored in "records" array
@param[out]	matched_rec	The matched record; either exact match or partial match as explained in the note below
@return     Non-zero if a match is found; else returns zero
@note       This function support "don't care" cases in device specific configuration records.
            The search priority is as follows (in the format (Manf id, Product Type, Product id))
            : (V, V, V), (V, V, X), (V, X, X), (X, X, X) where V="valid value"; X="don't care".
*/
int zwdev_cfg_find(dev_rec_srch_key_t *srch_key, const dev_rec_t *records, int record_cnt, dev_rec_t *matched_rec)
{
    dev_rec_t   search_key;
    dev_rec_t   *srch_res;

    if (!srch_key || !records)
    {
        return 0;
    }

    //Find a record
    search_key.vid = srch_key->vid;
    search_key.ptype = srch_key->ptype;
    search_key.pid = srch_key->pid;

    //Try exact (V, V, V) search
    srch_res = (dev_rec_t *)bsearch ((const void *)&search_key, records, record_cnt, sizeof(dev_rec_t), zwdev_rec_compare);
    if (srch_res)
    {
        *matched_rec = *srch_res;
        return 1;
    }

    //Try (V, V, X) search
    search_key.pid = DEV_REC_ID_WILDCARD;
    srch_res = (dev_rec_t *)bsearch ((const void *)&search_key, records, record_cnt, sizeof(dev_rec_t), zwdev_rec_compare);
    if (srch_res)
    {
        *matched_rec = *srch_res;
        return 1;
    }

    //Try (V, X, X) search
    search_key.ptype = DEV_REC_ID_WILDCARD;
    srch_res = (dev_rec_t *)bsearch ((const void *)&search_key, records, record_cnt, sizeof(dev_rec_t), zwdev_rec_compare);
    if (srch_res)
    {
        *matched_rec = *srch_res;
        return 1;
    }

    //Try (X, X, X) default search
    search_key.vid = DEV_REC_ID_WILDCARD;
    srch_res = (dev_rec_t *)bsearch ((const void *)&search_key, records, record_cnt, sizeof(dev_rec_t), zwdev_rec_compare);
    if (srch_res)
    {
        *matched_rec = *srch_res;
        return 1;
    }

    return 0;
}


/**
zwdev_if_get - Get interface record
@param[in]	ep_rec	    Device specific configuration endpoint records
@param[in]	ep_id	    Endpoint id
@param[in]	if_type	    Interface record type, IF_REC_TYPE_XXX
@return Interface record pointer on success; else returns NULL
*/
void *zwdev_if_get(ep_rec_t *ep_rec, uint8_t ep_id, uint8_t if_type)
{
    if_rec_t    *if_rec;

    while (ep_rec)
    {
        if (ep_rec->id == ep_id)
        {
            if_rec = ep_rec->if_rec;
            while (if_rec)
            {
                if (if_rec->type == if_type)
                {
                    return if_rec->rec.grp;
                }
                if_rec = if_rec->next;
            }
            //Interface record not found
            return NULL;

        }
        ep_rec = ep_rec->next;
    }

    return NULL;

}


/**
zwdev_cls_2_rec_if_type - Find record interface type from the given command class
@param[in]	cmd_cls	    Command class
@return     Record interface type; 0 if not found
*/
static uint8_t zwdev_cls_2_rec_if_type(uint16_t cmd_cls)
{
    uint8_t     if_type = 0;
    int         num_ent;
    int         i;

    //Find corresponding record interface type
    num_ent = sizeof(cmd_cls_rec_type_tbl) / sizeof(zwdev_if_rec_cls);

    for (i=0; i<num_ent; i++)
    {
        if (cmd_cls == cmd_cls_rec_type_tbl[i].cmd_cls)
        {
            if_type = cmd_cls_rec_type_tbl[i].if_rec_type;
            break;
        }
    }

    return if_type;
}


/**
zwdev_if_get_by_cmd_cls - Get interface record by command class
@param[in]	ep_rec	    Device specific configuration endpoint records
@param[in]	ep_id	    Endpoint id
@param[in]	cmd_cls	    Command class
@return Interface record pointer on success; else returns NULL
*/
if_rec_t *zwdev_if_get_by_cmd_cls(ep_rec_t *ep_rec, uint8_t ep_id, uint16_t cmd_cls)
{
    if_rec_t    *if_rec;
    uint8_t     if_type;

    if_type = zwdev_cls_2_rec_if_type(cmd_cls);

    if (!if_type)
    {
        return NULL;
    }

    while (ep_rec)
    {
        if (ep_rec->id == ep_id)
        {
            if_rec = ep_rec->if_rec;
            while (if_rec)
            {
                if (if_rec->type == if_type)
                {
                    return if_rec;
                }
                if_rec = if_rec->next;
            }
            //Interface record not found
            return NULL;

        }
        ep_rec = ep_rec->next;
    }

    return NULL;

}


/**
zwdev_if_ver_get - Get user-defined interface version
@param[in]	ep_rec	    Device specific configuration endpoint records
@param[in]	ep_id	    Endpoint id
@param[in]	cmd_cls	    Command class
@return Interface version (>= 1); 0 if no user-defined interface version found
*/
uint8_t zwdev_if_ver_get(ep_rec_t *ep_rec, uint8_t ep_id, uint16_t cmd_cls)
{
    if_rec_t    *if_rec;
    uint8_t     if_type;

    if_type = zwdev_cls_2_rec_if_type(cmd_cls);

    if (!if_type)
    {
        return 0;
    }

    while (ep_rec)
    {
        if (ep_rec->id == ep_id)
        {
            if_rec = ep_rec->if_rec;
            while (if_rec)
            {
                if (if_rec->type == if_type)
                {
                    return if_rec->usr_def_ver;
                }
                if_rec = if_rec->next;
            }
            //Interface record not found
            return 0;

        }
        ep_rec = ep_rec->next;
    }

    return 0;
}


/**
zwdev_redir_ep_get - Get redirection target endpoint
@param[in]	ep_rec	    Device specific configuration endpoint records
@param[in]	ep_id	    Endpoint id
@param[in]	cmd_cls	    Command class
@param[in]	cmd			Command
@param[out]	ptarget_ep	The target endpoint id if found
@return 1 -- redirection record is found. Target endpoint ID is stored in ptarget_ep; 0 -- No redirection record found
*/
uint8_t zwdev_redir_ep_get(ep_rec_t *ep_rec, uint8_t ep_id, uint16_t cmd_cls, uint8_t cmd, uint8_t *ptarget_ep)
{
    redir_rec_t *redir_rec;

    while (ep_rec)
    {
        if (ep_rec->id == ep_id)
        {
            redir_rec = ep_rec->redir_rec;
            while (redir_rec)
            {
                if (redir_rec->if_id == cmd_cls)
                {
                    if (redir_rec->cmd == -1 ||
                        redir_rec->cmd == (int)cmd)
                    {
                        //Found
                        *ptarget_ep = redir_rec->target_ep;
                        return 1;
                    }
                }
                redir_rec = redir_rec->next;
            }
            //Redirection record not found
            return 0;

        }
        ep_rec = ep_rec->next;
    }

    return 0;
}


/**
zwdev_bin_snsr_match_ld - Load bin_snsr match record from JSON array
@param[in]		json_if_arry    Binary sensor match record JSON array
@param[in,out]	ppbin_snsr_match_rec  Address of the bin_snsr match record pointer
@return		ZW_ERR_xxx
*/
static int zwdev_bin_snsr_match_ld(cJSON *json_if_arry, if_rec_bsnsr_match_t **ppbin_snsr_match_rec)
{
    if_rec_bsnsr_match_t *pMatchRec = *ppbin_snsr_match_rec;
    cJSON       *if_rec_json;       //Binary sensor interface record
    cJSON       *if_rec_ent;        //Binary sensor interface record entry
    int         res;
    unsigned    if_rec_ent_msk;     //Bit mask to check all mandatory entries in if_rec_ent

#define IF_ENTRY_MSK_BIN_SNSR_TYPE          0x01
#define IF_ENTRY_MSK_BIN_SNSR_STATE         0x02
#define IF_ENTRY_MSK_BIN_SNSR_RESULT_TYPE   0x04

#define IF_ENTRY_MSK_BIN_SNSR_MATCH_ALL_PRESENT   (IF_ENTRY_MSK_BIN_SNSR_STATE | IF_ENTRY_MSK_BIN_SNSR_RESULT_TYPE)

    //"Match" block has only one entry
    if_rec_json = json_if_arry->child;

    //for (if_rec_json = json_if_arry->child; if_rec_json; if_rec_json = if_rec_json->next)
    if (if_rec_json)
    {
        for (if_rec_ent_msk = 0, if_rec_ent = if_rec_json->child;
            if_rec_ent; if_rec_ent = if_rec_ent->next)
        {
            if ((if_rec_ent->type == cJSON_String) && (!strcmp(if_rec_ent->string, "_comment")))
            {
                continue;
            }

            if (if_rec_ent->type == cJSON_Number)
            {
                if (!strcmp(if_rec_ent->string, "type") && ((if_rec_ent_msk & IF_ENTRY_MSK_BIN_SNSR_TYPE) == 0))
                {
                    if_rec_ent_msk |= IF_ENTRY_MSK_BIN_SNSR_TYPE;
                    pMatchRec->type = (uint8_t)if_rec_ent->valueint;
                }
                else if (!strcmp(if_rec_ent->string, "state") && ((if_rec_ent_msk & IF_ENTRY_MSK_BIN_SNSR_STATE) == 0))
                {
                    if_rec_ent_msk |= IF_ENTRY_MSK_BIN_SNSR_STATE;
                    pMatchRec->state = if_rec_ent->valueint;
                }
                else if (!strcmp(if_rec_ent->string, "result_if_type") && ((if_rec_ent_msk & IF_ENTRY_MSK_BIN_SNSR_RESULT_TYPE) == 0))
                {
                    if_rec_ent_msk |= IF_ENTRY_MSK_BIN_SNSR_RESULT_TYPE;
                    pMatchRec->resultType = (uint8_t)if_rec_ent->valueint;
                }
            }
        }
        //Check whether all the mandatory entries present
        if ((if_rec_ent_msk & IF_ENTRY_MSK_BIN_SNSR_MATCH_ALL_PRESENT) != IF_ENTRY_MSK_BIN_SNSR_MATCH_ALL_PRESENT)
        {
            res = ZW_ERR_MISSING_ENTRY;
            goto l_BIN_SNSR_MATCH_LD_ERR1;
        }
    }

    return ZW_ERR_NONE;

l_BIN_SNSR_MATCH_LD_ERR1:
    zwdev_bin_snsr_if_match_rec_free(*ppbin_snsr_match_rec);
    *ppbin_snsr_match_rec = NULL;
    return res;
}


/**
zwdev_bin_snsr_if_ld - Load bin_snsr interface records from JSON array
@param[in]	json_if_arry        Binary sensor interface JSON array
@param[in]	if_rec_lst_hd	    Interface record list head
@param[out]	if_rec				Interface record
@return		ZW_ERR_xxx
@post       Caller must call zwdev_bin_snsr_if_free() if return value is ZW_ERR_NONE
*/
static int zwdev_bin_snsr_if_ld(cJSON *json_if_arry, if_rec_t *if_rec_lst_hd, if_rec_t **if_rec)
{
    if_rec_t                *tmp_if = NULL;
    if_rec_bsnsr_match_t    *curr_bsnsr_match_rec = NULL;
    if_rec_bsnsr_match_t    *bsnsr_match_rec = NULL;
    cJSON                   *if_rec_json;       //Binary sensor interface record
    cJSON                   *if_rec_ent;        //Binary sensor interface record entry
    int                     res;
    int                     bsnsr_match_rec_cnt;
    unsigned                if_rec_ent_msk;     //Bit mask to check all mandatory entries in if_rec_ent

#define IF_ENTRY_MSK_BIN_SNSR_MATCH_REC		    0x01
#define IF_ENTRY_MSK_BIN_SNSR_RESULT_REC		0x02

    //Define the mandatory fields
#define IF_ENTRY_MSK_BIN_SNSR_REC_ALL_PRESENT   (IF_ENTRY_MSK_BIN_SNSR_MATCH_REC | IF_ENTRY_MSK_BIN_SNSR_RESULT_REC)


    for (if_rec_json = json_if_arry->child;
        if_rec_json; if_rec_json = if_rec_json->next)
    {   //Iterate each {match, result} block

        for (if_rec_ent_msk = 0, bsnsr_match_rec = NULL, if_rec_ent = if_rec_json->child;
            if_rec_ent; if_rec_ent = if_rec_ent->next)
        {
            switch (if_rec_ent->type)
            {
                case cJSON_String:
                    if (!strcmp(if_rec_ent->string, "_comment"))
                        break;

                case cJSON_Array:
                    if (!strcmp(if_rec_ent->string, "match"))
                    {
                        if (if_rec_ent_msk & IF_ENTRY_MSK_BIN_SNSR_MATCH_REC)
                        {   //Skip duplicate field
                            break;
                        }

                        if_rec_ent_msk |= IF_ENTRY_MSK_BIN_SNSR_MATCH_REC;

                        bsnsr_match_rec_cnt = cJSON_GetArraySize(if_rec_ent);

                        if (bsnsr_match_rec_cnt != 1)  //expect only 1 match record
                        {
                            res = ZW_ERR_VALUE;
                            goto l_BIN_SNSR_IF_LD_ERR1;
                        }

                        if (!bsnsr_match_rec)
                        {
                            zwdev_bin_snsr_if_match_rec_create(&bsnsr_match_rec);
                            if (!bsnsr_match_rec)
                                return ZW_ERR_MEMORY;
                        }

                        //Fill info for the match record
                        res = zwdev_bin_snsr_match_ld(if_rec_ent, &bsnsr_match_rec);

                        if (res != ZW_ERR_NONE)
                        {
                            goto l_BIN_SNSR_IF_LD_ERR1;
                        }

                        if (curr_bsnsr_match_rec)
                        {   //Append as next match record
                            curr_bsnsr_match_rec->next = bsnsr_match_rec;
                        }
                        else
                        {   //Create interface record and assign to it as first match record
                            if (!tmp_if)
                            {   //Allocate memory for the interface record
                                tmp_if = (if_rec_t *)calloc(1, sizeof(if_rec_t));
                                if (tmp_if)
                                {
                                    tmp_if->type = IF_REC_TYPE_BIN_SENSOR;
                                    zwdev_bin_snsr_if_rec_create(&tmp_if->rec.bsnsr);

                                    if (!tmp_if->rec.bsnsr)
                                    {
                                        zwdev_bin_snsr_if_match_rec_free(bsnsr_match_rec);
                                    }
                                }
                                else
                                {
                                    zwdev_bin_snsr_if_match_rec_free(bsnsr_match_rec);
                                }
                            }

                            if (!tmp_if || !tmp_if->rec.bsnsr)
                            {
                                res = ZW_ERR_MEMORY;
                                goto l_BIN_SNSR_IF_LD_ERR1;
                            }

                            tmp_if->rec.bsnsr->rec_match = bsnsr_match_rec;
                            *if_rec = tmp_if;
                        }
                        curr_bsnsr_match_rec = bsnsr_match_rec;
                    }
                    else if (!strcmp(if_rec_ent->string, "result"))
                    {
                        if (if_rec_ent_msk & IF_ENTRY_MSK_BIN_SNSR_RESULT_REC)
                        {   //Skip duplicate field
                            break;
                        }
                        if_rec_ent_msk |= IF_ENTRY_MSK_BIN_SNSR_RESULT_REC;

                        if ((if_rec_ent_msk & IF_ENTRY_MSK_BIN_SNSR_MATCH_REC) == 0)
                        {   //Error, match block should come before result block
                            res = ZW_ERR_ORDER;
                            goto l_BIN_SNSR_IF_LD_ERR1;
                        }

                        bsnsr_match_rec_cnt = cJSON_GetArraySize(if_rec_ent);

                        if (bsnsr_match_rec_cnt != 1)  //expect only 1 result record
                        {
                            res = ZW_ERR_VALUE;
                            goto l_BIN_SNSR_IF_LD_ERR1;
                        }

                        //Fill info for the result record
                        res = zwdev_gen_result_ld(if_rec_ent, if_rec_lst_hd, bsnsr_match_rec->resultType, &bsnsr_match_rec->pResult, *if_rec);

                        if (res != ZW_ERR_NONE)
                        {
                            goto l_BIN_SNSR_IF_LD_ERR1;
                        }

                    }
                    break;
            }
        }
        //Check whether all the mandatory entries present
        if ((if_rec_ent_msk & IF_ENTRY_MSK_BIN_SNSR_REC_ALL_PRESENT) != IF_ENTRY_MSK_BIN_SNSR_REC_ALL_PRESENT)
        {
            res = ZW_ERR_MISSING_ENTRY;
            goto l_BIN_SNSR_IF_LD_ERR1;
        }
    }

    return ZW_ERR_NONE;

l_BIN_SNSR_IF_LD_ERR1:
    if (tmp_if)
    {
        zwdev_cfg_if_free(tmp_if);
        *if_rec = NULL;
    }
    return res;
}

/**
@}
*/


