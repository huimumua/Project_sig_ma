/**
@file   zip_platform.c - Z/IP host controller interface platform specific implementation.

        To implement some common functions in multi-platform environment.

@author David Chow

@version    1.0 7-6-11  Initial release

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

#if defined(_WINDOWS) || defined(WIN32)
    #include "../include/zw_plt_windows.h"     //use in Windows platform
    #include <process.h>                    //for _beginthread, _endthread
#elif defined(OS_LINUX)
    //#include "../include/zw_plt_linux.h"        //use in Linux platform
    #include <pthread.h>
    #include <semaphore.h>
    #include <errno.h>
    #include <unistd.h>
    #ifdef OS_MAC_X
    #include <sys/time.h>
    #endif
#endif
#include <stdlib.h>
#ifndef OS_MAC_X
#include <malloc.h>
#endif
#include <memory.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

#include "../include/zip_platform.h"
#include "../include/zip_util.h"
#include "../include/zip_error.h"
#include "../include/zip_api.h"

/**
@defgroup Platform Multi-platform APIs
Some common functions in multi-platform environment.
@{
*/
//-----------------------------------------------------------------------------------------------------
//  Common platform
//-----------------------------------------------------------------------------------------------------

/**
plt_utf8_chk - Count number of valid UTF-8 bytes
@param[in]	utf8	    Input buffer of UTF-8 text
@param[in]	utf8_len    UTF-8 text length
@return     Return the number of valid UTF-8 bytes.
*/
int plt_utf8_chk(const uint8_t *utf8, uint8_t utf8_len)
{
    int i;
/*
    bytes | bits | representation
     1 |    7 | 0vvvvvvv
     2 |   11 | 110vvvvv 10vvvvvv
     3 |   16 | 1110vvvv 10vvvvvv 10vvvvvv
     4 |   21 | 11110vvv 10vvvvvv 10vvvvvv 10vvvvvv
*/
    i = 0;
    while (i < utf8_len)
    {
        if ((utf8[i] & 0x80) == 0)
        {   //1-byte UTF-8 character
            i++;
        }
        else if ((utf8[i] & 0xE0) == 0xC0)
        {   //2-byte UTF-8 character

            if ((i + 1) >= utf8_len)
            {   //Too few bytes
                break;
            }

            if ((utf8[i+1] & 0xC0) == 0x80)
            {
                i += 2;
            }
            else
                break;
        }
        else if ((utf8[i] & 0xF0) == 0xE0)
        {   //3-byte UTF-8 character
            if ((i + 2) >= utf8_len)
            {   //Too few bytes
                break;
            }

            if (((utf8[i+1] & 0xC0) == 0x80)
                && ((utf8[i+2] & 0xC0) == 0x80))
            {
                i += 3;
            }
            else
                break;
        }
        else if ((utf8[i] & 0xF8) == 0xF0)
        {   //4-byte UTF-8 character
            if ((i + 3) >= utf8_len)
            {   //Too few bytes
                break;
            }

            if (((utf8[i+1] & 0xC0) == 0x80)
                && ((utf8[i+2] & 0xC0) == 0x80)
                && ((utf8[i+3] & 0xC0) == 0x80))
            {
                i += 4;
            }
            else
                break;
        }
    }

    return i;
}


/**
plt_memmem - Find a byte sequence within a memory buffer
@param[in]	buf	            pointer to buffer
@param[in]	buf_len         size of buffer in bytes
@param[in]	byte_seq	    byte sequence to look for
@param[in]	byte_seq_len    size of byte sequence in bytes
@return     if successful, returns a pointer to the first
            occurrence of byte_seq in buf;  otherwise, returns NULL
*/
void *plt_memmem(const void *buf, size_t buf_len,
			 const void *byte_seq, size_t byte_seq_len)
{
	uint8_t *bf = (uint8_t *)buf;
	uint8_t *bs = (uint8_t *)byte_seq;
	uint8_t *p  = bf;
    int      b = *bs;

	while (byte_seq_len <= (buf_len - (p - bf)))
	{
		if ((p = (uint8_t *) memchr(p, b, buf_len - (p - bf))) != NULL)
		{
			if ((memcmp(p, byte_seq, byte_seq_len)) == 0)
				return p;
			else
				p++;
		}
		else
		{
			break;
		}
	}
	return NULL;
}


/**
plt_rand_get - Get a random number
@return     Return a 16-bit pseudo random number.

*/
int16_t plt_rand_get(void)
{
    return(rand() & 0xFFFF);
}


//-----------------------------------------------------------------------------------------------------
//  Windows platform
//-----------------------------------------------------------------------------------------------------
#if defined(_WINDOWS) || defined(WIN32)

///Timer context to facilitate callback and stopping the timer
typedef struct
{
    HANDLE      tmr_hdl;   ///< Timer handle
    tmr_cb_t    tmr_cb;    ///< The callback function to call when timer expires
    void        *data;     ///< The data passed as parameter to the callback function
} tmr_ctx_t;


/**
plt_bin_show - show binary string to the user
@param[in] pltfm_ctx    Context
@param[in] buf          The buffer that stores the binary string
@param[in] len          The length of the binary string.
@return
*/
void plt_bin_show(void *pltfm_ctx, void * buf, uint32_t len)
{
    uint32_t    line_len;
    uint8_t     *bin_byte = (uint8_t *)buf;
    char        tmp[8];
    char        hex_str[MAX_BIN_LINE_LEN * 4];

    while (len > 0)
    {
        //Split the binary string into multiple of MAX_BIN_LINE_LEN
        if (len >= MAX_BIN_LINE_LEN)
        {
            len -= MAX_BIN_LINE_LEN;
            line_len = MAX_BIN_LINE_LEN;
        }
        else
        {
            line_len = len;
            len = 0;
        }

        hex_str[0] = '\0';

        //Convert a line of binary data into hex string
        while (line_len-- > 0)
        {
#ifdef USE_SAFE_VERSION
            sprintf_s(tmp, 8, "%02X ",(unsigned) *bin_byte++);
            strcat_s(hex_str, MAX_BIN_LINE_LEN * 4, tmp);
#else
            sprintf(tmp,"%02X ",(unsigned) *bin_byte++);
            strcat(hex_str, tmp);
#endif
        }

        plt_msg_show(pltfm_ctx, "%s", hex_str);
    }
}


/**
plt_msg_show - show message to the user
@param[in] pltfm_ctx    Context
@param[in] format       The formating string as in call to printf.
@param[in] ...          Variable argument list.
@return
*/
void plt_msg_show(void *pltfm_ctx, char * format, ...)
{
    plt_ctx_t   *ctx = (plt_ctx_t *)pltfm_ctx;
    char  utf8_buf[MAX_PLT_MSG_LEN];   //Buffer for the input formatting string
    int   op_utf16_len;
    TCHAR op_msg[MAX_PLT_MSG_LEN];     //Output message to the edit control window
    va_list args;

    if (!ctx || !ctx->print_txt)
    {
        return;
    }

    va_start (args, format);
    memset(utf8_buf, 0, MAX_PLT_MSG_LEN);
#ifdef USE_SAFE_VERSION
    vsnprintf_s(utf8_buf, MAX_PLT_MSG_LEN -1, _TRUNCATE, format, args);
    strcat_s(utf8_buf, MAX_PLT_MSG_LEN, "\r\n");
#else
    vsnprintf(utf8_buf, MAX_PLT_MSG_LEN -1, format, args);
    strcat(utf8_buf, "\r\n");
#endif

#ifdef  _UNICODE
    //Convert UTF-8 to UTF-16 UNICODE
    op_utf16_len = MultiByteToWideChar(CP_UTF8, 0, utf8_buf, -1, op_msg, 0);

    if (op_utf16_len > MAX_PLT_MSG_LEN)
    {   //Don't print if message is too long
        return;
    }
    MultiByteToWideChar(CP_UTF8, 0, utf8_buf, -1, op_msg, MAX_PLT_MSG_LEN);
#endif

    va_end (args);

#ifdef TCP_PORTAL
    #ifdef  _UNICODE
    ctx->print_txt(op_msg, ctx->display_ctx);
    #else
    ctx->print_txt(utf8_buf, ctx->display_ctx);
    #endif
#else
    #ifdef  _UNICODE
    ctx->print_txt(op_msg);
    #else
    ctx->print_txt(utf8_buf);
    #endif
#endif
}


