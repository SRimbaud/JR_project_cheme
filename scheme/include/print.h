
/** * @file print.h
 * @author François Cayre <cayre@yiking.(null)>
 * @date Fri Jun 22 20:12:52 2012
 * @brief Printing stuff for SFS.
 *
 * Printing stuff for SFS.
 */

#ifndef _PRINT_H_
#define _PRINT_H_

#ifdef __cplusplus
extern "C" {
#endif
#include "object.h"

typedef struct object_t* object ;
void sfs_print( object  o);/* Affiche un objet. 1er incrément. */

void sfs_print_atom( object o ); /* Affiche un atome.1er incrément.*/
void sfs_print_pair( object o );/* Affiche une paire.1er incrément.*/
/* Fonctions d'affichage pour chaque type
 */

/*Affichage*/
void OBJECT_print_number(object o);
void OBJECT_print_character(object o);
void OBJECT_print_string(object o);
void OBJECT_print_symbol(object o);
void OBJECT_print_bool(object o);
void OBJECT_print_nil(int place);
void OBJECT_print_fail();
void OBJECT_print_pair(object o);
void OBJECT_print_atom(object o);

#ifdef __cplusplus
}
#endif

#endif /* _PRINT_H_ */
