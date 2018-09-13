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
 * \addtogroup mem
 * @{
 */


/**
 * \defgroup memb Memory block management functions
 *
 * The memory block allocation routines provide a simple yet powerful
 * set of functions for managing a set of memory blocks of fixed
 * size. A set of memory blocks is statically declared with the
 * MEMB() macro. Memory blocks are allocated from the declared
 * memory by the memb_alloc() function, and are deallocated with the
 * memb_free() function.
 *
 * @{
 */


/**
 * \file
 *         Memory block allocation routines.
 * \author
 *         Adam Dunkels <adam@sics.se>
 *
 */

#ifndef MEMB_H_
#define MEMB_H_

/**
 * Declare a memory block.
 *
 * This macro is used to statically declare a block of memory that can
 * be used by the block allocation functions. The macro statically
 * declares a C array with a size that matches the specified number of
 * blocks and their individual sizes.
 *
 * Example:
 \code
MEMB(connections, struct connection, 16);
 \endcode
 *
 * \param name The name of the memory block (later used with
 * memb_init(), memb_alloc() and memb_free()).
 *
 * \param structure The name of the struct that the memory block holds
 *
 * \param num The total number of memory chunks in the block.
 *
 */
 
 #define CC_CONCAT2(s1, s2) s1##s2
/**
 * A C preprocessing macro for concatenating two preprocessor tokens.
 *
 * We need use two macros (CC_CONCAT and CC_CONCAT2) in order to allow
 * concatenation of two \#defined macros.
 */
#define CC_CONCAT(s1, s2) CC_CONCAT2(s1, s2)
#define CC_CONCAT_EXT_2(s1, s2) CC_CONCAT2(s1, s2)


#define MEMB(name, structure, num) \
        static char CC_CONCAT(name,_memb_count)[num]; \
        static structure CC_CONCAT(name,_memb_mem)[num]; \
        static struct memb name = {sizeof(structure), num, \
                                          CC_CONCAT(name,_memb_count), \
                                          (void *)CC_CONCAT(name,_memb_mem)}

struct memb {
  unsigned short size;
  unsigned short num;
  char *count;
  void *mem;
};

/**
 * Initialize a memory block that was declared with MEMB().
 *
 * \param m A memory block previously declared with MEMB().
 */
void  memb_init(struct memb *m);

/**
 * Allocate a memory block from a block of memory declared with MEMB().
 *
 * \param m A memory block previously declared with MEMB().
 */
void *memb_alloc(struct memb *m);

/**
 * Deallocate a memory block from a memory block previously declared
 * with MEMB().
 *
 * \param m m A memory block previously declared with MEMB().
 *
 * \param ptr A pointer to the memory block that is to be deallocated.
 *
 * \return The new reference count for the memory block (should be 0
 * if successfully deallocated) or -1 if the pointer "ptr" did not
 * point to a legal memory block.
 */
signed char  memb_free(struct memb *m, void *ptr);

int memb_inmemb(struct memb *m, void *ptr);

int  memb_numfree(struct memb *m);

/** @} */
/** @} */

#endif /* MEMB_H_ */
