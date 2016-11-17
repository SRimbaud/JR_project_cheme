#include"primitive.h"

/** @fn void init_primitive()
 * @brief Initialise l'environnement courant avec toutes les 
 * primitives.
 *
 * Primitives implémentées le 17 Novembre 2016 :
 *
 * - +
 * - -
 * - *
 * - /
 *
 */
void init_primitive()
{
	PRIM_make( "+", PRIM_somme);
	PRIM_make("-" ,  PRIM_soustrait);
	PRIM_make("/" ,  PRIM_divise);
	PRIM_make("*" ,  PRIM_multiplie);
	INFO_MSG("Primitives initiated in top level ENV");
}



/** @fn object PRIM_make(char* name, ptr_primitive function);
 * @brief Ajoute dans l'environnement la primitive name.
 *
 * La primitive est ajoutée dans l'environnement en tant que variable.
 * Celle ci aura le nom name et aura comme valeur un object
 * de type sfs_prim. Cet object contient un pointeur
 * de fonction sur la primitive voulue.
 *
 * @return Renvoie un object de type primitive contenant function.
 */
object PRIM_make(char* name, ptr_primitive function)
{
	object nom = OBJECT_build_symbol(name);
	object value = make_object(SFS_PRIM);
	value->this.function = function ;
	return(ENV_add_var(nom, value));
}


/* Primitives */

/* Opérateurs mathématiques */
/** @fn object PRIM_somme(object a) 
 * @brief Somme une liste d'objet.
 *
 * Alloue dynamique le nouvel objet.
 * @return Renvoie la somme des objets.
 */
object PRIM_somme(object a) 
{
	
}

/** @fn object PRIM_soustrait(object a) 
 * @brief
 *
 * @return
 */
object PRIM_soustrait(object a) 
{
	return(NULL);
}
/** @fn object PRIM_multiplie(object a) 
 * @brief
 *
 * @return
 */
object PRIM_multiplie(object a) 
{
	return(NULL);
}
/** @fn object PRIM_divise(object a)
 * @brief
 *
 * @return
 */
object PRIM_divise(object a)
{
	return(NULL);
}
