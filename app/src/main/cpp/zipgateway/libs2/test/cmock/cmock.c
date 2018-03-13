/* ==========================================
    CMock Project - Automatic Mock Generation for C
    Copyright (c) 2007 Mike Karlesky, Mark VanderVoord, Greg Williams
    [Released under MIT License. Please refer to license.txt for details]
========================================== */

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
#include "unity.h"

#include "cmock_internals.h"

//private variables
#ifdef CMOCK_MEM_DYNAMIC
static unsigned char*         CMock_Guts_Buffer = NULL;
static CMOCK_MEM_INDEX_TYPE   CMock_Guts_BufferSize = CMOCK_MEM_ALIGN_SIZE;
static CMOCK_MEM_INDEX_TYPE   CMock_Guts_FreePtr;
#else
static unsigned char          CMock_Guts_Buffer[CMOCK_MEM_SIZE + CMOCK_MEM_ALIGN_SIZE];
static CMOCK_MEM_INDEX_TYPE   CMock_Guts_BufferSize = CMOCK_MEM_SIZE + CMOCK_MEM_ALIGN_SIZE;
static CMOCK_MEM_INDEX_TYPE   CMock_Guts_FreePtr;
#endif

//-------------------------------------------------------
// CMock_Guts_MemNew
//-------------------------------------------------------
CMOCK_MEM_INDEX_TYPE CMock_Guts_MemNew(CMOCK_MEM_INDEX_TYPE size)
{
  CMOCK_MEM_INDEX_TYPE index;

  //verify arguments valid (we must be allocating space for at least 1 byte, and the existing chain must be in memory somewhere)
  if (size < 1)
    return CMOCK_GUTS_NONE;

  //verify we have enough room
  size = size + CMOCK_MEM_INDEX_SIZE;
  if (size & CMOCK_MEM_ALIGN_MASK)
    size = (size + CMOCK_MEM_ALIGN_MASK) & ~CMOCK_MEM_ALIGN_MASK;
  if ((CMock_Guts_BufferSize - CMock_Guts_FreePtr) < size)
  {
#ifdef CMOCK_MEM_DYNAMIC
    CMock_Guts_BufferSize += CMOCK_MEM_SIZE + size;
    CMock_Guts_Buffer = realloc(CMock_Guts_Buffer, (size_t)CMock_Guts_BufferSize);
    if (CMock_Guts_Buffer == NULL)
#endif //yes that if will continue to the return below if TRUE
      return CMOCK_GUTS_NONE;
  }

  //determine where we're putting this new block, and init its pointer to be the end of the line
  index = CMock_Guts_FreePtr + CMOCK_MEM_INDEX_SIZE;
  *(CMOCK_MEM_INDEX_TYPE*)(&CMock_Guts_Buffer[CMock_Guts_FreePtr]) = CMOCK_GUTS_NONE;
  CMock_Guts_FreePtr += size;

  return index;
}

//-------------------------------------------------------
// CMock_Guts_MemChain
//-------------------------------------------------------
CMOCK_MEM_INDEX_TYPE CMock_Guts_MemChain(CMOCK_MEM_INDEX_TYPE root_index, CMOCK_MEM_INDEX_TYPE obj_index)
{
  CMOCK_MEM_INDEX_TYPE index;
  void* root;
  void* obj;
  void* next;

  if (root_index == CMOCK_GUTS_NONE)
  {
    //if there is no root currently, we return this object as the root of the chain
    return obj_index;
  }
  else
  {
    //reject illegal nodes
    if ((root_index < CMOCK_MEM_ALIGN_SIZE) || (root_index >= CMock_Guts_FreePtr))
    {
      return CMOCK_GUTS_NONE;
    }
    if ((obj_index < CMOCK_MEM_ALIGN_SIZE) || (obj_index >= CMock_Guts_FreePtr))
    {
      return CMOCK_GUTS_NONE;
    }

    root = (void*)(&CMock_Guts_Buffer[root_index]);
    obj  = (void*)(&CMock_Guts_Buffer[obj_index]);

    //find the end of the existing chain and add us
    next = root;
    do {
      index = *(CMOCK_MEM_INDEX_TYPE*)((CMOCK_MEM_PTR_AS_INT)next - CMOCK_MEM_INDEX_SIZE);
      if (index >= CMock_Guts_FreePtr)
        return CMOCK_GUTS_NONE;
      if (index > 0)
        next = (void*)(&CMock_Guts_Buffer[index]);
    } while (index > 0);
    *(CMOCK_MEM_INDEX_TYPE*)((CMOCK_MEM_PTR_AS_INT)next - CMOCK_MEM_INDEX_SIZE) = (CMOCK_MEM_INDEX_TYPE)((CMOCK_MEM_PTR_AS_INT)obj - (CMOCK_MEM_PTR_AS_INT)CMock_Guts_Buffer);
    return root_index;
  }
}

