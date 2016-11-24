; tests de bases pour
; TEST_RETURN_CODE=PASS;
; TEST_COMMENT = "Basic cons"

(cons 4 (cons "chaine" 45))

(define x (cons 4 5))
x

(define y (cons "quatre" "cinq"))
y

(define z (cons x y)); Vérif de l'interprétation.
z
; Ne doit rien renvoyer
(cons 4 5 6)
