
/**
 * @file object.h
 * @author François Cayre <cayre@yiking.(null)>
 * @date Fri Jun 15 17:49:46 2012
 * @brief Object definition for SFS.
 *
 * Object definition for SFS.
 */

#ifndef _OBJECT_H_
#define _OBJECT_H_

#ifdef __cplusplus
extern "C" {
#endif

/** @var NB_FORM 
 * @brief Nombre de formes reconnues par l'interpréteur.
 *
 * 22 Oct 2016 ==> 6 formes.
 */
#define NB_FORM 6

#include<strings.h>
#include<ctype.h>
#include<stdlib.h>
#include<limits.h>
#include "number.h"
#include "basic.h"
#include "mem.h"
#include"notify.h"

/* Enumérations */
typedef enum mots_clefs FORMS;
enum mots_clefs{
	QUOTE,  /**< La forme QUOTE valant 0 */
	DEFINE, /**< La forme DEFINE valant 1 */
       	SET, /**< La forme SET! valant 2 */
       	AND, /**< La forme AND valant 3 */
       	IF, /**< La forme IF valant 4 */
       	OR, /**< La forme IF valant 5 */
	NONE = 9999 /**< Forme inconnue valant 9999*/
};
/* Prototypages supplémentaires car inclusion multiples. */

void check_alloc(void* ptr, char* message);

/*Fin protoypage supplémentaires*/


typedef struct object_t {

    uint type;

    union {

        num              number;
        char             character;
        string           string;
        string           symbol;

        struct pair_t {
            struct object_t *car;
            struct object_t *cdr;
        } pair;

        struct object_t *special;

    } this;

} *object;

extern object vrai;
extern object faux;
extern object nil;
extern object form[NB_FORM];

/* Initialisation de l'interpréteur */
object make_object( uint type ); /* Créé un objet de type type, c'est une alloc,*/ 
object make_nil( void ); /* Crée la case NIL qui correspond à la fin d'une liste (ou liste vide). Aussi alloc.*/
object make_true(void);
object make_false(void);
void make_forms();
/*Fermeture de l'interpréteur */
void free_forms(void);

/* Constructeurs */

object OBJECT_build_pair(object car, object cdr);
object OBJECT_build_symbol(char* symbol);
object OBJECT_build_cpy(object o);
/* Desctructeurs */
void OBJECT_destroy(object* o);
void OBJECT_destroy_pair(object* o);
/*Accesseurs */
/* Get */
object OBJECT_get_cxr(object o, char* place);
/* Set */

void OBJECT_set_cdr(object o, object cdr);
void OBJECT_set_car(object o, object car);
void OBJECT_set_cxr(object o, object val, char* place);
/*Autres */

object OBJECT_built_short_quote(char* input, uint* here);
int OBJECT_isempty(object o);
void OBJECT_rewrite(object changed, object nouveau);
/* Comparaison */
int OBJECT_isEqual(object a, object b);
int OBJECT_cmp_bool(object a, object b);
int OBJECT_cmp_number(object a, object b);
int OBJECT_cmp_symb(object a, char* symbol);

/* Outils */
void check_alloc(void* ptr, char* message);
int check_type(object o, int sfs_type);
int string_cmp(char* a, char* b);
int is_form(object symbol, FORMS* forme);
int check(void* ptr, char* message);
/* Utilitaires d'évaluation d'expressions*/
int isealnum(char c);
int wait_first_non_blank_char(char* s1, uint* s1_cursor);



#define SFS_NUMBER       0x00
#define SFS_CHARACTER    0x01
#define SFS_STRING       0x02
#define SFS_PAIR         0x03
#define SFS_NIL          0x04
#define SFS_BOOLEAN      0x05
#define SFS_SYMBOL       0x06
#define SFS_UNKNOWN 	 0x07 /* Cas d'un type non reconnu utile au traitement des exceptions */
#define SFS_CDR 	 0x08
#define SFS_CAR 	 0x09
#define SFS_ENV 	 0x0A


/* Environnements */
extern object env;
/** @def ENV_ENABLE_CREATION
 *
 * Autorise la création d'une variable lorsqu'on essaie
 * d'en modifier une inexistante.
 */
#define ENV_ENABLE_CREATION 1 
/** @def ENV_DISABLE_CREATION
 *
 * Empêche la création d'une variable lorsqu'on essaie
 * d'en modifier une inexistante.
 */
#define ENV_DISABLE_CREATION 0 
object ENV_build(object father);
object ENV_get_var(object var, int* flag);
object ENV_add_var(object name, object value);
object ENV_update_var(object name,const object val, int mode, int* free_flag);
object ENV_get_var_in_env(object var, object environ, int* flag);
int ENV_check_loop(object name);

#ifdef __cplusplus
}
#endif

#endif /* _OBJECT_H_ */