//-------------------------------------------------------
// CMock_Guts_MemNext
//-------------------------------------------------------
CMOCK_MEM_INDEX_TYPE CMock_Guts_MemNext(CMOCK_MEM_INDEX_TYPE previous_item_index)
{
  CMOCK_MEM_INDEX_TYPE index;
  void* previous_item;

  //There is nothing "next" if the pointer isn't from our buffer
  if ((previous_item_index < CMOCK_MEM_ALIGN_SIZE) || (previous_item_index  >= CMock_Guts_FreePtr))
    return CMOCK_GUTS_NONE;
  previous_item = (void*)(&CMock_Guts_Buffer[previous_item_index]);

  //if the pointer is good, then use it to look up the next index
  //(we know the first element always goes in zero, so NEXT must always be > 1)
  index = *(CMOCK_MEM_INDEX_TYPE*)((CMOCK_MEM_PTR_AS_INT)previous_item - CMOCK_MEM_INDEX_SIZE);
  if ((index > 1) && (index < CMock_Guts_FreePtr))
    return index;
  else
    return CMOCK_GUTS_NONE;
}

//-------------------------------------------------------
// CMock_Guts_MemEndOfChain
//-------------------------------------------------------
CMOCK_MEM_INDEX_TYPE CMock_Guts_MemEndOfChain(CMOCK_MEM_INDEX_TYPE root_index)
{
  CMOCK_MEM_INDEX_TYPE index = root_index;
  CMOCK_MEM_INDEX_TYPE next_index;

  for (next_index = root_index;
       next_index != CMOCK_GUTS_NONE;
       next_index = CMock_Guts_MemNext(index))
  {
    index = next_index;
  }

  return index;
}

//-------------------------------------------------------
// CMock_GetAddressFor
//-------------------------------------------------------
void* CMock_Guts_GetAddressFor(CMOCK_MEM_INDEX_TYPE index)
{
  if ((index >= CMOCK_MEM_ALIGN_SIZE) && (index < CMock_Guts_FreePtr))
  {
    return (void*)(&CMock_Guts_Buffer[index]);
  }
  else
  {
    return NULL;
  }
}

//-------------------------------------------------------
// CMock_Guts_MemBytesFree
//-------------------------------------------------------
CMOCK_MEM_INDEX_TYPE CMock_Guts_MemBytesFree(void)
{
  return CMock_Guts_BufferSize - CMock_Guts_FreePtr;
}

//-------------------------------------------------------
// CMock_Guts_MemBytesUsed
//-------------------------------------------------------
CMOCK_MEM_INDEX_TYPE CMock_Guts_MemBytesUsed(void)
{
  return CMock_Guts_FreePtr - CMOCK_MEM_ALIGN_SIZE;
}

//-------------------------------------------------------
// CMock_Guts_MemFreeAll
//-------------------------------------------------------
void CMock_Guts_MemFreeAll(void)
{
  CMock_Guts_FreePtr = CMOCK_MEM_ALIGN_SIZE; //skip the very beginning
}

//-------------------------------------------------------
// CMock_Guts_MemFreeFinal
//-------------------------------------------------------
void CMock_Guts_MemFreeFinal(void)
{
  CMock_Guts_FreePtr = CMOCK_MEM_ALIGN_SIZE;
#ifdef CMOCK_MEM_DYNAMIC
  if (CMock_Guts_Buffer)
  {
    free(CMock_Guts_Buffer);
    CMock_Guts_Buffer = NULL;
  }
#endif
}

