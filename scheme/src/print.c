
/**
 * @file print.c
 * @author François Cayre <cayre@yiking.(null)>
 * @date Fri Jun 22 20:12:17 2012
 * @brief Printing stuff for SFS.
 *
 * Printing stuff for SFS.
 */

#include "print.h"

#include <stdio.h>

/** @fn  void sfs_print_atom( object o )
 * @brief affiche un atome.
 *
 * @sa OBJECT_print_atom(object o)
 *
 */
void sfs_print_atom( object o )
{
	
	OBJECT_print_atom(o);
   return;
}

/** @fn  void sfs_print_pair( object o )
 * @brief Affiche une paire.
 *
 * Affiche un paire en ouvrant une parenthèse puis en
 * appelant OBJECT_print_pair.
 *
 */
void sfs_print_pair( object o )
{

	printf("(");
	OBJECT_print_pair(o);
    return;
}

/** @fn  void sfs_print( object o ) {
 * @brief Affiche un objet dans l'interpréteur.
 *
 * Affiche un objet.
 * Si celui-ci est de type SFS_UNKNOWN retourne seulement
 * un blanc.
 * Si l'objet est NULL, retourne un blanc également.
 *
 * @sa sfs_print_pair
 * @sa sfs_print_atom
 */
void sfs_print( object o )
{
	if(OBJECT_isempty(o))
	{
		OBJECT_print_fail();
		return;
	}
	if(check_type(o, SFS_UNKNOWN)) return;
	printf( "==> " );

    if ( SFS_PAIR == o->type ) {
        sfs_print_pair( o );
    }
    else if (o->type == SFS_NIL)
    {
	    printf("()");/* Pour gérer cas liste et pas liste */
    }
    else {
        sfs_print_atom( o );
    }
    printf( "\n" );

    return ;

}


/*Fonctions d'affichage*/

/**@fn void OBJECT_print_number( object o)
 * @brief Affiche un object de type entier.
 * @param object : l'objet à afficher.
 */
void OBJECT_print_number(object o)
{
	NUM_print(o->this.number);
}

/**@fn void OBJECT_print_character( object o)
 * @brief Affiche un object de type character.
 * @param object : l'objet à afficher.
 */
void OBJECT_print_character(object o)
{
	if(o->this.character =='\n')
	{
		printf("#\\newline");
		return ;
	}
	if(o->this.character==' ')
	{
		printf("#\\space");
		return;
	}
	printf("#\\%c", o->this.character ) ;	
}

/**@fn void OBJECT_print_string( object o)
 * @brief Affiche un object de type string.
 * @param object : l'objet à afficher.
 */
void OBJECT_print_string(object o)
{
	printf("\"%s\"", o->this.string);
}

/**@fn void OBJECT_print_symbol( object o)
 * @brief Affiche un object de type string.
 * @param object : l'objet à afficher.
 */
void OBJECT_print_symbol(object o)
{
	printf("%s", o->this.symbol);
}
/**@fn void OBJECT_print_bool( object o)
 * @brief Affiche un object de type bool.
 * @param object : l'objet à afficher.
 */
void OBJECT_print_bool(object o)
{
	/* On évite de trop utiliser la variable globale.*/
	if(o==vrai) printf("#t");
	else printf("#f");
	/* Penser à gérer cas #m, #r ...*/	
}
/**@fn void OBJECT_print_nil( int place)
 * @brief Affiche un object de type nil.
 * @param place : lieu du nil (SFS_CDR ou SFS_CAR)
 *
 * Si nil se situe dans un car affiche celui-ci sous forme
 * () sinon seulement sous forme ")".
 * @param object : l'objet à afficher.
 */
void OBJECT_print_nil(int place)
{
	if(place == SFS_CDR)
	{
		printf(")");
		return;
	}
	else
	{
		printf("()");
		return;
	}
}
/**@fn void OBJECT_print_fail()
 * @brief Affiche un object qui n'est pas reconnu. Affiche un warning.
 *
 * Affiche alors #unrecognize.
 * C'est une interprétation de scheme ici.
 */
void OBJECT_print_fail()
{
	WARNING_MSG("Try to print an unrecognized type");
}

