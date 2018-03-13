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
#include "Serialapi.h"
#include <ZW_controller_api.h>
#include <stdio.h>
#ifndef _MSC_VER
/* According to POSIX.1-2001 */
#include <sys/select.h>
/* According to earlier standards */
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#else
#undef BOOL
#undef BYTE
#undef PBYTE
#undef code
#include<windows.h>
#include<winbase.h>
#endif



/* A list of the known command classes. Except the basic class which allways */
/* should be supported. Used when node info is send */
const APPL_NODE_TYPE MyType = {GENERIC_TYPE_ZIP_GATEWAY,SPECIFIC_TYPE_ZIP_ADV_GATEWAY};
const BYTE MyClasses[] = { COMMAND_CLASS_NO_OPERATION, COMMAND_CLASS_SIMPLE_AV_CONTROL };

const BYTE NumberOfClasses = sizeof(MyClasses);

#define COMMAND_CLASS_SIMPLE_AV_CONTROL                                                  0x94

/* Simple Av Control command class commands */
#define SIMPLE_AV_CONTROL_VERSION                                                        0x01
#define SIMPLE_AV_CONTROL_GET                                                            0x02
#define SIMPLE_AV_CONTROL_REPORT                                                         0x03
#define SIMPLE_AV_CONTROL_SET                                                            0x01
#define SIMPLE_AV_CONTROL_SUPPORTED_GET                                                  0x04
#define SIMPLE_AV_CONTROL_SUPPORTED_REPORT                                               0x05
/* Values used for Simple Av Control Set command */
#define SIMPLE_AV_CONTROL_SET_PROPERTIES1_KEY_ATTRIBUTES_MASK                            0x07
#define SIMPLE_AV_CONTROL_SET_PROPERTIES1_RESERVED_MASK                                  0xF8
#define SIMPLE_AV_CONTROL_SET_PROPERTIES1_RESERVED_SHIFT                                 0x03

#define SIMPLE_AV_CMD_PLAY    0x0013
#define SIMPLE_AV_CMD_STOP    0x0014
#define SIMPLE_AV_CMD_CH_UP     0x0004
#define SIMPLE_AV_CMD_CH_DOWN   0x0005
#define SIMPLE_AV_CMD_VOL_UP    0x0003
#define SIMPLE_AV_CMD_VOL_DOWN  0x0002
#define SIMPLE_AV_CMD_MUTE    0x0001
#define SIMPLE_AV_CMD_MENU    0x001D
#define SIMPLE_AV_CMD_MENU_UP      0x001E
#define SIMPLE_AV_CMD_MENU_DOWN    0x001F
#define SIMPLE_AV_CMD_MENU_LEFT    0x0020
#define SIMPLE_AV_CMD_MENU_RIGHT   0x0021
#define SIMPLE_AV_CMD_SELECT  0x0024
#define SIMPLE_AV_CMD_PAGEUP  0x0022
#define SIMPLE_AV_CMD_PAGEDOWN  0x0023
#define SIMPLE_AV_CMD_POWER    0x0027

typedef struct _AV_CMD_STRINGS_
{
  unsigned short avCmd;
  unsigned char* str;
}AV_CMD_STRINGS;

AV_CMD_STRINGS avCmdStrings[] = {{SIMPLE_AV_CMD_PLAY,"Key: PLAY"},{SIMPLE_AV_CMD_STOP,"Key: STOP"},
                                 {SIMPLE_AV_CMD_CH_UP,"Key: CHANNEL UP"},{SIMPLE_AV_CMD_CH_DOWN,"Key: CHANNEL DOWN"},
                                 {SIMPLE_AV_CMD_VOL_UP,"Key: VOLUME UP"},{SIMPLE_AV_CMD_VOL_DOWN,"Key: VOLUME DOWN"},
                                 {SIMPLE_AV_CMD_MUTE,"Key: MUTE"},{SIMPLE_AV_CMD_MENU,"Key: MENU"},
                                 {SIMPLE_AV_CMD_MENU_UP,"Key: MENU UP"},{SIMPLE_AV_CMD_MENU_DOWN,"Key: MENU DOWN"},
                                 {SIMPLE_AV_CMD_MENU_LEFT,"Key: MENU LEFT"},{SIMPLE_AV_CMD_MENU_RIGHT,"Key: MENU RIGHT"},
                                 {SIMPLE_AV_CMD_SELECT,"Key: SELECT"},{SIMPLE_AV_CMD_PAGEUP,"Key: PAGE UP"},
                                 {SIMPLE_AV_CMD_PAGEDOWN,"Key: PAGE DOWN"},{SIMPLE_AV_CMD_POWER,"Key: POWER"},
                                 {0,"NOT DEFINED"}
                                };


