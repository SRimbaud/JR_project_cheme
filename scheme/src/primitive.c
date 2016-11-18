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
	value->this.prim.function = function ;
	value->this.prim.name = nom ;
	return(ENV_add_var(nom, value));
}


/* Primitives */

/** @fn object PRIM_eval(const object args)
 * @brief Evalue la liste d'arguments passé en paramètre.
 *
 * Effectue une copie des arguments dans la mémoire.
 * Les arguments ne sont donc pas modifiés.
 * @return Renvoie La liste des arguments évalué.
 */
object PRIM_eval(const object args)
{
	DEBUG_MSG("Begin evaluating primitiv arguments");
	object evaluated = OBJECT_build_cpy(args);
	object i = evaluated;
	/* i est normalement une paire dont on évalue le car et
	 * dont on prend le cdr pour itérer.
	 */
	for(i = evaluated ; i!=nil && !OBJECT_isempty(i);
		i = i->this.pair.cdr)
	{
		if(!check_type(i, SFS_PAIR))
		{
			/* Gestion d'un cons */
			i = sfs_eval(i);
		}
		else
		{
			i->this.pair.car = sfs_eval(i->this.pair.car);
		}
	}	
	return(evaluated);
}

/* Opérateurs mathématiques */
/** @fn object PRIM_somme(object a) 
 * @brief Somme une liste d'objet.
 *
 * On effectue une copie du premier élément de a. (son car).
 * On sommera ensuite tous les autres éléments dans celui-ci.
 * Si a est NULL ou nil on alloue un object valant 0.
 * @return Renvoie la somme des objets.
 */
object PRIM_somme(object a) 
{
	if(a == nil || OBJECT_isempty(a))
	{
		/* Dans ce cas on renvoie 0 */
		a = make_object(SFS_NUMBER);
		NUM_build(&(a->this.number), NULL, NUM_INTEGER);
		return(a);
	}
	object result = OBJECT_build_cpy(a->this.pair.car) ;
	object terme = a ;
	for( terme = a->this.pair.cdr; terme != nil && !OBJECT_isempty(terme);
		terme = terme->this.pair.cdr)
	{
		result = OBJECT_add(result, terme->this.pair.car, result);
		if(result == nil || OBJECT_isempty(result) )
		{
			/* Cas ou la somme fail */
			return(nil);
		}
	}	
	return(result);
}

/** @fn object PRIM_soustrait(object a) 
 * @brief Calcul la soustraction de tous les objects de a.
 *
 * Fonctionnement similaire a PRIM_somme()
 * Si a vaut nil ==> renvoie nil.
 * @sa PRIM_somme(object a )
 * @return Renvoie un object alloué qui contient le résultat.
 */
object PRIM_soustrait(object a) 
{
	if(a == nil) return(a);
	object result = OBJECT_build_cpy(a->this.pair.car) ;
	object terme = a ;
	for( terme = a->this.pair.cdr; terme != nil && !OBJECT_isempty(terme);
			terme = terme->this.pair.cdr)
	{
		result = OBJECT_sub(result, terme->this.pair.car, result);
		if(result == nil || OBJECT_isempty(result) )
		{
			/* Cas ou la somme fail */
			return(nil);
		}
	}	
	return(result);
}

/** @fn object PRIM_multiplie(object a) 
 * @brief Calcul le produit de tous les objects a.
 *
 * Si a est nil ou NULL retourne un object valant 1.
 * (élément neutre du produit).
 * @sa PRIM_somme()
 *
 * @return
 */
object PRIM_multiplie(object a) 
{
	if(a == nil || OBJECT_isempty(a))
	{
		/* Dans ce cas on renvoie 0 */
		a = make_object(SFS_NUMBER);
		long int neutral_elt = 1;
		NUM_build(&(a->this.number), &neutral_elt, NUM_INTEGER);
		return(a);
	}
	object result = OBJECT_build_cpy(a->this.pair.car) ;
	object terme = a ;
	for( terme = a->this.pair.cdr; terme != nil && !OBJECT_isempty(terme);
		terme = terme->this.pair.cdr)
	{
		result = OBJECT_mul(result, terme->this.pair.car, result);
		if(result == nil || OBJECT_isempty(result) )
		{
			/* Cas ou la somme fail */
			return(nil);
		}
	}	
	return(result);
}

/** @fn object PRIM_divise(object a)
 * @brief
 *
 * @return
 */
object PRIM_divise(object a)
{
	if(a == nil) return(a);
	object result = OBJECT_build_cpy(a->this.pair.car) ;
	object terme = a ;
	for( terme = a->this.pair.cdr; terme != nil && !OBJECT_isempty(terme);
			terme = terme->this.pair.cdr)
	{
		result = OBJECT_div(result, terme->this.pair.car, result);
		if(result == nil || OBJECT_isempty(result) )
		{
			/* Cas ou la somme fail */
			return(nil);
		}
	}	
	return(result);
}

