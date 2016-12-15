; tests avancés pour begin
; TEST_RETURN_CODE=PASS
; TEST_COMMENT="advanced begin"



(begin (define x 5) (+ x 3))
(begin (define x 5) (- x 3))
(begin (define x 5) (* x x))
(begin (define x 8) (/ x 2))
(begin (define x 1) (set! x 5) (+ x 3))
(begin (set! x 5) (define x 1) (+ x 3))
(begin (define x 1) (set! x 5) (- x 3))
(begin (define x 1) (set! x 5) (* x x))
(begin (define x 1) (set! x 8) (/ x 2))
(begin (and #t #f) (and #t #t))
(begin (and #t #f) (or #t #t))
(begin (or #t #f) (or #t #t))
(begin (or #t #t) (or #t #f))
(begin 4 (quote 2))
(begin (quote 2) 4)
(begin #f #f)
(begin #t #f)
(begin (and #t #t) (and #t #f))
(begin (or #t #t) (and #t #f))
(begin (+ x 3) (define x 5))