void
ApplicationCommandHandler(
  BYTE  rxStatus,                   /*IN  Frame header info */
#if defined(ZW_CONTROLLER) && !defined(ZW_CONTROLLER_STATIC) && !defined(ZW_CONTROLLER_BRIDGE)
  BYTE  destNode,                  /*IN  Frame destination ID, only valid when frame is not Multicast*/
#endif
  BYTE  sourceNode,                 /*IN  Command sender Node ID */
  ZW_APPLICATION_TX_BUFFER *pCmd,  /*IN  Payload from the received frame, the union
                                          should be used to access the fields*/
  BYTE   cmdLength) CC_REENTRANT_ARG                /*IN  Number of command bytes including the command */
{
     printf("ApplicationCommandHandler node %d class %d size %d\n",sourceNode,pCmd->ZW_Common.cmdClass,cmdLength);
     if(pCmd->ZW_Common.cmdClass == COMMAND_CLASS_SIMPLE_AV_CONTROL) {
	switch (pCmd->ZW_Common.cmd)
	{
	  case SIMPLE_AV_CONTROL_SET:
{
	    unsigned short index;
	    unsigned short avCommand = pCmd->ZW_SimpleAvControlSet1byteFrame.variantgroup1.command1;
	    avCommand <<=8;
	    avCommand |= pCmd->ZW_SimpleAvControlSet1byteFrame.variantgroup1.command2;
	    for (index = 0; avCmdStrings[index].avCmd != 0;index++)
	    {
	      if (avCommand == avCmdStrings[index].avCmd)
	      {
	        break;
	      }
	    }
	    printf("SIMPLE_AV_CONTROL_SET \n");
      printf("%s ",avCmdStrings[index].str);
	    if ((pCmd->ZW_SimpleAvControlSet1byteFrame.properties1 & 0x07) == 0)
	    {
	      printf("Attribute :Key Down\n");
	    }
	    else  if ((pCmd->ZW_SimpleAvControlSet1byteFrame.properties1 & 0x07) == 1)
	    {
	      printf("Attribute :Key Up\n");
	    }
	    else  if ((pCmd->ZW_SimpleAvControlSet1byteFrame.properties1 & 0x07) == 2)
	    {
	      printf("Attribute :Keep Alive\n");
	    }
	    else
	    {
	      printf("Attribute :UNKNOWN \n");
	    }

}
	  break;
	}
  }
}

void ApplicationNodeInformation(BYTE *deviceOptionsMask, /*OUT Bitmask with application options    */
		APPL_NODE_TYPE *nodeType, /*OUT  Device type Generic and Specific   */
		BYTE **nodeParm, /*OUT  Device parameter buffer pointer    */
		BYTE *parmLength /*OUT  Number of Device parameter bytes   */
		) {
	/* this is a listening node and it supports optional CommandClasses */
	*deviceOptionsMask = APPLICATION_NODEINFO_LISTENING
		| APPLICATION_NODEINFO_OPTIONAL_FUNCTIONALITY;
	nodeType->generic = MyType.generic; /* Generic device type */
	nodeType->specific = MyType.specific; /* Specific class */
	*nodeParm = (BYTE*)MyClasses; /* Send list of known command classes. */
	*parmLength = sizeof(MyClasses); /* Set length*/
}

void ApplicationControllerUpdate(BYTE bStatus, /*IN  Status event */
		BYTE bNodeID, /*IN  Node id of the node that send node info */
		BYTE* pCmd, /*IN  Pointer to Application Node information */
		BYTE bLen /*IN  Node info length                        */
		) {
	printf("Got node info from node %d\n",bNodeID);
}

/*
 * Setup callback funtions. See Z-Wave Application Programmer
 * guide for information on what their roles are.
 */
