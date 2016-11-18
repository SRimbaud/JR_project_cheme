/* Complet pour livrable 1. */
/* Livrable 2 :
 * Pb avec les booléens, je crée un objet dont special
 * pointe vrai ou faux. Je devrais directement avoir un
 * object qui pointe vrai ou faux.
 * On verra pour changer ça je sais pas quel impact ça aura. 
 * Je me demande si c'est pareil pour nil.
 * En effet on perd beaucoup de mémoire du coup.
 */
/**
 * @file object.c
 * @author François Cayre <cayre@yiking.(null)> and Rimbaud Sébastien <sebastien.rimbaud@phelma.grenoble-inp.fr>
 * @date Sun September 2016 16:35
 * @brief Fonctions relatives à la structure object.
 *
 * Contient toutes les fonctions permettant de manipuler (affichage, édition) de la structure object.
 * Contient également les fonctions de création de NIL, TRUE et FALSE.
 */
#include "object.h"

/** @fn make_object ( uint type )
 * @brief Alloue un objet de type type, vérifie l'alloc.
 * @param type : type de l'objet (num, char, string , string (symbol), PAIR, special (nil)
 * 
 * @return Renvoie un pointeur sur l'objet créé.
 */
object make_object( uint type ) {

    object t = sfs_malloc( sizeof( *t ) );
	check_alloc(t, "make_object");
    t->type = type;

    return t;
}

/** @fn make_nil ( void )
 * @brief Alloue l'objet nil qui marque fin de o. Programme se termine si l'alloc rate.
 *
 * Le programme se termine si la création de l'objet nil rate.
 * En effet cet objet étant utilisé pour toutes @n
 * les listes scheme, un échec de sa création empêchera
 * inévitablement le bon fonctionnement de l'interpréteur.
 * 
 * @return Renvoie un pointeur sur l'objet créé.
 */

object make_nil( void )
{

    object t = make_object( SFS_NIL );

   if (t == NULL)
   {
	   ERROR_MSG("Erreur allocation nil");
	   exit(EXIT_FAILURE);
   }
    t->this.special = t;

    return t;
}

/**@fn object make_true(void)
 * @brief Créé le booléen vrai (variable globale). Fonction appelée dans init_interpreter().
 *
 * Cette fonction quitte l'interpréteur si l'allocation échoue.
 * Inutile de continuer si le booléen
 * n'existe pas dans l'interpréteur.
 * L'objet vrai est de type SFS_BOOLEAN et possède son 
 * this.number mis à 1.
 */
object make_true(void)
{
	object vrai = make_object(SFS_BOOLEAN);
	if(vrai == NULL)
	{
		ERROR_MSG("Fail while creating true object");
	}
	NUM_set_int(&(vrai->this.number), 1); /* Mise à vrai du booléen */
	return(vrai);
}

/**@fn object make_false(void)
 * @brief Créé le booléen faux (variable globale). Fonction appelée dans init_interpreter().
 *
 * Cette fonction quitte l'interpréteur si l'allocation échoue.
 * Inutile de continuer si le booléen
 * n'existe pas dans l'interpréteur.
 * L'objet vrai est de type SFS_BOOLEAN et possède son 
 * this.number mis à 0.
 */
object make_false(void)
{
	object faux = make_object(SFS_BOOLEAN);
	if(faux == NULL)
	{
		ERROR_MSG("Fail while creating true object");
	}
	NUM_set_int(&(faux->this.number), 0); /* Mise à faux du booléen */
	return(faux);
}

/** @fn void make_forms()
 * @brief Initialise formes de l'interpréteur.
 * @sa FORMS;
 */
void make_forms()
{
	form[QUOTE]= OBJECT_build_symbol("quote");
	form[DEFINE]= OBJECT_build_symbol("define");
	form[SET]= OBJECT_build_symbol("set!");
	form[AND]= OBJECT_build_symbol("and");
	form[IF]= OBJECT_build_symbol("if");
	form[OR]= OBJECT_build_symbol("or");
	DEBUG_MSG(" init : %s", form[QUOTE]->this.symbol);
}
/*Accesseurs */

/* Get */

/** @fn void OBJECT_set_cxr(object o, object val, char* place)
 * @brief Modifie l'emplacement donné et y met la variable val.
 *
 * Cette fonction libère l'ancienne valeur.
 * L'objet à ajouter doit-être préalablement alloué.
 * Ne fait rien si o est a NULL.
 * Ajoute nil si val est a NULL avec un appel récursif.
 *
 * @sa OBJECT_destroy
 * @sa OBJECT_get_cxr
 *
 * @return
 */
