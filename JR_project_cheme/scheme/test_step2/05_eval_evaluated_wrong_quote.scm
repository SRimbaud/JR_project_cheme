; Fonctions qui devraient fonctionner malgré les erreurs
; TEST_RETURN_CODE=PASS
; TEST_COMMENT="Erreur que dans évaluation qui devrait pas se faire"
(quote (if (< 3 4) (> 4 -6) "Chaine qui devrait pas être là"))
'("Je suis erreur" define x 4)
(quote (define define 5))
'(define x 4 5 6 )
x

