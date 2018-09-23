/*************************************************************************\
 * File Name    : uXOS.h                                                 *
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
 * 27-05-2016     charlie_weng     V1.5          update to uXos-II       *
 *                                                                       *
\*************************************************************************/

#ifndef __UXOS_H__
#define __UXOS_H__

#include "config.h"
#include "os/opt.h"

///////////////////////////////////////////////////////////////////////////

/**
 * \file
 * Protothreads implementation.
 * \author
 * Adam Dunkels <adam@sics.se>
 *
 */
typedef unsigned short lc_t;

#define LC_INIT(s)      s = 0;
#define LC_RESUME(s)    switch(s) { case 0:
#define LC_SET(s)       s = __LINE__; case __LINE__:
#define LC_YIELD(s)     s = __LINE__; return uPT_YIELDED; case __LINE__:
#define LC_END(s)       }

////////////////////////////////////////////////////////////////////////////
struct pt {
    lc_t lc;
};

typedef struct pt TASK_TCB;

#define uPT_WAITING      0
#define uPT_YIELDED      1
#define uPT_EXITED       2
#define uPT_ENDED        3

/**
 * Initialize a protothread.
 *
 * Initializes a protothread. Initialization must be done prior to
 * starting to execute the protothread.
 *
 * \param pt A pointer to the protothread control structure.
 *
 * \sa uPT_SPAWN()
 *
 * \hideinitializer
 */
#define uPT_INIT(pt)   LC_INIT((pt)->lc)

/**
 * Declaration of a protothread.
 *
 * This macro is used to declare a protothread. All protothreads must
 * be declared with this macro.
 *
 * \param name_args The name and arguments of the C function
 * implementing the protothread.
 *
 * \hideinitializer
 */
#define uPT_THREAD(name_args) char name_args

/**
 * Declare the start of a protothread inside the C function
 * implementing the protothread.
 *
 * This macro is used to declare the starting point of a
 * protothread. It should be placed at the start of the function in
 * which the protothread runs. All C statements above the uPT_BEGIN()
 * invokation will be executed each time the protothread is scheduled.
 * [modify by charlie 2011.01.28]
 * \param pt A pointer to the protothread control structure.
 *
 * \hideinitializer
 */
#define uPT_BEGIN(pt) { LC_RESUME((pt)->lc)

/**
 * Declare the end of a protothread.
 *
 * This macro is used for declaring that a protothread ends. It must
 * always be used together with a matching uPT_BEGIN() macro.
 * [modify by charlie 2011.01.28]
 * \param pt A pointer to the protothread control structure.
 *
 * \hideinitializer
 */
#define uPT_END(pt) LC_END((pt)->lc);  \
                   uPT_INIT(pt); return uPT_ENDED; }


/**
 * Block and wait until condition is true.
 *
 * This macro blocks the protothread until the specified condition is
 * true.
 *
 * \param pt A pointer to the protothread control structure.
 * \param condition The condition.
 *
 * \hideinitializer
 */
#define uPT_WAIT_UNTIL(pt, condition)           \
  do {                                          \
    LC_SET((pt)->lc);                           \
    if(!(condition)) {                          \
      return uPT_WAITING;                       \
    }                                           \
  } while(0)

/**
 * Block and wait while condition is true.
 *
 * This function blocks and waits while condition is true. See
 * uPT_WAIT_UNTIL().
 *
 * \param pt A pointer to the protothread control structure.
 * \param cond The condition.
 *
 * \hideinitializer
 */
#define uPT_WAIT_WHILE(pt, cond)  uPT_WAIT_UNTIL((pt), !(cond))

/**
 * Block and wait until a child protothread completes.
 *
 * This macro schedules a child protothread. The current protothread
 * will block until the child protothread completes.
 *
 * \note The child protothread must be manually initialized with the
 * uPT_INIT() function before this function is used.
 *
 * \param pt A pointer to the protothread control structure.
 * \param thread The child protothread with arguments
 *
 * \sa uPT_SPAWN()
 *
 * \hideinitializer
 */
#define uPT_WAIT_THREAD(pt, thread) uPT_WAIT_WHILE((pt), uPT_SCHEDULE(thread))

/**
 * Spawn a child protothread and wait until it exits.
 *
 * This macro spawns a child protothread and waits until it exits. The
 * macro can only be used within a protothread.
 *
 * \param pt A pointer to the protothread control structure.
 * \param child A pointer to the child protothread's control structure.
 * \param thread The child protothread with arguments
 *
 * \hideinitializer
 */
#define uPT_SPAWN(pt, child, thread)            \
  do {                                          \
    uPT_INIT((child));                          \
    uPT_WAIT_THREAD((pt), (thread));            \
  } while(0)

/**
 * Restart the protothread.
 *
 * This macro will block and cause the running protothread to restart
 * its execution at the place of the uPT_BEGIN() call.
 *
 * \param pt A pointer to the protothread control structure.
 *
 * \hideinitializer
 */
