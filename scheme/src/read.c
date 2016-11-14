
/**
 * @file read.c
 * @author François Cayre <cayre@yiking.(null)>
 * @date Fri Jun 15 18:31:05 2012
 * @brief Read routine for SFS.
 *
 * Read routine for SFS.
 */

#include <stdio.h>
#include <ctype.h>
#include"read.h"


/** @fn  void flip( uint *i ) 
 * @brief Prend la négation de i. Equivalent à une porte non.
 *
 * @return Retourne !i.
 */
void flip( uint *i ) 
{

    if ( *i == FALSE ) {
        *i = TRUE;
    }
    else {
        *i = FALSE;
    }
}

/**
 * @fn char* first_usefull_char(char* line)
 *
 * @brief retourne un pointeur sur le premier caractere utile dans line
 * ou NULL si line ne contient que des espaces et des commentaires
 */
char* first_usefull_char(char* line) {

    int i=0;
    if (line == NULL) {
        return NULL;
    }
    i = 0;
    /* on saute les espaces */
    while(line[i] != '\0' && isspace(line[i])) {
        i++;
    }
    /* si fin de ligne => ligne inutile */
    if(line[i] == '\0') {
        return NULL;
    }
    /* si premier caractere non espace est ';' => ligne inutile */
    if(line[i] == ';') {
        return NULL;
    }
    return line + i; /* ligne utile */
}

/**
 * @fn uint  sfs_get_sexpr( char *input, FILE *fp )
 *
 * @brief extrait la prochaine S-Expression dans le flux fp et la stocke dans input
 * (meme si elle est repartie sur plusieurs lignes)
 * @param fp (FILE *) flux d'entree (ouvert en ecriture, mode texte)
 * @param input (char *) chaine allouee de taille BIGSTRING, dans laquelle la S-Expression sera stockée
 *
 * @return S_OK si une S-Expression apparemment valide a ete trouvee
 * @return S_KO si on n'a pas trouve de S-Expression valide
 * @return S_END si fin de fichier atteinte sans avoir lu de caractere utile.
 *
 * sfs_get_sexpr commence par lire une ligne dans fp,
 * puis compte le nombre de parentheses ouvrantes et fermantes sur la ligne.
 * Les parentheses dans des chaines et les caracteres Scheme #\( et #\)
 * ne sont pas comptes.
 *
 * Si le compte devient zéro et que 
 *        - la ligne est fini, la fonction retourne S_OK
 * 				- la ligne n'est pas fini la fonction retourne S_KO
 *
 * S'il y a plus de parentheses fermantes qu'ouvrantes,
 * la fonction retourne S_KO.
 * Les commentaires et espaces qui figurent a la fin de chaque ligne
 * sont remplacés par un espace.
 * Les espaces qui figurent a la fin de la S-Expression (y compris '\n')
 * sont supprimés.
 *
 * Attention : cette fonction refuse les S-Expression multiple sur une seule ligne. Ainsi :
 *    a b c
 *    (qqchose) (autrechose)
 *    (qqchose) 78
 * seront considereees comme des erreurs
 * et la fonction retournera S_KO
 *
 * @pre fp doit etre prealablement ouvert en lecture
 * @pre input doit etre prealablement alloue en memoire, de taille BIGSTRING
 */

typedef enum {
    NOTHING,        /*!< rien n'a ete trouve encore.. */
    STRING_ATOME,   /*!< la premiere trouvee dans la ligne semble etre un atome */
    BASIC_ATOME,    /*!< la premiere trouvee dans la ligne semble etre d'une chaine */
    S_EXPR_PARENTHESIS, /*!< la premiere trouvee dans la ligne semble etre une expression parenthesee */
    FINISHED        /*!< on a trouve une S-Expr bien formee */
} EXPRESSION_TYPE_T;

