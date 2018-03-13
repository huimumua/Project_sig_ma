/**
@file   zip_platform.h - Z/IP interface platform specific implementation header file.

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

#ifndef _ZIP_PLATFORM_DAVID_
#define _ZIP_PLATFORM_DAVID_

#include "zip_util.h"

/**
@defgroup Platform Multi-platform APIs
Some common functions in multi-platform environment.
@ingroup zwarecapi
@{
*/

///
/// Platform timer resolution in milliseconds
#define PLT_TIMER_RESOLUTION    100

#if defined(_WINDOWS) || defined(WIN32)
///
/// Use safe version of functions like sprintf_s
#define USE_SAFE_VERSION
#endif

///
/// Maximum message length for displaying message
#define MAX_PLT_MSG_LEN     256

///
/// Maximum number of binary bytes for displaying in a line
#define MAX_BIN_LINE_LEN    10

///
/// Timer callback function
typedef void    (*tmr_cb_t)(void *data);

///
/// Print text function
#ifdef TCP_PORTAL
typedef void    (*print_fn)(void *msg, void *display_ctx);
#else
typedef void    (*print_fn)(void *msg);
#endif

#if defined(_WINDOWS) || defined(WIN32)
struct plt_mtx_t
{
    HANDLE  handle;
};                    ///< Mutex type for multi-platform support

///Platform context
typedef struct
{
    print_fn            print_txt;      ///< Print text function
    int                 init_done;      ///< Counter to indicated how many times platform initialization has been invoked
    #ifdef TCP_PORTAL
    void                *display_ctx;   ///< Display context
    #endif
} plt_ctx_t;


#elif defined(OS_LINUX)

struct plt_mtx_t;     ///< Mutex type for multi-platform support

///Platform context
typedef struct
{
    volatile uint32_t   tmr_tick;       ///< Timer tick every PLT_TIMER_RESOLUTION
    uint16_t            id_gen;         ///< One-shot Timer identifier generator (range 1 to 0x7FFF)
    uint16_t            per_id_gen;     ///< Periodic Timer identifier generator (range 0x8000 to 0xFFFF)
    int                 init_done;      ///< Counter to indicated how many times platform initialization has been invoked
    struct plt_mtx_t    *tmr_mtx;       ///< mutex for accessing timer list
    void                *tmr_sem;       ///< semaphore for waiting timer tick event
    util_lst_t          *tmr_lst_hd;    ///< head of linked list for timers
    volatile int        tmr_tck_thrd_run;   ///< control the timer tick thread whether to run. 1 = run, 0 = stop
    volatile int        tmr_tck_thrd_sts;   ///< timer tick thread status. 1 = running, 0 = thread exited
    volatile int        tmr_chk_thrd_run;   ///< control the timer check thread whether to run. 1 = run, 0 = stop
    volatile int        tmr_chk_thrd_sts;   ///< timer check thread status. 1 = running, 0 = thread exited
    print_fn            print_txt;      ///< Print text function
    #ifdef TCP_PORTAL
    void                *display_ctx;   ///< Display context
    #endif

} plt_ctx_t;


#endif

void        plt_msg_show(void *pltfm_ctx, char * format, ...);
/**<
show message to the user
@param[in] pltfm_ctx    Context
@param[in] format       The formating string as in call to printf.
@param[in] ...          Variable argument list.
@return
*/

void        plt_msg_ts_show(void *pltfm_ctx, char * format, ...);
/**<
show message to the user with time stamp
@param[in] pltfm_ctx    Context
@param[in] format       The formating string as in call to printf.
@param[in] ...          Variable argument list.
@return
*/

void        plt_bin_show(void *pltfm_ctx, void * buf, uint32_t len);
/**<
show binary string to the user
@param[in] pltfm_ctx    Context
@param[in] buf          The buffer that stores the binary string
@param[in] len          The length of the binary string.
@return
*/

void        *plt_tmr_start(plt_ctx_t *pltfm_ctx, uint32_t  tmout_ms, tmr_cb_t  tmout_cb, void *data);
/**<
Start a timer
@param[in] pltfm_ctx    Context
@param[in] tmout_ms     Timeout value in milliseconds
@param[in] tmout_cb     Pointer to the timeout callback function
@param[in] data         The data that will be passed back as parameter when the callback function is invoked
@return     Return a pointer to the internal timer context, NULL indicates failure.
@post 		Must call plt_tmr_stop to release the timer resource even if the timer has expired.

*/

