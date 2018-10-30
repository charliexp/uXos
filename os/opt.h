/*************************************************************************\
 * File Name        : conf.h                                             *
 * --------------------------------------------------------------------- *
 * Title        : uXOS Cooperative system hardware config file           *
 * Revision        : V1.0                                                *
 * Notes        :                                                        *
 *                                                                       * 
 * --------------------------------------------------------------------- *
 * Revision History:                                                     *
 *   When           Who          Revision        Description of change   *
 * -----------   -----------     ---------      ------------------------ *
 * 19-08-2011     charlie        V1.0            Created the program     *
 *                                                                       * 
\*************************************************************************/

#ifndef __OPT_H__
#define __OPT_H__

//////////////////////////////////////////////////////////////////////////
#define  UXOS_VERSION   		   "uXOS 1.6.0"

//////////////////////////////////////////////////////////////////////////

typedef  unsigned char              UCHAR;
typedef  unsigned char              BYTE;
typedef  unsigned short             WORD;
typedef  unsigned long              DWORD;
typedef  BYTE              			BOOL;
typedef  BYTE                       HANDLE;
///////////////////////////////////////////////////////////////////////////
#ifndef  FALSE
 #define FALSE                      0
#endif

#ifndef  TRUE
 #define TRUE                       !FALSE
#endif
///////////////////////////////////////////////////////////////////////////
#ifndef  NULL
#define  NULL                       0
#endif  /* NULL */

#ifndef  UINT8_MAX
#define  UINT8_MAX                  0xFF
#endif

#ifndef  UINT16_MAX
#define  UINT16_MAX                 0xFFFF
#endif

#ifndef  UINT32_MAX
#define  UINT32_MAX                 0xFFFFFFFF
#endif

#ifndef  TICK
#define  TICK 						unsigned int
#define  STICK   			        int
#endif
#define  ENTER_CRITICAL_SECTION()
#define  EXIT_CRITICAL_SECTION()
#define  TICK_MAX                   UINT32_MAX


#ifndef  MAX_TASK_NUM
#define  MAX_TASK_NUM    			10
#endif

#ifndef  TIMER_NUMBER
#define  TIMER_NUMBER    			6
#endif


#define max(a,b) 					(a>b)?(a):(b) 

////////////////////////////////////////////////////////////////////////////
#define LongToBin(n) \
(                    \
((n >> 21) & 0x80) | \
((n >> 18) & 0x40) | \
((n >> 15) & 0x20) | \
((n >> 12) & 0x10) | \
((n >>  9) & 0x08) | \
((n >>  6) & 0x04) | \
((n >>  3) & 0x02) | \
((n      ) & 0x01)   \
)
#define BIN(n) LongToBin(0x##n##l)    //write binary charactor set,exsample : Bin(11111111) = 0xff

#define MAKEWORD(x1,x2)    			(WORD)(((WORD)(x1)<<8) + (WORD)(x2))
///////////////////////////////////////////////////////////////////////////
#endif 