uint  sfs_get_sexpr( char *input, FILE *fp ) {
    int       parlevel = 0;
    uint      in_string = FALSE;
    uint      s = 0;
    char      k[BIGSTRING];
    char      *ret = NULL;
    char      *chunk = NULL;
    uint      i = 0;
    string    sfs_prompt;

    EXPRESSION_TYPE_T typeOfExpressionFound = NOTHING;

    parlevel = 0;
    memset( input, '\0', BIGSTRING );

    do {
        ret = NULL;
        chunk=k;
        memset( chunk, '\0', BIGSTRING );

        /* si en mode interactif*/
        if ( stdin == fp ) {
            uint nspaces = 2*parlevel;

            init_string( sfs_prompt );

            /* le prompt indique le niveau de parenthese
               et decale la prochaine entrée en fonction
               de ce niveau (un peu à la python)*/
            sprintf( sfs_prompt, "SFS:%u > ", parlevel );

            for ( i= 0; i< nspaces; i++ ) {
                sfs_prompt[strlen(sfs_prompt)] = ' ';
            }

            /* si sur plusieurs lignes, le \n équivaut à un espace*/
            if (nspaces>0) {
                input[strlen(input)+1] = '\0';
                input[strlen(input)] = ' ';
            }

            /*saisie de la prochaine ligne à ajouter dans l'input*/
            printf("%s",sfs_prompt);
            ret = fgets( chunk, BIGSTRING, fp );
            if (ret && chunk[strlen(chunk)-1] == '\n') chunk[strlen(chunk)-1] = '\0';

        }
        /*si en mode fichier*/
        else {
            ret = fgets( chunk, BIGSTRING, fp );

            if ( NULL == ret ) {
                /* fin de fichier...*/
                if ( parlevel != 0 ) {
                    WARNING_MSG( "Parse error: missing ')'" );
                    return S_KO;
                }
                return S_END;
            }

            if (strlen(chunk) == BIGSTRING-1
                    && chunk[BIGSTRING-1] != '\n'
                    && !feof(fp)) {
                WARNING_MSG( "Too long line for this interpreter!" );
                return S_KO;
            }
        }

        /* si la ligne est inutile 
        	=> on va directement à la prochaine iteration */
        if (first_usefull_char(chunk) == NULL) {
            continue;
        }


        s = strlen( chunk );

        if ( s > 0 ) {
            if (strlen(input) + s > BIGSTRING-1 ) {
                WARNING_MSG( "Too long a S-expression for this interpreter!" );
                return S_KO;
            }

            for ( i = 0; i< strlen(chunk); i++ ) {
                /* si la fin de la ligne chunk est inutile,
                   on ajoute un espace dans input et on sort de la boucle*/
                if ( in_string == FALSE && first_usefull_char(chunk + i) == NULL ) {
                    chunk[i]='\0';
                    input[strlen(input)] = ' ';
                    break;
                }


                switch(chunk[i]) {
                case '(':
                    if (in_string == FALSE
                            && ! ( i>1 && chunk[i-1] == '\\' && chunk[i-2] == '#' ) ) {
                        parlevel++;
                        typeOfExpressionFound = S_EXPR_PARENTHESIS;
                    }
                    break;
                case ')':
                    if ( in_string == FALSE
                            && ! ( i>1 && chunk[i-1] == '\\' && chunk[i-2] == '#' ) ) {
                        parlevel--;
                        if (parlevel == 0 ) {
                            typeOfExpressionFound = FINISHED;
                        }
                        if ( parlevel < 0 ) {
                            WARNING_MSG( "Parse error : cannot start with ')'" );
                            return S_KO;
                        }
                    }
                    break;
                case '"':
                    if ( i<2 || chunk[i-1] != '\\' ) {
                        if ( in_string == FALSE ) {
                            if(typeOfExpressionFound == BASIC_ATOME) {
                                WARNING_MSG("Parse error: invalid string after atom : '%s'", chunk+i);
                                return S_KO;
                            }
                            in_string = TRUE;
                            if(typeOfExpressionFound != S_EXPR_PARENTHESIS) {
                                typeOfExpressionFound = STRING_ATOME;
                            }
                        }
                        else {
                            in_string = FALSE;
                            if(typeOfExpressionFound == STRING_ATOME) {
                                typeOfExpressionFound = FINISHED;
                            }
                        }
                    }
                    break;
                default:
                    if(in_string == FALSE) {
                        if(isspace(chunk[i])) {
                            if(typeOfExpressionFound == BASIC_ATOME) {
                                typeOfExpressionFound = FINISHED;
                            }
                        } else if(typeOfExpressionFound != S_EXPR_PARENTHESIS) {
                            typeOfExpressionFound = BASIC_ATOME;
                        }
                    }
                    break;
                }


                if(typeOfExpressionFound == FINISHED) {
                    char *first_useful = first_usefull_char(chunk + i + 1);
                    if( first_useful != NULL) {
                        if(*first_useful == ')' ) {
                            WARNING_MSG( "Parse error: too many closing parenthesis')'" );
                        }
                        else {
                            WARNING_MSG("Parse error: invalid trailing chars after S-Expr : '%s'", chunk+i);
                        }
                        return S_KO;
                    }
                }

                /* recopie char par char*/
                input[strlen(input)] = chunk[i];
            }
            if(in_string == TRUE) {
                WARNING_MSG( "Parse error: non terminated string on line %s", chunk );
                return S_KO;
            }
        }


        if ( parlevel > 0 && fp != stdin ) {
            if ( feof( fp ) ) {
                WARNING_MSG( "Parse error: missing ')'" );
                return S_KO;
            }

            if (input[strlen(input)-1] == '\n') input[strlen(input)-1] = ' ';
        }
    } while ( parlevel > 0 );

    /* Suppression des espaces restant a la fin de l'expression, notamment le dernier '\n' */
    while (isspace(input[strlen(input)-1])) input[strlen(input)-1] = '\0';

    return S_OK;
}

