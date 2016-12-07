
/**
 * @file repl.c
 * @author François Cayre <cayre@yiking.(null)>
 * @author Rimbaud Sébastien.
 * @date 22 Oct 2016 19:33
 * @brief Boucle principale interpréteur scheme.
 *
 * Constitue la boucle principale de l'interpréteur.
 * Se charge d'analyser la chaine et de voir si celle-ci
 * possède le bon format (nombre de parenthèses, guillemets ect).
 * Effectue :
 * - Livrable 1 : Construction des objets depuis un lecture.
 * - Livrable 2 : Evaluation des objet.
 * 	- define
 * 	- set!
 * 	- if
 * 	- and
 * 	- or
 * 	- quote et sa forme abrégée '
 */



#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#include "primitive.h"
#include "read.h"
#include "eval.h"
#include "print.h"

/* mode d'interaction avec l'interpreteur (exemple)*/
typedef enum {INTERACTIF,SCRIPT} inter_mode;


/** @fn void usage_error( char *command ) 
 * @brief Renvoie un message d'erreur si la fonction est mal utilisée
 * en ligne de commande.
 *
 */
void usage_error( char *command ) {

    fprintf( stderr, "Usage: %s [file.scm]\n   If no file is given, executes in Shell mode.\n", command );
}


/** @var object nil
 * @brief Représente la fin d'une liste ou la liste vide.
 */
object nil;
/** @var object vrai
 * @brief Représente booléen TRUE.
 */
object vrai;
/** @var object faux
 * @brief Représente booléen faux.
 */
object faux;
/** @var object env
 * @brief Représente l'environnement courant.
 */
object env;
/** @var object form[6];
 * @brief Décrit l'ensemble des formes du langage.
 */
object form[NB_FORM];
/** @fn void init_interpreter ( void ) 
 * @brief Initialise les variables globales de l'interpréteur.
 *
 * Initialise nil, true, false, env.
 * @sa nil
 * @sa vrai
 * @sa faux
 * @sa env
 */

void init_interpreter ( void ) {

	nil = make_nil();
	vrai = make_true();
	faux = make_false();
	/* Construction environnement père */
	env = ENV_build(NULL);
	make_forms();
	/* Construction des primitives dans le top level */
	init_primitive();
}

int main ( int argc, char *argv[] ) {

    char     input[BIGSTRING];
    uint     here = 0;
    object   output = NULL;
    object   sexpr = NULL;
    inter_mode mode;
    FILE *   fp = NULL; /* le flux dans lequel les commande seront lues : stdin (mode shell) ou un fichier */

    /* exemples d'utilisation des macros du fichier notify.h */
    /* WARNING_MSG : sera toujours affiche */
    WARNING_MSG("Un message WARNING_MSG !");

    /* macro INFO_MSG : uniquement si compil avec -DVERBOSE. Cf Makefile*/
    INFO_MSG("Un message INFO_MSG : Debut du programme %s", argv[0]);

    /* macro DEBUG_MSG : uniquement si compil avec -DDEBUG (ie : compil avec make debug). Cf Makefile */
    DEBUG_MSG("Un message DEBUG_MSG !");

    /* La ligne suivante provoquerait l'affichage du message
       puis la sortie du programme avec un code erreur non nul (EXIT_FAILURE) */
    /* ERROR_MSG("Erreur. Arret du programme"); */

    if ( argc > 2 ) {
        usage_error( argv[0] );
        exit( EXIT_FAILURE );
    }
    if(argc == 2 && strcmp(argv[1], "-h") == 0) {
        usage_error( argv[0] );
        exit( EXIT_SUCCESS );
    }

    init_interpreter();

    /*par defaut : mode shell interactif */
    fp = stdin;
    mode = INTERACTIF;
    if(argc == 2) {
        /* mode fichier de commandes */
        fp = fopen( argv[1], "r" );
        if ( fp == NULL ) {
            perror( "fopen" );
            exit( EXIT_FAILURE );
        }
        mode = SCRIPT;
    }


    while ( 1 ) {
        input[0]='\0';
        here = 0;
        output = NULL;
        sexpr = NULL;
        uint Sexpr_err;

        Sexpr_err = sfs_get_sexpr( input, fp );

        if ( S_OK != Sexpr_err) {
            /* si fichier alors on sort*/
            if (mode == SCRIPT) {
                fclose( fp );
                if (Sexpr_err==S_END) {
                    /* Cas fin de fichier script */
                    exit(EXIT_SUCCESS);
                }
                /* Cas S-Expression mal formee dans le fichier script */
                ERROR_MSG("Malformed S-expression --- Aborts");
            }
            /*sinon on rend la main à l'utilisateur*/
            continue;
        }

        if ( 0 == strlen( input ) ) {
            continue;
        }

        /* Pour le step 0 on se contente d'afficher */
        /* la S-Expression lue */
        /* puis d'en lire une autre */
        /* METTRE EN COMMENTAIRE LES DEUX LIGNES SUIVANTES */
        /* POUR PASSER A L'INCREMENT 1 */
        /*printf("%s\n", input );*/
        /*continue;*/

        here  = 0;
        sexpr = sfs_read( input, &here );
        if ( NULL == sexpr ) {
            /* si fichier alors on sort*/
            if (mode == SCRIPT) {
                fclose( fp );
                /*macro ERROR_MSG : message d'erreur puis fin de programme ! */
                ERROR_MSG("Invalid S-expression --- Aborts");
            }
            /*sinon on rend la main à l'utilisateur*/
            continue ;
        }
	
        output = sfs_eval( sexpr, env); 
        if( NULL == output) {
            /* si fichier alors on sort*/
            if (mode == SCRIPT) {
                fclose( fp );
                /*macro ERROR_MSG : message d'erreur puis fin de programme ! */
                ERROR_MSG("Error while evaluating input --- Aborts");
            }
            /*sinon on rend la main à l'utilisateur*/
            continue ;
        }

        sfs_print( output );
	/* On free tout ce qu'on a lu et évalué
	 * Normalement eval travaille sur la même
	 * allocation que celle de read,
	 * elle ne fait de nouvelles allocations
	 * que pour enregistrer ou mettre à jour
	 * des variables. Dans ce second cas elle
	 * se charge de la suppression des anciennes
	 * valeurs.
	 */
	/*OBJECT_destroy(&sexpr);*/
    }

    if (mode == SCRIPT) {
        fclose( fp );
    }
    exit( EXIT_SUCCESS );
}
