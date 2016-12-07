#include"primitive.h"

/** @fn void init_primitive()
 * @brief Initialise l'environnement courant avec toutes les 
 * primitives.
 *
 * Primitives implémentées le 24 Novembre 2016 :
 *
 * - +
 * - -
 * - *
 * - /
 * - remainder
 * - quotient
 * - <
 * - >
 * - <=
 * - >=
 * - =
 * - null?
 * - boolean?
 * - symbol?
 * - integer?
 * - real?
 * - char?
 * - string?
 * - pair?
 * - list?
 * - car
 * - cdr
 * - set-car!
 * - set-cdr!
 * - list
 * - cons
 * - eq?
 * - char->integer
 * - integer->char
 * - string->symbole
 * - symbole->string
 * - string->number
 * - number->string
 * - string->integer
 * - integer->string
 * - string->real
 * - real->string
 *
 *
 */
void init_primitive()
{
	PRIM_make( "+", PRIM_somme);
	PRIM_make("-" ,  PRIM_soustrait);
	PRIM_make("/" ,  PRIM_divise);
	PRIM_make("*" ,  PRIM_multiplie);
	PRIM_make("remainder", PRIM_remainder);
	PRIM_make("quotient", PRIM_quotient);
	PRIM_make("<", PRIM_is_inf);
	PRIM_make("<=", PRIM_is_inf_equal);
	PRIM_make(">", PRIM_is_sup);
	PRIM_make(">=", PRIM_is_sup_equal);
	PRIM_make("=", PRIM_is_equal);
	PRIM_make("null?", PRIM_is_null);
	PRIM_make("boolean?", PRIM_is_boolean);
	PRIM_make("symbol?", PRIM_is_symbol);
	PRIM_make("integer?", PRIM_is_integer);
	PRIM_make("real?", PRIM_is_real);
	PRIM_make("char?", PRIM_is_char);
	PRIM_make("string?", PRIM_is_string);
	PRIM_make("pair?", PRIM_is_pair);
	PRIM_make("list?", PRIM_is_list);
	PRIM_make("car", PRIM_car);
	PRIM_make("cdr", PRIM_cdr);
	PRIM_make("set-car!", PRIM_set_car);
	PRIM_make("set-cdr!", PRIM_set_cdr);
	PRIM_make("list", PRIM_list);
	PRIM_make("cons", PRIM_cons);
	PRIM_make("eq?", PRIM_eq);
	PRIM_make("char->integer",PRIM_char_to_integer);
	PRIM_make("integer->char",PRIM_integer_to_char);
	PRIM_make("string->symbol",PRIM_string_to_symbol);
	PRIM_make("symbol->string",PRIM_symbol_to_string);
	PRIM_make("string->number",PRIM_string_to_number);
	PRIM_make("number->string",PRIM_number_to_string);
	
	PRIM_make("string->integer",PRIM_string_to_integer);
	PRIM_make("integer->string",PRIM_integer_to_string);
	PRIM_make("string->real", PRIM_string_to_real);
	PRIM_make("real->string", PRIM_real_to_string);
	INFO_MSG("Primitives initiated in top level ENV");
}



/** @fn object PRIM_make(char* name, ptr_primitive function);
 * @brief Ajoute dans l'environnement la primitive name.
 *
 * La primitive est ajoutée dans l'environnement en tant que variable.
 * Celle ci aura le nom name et aura comme valeur un object
 * de type sfs_prim. Cet object contient un pointeur
 * de fonction sur la primitive voulue.
 * L'environnement est la variable globale env.
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
 * Evalue une liste d'object.
 *
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

/** @fn int PRIM_check_number_arg(object args, int number);
 * @brief Vérifie que args contient bien number arguments.
 *
 * En pratique on vérifie qu'on a bien number car dans notre
 * liste d'arguments. Si c'est pas le cas on renvoie 0.
 * Si le nombre d'arguments ne correspond pas on affiche
 * un warning expliquant le nombre attendu et celui donné.
 *
 * @return 1 si on a number args, 0 sinon.
 */
int PRIM_check_number_arg(object args, int number)
{
	object i = args;
	int j =0 ;
	for(i = args, j = 0; !OBJECT_isempty(i) && check_type(i, SFS_PAIR);
			i=i->this.pair.cdr, j++)
	{}
	if(number == j) return(1);
	WARNING_MSG("%d arguments expected but %d given.", number, j); 
	return(0);
}

