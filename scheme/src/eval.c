
/**
 * @file eval.c
 * @author François Cayre <cayre@yiking.(null)>
 * @date Fri Jun 22 20:11:30 2012
 * @brief Outils d'évaluation pour interpréteurs et d'autres utilitaires.
 *
 * Evaluation stuff for SFS.
 */

#include "eval.h"

/** @fn object sfs_eval( object input ) 
 * @brief Fontion d'évalution d'expression scheme.
 *
 * Evaluation de l'input.
 * Renvoie NULL si :
 * input est NULL
 * - Si on a un symbole correspondant à un forme scheme seul.
 * - Si on donne une variable inexistante.
 * - Si on commence une liste avec un symbole
 * qui n'est ni un mot ni une variable.
 * - Si par inadvertance le car de input est NULL.
 *
 *
 * @sa is_form(object symbol, int* forme);
 *
 * @return Renvoie l'objet évalué.
 */
object sfs_eval( object input ) 
{
	/* La forme que l'on va identifier */
	FORMS forme = NONE ;
	/* la variable que l'on va chercher */
	object var = NULL;
	/* Vérif si on trouve bien var */
	int flag = FALSE;
	/* Vérif qu'on ait pas un input NULL */
	if(OBJECT_isempty(input))
	{ return NULL ; }


	/*return(input);*/
	/* On évalue seulement les objets de type pair,
	 * les autres sont des atomes on évalue pas
	 */
	object lecture = input;
	if ( !check_type(lecture, SFS_PAIR)
		       	&& !check_type(lecture, SFS_SYMBOL))
	{
		DEBUG_MSG( "Eval called on atom");
		return(lecture);
	}
	/* Si l'input est une primitive seule on a une erreur */
	if(check_type(input, SFS_PRIM))
	{
		DEBUG_MSG("Primitive detected");
		return(input);
	}
	/* Cas d'un symbole seul : On l'affiche */
	if(check_type(input, SFS_SYMBOL))
	{
		
		if(is_form(input, &forme))
		{
			WARNING_MSG("Invalid format for %s", input->this.symbol);
			/* A voir avec le temps si on ne peut pas
			 * avoir des symboles seul hors d'une liste
			 */
			return(NULL);
		}
		else
		{
			var = ENV_get_var(input, &flag);
			if(OBJECT_isempty(var))
			{
				WARNING_MSG("Error or variable doesn't exist");
				return(NULL);
			}
			object saved_name = var->this.pair.car;
			var = var->this.pair.cdr ;
			DEBUG_MSG("Var->type %d", var->type);
			if(flag)
			{
				/* On gère le cas du 
				 * Symbole en valant un autre
				 */
				if(check_type(var, SFS_SYMBOL))
				{
					DEBUG_MSG("Symbol detected");
					return(sfs_eval(var));
				}
				else if(check_type(var, SFS_PRIM))
				{
					/* Cas ou on a une primitive seule 
					 * Ne devrait pas se produire,
					 * les primitives sont évaluées seulement
					 * dans des paires. Une primitive hors d'une paire
					 * ne devrait pas se produire.
					 */
					WARNING_MSG("Wrong format for primitive type : %s ",
							saved_name->this.symbol);
					return(NULL);
				}
				else
				{
					DEBUG_MSG("%s find ", input->this.symbol);
					return(var);
				}
			}
			else
			{
				WARNING_MSG("%s not defined", 
						input->this.symbol);
				return(NULL);
			}
		}
	}

	/* Cas d'une paire */

	/* On lit le symbole dans le car mais avant
	 * on teste la validité du type (si c'est pas un
	 * symbole on évalue pas.
	 * Et on rend la main.
	 * On affiche alors un blanc.
	 */
	if(check_type(input, SFS_PAIR))
		
