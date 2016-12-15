; tests avancés pour define
; TEST_RETURN_CODE=PASS
; TEST_COMMENT="advanced define"

(define (somme x) (+ x 2))
(somme 3)
(somme (somme 3))
(define (somme x y) (+ x y))
(somme 3 4)


(define (produit x) (* x 2))
(produit 3)
(produit (produit 3))
(define (produit x y) (* x y))
(produit 3 4)


(define (division x) (/ x 2))
(division 4)
(division (division 8))
(define (division x y) (/ x y))
(division 8 2)


(somme (somme 3 4))
(produit (produit 3 4))
(division (division 8 2))
; pas implémenté.
