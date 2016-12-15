; tests de bases pour lambda
; TEST_RETURN_CODE=PASS
; TEST_COMMENT="Basic lambda"

(lambda (x) (+ x 2))
((lambda (x) (+ x 2)) 3)
(lambda (x) (- x 2))
((lambda (x) (- x 2)) 3)
(lambda (x) (* x 2))
((lambda (x) (* x 2)) 3)
(lambda (x) (/ x 2))
((lambda (x) (/ x 2)) 4)