/**
plt_msg_ts_show - show message to the user with time stamp
@param[in] pltfm_ctx    Context
@param[in] format       The formating string as in call to printf.
@param[in] ...          Variable argument list.
@return
*/
void plt_msg_ts_show(void *pltfm_ctx, char * format, ...)
{
    plt_ctx_t   *ctx = (plt_ctx_t *)pltfm_ctx;
    SYSTEMTIME lt;      // the local time
    int     time_len;   // the string length of local time
    char  utf8_buf[MAX_PLT_MSG_LEN];   //Buffer for the input formatting string
    int   op_utf16_len;
    TCHAR op_msg[MAX_PLT_MSG_LEN];     //Output message to the edit control window
    va_list args;

    if (!ctx || !ctx->print_txt)
    {
        return;
    }

    GetLocalTime(&lt);

#ifdef USE_SAFE_VERSION
    sprintf_s(utf8_buf, MAX_PLT_MSG_LEN, "%02d:%02d:%02d -- ", lt.wHour, lt.wMinute, lt.wSecond);
#else
    sprintf(utf8_buf, "%02d:%02d:%02d -- ", lt.wHour, lt.wMinute, lt.wSecond);
#endif
    time_len = strlen(utf8_buf);

    va_start (args, format);
    memset(utf8_buf + time_len, 0, MAX_PLT_MSG_LEN - time_len);
#ifdef USE_SAFE_VERSION
    vsnprintf_s(utf8_buf + time_len, MAX_PLT_MSG_LEN - time_len -3, _TRUNCATE,  format, args);
    strcat_s(utf8_buf, MAX_PLT_MSG_LEN, "\r\n");
#else
    vsnprintf (utf8_buf + time_len, MAX_PLT_MSG_LEN - time_len -3, format, args);
    strcat(utf8_buf, "\r\n");
#endif

#ifdef  _UNICODE
    //Convert UTF-8 to UTF-16 UNICODE
    op_utf16_len = MultiByteToWideChar(CP_UTF8, 0, utf8_buf, -1, op_msg, 0);

    if (op_utf16_len > MAX_PLT_MSG_LEN)
    {   //Don't print if message is too long
        return;
    }
    MultiByteToWideChar(CP_UTF8, 0, utf8_buf, -1, op_msg, MAX_PLT_MSG_LEN);
#endif

    va_end (args);

#ifdef TCP_PORTAL
    #ifdef  _UNICODE
    ctx->print_txt(op_msg, ctx->display_ctx);
    #else
    ctx->print_txt(utf8_buf, ctx->display_ctx);
    #endif
#else
    #ifdef  _UNICODE
    ctx->print_txt(op_msg);
    #else
    ctx->print_txt(utf8_buf);
    #endif
#endif

}


/**
plt_tmr_cb - timer callback function from the OS
@param[in] param 	            Pointer to timer context.
@param[in] timeout_occurred     Always true for timer callback.
@return
@note    This callback function must not call the TerminateThread function
*/
VOID CALLBACK plt_tmr_cb(PVOID param, BOOLEAN timeout_occurred)
{
    tmr_ctx_t       *timer_context = (tmr_ctx_t *)param;
    timer_context->tmr_cb(timer_context->data);
}


/**
plt_tmr_start - Start a timer
@param[in] pltfm_ctx    Context
@param[in] tmout_ms     Timeout value in milliseconds
@param[in] tmout_cb     Pointer to the timeout callback function
@param[in] data         The data that will be passed back as parameter when the callback function is invoked
@return     Return a pointer to the internal timer context, NULL indicates failure.
@post 		Must call plt_tmr_stop to release the timer resource even if the timer has expired.

*/
void    *plt_tmr_start(plt_ctx_t *pltfm_ctx, uint32_t  tmout_ms, tmr_cb_t  tmout_cb, void *data)
{
    tmr_ctx_t       *tmr_ctx;

    //Allocate memory for timer context
    tmr_ctx = (tmr_ctx_t *)malloc(sizeof(tmr_ctx_t));

    if (!tmr_ctx)
        return NULL;

    //Save the parameters for callback when timer expires
    tmr_ctx->tmr_cb = tmout_cb;
    tmr_ctx->data = data;
    if (CreateTimerQueueTimer(&tmr_ctx->tmr_hdl, NULL, plt_tmr_cb, tmr_ctx,
                              tmout_ms, 0, WT_EXECUTEINIOTHREAD | WT_EXECUTEONLYONCE))
    {
        return tmr_ctx;
    }
    return NULL;

}

/**
plt_tmr_stop - Stop the timer and release timer resource
@param[in] pltfm_ctx    Context
@param[in] context     The timer context returned from the plt_tmr_start
@return     Return non-zero indicates success, zero indicates failure.
@post		The caller should not use the timer context after this call.
*/
uint32_t     plt_tmr_stop(plt_ctx_t *pltfm_ctx, void  *context)
{
    tmr_ctx_t     *tmr_ctx = (tmr_ctx_t *)context;

    if (!tmr_ctx)
        return 0;

    if (!DeleteTimerQueueTimer(NULL, tmr_ctx->tmr_hdl, NULL))
    {
        if (GetLastError() != ERROR_IO_PENDING)
        {
            free(tmr_ctx);
            return 0;
        }
    }
    free(tmr_ctx);
    return 1;
}



/**
plt_mtx_init - Initialize a recursive mutex
@param[in, out] context     Pointer to a void * for storing the mutex context
@return     Return non-zero indicates success, zero indicates failure.
@post       Should call  plt_mtx_destroy to free the mutex context
*/
uint32_t     plt_mtx_init(struct plt_mtx_t **context)
{
    struct plt_mtx_t    *mtx = (struct plt_mtx_t *)malloc(sizeof (struct plt_mtx_t));

    if (!mtx)
    {
        return 0;
    }

    mtx->handle = CreateMutex(NULL, FALSE, NULL);
    if (mtx->handle == NULL)
    {
        free(mtx);
        return 0;
    }
    *context = mtx;
    return 1;

}


/**
plt_mtx_trylck - Try to lock a mutex
@param[in] context     The mutex context
@return     Zero if a lock on the mutex object referenced by mutex is acquired.
            Otherwise, an error number is returned to indicate the error.
@pre       The mutex context must be initialized by plt_mtx_init
*/
int     plt_mtx_trylck(struct plt_mtx_t *context)
{
    DWORD   ret;

    ret = WaitForSingleObject(context->handle, 0);

    return (ret == WAIT_OBJECT_0)? 0 : -1;
}

/**
plt_mtx_lck - Lock a mutex
@param[in] context     The mutex context
@return
@pre       The mutex context must be initialized by plt_mtx_init
*/
void     plt_mtx_lck(struct plt_mtx_t *context)
{
    WaitForSingleObject(context->handle, INFINITE);
}

/**
plt_mtx_ulck - Unlock a mutex
@param[in] context     The mutex context
@return
@pre       The mutex context must be initialized by plt_mtx_init
*/
void     plt_mtx_ulck(struct plt_mtx_t *context)
{
    ReleaseMutex(context->handle);
}

/**
plt_mtx_destroy - Destroy a mutex
@param[in] context     The mutex context
@return
@pre       The mutex context must be initialized by plt_mtx_init
*/
void     plt_mtx_destroy(struct plt_mtx_t *context)
{
    CloseHandle(context->handle);
    free(context);
}

/**
plt_sem_init - Init a semaphore
@param[in, out] context     Pointer to a void * for storing the semaphore context
@return     Return non-zero indicates success, zero indicates failure.
@post       Should call  plt_sem_destroy to free the semaphore context

*/
uint32_t     plt_sem_init(void **context)
{
    HANDLE  *handle = (HANDLE  *)malloc(sizeof (HANDLE));
    if (!handle)
        return 0;

    *handle = CreateSemaphore(
                             NULL,           // default security attributes
                             0,              // initial count
                             65535,          // maximum count
                             NULL);          // unnamed semaphore

    if (*handle == NULL)
    {
        free(handle);
        return 0;
    }
    *context = handle;
    return 1;

}

/**
plt_sem_wait - Wait until the semaphore is signaled
@param[in] context     The semaphore context
@return
@pre       	The semaphore context must be initialized by plt_sem_init
*/
void     plt_sem_wait(void *context)
{
    HANDLE  *handle = (HANDLE  *)context;
    WaitForSingleObject(*handle, INFINITE);
}

/**
plt_sem_post - Increment the semaphore count by 1
@param[in] context     The semaphore context
@return
@pre       	The semaphore context must be initialized by plt_sem_init
*/
void     plt_sem_post(void *context)
{
    HANDLE  *handle = (HANDLE  *)context;
    ReleaseSemaphore(*handle, 1, NULL);
}

/**
plt_sem_destroy - Destroy a semaphore
@param[in] context     The semaphore context
@return
@pre       	The semaphore context must be initialized by plt_sem_init
*/
void     plt_sem_destroy(void *context)
{
    HANDLE  *handle = (HANDLE  *)context;
    CloseHandle(*handle);
    free(handle);
}

typedef struct
{
    uint16_t          waiter_cnt;       ///< Count of the number of waiters.

    CRITICAL_SECTION  waiter_cnt_lck;   ///< Waiter count lock

    enum
    {
        COND_SIGNAL = 0,
        COND_BROADCAST = 1,
        COND_MAX_EVENTS = 2
    }cond_type_e;

    HANDLE events[COND_MAX_EVENTS];     ///< Signal and broadcast event HANDLEs.

} pthread_cond_t;

/**
plt_cond_init - Initialize condition variables
@param[out] context     Condition variables context
@return     Return non-zero indicates success, zero indicates failure.
@post       Should call  plt_cond_destroy to free the context

*/
int     plt_cond_init(void **context)
{
    pthread_cond_t  *cv = (pthread_cond_t  *)malloc(sizeof(pthread_cond_t));

    if (!cv)
        return 0;

    // Initialize the count to 0.
    cv->waiter_cnt = 0;

    // Create an auto-reset event.
    cv->events[COND_SIGNAL] = CreateEvent (NULL,  // no security
                                           FALSE, // auto-reset event
                                           FALSE, // non-signaled initially
                                           NULL); // unnamed

    if (cv->events[COND_SIGNAL] == NULL)
    {
        free(cv);
        return 0;
    }


    // Create a manual-reset event.
    cv->events[COND_BROADCAST] = CreateEvent (NULL,  // no security
                                              TRUE,  // manual-reset
                                              FALSE, // non-signaled initially
                                              NULL); // unnamed

    if (cv->events[COND_BROADCAST] == NULL)
    {
        CloseHandle(cv->events[COND_SIGNAL]);
        free(cv);
        return 0;
    }

    //Initialize critical section
    InitializeCriticalSection(&cv->waiter_cnt_lck);

    *context = cv;
    return 1;
}

