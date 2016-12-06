; tests de bases pour list
; TEST_RETURN_CODE=PASS;
; TEST_COMMENT = "Basic list"

(list 3 4)
(list 4 5 3 4 5)
(list "Je" 4 5 "suis" #t () "Hétérogène" 99999999999999999); un infini
(list -9999999999999999999 999999999999999999 0 ()); infini positif et négatif.