void OBJECT_set_cxr(object o, object val, char* place)
{
	/* Pour pouvoir effectuer le changement proprement
	 * on doit s'arrêter un bloc avant le bloc a changer
	 * dans la liste. Cela implique de modifier la chaine
	 * place pour y supprimer la première lettre qui est un
	 * a ou un d
	 */
	if(OBJECT_isempty(o))
	{
		WARNING_MSG("Cannot modify NULL object");
		return;
	}
	if(OBJECT_isempty(val))
	{
		OBJECT_set_cxr(o, nil, place);
	}

	char position = place[1];
	char* cpy = place;
	place[1] = 'c';
	cpy ++;
	/* Ainsi "caadr" devient 'a' et "cadr" */

	object before_changed = OBJECT_get_cxr(o, cpy);
	if(position == 'a')
	{
		OBJECT_destroy(&(before_changed->this.pair.car));
		OBJECT_set_car(before_changed, val);
	}
	else if(position== 'c')
	{

		OBJECT_destroy(&(before_changed->this.pair.cdr));
		OBJECT_set_cdr(before_changed, val);
	}
	else 
	{
		WARNING_MSG("%c unrecognized position in OBJECT_set_cxr", position);
	}

}

/*set*/

/** @fn  void OBJECT_set_cdr(object o, object cdr)
 * @brief Met à jour le cdr de o avec l'object cdr.
 *
 * Si cdr est a NULL place la variable globale nil.
 *
 */
void OBJECT_set_cdr(object o, object cdr)
{
	if(cdr ==NULL)
	{
		o->this.pair.cdr = nil;
		return;
	}
	o->this.pair.cdr = cdr;
}


/** @fn  void OBJECT_set_car(object o, object car)
 * @brief Met à jour le car de o avec l'objet car.
 * 
 * Si car est NULL place la variable globale nil.
 *
 */
void OBJECT_set_car(object o, object car)
{
	if(car ==NULL)
	{
		o->this.pair.car = nil;
		return;
	}
	o->this.pair.car = car;
}
/* Constructeurs */
/** @fn  object OBJECT_build_pair(object car, object cdr)
 * @brief Construit une paire avec un car et cdr donné.
 *
 * Construit une liste avec un car et cdr donné.
 * Si car ou cdr == NULL alors il seront mis à nil.
 *
 * @sa OBJECT_set_cdr(object o, object car)
 * @sa OBJECT_set_car(object o, object car)
 *
 * @return Renvoie un object avec car et le cdr donné.
 */
object OBJECT_build_pair(object car, object cdr)
{
	object pair = make_object(SFS_PAIR);

	check_alloc(pair, "OBJECT_build_pair");

	if(car ==NULL)
	{
		OBJECT_set_car(pair, nil);
	}
	else
	{
			
		OBJECT_set_car(pair, car);
	}
	if(cdr ==NULL)
	{
		OBJECT_set_cdr(pair, nil);
	}
	else
	{

		OBJECT_set_cdr(pair, cdr);
	}
	return(pair);
}


/** @fn  object OBJECT_build_symbol(char* symbol)
 * @brief Construit un object de type symbole contenant symbole.
 *
 * Doit contenir 256 caractère définit pas string et STRLEN.
 *
 * @return
 */
object OBJECT_build_symbol(char* symbol)
{
	object symb = make_object(SFS_SYMBOL);
	strncpy(symb->this.symbol, symbol, STRLEN);
	return(symb);
}

/** @fn  object OBJECT_build_cpy(object o)
 * @brief Copie l'objet o en en allouant un nouveau.
 *
 * Si l'objet est nil ou un booléen on 
 * ne refait pas d'alloc on renvoie la 
 * variable globale correspondante.
 * Si l'objet est NULL renvoie null
 * et affiche un warning.
 *
 * @return Renvoie l'objet créé.
 */
object OBJECT_build_cpy(object o)
{
	if(!check(o, "Try to copy NULL ptr")) return NULL;
	if(check_type(o, SFS_NIL)) return nil;
	if(check_type(o, SFS_BOOLEAN)) return o;
	if(check_type(o, SFS_PAIR))
	{
		DEBUG_MSG("Copying pair");
		object cpy = make_object(SFS_PAIR);
		check_alloc(cpy, "Fail in cpy");
		cpy->this.pair.car = OBJECT_build_cpy(o->this.pair.car);
		cpy->this.pair.cdr = OBJECT_build_cpy(o->this.pair.cdr);
		return (cpy);
	}
	if(check_type(o, SFS_ENV))
	{
		INFO_MSG("Copying environment");
		/* On va recopier le cdr de l'environnement
		 * car il contient les variables
		 * on recopie pas son père
		 */
		object cpy = make_object(SFS_ENV);
		check_alloc(cpy, "Fail in cpy");
		/* Récursivité sur les variables */
		cpy->this.pair.cdr = OBJECT_build_cpy(o->this.pair.cdr);
		return(cpy);
	}
	else
	{
		object cpy = make_object(o->type);
		check_alloc(cpy, "Fail in cpy");
		*cpy = *o ;
		return(cpy);
	}
}




