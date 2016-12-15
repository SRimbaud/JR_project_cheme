; tests de bases pour define
; TEST_RETURN_CODE=PASS
; TEST_COMMENT="Basic define"

(define somme (lambda (x) (+ x 2)))
(somme 3)
(somme (somme 3))

(define somme (lambda (x y) (+ x y)))
(somme 3 4)

(define produit (lambda (x) (* x 2)))
(produit 3)
(produit (produit 3))

(define produit (lambda (x y) (* x y)))
(produit 3 4)

(define division (lambda (x) (/ x 2)))
(division 4)
(division (division 8))

(define division (lambda (x y) (/ x y)))
(division 4 2)
(division (division 8 2) 1)

