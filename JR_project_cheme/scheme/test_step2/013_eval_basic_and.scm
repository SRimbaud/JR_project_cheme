; tests basiques pour and
; TEST_RETURN_CODE=PASS
; TEST_COMMENT="basic and"

(and) ; and renvoie vrai par défaut
(and 4); and d'un atome renvoie l'atome
(and 4 5); and de deux atomes qui ne sont pas des booléens renvoie le second atome
(and #t 8); renvoie le second
(and #f 5) ; s'arrete des qu'il trouve faux
(and #t #t); vrai et vrai retourne vrai
(and #f #f); s'arrete des qu'il trouve faux
(and #t #f) ; s'arrete des qu'il trouve faux
(and #t #t #t); renvoie vrai
(and #f #t #t); s'arrete des qu'il trouve faux
(and #t #t 8); renvoie le dernier atome

(and #t #f 8); s'arrete des qu'il trouve faux










