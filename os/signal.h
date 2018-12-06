/*************************************************************************\
 * File Name    : signal.h                                               *
 * --------------------------------------------------------------------- *
 * Title        : uXOS --- Cooperative system                            *
 * Revision     : V1.0                                                   *
 * Notes        :                                                        *
 * --------------------------------------------------------------------- *
 * Revision History:                                                     *
 *   When             Who         Revision       Description of change   *
 * -----------    -----------    ---------      ------------------------ *
 * 12-11-2010     charlie_weng     V1.0          Created the program     *
 * 28-02-2013     charlie_weng     V1.3          add etimer,stimer       *
 * 04-09-2018     charlie_weng     V1.6          reconsitution           * 
 *                                                                       *
\*************************************************************************/

#ifndef _SIGNAL_H_
#define _SIGNAL_H_

//////////////////////////////////////////////////////////////////////////
#ifndef WHEEL_MAX
#define WHEEL_MAX      8
#endif

#ifndef MAX_SLOT_TCB
#define MAX_SLOT_TCB  20
#endif

#ifndef USER_SIGNAL_BASE
#define USER_SIGNAL_BASE  0x7F   // 1-7F 自定义信号
#endif

#define SLOT(x)       ((void*)x)
#define SLOT_FUNC(f)  ((void (*)(int x,...))f)
#define SIGNAL        int    
//////////////////////////////////////////////////////////////////////////
typedef struct slot_tcb
{
    SIGNAL  sig;
    void   (*slot)(int x, ...);
    struct slot_tcb *next;
}slot_t;

/////////////////////////////////////////////////////////////////////////
int    connect( SIGNAL sig, void *slot );
int    disconnect( SIGNAL sig );
void   emit( SIGNAL sig, int arg_num, ... );
void   signal_slot_init( void );
SIGNAL create_signal( void );

#endif
