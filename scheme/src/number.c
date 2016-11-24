#include"number.h"

/** @file number.c
 * @author Rimbaud Sébastien sebastien.rimbaud@phelma.grenoble-inp.fr
 * @brief ensemble de fonctions relatives au type num.
 */

/* Affichage */

/**@fn void NUM_print( num printed)
 * @brief Affiche un num.
 * @param printed : le num à afficher.
 */
void NUM_print( num printed)
{
	switch (printed.numtype)
	{
		case NUM_INTEGER :
			printf("%ld", printed.this.integer);
			break;
		case NUM_REAL :
			printf("%lf", printed.this.real);
			break;
		case NUM_PINFTY :
			printf("+inf");
			break;
		case NUM_MINFTY :
			printf("-inf");
			break;
		default :
			printf("Unrecognized num type or not implemented");
	}
}

/* Constructeur ultime */
/**@fn num NUM_build(num* a, void* v_ptr, int mode)
 * @brief Enregistre dans a la valeur v, retourne le nouveau num. 
 * @param num* : num ou on doit enregistrer la valeur.
 * @param v : Pointeur sur la valeur qu'on doit enregistrer. 
 * @param mode : Mode NUM_PINFTY NUM_MINFTY ou NUM_INTEGER ou NUM_REAL.
 *
 * En pratique v est casté selon le mode précisé. La fonction ne garanti
 * pas de bon fonctionnement si v n'est pas castable dans le mode voulu.11
 * Si v_ptr est NULL la valeur par défaut sera 0.
 * Pour les entiers la fonction gère l'overflow et construit un 
 * +inf ou -inf selon le débordement. Pour que cela fonctionne
 * *v_ptr doit prendre les valeurs LONG_MAX ou LONG_MIN.
 *
 * Si le mode n'est pas une constante implémentée dans la fonction :
 *
 * - NUM_REAL    	 
 * - NUM_INTEGER  
 * - NUM_UINTEGER	(Not implemented) 
 * - NUM_COMPLEX  	(Not implemented)
 * - NUM_UNDEF    
 * - NUM_PINFTY   
 * - NUM_MINFTY   
 * Il est mis par défaut à NUM_UNDEF
 * @return : Renvoie un pointeur sur le num modifié.
 */
num* NUM_build(num* a, void* v_ptr, int mode)
{
	/* Entier */
	long int e = 0;
	if(v_ptr) e = *( (long int*) v_ptr);
	/*Double */
	double d = 0;
	if(v_ptr) d = *( (double*) v_ptr);

	switch(mode)
	{
		case NUM_INTEGER :
			a->numtype= NUM_INTEGER;
			/* Gestion de l'overflow*/
			if(e == LONG_MAX) /* +inf */
			{
				a->numtype = NUM_PINFTY;
			}
			else if(e == LONG_MIN)
			{
				a->numtype = NUM_MINFTY;
			}
			else a->this.integer = e;
			break;
		case NUM_REAL :
			a->numtype = NUM_REAL;
			a->this.real = d;
		case NUM_PINFTY :
			a->numtype= mode ;
			break;
		case NUM_MINFTY :
			a->numtype=mode;
			break;
		default :
			a->numtype=NUM_UNDEF;
	}
	return(a);
}


/* Mise à jour */
/**@fn num NUM_set_int(num* a, int v)
 * @brief Enregistre dans a l'entier v, retourne le nouveau num. Met à jour champ numtype à NUM_INTEGER.
 * @param num* : num ou on doit enregistrer la valeur.
 * @param v : entier à enregistrer.
 *
 * @return : Renvoie un pointeur sur le num modifié.
 */
num* NUM_set_int(num* a, int v)
{
	a->numtype=NUM_INTEGER;
	a->this.integer = v ;
	return(a);
}