/** @fn object sfs_read( char* input, uint *here)
 * @brief Lit une expression entrée par l'utilisateur et construit
 * l'objet correspondant.
 * @param input : Entrée utilisateur
 * @param here : Curseur sur le lieu ou l'on se situe dans input.
 *
 * @return Renvoie l'object correspondant à l'input
 */
object sfs_read( char *input, uint *here )
{

	*here = wait_first_non_blank_char(input, here);
	if(input[*here] == '\'')
	{
		return(OBJECT_built_short_quote(input, here));
	}
    if ( input[*here] == '(' ) {
	    DEBUG_MSG("Pair detected");
	*here = wait_first_non_blank_char(input, here);
        if ( input[(*here)] == ')' ) {
            *here += 1;
            return nil;
        }
        else {
            (*here)++;
            return sfs_read_pair( input, here );
        }
    }
    else {
        return sfs_read_atom( input, here );
    }
}

/**@fn sfs_read_atom( char* input, uint* here)
 * @brief Lit une entrée utilisateur et construit l'objet correspondant.
 * @param input : Entrée
 * @param here : curseur sur l'entrée.
 *
 * L'objet renvoyé possède le type SFS_UNKNOWN si l'entrée utilisateur est mauvaise.
 * Dans ce cas la fonction affiche un warning spécifiant que le type n'est pas reconnu.
 * Elle renvoie alors NULL.
 * Fait appel à sfs_type_atom pour identifier le type de l'entrée.
 * Lorsque le type n'est pas reconnu elle renvoie également un warning sans interrompre le 
 * programme.
 * Pour plus de détails sur les erreurs voir :
 * - OBJECT_int_update(object o, char* input, uint* here)
 * - OBJECT_character_update(object o, char* input, uint* here)
 * - OBJECT_string_update(object o, char* input, uint* here)
 * - OBJECT_boolean_update(object o, char* input, uint* here)
 * - OBJECT_symbol_update(object o, char* input, uint*here)
 *
 * Types reconnus :
 * - entier signés
 * - caractères
 * - booléens
 * - symboles
 * - listes (paires)
 * - nil
 * - chaine de caractère
 *
 * @return Renvoie l'envoie construit.
 */
