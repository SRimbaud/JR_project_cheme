; tests de bases pour set_car
; TEST_RETURN_CODE=PASS;
; TEST_COMMENT = "Basic set_car!"

(set-car! (cons 3 4) #t)
(define x (list "chaine" ''pata$*te))
x
(set-car! x "Ce symbole est vraiment lait")
x