/* Destructeurs */


/** @fn void OBJECT_destroy(object* o)
 * @brief Supprime un objet.
 *
 * Supprime tous les éléments d'un objet si c'est une paire,
 * de même pour un environnement.
 * @n
 * Fonction a modifier si on alloue dynamiquement les string et 
 * symboles. Pour l'instant c'est un free classique.
 * Voir le pb sur les booléens aussi.
 */
void OBJECT_destroy(object* o)
{
	if(OBJECT_isempty(*o)) return;
	/* Les seuls objet un peu retord sont les environnements et
	 * les pairs.
	 * On a aussi les strings et symboles qui
	 * s'ils sont alloués dynamiquement doivent être libéré,
	 * pour le reste c'est un free classique.
	 * ON NE DETROY PAS TRUE, FALSE, NIL !!!!
	 */
	switch((*o)->type)
	{
		case SFS_PAIR :
			OBJECT_destroy_pair(o);
			break;
		case SFS_ENV :
			/* A compléter */
			break;
		case SFS_SYMBOL :
			free(*o); /* Car pas d'alloc pour l'instant */
			break;
		case SFS_STRING :
			free(*o);
			break; /* Cas pas d'alloc pour l'instant */
		case SFS_NIL :
			break;
		case SFS_BOOLEAN :
			/* A cause de notre implémentation
			 * foireuse on doit faire un free
			 * A modifier si correctif
			 */
			/*free(*o);*/
			break;
		default :
			free(*o);
	}
	
	
}
/** @fn void OBJECT_destroy_pair(object* o)
 * @brief Supprime une paire. Fonction récursive.
 * 
 * Pas de controle sur o pour savoir si c'est bien
 * une paire.
 * Fonction appelée par OBJECT_destroy.
 * N'utiliser que OBJECT_destroy pas celle-ci,
 * elle n'est utile qu'à l'implémentation du
 * destructeur général.
 */
void OBJECT_destroy_pair(object* o)
{
	OBJECT_destroy(&((*o)->this.pair.car));
	OBJECT_destroy(&((*o)->this.pair.cdr));
}

/*Autres */
/** @fn  object OBJECT_built_short_quote(char* input, uint* here)
 * @brief Construit le début de pair pour quote sous forme '
 *
 * Lorsque que l'on a '(objects) cette fonction construit
 * l'object équivalent à (quote (object))
 * Elle appelle sfs_read sur le reste de la liste.
 *
 *
 * @return Renvoie une forme quote.
 */
object OBJECT_built_short_quote(char* input, uint* here)
{
	/* Ici input[*here] == '*/
	DEBUG_MSG("Call OBJECT_build_short_quote");
	(*here)++;
	object quote = OBJECT_build_pair(OBJECT_build_symbol("quote"),
			make_object(SFS_PAIR));
	/* On a quote avec un cdr qui contient une paire*/
	object quote_cdr = quote->this.pair.cdr ;
	/*Le car de cette paire contient le reste de l'entrée */
	OBJECT_set_car(quote_cdr, sfs_read(input, here));
	/* Le cdr de cette paire contient nil*/
	OBJECT_set_cdr(quote_cdr, NULL);

	return(quote);

}

/** @fn  int OBJECT_isempty(object o)
 * @brief Vérifie si un objet est vide (égal à NULL)
 *
 * @return Vrai si vide, faux sinon.
 */
int OBJECT_isempty(object o)
{
	if(o==NULL) return(TRUE);
	return (FALSE);
}


/** @fn object OBJECT_get_cxr(object o, char* place)
 * @brief Renvoie de l'élement de la pair correspondant à la chaine.
 *
 * Renvoie l'élément car si la chaine "car" est donnée.
 * Renvoie l'élément cdr si la chaine "cdr" est donnée.
 * Renvoie le car du cdr si la chaine "cadr" est donnée.
 * Lis la chaine à l'envers et prend le cdr si d est donné ou
 * le car si a est donnée.
 * Le format d'entré doit contenir en première 
 * lettre un c en dernière un r.
 * En pratique si d'autres lettres sont placées la fonction ne
 * fera pas la différence car elle les ignore.
 * Au milieu on aura que des a et des d.
 * Si on trouve une autre lettre affiche un warning et
 * retourne NULL.
 * Si on spécifie une location inexistante dans la paire la fonction
 * affiche un warning et renvoie NULL.
 * Si o n'est pas une paire, revoie NULL.
 *
 * @return Renvoie l'objet lu correspondant à la chaine.
 */
