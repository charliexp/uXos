/*************************************************************************\
 * File Name    : uXOS.c                                                 *
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

#include "uXOS.h"
#include "lib/list.h"
#include "lib/memb.h"

///////////////////////////////////////////////////////////////////////////
LIST( tasklist );
MEMB( taskblks, struct TASK, MAX_TASK_NUM+1 );

///////////////////////////////////////////////////////////////////////////
volatile TICK  sysTick;
static struct  TASK  *curr_task;		 
static   void (*sysCallback)( void );
static   void  scheduler(void);

/*************************************************************************\
 *                                                                       *
 *   Function name   : taskRemove                                        *
 *   Returns         : void                                              *
 *   Parameters      : TASK*                                             *
 *   Purpose         :                                                   *
 *                                                                       *
\*************************************************************************/
void task_remove(struct TASK *t)
{
	list_remove( tasklist, t );
	memb_free( &taskblks, t );
}

/*************************************************************************\
 *                                                                       *
 *   Function name   : XOSCBInit                                         *
 *   Returns         : void                                              *
 *   Parameters      : callback function                                 *
 *   Purpose         : set callback                                      *
 *                                                                       *
\*************************************************************************/
void os_init( void (*cb)(void) )
{
	list_init(  tasklist );
	memb_init( &taskblks );
    sysCallback = cb;
}

/*************************************************************************\
 *                                                                       *
 *   Function name   : OSStart                                           *
 *   Returns         : void                                              *
 *   Parameters      : void                                              *
 *   Purpose         : start scheduler                                   *
 *                                                                       *
\*************************************************************************/
void os_start(void)                                              
{
	ENABLE_ISR();           
    
	curr_task = (struct TASK*)list_head( tasklist );
    
	while(1)                      
    {                                                      
        scheduler();                                
    }
}                                                                

		
/*************************************************************************\
 *                                                                       *
 *   Function name   : createTask                                        *
 *   Returns         : HANDLE                                            *
 *   Parameters      : pTask,pt                                          *
 *   Purpose         : Create a Task                                     *
 *                                                                       *
\*************************************************************************/
struct TASK* create_task( uXOS_THREAD((*lpTask )),BYTE pri )             
{
	struct TASK *t;
    
	t = (struct TASK*)memb_alloc( &taskblks );
    
    t->pTask = lpTask;   
    t->pri   = pri; 
	uPT_INIT( &t->pt );
    list_remove( tasklist, t );
	list_add( tasklist, t );

    return t;
}
/*************************************************************************\
 *                                                                       *
 *   Function name   : getTaskPri                                        *
 *   Returns         : BYTE                                              *
 *   Parameters      : struct TASK *t                                    *
 *   Purpose         :                                                   *
 *                                                                       *
\*************************************************************************/
BYTE get_task_pri( struct TASK *t )             
{
	BYTE pri;
    
	if( t )
	{
		pri = t->pri;  // pri = 1--255
		return pri;
	}
    return 0;
}
/*************************************************************************\
 *                                                                       *
 *   Function name   : setTaskPri                                        *
 *   Returns         : BYTE                                              *
 *   Parameters      : struct TASK *t ,BYTE pri                          *
 *   Purpose         :                                                   *
 *                                                                       *
\*************************************************************************/
BYTE set_task_pri( struct TASK *t, BYTE pri )             
{
	if( t && pri )
	{
		t->pri = pri;
		return  pri;  // pri = 1--255
	}
    return  0;
}
/*************************************************************************\
 *                                                                       *
 *   Function name   : get_curr_task                                     *
 *   Returns         : void                                              *
 *   Parameters      : none                                              *
 *   Purpose         : get current task                                  *
 *                                                                       *
\*************************************************************************/
struct TASK* get_curr_task(void)
{
    return curr_task;
}
/*************************************************************************\
 *                                                                       *
 *   Function name   : scheduler                                         *
 *   Returns         : NONE                                              *
 *   Parameters      : NONE                                              *
 *   Purpose         : scheduler Task                                    *
 *                                                                       *
\*************************************************************************/
void scheduler(void)
{
           BYTE  ret  = 0;
    struct TASK *t; 
    static BYTE  cpri = 0;
		 
    if( sysCallback )
    {
        sysCallback();
    }
	
    t = curr_task;
	
    if( ( t->pTask ) && ( cpri%(t->pri) == 0 ) )
    {
        ret = (t->pTask)( &t->pt );
    }
	curr_task = (struct TASK*)list_item_next(t);
    if( !curr_task )
	{
		curr_task = (struct TASK*)list_head(tasklist);
        cpri++;
		CLEAR_SYS_WDT();
	}	
    if( ret >= uPT_EXITED )
	{
		task_remove(t);
	}
}

///////////////////////////////////////////////////////////////////////////

