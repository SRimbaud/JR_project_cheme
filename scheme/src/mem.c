
/**
 * @file mem.c
 * @author François Cayre <cayre@yiking.(null)>
 * @date Fri Jun 15 18:06:23 2012
 * @brief Memory wrappers for malloc/free.
 *
 * Memory wrappers for malloc/free.
 */

#include "mem.h"


/** @fn  void *sfs_malloc( size_t size ) {
 * @brief alloue un bloc de taille t
 *
 * @return Pointeur sur bloc de taille t.
 */
void *sfs_malloc( size_t size )
{

    return calloc(1, size );
}

/** @fn  void  sfs_free( void *ptr ) {
 * @brief libère ptr
 *
 */
void  sfs_free( void *ptr )
{

    free( ptr );
}