object sfs_read_atom( char *input, uint *here ) {


    *here = wait_first_non_blank_char(input, here);
    object atom = NULL;
    int type = sfs_type_atom(input, here);
    int error = 0;
    DEBUG_MSG("Type : %d", type);
    /* Construction de l'object avec le type voulu puis mise à jour du contenu */
    atom = make_object(type);
    
    switch(type)
    {
	    case SFS_NUMBER :
		    error = OBJECT_int_update(atom, input, here);
		    break;
	    case SFS_CHARACTER :
		    error = OBJECT_character_update(atom, input, here);
		    break;
	    case SFS_STRING :
		    error = OBJECT_string_update(atom, input, here);
		    break;
	    case SFS_BOOLEAN :
		    error = OBJECT_boolean_update(&atom, input, here);
		    /* Inutile de free atom il sera free après
		     * sfs_print dans repl.c*/
		    break;
	    case SFS_SYMBOL :
		    error = OBJECT_symbol_update(atom, input, here);
		    break;
	    default : /* Cas d'une erreur */
		    WARNING_MSG("Unrecognized type : %s", input + *here);
    }
    if(error ==1 ) return NULL;
    return atom;
}

/** @fn sfs_read_pair( char *stream, uint *i)
 * @brief Construit une liste à partir d'une entrée utilisateur. Récursif.
 * @param stream : Entrée.
 * @param i : Curseur sur l'entrée. Incrémente le curseur.
 *
 * Fonction récursive.
 * Les conditions sont une parenthèse fermante ou une fin de chaine.
 * Dans ce cas cette fonction retourne la variable globale nil.
 *
 * @sa repl.c
 * 
 * @return : Renvoie un object qui correspond à la liste construite.
 */
object sfs_read_pair( char *stream, uint *i ) {

	*i = wait_first_non_blank_char(stream, i);
	if( stream[*i] == ')') 
	{
		(*i)++;
		return(nil);
	}
	if( stream[*i]=='\0') return(nil); /* Cas fin de chaine */
    object pair = make_object(SFS_PAIR);
	if(!pair) /* Controle de l'alloc */
	{
		ERROR_MSG("Allocation error --abort");
		return(nil);
	}
	/* Condition d'arrêt */
	pair->this.pair.car = sfs_read(stream, i);
	pair->this.pair.cdr = sfs_read_pair(stream, i);


    return pair;
}
/** @fn int sfs_type_atom(char* chaine, uint *here)
 * @brief détermine le type d'un atome et le renvoie. N'incrémente pas here.
 * @param chaine : Chaine dans laquelle trouver le type de l'atome
 * @parem here : curseur sur la chaine.
 *
 * @return : Renvoie le type de l'atome. Retourne SFS_UNKNOWN aucun type ne correspond.
 */
