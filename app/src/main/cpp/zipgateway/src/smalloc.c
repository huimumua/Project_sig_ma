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
#include <assert.h>
#include <smalloc.h>
//#include <string.h>
//#include <stdio.h>
#define ASSERT assert

#define CHUNK_USED 0x80
#define MIN_CHUNK_SIZE 4


/*
 * Memory chunks are stored as len1 | data1| len2 | data2 | .....| lenN | dataN
 *
 * A better approach would be to store len1|len2|...|lenN| data1 | data2 | ...| dataN
 * However in the second approach fragmentation might be more difficult to handle.
 * */

/* Write a small memory chunk to storage and return a pointer to the
 * small memory chunk in the device storage. */
uint16_t smalloc(const small_memory_device_t* dev ,uint8_t datalen) {
  uint16_t p,start,end;
  uint8_t l,d;
  uint8_t reminder;

  ASSERT(datalen < 0x80);
  ASSERT(dev->offset > 0);

  p = dev->offset;
  end = (dev->offset) +  dev->size - 1;
  while((p+datalen) < end) {
    /*Find the number of consecutive free bytes */
    start = p;

    while( 1 ) {
      dev->read(p,1,&l);
      p = (p + (l & 0x7F) + 1);

      if( l & CHUNK_USED ) break;

      /*The end of the allocated area has 0 length*/
      if(l==0) {
        d=datalen | CHUNK_USED;
        dev->write(start,1,&d);

        d=0; /*Write the new end of list marker*/
        dev->write(start+1+datalen,1,&d);
        return start+1;
      }

      if(p >= end) {
        return 0;
      }

      /*We found an exact match, the +1 is because we need to have room for the length field */
      if( (datalen+1) == (p-start) ) {
        d = datalen | CHUNK_USED;
        dev->write(start,1,&d);
        return start+1;
      }

      /*
       * We found a number of free segments which we may divide into a new allocated segment
       * and a smaller free one.
       */
      if( (datalen+1)  <  (p-start) ) {
        reminder = (p-start) - (datalen+1);

        if(reminder < MIN_CHUNK_SIZE) {
          d = (p-start-1) | CHUNK_USED;
        } else {
          /*Divide length marker at the end */
          d=reminder-1;
          dev->write( start+datalen+1,1,&d );
          d = datalen | CHUNK_USED;
        }
        dev->write(start,1,&d);
        return start+1;
      }
    }
  }
  return 0;
}

void smfree(const small_memory_device_t* dev ,uint16_t ptr) {
  uint8_t c;

  ASSERT(ptr > (dev->offset));
  ASSERT(ptr < (dev->offset +  dev->size));
  dev->read(ptr-1,1,&c);

  ASSERT(c & CHUNK_USED);
  c &= ~CHUNK_USED;
  dev->write(ptr-1,1,&c);
}


uint16_t smalloc_write(const small_memory_device_t* dev ,uint8_t datalen,void* data) {
  uint16_t ptr;
  ptr = smalloc(dev,datalen);
  if(ptr) {
      dev->write(ptr,datalen,data);
  }
  return ptr;
}

void smformat(const small_memory_device_t* dev) {
  uint8_t nul;
  nul=0;
  dev->write(dev->offset,1,&nul);
}


//#define TEST_MODE

#ifdef TEST_MODE

static char rd_membuf[0x1000];

static uint16_t rd_mem_read(uint16_t offset,uint8_t len,void* data) {
  memcpy(data,rd_membuf + offset, len);
  return len;
}


static uint16_t rd_mem_write(uint16_t offset,uint8_t len,void* data) {
  memcpy(rd_membuf + offset,data, len);
  return len;
}


const small_memory_device_t m  = {
  .offset = 1,
  .size = 0xFFF,
  .psize = 8,
  .read = rd_mem_read,
  .write = rd_mem_write,
};



#define NUM_ELEM 16
int main(int argc, char **argp) {
  int j,i;

  struct elem{
    uint16_t ptr;
    uint8_t len;
  };

  struct elem e[NUM_ELEM];

  memset(rd_membuf,0,sizeof(rd_membuf));
  memset(e,0,sizeof(e));

  while(1) {
    j = rand() % NUM_ELEM;
    if( e[j].ptr==0 ) {
      e[j].len = rand() % 0x7c +1;

      e[j].ptr = smalloc(&m,e[j].len);

      printf("Alloc %02x ptr=%04x \n",e[j].len,e[j].ptr);

      if(e[j].ptr==0) {
        printf("Out of mem\n");
      } else {
          memset(& (rd_membuf[e[j].ptr]) ,e[j].len,e[j].len);
      }
    } else {

        printf("Free %02x\n",e[j].len);

        for(i=0; i < e[j].len; i++) {
          if(rd_membuf[e[j].ptr+i] != e[j].len) {
              printf("Memory corruption got %02x expected %02x ptr %04x\n",rd_membuf[e[j].ptr+i],e[j].len,e[j].ptr);
              return 1;
          }
        }
      smfree(&m,e[j].ptr);
      e[j].ptr =0;
    }
  }

}
#endif