object OBJECT_get_cxr(object o, char* place)
{
	if(!check_type(o, SFS_PAIR) && !check_type(o, SFS_ENV))
	{
		WARNING_MSG("Try to use OBJECT_get_cxr on type which is	not SFS_PAIR");
		return(NULL);
	}
	int i ;
	object resultat = o;
	/* On se fiche du c et r de la chaine */
	/* On a retiré le c  */

	for(i = strlen(place) - 2; i != 0; i--)
	{
		if(OBJECT_isempty(resultat))
		{
			WARNING_MSG("Try to access an unreachable point in OBJECT_get_cxr");
			return(resultat);
		}

		if(place[i] == 'a')
		{
			resultat=resultat->this.pair.car;
		}
		else if (place[i] == 'd')
		{
			resultat=resultat->this.pair.cdr;
		}
		else 
		{
			WARNING_MSG("Unrecognized char : %c", place[i]);
			return(NULL);
		}
	}
	return(resultat);
}

/** @fn void OBJECT_rewrite(object changed, object nouveau)
 * @brief Réécris avec la valeur de nouveau.
 *
 * Attention si changed est de type SFS_NIL, un
 * nouvel objet est renvoyé.
 * @bug Incomplète
 *
 */
void OBJECT_rewrite(object changed, object nouveau)
{
	/* Traitement particulier pour nil*/
	if(check_type(changed, SFS_NIL))
	{
		object creation = make_object(nouveau->type);
		*creation = *nouveau;
	/*	return(creation); */
	}
}
/*Comparaisons */

/** @fn int OBJECT_isEqual(object a, object b)
 * @brief Compare deux object Renvoie 1 s'ils sont égaux 0 sinon.
 *
 * Deux objets sont considérés comme égaux s'ils ont le même type
 * et la même valeur.
 * COMPARAISON DE PAIRE NON IMPLEMENTEE.
 *
 * @return 1 si a==b, 0 sinon.
 */
int OBJECT_isEqual(object a, object b)
{
	if(!check_type(a, b->type)) return(0);
	/* Cas ou ils ont même type */
	switch(a->type)
	{
		case SFS_NIL :
			return(1);
			break;
		case SFS_NUMBER :
			return(OBJECT_cmp_number(a, b));
			break;
		case SFS_SYMBOL :
			if(string_cmp(a->this.symbol, b->this.symbol))
				return(1);
			break;
		case SFS_BOOLEAN :
			if(OBJECT_cmp_bool(a,b)) return(1);
			break;
		case SFS_PAIR :
			/* Récusivité
			 * Il faut que les car et cdr de la paire
			 * soient égaux. */
			return(OBJECT_isEqual(a->this.pair.car, b->this.pair.car) &&
					OBJECT_isEqual(a->this.pair.cdr, b->this.pair.cdr));
			break;
		case SFS_STRING :
			if(string_cmp(a->this.string, b->this.string))
				return(1);
			break;
		case SFS_CHARACTER :
			if(a->this.character == b->this.character) return(1);
			break;
		default :
			return(0);
	}
	return(0); /* Fait office de cas else pour chaque if*/
}

/** @fn int OBJECT_cmp_bool(object a, object b)
 * @brief Compare 2 booléens. Renvoie 0 s'ils ne sont pas booléens.
 *
 * @return 1 si les 2 booléens sont égaux, 0 sinon.
 */
int OBJECT_cmp_bool(object a, object b)
{
	if(!check_type(a, SFS_BOOLEAN)) return(0);
	if(!check_type(b, SFS_BOOLEAN)) return(0);
	if(a== b)
		return(1);
	return(0);
	/* On pourrait essayer d'améliorer la comparaison.
	 * En vrai on devrait passer par un == mais j'ai des doutes sur 
	 * la validité de mon implémentation des booléens et du nil.
	 * Je garde donc cette implémentation pour l'instant qui est robuste.
	 */
}

/** @fn int OBJECT_cmp_number(object a, object b)
 * @brief Compare deux objets de type entier.
 *
 * @return Renvoie 1 si a et b contiennent les mêmes entiers. 0 sinon.
 */
int OBJECT_cmp_number(object a, object b)
{

	if(!check_type(a, SFS_NUMBER)) return(0);
	if(!check_type(b, SFS_NUMBER)) return(0);
	return(NUM_cmp(a->this.number, b->this.number));
	
}

/** @fn int OBJECT_cmp_symb(object a, char* symbol)
 * @brief Compare a avec le symbol symbol.
 *
 * @return Renvoie 1 si égaux, O sinon.
 */
