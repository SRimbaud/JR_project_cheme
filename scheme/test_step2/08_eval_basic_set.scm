; tests de bases pour set
; TEST_RETURN_CODE=PASS
; TEST_COMMENT="Basic set"

(define x 2)
x
; on définit x et on met 2 dedans

(set! x 3)
x
; on change la valeur de x en 3

(set! x "abc")
x
; on change le type de x en chaine et on met "abc" dedans

(set! x #\t)
x
; on change le type de x en caractère et on met #\t dedans

(set! x #f)
x
; on change le type de x en booléen et on met #f dedans