/**@fn num NUM_set_real(num* a, double v)
 * @brief Enregistre dans a le réel v, retourne le nouveau num. Met à jour champ numtype à NUM_REAL.
 * @param num* : num ou on doit enregistrer la valeur.
 * @param v : réel à enregistrer.
 *
 * @return : Renvoie un pointeur sur le num modifié.
 */
num* NUM_set_real(num* a, double v)
{
	a->numtype= NUM_REAL;
	a->this.real = v;
	return(a);
}

/* Comparaison */


/** @fn int NUM_cmp_type(num a, num b)
 * @brief Compare les types de 2 structures num.
 * Renvoie 1 si égale
 * 0 sinon.
 *
 * @return Renvoie 1 si égale 0 sinon.
 */
int NUM_cmp_type(num a, num b)
{
	if(a.numtype == b.numtype) return(1);
	return(0);
}


/** @fn int NUM_lint_cmp(num a, long int v)
 * @brief Compare le num avec l'int v. Num doit être de type NUM_INTEGER.
 * @param a : Le num à comparer de type int.
 * @param v : L'entier que l'on compare.
 *
 * @return : Revoie 1 si num = v et 0 sinon (renvoie 0 si num n'est pas de type entier)
 */
int NUM_lint_cmp(num a, long int v)
{
	if(a.numtype!=NUM_INTEGER) return(0); /* Proposer un traitement de ce cas là plus tard. */
	if(a.this.integer == v) return(1);
	return(0);
}

/** @fn int NUM_cmp_lint(num a, num b)
 * @brief Compare deux num de type NUM_INTEGER.
 *
 * Renvoie 0 si ils n'ont le même type.
 * Est appelé par NUM_cmp.
 * Renvoie 0 si a ou b pas de type NUM_INTEGER
 * @return Renvoie 1 si égaux 0 sinon.
 */
int NUM_cmp_lint(num a, num b)
{
	if(a.numtype != NUM_INTEGER) return(0);
	if(!NUM_cmp_type(a, b)) return(0);
	/* Ici a et b nécessaire NUM_INTEGER */
	if(a.this.integer == b.this.integer) return(1);
	return(0);
}

/** @fn int NUM_cmp(num a, num b)
 * @brief Compare deux num. Renvoie 1 si égaux, 0 sinon.
 *
 * Compare deux num. Si leurs types sont différents renvoie 0.
 * Actuellement implémenté (22 oct 2016)
 * - NUM_INTEGER
 * - NUM_REAL
 * - NUM_PINFTY
 * - NUM_MINFTY
 * - NUM_REAL
 *
 * @return Renvoie 1 si égaux 0 sinon.
 */
int NUM_cmp(num a, num b)
{
	if(!NUM_cmp_type(a, b))	return(0);
	
	switch(a.numtype)
	{
		case NUM_INTEGER :
			return(NUM_cmp_lint(a, b));
			break ;
		case NUM_REAL :
			return(a.this.real==b.this.real);
			break;
		case NUM_PINFTY :
			return(1);
			/* Si a et b ont même type
			 * et NUM_PINFTY alors
			 * ils sont égaux
			 */
			break;
		case NUM_MINFTY :
			return(1);
			break;
		default :
			return(0);
	}
	return(0);

}


/** @fn int NUM_sign(num a)
 * @brief Retourne 1 si a positif, -1 si a négatif.
 *
 * Renvoie 0 pour un num non typé ou de type n'ayant pas
 * de signe.
 * NUM_COMPLEX : pas de signe.
 * @return 1 si a > 0, -1 si a < 0 et 0 si a nul.
 */
int NUM_sign(num a)
{
	switch(a.numtype)
	{
		case NUM_INTEGER :
			if(a.this.integer >= 0) return(1);
			else if(a.this.integer == 0) return(0);
			return(-1);
			break;
		case NUM_REAL :
			if(a.this.real >= 0) return(1);
			else if(a.this.real ==0) return(0);
			return(-1);
			break;
		case NUM_PINFTY :
			return(1);
		case NUM_MINFTY :
			return(-1);
		default :
			return(0);
	}
	return(0);
}

