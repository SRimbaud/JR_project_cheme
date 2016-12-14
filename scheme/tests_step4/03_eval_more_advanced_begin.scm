; tests plus avancés pour begin
; TEST_RETURN_CODE=PASS
; TEST_COMMENT=" more advanced begin"

(define x 8)
(if (> x 5) (begin (set! x 5) (* x x)) (- x 5))
(if (> x 5) (begin (define y 3) (* x y)) (- x 5))
(if (> x 5) (begin (define y 3) (quote y)) (- x 5))
(if (< x 5) (begin (set! x 5) (* x x)) (- x 5))
(if (< x 5) (begin (define y 3) (* x y)) (+ x 5))
(if (< x 5) (begin (define y 3) (quote y)) (* x 5))