/** @fn int PRIM_check_enought_number_arg(object arg, int number)
 * @brief Vérifie que l'on a au moins number arguments.
 *
 * Fonctionnement similaire à PRIM_check_number_arg(object arg, int number)
 * @return Renvoie 1 si il y a le bon nombre d'arguments 0 sinon.
 */
int PRIM_check_enought_number_arg(object arg, int number)
{
	object i = arg;
	if(i->this.pair.cdr == nil)
	{
		WARNING_MSG("Expected at least %d arguments", number);
		return(0);
	}
	int j =0 ;
	for(i = arg, j = 0; !OBJECT_isempty(i) && check_type(i, SFS_PAIR);
			i=i->this.pair.cdr, j++)
	{}
	if(j >= number) return(1);
	WARNING_MSG("at least : %d arguments expected but %d given.", number, j); 
	return(0);
}

/*========== Predicats============ */
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
 * Renvoie NULL si a n'est pas une paire.
 *
 * @return Renvoit le car de a.
 */
object PRIM_check_predicate(object a)
{
	/* On vérifie qu'on ait bien une paire */
	if(!check_type(a, SFS_PAIR))
	{
		WARNING_MSG("Error in predicate, need 1 argument");
		return(NULL);
	}
	if(!check_type(a->this.pair.cdr, SFS_NIL))
	{
		WARNING_MSG("Too many arguments one needed.\nContinuing on first one");
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
 * Renvoie NULL en cas d'erreur dans la somme.
 *
 * @sa OBJECT_add
 *
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
			return(NULL);
		}
	}	
	return(result);
}

/** @fn object PRIM_soustrait(object a) 
 * @brief Calcul la soustraction de tous les objects de a.
 *
 * Fonctionnement similaire a PRIM_somme()
 * Si a vaut nil ==> renvoie nil.
 *
 * @sa PRIM_somme(object a )
 * @sa OBJECT_sub
 *
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
			return(NULL);
		}
	}	
	return(result);
}

/** @fn object PRIM_multiplie(object a) 
 * @brief Calcul le produit de tous les objects a.
 *
 * Si a est nil ou NULL retourne un object valant 1.
 * (élément neutre du produit).
 *
 * @sa PRIM_somme()
 * @sa OBJECT_mul
 *
 * @return Renvoie un object alloué qui contient le résultat.
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
			return(NULL);
		}
	}	
	return(result);
}

/** @fn object PRIM_divise(object a)
 * @brief Calcule le quotient de tous les objets.
 *
 * Si a contient a b c d renvoie a/b/c/d.
 *
 * Renvoie NULL si erreur.
 *
 * @sa OBJECT_div
 * @sa OBJECT_add
 * @return Renvoie le résultat.
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
			return(NULL);
		}
	}	
	return(result);
}

/** @fn object PRIM_quotient(object a)
 * @brief Effectue la division des arguments de a.
 *
 * Les arguments doivent être entier.
 * Calcule le quotient d'entier uniquement en faisant
 * appel à OBJECT_div.
 *
 * @sa PRIM_divise.
 * @sa OBJECT_quotient
 *
 * @return Renvoie l'object résultat.
 */
object PRIM_quotient(object a)
{
	/* On se content d'effectuer une division sur des 
	 * entiers on va donc se contenter de vérifier
	 * que les nombre sont bien des entiers.
	 */

	if(a == nil) return(a);
	object result = OBJECT_build_cpy(a->this.pair.car) ;
	object terme = a ;
	for( terme = a->this.pair.cdr; terme != nil && !OBJECT_isempty(terme);
			terme = terme->this.pair.cdr)
	{
		/* On test si c'est un entier */
		
		result = OBJECT_quotient(result, terme->this.pair.car, result);
		if(result == nil || OBJECT_isempty(result) )
		{
			/* Cas ou la somme fail */
			return(NULL);
		}
		
	}	
	return(result);
}

/** @fn object PRIM_remainder(object a)
 * @brief Calcul le reste de la division euclidienne
 * des objets de a.
 *
 * Prend une infinité d'arguments.
 * Si un des arguments n'est pas un entier retourne NULL.
 * Calcul la division du premier pas le second puis le resultat
 * par le troisième le résultat pas le quatrième ....
 *
 * @sa PRIM_sum
 * @sa OBJECT_modulo
 *
 * @return Renvoie un SFS_NUMBER entier résultat.
 */
