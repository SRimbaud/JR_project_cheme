-------------------------Livrable 1 ------------------------------
Dans ce dossier le code source du livrable ainsi que le makefile.
Dans le dossier doxygen se trouve la documentation du code au 
format html et LaTex. Le format LaTeX n'est pas compilé cependant
le makefile est disponible (c'est celui généré automatiquement par
doxygen)
Dans ce livrable l'interpréteur supporte :
-Les entiers signés ou non.
-Les caractères
-Les chaines
-Les symboles
-Les listes
-Les booléens

Vous trouverez également le rapport au format PDF.
Celui-ci décrit les difficultés, la démarche, ce qui
marche et ne marche pas.
Il y a également un fichier de configuration doxyfile 
pour générer la docuementation doxygène. Celui-ci a été
obtenu à partir de doxywizard.
Vous trouverez également le rapport du premier livrable
au format pdf intitulé rapport_livrable_1.

--------------------------------------------------------------
Squelette de code de l'interpréteur Scheme du projet info PHELMA.
Le code fourni permet d'acquérir des chaînes de caractères et de vérifier
que ces chaînes contiennent une S-expression valide. 
Ce code fonctionne aussi bien en mode interactif qu'en mode script.

Votre tâche sera de le compléter de façon à lire ces chaînes, extraire
les objets Scheme qu'elles contiennent et afficher les objet ainsi acquis.  

Le répertoire est organisé ainsi : 
- Makefile pour compiler soit en mode debug (afficher les traces du programme)
soit en mode release (produit final)
- src : qui contient le code C de votre interpréteur. Vous aurez notamment à modifier le fichier read.c et à bien lire le fichier repl.c.      
- include : qui contient toutes les définitions de types et prototypes de votre programme. Lisez attentivement les headers avant de commencer à coder et de les modifier.
- tests_step0 : tests du code tel qu'il est fourni 
- tests_step1 : tests du code tel qu'il devrait être livré pour le premier incrément -> à vous de le compléter !

--- pour compiler le code en mode debug (il créé l'exécutable 'scheme')
$ make debug 

--- pour le lancer en mode interactif 
$ ./scheme 

--- pour le lancer en mode script 
$ ./scheme monFichier.scm

--- pour le lancer sur le fichier 02_readint.scm
$ ./scheme tests/02_readint.scm

--- pour lancer le programme de test sur l'ensemble des fichiers de tests_step0 en mode batch (non interactif)
$ ../testing/simpleUnitTest.sh -b -e ./scheme tests/simple/*.scm

--- pour créer l'archive à envoyer au profs (cela vous créé un tgz avec votre nom et la date)
$ make tarball