/**
plt_cond_timedwait - Wait on a condition
@param[in] cond_ctx     The condition variables context
@param[in] mtx          The mutex which must be acquired (locked) or else undefined behavior results
@param[in] timeout_ms   Wait timeout in milliseconds
@return     0=signal received (wait succeeded); 1=timeout; 2=failed
@pre       	The cond_ctx context must be initialized by plt_cond_init
*/
int     plt_cond_timedwait(void *cond_ctx, struct plt_mtx_t *mtx, uint16_t timeout_ms)
{
    pthread_cond_t  *cv = (pthread_cond_t  *)cond_ctx;
    int last_waiter;
    int result;

    // Increment number of waiters
    EnterCriticalSection (&cv->waiter_cnt_lck);
    cv->waiter_cnt++;
    LeaveCriticalSection (&cv->waiter_cnt_lck);

    // It's ok to release the <external_mutex> here since Win32
    // manual-reset events maintain state when used with
    // <SetEvent>.  This avoids the "lost wakeup" bug...
    plt_mtx_ulck(mtx);

    // Wait for either event to become signaled due to <pthread_cond_signal>
    // being called _OR_ <pthread_cond_broadcast> being called.
    result = WaitForMultipleObjects (2, cv->events, FALSE, timeout_ms);
    switch (result)
    {
        case WAIT_TIMEOUT:
            result = 1;
            break;

        case WAIT_FAILED:
            result = 2;
            break;

        case WAIT_OBJECT_0:
        case WAIT_OBJECT_0 + 1:
            result = 0;
            break;

        default:
            result = 2;
    }

    EnterCriticalSection (&cv->waiter_cnt_lck);
    cv->waiter_cnt--;
    last_waiter =((result == (WAIT_OBJECT_0 + COND_BROADCAST))
                  &&(cv->waiter_cnt == 0));

    LeaveCriticalSection (&cv->waiter_cnt_lck);

    // Some thread called <pthread_cond_broadcast>.
    if (last_waiter)
        // We're the last waiter to be notified or to stop waiting, so
        // reset the manual event.
        ResetEvent (cv->events[COND_BROADCAST]);

    // Reacquire the <external_mutex>.
    plt_mtx_lck(mtx);

    return result;
}

/**
plt_cond_broadcast - Unblock all threads currently blocked on the specified condition variable
@param[in] cond_ctx     The condition variables context
@return
@pre       	The cond_ctx context must be initialized by plt_cond_init
*/
void     plt_cond_broadcast(void *cond_ctx)
{
    pthread_cond_t  *cv = (pthread_cond_t  *)cond_ctx;
    int have_waiters;

    // Avoid race conditions.
    EnterCriticalSection (&cv->waiter_cnt_lck);
    have_waiters = cv->waiter_cnt > 0;
    LeaveCriticalSection (&cv->waiter_cnt_lck);

    if (have_waiters)
        SetEvent (cv->events[COND_BROADCAST]);

}

/**
plt_cond_signal - Unblock at least one of the threads that are blocked on the specified condition variable
@param[in] cond_ctx     The condition variables context
@return
@pre       	The cond_ctx context must be initialized by plt_cond_init
*/
void     plt_cond_signal(void *cond_ctx)
{
    pthread_cond_t  *cv = (pthread_cond_t  *)cond_ctx;
    int have_waiters;

    // Avoid race conditions.
    EnterCriticalSection (&cv->waiter_cnt_lck);
    have_waiters = cv->waiter_cnt > 0;
    LeaveCriticalSection (&cv->waiter_cnt_lck);

    if (have_waiters)
        SetEvent (cv->events[COND_SIGNAL]);

}

/**
plt_cond_destroy - Destroy condition variables
@param[in] cond_ctx     The condition variables context
@return
@pre       	The cond_ctx context must be initialized by plt_cond_init
*/
void     plt_cond_destroy(void *cond_ctx)
{
    pthread_cond_t  *cv = (pthread_cond_t  *)cond_ctx;

    DeleteCriticalSection(&cv->waiter_cnt_lck);
    CloseHandle(cv->events[COND_SIGNAL]);
    CloseHandle(cv->events[COND_BROADCAST]);
    free(cv);
}

/**
plt_thrd_create - Create a thread and run it
@param[in] start_adr    Pointer to a function for the newly created thread to execute.
@param[in] args         The argument passed to the newly created thread function.
@return     Return zero indicates success, negative number indicates failure.
*/
int     plt_thrd_create(void(*start_adr)( void * ), void *args)
{

    if (_beginthread(start_adr, 0, args) == -1L)
    {
        return -1;
    }
    return 0;

}

/**
plt_sleep - Suspends the execution of the current thread until the time-out interval elapses.
@param[in] tmout_ms   The time interval for which execution is to be suspended, in milliseconds.
@return
*/
void     plt_sleep(uint32_t    tmout_ms)
{
    Sleep(tmout_ms);
}


/**
plt_periodic_start - Start a periodic timer
@param[in] pltfm_ctx    Context
@param[in] tmout_ms     Periodic timeout value in milliseconds
@param[in] tmout_cb     Pointer to the timeout callback function which executes a short task only
@param[in] data         The data that will be passed back as parameter when the callback function is invoked
@return     Return a pointer to the internal timer context, NULL indicates failure.
@post 		Must call plt_tmr_stop to release the timer resource even if the timer has expired.
@note       The periodic callback function (tmout_cb) MUST NOT sleep or be blocked
            (e.g. calling plt_sem_wait, plt_mtx_lck, etc) in order to avoid deadlock
*/
void    *plt_periodic_start(plt_ctx_t *pltfm_ctx, uint32_t  tmout_ms, tmr_cb_t  tmout_cb, void *data)
{
    tmr_ctx_t       *tmr_ctx;

    //Allocate memory for timer context
    tmr_ctx = (tmr_ctx_t *)malloc(sizeof(tmr_ctx_t));

    if (!tmr_ctx)
        return NULL;

    //Save the parameters for callback when timer expires
    tmr_ctx->tmr_cb = tmout_cb;
    tmr_ctx->data = data;
    if (CreateTimerQueueTimer(&tmr_ctx->tmr_hdl, NULL, plt_tmr_cb, tmr_ctx,
                              tmout_ms, tmout_ms, WT_EXECUTEINPERSISTENTTHREAD))
    {
        return tmr_ctx;
    }
    return NULL;

}


/**
Converts Unicode UTF-16 text to Unicode UTF-8
@param[in]	utf16_src   Source location having UTF16 text terminated by L'\0'
@param[out]	utf8_output	Output buffer for UTF8 text terminated by '\0'
@param[in]	out_buf_len	Output buffer length in bytes
@param[in]	big_endian	UTF16 endianness; 1=big-endian, 0=little-endian
@return     Return zero indicates success, negative number indicates failure.
*/
int plt_utf16_to_8(const char *utf16_src, char *utf8_output, uint8_t out_buf_len, uint8_t big_endian)
{
    int     loop;
	int     result;
	int     char_len;
	LPWSTR  tmp_utf16;

    if ((utf16_src == NULL) || ((*((WCHAR *)utf16_src)) == L'\0'))
    {
        return -1;
    }

    //Save a copy of the utf16 source
    char_len = wcslen((WCHAR *)utf16_src);
	tmp_utf16 = (LPWSTR)calloc(char_len + 1, sizeof(WCHAR));

    if (!tmp_utf16)
    {
        return -2;
    }

#ifdef USE_SAFE_VERSION
	wcsncpy_s(tmp_utf16, char_len + 1, (WCHAR *)utf16_src, char_len + 1);
#else
	wcsncpy(tmp_utf16, (WCHAR *)utf16_src, char_len + 1);
#endif

    //Convert byte order if host and  UTF-16 input string are different in endianess
	if (util_is_host_big_endian() != big_endian)
	{
        int			i;
        uint16_t	*word_ptr;

        word_ptr = (uint16_t *)tmp_utf16;

        for (i=0; i<char_len; i++)
        {
            *word_ptr = (*word_ptr << 8) | (*word_ptr >> 8);
            word_ptr++;
        }

	}

    //Find the length of UTF-16 that will convert to UTF-8 that fits into the given output buffer
    loop = 100;
	while (loop-- > 0)
	{
		if (*tmp_utf16 == L'\0')
		{
			*utf8_output = '\0';
			free(tmp_utf16);
			return 0;
		}

		result = WideCharToMultiByte(CP_UTF8, 0, tmp_utf16, -1, NULL, 0, NULL, NULL);
		if ((result == 0) || (result > out_buf_len))
		{
			tmp_utf16[wcslen(tmp_utf16) - 1] = L'\0';		// Truncate
		}
		else
		{
			break;
		}
	}

    if (result == 0)
    {
        free(tmp_utf16);
        return -3;
    }

    //Do the conversion
	result = WideCharToMultiByte(CP_UTF8, 0, tmp_utf16, -1, utf8_output, result, NULL, NULL);

    free(tmp_utf16);

    if (result == 0)
    {   //Failed
        return -4;
    }

    return 0;

}


