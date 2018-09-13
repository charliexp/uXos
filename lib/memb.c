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
 *                                                                       *
\*************************************************************************/

 /**
 * \file
 * Memory block allocation routines.
 * \author Adam Dunkels <adam@sics.se>
 */
#include <string.h>
#include "memb.h"

///////////////////////////////////////////////////////////////////////////////
void memb_init(struct memb *m)
{
	memset(m->count, 0, m->num);
	memset(m->mem, 0, m->size * m->num);
}

///////////////////////////////////////////////////////////////////////////////
void *memb_alloc(struct memb *m)
{
	int i;

	for( i = 0; i < m->num; ++i ) 
	{
		if(m->count[i] == 0) 
		{
		/*  If this block was unused, we increase the reference count to
			indicate that it now is used and return a pointer to the
			memory block. */
			++(m->count[i]);
			return (void *)((char *)m->mem + (i * m->size));
		}
	}

	/*  No free block was found, so we return NULL to indicate failure to
		allocate block. */
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
signed char memb_free(struct memb *m, void *ptr)
{
	int i;
	char *ptr2;

	/* Walk through the list of blocks and try to find the block to
       which the pointer "ptr" points to. */
	ptr2 = (char *)m->mem;
	for(i = 0; i < m->num; ++i) 
	{
		if(ptr2 == (char *)ptr) 
		{
			/* We've found to block to which "ptr" points so we decrease the
			reference count and return the new value of it. */
			if(m->count[i] > 0) 
			{
				/* Make sure that we don't deallocate free memory. */
				--(m->count[i]);
			}
			return m->count[i];
		}
		ptr2 += m->size;
	}
	return -1;
}

///////////////////////////////////////////////////////////////////////////////
int memb_inmemb(struct memb *m, void *ptr)
{
	return (char *)ptr >= (char *)m->mem &&
		   (char *)ptr < (char *)m->mem + (m->num * m->size);
}

///////////////////////////////////////////////////////////////////////////////
int memb_numfree(struct memb *m)
{
	int i;
	int num_free = 0;

	for(i = 0; i < m->num; ++i) 
	{
		if(m->count[i] == 0) 
		{
			++num_free;
		}
	}
	return num_free;
}
///////////////////////////////////////////////////////////////////////////////
/** @} */
