; tests basiques pour or
; TEST_RETURN_CODE=PASS
; TEST_COMMENT="basic or"

(or) ; or renvoie faux par défaut

(or 4); or d'un atome renvoie l'atome

(or 4 5); or renvoie le premier vrai = 4 (le premier vrai = atome différent du booléen faux )

(or #t 8);  s'arrete des qu'il trouve vrai

(or #f 5) ; renvoie le premier vrai

(or #t #t); s'arrete des qu'il trouve vrai

(or #f #f); renvoie faux

(or #t #f) ; s'arrete des qu'il trouve vrai


(or #f #f #f); renvoie faux

(or #t #f #f); s'arrete des qu'il trouve vrai

(or #f #f 8); renvoie le premier vrai

(and #t #f 8); s'arrete des qu'il trouve vrai