/**
plt_init - Initialize platform
@param[in] pltfm_ctx        Context
@param[in] display_txt_fn   Function pointer to display null terminated string
@param[in] display_ctx      Display context
@return    Return zero on success; negative error number indicates failure.
*/
#ifdef TCP_PORTAL
int plt_init(plt_ctx_t *pltfm_ctx, print_fn display_txt_fn, void *display_ctx)
#else
int plt_init(plt_ctx_t *pltfm_ctx, print_fn display_txt_fn)
#endif
{
    if (pltfm_ctx->init_done > 0)
    {
        //Update initialization count
        pltfm_ctx->init_done++;

        return 0;
    }

    pltfm_ctx->print_txt = display_txt_fn;
#ifdef TCP_PORTAL
    pltfm_ctx->display_ctx = display_ctx;
#endif

    //Init random number seed
    srand((unsigned)time(NULL));

    pltfm_ctx->init_done++;

    return 0;
}


/**
plt_exit - Clean up the platform
@param[in] pltfm_ctx    Context
@return
*/
void plt_exit(plt_ctx_t *pltfm_ctx)
{

    //Update initialization count
    pltfm_ctx->init_done--;

    if (pltfm_ctx->init_done > 0)
    {   //There are some threads still using this platform functions, don't clean up
        return;
    }

    memset(pltfm_ctx, 0, sizeof(plt_ctx_t));
}


#endif //#if defined(_WINDOWS) || defined(WIN32)


//-----------------------------------------------------------------------------------------------------
//  Linux platform
//-----------------------------------------------------------------------------------------------------
#ifdef OS_LINUX
#define  TRUNCATE_ON    ///< Truncate utf32 to fit utf8 buffer

///Thread context
typedef struct
{
    void (*start_adr)(void * ); ///< The function to run
    void *args;                 ///< Argument to pass to the start_adr()
} thrd_ctx_t;

///Timer context to facilitate callback and stopping the timer
typedef struct
{
    uint32_t    tmr_expiry;///< Timer ticks the timer will expire
    uint32_t    tmr_reload;///< Timer ticks to reload (for periodic timer only)
    tmr_cb_t    tmr_cb;    ///< The callback function to call when timer expires
    void        *data;     ///< The data passed as parameter to the callback function
    uint16_t    id;        ///< Timer identifier
} tmr_ctx_t;


#ifdef OS_MAC_X
///Semaphore context
typedef struct
{
    char name[12];      ///< Semaphore name
    sem_t *sem;        ///< Pointer to allocated semaphore
} sem_ctx_t;
#endif


/**
plt_bin_show - show binary string to the user
@param[in] pltfm_ctx    Context
@param[in] buf          The buffer that stores the binary string
@param[in] len          The length of the binary string.
@return
*/
void plt_bin_show(void *pltfm_ctx, void * buf, uint32_t len)
{
    uint32_t    line_len;
    uint8_t     *bin_byte = (uint8_t *)buf;
    char        tmp[8];
    char        hex_str[MAX_BIN_LINE_LEN * 4];

    while (len > 0)
    {
        //Split the binary string into multiple of MAX_BIN_LINE_LEN
        if (len >= MAX_BIN_LINE_LEN)
        {
            len -= MAX_BIN_LINE_LEN;
            line_len = MAX_BIN_LINE_LEN;
        }
        else
        {
            line_len = len;
            len = 0;
        }

        hex_str[0] = '\0';

        //Convert a line of binary data into hex string
        while (line_len-- > 0)
        {
            sprintf(tmp,"%02X ",(unsigned) *bin_byte++);
            strcat(hex_str, tmp);
        }

        ALOGI("%s", hex_str);
    }
}


/**
plt_msg_show - show message to the user
@param[in] pltfm_ctx    Context
@param[in] format       The formating string as in call to printf.
@param[in] ...          Variable argument list.
@return
*/
void plt_msg_show(void *pltfm_ctx, char * format, ...)
{
    plt_ctx_t   *ctx = (plt_ctx_t *)pltfm_ctx;
    char    ansi_char_buf[MAX_PLT_MSG_LEN];   //Buffer for the input formatting string
    va_list args;

    if (!ctx || !ctx->print_txt)
    {
        return;
    }

    va_start (args, format);
    memset(ansi_char_buf, 0, MAX_PLT_MSG_LEN);
    vsnprintf(ansi_char_buf, MAX_PLT_MSG_LEN -1, format, args);
    strcat(ansi_char_buf, "\n");

    va_end (args);

#ifdef TCP_PORTAL
    ctx->print_txt(ansi_char_buf, ctx->display_ctx);
#else
    ctx->print_txt(ansi_char_buf);
#endif
}


/**
plt_msg_ts_show - show message to the user with time stamp
@param[in] pltfm_ctx    Context
@param[in] format       The formating string as in call to printf.
@param[in] ...          Variable argument list.
@return
*/
void plt_msg_ts_show(void *pltfm_ctx, char * format, ...)
{
    plt_ctx_t   *ctx = (plt_ctx_t *)pltfm_ctx;
    int         time_len;   // the string length of local time
    char        ansi_char_buf[MAX_PLT_MSG_LEN];   //Buffer for the input formatting string
    struct tm   *local_time;
    time_t      time_val;
    va_list     args;

    if (!ctx || !ctx->print_txt)
    {
        return;
    }

    //Get local time
    time (&time_val);
    local_time = localtime (&time_val);

    sprintf(ansi_char_buf, "%02d:%02d:%02d -- ", local_time->tm_hour, local_time->tm_min, local_time->tm_sec);
    time_len = strlen(ansi_char_buf);

    va_start (args, format);
    memset(ansi_char_buf + time_len, 0, MAX_PLT_MSG_LEN - time_len);
    vsnprintf (ansi_char_buf + time_len, MAX_PLT_MSG_LEN - time_len -3, format, args);
    strcat(ansi_char_buf, "\n");

    va_end (args);

#ifdef TCP_PORTAL
    ctx->print_txt(ansi_char_buf, ctx->display_ctx);
#else
    ctx->print_txt(ansi_char_buf);
#endif

}


/**
plt_tmr_create - Create and run a timer
@param[in] pltfm_ctx    Context
@param[in] tmout_ms     Timeout value in milliseconds
@param[in] tmout_cb     Pointer to the timeout callback function
@param[in] data         The data that will be passed back as parameter when the callback function is invoked
@param[in] periodic     Flag to indicate timer is a periodic timer.
@return     Return a pointer to the internal timer context, NULL indicates failure.
@pre        Must call plt_init once before using all timer related functions
@post 		Must call plt_tmr_stop to release the timer resource even if the timer has expired.

*/
static void *plt_tmr_create(plt_ctx_t *pltfm_ctx, uint32_t  tmout_ms, tmr_cb_t  tmout_cb, void *data, int16_t periodic)
{
    tmr_ctx_t   tmr_ctx;
    int         ret_val;
    uint32_t    tmr_period;

    //Generate timer id
    //Note: Periodic timer id should be in different range from one-shot timer id.  If not
    //      there will be a possibility one-shot timer id equals to periodic timer id after id roll-over
    //      from 0xFFFF and this may cause the periodic timer being removed while the real intention is to
    //      remove the one-shot timer with the identical timer id.
    plt_mtx_lck(pltfm_ctx->tmr_mtx);

    if (!periodic)
    {   //One-shot
        tmr_ctx.id = ++pltfm_ctx->id_gen & ~0x8000;
        if (tmr_ctx.id == 0)
        {   //Don't use value of zero as it also represents NULL
            tmr_ctx.id++;
            pltfm_ctx->id_gen++;
        }
    }
    else
    {   //Periodic
        tmr_ctx.id = (++pltfm_ctx->per_id_gen) | 0x8000;
    }


    plt_mtx_ulck(pltfm_ctx->tmr_mtx);

    //Save the parameters for callback when timer expires
    tmr_ctx.tmr_cb = tmout_cb;
    tmr_ctx.data = data;

    //Calculate expiry timer ticks
    tmr_period = tmout_ms / PLT_TIMER_RESOLUTION;

    if ((tmout_ms % PLT_TIMER_RESOLUTION) > 0)
    {
        tmr_period++;
    }

    tmr_ctx.tmr_reload = (periodic)? tmr_period : 0;
    tmr_ctx.tmr_expiry = pltfm_ctx->tmr_tick + tmr_period;

    //Add to the timer list
    ret_val = util_list_add(pltfm_ctx->tmr_mtx, &pltfm_ctx->tmr_lst_hd, (uint8_t *)&tmr_ctx, sizeof(tmr_ctx_t));
    if (ret_val == 0)
    {
		return (void *)((uintptr_t)tmr_ctx.id);
    }
    return NULL;

}


/**
plt_tmr_start - Start a timer
@param[in] pltfm_ctx    Context
@param[in] tmout_ms     Timeout value in milliseconds
@param[in] tmout_cb     Pointer to the timeout callback function
@param[in] data         The data that will be passed back as parameter when the callback function is invoked
@return     Return a pointer to the internal timer context, NULL indicates failure.
@pre        Must call plt_init once before using all timer related functions
@post 		Must call plt_tmr_stop to release the timer resource even if the timer has expired.

*/
void    *plt_tmr_start(plt_ctx_t *pltfm_ctx, uint32_t  tmout_ms, tmr_cb_t  tmout_cb, void *data)
{
    return plt_tmr_create(pltfm_ctx, tmout_ms, tmout_cb, data, 0);
}


