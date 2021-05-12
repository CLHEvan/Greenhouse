# Greenhouse

le programme est une base de code pour une serre interieur, aucune utilité particulière.
la base de code actuelle permet la gestion de la lumière et du chauffage.

le programme n'est pas finis donc pas fonctionnel.

## Fonctionnement:

Les sortis(lumière, chauffage) sont géré par les les classes controleur (commence par C), ces class contiennent la logique de controle pour les sortie.

Les classes controle s’appuient sur des entrées sous forme de classes abstraite, cela permet de facilement changer de type de capteur ou module.

La classe OGConfig sert d’interpréteur de commandes pour géré les différentes classes controleur, si nous voulons pouvoir interagir
avec les modules il suffit d’utiliser cette classe par exemple via communication serie, wifi, radio...
Cette classe enregistre dans une carte SD sous forme de commandes les paramètres modifiables des classes controleur, utile lors de mise hors tension.