object PRIM_remainder(object a)
{
	if(a == nil) return(a);
	object result = OBJECT_build_cpy(a->this.pair.car) ;
	object terme = a ;
	if(a->this.pair.car->this.number.numtype != NUM_INTEGER)
	{
		WARNING_MSG("remainder : Args should be integer");
		return (NULL);
	}
	for( terme = a->this.pair.cdr; terme != nil && !OBJECT_isempty(terme);
			terme = terme->this.pair.cdr)
	{
		result = OBJECT_modulo(result, terme->this.pair.car, result);
		if(result == nil || OBJECT_isempty(result) )
		{
			/* Cas ou la somme fail */
			return(NULL);
		}
	}	
	return(result);
}
/* Comparaisons */


/** @fn object PRIM_is_equal(object a);
 * @brief Compare le premier argument de a à tous les autres.
 *
 * Affiche un WARNING et renvoie NULL si un des arguments n'est pas
 * un SFS_NUMBER.
 *
 * @sa OBJECT_cmp_is_equal
 * @return Renvoie vrai si tous les arguments sont égaux faux sinon. Renvoie 1 si un des arguments
 * n'est pas entier.
 */
object PRIM_is_equal(object a)
{
	if(!PRIM_check_enought_number_arg(a, 2))
	{
		return(NULL);
	}

	object terme = a ;
	object reference = OBJECT_get_cxr(a, "car");
	int result = 0;
	int flag = 0;
	for( terme = a->this.pair.cdr; terme != nil && !OBJECT_isempty(terme);
			terme = terme->this.pair.cdr)
	{
		/* On test si c'est un entier */

		result = OBJECT_cmp_is_equal(reference
				, terme->this.pair.car, &flag);

		if(flag) return(NULL);	
		if(!result) return(faux);

	}
	/* Si on arrive ici on a tout vrai */
	return(vrai);

}
/** @fn object PRIM_is_sup(object a)
 * @brief Regarde si le premier est supérieur aux autres.
 *
 * Regarde si le premier éléments est supérieur aux autres.
 * Controle qu'il y a ait au moins 2 arguments.
 * Renvoie NULL si mauvais nombre d'arguments.
 * Si il y a un problème dans la comparaison un warning est 
 * affiché et renvoie NULL.
 *
 * @sa OBJECT_is_inf
 * @sa PRIM_check_enought_number_arg()
 *
 * @return Vrai si premier arg supérieur aux autres, faux sinon.
 */
object PRIM_is_sup(object a)
{
	if(a==nil) return(NULL);
	if(!PRIM_check_enought_number_arg(a, 2))
	{
		return(NULL);
	}

	object terme = a ;
	object reference = OBJECT_get_cxr(a, "car");
	int result = 0;
	int flag = 0;
	for( terme = a->this.pair.cdr; terme != nil && !OBJECT_isempty(terme);
			terme = terme->this.pair.cdr)
	{
		/* On test si c'est un entier */

		result = OBJECT_cmp_is_sup(reference
				, terme->this.pair.car, &flag);

		if(flag) return(NULL);	
		if(!result) return(faux);

	}
	/* Si on arrive ici on a tout vrai */
	return(vrai);
}
/** @fn object PRIM_is_inf(object a)
 * @brief Regarde si le premier argument est inférieur à tous les autres.
 *
 * @sa PRIM_is_sup()
 *
 * @return Renvoie vrai si inf à tous les autres faux sinon. NULL si erreur.
 */
object PRIM_is_inf(object a)
{
	if(a==nil) return(NULL);
	if(!PRIM_check_enought_number_arg(a, 2))
	{
		return(NULL);
	}

	object terme = a ;
	object reference = OBJECT_get_cxr(a, "car");
	int result = 0;
	int flag = 0;
	for( terme = a->this.pair.cdr; terme != nil && !OBJECT_isempty(terme);
			terme = terme->this.pair.cdr)
	{
		/* On test si c'est un entier */

		result = OBJECT_cmp_is_inf(reference
				, terme->this.pair.car, &flag);

		if(flag) return(NULL);	
		if(!result) return(faux);

	}
	/* Si on arrive ici on a tout vrai */
	return(vrai);
}

