; tests plus avancés pour or
; TEST_RETURN_CODE=FAIL
; TEST_COMMENT=" avanced or"

(or (define x 5) (define a 8))
x; renvoie le premier vrai 

(or (define x 5) (set! x 8 ))
x ;renvoie le premier vrai

(or (set! b 8 ) (define b 5))


(define x 'abc')
(or (set! x 5) (set! x 7))
x;renvoie le premier vrai

(define x 'abc')
(or (set! x (quote 4)) (set! x 7) )
x; renvoie le premier vrai 

(define x 8)
(or (set! x (quote (define y 5))) (define z 'abc') )
x
y; N'existe pas ne renvoie rien.
a ; Doit pas être évalué au début
b ; ne renvoie rien car y n'est pas défini dans la première expression

