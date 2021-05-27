# Open Greenhouse

Open Greenhouse est un simple projet dans le but de m’entraîner.

C'est un programme qui fonctionne sous Arduino, il est fait le plus modifiable et adaptable possible,
on peut ainsi facilement ajouter ou changer des capteurs et fonctionnalité.

## Fonctionnement

le programme se compose de deux type classes, **control** et **sensor**.

Les classes **sensor**: <br />
	sont les entrées, par exemple **STemp** qui fournit une température
	par un capteur, ou la classe **STime** qui fournie la date et l'heure par
	un module RTC(qui n'est pas vraiment un capteur mais c'est pour l'image).

Les classes **control**: <br />
	sont comme leur nom l'indique des classes qui contrôle, par exemple la classe **CHeater**
	contrôle le chauffage avec la température que **STemp** lui fournit, on peut dire que les
	les classes **control** gèrent les sortie.

Maintenant que notre programme peut utiliser des capteurs et contrôler des sorties il faut pouvoir fournir
à l'utilisateur un moyen de paramétrer tout cela.
C'est la qu'entre en jeux **OGConfig**, on donne à cette classe les paramètre par référence(voir **IConfigurable** dans **OGConfig.h**) comme
la température voulu ou  la dentition de l'heure du système, et fournit un système relativement simple et adaptable de commande qui permet d’interagir avec 
ses paramètres, ça se résume en gros par un get/set qui s'adapte aux paramètres qu'on lui donne.

Par défaut **OGconfig** est relié au port Serial, il est tout à fait possible de le connecter à un module wifi ou autre.
Pour pouvoir utiliser la configuration il faut donc créer une application sur ordinateur ou téléphone pour géré les paramètres, bien qu'il soit
aussi possible que faire une interface avec le MCU.

Enfin le programme permet par carte SD de sauvegarder la configuration actuelle et la rétablir au démarage.
Il est possible de sauvegarder ou rétablir par le système de commandes.