/** @fn int NUM_is_inf(const num a, const num b, int* flag)
 * @brief Renvoie a < b
 * @param a : premier paramètre de la comparaison.
 * @param b : Deuxième paramètre de la comparaison.
 * @param flag : Flag mis à 1 si erreur.
 * Flag mit à 1 si jamais on a un type inconnu.
 * Type comparable : REEL, ENTIER.
 * On ne compare pas les infinis.
 * Le flag peut valoir NULL.
 * @return
 */
int NUM_is_inf(const num a, const num b, int* flag)
{
	int existing_flag = 0;
	if(flag) existing_flag = 1;
	if(NUM_cmp_type(a, b) )
	{
		/* Cas ou a et b sont de mêmes types */
		if(a.numtype == NUM_REAL)
		{
			return(a.this.real < b.this.real);
		}
		if(a.numtype == NUM_INTEGER)
		{
			return(a.this.integer < b.this.integer);
		}
		/* Deux infinis de même signe ne sont pas comparables.
		 */
	}
	else 
	{
		if(a.numtype == NUM_REAL && b.numtype == NUM_INTEGER)
		{
			return(a.this.real < b.this.integer);
		}
		if(a.numtype == NUM_INTEGER && b.numtype == NUM_REAL)
		{
			return(a.this.integer < b.this.real);
		}
		if(a.numtype == NUM_PINFTY) return (0);
		if(a.numtype == NUM_MINFTY) return(1);
		/* Inifini plus grand que tout !*/
		if(b.numtype == NUM_MINFTY) return(0);
		if(b.numtype == NUM_PINFTY) return(1);
		/* Moins infini plus petit que tout */
	}
	WARNING_MSG("< : Impossible to evaluate");
	if(existing_flag) *flag = 1;
	return(0);

}


/** @fn int NUM_is_sup(const num a, const num b, int* flag)
 * @brief renvoie a > b.
 *
 * Fonctionnement identique à NUM_is_inf.
 * @sa NUM_is_inf()
 *
 * @return Renvoie a > b si evaluation possible. 0 sinon.
 */
int NUM_is_sup(const num a, const num b, int* flag)
{
	int existing_flag = 0;
	if(flag) existing_flag = 1;
	if(NUM_cmp_type(a, b) )
	{
		/* Cas ou a et b sont de mêmes types */
		if(a.numtype == NUM_REAL)
		{
			return(a.this.real > b.this.real);
		}
		if(a.numtype == NUM_INTEGER)
		{
			return(a.this.integer > b.this.integer);
		}
		/* Deux infinis de même signe ne sont pas comparables.
		 */
	}
	else 
	{
		if(a.numtype == NUM_REAL && b.numtype == NUM_INTEGER)
		{
			return(a.this.real > b.this.integer);
		}
		if(a.numtype == NUM_INTEGER && b.numtype == NUM_REAL)
		{
			return(a.this.integer > b.this.real);
		}
		if(a.numtype == NUM_PINFTY) return (1);
		if(a.numtype == NUM_MINFTY) return(0);
		/* Inifini plus grand que tout !*/
		if(b.numtype == NUM_MINFTY) return(1);
		if(b.numtype == NUM_PINFTY) return(0);
		/* Moins infini plus petit que tout */
		
	}
	WARNING_MSG("> : Impossible to evaluate");
	if(existing_flag) *flag = 1;
	return(0);
}
/** @fn int NUM_is_sup_equal(const num a, const num b, int* flag)
 * @brief Renvoie a >= b.
 *
 * @sa NUM_is_inf().
 *
 * @return Renvoie a >= b si comparaison possible 0 sinon.
 */