int OBJECT_cmp_symb(object a, char* symbol)
{
	if(!check_type(a, SFS_SYMBOL)) return(0);
	
	if(strcmp(a->this.symbol, symbol) == 0) return 1 ;
	return(0);
}
	
/* Opérateurs arithmétiques */


/** @fn object OBJECT_add(const object a,const object b, object result)
 * @brief Effectue la somme de 2 objets, écrit résultat dans résult.
 *
 * La somme est faite sur des objects de type
 * SFS_NUM.
 * Le résultat est écrit dans result et result est retourné.
 * Il faut toujours prendre la valeur retournée par la fonction,
 * result permet d'éviter un trop grand nombre d'allocation
 * dans PRIM_somme.
 * Retourne nil si la somme est irréalisable
 * (Utile pour les affectations).
 *
 * @sa NUM_sum(num a, num b)
 *
 * @return Retourne le résultat de la somme ou nil si erreur.
 */
object OBJECT_add(const object a, const object b, object result)
{
	if(check_type(a, SFS_NIL) || check_type(b, SFS_NIL)) 
	{
		DEBUG_MSG("Sum on nil, nil returned");
		return(nil);
	}
	if(!check_type(a, SFS_NUMBER) || !check_type(b, SFS_NUMBER))
	{
		WARNING_MSG("Sum on not Number type");
		return (nil);
	}
	int flag = 0;
	result->type = SFS_NUMBER;
	result->this.number = NUM_sum(a->this.number, b->this.number, &flag);
	if(flag) 
	{
		WARNING_MSG(" '+' impossible to calculate result");
		return(nil);
	}
	return(result);
}



/** @fn object OBJECT_sub(const object a, const object b, object result)
 * @brief Soustraction de 2 object. Le résultat est mis dans résult.
 *
 * Fonctionnement similaire à OBJECT_add.
 * @sa OBJECT_add
 *
 * @return Renvoie result mis à jour si opération possible, nil sinon.
 */
object OBJECT_sub(const object a, const object b, object result)
{
	if(check_type(a, SFS_NIL) || check_type(b, SFS_NIL)) 
	{
		DEBUG_MSG("Sub on nil, nil returned");
		return(nil);
	}
	if(!check_type(a, SFS_NUMBER) || !check_type(b, SFS_NUMBER))
	{
		WARNING_MSG("Sub on not Number type");
		return (nil);
	}
	int flag = 0;
	result->type = SFS_NUMBER;
	result->this.number = NUM_sub(a->this.number, b->this.number, &flag);
	if(flag) 
	{
		WARNING_MSG(" '-' impossible to calculate result");
		return(nil);
	}
	return(result);
}

/** @fn object OBJECT_mul(const object a, const object b, object result)
 * @brief Calcul le produit de 2 objects a par b.
 *
 * A priori, le produit ne pose pas d'impossibilité de calcul, que
 * ce soit un nombre ou un infini on peut toujours calculer le produit
 * et trouver un résultat.
 *
 * @sa OBJECT_sum(const object a, const object b, object result)
 *
 * @return Renvoie le result mis à jour si produit possible, nil sinon.
 */
object OBJECT_mul(const object a, const object b, object result)
{
	if(check_type(a, SFS_NIL) || check_type(b, SFS_NIL)) 
	{
		DEBUG_MSG("Product on nil, nil returned");
		return(nil);
	}
	if(!check_type(a, SFS_NUMBER) || !check_type(b, SFS_NUMBER))
	{
		WARNING_MSG("Product on not Number type");
		return (nil);
	}
	int flag = 0;
	result->type = SFS_NUMBER;
	result->this.number = NUM_mul(a->this.number, b->this.number, &flag);
	if(flag) 
	{
		WARNING_MSG(" '*' impossible to calculate result");
		return(nil);
	}
	return(result);
}




/** @fn  void check_alloc(void* ptr, char* message)
 * @brief Vérifie une allocation dynamique.
 *
 * Quitte le programme avec un ERROR_MSG si une alloc
 * rate.
 */
void check_alloc(void* ptr, char* message)
{
	if(ptr ==NULL)
	{
		ERROR_MSG("Alloc error : %s", message);
	}
}


/** @fn int check_type(object o, int sfs_type)
 * @brief Vérifie que o est bien de type sfs_type.
 *
 * @return Renvoie 1 si o est de type sfs_type, 0 sinon.
 * Renvoie 0 si o de type SFS_UNKNOW quelque soit
 * sfs_type.
 */
int check_type(object o, int sfs_type)
{
	if(sfs_type == SFS_UNKNOWN) return(0);
	if(OBJECT_isempty(o)) return(0);
	if(o->type== sfs_type) return(1);
	return(0);
}


