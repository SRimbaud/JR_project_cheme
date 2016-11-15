
/**
 * @file read.h
 * @author François Cayre <cayre@yiking.(null)>
 * @date Fri Jun 15 18:30:11 2012
 * @brief Read routine for SFS.
 *
 * Read routine for SFS.
 */

#ifndef _READ_H_
#define _READ_H_

#ifdef __cplusplus
extern "C" {
#endif
#include <stdio.h>
#include <ctype.h>
#include "eval.h"
#include"number.h"
#include "object.h"

typedef struct object_t* object ;
extern object nil;

enum {S_OK, S_KO, S_END};
/* Remplis input avec la première S-expression trouvée dans FILE.
 * Renvoie S_OK si une apparemment valide est trouvée.
 * Renvoie s_KO si pas de valide
 * Renvoie S_END si pas de carac utile avant fin de fichier.
 */
uint   sfs_get_sexpr( string input, FILE *fp );


object sfs_read( char *input, uint *here ); /* Lis une ligne de l'interpréteur.*/
object sfs_read_atom( char *input, uint *here );/* Lis un atome.*/
object sfs_read_pair( char *stream, uint *i );/* Lis une paire.*/

/* Fonctions de lecture d'atomes */
/** @fn  int sfs_type_atom(char*)
 * @brief Donne le type de la chaine de caractère.
 * @param chaine : chaine dont on cherche le type.
 * @param here : curseur sur la chaine.
 * @return : Renvoie le type de la chaine. Si type inconnu renvoie SFS_UNKNOWN.
 */
int sfs_type_atom(char* chaine, uint* here);

/*Mise à jour données sur OBJECT */

int OBJECT_int_update(object o, char* input, uint *here);
int OBJECT_real_update(object o, char* input, uint *here);
int OBJECT_character_update(object o, char* input, uint* here);
int OBJECT_string_update(object o, char* input, uint* here);
int OBJECT_boolean_update(object* o, char* input, uint* here);
int OBJECT_symbol_update(object o, char* input, uint*here);

#ifdef __cplusplus
}
#endif

#endif /* _READ_H_ */