/**
plt_tmr_stop - Stop the timer and release timer resource
@param[in] pltfm_ctx   Context
@param[in] context     The timer context returned from the plt_tmr_start
@return     Return non-zero indicates success, zero indicates failure.
@pre        Must call plt_init once before using all timer related functions
@post		The caller should not use the timer context after this call.
*/
uint32_t     plt_tmr_stop(plt_ctx_t *pltfm_ctx, void  *context)
{
    uint32_t        ret_val;
	uint16_t        tmr_id = (uintptr_t)context;
    util_lst_t      *prev_ent;     //Pointer to previous list entry
    util_lst_t      *temp;
    tmr_ctx_t       *tmr_ctx;

    if (!tmr_id)
        return 0;

    plt_mtx_lck(pltfm_ctx->tmr_mtx);

    if (pltfm_ctx->tmr_lst_hd == NULL)
    {   //Nothing to remove
        plt_mtx_ulck(pltfm_ctx->tmr_mtx);
        return 0;
    }

    temp = pltfm_ctx->tmr_lst_hd;
    prev_ent = NULL;

    ret_val = 0;    //initialize return value to failure
    while (temp)
    {
        tmr_ctx = (tmr_ctx_t *)temp->wr_buf;

        if (tmr_ctx->id == tmr_id)
        {   //Found timer
            if (!prev_ent)
            {   //No previous entry, this is the head of list
                pltfm_ctx->tmr_lst_hd = temp->next;
            }
            else
            {   //Assign the next entry to the previous entry
                prev_ent->next = temp->next;
            }
            free(temp);
            ret_val = 1;
            break;
        }
        prev_ent = temp;
        temp = temp->next;
    }

    plt_mtx_ulck(pltfm_ctx->tmr_mtx);
    return ret_val;
}


/**
plt_mtx_init - Initialize a recursive mutex
@param[in, out] context     Pointer to a void * for storing the mutex context
@return     Return non-zero indicates success, zero indicates failure.
@post       Should call  plt_mtx_destroy to free the mutex context
*/
uint32_t     plt_mtx_init(struct plt_mtx_t **context)
{
    uint32_t                ret_val;
    pthread_mutexattr_t     attr;
    pthread_mutex_t         *mutex = (pthread_mutex_t  *)malloc(sizeof (pthread_mutex_t));

    if (!mutex)
        return 0;

    if (pthread_mutexattr_init(&attr) != 0)
    {
        free(mutex);
        return 0;
    }

    //Set attribute to recursive mutex
#ifdef OS_MAC_X
    if (pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE) != 0)
    {
        pthread_mutexattr_destroy(&attr);
        free(mutex);
        return 0;
    }
#else
    if (pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE_NP) != 0)
    {
        pthread_mutexattr_destroy(&attr);
        free(mutex);
        return 0;
    }
#endif
    //Initialize mutex
    if (pthread_mutex_init (mutex, &attr) == 0)
    {   //Successful
        *context = (struct plt_mtx_t *)mutex;
        ret_val = 1;
    }
    else
    {
        free(mutex);
        ret_val = 0;
    }

    pthread_mutexattr_destroy(&attr);
    return ret_val;

}


/**
plt_mtx_trylck - Try to lock a mutex
@param[in] context     The mutex context
@return     Zero if a lock on the mutex object referenced by mutex is acquired.
            Otherwise, an error number is returned to indicate the error.
@pre       The mutex context must be initialized by plt_mtx_init
*/
int     plt_mtx_trylck(struct plt_mtx_t *context)
{
    return pthread_mutex_trylock((pthread_mutex_t *)context);
}


/**
plt_mtx_lck - Lock a mutex
@param[in] context     The mutex context
@return
@pre       The mutex context must be initialized by plt_mtx_init
*/
void     plt_mtx_lck(struct plt_mtx_t *context)
{
    pthread_mutex_lock((pthread_mutex_t *)context);
}

/**
plt_mtx_ulck - Unlock a mutex
@param[in] context     The mutex context
@return
@pre       The mutex context must be initialized by plt_mtx_init
*/
void     plt_mtx_ulck(struct plt_mtx_t *context)
{
    pthread_mutex_unlock((pthread_mutex_t *)context);
}

/**
plt_mtx_destroy - Destroy a mutex
@param[in] context     The mutex context
@return
@pre       The mutex context must be initialized by plt_mtx_init
*/
void     plt_mtx_destroy(struct plt_mtx_t *context)
{
    pthread_mutex_t *mutex = (pthread_mutex_t  *)context;
    pthread_mutex_destroy(mutex);
    free(mutex);
}

#ifdef OS_MAC_X
/**
plt_sem_init - Init a semaphore
@param[in, out] context     Pointer to a void * for storing the semaphore context
@return     Return non-zero indicates success, zero indicates failure.
@post       Should call  plt_sem_destroy to free the semaphore context

*/
uint32_t     plt_sem_init(void **context)
{
    int       retries;
    int       random_fd;
    uint16_t  rand_num;
    sem_ctx_t *sem = (sem_ctx_t *)malloc(sizeof(sem_ctx_t));

    if (!sem)
        return 0;

    retries = 200;

    random_fd = open("/dev/urandom", O_RDONLY);
    if (random_fd == -1)
    {
        free(sem);
        return 0;
    }

    while (retries-- > 0)
    {
        //Generate random semaphore name
        if (read(random_fd, &rand_num, 2) != 2)
        {
            free(sem);
            close(random_fd);
            return 0;
        }
        sprintf(sem->name, "/s%u", rand_num);

        //Initialize semaphore
        sem->sem = sem_open(sem->name, O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 0);

        if (sem->sem != SEM_FAILED)
        {   //Successful
            *context = sem;
            close(random_fd);
            return 1;
        }
        else
        {
            if (errno == EEXIST)
            {   //The semaphore has already existed, try again with different name
                continue;
            }
            //other error, stop retry
            //perror("plt_sem_init");
            break;
        }
    }
    free(sem);
    close(random_fd);
    return 0;
}

/**
plt_sem_wait - Wait until the semaphore is signaled
@param[in] context     The semaphore context
@return
@pre       	The semaphore context must be initialized by plt_sem_init
*/
void     plt_sem_wait(void *context)
{
    sem_ctx_t *sem = (sem_ctx_t *)context;
    sem_wait(sem->sem);
}

/**
plt_sem_post - Increment the semaphore count by 1
@param[in] context     The semaphore context
@return
@pre       	The semaphore context must be initialized by plt_sem_init
*/
void     plt_sem_post(void *context)
{
    sem_ctx_t *sem = (sem_ctx_t *)context;
    sem_post(sem->sem);
}

/**
plt_sem_destroy - Destroy a semaphore
@param[in] context     The semaphore context
@return
@pre       	The semaphore context must be initialized by plt_sem_init
*/
void     plt_sem_destroy(void *context)
{
    sem_ctx_t *sem = (sem_ctx_t *)context;
    int ret;
    ret = sem_close(sem->sem);
    if (ret < 0)
    {
        //perror("sem_close");
    }

    ret = sem_unlink(sem->name);
    if (ret < 0)
    {
//      char err_str[80];
//      sprintf(err_str, "sem_unlink->%s:",sem->name);
//      perror(err_str);
    }
    free(context);
}

#else
/**
plt_sem_init - Init a semaphore
@param[in, out] context     Pointer to a void * for storing the semaphore context
@return     Return non-zero indicates success, zero indicates failure.
@post       Should call  plt_sem_destroy to free the semaphore context

*/
uint32_t     plt_sem_init(void **context)
{
    sem_t *sem = (sem_t *)malloc(sizeof(sem_t));

    if (!sem)
        return 0;

    //Initialize semaphore
    if (sem_init (sem, 0, 0) != -1)
    {   //Successful
        *context = sem;
        return 1;
    }
    else
    {
    	//perror("plt_sem_init");
    }
    free(sem);
    return 0;
}

/**
plt_sem_wait - Wait until the semaphore is signaled
@param[in] context     The semaphore context
@return
@pre       	The semaphore context must be initialized by plt_sem_init
*/
void     plt_sem_wait(void *context)
{
    sem_wait((sem_t *)context);
}

/**
plt_sem_post - Increment the semaphore count by 1
@param[in] context     The semaphore context
@return
@pre       	The semaphore context must be initialized by plt_sem_init
*/
void     plt_sem_post(void *context)
{
    sem_post((sem_t *)context);
}

/**
plt_sem_destroy - Destroy a semaphore
@param[in] context     The semaphore context
@return
@pre       	The semaphore context must be initialized by plt_sem_init
*/
void     plt_sem_destroy(void *context)
{
    sem_destroy((sem_t *)context);
    free(context);
}
#endif

/**
plt_cond_init - Initialize condition variables
@param[out] context     Condition variables context
@return     Return non-zero indicates success, zero indicates failure.
@post       Should call  plt_cond_destroy to free the context

*/
int     plt_cond_init(void **context)
{
    pthread_cond_t  *cv = (pthread_cond_t  *)malloc(sizeof(pthread_cond_t));

    if (!cv)
        return 0;

    if (pthread_cond_init(cv, NULL) == 0)
    {
        *context = cv;
        return 1;//o.k.
    }
    else
    {
        free(cv);
        return 0;
    }
}

