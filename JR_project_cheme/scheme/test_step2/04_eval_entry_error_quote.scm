; Test de quote dont l'entrée est fausse ;
; TEST_RETURN_CODE=PASS
; TEST_COMMENT="Test d'entrée utilisateur fausse"

quote (define x 4) ; Manque les parenthèses sur quote
x
(quote (define y #\erreur))
y

'(sy[]mbole 4)
; Toutes ces choses doivent planter, ce sont des erreurs sur
; les formats d'entrée.
