; tests de bases pour list
; TEST_RETURN_CODE=PASS;
; TEST_COMMENT = "Basic list"

(list (list 3 4) (list "TEST" 'define 'a 4))
(define x (cons 3 4))
x
(list x 4)
(list 4 x)
(list x x)