uint32_t    plt_tmr_stop(plt_ctx_t *pltfm_ctx, void	*context);
/**<
Stop the timer and release timer resource
@param[in] pltfm_ctx    Context
@param[in] context     The timer context returned from the plt_tmr_start
@return     Return non-zero indicates success, zero indicates failure.
@post		The caller should not use the timer context after this call.
*/

uint32_t    plt_mtx_init(struct plt_mtx_t **context);
/**<
Initialize a recursive mutex
@param[in, out] context     Pointer to a void * for storing the mutex context
@return     Return non-zero indicates success, zero indicates failure.
@post       Should call  plt_mtx_destroy to free the mutex context
*/

void        plt_mtx_destroy(struct plt_mtx_t *context);
/**<
Destroy a mutex
@param[in] context     The mutex context
@return
@pre       The mutex context must be initialized by plt_mtx_init
*/

int         plt_mtx_trylck(struct plt_mtx_t *context);
/**<
Try to lock a mutex
@param[in] context     The mutex context
@return     Zero if a lock on the mutex object referenced by mutex is acquired.
            Otherwise, an error number is returned to indicate the error.
@pre       The mutex context must be initialized by plt_mtx_init
*/

void        plt_mtx_lck(struct plt_mtx_t *context);
/**<
Lock a mutex
@param[in] context     The mutex context
@return
@pre       The mutex context must be initialized by plt_mtx_init
*/

void        plt_mtx_ulck(struct plt_mtx_t *context);
/**<
Unlock a mutex
@param[in] context     The mutex context
@return
@pre       The mutex context must be initialized by plt_mtx_init
*/

uint32_t    plt_sem_init(void **context);
/**<
Init a semaphore
@param[in, out] context     Pointer to a void * for storing the semaphore context
@return     Return non-zero indicates success, zero indicates failure.
@post       Should call  plt_sem_destroy to free the semaphore context
*/

void        plt_sem_destroy(void *context);
/**<
Destroy a semaphore
@param[in] context     The semaphore context
@return
@pre       	The semaphore context must be initialized by plt_sem_init
*/

void        plt_sem_wait(void *context);
/**<
Wait until the semaphore is signaled
@param[in] context     The semaphore context
@return
@pre       	The semaphore context must be initialized by plt_sem_init
*/

void        plt_sem_post(void *context);
/**<
Increment the semaphore count by 1
@param[in] context     The semaphore context
@return
@pre       	The semaphore context must be initialized by plt_sem_init
*/

int         plt_cond_init(void **context);
/**<
Initialize condition variables
@param[out] context     Condition variables context
@return     Return non-zero indicates success, zero indicates failure.
@post       Should call  plt_cond_destroy to free the context
*/

int         plt_cond_timedwait(void *cond_ctx, struct plt_mtx_t *mtx, uint16_t timeout_ms);
/**<
Wait on a condition
@param[in] cond_ctx     The condition variables context
@param[in] mtx          The mutex which must be acquired (locked) or else undefined behavior results
@param[in] timeout_ms   Wait timeout in milliseconds
@return     0=signal received (wait succeeded); 1=timeout; 2=failed
@pre       	The cond_ctx context must be initialized by plt_cond_init
*/

void        plt_cond_broadcast(void *cond_ctx);
/**<
Unblock all threads currently blocked on the specified condition variable
@param[in] cond_ctx     The condition variables context
@return
@pre       	The cond_ctx context must be initialized by plt_cond_init
*/

void        plt_cond_signal(void *cond_ctx);
/**<
Unblock at least one of the threads that are blocked on the specified condition variable
@param[in] cond_ctx     The condition variables context
@return
@pre       	The cond_ctx context must be initialized by plt_cond_init
*/

void        plt_cond_destroy(void *cond_ctx);
/**<
Destroy condition variables
@param[in] cond_ctx     The condition variables context
@return
@pre       	The cond_ctx context must be initialized by plt_cond_init
*/

int         plt_thrd_create(void (*start_adr)( void * ), void *args);
/**<
Create a thread and run it
@param[in] start_adr    Pointer to a function for the newly created thread to execute.
@param[in] args         The argument passed to the newly created thread function.
@return     Return zero indicates success, negative number indicates failure.
*/

#ifdef OS_LINUX
int         plt_cancel_thrd_create(void(*start_adr)( void * ), void *args, void **context);
/**<
Create a cancelable thread and run it
@param[in] start_adr    Pointer to a function for the newly created thread to execute.
@param[in] args         The argument passed to the newly created thread function.
@param[out] context     Thread context
@return     Return zero indicates success, negative number indicates failure.
@post       Should call  plt_cancel_thrd_join() to free the thread context
*/

int         plt_cancel_thrd_exit(void *context);
/**<
Force a cancelable thread to exit
@param[in] context     Thread context
@return     Return zero indicates success, negative number indicates failure.
*/

