; tests de bases pour cdr
; TEST_RETURN_CODE=PASS;
; TEST_COMMENT = "Basic cdr"

(cdr (cons 4 5))
(define x (cons 5 6))
(cdr x)
(define x (cons (cons #\c "chaine") (cons 34 56.98)))
(cdr x)