/**@fn void OBJECT_print_pair( object o)
 * @brief Affiche un object de type pair. Fonction récursive.
 * @param object : l'objet à afficher.
 *
 * La fonction gére de façon particulière le cas de nil
 * selon si il se trouve dans le car ou le cdr.
 * Un nil seul est géré par sfs_print()
 *
 * @n Gestion d'une paire impure :
 * Une paire pure, contient une paire et
 * un cdr qui n'est pas une paire.
 * Dans ce cas affiche au forme (car . cdr)
 *
 * @n Gestion type unknown :
 * Si un type unknown est croisé dans une liste
 * affiche #undef pour avoir un affichage cohérent.
 *
 * @sa OBJECT_print_unknown
 *
 */
void OBJECT_print_pair(object o)
{
	if(o->type != SFS_PAIR)
	{
		OBJECT_print_atom(o);
		
		return;
	}
	else
	{/* Cas ou on a affaire à une paire */
		/*Affichage car */
		if(o->this.pair.car->type == SFS_PAIR) printf("(");
		if(o->this.pair.car->type==SFS_NIL)
		{
			OBJECT_print_nil(SFS_CAR);
			
		}
		else if(check_type(o->this.pair.car, SFS_UNKNOWN))
			OBJECT_print_unknown(o);
		else OBJECT_print_pair(o->this.pair.car);

		/*affichage cdr */
		if(o->this.pair.cdr->type !=SFS_NIL)
		{
			if(!check_type(o->this.pair.cdr, SFS_PAIR))
			{
				/* Cas d'une paire pure */
				printf(" . ");
				OBJECT_print_atom(o->this.pair.cdr);
				if(check_type(o->this.pair.cdr, SFS_UNKNOWN))
					OBJECT_print_unknown(o);
				printf(")");
			}
			else
			{
				printf(" ");
				OBJECT_print_pair(o->this.pair.cdr);
			}

		}
		
		else
		{
			OBJECT_print_nil(SFS_CDR);
		}
		return;
	}
}

/** @fn void OBJECT_print_prim(object o)
 * @brief Affiche un type primitive.
 *
 * En pratique on va chercher le nom de la primitive
 * dans la structure prim de object pour l'afficher.
 *
 */
void OBJECT_print_prim(object o)
{
	OBJECT_print_symbol(o->this.prim.name);
}

/**@fn void OBJECT_print_atom( object o)
 * @brief Affiche un atome. Appel OBJECT_print_fail() si le type
 * est non reconnu.
 *
 * Ne devrait pas avoir à afficher nil. On préfère utiliser sfs_print
 * pour afficher nil ou sfs_print_pair lorsque celui-ci est 
 * dans une liste.
 * Le cas est cependant présent au cas est affiche un warning s'il
 * se produit. Un print("()") est alors fait dans ce cas.
 *
 * @param object : l'objet à afficher.
 */
void OBJECT_print_atom ( object o ) 
{

    switch(o->type)
    {
	    case SFS_NUMBER :
		    OBJECT_print_number(o);
		    break;
	    case SFS_CHARACTER :
		    OBJECT_print_character(o); 
		    break;
	    case SFS_STRING :
		    OBJECT_print_string(o);
		    break;
	    case SFS_SYMBOL :
		    OBJECT_print_symbol(o);
		    break;
	    case SFS_BOOLEAN :
		    OBJECT_print_bool(o);
		    break;
	    case SFS_NIL :
		    printf("()");
		    break;
		    /*Ne devrait pas se produire*/
	    case SFS_PRIM :
		    OBJECT_print_prim(o);
		    break;
	    case SFS_COMP :
		    OBJECT_print_compound(o);
		    break;
	    case SFS_UNKNOWN :
		    break;
	    default :
		    DEBUG_MSG("Fail print :");
		    OBJECT_print_fail(); /* Cas ou on a pas de correspondance.*/
    }
    return;
}


/** @fn void OBJECT_print_compound(object o);
 * @brief Affiche un agrégat.
 *
 * Affiche #<procedure>
 */
void OBJECT_print_compound(object o)
{
	printf("#<procedure>");
}

/** @fn void OBJECT_print_unknown(object o)
 * @brief Affiche un type unknown (de base : #undef)
 *
 * Affiche un type unknown. De base affiche #undef.
 * Peut-être enrichi pour afficher des messages
 * d'erreur selon le contenu de o.
 * Utile pour l'affiche d'un unknown dans une paire.
 *
 * @return
 */
void OBJECT_print_unknown(object o)
{
	printf("#undef");
}