#define uPT_RESTART(pt)                         \
  do {                                          \
    uPT_INIT(pt);                               \
    return uPT_WAITING;                         \
  } while(0)

/**
 * Exit the protothread.
 *
 * This macro causes the protothread to exit. If the protothread was
 * spawned by another protothread, the parent protothread will become
 * unblocked and can continue to run.
 *
 * \param pt A pointer to the protothread control structure.
 *
 * \hideinitializer
 */
#define uPT_EXIT(pt)                            \
  do {                                          \
    uPT_INIT(pt);                               \
    return uPT_EXITED;                          \
  } while(0)


/**
 * Schedule a protothread.
 *
 * This function shedules a protothread. The return value of the
 * function is non-zero if the protothread is running or zero if the
 * protothread has exited.
 *
 * \param f The call to the C function implementing the protothread to
 * be scheduled
 *
 * \hideinitializer
 */
#define uPT_SCHEDULE(f) ((f) < uPT_EXITED)


/**
 * Yield from the current protothread.
 *
 * This function will yield the protothread, thereby allowing other
 * processing to take place in the system. [modify by charlie 2011.01.28]
 *
 * \param pt A pointer to the protothread control structure.
 *
 * \hideinitializer
 */
#define uPT_YIELD(pt)                           \
  do {                                          \
    LC_YIELD((pt)->lc);                         \
  } while(0)

/**
 * \brief      Yield from the protothread until a condition occurs.
 * \param pt   A pointer to the protothread control structure.
 * \param cond The condition.
 *
 *             This function will yield the protothread, until the
 *             specified condition evaluates to true.
 *  [modify by charlie 2011.01.28]
 *
 * \hideinitializer
 */
#define uPT_YIELD_UNTIL(pt, cond)               \
  do {                                          \
    LC_YIELD((pt)->lc);                         \
    if(!(cond)) {                               \
      return uPT_YIELDED;                       \
    }                                           \
  } while(0)

//////////////////////////////////////////////////////////////////////////

/**
 * \file
 * uXos implementation.
 * \author
 * charlie weng <charlie_weng@163.com>
 *
 */
struct TASK
{
	  void       *next;
      uPT_THREAD((*pTask)(struct pt* pt));      // task fuction pointer
      struct pt  pt;                            // protothread data
      TICK       tick;
      BYTE       pri;  
};
//////////////////////////////////////////////////////////////////////////

struct XOS_SEM
{
    BYTE count;
};


#define uXOS_THREAD( task )             uPT_THREAD(task(struct pt *pt ))
#define uXOS_BEGIN()                    uPT_BEGIN(pt)
#define uXOS_END()                      uPT_END(pt)
#define uXOS_WAIT_UNTIL(condition)      uPT_WAIT_UNTIL(pt, condition)
#define uXOS_WAIT_WHILE(cond)           uXOS_WAIT_UNTIL(!(cond))
#define uXOS_WAIT_THREAD(child, thread) uXOS_WAIT_WHILE(uPT_SCHEDULE(thread(child)))
#define uXOS_SPAWN(child, thread)       uPT_SPAWN(pt, child, thread( child ))
#define uXOS_RESTART()                  uPT_RESTART(pt)
#define uXOS_EXIT()                     uPT_EXIT(pt)
#define uXOS_YIELD()                    uPT_YIELD(pt)
#define uXOS_YIELD_UNTIL(cond)          uPT_YIELD_UNTIL(pt, cond)
#define uXOS_RESET( t )                 uPT_INIT( &t->pt )  // restart other task
#define uXOS_EXEC(child, thread)        thread( child )
//////////////////////////////////////////////////////////////////////////

#define uXOS_SEM_INIT(s, c)  (s)->count = c

#define uXOS_SEM_WAIT(s)                                         \
  do {                                                           \
    uPT_WAIT_UNTIL(pt, (s)->count > 0);                          \
    --(s)->count;                                                \
  } while(0)

#define uXOS_SEM_SIGNAL(s)   ++(s)->count

///////////////////////////////////////////////////////////////////////////

extern volatile TICK  			sysTick;
#define  sysTickHandler()     	sysTick++
#define  get_tick()    			sysTick

///////////////////////////////////////////////////////////////////////////
#define  uXOS_DELAY( param )                                                       \
  do{                                                                              \
        get_curr_task()->tick = get_tick();                                        \
        uPT_WAIT_UNTIL( pt, (TICK)(get_tick()- get_curr_task()->tick ) >= param ); \
    }while(0)

///////////////////////////////////////////////////////////////////////////
struct TASK*   create_task(uXOS_THREAD((*lpTask )), BYTE pri  );
void           os_init( void (*cb)(void) );
void           os_start(void);
BYTE 		       set_task_pri( struct TASK *t, BYTE pri );
BYTE 		       get_task_pri( struct TASK *t );     
struct TASK*   get_curr_task(void);
void           os_set_tmr_cb( void (*cb)(void) );
///////////////////////////////////////////////////////////////////////////
#include "uTimer.h"
#include "eTimer.h"		

#endif
