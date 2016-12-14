; tests de bases pour begin
; TEST_RETURN_CODE=PASS
; TEST_COMMENT="Basic begin"


(begin 1 2)
(begin 1 2 3)
(begin 1 (+ 5 3))
(begin (+ 5 3) 1)
(begin 4 (- 5 3))
(begin 4 (* 5 3))
(begin 4 (/ 8 4))
(begin #t #t)
(begin #f #t)
(begin 4 "abc")
(begin 4 #t "abc")
(begin 4 #f "abc")
(begin 4 #\t)
(begin 4 #f #\t)
(begin 4 "abc" #\t)