/** @fn object PRIM_is_sup_equal(object a)
 * @brief Regarde si le premier arg est supérieur ou égal à tous les autres.
 *
 * @sa PRIM_is_sup()
 *
 * @return Renvoie vrai si le premier arg est supérieur ou égal à tous les autres, faux sinon. Renvoie NULL si erreur.
 */
object PRIM_is_sup_equal(object a)
{
	if(a==nil) return(NULL);
	if(!PRIM_check_enought_number_arg(a, 2))
	{
		return(NULL);
	}

	object terme = a ;
	object reference = OBJECT_get_cxr(a, "car");
	int result = 0;
	int flag = 0;
	for( terme = a->this.pair.cdr; terme != nil && !OBJECT_isempty(terme);
			terme = terme->this.pair.cdr)
	{
		/* On test si c'est un entier */

		result = OBJECT_cmp_is_sup_equal(reference
				, terme->this.pair.car, &flag);

		if(flag) return(NULL);	
		if(!result) return(faux);

	}
	/* Si on arrive ici on a tout vrai */
	return(vrai);
}
/** @fn object PRIM_is_inf_equal(object a)
 * @brief Regarde si le premier arguent est inférieur ou égal à tous les autres.
 *
 * @sa PRIM_is_sup()
 *
 * @return Renvoie vrai si premier arg <= a tous les autres, faux sinon. Renvoie NULL si erreur.
 */
object PRIM_is_inf_equal(object a)
{
	if(a==nil) return(NULL);
	if(!PRIM_check_enought_number_arg(a, 2))
	{
		return(NULL);
	}

	object terme = a ;
	object reference = OBJECT_get_cxr(a, "car");
	int result = 0;
	int flag = 0;
	for( terme = a->this.pair.cdr; terme != nil && !OBJECT_isempty(terme);
			terme = terme->this.pair.cdr)
	{
		/* On test si c'est un entier */

		result = OBJECT_cmp_is_inf_equal(reference
				, terme->this.pair.car, &flag);

		if(flag) return(NULL);	
		if(!result) return(faux);

	}
	/* Si on arrive ici on a tout vrai */
	return(vrai);
}



/* =================== Manip listes ============== */

/** @fn object PRIM_car(object a);
 * @brief Renvoie le car de l'argument.
 *
 * En pratique on applique PRIM_check_predicate(), du coup
 * on renvoie exactement le cadr.
 * Renvoie NULL si mauvais nombre d'arguments.
 *
 * @sa OBJECT_get_cxr
 * @sa PRIM_checkpredicate
 *
 * @return Le car de a.
 */
object PRIM_car(object a)
{
	/* On fait face à la même pbmatique
	 * que pour les predicats du coup
	 * on recycle
	 */
	int test = PRIM_check_number_arg(a, 1);
	if(!test) return(NULL);
	return(OBJECT_get_cxr(a, "caar"));
}

/** @fn object PRIM_cdr(object a);
 * @brief Renvoie le cdr de a.
 *
 * En pratique on applique PRIM_check_predicate(), du coup
 * on renvoie exactement le cadr.
 * @n
 * Renvoie NULL si mauvais nombre d'arguments
 *
 * @sa OBJECT_get_cxr
 * @sa PRIM_check_number_arg
 *
 * @return car de a.
 */
object PRIM_cdr(object a)
{
	/* On fait face à la même pbmatique
	 * que pour les predicats du coup
	 * on recycle
	 */
	int test = PRIM_check_number_arg(a, 1);
	if(!test) return(NULL);
	return(OBJECT_get_cxr(a, "cdar"));
}

/** @fn object PRIM_set_car(object a);
 * @brief Met a jour le car du premier arg de a par le second de a.
 *
 * Cette fonction vérifie qu'elle ait le bon nombre d'arguments.
 *
 * @sa PRIM_check_number_arg
 * @sa OBJECT_get_cxr
 *
 * @return Renvoie NULL.
 */
object PRIM_set_car(object a)
{
	/* On fait face à la même pbmatique
	 * que pour les predicats du coup
	 * on recycle
	 */
	int test = PRIM_check_number_arg(a, 2);
	if(!test) return(NULL);
	object new = OBJECT_get_cxr(a, "cadr");
	/* Mise à jour du premier élément de a */
	OBJECT_set_cxr(a, new, "caar");
	return(NULL);
}