int NUM_is_sup_equal(const num a, const num b, int* flag)
{
	int existing_flag = 0;
	if(flag) existing_flag = 1;
	if(NUM_cmp_type(a, b) )
	{
		/* Cas ou a et b sont de mêmes types */
		if(a.numtype == NUM_REAL)
		{
			return(a.this.real >= b.this.real);
		}
		if(a.numtype == NUM_INTEGER)
		{
			return(a.this.integer >= b.this.integer);
		}
		/* Deux infinis de même signe ne sont pas comparables.
		 */
	}
	else 
	{
		if(a.numtype == NUM_REAL && b.numtype == NUM_INTEGER)
		{
			return(a.this.real >= b.this.integer);
		}
		if(a.numtype == NUM_INTEGER && b.numtype == NUM_REAL)
		{
			return(a.this.integer >= b.this.real);
		}
		if(a.numtype == NUM_PINFTY) return (1);
		if(a.numtype == NUM_MINFTY) return(0);
		/* Inifini plus grand que tout !*/
		if(b.numtype == NUM_MINFTY) return(1);
		if(b.numtype == NUM_PINFTY) return(0);
		/* Moins infini plus petit que tout */
	}
	WARNING_MSG(">= : Impossible to evaluate");
	if(existing_flag) *flag = 1;
	return(0);
}
/** @fn int NUM_is_inf_equal(const num a, const num b, int* flag)
 * @brief Renvoie a <= b.
 *
 * @sa NUM_is_inf()
 *
 * @return Renvoie a<=b, 0 si évaluation impossible.
 */
int NUM_is_inf_equal(const num a, const num b, int* flag)
{
	int existing_flag = 0;
	if(flag) existing_flag = 1;
	if(NUM_cmp_type(a, b) )
	{
		/* Cas ou a et b sont de mêmes types */
		if(a.numtype == NUM_REAL)
		{
			return(a.this.real <= b.this.real);
		}
		if(a.numtype == NUM_INTEGER)
		{
			return(a.this.integer <= b.this.integer);
		}
		/* Deux infinis de même signe ne sont pas comparables.
		 */
	}
	else 
	{
		if(a.numtype == NUM_REAL && b.numtype == NUM_INTEGER)
		{
			return(a.this.real <= b.this.integer);
		}
		if(a.numtype == NUM_INTEGER && b.numtype == NUM_REAL)
		{
			return(a.this.integer <= b.this.real);
		}
		if(a.numtype == NUM_PINFTY) return (0);
		if(a.numtype == NUM_MINFTY) return(1);
		/* Inifini plus grand que tout !*/
		if(b.numtype == NUM_MINFTY) return(0);
		if(b.numtype == NUM_PINFTY) return(1);
		/* Moins infini plus petit que tout */
	}
	WARNING_MSG("<= : Impossible to evaluate");
	if(existing_flag) *flag = 1;
	return(0);

}



/* Opérateur */

/** @fn num NUM_sum(num a, num b, int* flag )
 * @brief Effectue la somme de 2 num.
 *
 * Le flag est mis à un lorsqu'on a une erreur
 * de calcul
 * - Somme d'infinis de signe opposés
 * - Somme d'éléments de type non réels ou non entiers.
 *
 * @return Renvoie la somme de deux num.
 */