/**
plt_cond_timedwait - Wait on a condition
@param[in] cond_ctx     The condition variables context
@param[in] mtx          The mutex which must be acquired (locked) or else undefined behavior results
@param[in] timeout_ms   Wait timeout in milliseconds
@return     0=signal received (wait succeeded); 1=timeout; 2=failed
@pre       	The cond_ctx context must be initialized by plt_cond_init
*/
int     plt_cond_timedwait(void *cond_ctx, struct plt_mtx_t *mtx, uint16_t timeout_ms)
{
    int             ret;
    pthread_cond_t  *cv = (pthread_cond_t  *)cond_ctx;
    pthread_mutex_t *mutex = (pthread_mutex_t  *)mtx;
    struct timespec ts;
    uint32_t        nanosec;
#ifdef OS_MAC_X
    struct timeval  tv;
    gettimeofday(&tv, NULL);
    ts.tv_sec = tv.tv_sec;
    ts.tv_nsec = tv.tv_usec * 1000; //convert us to ns
#else
    clock_gettime(CLOCK_REALTIME, &ts);
#endif

    //Add the realtime clock seconds part
    ts.tv_sec += (timeout_ms / 1000);

    //Add the realtime clock nano seconds part
    nanosec = (timeout_ms % 1000) * 1000000; //convert ms to ns

    nanosec += ts.tv_nsec;

    if (nanosec >= 1000000000)
    {   //Overflow, add 1 second
        ts.tv_sec++;
        nanosec -= 1000000000;
    }

    ts.tv_nsec = nanosec;

    if ((ret = pthread_cond_timedwait(cv, mutex, &ts)) == 0)
    {
        return 0;
    }

    if (ret == ETIMEDOUT)
    {
        return 1;
    }

    return 2; //error

}

/**
plt_cond_broadcast - Unblock all threads currently blocked on the specified condition variable
@param[in] cond_ctx     The condition variables context
@return
@pre       	The cond_ctx context must be initialized by plt_cond_init
*/
void     plt_cond_broadcast(void *cond_ctx)
{
    pthread_cond_t  *cv = (pthread_cond_t  *)cond_ctx;

    pthread_cond_broadcast(cv);
}

/**
plt_cond_signal - Unblock at least one of the threads that are blocked on the specified condition variable
@param[in] cond_ctx     The condition variables context
@return
@pre       	The cond_ctx context must be initialized by plt_cond_init
*/
void     plt_cond_signal(void *cond_ctx)
{
    pthread_cond_t  *cv = (pthread_cond_t  *)cond_ctx;

    pthread_cond_signal(cv);

}

/**
plt_cond_destroy - Destroy condition variables
@param[in] cond_ctx     The condition variables context
@return
@pre       	The cond_ctx context must be initialized by plt_cond_init
*/
void     plt_cond_destroy(void *cond_ctx)
{
    pthread_cond_t  *cv = (pthread_cond_t  *)cond_ctx;

    pthread_cond_destroy(cv);
    free(cv);
}

/**
plt_thrd_run - Run a thread
@param[in] args         Thread context.
@return     NULL on completion.
*/
static void *plt_thrd_run(void *args)
{
    thrd_ctx_t      *thrd_ctx = (thrd_ctx_t *)args;
    thrd_ctx->start_adr(thrd_ctx->args);
    free(thrd_ctx);
    return NULL;
}

/**
plt_thrd_create - Create a thread and run it
@param[in] start_adr    Pointer to a function for the newly created thread to execute.
@param[in] args         The argument passed to the newly created thread function.
@return     Return zero indicates success, negative number indicates failure.
*/
int     plt_thrd_create(void(*start_adr)( void * ), void *args)
{
    thrd_ctx_t      *thrd_ctx;
    pthread_t       thrd;
    pthread_attr_t  attrib;

    thrd_ctx = (thrd_ctx_t *)malloc(sizeof(thrd_ctx_t));
    if (!thrd_ctx)
    {
        return -1;
    }
    thrd_ctx->start_adr = start_adr;
    thrd_ctx->args = args;

    pthread_attr_init(&attrib);
    pthread_attr_setdetachstate(&attrib, PTHREAD_CREATE_DETACHED);

    if (pthread_create(&thrd, &attrib, plt_thrd_run, thrd_ctx) == 0)
    {   //Create thread succeeded
        return 0;
    }
    return -2;
}

#ifndef OS_ANDROID
/**
plt_cancel_thrd_join - Join a cancelable thread and free it resources
@param[in] context     Thread context
@return     Return 0 if thread terminated normally; 1 if thread was canceled; else negative number indicates failure.
*/
int     plt_cancel_thrd_join(void *context)
{
    pthread_t   *thrd_hdlr = (pthread_t *)context;
    int         res;
    void        *ret_code;

    res = pthread_join(*thrd_hdlr, &ret_code);
    free(context);

    if (res != 0)
    {
        //printf("Error joining thread = %d\n", res);
        return -1;
    }

    if (ret_code == PTHREAD_CANCELED)
    {
        return 1;
    }
    return 0;
}

/**
plt_cancel_thrd_exit - Force a cancelable thread to exit
@param[in] context     Thread context
@return     Return zero indicates success, negative number indicates failure.
*/
int     plt_cancel_thrd_exit(void *context)
{
    pthread_t   *thrd_hdlr = (pthread_t *)context;
    int         res;

    res = pthread_cancel(*thrd_hdlr);
    if (res != 0)
    {
        //printf("Error canceling thread = %d\n", res);
        return -1;
    }
    return 0;
}

/**
plt_cancel_thrd_run - Run a cancelable thread
@param[in] args         Thread context.
@return     NULL on completion.
*/
static void *plt_cancel_thrd_run(void *args)
{
    thrd_ctx_t      *thrd_ctx = (thrd_ctx_t *)args;
    void (*thrd_fn)(void * );
    void *param;
    int  old_type;

    thrd_fn = thrd_ctx->start_adr;
    param = thrd_ctx->args;
    free(thrd_ctx);

    //Set cancel type as asynchronous
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &old_type);

    thrd_fn(param);
    return NULL;
}

/**
plt_cancel_thrd_create - Create a cancelable thread and run it
@param[in] start_adr    Pointer to a function for the newly created thread to execute.
@param[in] args         The argument passed to the newly created thread function.
@param[out] context     Thread context
@return     Return zero indicates success, negative number indicates failure.
@post       Should call  plt_cancel_thrd_join() to free the thread context
*/
int     plt_cancel_thrd_create(void(*start_adr)( void * ), void *args, void **context)
{
    thrd_ctx_t      *thrd_ctx;
    pthread_t       *thrd_hdlr = (pthread_t  *)malloc(sizeof(pthread_t));

    if (!thrd_hdlr)
    {
        return -1;
    }

    thrd_ctx = (thrd_ctx_t *)malloc(sizeof(thrd_ctx_t));
    if (!thrd_ctx)
    {
        free(thrd_hdlr);
        return -1;
    }
    thrd_ctx->start_adr = start_adr;
    thrd_ctx->args = args;

    if (pthread_create(thrd_hdlr, NULL, plt_cancel_thrd_run, thrd_ctx) == 0)
    {   //Create thread succeeded
        *context = thrd_hdlr;
        return 0;
    }

    free(thrd_hdlr);
    free(thrd_ctx);
    return -2;
}
#endif

/**
plt_sleep - Suspends the execution of the current thread until the time-out interval elapses.
@param[in] tmout_ms   The time interval for which execution is to be suspended, in milliseconds.
@return
*/
void     plt_sleep(uint32_t  tmout_ms)
{
    struct timespec timeout_val;

    // Calculate timeout
    timeout_val.tv_sec = tmout_ms / 1000;
    timeout_val.tv_nsec = (tmout_ms % 1000) * 1000000; //convert ms to ns

    nanosleep(&timeout_val, NULL);
}


/**
plt_periodic_start - Start a periodic timer
@param[in] pltfm_ctx    Context
@param[in] tmout_ms     Periodic timeout value in milliseconds
@param[in] tmout_cb     Pointer to the timeout callback function which executes a short task only
@param[in] data         The data that will be passed back as parameter when the callback function is invoked
@return     Return a pointer to the internal timer context, NULL indicates failure.
@pre        Must call plt_init once before using all timer related functions
@post 		Must call plt_tmr_stop to release the timer resource even if the timer has expired.
@note       The periodic callback function (tmout_cb) MUST NOT sleep or be blocked
            (e.g. calling plt_sem_wait, plt_mtx_lck, etc) in order to avoid deadlock
*/
void    *plt_periodic_start(plt_ctx_t *pltfm_ctx, uint32_t  tmout_ms, tmr_cb_t  tmout_cb, void *data)
{
    return plt_tmr_create(pltfm_ctx, tmout_ms, tmout_cb, data, 1);
}


/**
tmr_tck_thrd - thread to generate timer tick
@param[in]	data		Context
@return
*/
static void tmr_tck_thrd(void   *data)
{
    plt_ctx_t *plt_ctx = (plt_ctx_t *)data;
    struct timespec timeout_val;

    plt_ctx->tmr_tck_thrd_sts = 1;

    // Calculate timeout
    timeout_val.tv_sec = 0;
    timeout_val.tv_nsec = PLT_TIMER_RESOLUTION * 1000000; //convert ms to ns

    while (plt_ctx->tmr_tck_thrd_run)
    {
        nanosleep(&timeout_val, NULL);
        //Update timer tick
        plt_ctx->tmr_tick++;

        //Send timer tick event
        plt_sem_post(plt_ctx->tmr_sem);

    }

    plt_ctx->tmr_tck_thrd_sts = 0;

}