/** @fn object PRIM_set_cdr(object a);
 * @brief Met a jour le cdr du premier arg de a, avec son second.
 *
 * @sa PRIM_set_car
 *
 * @return Renvoie NULL.
 */
object PRIM_set_cdr(object a)
{
	/* On fait face à la même pbmatique
	 * que pour les predicats du coup
	 * on recycle
	 */
	int test = PRIM_check_number_arg(a, 2);
	if(!test) return(NULL);
	object new = OBJECT_get_cxr(a, "cadr");
	/* Mise à jour du premier élément de a */
	OBJECT_set_cxr(a, new, "cdar");
	return(NULL);
}


/** @fn object PRIM_list(object a);
 * @brief Construit un liste composée des arguments de a.
 *
 * En pratique la liste est déjà construite à la lecture,
 * on renvoie directement a.
 *
 * @return Renvoie a.
 */
object PRIM_list(object a)
{
	return(a);
}


/** @fn object PRIM_cons(object a);
 * @brief Construit une paire grâce aux 2 arguments de a.
 *
 * La paire ne contenir que deux arguments pas plus.
 * a doit posséder uniquement 2 arguments.
 *
 * @sa PRIM_check_number_arg
 * @sa OBJECT_build_pair
 * @sa OBJECT_get_cxr
 *
 * @return Renvoie une paire constituée des arguments de a.
 */
object PRIM_cons(object a)
{
	int test = PRIM_check_number_arg(a, 2);
	if(!test) return(NULL);
	return(OBJECT_build_pair(OBJECT_get_cxr(a, "car"), OBJECT_get_cxr(a, "cadr") ));
}


/** @fn object PRIM_eq?(object a);
 * @brief Vérifie ques les arguments désignent la même entité.
 *
 * Seulement 2 arguments.
 * Si les arguments sont égaux (type et valeur)
 * renvoie vrai, faux sinon.
 *
 * @sa OBJECT_isEqual
 * @return Vrai si tous les arguments sont de même entité, faux sinon.
 */
object PRIM_eq(object a)
{
	int test = PRIM_check_number_arg(a, 2);
	if(!test) return(NULL);
	if(OBJECT_isEqual(OBJECT_get_cxr(a, "car"), OBJECT_get_cxr(a, "cadr")))
		return(vrai);
	return(faux);

}

/* ========= Conversion de types ======= */

/** @fn int PRIM_check_converted_type(object converted,char* name_type, int type)
 * @brief Vérifie que converted est de type type.
 * @param name_type : nom en str du type utilisé lors de l'affichage.
 * @param type : Valeur du type (voir SFS_NUMBER...)
 * @param converted : Object à vérifier.
 *
 * Si il ne l'est pas affiche un message de warning indiquant
 * que le type qu'on souhaite convertir n'est pas le bon.
 *
 * @return
 */
int PRIM_check_converted_type(object converted,char* name_type, int type)
{
	if(!check_type(converted, type))
	{
		WARNING_MSG("Argument is not %s type", name_type);
		return(0);
	}
	return(1);
}



/** @fn object PRIM_char_to_integer(object a)
 * @brief Convertit en char en entier.
 *
 * Reçoit 1 argument.
 * Convertit le premier argument qui est un char
 * en entier. 
 * Ce sera son code ASCII.
 * Elle renvoie NULL si le premier argument n'est pas 
 * un char.
 * Renvoie NULL si nombre d'arguments différent de 1.
 *
 * @sa PRIM_check_converted_type
 *
 * @return Renvoie la conversion de l'object en integer.
 */
object PRIM_char_to_integer(object a)
{
	int test = PRIM_check_number_arg(a, 1);
	if(!test) return(NULL);
	
	object tmp= OBJECT_get_cxr(a, "car");
	if(!PRIM_check_converted_type(tmp, "char", SFS_CHARACTER)) return(NULL);

	int charac= (int) tmp->this.character;
	/* Conversion */
	tmp->type = SFS_NUMBER;
	NUM_build(&(tmp->this.number), &charac, NUM_INTEGER);
	return(tmp);	
}