	{
		object symb = lecture->this.pair.car ;
		
		if(symb == NULL)
		{
			WARNING_MSG("NULL ptr detected in eval for car\n");
			/* Au cas ou. Ne devrait pas se produire.
			 * Voir si on quitte l'interpréteur ou
			 * si on continue.
			 */
			return(NULL);
		}	
		DEBUG_MSG("Start compare eval");
		if(symb->type != SFS_SYMBOL)
		{
			WARNING_MSG("Pair should begin with symbol");
			/* Construire plus proprement le message */
			return(input);
		}

		/* On a une liste avec un symbole au début on peut traiter */

		/* On regarde si c'est une forme ou une variable */
				if(is_form(symb, &forme) )
		{
			/* Cas ou c'est une forme du langage */
			DEBUG_MSG("Scheme form identified");
			switch(forme)
			{
				case QUOTE : 
					return(EVAL_quote(input));
					break;
				case DEFINE : 
					return(EVAL_define(input));
					break;
				case SET : 
					return(EVAL_set(input));
					break;
				case AND :
					return(EVAL_and(input));
					break;
				case IF :
					return(EVAL_if(input));
					break;
				case OR :
					return(EVAL_or(input));
					break;
				case NONE :
					WARNING_MSG("Unrecognized form");
					return(NULL);
					break;
				/* Pas besoin de default, on serait pas
				 * dans le switch sinon
				 */
			}
		}
		else /* Cas ou c'est une variable définie par l'utilisateur */
		{
			DEBUG_MSG("Star looking for %s", symb->this.symbol);
			var = ENV_get_var(symb, &flag)->this.pair.cdr;
			DEBUG_MSG("Finded ? %d", flag);
			
			if(flag) /* On a trouvé quelque chose */
			{
				
				if(check_type(var, SFS_PRIM)) 
				{
					/* Si le car est une primitive on l'applique à l'évaluation
					 * de tous les object */
					DEBUG_MSG("Primitive detected");
					object argument = PRIM_eval(OBJECT_get_cxr(input, "cdr"));
					/* Pas d'erreur ici on a normalement une paire avec un car 
					 * et cdr */
					return((*var->this.prim.function)(argument) );
				}
				return(var);
			}
			else
			{
				WARNING_MSG("%s is not a scheme key word", symb->this.symbol);
				return NULL;
			}
		}
	}
	

	/* Si on arrive ici c'est qu'on a évalué un atome
	 * il est donc déjà créé par sfs_read on ne 
	 * fait que retourne son pointeur.
	 */
    return sfs_eval(input);
}

/*Evaluation*/
/** @fn object EVAL_set(object o)
 * @brief Mets à jour une variable.
 *
 * set! est de la forme :
 * (set! x <valeur>)
 * x le symbole à mettre à jour et
 * <valeur> sa nouvelle valeur.
 * Renvoie x. 
 * Si x n'existe affiche un warning et retourne NULL.
 * Si x n'est pas un symbole affiche un warning et 
 * retourne null.
 * Si <valeur> est NULL affiche un warning et retourne 
 * NULL
 *
 * @return Renvoie le nom de la variable modifiée.
 */
object EVAL_set(object o)
{
	object utile = OBJECT_get_cxr(o, "cdr");
	object name = OBJECT_get_cxr(utile, "car");
	/* Vérif existence */
	if(OBJECT_isempty(name))
	{
		WARNING_MSG("set! : Missing <variable> symbol");
		return(NULL);
	}
	/* Vérif name bien un symbole */
	if(!check_type(name, SFS_SYMBOL))
	{
		WARNING_MSG("set! : <variable> is not a symbol");
		return(NULL);
	}
	object evaluated = OBJECT_get_cxr(utile, "cadr");
	/* Vérif qu'il n'y ait pas trop d'argument */
	if(!check_type(OBJECT_get_cxr(utile, "cddr"), SFS_NIL))
	{
		WARNING_MSG("set! : too many args");
		return(NULL);
	}
	
	evaluated = sfs_eval(evaluated);
	if(OBJECT_isempty(evaluated) || check_type(evaluated, SFS_UNKNOWN))
	{
		WARNING_MSG("set! : Missing or wrong <value>.");
		return(NULL);
	}
	object check =ENV_update_var(name, evaluated, ENV_DISABLE_CREATION, NULL);
	if(OBJECT_isempty(check))
	{
		WARNING_MSG("set! : <variable> not defined");
		return(check);
	}
	return(check);
}

/** @fn object EVAL_define(object o)
 * @brief Evalue un define.
 *
 * Evalue un define.
 * évalue également de cddr.
 * Effectue également la création dans l'environnement
 * courant de la variable.
 * Retourne le nom de la variable créée c'est à dire
 * le symbole.
 * Si le format de la chaine n'est pas bon elle retourne NULL
 * et affiche un warning.
 *
 * @return Renvoie le symbole crée.
 */