/** @fn int string_cmp(char* a, char* b)
 * @brief Compare chaine a et b. Renvoie 1 si égale 0 sinon.
 *
 * Si les chaines n'ont pas la même longueur renvoie 0.
 * Compare les chaines avec strncmp.
 *
 * @return 1 si chaine égales, 0 sinon.
 */
int string_cmp(char* a, char* b)
{
	int l = strlen(a);
	if(l != strlen(b)) return(0);
	if(!strncmp(a, b, l)) return(1);
	return(0);
}

/** @fn int is_form(object symbol, FORMS* forme)
 * @brief Renvoie 1 si symbol est un mot clef du langage scheme.
 * @param symbol : Le symbole dont on veut savoir si c'est une forme.
 * @param forme : Pointeur qui est met à jour pour connaitre
 * la forme dans la variable globale form[].
 *
 * Vérifie si symbol est un mot clef du langage.
 * Met à jour forme sur le bon numéro de la forme.
 * Renvoie 1 si c'est le cas 0 sinon.
 * Peut recevoir forme égale à NULL.
 *
 * @sa make_forms()
 * @sa NB_FORMS
 *
 * @return Renvoie 1 si c'est une forme, 0 sinon.
 */
int is_form(object symbol, FORMS* forme)
{
	int i = 0;
	for(i =0; i<NB_FORM; i++)
	{

		if(OBJECT_cmp_symb(symbol, form[i]->this.symbol))
		{
			
			if(forme) *forme = i;
			return(1);
		}
	}
	if(forme) *forme = NONE;
	return(0);
}



/** @fn int check(void* ptr, char* message)
 * @brief Si ptr est NULL affiche message dans un warning.
 *
 * @return Renvoie 0 si ptr==NULL
 */
int check(void* ptr, char* message)
{
	if(ptr) return(1);
	WARNING_MSG("%s", message);
	return(0);
}
/* Utilitaires */
/** @fn int isealnum(char c)
 * @brief Vérifie si c est un caractère compatible pour une expression scheme (voir R5RS)
 * 
 * Les caractères compatibles sont [a-z] [A-Z] [0-9] et ! $ * = + - . / : < > ? @ & % : ^ _ ~.
 *
 * @sa <a href= "https://linux.die.net/man/3/isalnum "> isalnum </a>
 * @return Renvoie 1 si c est compatible 0 sinon.
 */
int isealnum(char c)
{
	if (isalnum(c)) return(1); 
	/* Si alphanum on s'arrête tout de suite.*/
	if('!' ==c) return(1);
	if('$' ==c) return(1);
	if('*' ==c) return(1);
	if('=' ==c) return(1);
	if('+' ==c) return(1);
	if('-' ==c) return(1);
	if('.' ==c) return(1);
	if('/' ==c) return(1);
	if(':' ==c) return(1);
	if('<' ==c) return(1);
	if('>' ==c) return(1);
	if('?' ==c) return(1);
	if('@' ==c) return(1);
	if('&' ==c) return(1);
	if('%' ==c) return(1);
	if('^' ==c) return(1);
	if('_' ==c) return(1);
	if('~' ==c) return(1);
	/* Si aucun des précédents le caractère n'est pas supporté dans scheme*/
	return(0);
}

/** @fn int wait_first_non_blank_char(char* s1, uint* s1_cursor)
 * @brief incrémente le curseur tant qu'il y indique un espace dans la chaine
 * @param s1 : Chaine dans laquelle on veut sauter les espaces.
 * @param s1_cursor : Curseur dans la chaine.
 *
 * @sa <a href= "https://linux.die.net/man/3/isspace"> isspace </a>
 * @return : Renvoie le premier indice qui n'est pas un espace.
 */
int wait_first_non_blank_char(char* s1, uint* s1_cursor)
{
	while(isspace(s1[*s1_cursor])) *s1_cursor += 1;
	return(*s1_cursor);
	/* A priori aucun risque de boucle infinie une chaine se termine tjrs par \0*/
}

/* Environnements */

/** @fn object ENV_build(object father)
 * @brief Construit un environnement dont le père est father.
 *
 * L'environnement construit possède comme parent father.
 * Si father est mis à nul, construit un top-level.
 *
 * @return
 */
object ENV_build(object father)
{
	object n_env = make_object(SFS_ENV);
	if(father!=NULL) OBJECT_set_car(n_env, father);
	return(n_env);
}


