-------------------------Livrable 3 ------------------------------
Dans ce dossier vous trouverez les sources, la documentation ainsi que le
rapport au format PDF.
Les fichiers tests sont incomplets et faux pour certains (corrigés
jusqu'au 06 et ils passent).

Nous avons implémenté toutes les primitives suivantes :


    +
    -
    *
    /
    remainder
    quotient
    <
    >
    <=
    >=
    =
    null?
    boolean?
    symbol?
    integer?
    real?
    char?
    string?
    pair?
    list?
    car
    cdr
    set-car!
    set-cdr!
    list
    cons
    eq?
    char->integer
    integer->char
    string->symbole
    symbole->string
    string->number
    number->string

Les primitives qui suivent ont été implémentées par erreur.
Nous les avons laissé pensant qu'elle pourrait servir.

    string->integer
    integer->string
    string->real
    real->string

Elles sont toutes fonctionnelles lorsque nous avons testé à la main
mais des fichiers tests les concernant n'ont pas été réalisés, il peut donc rester
des bugs que nous n'avons pas vu.