/**
tmr_expr_cb_thrd - Timer expire callback thread
@param[in] args         Thread context.
@return
*/
static void tmr_expr_cb_thrd(void *args)
{
    thrd_ctx_t      *thrd_ctx = (thrd_ctx_t *)args;
    thrd_ctx->start_adr(thrd_ctx->args);
    free(thrd_ctx);
}

/**
tmr_chk_thrd - thread to process timer tick event
@param[in]	data		Context
@return
*/
static void tmr_chk_thrd(void   *data)
{
    plt_ctx_t       *plt_ctx = (plt_ctx_t *)data;
    util_lst_t      *prev_ent;     //Pointer to previous list entry
    util_lst_t      *temp;
    tmr_ctx_t       *tmr_ctx;
    tmr_cb_t        tmr_cb;         //The callback function to call when timer expires
    void            *cb_prm;        //The data passed as parameter to the callback function

    plt_ctx->tmr_chk_thrd_sts = 1;

    while (1)
    {
        //Wait for timer tick event
        plt_sem_wait(plt_ctx->tmr_sem);

        //Check whether to exit the thread
        if (plt_ctx->tmr_chk_thrd_run == 0)
        {
            plt_ctx->tmr_chk_thrd_sts = 0;
            return;
        }

        //Check each of the timer contexts for expiry
        plt_mtx_lck(plt_ctx->tmr_mtx);

        if (plt_ctx->tmr_lst_hd == NULL)
        {   //Nothing to remove
            plt_mtx_ulck(plt_ctx->tmr_mtx);
            continue;
        }

        temp = plt_ctx->tmr_lst_hd;
        prev_ent = NULL;

        while (temp)
        {
            tmr_ctx = (tmr_ctx_t *)temp->wr_buf;

            if (util_tmr_exp_chk32(plt_ctx->tmr_tick, tmr_ctx->tmr_expiry))
            {   //Timer expired
                thrd_ctx_t      *thrd_ctx;
                int             periodic;   //flag whether timer is periodic

                tmr_cb = tmr_ctx->tmr_cb;
                cb_prm = tmr_ctx->data;

                //Check whether to reload timer expiry value for periodic timer
                if (tmr_ctx->tmr_reload > 0)
                {   //Periodic timer
                    periodic = 1;
                    tmr_ctx->tmr_expiry = plt_ctx->tmr_tick + tmr_ctx->tmr_reload;
                }
                else
                {   //One shot timer, remove it.
                    periodic = 0;
                    if (!prev_ent)
                    {   //No previous entry, this is the head of list
                        plt_ctx->tmr_lst_hd = temp->next;
                    }
                    else
                    {   //Assign the next entry to the previous entry
                        prev_ent->next = temp->next;
                    }
                    free(temp);

                }

                //Callback timer callback function
                //DON'T hold the lock as this could cause deadlock if timer callback function calls
                //plt_sem_wait() etc. that may sleep.
                plt_mtx_ulck(plt_ctx->tmr_mtx);
                if (periodic)
                {
                    //IMPORTANT: Make sure the periodic callback function does not sleep/block
                    //          (by calling plt_sem_wait, plt_mtx_lck, etc) to avoid deadlock
                    tmr_cb(cb_prm);
                }
                else
                {
                    //Create new thread for every callback to avoid deadlock
                    thrd_ctx = (thrd_ctx_t *)malloc(sizeof(thrd_ctx_t));
                    if (thrd_ctx)
                    {
                        thrd_ctx->start_adr = tmr_cb;
                        thrd_ctx->args = cb_prm;
                        plt_thrd_create(tmr_expr_cb_thrd, thrd_ctx);
                    }
                }
                plt_mtx_lck(plt_ctx->tmr_mtx);

                //Start checking from the list head again since the timer callback function
                //may have changed the timer list.
                temp = plt_ctx->tmr_lst_hd;
                prev_ent = NULL;
                continue;
            }
            prev_ent = temp;
            temp = temp->next;
        }

        plt_mtx_ulck(plt_ctx->tmr_mtx);
    }
}

#ifdef  TRUNCATE_ON
/**
plt_utf32_to_8 - Converts Unicode UTF-32 text to Unicode UTF-8
@param[in]	utf32_src   Source location having UTF32 text
@param[in]	utf32_len   Number of utf32 characters in utf32_src
@param[in]	utf8_out	Output buffer for UTF8 text terminated by '\0'
@param[in]	out_buf_len	Output buffer length in bytes
@return     Return the number of UTF8 characters (exclude NULL character); negative number indicates failure.
*/
static int plt_utf32_to_8(const uint32_t *utf32_src, uint8_t utf32_len, uint8_t *utf8_out, uint8_t out_buf_len)
{
    uint32_t    utf32_ch;
    int         i;
    int         j;
    int         max_output_len;

    if (!utf8_out || (out_buf_len == 0))
    {
        return -1;
    }

/*
    bytes | bits | representation
     1 |    7 | 0vvvvvvv
     2 |   11 | 110vvvvv 10vvvvvv
     3 |   16 | 1110vvvv 10vvvvvv 10vvvvvv
     4 |   21 | 11110vvv 10vvvvvv 10vvvvvv 10vvvvvv
*/
    j = 0;
    max_output_len = out_buf_len - 1; //exclude terminating NULL char.
    for (i=0; i<utf32_len; i++)
    {
        utf32_ch = utf32_src[i];

        if (utf32_ch < 0x80)
        {
            if ((j + 1) > max_output_len)
                break;
            utf8_out[j++] = (uint8_t) utf32_ch;
        }
        else if (utf32_ch < 0x800)
        {
            if ((j + 2) > max_output_len)
                break;
            utf8_out[j++] = (0xC0 | (utf32_ch>>6));
            utf8_out[j++] = (0x80 | (utf32_ch & 0x3F));
        }
        else if (utf32_ch < 0x10000)
        {
            if ((j + 3) > max_output_len)
                break;
            utf8_out[j++] = (0xE0 | (utf32_ch>>12));
            utf8_out[j++] = (0x80 | ((utf32_ch>>6) & 0x3F));
            utf8_out[j++] = (0x80 | (utf32_ch & 0x3F));
        }
        else if (utf32_ch < 0x200000)
        {
            if ((j + 4) > max_output_len)
                break;
            utf8_out[j++] = (0xF0 | (utf32_ch>>18));
            utf8_out[j++] = (0x80 | ((utf32_ch>>12) & 0x3F));
            utf8_out[j++] = (0x80 | ((utf32_ch>>6) & 0x3F));
            utf8_out[j++] = (0x80 | (utf32_ch & 0x3F));
        }
    }
    utf8_out[j] = '\0';

    return j;
}

#else
/**
plt_utf32_to_8 - Converts Unicode UTF-32 text to Unicode UTF-8
@param[in]	utf32_src   Source location having UTF32 text
@param[out]	utf8_out	Output buffer for UTF8 text terminated by '\0'
@param[in]	utf32_len   Number of utf32 characters in utf32_src
@return     Return the number of UTF8 characters (exclude NULL character); negative number indicates failure.
@post       The caller of this function should free utf8_out if return value is > zero.
*/
static int plt_utf32_to_8(const uint32_t *utf32_src, uint8_t **utf8_out, uint8_t utf32_len)
{
    uint8_t     *utf8_ptr;
    uint32_t    utf32_ch;
    int         i;
    int         j;

    //Allocate memory for the utf8 output, assuming each utf16 characters can be converted
    //to maximum 4 utf8 characters for the worst case.
    utf8_ptr = (uint8_t *) malloc((utf32_len * 4) + 1);
    if (!utf8_ptr)
    {
        return -1;
    }

/*
    bytes | bits | representation
     1 |    7 | 0vvvvvvv
     2 |   11 | 110vvvvv 10vvvvvv
     3 |   16 | 1110vvvv 10vvvvvv 10vvvvvv
     4 |   21 | 11110vvv 10vvvvvv 10vvvvvv 10vvvvvv
*/
    j = 0;
    for (i=0; i<utf32_len; i++)
    {
        utf32_ch = utf32_src[i];

        if (utf32_ch < 0x80)
        {
            utf8_ptr[j++] = (uint8_t) utf32_ch;
        }
        else if (utf32_ch < 0x800)
        {
            utf8_ptr[j++] = (0xC0 | (utf32_ch>>6));
            utf8_ptr[j++] = (0x80 | (utf32_ch & 0x3F));
        }
        else if (utf32_ch < 0x10000)
        {
            utf8_ptr[j++] = (0xE0 | (utf32_ch>>12));
            utf8_ptr[j++] = (0x80 | ((utf32_ch>>6) & 0x3F));
            utf8_ptr[j++] = (0x80 | (utf32_ch & 0x3F));
        }
        else if (utf32_ch < 0x200000)
        {
            utf8_ptr[j++] = (0xF0 | (utf32_ch>>18));
            utf8_ptr[j++] = (0x80 | ((utf32_ch>>12) & 0x3F));
            utf8_ptr[j++] = (0x80 | ((utf32_ch>>6) & 0x3F));
            utf8_ptr[j++] = (0x80 | (utf32_ch & 0x3F));
        }
    }
    utf8_ptr[j] = '\0';
    *utf8_out = utf8_ptr;

    return j;
}
#endif