/** @fn object ENV_get_var_in_env(object var, object environ, int* flag)
 * @brief Chercher la variable de nom var dans l'environnement environ.
 * @param var : Nom de la variable que l'on cherche.
 * @param environ : Environnement dans lequel on cherche
 * @param flag : Mis à vrai si on a trouvé.
 * Retourne la première occurrence de le variable dans l'environnement
 * courant.
 * Renvoie une pair dont le car porte le nom de la variable et le
 * cdr porte sa valeur.
 * Si jamais il ne la trouve pas il passe le flag à false.
 * S'il trouve il le met à TRUE.
 *
 * @return Renvoie une paire dont le car porte le nom de la variable
 * (symbole) et le cdr sa valeur.
 */
object ENV_get_var_in_env(object var, object environ, int* flag)
{
	DEBUG_MSG("call ENV_get_var_in_env");
	object i = OBJECT_get_cxr(environ, "cdr");
	/*Parcours des variables */
	object tmp_bloc= i; /* Le bloc, (nom, valeur) */
	object tmp_name= i; /* Le nom */

	while(!OBJECT_isempty(i) && !check_type(i, SFS_NIL))
	{
		tmp_bloc = OBJECT_get_cxr(i,"car");
		tmp_name = OBJECT_get_cxr(tmp_bloc, "car");
		/*Accès au nom de la variable*/
		if(OBJECT_isEqual(var,tmp_name)) 
		{
			DEBUG_MSG("Find var in environment : name %s\n type : %d int value : %ld",
					tmp_name->this.symbol, tmp_bloc->this.pair.cdr->type,
					tmp_bloc->this.pair.cdr->this.number.this.integer);
			if(flag) *flag = TRUE;
			return(tmp_bloc);
			/*Renvoie le pointeur qui correspond à 
			 * la variable trouvée 
			 */
		}
		i = OBJECT_get_cxr(i, "cdr");
	}
	/* Si on est là c'est qu'on a rien trouvé */
	if(flag) *flag = FALSE;
	return(tmp_bloc);

}

/** @fn object ENV_get_var(object var, int* flag)
 * @brief Cherche le symbole var dans les environnements.
 * @param var : Symbole à chercher.
 * @parem flat : Drapeau pour savoir s'il est trouvé. Mis à 1 si trouvé.
 *
 * Cherche la variable var dans les environnements.
 * Si var n'est pas un symbole cette fonction affiche un warning
 * et renvoie var.
 * Retourne la première occurrence de le variable dans l'environnement
 * courant.
 * Si elle n'est pas trouvée il cherche dans les pères jusqu'à 
 * trouver la première occurrence de la variable.
 * Si jamais il ne la trouve pas il renvoie NULL.
 * S'il trouve le symbole le flag est mis à 1.
 *
 * @sa ENV_get_var_in_env
 *
 * @return Renvoie une paire dont le car porte le nom de la variable
 * et le cdr sa valeur. 
 */
object ENV_get_var(object var, int* flag)
{
	DEBUG_MSG("Call ENV_get_var");
	if(check_type(var, SFS_ENV))
	{
		WARNING_MSG("Try to get a variable of type SFS_ENV");
		return(var);
	}
	if(check_type(var, SFS_SYMBOL))
	{
		object j = env; /* Parcours des environnements*/
		object trouve = NULL;

		for(j=env; !OBJECT_isempty(j) && !(*flag) ;
				j = OBJECT_get_cxr(j, "car"))
		{
			trouve = ENV_get_var_in_env(var, env, flag);	
		}
		return(trouve);

	}
	else /* Cas ou ce ne serait pas un symbole */
	{
		WARNING_MSG("Try get a variable which not a symbol");
		return(var);
	}

}


/** @fn object ENV_add_var(object name, object value)
 * @brief Ajoute la variable de nom name et de valeur value
 * l'environnement courant.
 * @param name : Nom de la variable (symbole)
 * @param value : valeur de la variable.
 *
 * Ajoute la variable de nom name et de valeur value
 * à l'environnement courant. Si jamais elle existe déjà
 * la variable est écrasée et reçoit une nouvelle valeur.
 * Renvoie name et affiche un warning si name n'est pas un symbole.
 * Renvoie value et affiche un warning si value est de type 
 * SFS_ENV.
 * Dans les 2 précédents cas quitte la fonction prématurément
 * sans effectuer d'actions.
 *
 *
 * @return Renvoie la valeur de la variable crée.
 */
object ENV_add_var(object name, object value)
{
	/* Ici algo simple on ajoute la variable au début de l'environnement
	 * on ne cherche pas à le parcourir pour aller à la fin*/
	if(!check_type(name, SFS_SYMBOL))
	{
		WARNING_MSG("Trying to name a variable with non symbol type");
		return(name);
	}
	if(check_type(value, SFS_ENV))
	{
		WARNING_MSG("Trying to create a variable which is an environment");
		return(value);
	}
	DEBUG_MSG("Valid format for ENV_add_var");
	object tmp = OBJECT_get_cxr(env, "cdr");
	/* Les variables name et value sont déjà allouées normalement
	 * c'est fait dans le define */
	object bloc = OBJECT_build_pair(name, value);
	/* COnstruction de la paire qui contient à la fois 
	 * le nom et la valeur d'une variable.
	 */
	object branche = OBJECT_build_pair(bloc, tmp);
	/* Construction de l'embranchement pour ajouter le
	 * bloc à l'environnement */
	env->this.pair.cdr= branche;
	return(value);
}