num NUM_sum(num a, num b, int* flag )
{
	/* On vérifie que le flag existe */
	char existing_flag = 0;
	if(flag) existing_flag = 1;

	if(NUM_cmp_type(a, b))
	{
		/* Num ont le même type */
		if(a.numtype == NUM_INTEGER)
		{
			a.this.integer += b.this.integer;
			return(a);
		}
		if(a.numtype == NUM_REAL)
		{
			a.this.real += b.this.real ;
			return(a);
		}
		if(a.numtype == NUM_PINFTY) return(a);
		if(a.numtype == NUM_MINFTY) return(a);
		/* Si aucun des cas ci-dessus c'est
		 * un cas non implémenté on
		 * renvoie a et un warning.
		 */
		WARNING_MSG("Trying to do an undefined sum");
		if(existing_flag) *flag = 1 ;
		return(a);
	}

	else
	{
		if(a.numtype == NUM_INTEGER && b.numtype == NUM_REAL)
		{
			/* a est un entier de b flottant
			 * on somme dans b
			 */
			b.this.real += a.this.integer ;
			return(b);
		}
		if(a.numtype == NUM_REAL && b.numtype == NUM_INTEGER)
		{
			/* Cas inverse
			 */
			a.this.real += b.this.integer ;
			return(a);
		}
		if(a.numtype == NUM_PINFTY || a.numtype == NUM_MINFTY)
		{
			if(b.numtype== NUM_REAL || b.numtype == NUM_INTEGER)
				return(a);
		}
		if(b.numtype == NUM_PINFTY || b.numtype == NUM_MINFTY)
		{
			if(a.numtype== NUM_REAL || a.numtype == NUM_INTEGER)
				return(b);
		}
		/* Ici on a un type pas implémenté
		 * ou des infinis de signe opposé.
		 */
		WARNING_MSG("Trying to do an undefined sum");
		if(existing_flag) *flag = 1 ;
		return(a);
	}
}

/** @fn num NUM_sub(num a, num b, int* flag )
 * @brief Calcul a - b.
 *
 * Si les num ont des types dont la 
 * soustraction n'est pas définie ou implémenté
 * le flag passe à 1.
 * Soustraction d'infinis de même signe cause une erreur.
 * Une opération sur un type autre que infini, réel, entier ==> erreur.
 * Le type renvoyer sera flottant si a ou b est flottant. 
 *
 * @return Renvoie le num résultat de la somme. a si erreur.
 */
num NUM_sub(num a, num b, int* flag )
{
	/* On vérifie que le flag existe */
	char existing_flag = 0;
	if(flag) existing_flag = 1;
	/* Le résultat renvoyé est toujours a */
	if(NUM_cmp_type(a, b))
	{
		/* Num ont le même type */
		if(a.numtype == NUM_INTEGER)
		{
			a.this.integer -= b.this.integer;
			return(a);
		}
		if(a.numtype == NUM_REAL)
		{
			a.this.real -= b.this.real ;
			return(a);
		}
		/* Si aucun des cas ci-dessus c'est
		 * un cas non implémenté on
		 * renvoie a et un warning.
		 */
		WARNING_MSG("Trying to do an undefined sum");
		if(existing_flag) *flag = 1 ;
		return(a);
	}

	else
	{
		if(a.numtype == NUM_INTEGER && b.numtype == NUM_REAL)
		{
			/* (a -b) = -(b - a)
			 * En effet je n'arrive pas
			 * convertir a d'entier à réel.
			 */
			b.this.real = (-1)*(b.this.real - a.this.integer) ;
			return(b);
		}
		if(a.numtype == NUM_REAL && b.numtype == NUM_INTEGER)
		{
			/* Cas inverse
			*/
			a.this.real -= b.this.integer ;
			return(a);
		}
		/* C'est l'opposé pour les infinis de
		 * la somme
		 * Quelque soit le type de b,
		 * on aura infini même si on a un infini
		 * c'est celui de signe opposé qui ramène
		 * a une somme selon
		 * +inf - truc_non infini = +inf
		 * -inf - truc_non_infi = -inf
		 */
		if(a.numtype == NUM_PINFTY ) return(a);
		if(a.numtype == NUM_MINFTY ) return(a);

		/* Ici on a un type pas implémenté
		 * ou des infinis de signe opposé.
		 */
		WARNING_MSG("Trying to do an undefined sum");
		if(existing_flag) *flag = 1 ;
		return(a);
	}
}

/** @fn num NUM_mul(num a, num b, int* flag )
 * @brief Produit de a par b.
 *
 * Similaire à PRIM_sum.
 * Tous les infinis passent seulement leur signe change.
 * 
 *
 * @sa PRIM_sum.
 *
 * @return
 */
