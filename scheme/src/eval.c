/**
 * @file eval.c
 * @author François Cayre <cayre@yiking.(null)>
 * @date Fri Jun 22 20:11:30 2012
 * @brief Outils d'évaluation pour interpréteurs et d'autres utilitaires.
 *
 * Evaluation stuff for SFS.
 */

#include "eval.h"


/** @fn object sfs_eval_compound(object pair);
 * @brief Evalue un agrégat.
 * @pair : L'entrée de l'utilisateur dont le symbole est évalué.
 * @env : L'environnement dans lequel on est.
 *
 * pair correspond à l'entrée de l'utilisateur à la seule différence
 * que le premier élément de l'instruction n'est pas un symbole mais
 * un agregat : Le symbole a été interprété.
 * Procède par étape :
 * Construit un environnement dont le père est env.
 * Créer dans cet environnement les variables associées à
 * l'agregat (paramètres).
 * Exécute le body par sfs_eval dans cet environnement.
 * Ne supprime pas l'environnement.
 *
 * Renvoie l'évaluation du body.
 *
 * Si trop d'arguments sont données affiche un warning,
 * et retourne NULL.
 * Si il manque des arguments, renvoie NULL et affiche un
 * warning.
 * @return Renvoie l'évaluation du body sur les arguments.
 */
object sfs_eval_compound(object pair)
{

	/* On étend l'environnement */
	object agregat = OBJECT_get_cxr(pair, "car");
	object env = ENV_build(agregat->this.compound.envt);
	/* Création des paramètres dans l'environnement 
	 * On parcours récursivement la liste des noms de 
	 * paramètres et on associe à chaque nom sa valeur dans
	 * l'environnement créé */
	
	object i = NULL, j = NULL;
	int compteur_warning = 0;
	/* i parcours valeurs des arguments, j leurs noms */
	object arguments = OBJECT_get_cxr(pair, "cdr");
	object noms = agregat->this.compound.param ;
	for(i = arguments, j= noms, compteur_warning =0 ;
		       	(i!= nil && !OBJECT_isempty(i)) &&
			(j!=nil && !OBJECT_isempty(j)) ;
			i =i->this.pair.cdr, j=j->this.pair.cdr,
			compteur_warning ++)
	{
		object nom = j->this.pair.car ;
		object valeur = i->this.pair.car ;
		if(!ENV_add_var(nom,valeur, env))
		{
			WARNING_MSG("Error while extending environment in lambda");
			return(NULL);
		}
		
	}
	/* Test si jamais trop d'arguments */
	if((j == nil || OBJECT_isempty(j))
			&& (i != nil && !OBJECT_isempty(i)))
	{
		DEBUG_MSG("j : %p, i : %p", j, i);
		WARNING_MSG("Too many arguments %d needed", compteur_warning);
		return(NULL);
	}
	if((i == nil || OBJECT_isempty(i))
			&& (j != nil && !OBJECT_isempty(j)))
	{
		WARNING_MSG("Too few arguments %d given", compteur_warning);
		return(NULL);
	}


	/* Evaluation du body dans l'environnement 
	 * Evaluation récursive
	 * Petit if au cas ou, mais ne devrait pas
	 * se produire.
	 */
	if(!check_type(agregat->this.compound.body, SFS_PAIR))
		return( sfs_eval(agregat->this.compound.body, env));

	object tmp = NULL ;
	for(i = agregat->this.compound.body ; i!= nil && !OBJECT_isempty(i);
			i=i->this.pair.cdr)
	{
		tmp = sfs_eval(i->this.pair.car, env);
	}
	return(tmp);
}