const struct SerialAPI_Callbacks serial_api_callbacks = {
	ApplicationCommandHandler,
	ApplicationNodeInformation,
	ApplicationControllerUpdate,
	0,
	0,
	0,
	0,
};

#ifdef _MSC_VER
int chready() {
  //
  return (WaitForSingleObject(GetStdHandle(STD_INPUT_HANDLE), 2) & WAIT_TIMEOUT)==0; 
}

#else
int chready() {
	fd_set rfds;
	struct timeval tv;
	int retval;

	/* Watch stdin (fd 0) to see when it has input. */
	FD_ZERO(&rfds);
	FD_SET(0, &rfds);

	/* Wait up to five seconds. */
	tv.tv_sec = 0;
	tv.tv_usec = 100*1000;

	retval = select(1, &rfds, NULL, NULL, &tv);
	/* Don't rely on the value of tv now! */

	if (retval == -1)
		return 0;
	else if (retval)
		return 1;
	else
		return 0;
}
#endif

static void AddNodeStatusUpdate(LEARN_INFO* inf) {
	printf("AddNodeStatusUpdate status=%d info len %d\n",inf->bStatus,inf->bLen);

	switch(inf->bStatus) {
		case ADD_NODE_STATUS_LEARN_READY:
			break;
		case ADD_NODE_STATUS_NODE_FOUND:
			break;
		case ADD_NODE_STATUS_ADDING_SLAVE:
		case ADD_NODE_STATUS_ADDING_CONTROLLER:
			if(inf->bLen) {
				printf("Node added with nodeid %d\n", inf->bSource );
			}
			break;
		case ADD_NODE_STATUS_PROTOCOL_DONE:
			ZW_AddNodeToNetwork(ADD_NODE_STOP,AddNodeStatusUpdate);
			break;
		case ADD_NODE_STATUS_DONE:
		case ADD_NODE_STATUS_FAILED:
		case ADD_NODE_STATUS_NOT_PRIMARY:
			break;
	}
}


void RemoveNodeStatusUpdate(LEARN_INFO* inf) {
  printf("RemoveNodeStatusUpdate status=%d\n",inf->bStatus);
  switch(inf->bStatus) {
  case ADD_NODE_STATUS_LEARN_READY:
    break;
  case REMOVE_NODE_STATUS_NODE_FOUND:
    printf("Removing node %d\n",inf->bSource);
    break;
  case REMOVE_NODE_STATUS_DONE:
    break;
  case REMOVE_NODE_STATUS_FAILED:
    break;
  }

}

void SetDefaultUpdate(){
    printf("Set Default done\n");
}



void test() {
	printf("Hello\n");
}

int main(int argc, char** argv) {
	char buf[64];
	int k;
        NODEINFO nif;
	memset(buf,0,sizeof(buf));
	if(argc!=2) {
		fprintf(stderr, "Usage: %s <serialport_dev>\n",argv[0]);
		return 1;
	}

	if(!SerialAPI_Init(argv[1],&serial_api_callbacks)) {
	  fprintf(stderr,"SerialAPI not initilized\n");
	  return 1;
	}
	ZW_Version(buf);
	printf("Vesion: %s\n", buf );
	ZW_TIMER_START(test,100,TIMER_FOREVER);

	
	ZW_SoftReset();
	while(1) {
		SerialAPI_Poll();

		/*		if(ZW_Version(buf) ) {
				printf("Vesion: %s\n", buf );
				} else {
				printf("Error\n");
				}*/
		if(chready()) {
			switch(getc(stdin)) {
				case 'a':
					printf("Addnode start\n");
					ZW_AddNodeToNetwork(ADD_NODE_ANY|ADD_NODE_OPTION_NETWORK_WIDE,AddNodeStatusUpdate);
					break;
				case 'r':
					printf("Remove node start\n");
					ZW_RemoveNodeFromNetwork(REMOVE_NODE_ANY,RemoveNodeStatusUpdate);
					break;
				case 'd':
					printf("Set Default start\n");
					ZW_SetDefault(SetDefaultUpdate);
					break;
				case 'q':
					printf("Quit\n");
					return 0;
			}
		}
	}
	return 0;
}
