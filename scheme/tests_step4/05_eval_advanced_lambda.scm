; tests avancés pour lambda
; TEST_RETURN_CODE=FAIL
; TEST_COMMENT="advanced lambda"


(lambda (x y) (+ x y))
(lambda (x y) (+ x y) 3 4)
(lambda (x y) (+ (* x x)(* y y)) )
(lambda (x y) (+ (* x x)(* y y)) 3 4 )
(lambda (1) (+ 2 3))
(lambda (define x 5) (+ x 3))
(lambda (x) (quote x))
(lambda (x) (quote x) 5)
(lambda (x) (begin 1 2 x) )
(lambda (x) (begin 1 2 x) 5)
(lambda (x) (> x 5))
(lambda (x) (> x 5) 8)
(lambda (x) (< x 5))
(lambda (x) (< x 5) 8)
(lambda (x) (= x 5))
(lambda (x) (= x 5) 5)
(lambda (x) (and x x))
(lambda (x) (and x x) #t)
(lambda (x) (or x x))
(lambda (x) (or x x) #t)

