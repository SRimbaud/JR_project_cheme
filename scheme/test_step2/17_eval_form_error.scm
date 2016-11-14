;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; TEST_RETURN_CODE=PASS
;; TEst d'entrées avec erreur ;;
(or (define (define x 4) ) )
; Mauvais pour le define.
(define define 4)
; on peut pas nommer un symbole scheme
define
(if) 
; manque predicat et consequence
(if #t)
; manque conséquence
(if (defined x 4) #t)
; symbole inexistant
(if #t #t #f #t)
;Trop d'arguments dans if
(define x 4 5)
;trop d'arguments dans define
(set! x 4 5)
;trop d'arguments dans set.
(if (if #t #t) (define x 4) (erreur y 43) )
x
y
;Pas d'erreur on évalue pas la fin on a donc un retour ici