int         plt_cancel_thrd_join(void *context);
/**<
Join a cancelable thread and free it resources
@param[in] context     Thread context
@return     Return 0 if thread terminated normally; 1 if thread was canceled; else negative number indicates failure.
*/
#endif

void        plt_sleep(uint32_t    tmout_ms);
/**<
Suspends the execution of the current thread until the time-out interval elapses.
@param[in] tmout_ms   The time interval for which execution is to be suspended, in milliseconds.
@return
*/

void        *plt_periodic_start(plt_ctx_t *pltfm_ctx, uint32_t  tmout_ms, tmr_cb_t  tmout_cb, void *data);
/**<
Start a periodic timer
@param[in] pltfm_ctx    Context
@param[in] tmout_ms     Periodic timeout value in milliseconds
@param[in] tmout_cb     Pointer to the timeout callback function which executes a short task only
@param[in] data         The data that will be passed back as parameter when the callback function is invoked
@return     Return a pointer to the internal timer context, NULL indicates failure.
@post 		Must call plt_tmr_stop to release the timer resource even if the timer has expired.
@note       The periodic callback function (tmout_cb) MUST NOT sleep or be blocked
            (e.g. calling plt_sem_wait, plt_mtx_lck, etc) in order to avoid deadlock
*/

#if defined(_WINDOWS) || defined(WIN32)
int         plt_utf16_to_8(const char *utf16_src, char *utf8_output, uint8_t out_buf_len, uint8_t big_endian);
/**<
Converts Unicode UTF-16 text to Unicode UTF-8
@param[in]	utf16_src   Source location having UTF16 text terminated by L'\0'
@param[out]	utf8_output	Output buffer for UTF8 text terminated by '\0'
@param[in]	out_buf_len	Output buffer length in bytes
@param[in]	big_endian	UTF16 endianness; 1=big-endian, 0=little-endian
@return     Return zero indicates success, negative number indicates failure.
*/
#else
int         plt_utf16_to_8(const uint16_t *utf16_src, uint8_t *utf8_output, uint8_t out_buf_len, uint8_t big_endian);
/**<
Converts Unicode UTF-16 text to Unicode UTF-8
@param[in]	utf16_src   Source location having UTF16 text terminated by L'\0'
@param[in]	utf8_output	Output buffer for UTF8 text terminated by '\0'
@param[in]	out_buf_len	Output buffer length in bytes
@param[in]	big_endian	UTF16 endianness; 1=big-endian, 0=little-endian
@return     Return zero indicates success, negative number indicates failure.
*/
#endif

int         plt_utf8_chk(const uint8_t *utf8, uint8_t utf8_len);
/**<
Count number of valid UTF-8 bytes
@param[in]	utf8	    Input buffer of UTF-8 text
@param[in]	utf8_len    UTF-8 text length
@return     Return the number of valid UTF-8 bytes.
*/

#ifdef TCP_PORTAL
int         plt_init(plt_ctx_t *pltfm_ctx, print_fn display_txt_fn, void *display_ctx);
/**<
Initialize platform
@param[in] pltfm_ctx        Context
@param[in] display_txt_fn   Function pointer to display null terminated string
@param[in] display_ctx      Display context
@return    Return zero on success; negative error number indicates failure.
*/
#else
int         plt_init(plt_ctx_t *pltfm_ctx, print_fn display_txt_fn);
/**<
Initialize platform
@param[in] pltfm_ctx        Context
@param[in] display_txt_fn   Function pointer to display null terminated string
@return    Return zero on success; negative error number indicates failure.
*/
#endif

void        plt_exit(plt_ctx_t *pltfm_ctx);
/**<
Clean up the platform
@param[in] pltfm_ctx    Context
@return
*/

int16_t     plt_rand_get(void);
/**<
Get a random number
@return     Return a 16-bit pseudo random number.
*/

void        *plt_memmem(const void *buf, size_t buf_len, const void *byte_seq, size_t byte_seq_len);
/**<
Find a byte sequence within a memory buffer
@param[in]	buf	            pointer to buffer
@param[in]	buf_len         size of buffer in bytes
@param[in]	byte_seq	    byte sequence to look for
@param[in]	byte_seq_len    size of byte sequence in bytes
@return     if successful, returns a pointer to the first
            occurrence of byte_seq in buf;  otherwise, returns NULL
*/

#if defined(_WINDOWS) || defined(WIN32)
#define strcasecmp _stricmp     ///< compares two strings irrespective of the case of characters
#endif

/**
@}
*/



#endif /* _ZIP_PLATFORM_DAVID_ */

