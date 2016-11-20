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
	PRIM_make("null?", PRIM_is_null);
	PRIM_make("boolean?", PRIM_is_boolean);
	PRIM_make("symbol?", PRIM_is_symbol);
	PRIM_make("integer?", PRIM_is_integer);
	PRIM_make("real?", PRIM_is_real);
	PRIM_make("char?", PRIM_is_char);
	PRIM_make("string?", PRIM_is_string);
	PRIM_make("pair?", PRIM_is_pair);
	PRIM_make("list?", PRIM_is_list);


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

/* Predicats */
/* Chaque predicat reçoit en paramètre une liste 
 * d'argument.
 * On s'intéresse donc au car de cette liste.
 */


/** @fn object PRIM_check_predicate(object a);
 * @brief Met en forme le predicat en prenant le car de a.
 * @param Flag mis a 1 si il y a plus d'un paramètre.
 *
 * Vérifie également qu'il y a bien qu'un seul argument
 * et non 2.
 *
 * @return Renvoit le car de a.
 */
object PRIM_check_predicate(object a)
{
	/* On vérifie qu'on ait bien une paire */
	if(!check_type(a, SFS_PAIR))
	{
		WARNING_MSG("Error in predicate");
		return(nil);
	}
	if(!check_type(a->this.pair.cdr, SFS_NIL))
	{
		WARNING_MSG("Too many arguments for predicate\nContinuing on first one");
	}
	return(OBJECT_get_cxr(a, "car"));
}

/** @fn object PRIM_is_null(object a);
 * @brief Vérifie si a est nil. Renvoie vrai ou faux.
 *
 * @return vrai si a nil, faux sinon.
 */
object PRIM_is_null(object a)
{
	a = PRIM_check_predicate(a);
	if(check_type(a, SFS_NIL)) return(vrai);
	if(a==nil) return(vrai);
	return(faux);
}

/** @fn object PRIM_is_boolean(object a);
 * @brief Vérifie si a est un booléen.
 *
 * @return Vrai si a booléen faux sinon.
 */
object PRIM_is_boolean(object a)
{
	
	a = PRIM_check_predicate(a);
	if(a==vrai || a==faux) return(vrai);
	return(faux);
}

/** @fn object PRIM_is_symbol(object a);
 * @brief Vérifie si a symbole.
 *
 * @return Vrai si a symbole, faux sinon.
 */
object PRIM_is_symbol(object a)
{
	a = PRIM_check_predicate(a);
	if(check_type(a, SFS_SYMBOL)) return(vrai);
	return(faux);
}

/** @fn object PRIM_is_integer(object a)
 * @brief Renvoie vrai si a est un entier faux sinon.
 *
 * En pratique si a n'est pas un type SFS_NUMBER renvoie
 * faux.
 *
 * @return Vrais si a entier faux sinon.
 */
object PRIM_is_integer(object a)
{
	a = PRIM_check_predicate(a);
	if(!check_type(a, SFS_NUMBER)) return(faux);
	if(a->this.number.numtype == NUM_INTEGER) return(vrai);
	return(faux);
}

/** @fn object PRIM_is_real(object a);
 * @brief Renvoie vrai si a réel, faux sinon.
 *
 * @sa PRIM_is_integer()
 *
 * @return Vrai si a réel, faux sinon.
 */
object PRIM_is_real(object a)
{
	a = PRIM_check_predicate(a);
	if(!check_type(a, SFS_NUMBER)) return(faux);
	if(a->this.number.numtype == NUM_REAL) return(vrai);
	return(faux);
}

/** @fn object PRIM_is_char(object a);
 * @brief Renvoie vrai si a char.
 *
 * @return Vrais si a char, faux sinon.
 */
object PRIM_is_char(object a)
{
	a = PRIM_check_predicate(a);
	if(check_type(a, SFS_CHARACTER)) return(vrai);
	return(faux);
}

/** @fn object PRIM_is_string(object a);
 * @brief Renvoie vrai si a string.
 *
 * @return Vrai si a string, faux sinon.
 */
object PRIM_is_string(object a)
{
	a = PRIM_check_predicate(a);
	if(check_type(a, SFS_STRING)) return(vrai);
	return(faux);
}


/** @fn object PRIM_is_pair(object a);
 * @brief Renvoie vrai si a pair.
 *
 * Une paire est constitué d'une paire
 * dont le car est le cdr ne sont pas des paires.
 * Notes importante une liste est aussi une paire.
 * En revanche une paire n'est pas une liste.
 *
 * @return Renvoie vrai si a paire, faux sinon.
 */
object PRIM_is_pair(object a)
{
	a = PRIM_check_predicate(a);
	if(check_type(a, SFS_PAIR)) return(vrai);
	return(faux);
}


/** @fn object PRIM_is_list(object a);
 * @brief Renvoie vrai si a une liste.
 *
 * Une paire n'est pas une liste on vérifie donc que le car
 * est bien de type sfs_pair.
 *
 * @return Vrai si a liste, faux sinon.
 */
object PRIM_is_list(object a)
{
	a = PRIM_check_predicate(a);
	if(!check_type(a, SFS_PAIR)) return(faux);
	if(!check_type(OBJECT_get_cxr(a, "cdr"), SFS_PAIR) ) return(faux);
	return(vrai);
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

