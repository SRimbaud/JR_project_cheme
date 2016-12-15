; tests de bases pour let
; TEST_RETURN_CODE=PASS
; TEST_COMMENT="Basic let"

(let ((x 8) (y 4)) (+ x y))
(let ((x 8) (y 4)) (- x y))
(let ((x 8) (y 4)) (* x y))
(let ((x 8) (y 4)) (/ x y))



(let ((x 2) (y 3))
	(let ( (x 7) (z (+ x y)) )
		(* z x)
	)
)

(let ((x #t) (y #t)) (and x y))
(let ((x #t) (y #t)) (or x y))
(let ((x #t) (y #f)) (or x y))

(let ((x #f) (y #f)) (and x y))
(let ((x #t) (y #f)) (and x y))
(let ((x #f) (y #f)) (or x y))


