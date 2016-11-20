#ifndef PRIMITIVE_H
#define PRIMITIVE_H

/** @file primitive.h
 * @brief Fonctions d'implémentation des primitives.
 *
 * Ensemble de fonctions permettant de coder les primitives.
 * On retrouvera le constructeur de la liste globale
 * regroupant toutes les primitives possibles.
 * On aura les fonctions correspondant à chaque 
 * primitive.
 *
 * @author Rimbaud Sébatien.
 * @date 16 novembre 2016
 */

#include"object.h"
#include"eval.h"

#ifdef __cplusplus
extern "C" {
#endif
#define NB_PRIMITIVE 4
typedef enum liste_primitive PRIMIVE ;
enum liste_primitive 
{
	ADD, 
	SUB,
	DIV,
	MUL
};

/* Initialise tableau primitives */
void init_primitive();
typedef object(*ptr_primitive)(object) ;

/* Repèrage de priiimiiiiiveuuuuh */

object PRIM_make(char* name, ptr_primitive function);

/* Primitives */

object PRIM_eval(const object args);
/* Predicats */
object PRIM_check_predicate(object a);
object PRIM_is_null(object a);
object PRIM_is_boolean(object a);
object PRIM_is_symbol(object a);
object PRIM_is_integer(object a);
object PRIM_is_real(object a);
object PRIM_is_char(object a);
object PRIM_is_string(object a);
object PRIM_is_pair(object a);
object PRIM_is_list(object a);


/* Opérateurs numériques */

object PRIM_somme(object a) ;
object PRIM_soustrait(object a) ;
object PRIM_multiplie(object a) ;
object PRIM_divise(object a);

/* Extern */
extern object primitive_name[NB_PRIMITIVE] ;
extern ptr_primitive primitive_function [NB_PRIMITIVE];
extern object nil;
extern object vrai;
extern object faux;





#ifdef __cplusplus
}
#endif

#endif
