
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
object sfs_eval( object ); 




/* Evaluation */

object EVAL_quote(object o);
object EVAL_define(object o);
object EVAL_set(object o);
object EVAL_and(object o);
object EVAL_or(object o);
object EVAL_if(object o);

/* Outils d'évaluation */
object IF_predicat(object input);
object IF_consequence(object input);
object IF_alternative(object input);
object IF_overargument(object input);
#ifdef __cplusplus
}
#endif

#endif /* _EVAL_H_ */