/** @fn object PRIM_integer_to_char(object a)
 * @brief Convertit en char en entier.
 *
 * La conversion est effectuée comme en C 
 * en retirant les octets de poids les plus fort
 * et en gardant ceux de poids plus faibles.
 *
 * Fonctionnement similaire à PRIM_char_to_integer()
 *
 * @sa PRIM_check_converted_type
 * @sa PRIM_char_to_integer
 *
 * @return Renvoie l'object convertit.
 */
object PRIM_integer_to_char(object a)
{
	int test = PRIM_check_number_arg(a, 1);
	if(!test) return(NULL);
	
	object tmp= OBJECT_get_cxr(a, "car");
	if(!PRIM_check_converted_type(tmp, "int", SFS_NUMBER)) return(NULL);
	/* On vérifie qu'on essaie bien de convertir en entier */
	if(tmp->this.number.numtype != NUM_INTEGER)
	{
		WARNING_MSG("Cannot convert non integer type to char");
		return(NULL);
	}

	tmp->type = SFS_CHARACTER;
	tmp->this.character = (char) tmp->this.number.this.integer;
	return(tmp);
}

/** @fn object PRIM_string_to_symbol(object a)
 * @brief Convertit un symbole en string
 *
 * Au vu de l'implémentation actuelle object est une union
 * ou string et symbol sont des string soit char[256].
 * Autrement dit l'union fait que qu'on regarde sous forme
 * de string ou sous forme de symbole on voit la même chose.
 *
 * A modifier si on implémente une allocation dynamique.
 *
 * @sa PRIM_check_converted_type
 * @sa PRIM_char_to_integer
 *
 * @return Renvoie l'objet convertit.
 */
object PRIM_string_to_symbol(object a)
{
	int test = PRIM_check_number_arg(a, 1);
	if(!test) return(NULL);

	object converted = OBJECT_get_cxr(a, "car");
	if(!PRIM_check_converted_type(converted, "string", SFS_STRING)) return(NULL);
	converted->type = SFS_SYMBOL;
	return(converted);

}

/** @fn object PRIM_symbol_to_string(object a)
 * @brief Convertit un symbole en string.
 *
 * @sa PRIM_string_to_symbol()
 * @return Renvoie l'objet converti.
 */
object PRIM_symbol_to_string(object a)
{

	int test = PRIM_check_number_arg(a, 1);
	if(!test) return(NULL);

	object converted = OBJECT_get_cxr(a, "car");
	if(!PRIM_check_converted_type(converted, "symbol", SFS_SYMBOL)) return(NULL);
	converted->type = SFS_STRING;
	return(converted);
}

/** @fn object PRIM_string_to_integer(object a)
 * @brief Convertit un string en entier.
 *
 * Conversion d'un string en entier.
 * Utilisation de strtol.
 * Gère la conversion en infini si overflow.
 *
 * @sa PRIM_check_converted_type
 * @sa PRIM_char_to_integer
 *
 * @return Renvoie l'objet convertie.
 */
object PRIM_string_to_integer(object a)
{

	int test = PRIM_check_number_arg(a, 1);
	if(!test) return(NULL);

	object converted = OBJECT_get_cxr(a, "car");
	if(!PRIM_check_converted_type(converted, "string", SFS_STRING)) return(NULL);

	long int converted_string= strtol(converted->this.string, NULL, 10);
	NUM_build(&(converted->this.number), &converted_string, NUM_INTEGER);
	/* Normalement NUM_build gère l'overflow de strtod avec LONG_MAX
	 * et LONG_MIN
	 */
	converted->type = SFS_NUMBER;

	return(converted);
}

/** @fn object PRIM_integer_to_string(object a)
 * @brief Convertit un entier en en string.
 *
 * Utilise snprintf.
 *
 * @sa PRIM_check_converted_type
 * @sa PRIM_char_to_integer
 *
 * @return Renvoie l'objet convertit
 */
object PRIM_integer_to_string(object a)
{

	int test = PRIM_check_number_arg(a, 1);
	if(!test) return(NULL);

	object tmp = OBJECT_get_cxr(a, "car");
	if(!PRIM_check_converted_type(tmp, "int", SFS_NUMBER)) return(NULL);
	if(tmp->this.number.numtype != NUM_INTEGER)
	{
		WARNING_MSG("Trying to convert non integer type");
		return(NULL);
	}
	snprintf(tmp->this.string,STRLEN,"%ld",tmp->this.number.this.integer);
	tmp->type= SFS_STRING;
	return(tmp);
}