/** @fn object sfs_eval( object input, object env) 
 * @brief Fonction d'évaluation d'expression scheme.
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
object sfs_eval(object input, object env) 
{
	DEBUG_MSG("Call sfs_eval");
	/* La forme que l'on va identifier */
	FORMS forme = NONE ;
	/* la variable que l'on va chercher */
	object var = NULL;
	/* Vérif si on trouve bien var */
	int flag = FALSE;
	/* Vérif qu'on ait pas un input NULL */
	if(OBJECT_isempty(input))
	{ return NULL ; }
	/* Cas d'un type inconnu */
	if(check_type(input, SFS_UNKNOWN)) return(NULL);


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
			var = ENV_get_var(input, &flag, env);
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
				
			        if(check_type(var, SFS_PRIM))
				{
					/* Cas d'une primitive seule :
					 * On la renvoie sans l'interpréter
					 */	
					/*WARNING_MSG("Wrong format for primitive type : %s ",
							saved_name->this.symbol);*/
					return(var);
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
			/* Si on a une paire on évalue le car, et on recommence
			 * l'évaluation
			 */
			if(check_type(symb, SFS_PAIR))
			{
				DEBUG_MSG("Pair instead of symb, evaluating");
				OBJECT_set_car(input, sfs_eval(symb, env));
				return(sfs_eval(input, env));
			}
			else if(check_type(symb, SFS_COMP))
			{
				/* si on a un compound on l'évalue sur ses
				 * arguments
				 */
				/* Evaluation des arguments du compound
				 * On recycle PRIM_eval pour l'occasion
				 */
				OBJECT_set_cdr(input, PRIM_eval(OBJECT_get_cxr(input, "cdr"), env));
				return(sfs_eval_compound(input));
			}
			else 
			{
				WARNING_MSG("Pair should begin with symbol");
				/* Construire plus proprement le message */
				return(NULL);
			}
		}

		/* On a une liste avec un symbole au début on peut traiter */

		/* On regarde si c'est une forme ou une variable */
				if(is_form(symb, &forme) )
		{
			/* Cas ou c'est une forme du langage */
			DEBUG_MSG("Scheme form identified : %d", forme);
			switch(forme)
			{
				case QUOTE : 
					return(EVAL_quote(input, env));
					break;
				case DEFINE : 
					return(EVAL_define(input, env));
					break;
				case SET : 
					return(EVAL_set(input, env));
					break;
				case AND :
					return(EVAL_and(input, env));
					break;
				case IF :
					return(EVAL_if(input, env));
					break;
				case OR :
					return(EVAL_or(input, env));
					break;
				case BEGIN :
					return(EVAL_begin(input,env));
					break;
				case LAMBDA :
					DEBUG_MSG("Call EVAL_lambda");
					return(EVAL_lambda(input, env));
					break;
				case LET :
					DEBUG_MSG("Call EVAL_let");
					return(EVAL_let(input, env));
					break;
				default :
					WARNING_MSG("Unrecognized form");
					return(NULL);
					break;
					/* Pas besoin de default, on serait pas
					 * dans le switch sinon
					 */
			}
		}
		else /* Cas ou c'est une variable définie par l'utilisateur
		      * Ou une primitive
		      */
		{
			DEBUG_MSG("Star looking for %s", symb->this.symbol);
			var = ENV_get_var(symb, &flag, env)->this.pair.cdr;
			DEBUG_MSG("Finded ? %d", flag);
			
			if(flag) /* On a trouvé quelque chose */
			{
				
				if(check_type(var, SFS_PRIM)) 
				{
					/* Si le car est une primitive on l'applique à l'évaluation
					 * de tous les object */
					DEBUG_MSG("Primitive detected");
					object argument = PRIM_eval(OBJECT_get_cxr(input, "cdr"), env);
					/* Pas d'erreur ici on a normalement une paire avec un car 
					 * et cdr */
					DEBUG_MSG("Type argument in primitive : %d", argument->type);
					return((*var->this.prim.function)(argument) );
				}
				else if(check_type(var, SFS_COMP))
				{
					/* Cas agregat */
					OBJECT_set_car(input, var);
					/* On a évalué le symbole en un compound*/
					/* On évalue les arguments */
					/*OBJECT_set_cdr(input, PRIM_eval(OBJECT_get_cxr(input, "cdr"), env));*/
					return(sfs_eval(input, env));
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
    return sfs_eval(input, env);
}

/*Evaluation*/
/** @fn object EVAL_set(object o, object env)
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
object EVAL_set(object o, object env)
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
	
	evaluated = sfs_eval(evaluated, env);
	if(OBJECT_isempty(evaluated) || check_type(evaluated, SFS_UNKNOWN))
	{
		WARNING_MSG("set! : Missing or wrong <value>.");
		return(NULL);
	}
	object check =ENV_update_var(name, evaluated, env);
	if(OBJECT_isempty(check))
	{
		WARNING_MSG("set! : <variable> not defined");
		return(check);
	}
	return(make_object(SFS_UNKNOWN));
}

/** @fn object EVAL_define(object o, object env)
 * @brief Evalue un define.
 *
 * Prend en paramètre une instruction complète d'une
 * entrée define.
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
object EVAL_define(object o, object env)
{
	DEBUG_MSG("Call EVAL_define");
	object name = OBJECT_get_cxr(o, "cadr");
	/* Faut copier le nom pour pas la perdre à la 
	 * libération de ce qu'on a lu
	 */
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
	evaluated = sfs_eval(evaluated, env);
	/* petite ternaire pour gérer le cas NULL*/
	DEBUG_MSG("Type evaluated %d", (evaluated ? evaluated->type : 7));
	/* Vérification format de la variable. */
	if(OBJECT_isempty(evaluated) || check_type(evaluated, SFS_UNKNOWN))
	{
		WARNING_MSG("define : missing or wrong <value>");
		return(NULL);
	}
	DEBUG_MSG("Type evaluated %d\n", evaluated->type);

	ENV_add_var(name, evaluated, env);
	return(make_object(SFS_UNKNOWN));
}

/** @fn  object EVAL_quote(object o, object env);
 * @brief Evalue l'expression quote
 *
 * Concrètement retourne le cdr de o.
 *
 * @return Renvoie le cdr de o.
 */
object EVAL_quote(object o, object env)
{ 
	DEBUG_MSG("Call eval_quote");
	return(o->this.pair.cdr->this.pair.car);
}


/** @fn object EVAL_and(object o, object env)
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
object EVAL_and(object o, object env)
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
		tmp = sfs_eval(i->this.pair.car, env);	
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


/** @fn  object EVAL_or(object o, object env)
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
object EVAL_or(object o, object env)
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
	
	object premier_terme = o->this.pair.cdr;
	if(!check(o,"or : NULL ptr given in cdr")) return NULL;
	object i = premier_terme;
	object tmp;
	for(i = premier_terme; i!=nil && 
			check(i, "or : NULL ptr while evaluating");
		i= i->this.pair.cdr)
	{
		
		tmp = sfs_eval(i->this.pair.car, env);
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
	return(faux);
}

/** @fn  object EVAL_if(object o, object env)
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
object EVAL_if(object o, object env)
{
	object predicat = sfs_eval(IF_predicat(o), env);
	DEBUG_MSG("Predicat %p", predicat);
	if(OBJECT_isempty(predicat) ||  o ==  nil)
	{
		WARNING_MSG("if : need predicate");
		return NULL;
	}
	object consequence = IF_consequence(o);
	object alternative = IF_alternative(o);
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
		return(sfs_eval(alternative,env));
	}
	else
	{
		return(sfs_eval(consequence, env));
		DEBUG_MSG("Conséquence");
	}

}

/** @fn object EVAL_begin(object o, object env)
 * @brief Evalue la forme begin.
 *
 * Evalue la forme begin :
 * Chaque instruction de la forme
 * est évaluée, l'évaluation de la dernière
 * instruction est retournée.
 * Prend en paramètre l'intégralité d'une instruction
 * begin 
 * ex : (begin (define a 4) (+ a 34) )
 *
 * @return Renvoie l'évaluation de la dernière
 * instruction.
 */
object EVAL_begin(object o, object env)
{
	object premier_arg = OBJECT_get_cxr(o, "cdr");
	/* La forme begin se renvoie elle même si
	 * elle n'a pas d'arguments
	 */
        if(check_type(premier_arg, SFS_NIL)) return(o);
	/* On évalue 1 par 1 les arguments */
	object tmp = NULL, i = NULL;
	for(i = premier_arg ; !check_type(i, SFS_NIL) ; 
			i= i->this.pair.cdr)
	{
		tmp = sfs_eval(i->this.pair.car, env);
	}

	return(tmp);
}

/** @fn object EVAL_lambda(object o, object env)
 * @brief Evalue lambda et créer l'objet de type
 * compound correspondant
 *
 * Prend en entrée la ligne entrée par l'utilisateur.
 *
 * @return Renvoie un agrégat.
 */
object EVAL_lambda(object o, object env)
{

	/* On vérifie que la structure du lambda est 
	 * la bonne */
	if(!LAMBDA_check_number_arg(o))
	{
		WARNING_MSG("Wrong number of args for lambda form");
		return(NULL);
	}

	object retour = make_object(SFS_COMP);
	retour->this.compound = COMP_build(LAMBDA_get_var(o),LAMBDA_get_body(o), env);

	
	return(retour);	
}


/** @fn object EVAL_let(object o, object env)
 * @brief Evalue la forme let.
 *
 * Renvoie NULL si erreur.
 * Appel de LET_check.
 * Création d'un environnement dont env est le père.
 * Appel de LET_eval si celui-ci.
 *
 * @sa LET_eval
 * @sa LET_check
 *
 * @return Renvoie evaluation de let.
 */
object EVAL_let(object o, object env)
{
	if(!LET_check(o)) return NULL;
	object environment = ENV_build(env);
	return(LET_eval(o, environment));
}
/* Outils d'évaluation */


/** @fn int LAMBDA_check_number_arg(object input)
 * @brief Vérifie que lambda ait bien le bon nombre 
 * d'arguments.
 *
 * Au minimum lambda possède aucun arguments
 * et un body qui est réduit à un atome.
 * Prend la ligne entrée par l'utilisateur (lambda ...)
 *
 * @return Renvoie 1 si le nombre d'arguments est
 * correct 0 sinon.
 */
int LAMBDA_check_number_arg(object input)
{
	object val = OBJECT_get_cxr(input, "cdr");
	if( check_type(val->this.pair.car, SFS_PAIR)
			&& (check_type(val->this.pair.cdr, SFS_NIL)))
		return(0);
	return(1);
}

/** @fn object LAMBDA_get_var(object input)
 * @brief Renvoie le nom des variables d'une forme lambda.
 *
 * Prend en paramètre la liste qui doit être évaluée
 * à lambda 
 * ex : (lambda (x y) (corps))
 *
 * @return Renvoie une liste de noms de variable.
 */
object LAMBDA_get_var(object input)
{
	return(OBJECT_get_cxr(input, "cadr"));
}

/** @fn object LAMBDA_get_body(object input)
 * @brief Renvoie les instructions d'un lambda.
 *
 * Prend en argument l'instruction lambda complète.
 *
 * @return renvoie la liste des instructions du body
 */
object LAMBDA_get_body(object input)
{
	return(OBJECT_get_cxr(input, "cddr"));
}

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


/** @fn object LET_eval(object input, object env)
 * @brief Evalue le corps d'un let dans l'environnement env.
 * @param input : input total d'un let (let ...)
 * @param env : Environnement dans lequel seront faite les évaluations.
 *
 * Evalue les arguments à l'aide de sfs_eval, en construisant
 * une instruction (define args value). La gestion des erreurs
 * est donc faite par sfs_eval et EVAL_define.
 *  Evalue le body avec sfs_eval dans l'env en ayant construit
 *  une instruction avec begin : (begin body ..)
 * @n
 *  ATTENTION
 * @n
 * Ne fait pas de test de la forme de input.
 *
 *  @sa EVAL_define
 *  @sa sfs_eval
 *
 *
 * @return Renvoie l'évaluation de d'un let dans env.
 */
object LET_eval(object input, object env)
{
	object args = OBJECT_get_cxr(input, "cadr");
	object i = args ;
	
	object i_car= i->this.pair.car, tmp_pair = NULL;
	if(!check_type(i, SFS_NIL))
	{
		for(i = args; i!=nil &&
				!OBJECT_isempty(i) ;
				i = i->this.pair.cdr)
		{
			i_car = i->this.pair.car ;
			DEBUG_MSG("LET_eval : i : %p, i_car : %p, nil : %p", i, i_car, nil);
			tmp_pair = OBJECT_build_pair(OBJECT_build_symbol("define"),
					i_car);

			if(!sfs_eval(tmp_pair, env))
			{
				WARNING_MSG("Error evaluating arguments");
				return(NULL);
			}
		}
	}

	object body = OBJECT_get_cxr(input, "cddr");
	tmp_pair = OBJECT_build_pair(OBJECT_build_symbol("begin"), body);
	return(sfs_eval(tmp_pair, env));


}


/** @fn int LET_check(object input)
 * @brief Vérifie que input possède bien la structure minimale de let.
 *
 * Vérifie que let ait le bon format :
 * Format minimal : (let () atome) qui renvoie atome.
 * Ne vérifie pas que les arguments ont le bon format c'est
 * LET_eval qui s'en charge.
 * Si erreur affiche un WARNING_MSG et renvoie 0.
 *
 * @return Renvoie 1 si format correct, 0 sinon.
 */
int LET_check(object input)
{
	object args= OBJECT_get_cxr(input, "cadr");
	object body = OBJECT_get_cxr(input, "cddr");
	if(!args || !check_type(body, SFS_PAIR))
	{
		WARNING_MSG("Let : Parsing error");
		DEBUG_MSG(" args : %p, body : %p", args, body);
		return(0);
	}
	if(!check_type(args, SFS_NIL) && !check_type(args, SFS_PAIR))
	{
		WARNING_MSG("(let xxx ...) Wrong format for xxx");
		return(0);
	}
	/* Si les args sont une paire il faut qu'ils soient bien
	 * une liste de liste ce qu'on va vérifier
	 */
	if(check_type(args, SFS_PAIR) && !check_type(args->this.pair.car, SFS_PAIR)) 
	{
		WARNING_MSG("(let xxx ... ) Wrong format for xxx");
		return(0);
	}
	return(1) ;
}
