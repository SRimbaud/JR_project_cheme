; Test de quote chainée ;
; TEST_RETURN_CODE=PASS
; TEST_COMMENT="Quote chainée"

(quote (quote -2))
'(quote -2)
'('(define x 4))
(quote '(define y 5))
''(a)
x
y
