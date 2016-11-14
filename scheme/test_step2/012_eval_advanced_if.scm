; tests avancés pour if
; TEST_RETURN_CODE=FAIL
; TEST_COMMENT="advanced if"

(if #t (define x 4) #f)
x
; si vrai on évalue la conséquence qui définit x et met 4 dedans

(if #f (define x 38) #f)
x
; si faux on retoune l'alternative faux et on n'évalue pas la conséquence

(if #t (quote 4) #f)
; si vrai on évalue la conséquence

(if #f (quote 4) #f)
; si faux on retourne l'alternative sans évaluer la conséquence

(define x "carotte")
(if #t (set! x 6) #f)
x
; si vrai on évalue la conséquence donc on change la valeur de x

(define x 4)
(if #f (set! x 6) #f)
x 
; si faux on retourne l'alternative sans évaluer la conséquence donc on ne change pas la valeur de x