/**
plt_utf16_to_32 - Converts Unicode UTF-16 text to Unicode UTF-32
@param[in]	utf16_src   Source location having UTF16 text
@param[out]	utf32_out	Output buffer for UTF32 text terminated by '\0'
@param[in]	utf16_len   Number of utf16 characters in utf16_src
@return     Return the number of UTF32 characters (exclude NULL character); negative number indicates failure.
@post       The caller of this function should free utf32_out if return value is > zero.
*/
static int plt_utf16_to_32(const uint16_t *utf16_src, uint32_t **utf32_out, uint8_t utf16_len)
{
    uint32_t    *utf32_ptr;
    int         utf32_len;
    int         i;
    int         j;
    int         found_hi_surg;  //Flag to indicate high surrogate found
    uint32_t    hi_surg = 0;        //High surrogate
    //uint32_t    SURROGATE_OFFSET = 0x10000 - (((uint32_t)0xD800) << 10) - 0xDC00;
#define     SURROGATE_OFFSET    0xFCA02400

    //Check for error in utf16 source and calculate number of  utf32 output characters
    found_hi_surg = 0;
    utf32_len = 0;

    for (i=0; i<utf16_len; i++)
    {
        if (utf16_src[i] == 0xFFFE || utf16_src[i] == 0xFFFF)
        {
            return -1;
        }

        if (utf16_src[i] >= 0xFDD0 && utf16_src[i] <= 0xFDEF)
        {
            return -1;
        }

        //Check for high surrogate
        if (utf16_src[i] >= 0xD800 && utf16_src[i] <= 0xDBFF)
        {
            found_hi_surg = 1;
        }

        //Check for low surrogate
        else if (utf16_src[i] >= 0xDC00 && utf16_src[i] <= 0xDFFF)
        {
            if (!found_hi_surg)
            {   //No preceding high surrogate
                return -1;
            }
            //Found preceding high surrogate
            found_hi_surg = 0;
            utf32_len++;

        }
        else
        {
            if (found_hi_surg)
            {   //High surrogate is not followed by a low surrogate
                return -1;
            }
            utf32_len++;
        }
    }

    //Allocate memory for output
    utf32_ptr = (uint32_t *)calloc(utf32_len + 1, sizeof(uint32_t));
    if (!utf32_ptr)
    {
        return -2;
    }

    //Convert utf16 to utf32
    j = 0;
    for (i=0; i<utf16_len; i++)
    {
        //Check for high surrogate
        if (utf16_src[i] >= 0xD800 && utf16_src[i] <= 0xDBFF)
        {
            hi_surg = utf16_src[i];
        }

        //Check for low surrogate
        else if (utf16_src[i] >= 0xDC00 && utf16_src[i] <= 0xDFFF)
        {

            utf32_ptr[j++] = (hi_surg << 10) + utf16_src[i] + SURROGATE_OFFSET;

        }
        else
        {
            utf32_ptr[j++] = (uint32_t) utf16_src[i];
        }
    }

    *utf32_out = utf32_ptr;

    return j;
}


/**
plt_utf16_to_8 - Converts Unicode UTF-16 text to Unicode UTF-8
@param[in]	utf16_src   Source location having UTF16 text terminated by L'\0'
@param[in]	utf8_output	Output buffer for UTF8 text terminated by '\0'
@param[in]	out_buf_len	Output buffer length in bytes
@param[in]	big_endian	UTF16 endianness; 1=big-endian, 0=little-endian
@return     Return zero indicates success, negative number indicates failure.
*/
int plt_utf16_to_8(const uint16_t *utf16_src, uint8_t *utf8_output, uint8_t out_buf_len, uint8_t big_endian)
{
    int             utf8_len;
    int             utf16_len;
    int             utf32_len;
    uint16_t        *utf16_buf;
    uint32_t        *utf32_buf;
    const uint16_t  *utf16_ptr = utf16_src;

    if ((utf16_src == NULL) || (*utf16_src == 0))
    {
        return -1;
    }

    //Calculate number of UTF16 characters
    utf16_len = 0;
    while (*utf16_ptr++)
    {
        utf16_len++;
    }

    //Make a working copy from the utf16 source
    utf16_buf = (uint16_t *)calloc(utf16_len + 1, sizeof(uint16_t));

    if (!utf16_buf)
    {
        return -2;
    }

    memcpy(utf16_buf, utf16_src, utf16_len * sizeof(uint16_t));

    //Convert byte order if host and  UTF-16 input string are different in endianess
	if (util_is_host_big_endian() != big_endian)
    {
        int         i;
        uint16_t    *word_ptr;

        word_ptr = (uint16_t *)utf16_buf;

        for (i=0; i<utf16_len; i++)
        {
            *word_ptr = (*word_ptr << 8) | (*word_ptr >> 8);
            word_ptr++;
        }
    }

    //Convert utf16 to utf32
    utf32_len = plt_utf16_to_32(utf16_buf, &utf32_buf, utf16_len);

    free(utf16_buf);

    if (utf32_len < 0)
    {
        return -3;
    }

    //Convert utf32 to utf8
    utf8_len = plt_utf32_to_8(utf32_buf, utf32_len, utf8_output, out_buf_len);

    free(utf32_buf);

    if (utf8_len < 0)
    {
        return -4;
    }

    return 0;

}


#ifdef TCP_PORTAL
/**
plt_init - Initialize platform
@param[in] pltfm_ctx        Context
@param[in] display_txt_fn   Function pointer to display null terminated string
@param[in] display_ctx      Display context
@return    Return zero on success; negative error number indicates failure.
*/
int plt_init(plt_ctx_t *pltfm_ctx, print_fn display_txt_fn, void *display_ctx)
#else
/**
plt_init - Initialize platform
@param[in] pltfm_ctx        Context
@param[in] display_txt_fn   Function pointer to display null terminated string
@return    Return zero on success; negative error number indicates failure.
*/
int plt_init(plt_ctx_t *pltfm_ctx, print_fn display_txt_fn)
#endif
{
    if (pltfm_ctx->init_done > 0)
    {
        //Update initialization count
        pltfm_ctx->init_done++;

        return ZWHCI_NO_ERROR;
    }

    pltfm_ctx->print_txt = display_txt_fn;
#ifdef TCP_PORTAL
    pltfm_ctx->display_ctx = display_ctx;
#endif


    if (!plt_mtx_init(&pltfm_ctx->tmr_mtx))
        return ZWHCI_ERROR_RESOURCE;

    if (!plt_sem_init(&pltfm_ctx->tmr_sem))
    {
        goto l_PLATFORM_INIT_ERROR;
    }

    //Init random number seed
    srand(time(NULL));

    //Start timer tick thread
    pltfm_ctx->tmr_tck_thrd_run = 1;
    if (plt_thrd_create(tmr_tck_thrd, pltfm_ctx) < 0)
    {
        goto l_PLATFORM_INIT_ERROR1;
    }

    //Start timer check thread
    pltfm_ctx->tmr_chk_thrd_run = 1;
    if (plt_thrd_create(tmr_chk_thrd, pltfm_ctx) < 0)
    {
        goto l_PLATFORM_INIT_ERROR2;
    }

    //Done. Update status
    pltfm_ctx->init_done = 1;
    return 0;

l_PLATFORM_INIT_ERROR2:
    //Stop the timer tick thread
    pltfm_ctx->tmr_tck_thrd_run = 0;
    while (pltfm_ctx->tmr_tck_thrd_sts)
    {
        plt_sleep(100);
    }

l_PLATFORM_INIT_ERROR1:
    plt_sem_destroy(pltfm_ctx->tmr_sem);

l_PLATFORM_INIT_ERROR:
    plt_mtx_destroy(pltfm_ctx->tmr_mtx);

    return ZWHCI_ERROR_RESOURCE;

}


/**
plt_exit - Clean up the platform
@param[in] pltfm_ctx    Context
@return
*/
void plt_exit(plt_ctx_t *pltfm_ctx)
{
    //Update initialization count
    pltfm_ctx->init_done--;

    if (pltfm_ctx->init_done > 0)
    {   //There are some threads still using this platform functions, don't clean up
        return;
    }

    //Stop all the threads
    pltfm_ctx->tmr_tck_thrd_run = 0;
    pltfm_ctx->tmr_chk_thrd_run = 0;
    plt_sem_post(pltfm_ctx->tmr_sem);

    while (pltfm_ctx->tmr_tck_thrd_sts)
    {
        plt_sleep(100);
    }

    while (pltfm_ctx->tmr_chk_thrd_sts)
    {
        plt_sleep(100);
    }

    plt_sleep(20);  //delay 20ms to give timer threads enough time to terminate and clean up
    util_list_flush(pltfm_ctx->tmr_mtx, &pltfm_ctx->tmr_lst_hd);
    plt_mtx_destroy(pltfm_ctx->tmr_mtx);
    plt_sem_destroy(pltfm_ctx->tmr_sem);
    memset(pltfm_ctx, 0, sizeof(plt_ctx_t));
}



#endif //#ifdef OS_LINUX

/**
@}
*/