object EVAL_define(object o)
{
	DEBUG_MSG("Call EVAL_define");
	object name = OBJECT_get_cxr(o, "cadr");
	/* Faut copier le nom pour pas la perdre à la 
	 * libération de ce qu'on a lu
	 */
	object name_cpy = OBJECT_build_cpy(name);
	/* Vérification du format*/
	if(OBJECT_isempty(name))
	{
		WARNING_MSG("define : missing <variable> symbol");
		return(NULL);
	}
	/* On vérifie le format de name */
	if(!check_type(name, SFS_SYMBOL))
	{
		WARNING_MSG("define : <variable> is not a symbol");
		return(NULL);
	}
	/* On vérifie qu'on utilise pas une forme du langage */
	if(is_form(name, NULL))
	{
		WARNING_MSG("define : %s is a reserved scheme word", name->this.symbol);
		return(NULL);
	}
	DEBUG_MSG("Name : %s", name->this.symbol);
	object evaluated = OBJECT_get_cxr(o, "caddr");
	if(!check_type(OBJECT_get_cxr(o, "cdddr"), SFS_NIL))
	{
		WARNING_MSG("define : wrong number of args");
		return(NULL);
	}
	evaluated = sfs_eval(evaluated);
	DEBUG_MSG("Type evaluated %d", evaluated->type);
	/* Vérification format de la variable. */
	if(OBJECT_isempty(evaluated) || check_type(evaluated, SFS_UNKNOWN))
	{
		WARNING_MSG("define : missing or wrong <value>");
		return(NULL);
	}
	DEBUG_MSG("Type evaluated %d\n", evaluated->type);

	int free_flag = FALSE;
	ENV_update_var(name_cpy, evaluated, ENV_ENABLE_CREATION, &free_flag);
	if(free_flag) INFO_MSG("Destruction effectuée");
	return(name);
}

/** @fn  object EVAL_quote(object o);
 * @brief Evalue l'expression quote
 *
 * Concrètement retourne le cdr de o.
 *
 * @return Renvoie le cdr de o.
 */
object EVAL_quote(object o)
{ 
	DEBUG_MSG("Call eval_quote");
	return(o->this.pair.cdr->this.pair.car);
}


/** @fn object EVAL_and(object o)
 * @brief Retourne un et logique de o.
 *
 * On évalue chaque terme si on croise
 * un terme faux on s'arrête et on retourne
 * faux.
 * Les termes sont évalués.
 * Si le terme évalué n'est pas un booléen
 * on le renvoie et on quitte la fonction.
 * Si and est vide renvoie vrai par défaut.
 * En cas de succession de termes non booléen,
 * seul le dernier est affiché.
 * Un terme non booléen est considéré comme vrai,
 * s'il est le dernier dans le and, il est retourné.
 * (fonctionnement de scheme48)
 *
 * Renvoie NULL et affiche un warning
 * en cas de problème.
 *
 * @return Renvoie le résultat. Renvoie un booléen !
 */
object EVAL_and(object o)
{
	/* Vérifications */
	if(!check(o, "and : NULL ptr given")) return NULL;
	if(!check_type(o, SFS_PAIR))
	{
		WARNING_MSG("and : need SFS_PAIR ");
		return(NULL);
	}
	/* On va parcourir le and et évaluer chaque
	 * expressions.
	 * Cela revient à évaluer le car d'un cdr à chaque 
	 * fois.
	 * On a un cdr, on évalue le car.
	 * Si le car est faux on s'arrête,
	 * sinon on prend le cddr et rebelotte.
	 * On s'arrête si on arrive à nil.
	 */
	object default_result =vrai;
	/* Résultat que l'on retourne si tous les éléments sont 
	 * vrai
	 */
	default_result->this.special= vrai ;

	object premier_terme = o->this.pair.cdr;
	if(!check(o,"and : NULL ptr given in cdr")) return NULL;
	object i = premier_terme;
	object tmp = i->this.pair.car;
	if(tmp == nil) return default_result;
	for(i = premier_terme; i!=nil && 
			check(i, "and : NULL ptr while evaluating");
		i= i->this.pair.cdr)
	{
		if(!check_type(i, SFS_PAIR)) return i;
		tmp = sfs_eval(i->this.pair.car);	
		/* S'il est faux on s'arrête */
		if( tmp== faux) return(tmp) ;
		/*et on renvoie faux */
		/* Sinon on fait rien on continue */
	}
	/* Si on est là c'est qu'on a atteint nil */
	if(!check_type(tmp, SFS_BOOLEAN)) return(tmp);
	/* On a donc tout vrai */
	return(default_result);

}


/** @fn  object EVAL_or(object o)
 * @brief Evaluation de la forme or.
 *
 * On évalue chaque terme du or 
 * tant qu'on a pas de terme vrai.
 * Comme pour and si un terme
 * n'est pas un booléen il est considéré 
 * comme vrai et on le retourne 
 * Si c'est le dernier, on le retourne
 * (scheme48)
 * or seul vaut faux par défaut.
 *
 * @return Renvoie un booléen qui correspond à un ou logique.
 */