/** @fn object ENV_update_var(object name, object val, int mode, int* free_flag)
 * @brief Mets à jour la variable de nom name à la valeur val.
 * @param name : Symbole à modifier.
 * @param val : Nouvelle valeur.
 * @param mode : Mode de modification.
 * @param  free_flag : Flag mis à vrai si on a effectué une suppression.
 *
 * Si name n'est pas un symbole affiche un warning et retourne nul.
 * Si val est de type SFS_ENV affiche un warning et retourne nul.
 * Dans les 2 cas il n'y pas d'ajout de variable.
 *
 * Le flag free_flag est mis à vrai si la fonction appelle 
 * OBJECT_destroy. Dans ce cas la valeur associé au symbole
 * name est supprimée. Si elle est associée à un autre
 * symbole celui-ci perd également sa valeur, ainsi
 * le drapeau le signal.
 * De ce fait il faut toujours donner une copie de la valeur
 * à ajouter et non la valeur elle même ainsi un symbole
 * n'est associé qu'à une seule structure.
 *
 * La valeur initiale du drapeau importe peu, il est mis à 
 * faux automatiquement au début de la fonction.
 *
 * @sa ENV_DISABLE_CREATION
 * @sa ENV_ENABLE_CREATION
 * @return Renvoie name.
 */
object ENV_update_var(object name,const object val, int mode, int* free_flag)
{
	/* Avant tout chose on copie le contenu de val et
	 * on travaillera avec */
	object val_cpy = OBJECT_build_cpy(val);
	if(free_flag) *free_flag = 0;
	DEBUG_MSG("val_cpy->type %d", val_cpy->type);
	if(!check_type(name, SFS_SYMBOL))
	{
		WARNING_MSG("Trying to name a variable with non symbol type");
		return(name);
	}
	if(check_type(val_cpy, SFS_ENV))
	{
		WARNING_MSG("Trying to update a variable to a environment one");
		return(val_cpy);
	}
	int flag = FALSE;
	object variable= ENV_get_var(name, &flag);
	if(!flag) /* Cas ou la variable n'existe pas */
	{

		if(mode == ENV_ENABLE_CREATION)
		{
			return(ENV_add_var(name, val_cpy));
		}
		else if(mode == ENV_DISABLE_CREATION)
		{
			return(NULL);
		}
		else
			return(NULL); /* Cas pour éviter bug*/
	}
	else
	{
		/* Cas ou la variable existe*/
		/* On supprime la VALEUR de l'ancienne variable */
		DEBUG_MSG("val_cpy->type : %d", val_cpy->type);
		/* Ici on a un problème sur le free qu'on
		 * cherche à résoudre.
		 * Surement un problème de copie.
		 * On copie des pointeurs et non des structures et quand on le
		 * free la structure supprimée est toujours pointée quelque part.
		 * Un problème du à un manque de rigueur surement.
		 * Mais en orienté objet le destructeur devrait nous garder 
		 * de ce genre de saloperie c'est relou quand même.
		 * Et de même quand on surcharge l'affectation
		 * BREEEEf J'espère que le problème vient de là.
		 */
		DEBUG_MSG("Appel destroy");
		object a_suppr = OBJECT_get_cxr(variable, "cdr");
		OBJECT_destroy(&a_suppr);
		DEBUG_MSG("val_cpy-type : %d", val_cpy->type);
		if(free_flag) *free_flag = TRUE;
		OBJECT_set_cdr(variable, val_cpy);
	}
	return(name);

}

/** @fn int ENV_check_loop(object name)
 * @brief Vérifie la validité d'un environnement.
 * @param name : Variable que l'on souhaite vérifier.
 *
 * Vérifie qu'un environnement en présente pas 
 * d'incohérences comme des appels circulaires
 * de variable :@n
 * x->y->z->x->y... .
 *
 * @return 0 si incohérences trouvées, 1 sinon.
 */
int ENV_check_loop(object name)
{
	object root = name;
	object parcours = NULL;
	int loop = FALSE ;
	for(parcours = NULL; !loop && check_type(parcours, SFS_SYMBOL)
			;parcours = ENV_get_var(name, NULL)->this.pair.car)
	{
		if(root==parcours) loop = TRUE;
	}
	return(loop);
}

