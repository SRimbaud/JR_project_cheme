; tests de bases pour car
; TEST_RETURN_CODE=PASS;
; TEST_COMMENT = "Basic car"

(car (cons 4 5))
(define x (cons 5 6))
(car x)
(define x (cons (cons #\c "chaine") (cons 34 56.98)))
(car x)


