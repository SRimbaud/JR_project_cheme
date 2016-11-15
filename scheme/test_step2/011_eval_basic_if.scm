; tests basiques pour if
; TEST_RETURN_CODE=PASS
; TEST_COMMENT="basic if"
(if #t #t #f); si vrai on retourne la conséquence
(if #f #t #f) ; si faux on retourne l'alternative
(if #f #t) ; si faux on retourne faux car il n'y a pas d"alternative



