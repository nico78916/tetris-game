Ce projet est un simple jeu de Tetris like ...

PATCH NOTE :
+ ajout de update.h (les fonctions étaient anciennement dans main.c)
+ ajout de save.h
+ ajout de game.h pour les besoins de save.h
Étant donné le nombre grandissant de modules, on passe sur un makefile basique


Composition :
main.c -> programme principale
module graphique contenant la definition des écrans
module de sauvegarde permettant de charger et d'enregistrer.
module du jeu permettant d'interagir avec l'utilisateur
