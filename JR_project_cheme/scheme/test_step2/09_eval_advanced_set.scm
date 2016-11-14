; tests avancés pour set
; TEST_RETURN_CODE=PASS
; TEST_COMMENT="avanced set"
(define y 7)
y
(define x (set! y 4))
x
y

(define x 8)
(define y 7)
(set! x (set! y 4))
x
y



