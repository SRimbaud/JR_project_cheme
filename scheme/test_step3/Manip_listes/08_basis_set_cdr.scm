; tests de bases pour set_cdr
; TEST_RETURN_CODE=PASS;
; TEST_COMMENT = "Basic set_cdr!"

(set-cdr! (cons 3 4) #t)
(define x (list "chaine" ''pata$*te))
x
(set-cdr! x "Ce symbole est vraiment lait")
x
