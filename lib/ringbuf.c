/*************************************************************************\
 * File Name    : ringbuf.c                                                 *
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
 * 04-09-2018     charlie_weng     V1.6          add ring buffer         * 
 *                                                                       *
\*************************************************************************/

#include <string.h>
#include "ringbuf.h"

//////////////////////////////////////////////////////////////////////////
//判断x是否是2的次方
#define is_power_of_2(x) 	((x) != 0 && (((x) & ((x) - 1)) == 0))
#define min(a, b) 			(((a) < (b)) ? (a) : (b))
/*************************************************************************\
 *                                                                       *
 *   Function name   : ringbuf_init                                      *
 *   Returns         : void                                              *
 *   Parameters      : size must be a power of 2                         *
 *   Purpose         :                                                   *
 *                                                                       *
\*************************************************************************/
BYTE ringbuf_init( struct ringbuf *r, void *dataptr, WORD len )
{
	if (!is_power_of_2(len))
		return 0;
	
	r->data    = dataptr;
	r->mask    = len - 1;
	r->put_ptr = 0;
	r->get_ptr = 0;
	
	return 1;
}
/*************************************************************************\
 *                                                                       *
 *   Function name   : ringbuf_put                                       *
 *   Returns         : void                                              *
 *   Parameters      : TASK*                                             *
 *   Purpose         :                                                   *
 *                                                                       *
\*************************************************************************/
WORD ringbuf_put( struct ringbuf *r, void* buffer, WORD len )
{
	WORD l = 0;
    
	len = min( len, r->mask + 1 - r->put_ptr + r->get_ptr );
	l   = min( len, r->mask + 1 - r->put_ptr & r->mask );
	
	memcpy( PTR_OFFSET( r->data, r->put_ptr & r->mask ), buffer, l );
    memcpy( r->data, PTR_OFFSET( buffer, l ), len - l  );

    r->put_ptr += len;
	
	return len;
}
/*************************************************************************\
 *                                                                       *
 *   Function name   : ringbuf_get                                       *
 *   Returns         : void                                              *
 *   Parameters      : TASK*                                             *
 *   Purpose         :                                                   *
 *                                                                       *
\*************************************************************************/
WORD ringbuf_get( struct ringbuf *r, void * buffer, WORD len  )
{
	WORD l = 0;
	
    len = min( len, r->put_ptr - r->get_ptr );     
    l   = min( len, r->mask + 1 - r->get_ptr & r->mask );
	
    memcpy( buffer, PTR_OFFSET( r->data, r->get_ptr & r->mask ), l );
    memcpy( PTR_OFFSET( buffer, l ), r->data, len - l );
	
    r->get_ptr += len;
		
    return len;
}
/*************************************************************************\
 *                                                                       *
 *   Function name   : ringbuf_size                                      *
 *   Returns         : size                                              *
 *   Parameters      : ringbuf                                           *
 *   Purpose         :                                                   *
 *                                                                       *
\*************************************************************************/
WORD ringbuf_size( struct ringbuf *r )
{
	return r->mask + 1;
}
/*************************************************************************\
 *                                                                       *
 *   Function name   : ringbuf_elements                                  *
 *   Returns         : num                                               *
 *   Parameters      : ringbuf                                           *
 *   Purpose         :                                                   *
 *                                                                       *
\*************************************************************************/
WORD ringbuf_elements( struct ringbuf *r )
{
	return r->put_ptr - r->get_ptr;
}

/*************************************************************************\
 *                                                                       *
 *   Function name   : ringbuf_elements                                  *
 *   Returns         : num                                               *
 *   Parameters      : ringbuf                                           *
 *   Purpose         :                                                   *
 *                                                                       *
\*************************************************************************/
WORD ringbuf_free_elements( struct ringbuf *r )
{
	return r->mask + 1 - r->put_ptr + r->get_ptr;
}


