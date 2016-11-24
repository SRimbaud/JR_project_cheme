
/**
 * @file number.h
 * @author François Cayre <cayre@yiking.(null)>
 * @date Sat Jun 23 00:42:34 2012
 * @brief Number handling.
 *
 * Number handling.
 */

#ifndef _NUMBER_H_
#define _NUMBER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "basic.h"
#include<limits.h>

#define NUM_REAL     0x00
#define NUM_INTEGER  0x01
#define NUM_UINTEGER 0x02
#define NUM_COMPLEX  0x03
#define NUM_UNDEF    0x04
#define NUM_PINFTY   0x05
#define NUM_MINFTY   0x06

typedef struct num_t {

    uint numtype;

    union {

        double       real;
        long int       integer;
    } this;

} num;
/* Fonctions num_t */

/* Affichage */
void NUM_print( num printed);
/* Constructeur ultime */

num* NUM_build(num* a, void* v_ptr, int mode);

/* Mise à jour */
num* NUM_set_int(num* a, int v);
num* NUM_set_real(num* a, double v);
/* Comparaison */
int NUM_lint_cmp(num a,long int v);
int NUM_cmp(num a, num b);
int NUM_cmp_lint(num a, num b);
int NUM_cmp_type(num a, num b);
int NUM_sign(num a);
/* Opérateur */
num NUM_sum(num a, num b, int * flag);
num NUM_sub(num a, num b, int* flag);
num NUM_mul(num a, num b, int* flag);
num NUM_div(num a, num b, int* flag);
num NUM_modulo(num a, num b, int* flag);
num NUM_quotient(num a, num b, int* flag);

#ifdef __cplusplus
}
#endif

#endif /* _NUMBER_H_ */