object EVAL_or(object o)
{
	/* Vérifications */
	if(!check(o, "if : NULL ptr given")) return NULL;
	if(!check_type(o, SFS_PAIR))
	{
		WARNING_MSG("or : need SFS_PAIR ");
		return(NULL);
	}
	/* On va parcourir le or et évaluer chaque
	 * expressions.
	 * Cela revient à évaluer le car d'un cdr à chaque 
	 * fois.
	 * On a un cdr, on évalue le car.
	 * Si le car est vrai on s'arrête,
	 * sinon on prend le cddr et rebelotte.
	 * On s'arrête si on arrive à nil.
	 */
	object default_result = make_object(SFS_BOOLEAN);
	/* Résultat que l'on retourne si tous les éléments sont 
	 * faux
	 */
	default_result==faux;

	object premier_terme = o->this.pair.cdr;
	if(!check(o,"or : NULL ptr given in cdr")) return NULL;
	object i = premier_terme;
	object tmp;
	for(i = premier_terme; i!=nil && 
			check(i, "or : NULL ptr while evaluating");
		i= i->this.pair.cdr)
	{
		
		tmp = sfs_eval(i->this.pair.car);
		if(check_type(tmp, SFS_BOOLEAN))
		{
			if(tmp== faux) continue;
			/* Si c'est faux on continue */
		}
		return(tmp);
		/* S'il est vrai on s'arrête */
	}
	/* Si on est là c'est qu'on a atteint nil */
	/* On a donc tout faux */
	return(default_result);
}

/** @fn  object EVAL_if(object o)
 * @brief Evalue un if.
 *
 * Si le prédicat est vrai renvoie la conséquence.@n
 * S'il n'y a pas de prédicat retourne NULL et affiche
 * des warnings.
 * Sinon renvoie l'alternative.
 * S'il n'y a pas d'alternative retourne
 * #f.
 * S'il n'y a pas de conséquence retourne
 * retourne NULL et affiche une erreur.
 * S'il y a trop d'arguments renvoie null et affiche un 
 * warning.
 *
 * @sa IF_consequence
 * @sa IF_alternative
 * @sa IF_predicat
 * @sa IF_overargument
 *
 * @return Renvoie l'évaluation d'un if.
 */
object EVAL_if(object o)
{
	object predicat = sfs_eval(IF_predicat(o));
	DEBUG_MSG("Predicat %p", predicat);
	if(OBJECT_isempty(predicat) ||  o ==  nil)
	{
		WARNING_MSG("if : need predicate");
		return NULL;
	}
	object consequence = sfs_eval(IF_consequence(o));
	object alternative = sfs_eval(IF_alternative(o));
	if(OBJECT_isempty(consequence) || consequence==nil)
	{
		WARNING_MSG("if : need consequence");
		return NULL;
	}
	/* On vérifie qu'il n'y ait pas trop d'arguments */
	object overarg = IF_overargument(o);
	if(overarg != nil && !OBJECT_isempty(overarg) ) 
	{
		WARNING_MSG("if : Too many arguments");
		return(NULL);
	}
	/* Comme on a mal implémenté faux et vrai */
	if(predicat== faux) 
	{
		DEBUG_MSG("Alternative");
		if(alternative == nil) return(faux);
		return(alternative);
	}
	else
	{
		return(consequence);
		DEBUG_MSG("Conséquence");
	}

}
/* Outils d'évaluation */

/** @fn  object IF_predicat(object input)
 * @brief Retourne le prédicat d'une structure if.
 *
 * Revient à prendre le cadr.
 * @sa OBJECT_get_cxr
 *
 * @return Renvoie l'alternative (le cadr).
 */
object IF_predicat(object input)
{
	return(OBJECT_get_cxr(input, "cadr"));
}


/** @fn  object IF_consequence(object input)
 * @brief Renvoie la conséquence d'une structure if.
 *
 * Renvoie le caddr.
 * @sa OBJECT_get_cxr
 *
 * @return Renvoie la conséquence (le caddr).
 */
object IF_consequence(object input)
{
	return(OBJECT_get_cxr(input, "caddr"));
}


/** @fn  object IF_alternative(object input)
 * @brief Retourne l'alternative d'une structure if.
 *
 * Renvoie le cadddr.
 * @sa OBJECT_get_cxr(input, "cadddr")
 *
 * @return Renvoie l'alternative (le cadddr)
 */
object IF_alternative(object input)
{
	return(OBJECT_get_cxr(input, "cadddr"));
	}
/** @fn  object IF_overargument(object input)
 * @brief Vérifie que if n'ait pas un argument en trop
 *
 * Renvoie nil si if contient le bon nombre d'argument
 * ce nil provient de la construction normale
 * du if que l'on obtient en allant chercher le
 * cddddr (ce qui suit le prédicat
 *
 * @return Renvoie nil si if est bon, autre chose sinon.
 */
object IF_overargument(object input)
{
	return(OBJECT_get_cxr(input, "cddddr"));
}
