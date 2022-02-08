/****************************************************************************
*  Program/module: RtxDef.h                                                 *
*  Description:                                                             *
*                                                                           *
*  Programmer: MHP                                                          *
*  Created: 990217  13:29:50                                                *
*  Last update: 990310 15:17:17                                             *
*                                                                           *
****************************************************************************/
/****************************************************************************
*                                                                           *
*  PROJECT:                                                                 *
*  CONTROLLING DOCUMENT:                                                    *
*  SYSTEM DEPENDENCIES:                                                     *
*                                                                           *
****************************************************************************/



/****************************************************************************
*                                 PVCS info                                 *
*****************************************************************************

$Author:   lgj  $
$Date:   06 Jun 2013 14:18:46  $
$Revision:   1.1  $
$Modtime:   16 Nov 2011 10:40:10  $

*/


#ifndef RTXDEF_H
#define RTXDEF_H


/*******************************************************************************
* Compiler dependent definitions                                               *
*******************************************************************************/

#ifdef __TURBOC__
  // This is Borland C/C++ 3.1
  #define PCCOMPILE
#endif

#if defined(_WIN32) || defined(__WIN32__)
  // This is Visual C++ or Borland C/C++ 5.x
  #define PCCOMPILE
  #define WIN32COMPILE
  #if defined(_CONSOLE) || defined(__CONSOLE__)
    #define WIN32CONSOLE
  #else
    #define WIN32_SIM
  #endif
  #ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN // Reduce windows.h
  #endif
#endif

// Use this macro for external declarations of C source in C++ projects
#ifdef __cplusplus
  #define EXTERN_C extern "C"
#else
  #define EXTERN_C extern
#endif


/*******************************************************************************
* GLOBAL TYPEDEFINITIONS                                                       *
*******************************************************************************/


typedef unsigned char   uint8;     // byte 0..255 
typedef   signed char   int8;      // byte -128..127
typedef unsigned short  uint16;    // word 0..65535 
typedef   signed short  int16;     // 16-bit -32768..32767 
typedef unsigned long   uint32;    // 32-bit 0..4294967295 
typedef   signed long   int32;     // 32-bit -2147483648..2147483647 
typedef unsigned char   boolean;   // 0 is logical FALSE, !0 is logical TRUE 


typedef void (* ProcPtrType)(void);

/*******************************************************************************
* GLOBAL DEFINITIONS                                                           *
*******************************************************************************/
#define TRUE       1
#define FALSE      0

#define OK         TRUE
#define NOT_OK     FALSE
#define BAD        FALSE

#define YES        TRUE
#define NO         FALSE

#define UP         TRUE
#define DOWN       FALSE

#define ON         TRUE
#define OFF        FALSE

#define OUTPUT     TRUE
#define INPUT      FALSE

#define ENABLE     TRUE
#define ENABLED    TRUE
#define DISABLE    FALSE
#define DISABLED   FALSE

#define HIGH       TRUE
#define LOW        FALSE

#define UPPER      TRUE
#define LOWER      FALSE

#define OPEN       TRUE
#define OPENED     TRUE
#define CLOSE      FALSE
#define CLOSED     FALSE

// 990310/LEH: Used in portdef.h
//#define START      TRUE
//#define STOP       FALSE


/* Define NULL pointer value */

#ifndef NULL
 #ifdef  __cplusplus
  #define NULL    0
 #else
  #define NULL    ((void *)0)
 #endif
#endif



/******************************************************************************
* Bit definitions                                                             *
******************************************************************************/


#define BIT0               0x01
#define BIT1               0x02
#define BIT2               0x04
#define BIT3               0x08
#define BIT4               0x10
#define BIT5               0x20
#define BIT6               0x40
#define BIT7               0x80

#define BIT8             0x0100
#define BIT9             0x0200
#define BIT10            0x0400
#define BIT11            0x0800
#define BIT12            0x1000
#define BIT13            0x2000
#define BIT14            0x4000
#define BIT15            0x8000

#define BIT16        0x00010000
#define BIT17        0x00020000
#define BIT18        0x00040000
#define BIT19        0x00080000
#define BIT20        0x00100000
#define BIT21        0x00200000
#define BIT22        0x00400000
#define BIT23        0x00800000

#define BIT24        0x01000000
#define BIT25        0x02000000
#define BIT26        0x04000000
#define BIT27        0x08000000
#define BIT28        0x10000000
#define BIT29        0x20000000
#define BIT30        0x40000000
#define BIT31        0x80000000


#define LOW_NIBBLE         0x0f
#define HIGH_NIBBLE        0xf0


/*******************************************************************************
* Bit counts/positions/masks                                                   *
*******************************************************************************/

/* Bit counts */

#define BITS_IN_UINT8              8
#define BITS_IN_INT8               8
#define BITS_IN_UINT16             16
#define BITS_IN_INT16              16
#define BITS_IN_UINT32             32
#define BITS_IN_INT32              32

/* Bit positions */

#define MSB_POS_IN_UINT8           7
#define MSB_POS_IN_INT8            7
#define MSB_POS_IN_UINT16          15
#define MSB_POS_IN_INT16           15
#define MSB_POS_IN_UINT32          31
#define MSB_POS_IN_INT32           31

/* Bit masks */

#define MSB_MASK_IN_UINT8          0x80
#define MSB_MASK_IN_INT8           0x80
#define MSB_MASK_IN_UINT16         0x8000
#define MSB_MASK_IN_INT16          0x8000
#define MSB_MASK_IN_UINT32         0x80000000
#define MSB_MASK_IN_INT32          0x80000000


/*******************************************************************************
* Conversion macros                                                            *
*******************************************************************************/
                 
#define INT8_TO_INT16(h, l)   ((h << BITS_IN_INT8) + l)
#define UINT8_TO_UINT16(h, l) ((h << BITS_IN_UINT8) + l)
#define HIGH_INT8(w)          ((int8) (w >> BITS_IN_INT8))
#define HIGH_UINT8(w)         ((uint8) (w >> BITS_IN_UINT8))
#define LOW_INT8(w)           ((int8) w)
#define LOW_UINT8(w)          ((uint8) w)


// New names for these ?
#define BYTE0_OF_LONG(var) ((unsigned char) var) 
#define BYTE1_OF_LONG(var) ((unsigned char) (var >> 8))
#define BYTE2_OF_LONG(var) ((unsigned char) (var >> 16))
#define BYTE3_OF_LONG(var) ((unsigned char) (var >> 24))




#endif  /* header */