int sfs_type_atom(char* chaine, uint* here)
{
	/* On regarde premier caractère
	 * Penser à ajouter une gestion propre des erreurs si jamais il y a un pb avec la chaine
	 */
	char first = chaine[*here];
	char second = chaine[*here +1]; /* Normalement pas d'erreur ici, ajouter gestion des erreurs.*/
	if(isdigit(first)) return(SFS_NUMBER);
	if(first == '+' || first == '-')
	/* Cas d'un nombre ENTIER ou du SYMBOLE + ou - voir flottant et complexe plus tard. */
	{
		if(isdigit(second)) return(SFS_NUMBER);
		if(isspace(second) || second =='\0') return(SFS_SYMBOL);
		return(SFS_UNKNOWN); /* Si on arrive ici c'est ni un symbole ni un int*/
	}
	if(first == '\"') /* On s'attend à recevoir une chaine de caractère */
	{
		return SFS_STRING ;
	}
	if (first== '#')
	{
		if(second == '\\') /* Cas d'un caractère */
		{
			return( SFS_CHARACTER);
		}
		else /* Cas d'un booléen */
		{
			return( SFS_BOOLEAN);
		}
	}
	
	if(isealnum(first)) /* Cas d'un symbole, à étoffer pour gérer les majuscules et minuscules */
	{
		return(SFS_SYMBOL);
	}
	if(first == '\'') /* Cas du quote abrégé */
	{
		DEBUG_MSG("Pair détectée \' ");
		return(SFS_PAIR);
	}
	return(SFS_UNKNOWN); /* Si on arrive ici erreur */
}

/* Création d'objet depuis l'entrée utilisateur */
/* Notes à moi même.
 * Pour la lecture des charactères, entiers et booléens on controle
 * le charactère qui suit l'entrée de leur valeur pour voir s'il est bon.
 * La parenthèse fermante est un caractère correct. Bien que #f) ne soit pas
 * un boléen il marque la fin d'une liste de plus les parenthèses sont 
 * controlées dans sfs_get_expression donc aucun pb
 */

/** @fn int OBJECT_int_update(object o, char* input, uint *here)
 * @brief Extrait un entier de l'entrée utilisateur et créé l'objet
 * correspondant.
 * @param o : L'object dans lequel mettre l'entier.
 * @param input : Entrée de l'utilisateur.
 * @param here : Curseur sur la chaine de l'utilisateur.
 *
 * On a contrôle de l'erreur ici. Si l'entier contient des caractères qui ne sont pas
 * des parenthèses ouvrantes ou fermantes de listes celui-ci est considéré comme mauvais.
 * Le type de l'object est alors mis à SFS_UNKNOWN et la fonction s'arrête en
 * retournant 1. L'entier est alors tronqué.@n
 * La fonction gère aussi l'overflow, si l'entier est trop grand (ou trop petit négatif)
 * le type est mis à SFS_NUMBER et la valeur à + ou -inf.(voir NUM_set_lint )
 * Dans ce cas affiche un WARNING et retourne 2.@n
 *
 * Voir <a href="http://manpagesfr.free.fr/man/man3/strtol.3.html " > strtol </a> pour 
 * plus d'informations.
 *
 * @return Renvoie 0, 1 si l'entier entré n'en est pas un, 2 si overflow.
 */
int OBJECT_int_update(object o, char* input, uint *here)
{
	/* here pointe sur +, - ou un digit. */
	uint here_cpy = *here ;
	char* debut = input + *here ;
	char** fin = &debut;
	long int val = strtol(debut, fin, 10);

	/* Mise à jour here */
	*here = debut - input ;
	DEBUG_MSG("*debut : %c, *here : %d, input[*here] : %c",
			*debut, *here, input[*here]);
	/*Test validité entier, petite parenthèse pour les listes.*/
	if(isspace(*debut) || *debut== '\0' || *debut == ')' 
			|| *debut =='(' || *debut == '"')
	{
		/* Mettre à jour num*/
		DEBUG_MSG("Valid format for int");
		/* Test d'overflow */
		if(val == LONG_MAX)
		{
			WARNING_MSG("Overflow error : %s is too long", input + here_cpy);
			NUM_set_lint(&(o->this.number), val, NUM_PINFTY);
			return(2);
		}
		else if(val == LONG_MIN)
		{
			WARNING_MSG("Underflow error : %s is too low",input + here_cpy);
			NUM_set_lint(&(o->this.number), val, NUM_MINFTY);
			return(2);
		}
		else
		{
			DEBUG_MSG("No overflow");
			NUM_set_lint(&(o->this.number), val, NUM_INTEGER);
			return(0);
		}
	}
	else
	{
		WARNING_MSG("%s is not a int", input + here_cpy);
		o->type = SFS_UNKNOWN;
		return(1);
	}
	
}