num NUM_mul(num a, num b, int* flag )
{
	/* On vérifie que le flag existe */
	char existing_flag = 0;
	if(flag) existing_flag = 1;

	if(NUM_cmp_type(a, b))
	{
		/* Num ont le même type */
		if(a.numtype == NUM_INTEGER)
		{
			a.this.integer *= b.this.integer;
			return(a);
		}
		if(a.numtype == NUM_REAL)
		{
			a.this.real *= b.this.real ;
			return(a);
		}
		if(a.numtype == NUM_PINFTY) return(a);
		if(a.numtype == NUM_MINFTY)
		{
			a.numtype = NUM_PINFTY;
			return(a);
		}
		/* Si aucun des cas ci-dessus c'est
		 * un cas non implémenté on
		 * renvoie a et un warning.
		 */
		WARNING_MSG("Trying to do an undefined product");
		if(existing_flag) *flag = 1 ;
		return(a);
	}

	else
	{
		if(a.numtype == NUM_INTEGER && b.numtype == NUM_REAL)
		{
			/* a est un entier de b flottant
			 * on somme dans b
			 */
			b.this.real *= a.this.integer ;
			return(b);
		}
		if(a.numtype == NUM_REAL && b.numtype == NUM_INTEGER)
		{
			/* Cas inverse
			*/
			a.this.real *= b.this.integer ;
			return(a);
		}
		/* Gestion produit d'infinis */
		if(a.numtype == NUM_PINFTY && b.numtype == NUM_MINFTY) return(b);
		if(a.numtype == NUM_MINFTY && b.numtype == NUM_PINFTY) return(a);
		/* Gestion produit d'infini par un nombre */
		if(a. numtype == NUM_PINFTY || a.numtype == NUM_MINFTY)
		{
			if(NUM_sign(b) == 1) return(a);
			if(NUM_sign(b) == -1)
			{
				if(a.numtype == NUM_PINFTY) a.numtype = NUM_MINFTY;
				else if(a.numtype == NUM_MINFTY) a.numtype = NUM_PINFTY;
				return(a);
			}
			if(NUM_sign(b)== 0)
			{
				WARNING_MSG("0 * inf Undeterminated product");
				if(existing_flag) *flag = 1;
				return(a);
			}
		}
		if(b. numtype == NUM_PINFTY || b.numtype == NUM_MINFTY)
		{
			if(NUM_sign(a) == 1) return(b);
			if(NUM_sign(a) == -1)
			{
				if(b.numtype == NUM_PINFTY) b.numtype = NUM_MINFTY;
				else if(b.numtype == NUM_MINFTY) b.numtype = NUM_PINFTY;
				return(b);
			}
			if(NUM_sign(a)== 0)
			{
				WARNING_MSG("0 * inf Undeterminated product");
				if(existing_flag) *flag = 1;
				return(a);
			}
		}
		/* Ici on a un type pas implémenté
		 * ou des infinis de signe opposé.
		 */
		WARNING_MSG("Trying to do an undefined product");
		if(existing_flag) *flag = 1 ;
		return(a);
	}
}

/** @fn num NUM_div(num a, num b, int* flag)
 * @brief Calcule a/b.
 *
 * @return Renvoie le résultat du calcul.
 */
