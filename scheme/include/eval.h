/**
 * @file eval.h
 * @author François Cayre <cayre@yiking.(null)>
 * @date Fri Jun 22 20:10:38 2012
 * @brief Evaluation stuff for SFS.
 *
 * Evaluation stuff for SFS.
 */

#ifndef _EVAL_H_
#define _EVAL_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "primitive.h"
/* Prototypages supplémentaires */

/* fin */
typedef struct object_t* object;
object sfs_eval( object input, object env ); 
object sfs_eval_compound(object pair);




/* Evaluation formes */

object EVAL_quote(object o,  object env);
object EVAL_define(object o, object env);
object EVAL_set(object o, object env);
object EVAL_and(object o, object env);
object EVAL_or(object o, object env);
object EVAL_if(object o, object env);
object EVAL_begin(object o, object env);
object EVAL_lambda(object o, object env);
object EVAL_let(object o, object env);

/* Outils d'évaluation */
object IF_predicat(object input);
object IF_consequence(object input);
object IF_alternative(object input);
object IF_overargument(object input);
object LAMBDA_get_var(object input);
object LAMBDA_get_body(object input);
int LAMBDA_check_number_arg(object input);
object LET_eval(object input, object env);
int LET_check(object input);
#ifdef __cplusplus
}
#endif

#endif /* _EVAL_H_ */
