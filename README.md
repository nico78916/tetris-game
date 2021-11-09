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


api : fonction print et indexOf

const : résolution

game : fonction d'utilisation des bloques

	initialisation						ok
	génération de bloques					ok
	gauche droite						ok
	rotation horaire et anti-horaire			ok
	déscente déscente_accélérée				à faire
	générérer bloques sur l'écran				à faire
	fonction pour gérer la descente				à faire
	actualiser image pendant la descente			à faire
	fonction pour vérif si le bloque a atteind le bas	à faire
	fonction de vérif si une ligne est complète		à faire
	fonction pour suppr une ligne si ligne complète		à faire
	fonction pour vérif si le coup est réalisable		à faire
	fonction pour vérifier si le jeu est finit		à faire
	fonction score						à faire
	fonction pause						à faire
	fonciton pour terminer le jeu si il est finit		à faire
    
graphics : fenêtres avec MLV

	fenêtre menu
		fenêtre nouvelle partie
			fenêtre 1 joueur
			fenêtre 2 joueurs
			fenêtre 3 joueurs
			fenêtre 4 joueurs
			retour menu
		continuer (charge la dernière save)
		charger
			gestion de 5 saves et appel une save
			donner un nom aux saves voir le nom du profil?
			retour menu
		fenêtre options
			résolution
			mode
			couleur
			son
			retour
		boutton quitter

save : fonction pour sauvegarde

		créer une sauvegarde				à faire
		vérifier les slots disponibles			à faire
		choix du slot					à faire
		écriture dans le fichier de sauvegarde		ok
		vérif si fichier de sauvegarde vide ou utilisé	à faire
		écraser fichier de sauvegarde déjà utilisé	à faire
		récupérer une sauvegarde			ok
		lancer une sauvegarde				à faire

string : manipulation de chaîne

update : fonction click

		fonction de de prise en compte du clic
		fonction de vérif validité clic
		
		menu
			appel nouvelle partie
				appel fenêtre 1 joueur
				appel fenêtre 2 joueurs
				appel fenêtre 3 joueurs
				appel fenêtre 4 joueurs
				retour
			appel continuer (charge la save)
			appel charger
				appel save 1
				appel save 2
				appel save 3
				appel save 4
				appel save 5
				retour
			appel options
				pas de nouvelles fenêtre
				retour
			quitter

Key binding