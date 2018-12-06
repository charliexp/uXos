/*************************************************************************\
 * File Name    : buffer.c                                               *
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

#include <string.h>
#include "buffer.h"


//////////////////////////////////////////////////////////////////////////
void buffer_init( struct Buffer *buf )
{
	buf->size = 0;
}

//////////////////////////////////////////////////////////////////////////
int buffer_add( struct Buffer *buf, const char *tmp, int len )
{
	if( buf->size + len > BUFFER_SIZE )
	{
		return -1;
	}
	memcpy( buf->data + buf->size, tmp, len );
	buf->size += len;
	return len;
}

//////////////////////////////////////////////////////////////////////////
int buffer_del( struct Buffer *buf, int len )
{
	int size = buf->size - len;
	if( size < 0 )
	{
		return -1;
	}
	memcpy( buf->data, buf->data + len, size );
	buf->size -= len;
	return len;
}

