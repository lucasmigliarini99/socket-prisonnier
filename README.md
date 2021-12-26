# Dilemme du prisonnier

*Auteurs : Lucas Migliarini, Julien Peyrol, Sapy Oscar, Kananke Acharige Sahan*

Ce projet a pour but que deux ou quatre joueurs se connectent sur un serveur afin de jouer à un jeu « Dilemme du prisonnier ». Le but du jeu est d’obtenir le plus de points possibles, le joueur doit choisir entre « Se taire » et « Dénoncer ».

## Configuration du fichier `src/Server/config/config.ini`

Dans ce fichier, vous trouverez 4 paramètres que vous pourrez modifier:

- L'adresse IP du serveur
- Le port du serveur
- Les deux joueurs de la partie 1 (les deux identifiants doivent être séparé d'un "-")
- Les deux joueurs de la partie 2

## Configuration du fichier `src/Client/config/config.ini`

Dans ce fichier, vous trouverez 2 paramètres que vous pourrez modifier:

- L'adresse IP du serveur
- Le port du serveur

## Commandes :

```bash
make clean
```
Commande qui "nettoie" les fichiers créés, cette commande est facultative, mais il peut parfois être nécesaire si les commandes `make` suivantes ne fonctionnent pas.

```bash
make server
```
Permet de générer le fichier qui permet de lancer le serveur.

```bash
make client
```
Permet de générer le fichier permettant d'ouvrir la fenêtre pour le client.

```bash
doxygen doxygen/configuration/dilemme-game-config
```
Permet de générer une page `html` contenant la documentation de l'application dans un dossier `documentation`.

## Lancement du serveur et des clients :

```bash
./output/server
./output/client
```
Il faut tout d'abord lancer le serveur, puis les clients qui se connecterons automatiquement au serveur.

## Résultats attendus :
Pour le jeu :
- Les deux joueurs se taisent, il gagnent tous les deux 2 points
- Le joueur se tait et son adversaire dénonce, le joueur perd 1 point, l’adversaire gagne 3 points
- Le joueur dénonce et l’adversaire se tait, le joueur gagne 3 points, l’adversaire perd 1 point
- Les deux joueurs dénoncent, aucun joueur ne gagne de point

Au bout de 5 tours de jeu :
Génération d'un fichier CSV, `Data.csv` avec les résultats de la partie.

ATTENTION il faut bien laisser le temps aux fenêtres de s'actualiser
