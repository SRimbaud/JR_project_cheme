; tests plus avancés pour lambda
; TEST_RETURN_CODE=PASS
; TEST_COMMENT="more advanced lambda"


((lambda x x) 3 4 5 6)
((lambda (x y . z) z)
3 4 5 6)
((lambda x x) #t #t #t #t)
((lambda x x) #f #t #t #t)
((lambda x x) #f #f #t #t)
((lambda (x y . z) z)
#t #t #t #t)
((lambda (x y . z) z)
#t #f #t #f)
((lambda (x y . z) z)
#f #f #t #t)
;Celui là on a rien implémenté donc normal que ça ne marche pas.
