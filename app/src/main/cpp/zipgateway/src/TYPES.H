/*********************************  types.h  ********************************
 *           #######
 *           ##  ##
 *           #  ##    ####   #####    #####  ##  ##   #####
 *             ##    ##  ##  ##  ##  ##      ##  ##  ##
 *            ##  #  ######  ##  ##   ####   ##  ##   ####
 *           ##  ##  ##      ##  ##      ##   #####      ##
 *          #######   ####   ##  ##  #####       ##  #####
 *                                           #####
 *          Z-Wave, the wireless language.
 *
 *              Copyright (c) 2007
 *              Zensys A/S
 *              Denmark
 *
 *              All Rights Reserved
 *
 *    This source file is subject to the terms and conditions of the
 *    Zensys Software License Agreement which restricts the manner
 *    in which it may be used.
 *
 *---------------------------------------------------------------------------
 *
 * Description: See below.
 *
 * Author:   Oleg Zadorozhnyy
 *
 * 2007-08-11   OZA     Integrated with Z-Wave environment.
 *
 * Last Changed By:  $Author: oza $
 * Revision:         $Revision: 1.2 $
 * Last Changed:     $Date: 2007/08/19 11:30:20 $
 *
 ****************************************************************************/

/****************************************************************************/
/*                      PRIVATE TYPES and DEFINITIONS                       */
/****************************************************************************/

#ifndef _TYPES_H_
#define _TYPES_H_

#define NO_MEM_FUNCTIONS

#include <stdint.h>

#define auto
#define code
#define CODE
#define xdata

#ifndef _ASIX_TYPES_H    //To avoid the multiple definitions of BOOL  
#ifndef _MSC_VER
typedef uint8_t BOOL;
#endif

#endif  //_ASIX_TYPES_H

#ifdef __ASIX_C51__
#define REENTRANT reentrant
#else
#define REENTRANT
#endif

#ifdef __ASIX_C51__
#define uintptr_t DWORD
#endif

typedef signed char CHAR;
typedef unsigned char UCHAR;
typedef int INT;
typedef unsigned int UINT;
typedef unsigned long ULONG;


typedef unsigned char BYTE;
typedef uint16_t WORD;
typedef int16_t INT16;
typedef uint32_t DWORD;
typedef int32_t LONG;
typedef float FLOAT;
typedef double DOUBLE;

typedef	 BYTE	* BYTE_P;

typedef unsigned long u32;
typedef unsigned short u16;



#define XBYTE       BYTE
#define PBYTE  BYTE        /* External data byte residing in lower 256 byte in xdata */
#define IBYTE  BYTE

#define NO_MEM_FUNCTIONS

#define ZW_LOG(a,b)

#include <ZW_typedefs.h>
#include <string.h>

#endif /* _TYPES_H_ */


