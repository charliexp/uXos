/*************************************************************************\
 * File Name    : buffer.h                                               *
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

#ifndef BUFFER_H_
#define BUFFER_H_

#define BUFFER_SIZE 1024

struct Buffer
{
	int size;
	char data[BUFFER_SIZE];
};

void buffer_init( struct Buffer *buf );
int  buffer_add( struct Buffer *buf, const char *tmp, int len);
int  buffer_del( struct Buffer *buf, int len );

#endif