num NUM_div(num a, num b, int* flag)
{
	/* On vérifie que le flag existe */
	char existing_flag = 0;
	if(flag) existing_flag = 1;
	/* Le résultat renvoyé est toujours a */

	/* Inverse de 0 c'est infini */
	if(NUM_sign(b) == 0)
	{
		/* Division par zéro on renvoie l'infini */
		if(NUM_sign(a) == 1)
		{
			a.numtype = NUM_PINFTY;
			return(a);
		}
		if(NUM_sign(a) == -1)
		{
			a.numtype = NUM_MINFTY;
			return(a);
		}
		else 
		{
			WARNING_MSG("0/0 error !");
			if(existing_flag) *flag = 1 ;
			return(a);
		}
	}

	if(NUM_cmp_type(a, b))
	{
		/* Num ont le même type */
		if(a.numtype == NUM_INTEGER)
		{
			
			a.numtype = NUM_REAL;
			a.this.real = a.this.integer/b.this.integer;
			return(a);
		}
		if(a.numtype == NUM_REAL)
		{
			a.this.real=a.this.real/ b.this.real ;
			return(a);
		}
		/* Si aucun des cas ci-dessus c'est
		 * un cas non implémenté on
		 * renvoie a et un warning.
		 */
		WARNING_MSG("Trying to do an undefined division");
		if(existing_flag) *flag = 1 ;
		return(a);
	}

	else
	{
		if(a.numtype == NUM_INTEGER && b.numtype == NUM_REAL)
		{
			a.numtype = NUM_REAL;
			a.this.real = a.this.integer/b.this.real ;	
			return (a);
		}
		if(a.numtype == NUM_REAL && b.numtype == NUM_INTEGER)
		{
			/* Cas inverse
			*/
			a.this.real= a.this.real / b.this.integer ;
			return(a);
		}
		
		/* Infini diviser par qqch */
		if((a.numtype == NUM_PINFTY || a.numtype == NUM_MINFTY) 
				&& (b.numtype != NUM_MINFTY || b.numtype != NUM_PINFTY))
		{
			/* Infini sur un truc positif ou nul = inf */
			if(NUM_sign(b) >= 0) return(a);
			else /* On renvoie opposé de a */
			{
				if(a.numtype == NUM_PINFTY)
				{
					a.numtype = NUM_MINFTY;
				}
				else if(a.numtype == NUM_MINFTY)
				{
					a.numtype = NUM_PINFTY;
				}
				return(a);
			}
		}
		
		/* Ici on a un type pas implémenté
		 * ou des infinis de signe opposé.
		 */
		WARNING_MSG("Trying to do an undefined division");
		if(existing_flag) *flag = 1 ;
		return(a);
	}
}

/** @fn num NUM_modulo(num a, num b, int* flag)
 * @brief Calcule a%b.
 *
 * Renvoie le reste de la division euclidienne de a par b.
 * Les num doivent être entier si ce n'est pas le cas le
 * flag passe a 1.
 *
 * @return Renvoie un num résultat.
 */
num NUM_modulo(num a, num b, int* flag)
{
	int existing_flag = 0;
	if(flag) existing_flag = 1;
	if(!NUM_cmp_type(a, b)) 
	{
		if(existing_flag) *flag = 1;
		return a ;
	}
	if(a.numtype != NUM_INTEGER)
	{
		if(existing_flag) *flag = 1;
		return(a);
	}

	a.this.integer = a.this.integer % b.this.integer ;
	return(a);
}

/** @fn num NUM_quotient(num a, num b, int* flag)
 * @brief Calcule a/b pour des entiers.
 *
 * Effectue une division euclidienne et renvoie le quotient.
 * En pratique on reprend le code de la division uniquement sur
 * les entiers et on ne caste pas en double. Flag mis à 
 * 1 si jamais ce ne sont pas des entiers.
 * Si b est O le flag est mis à 1 et un message de warning_msg apparaît.
 *
 * @return Renvoie le num résultat.
 */
num NUM_quotient(num a, num b, int* flag)
{
/* On vérifie que le flag existe */
	char existing_flag = 0;
	if(flag) existing_flag = 1;
	/* Le résultat renvoyé est toujours a */
	if(a.numtype ==NUM_INTEGER && NUM_cmp_type(a, b) )
	{
		a.this.integer = a.this.integer/b.this.integer;
		return(a);
	}
	if(existing_flag) *flag = 1;
	return(a);

}
