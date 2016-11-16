#include"primitive.h"

/** @fn void init_primitive()
 * @brief Initialise la liste de nom de primitives.
 *
 */
void init_primitive()
{
	primitive_name[ADD] =OBJECT_build_symbol("+");
	primitive_name[SUB] =OBJECT_build_symbol("-");
	primitive_name[DIV] =OBJECT_build_symbol("*");
	primitive_name[MUL] =OBJECT_build_symbol("/");
}

/** @fn void init_prim_func(int taille)
 * @brief Initialise la liste de pointeur de fonction.
 *
 */
void init_prim_func(int taille)
{
	primitive_function[ADD] = PRIM_somme;
	primitive_function[SUB] = PRIM_soustrait;
	primitive_function[DIV] = PRIM_divise;
	primitive_function[MUL] = PRIM_multiplie;
}


/* Repérage de primitive */

/** @fn ptr_primitive PRIM_isprimitiv(object symbole)
 * @brief Si le symbole est une primitive renvoie un pointeur sur
 * la fonction correspondante.
 *
 * @return Renvoie un pointeur sur la primitive, null sinon.
 */
ptr_primitive PRIM_isprimitiv(object symbole)
{
	int i ;
	ptr_primitive = NULL;
	for(i = 0; i < NB_PRIMITIVE ; i++)
	{
		if(OBJECT_isEqual(symbole, primitive_name[i]))
		{
			return( primitive_function[i] );
		}
	}	
	return( NULL);
}



/* Primitives */

/* Opérateurs mathématiques */
/** @fn object PRIM_somme(object a) 
 * @brief
 *
 * @return
 */
object PRIM_somme(object a) 
{
	return(NULL);
}

/** @fn object PRIM_soustrait(object a) 
 * @brief
 *
 * @return
 */
object PRIM_soustrait(object a) 
{
	return(NULL);
}
/** @fn object PRIM_multiplie(object a) 
 * @brief
 *
 * @return
 */
object PRIM_multiplie(object a) 
{
	return(NULL);
}
/** @fn object PRIM_divise(object a)
 * @brief
 *
 * @return
 */
object PRIM_divise(object a)
{
	return(NULL);
}
