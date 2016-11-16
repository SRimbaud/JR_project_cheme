#include"number.h"

/** @file number.c
 * @author Rimbaud Sébastien sebastien.rimbaud@phelma.grenoble-inp.fr
 * @brief ensemble de fonctions relatives au type num.
 */

/* Affichage */

/**@fn void NUM_print( num printed, int mode)
 * @brief Affiche un num.
 * @param printed : le num à afficher.
 * @param mode : le type du num (réel, entier, uentier, complexe.....).
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

/**@fn num NUM_set_lint(num* a, long int v, int mode)
 * @brief Enregistre dans a l'entier long v, retourne le nouveau num. Met à jour champ numtype à NUM_INTEGER.
 * @param num* : num ou on doit enregistrer la valeur.
 * @param v : entier long à enregistrer.
 * @param mode : Mode NUM_PINFTY NUM_MINFTY ou NUM_INTEGER
 *
 * Si le mode n'est pas une constante implémentée dans la fonction :
 *
 * - NUM_REAL    	(Not implemented) 
 * - NUM_INTEGER  
 * - NUM_UINTEGER	(Not implemented) 
 * - NUM_COMPLEX  	(Not implemented)
 * - NUM_UNDEF    
 * - NUM_PINFTY   
 * - NUM_MINFTY   
 * Il est mis par défaut à NUM_UNDEF
 * @return : Renvoie un pointeur sur le num modifié.
 */
num* NUM_set_lint(num* a, long int v, int mode)
{
	switch(mode)
	{
		case NUM_INTEGER :
			a->numtype= NUM_INTEGER;
			a->this.integer = v;
			break;
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

/* Opérateur */

/** @fn num NUM_sum(num a, num b, int* flag )
 * @brief Effectue la somme de 2 num.
 *
 * Le flag est mis à un lorsqu'on a une erreur
 * de calcul
 * - Somme d'infini de signe opposé
 * - Somme d'éléments de type non réel ou entier.
 *
 * @return Renvoie la somme de deux num.
 */
num NUM_sum(num a, num b, int* flag )
{
	/* On vérifie que le flag existe */
	char existing_flag = 0;
	if(!flag) existing_flag = 1;

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
		if(a.numtype == NUM_INTEGER)
		{
			/* a est un entier de b flottant
			 * on somme dans b
			 */
			b.this.real += a.this.integer ;
			return(b);
		}
		if(a.numtype == NUM_REAL)
		{
			/* Cas inverse
			 */
			a.this.real += b.this.integer ;
			return(a);
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
 * @brief Soustrait deux num.
 *
 * Si les num ont des types dont la 
 * soustraction n'est pas définie ou implémenté
 * le flag passe à 1.
 *
 * @return Renvoie le num résultat de la somme. a si erreur.
 */
num NUM_sub(num a, num b, int* flag )
{
	/* On vérifie que le flag existe */
	char existing_flag = 0;
	if(!flag) existing_flag = 1;

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
		if(a.numtype == NUM_INTEGER)
		{
			/* a est un entier de b flottant
			 * on somme dans b
			 */
			b.this.real -= a.this.integer ;
			return(b);
		}
		if(a.numtype == NUM_REAL)
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
		 */
		if(a.numtype == NUM_PINFTY) return(a);
		if(a.numtype == NUM_MINFTY) return(a);

		/* Ici on a un type pas implémenté
		 * ou des infinis de signe opposé.
		 */
		WARNING_MSG("Trying to do an undefined sum");
		if(existing_flag) *flag = 1 ;
		return(a);
	}
}
/*
num NUM_mul(num a, num b, int* flag )
{
	return(0);
}
num NUM_div(num a, num b)
{
	return(0);
}
*/