/** @fn object PRIM_string_to_real(object a)
 * @brief Convertit un string en réel.
 *
 * Accepte un argument de type string.
 * Fait appel à strtod.
 * Renvoie NULL en cas de problème.
 *
 * @sa PRIM_check_converted_type
 * @sa PRIM_char_to_integer
 *
 * @return Renvoie le réel convertit.
 */
object PRIM_string_to_real(object a)
{

	int test = PRIM_check_number_arg(a, 1);
	if(!test) return(NULL);

	object converted = OBJECT_get_cxr(a, "car");
	if(!PRIM_check_converted_type(converted, "string", SFS_STRING)) return(NULL);

	double converted_string= strtod(converted->this.string, NULL);
	NUM_build(&(converted->this.number), &converted_string, NUM_REAL);
	/* Normalement NUM_build gère l'overflow de strtod avec LONG_MAX
	 * et LONG_MIN
	 */
	converted->type = SFS_NUMBER;

	return(converted);
}

/** @fn object PRIM_real_to_string(object a)
 * @brief Convertit en reel en string.
 *
 * Fait appel à snprintf.
 * Renvoie NULL en cas de pb.
 *
 * @sa PRIM_check_converted_type
 * @sa PRIM_char_to_integer
 *
 * @return Renvoie la chaine de caractère.
 */
object PRIM_real_to_string(object a)
{

	int test = PRIM_check_number_arg(a, 1);
	if(!test) return(NULL);

	object tmp = OBJECT_get_cxr(a, "car");
	if(!PRIM_check_converted_type(tmp, "int", SFS_NUMBER)) return(NULL);
	if(tmp->this.number.numtype != NUM_REAL)
	{
		WARNING_MSG("Trying to convert non real type");
		return(NULL);
	}
	snprintf(tmp->this.string,STRLEN,"%lf",tmp->this.number.this.real);
	tmp->type= SFS_STRING;
	return(tmp);
}
/** @fn object PRIM_string_to_number(object a)
 * @brief Conversion d'une chaine en nombre (entier ou réel).
 *
 * Pour cette fonction on fait appel à OBJECT_update.
 * Renvoie NULL si trop d'arguments et si ce n'est pas
 * une chaine de caractère.
 * Max 1 argument.
 *
 * @sa PRIM_check_converted_type
 * @sa PRIM_char_to_integer
 *
 * @return Renvoie l'objet convertit.
 */
object PRIM_string_to_number(object a)
{
	int test = PRIM_check_number_arg(a, 1);
	if(!test) return(NULL);

	object converted = OBJECT_get_cxr(a, "car");
	if(!PRIM_check_converted_type(converted, "string", SFS_STRING)) return(NULL);

	int here = 0;
	/* On utilise OBJECT_int_update qui contient déjà tout*/
	OBJECT_int_update(converted, converted->this.string, &here);
	converted->type = SFS_NUMBER;
	return(converted);
}
/** @fn }object PRIM_number_to_string(object a)
 * @brief Convertit un number en chaine.
 *
 * Fait appel à snprintf.
 * Renvoie NULL si 
 * - Plus d'un argument.
 * - Argument n'est pas un number.
 *
 *   @sa PRIM_check_converted_type
 *   @sa PRIM_char_to_integer
 *
 * @return Renvoie le resultat de la conversion.
 */
object PRIM_number_to_string(object a)
{
	int test = PRIM_check_number_arg(a, 1);
	if(!test) return(NULL);

	object tmp = OBJECT_get_cxr(a, "car");
	if(!PRIM_check_converted_type(tmp, "number", SFS_NUMBER)) return(NULL);
	if(tmp->this.number.numtype == NUM_REAL)
	{	
		snprintf(tmp->this.string,STRLEN,"%lf",tmp->this.number.this.real);
		tmp->type= SFS_STRING;
	}
	else if(tmp->this.number.numtype == NUM_INTEGER)
	{

		snprintf(tmp->this.string,STRLEN,"%ld",tmp->this.number.this.integer);
		tmp->type= SFS_STRING;
	}
	else
		tmp->type = SFS_UNKNOWN;

	return(tmp);
}
