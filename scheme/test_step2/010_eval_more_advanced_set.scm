; tests plus avancés pour set
; TEST_RETURN_CODE=FAIL
; TEST_COMMENT="more avanced set"



(define x 'abc')
(set! x (quote 4))
x
(define x 8)
(set! x (quote (define y 5)))
x
y ; A partir d'ici ne renvoie plus rien.
(set! x (define y 5) )
x
y

