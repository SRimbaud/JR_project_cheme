; tests plus avancés pour and
; TEST_RETURN_CODE=FAIL
; TEST_COMMENT=" avanced and"

(and (define x 5) (define y 8))
x
y;renvoie le second

(and (define x 5) (set! x 42 ))
x ; évalue la pemière expression et renvoie le second atome

(and (set! x 8 ) (define x 5))
x ; ne renvoie rien car x n'est pas défini dans la première expression

(define x 'abc')
(and (set! x 5) (set! x 7))
x; évalue la pemière expression et renvoie le second atome

(define x 'abc')
(and (set! x 7) (set! x (quote 4)))
x

(define x 8)
(and (define z 'abc') (set! x (quote (define y 5))) )
x
y ;n'existe paaas



