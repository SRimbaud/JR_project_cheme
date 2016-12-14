; tests avancés pour let
; TEST_RETURN_CODE=FAIL
; TEST_COMMENT="advanced let"

(define add
(let ((x 4))
(lambda (y) (+ x y))))
(add 6)

(define soustrac
(let ((x 4))
(lambda (y) (- x y))))
(soustrac 6)

(define prod
(let ((x 4))
(lambda (y) (* x y))))
(prod 6)

(define division
(let ((x 8))
(lambda (y) (/ x y))))
(division 2)

(let ((x 8)) (define z x))
(let ((x 8)) (set! x 4))