/** @fn int OBJECT_character_update(object o, char* input, uint* here)
 * @brief Extrait le caractère de input et l'enregistre dans object.
 * Place here sur caractère suivant.
 * @param o : l'objet dans lequel on enregistre.
 * @param input : la chaine contenant le caractère.
 * @param here : Le curseur sur la chaine contenant le caractère
 *
 * @return Renvoie 0 si tout de passe bien 1 sinon.
 */
int OBJECT_character_update(object o, char* input, uint* here)
{
	/*Si cette fonction est appelée c'est qu'on a bien un #\ donc on passe directement au carac
	 * avec here qui pointe #. 
	 * On doit traiter le cas de spaceline ect.....
	 * Utiliser strstr() de string.h
	 */
	int l_space = strlen("#\\space");
	int l_newline = strlen("#\\newline");
	if(strncmp(input + *here, "#\\space",l_space) == 0)
		       	
	{
		/* Cas d'un espace*/
		o->this.character = ' ';
		/*Vérif d'erreur*/
		(*here)+= l_space;
		if( isealnum(input[*here]) && input[*here] != ')' && input[*here]!='(')
		{
			/* Cas d'une entrée type #spacefsfsd */
			o->type = SFS_UNKNOWN;
			return(1);
		}
		return(0);
	}
	if(strncmp(input + *here, "#\\newline", l_newline ) ==0 )
	{
		/*Cas d'un saut de ligne */
		o->this.character='\n';

		/*Vérif d'erreur*/
		(*here)+= l_newline;
		if( isealnum(input[*here]) && input[*here] != ')' && input[*here]!='(')

		{
			/* Cas d'une entrée type #sepacefsfsd */
			o->type = SFS_UNKNOWN;
			return(1);
		}

		return(0);
	}
	/* On se place sur le caractère (pour ignorer #\ car on sait déjà qu'ils y sont si on appelle
	 * cette fonction*/
	*here += 2 ; 
	if( (isealnum(input[*here +1] ) && input[*here +1] != ')' && input[*here +1]!= '(' )
	 	|| ( isspace(input[*here])  ) )
	{
		/* cas du type #\descarac ==> Erreur */
		WARNING_MSG("%s is not a char", input + *here- 2 );
		o->type = SFS_UNKNOWN;
		return(1);
	}
	/* Sinon on a juste un caractère gentil */
	o->this.character = input[*here];
	*here += 1;
	DEBUG_MSG("After char update : cursor %d, input %s", *here, input + *here);
	return(0);

}

/** @fn int OBJECT_string_update(object o, char* input, uint* here)
 * @brief Extrait la chaine de input et l'enregistre dans object.
 * @param o : l'objet dans lequel on enregistre.
 * @param input : la chaine contenant la chaine.
 * @param here : Le curseur sur le premier caractère utile de input.
 *
 * @return Renvoie 0 si tout de passe bien 1 sinon.
 */
int OBJECT_string_update(object o, char* input, uint* here)
{
	/* here pointe sur " on doit donc copier tous les caractères jusqu'au prochain "
	 * Si on trouve plusieurs " on renvoie le code d'erreur.
	 */

	*here += 1; /* On se place sur le caractère qui suit "*/
	int i;
	
	for(i=0; input[*here]!='\"' && *here < STRLEN  ; i++ , (*here)++)
	{
		if(input[*here]=='\\' && input[*here +1] == '"')
		{
			/* Cas de protection d'une guillemet*/
			o->this.string[i]='\"';
			(*here)++;	
		}
		else if (input[*here]=='\\' && input[*here+1] == '\\')
		{
			/* Cas ou on protège le backslash*/
			o->this.string[i]= '\\';
			(*here)++;
		}
		else o->this.string[i]= input[*here];
	}
			
	(*here)++ ; /* Saut de la guillemet fermante */
	DEBUG_MSG("After string update : cursor %d, input %s", *here, input + *here);
	return(0);
}

