; Des defines avec des expressions à évaluer
; TEST_RETURN_CODE=PASS
; TEST_COMMENT="Define avec expressions"

define(x (quote 4))
x
define(x (quote (define a 5)))
x
define(x (define y -456))
x
y


(define a 4)
(define b a)
(define c b)
(define a c)
a
b
c; permutation circulaire

(define x 4)
(define y x)
(define x y)
x

(define (x (define b x))) ; retourne rien 
x ; ne renvoie rien
(definé x 4); ne renvoie rien
(define x() 4) ; ne renvoie rien
(define x[ 4); ne renvoie rien
y ; ne renvoie rien
a ; renvoie rien erreur du début.