/** @fn int OBJECT_boolean_update(object* o, char* input, uint* here)
 * @brief Extrait le booléen de l'input. Déplace here au caractère suivant.
 * @param o : l'objet dans lequel on enregistre.
 * @param input : la chaine contenant le booléen.
 * @param here : Le curseur sur le premier caractère utile de input.
 *
 * La fonction vérifie que le booléen entré en est bien un (#t ou #f) autrement 
 * elle le considère comme mal entré. Dans ce cas elle appelle WARNING_MSG et 
 * retourne 1.
 * Elle place ensuite o à vrai ou à faux qui sont des variables globales
 * de repl.c (pas implémenté de cette façon encore).
 *
 * @return Renvoie 0 si tout de passe bien, 1 sinon.
 */
int OBJECT_boolean_update(object* o, char* input, uint* here)
{
	/* Si cette fonction est appelée here pointe sur caractère # */
	*here += 1; 
	int error = FALSE ;
	if(input[*here]== 't' && (isspace(input[*here +1]) || input[*here +1] == '\0' ||
				input[*here +1] == ')' || input[*here +1]=='('))
	{
		/*o->this.special = vrai;*/
		(*o) = vrai;
	}
	else if (input[*here] == 'f' && (isspace(input[*here +1]) || input[*here +1] == '\0' ||
				input[*here +1] == ')' || input[*here +1]=='('))
	{
		/*o->this.special = faux;*/
		(*o) = faux;
	}
	else /* Cas ou on a une mauvaise entrée */
	{
		/*o->type = SFS_UNKNOWN ;*/
		WARNING_MSG("%s is not a boolean", input + *here -1 );
		error = TRUE;
	}
	(*here)++; /* Curseur passe à la suite */
	DEBUG_MSG("After bool update : cursor %d, input %s", *here, input + *here);
	if(!error) return(0);
	return(1);
}

/** @fn int OBJECT_symbol_update(object o, char* input, uint *here)
 * @brief Extrait un symbole de la chaine input et met à jour l'object.
 * @param o : l'object de type symbole à éditer
 * @param input : La chaine entrée par l'utilisateur.
 * @param here: le curseur sur la chaine.
 *
 * Cette fonction vérifie que chaque caractère du symbole est bien un supporté par scheme
 * selon la R5RS voir isealum pour plus de détails.
 * Dans le cas d'une erreur passe le type à SFS_UNKNOWN .
 * Dans ce cas elle renvoie 1.
 * @return : Renvoie O si Aucun problème n'est détecté, 1 sinon.
 */
int OBJECT_symbol_update(object o, char* input, uint*here)
{
	/* A ce stade on est sur le premier caractère du symbole un symbole ne contient
	 * que des caractères alphanumérique et 
	 * d'autres caractères spéciaux voir isealnum on s'en sert pour la détection d'erreur. */
	int i,retour = 0, cpy= *here; /* Une copie pour signaler les erreurs */
	char readed_character = input[*here];

	for(i =0 ;input[*here]!='\0' && !isspace(input[*here])
			&& input[*here]!= '(' && input[*here]!=')';
		       	(*here )+= 1, i++)
	{
		if(!isealnum(readed_character))
		{
			WARNING_MSG("%s is not a symbol (Symbol cannot contain %c)", input+cpy, readed_character);
			o->type= SFS_UNKNOWN; /* Mauvais symbole ==> Aucun type (ou pas recconu)*/
			retour= 1;
		}
		readed_character = input[*here];
		o->this.symbol[i]= readed_character;
	}
	DEBUG_MSG("After symbol update : cursor %d, input %s", *here, input + *here);
	return(retour);
}


