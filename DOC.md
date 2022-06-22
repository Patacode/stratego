# Documentation

Le jeux se présente sous 2 formes, via une interface console et via une interface graphique.

## Interface Console

La console offre aux utilisateurs différentes fonctionnalitées en fonction de l'os sur lequel ils se 
trouvent. Sous unix ou Apple, un système de buffer pour enregistrer les commandes entrées sera 
disponible, les fléches pourront être utilisées pour naviguer dans le texte courant ou dans l'historique
des commandes, TABx2 permet d'enclencher une certaine action en fonction du contexte, et les touches **^** 
et **$** pourront être utilisés pour pointer sur le début ou la fin du texte courant, respectivement.

Les joueurs choisissent un fichier de configuration de plateau de jeu pour positionner leur pions et peuvent
commencer à jouer. 

Pour intéragir avec le jeux, les joueurs entrent des commandes dans le terminal. Voici la liste
de l'ensemble des commandes disponibles:

Les champs se trouvant entre **\<\>** sont obligatoire pour l'exécution d'une commande. Ceux entre
**[]** sont optionnels. De plus, il est a noté que l'ensemble des commandes sont insensible à la 
casse.

* MOVE: Déplace un pion
    - MOVE \<pos\> \<up|down|left|right\> [count]
    - **pos** représente la position de la pièce à déplacer
    - **up|down|left|right** représente la direction dans laquelle se déplacer
    - **count** représente le nombre de case du déplacement (1 par défaut)
* ATTACK: Attaque un pion
    - ATTACK \<pos\> \<up|down|left|right\> [count]
    - **id** représente la position de la pièce à faire attaquer
    - **up|down|left|right** représente la direction dans laquelle attaquer
    - **count** représente le nombre de case du déplacement (1 par défaut)
* HELP: Liste l'ensemble des commandes avec leurs descriptions ou donne des informations plus détaillés sur une commande en particulier
    - HELP [cmd]
    - **cmd** le nom de la commande à détailler
* RULES: Affiche les règles du jeux
    - RULES
* PIECE: Affiche le rang et nom de l'ensemble des pièces du jeux ou pour une pièce en particulier
    - PIECE [symbol]
    - **symbol** le symbole de la pièce à afficher
* STOP: Met fin à la partie
    - STOP
* STAT: Affiche des statistiques sur les pièces du joueur courant ou sur une pièce en particulier
    - STAT [symbol]
    - **symbol** le symbole de la pièce pour laquelle des stats veulent être obtenu
* HISTORY: Affiche l'historique des 50 dernières actions (déplacement et attaque)
    - HISTORY
* CLEAR: Nettoie le terminal
    - CLEAR

Voici les affichages que ces commandes devrait fournir:

**MOVE**
```
Après un move correct:
[SUCCESS] Déplacement du pion ... en ...

Après un move incorrect:
[ERROR] Déplacement invalide. Recommencez.
```

**ATTACK**
```
Après une attaque correct:
[SUCCESS] Attaque du pion ... en ... par le pion ... en ...

Après une attaque incorrect:
[ERROR] Attaque invalide. Recommencez.
```

Suite à un move ou une attaque, le joueur courant appuie sur **Enter** pour passer au joueur suivant.

**HELP**
```
Sans argument:
    MOVE    = Déplace un pion
    ATTACK  = Attaque un pion
    HELP    = Affiche l'ensemble des commandes
    RULES   = Affiche les règles du jeux
    PIECE   = Affiche le rang et nom de l'ensemble des pions ou d'un pion spécifique
    STOP    = Met fin à la partie
    STAT    = Affiche des statistiques sur les pions du joueur courant ou sur un pion spécifique
    HISTORY = Affiche l'historique des 50 dernières actions (déplacement et attaque)
    CLEAR   = Nettoie le terminal
Avec argument: 
    HELP MOVE
        Description: Déplace un pion
        Syntaxe: MOVE <id> <up|down|left|right> [count]
    HELP ATTACK
        Description: Attaque un pion
        Syntaxe: ATTACK <id> <up|down|left|right>
    ...
```

**RULES**
```
Stratego - Règles

...Règles du jeux...
```

**PIECE**
```
Sans argument:
    1  Espion
    2  Èclaireur
    3  Démineur
    4  Sergent
    5  Lieutenant
    6  Commandant
    7  Major
    8  Colonel
    9  Général
    10 Maréchal
    B  Bombe
    D  Drapeau
Avec argument:
    PIECE 7
        7 Major
    PIECE 10
        10 Maréchal
    ...
```

**STOP**
```
[PROMPT] Fin de la partie
```

**STAT**
```
Sans argument:
    1  Espion     - *(1) 
    2  Èclaireur  - ********(8)
    3  Démineur   - *****(5)
    4  Sergent    - ****(4)
    5  Lieutenant - ****(4)
    6  Commandant - ****(4)
    7  Major      - ****(3)
    8  Colonel    - **(2)
    9  Général    - *(1)
    10 Maréchal   - *(1)
    B  Bombe      - ******(6) 
    D  Drapeau    - *(1)
Avec argument:
    STAT 7
        7 Major - ***(3)
    STAT B
        B Bombe - ******(6)
```

**HISTORY**
```
1  Déplacement du pion ... en ...
2  Déplacement du pion ... en ...
3  Déplacement du pion ... en ...
4  Attaque du pion ... en ... par le pion ... en ...
5  Attaque du pion ... en ... par le pion ... en ...
6  Déplacement du pion ... en ...
7  Attaque du pion ... en ... par le pion ... en ...
8  Déplacement du pion ... en ...
9  Déplacement du pion ... en ...
10 Déplacement du pion ... en ...
...
```

## Interface Graphique

L'interface graphique proposée permet aux utilisateurs de jouer au jeu de Stratego de manière plus
simple.

Ces-derniers intéragissent avec le plateau de jeu via des cliques de souris et peuvent configurer leur plateau de jeu
de plusieurs manières.

Pour mettre en place leur plateau de jeu les utilisateurs peuvent par exemple, charger un fichier de configuration en donnant son nom ou en le déposant dans la fenêtre
via un mécanisme de Drag and Drop, ou ils peuvent le configurer eux-mêmes à la main.

Lors de la phase de mise en place, les joueurs disposeront également d'un espace de stockage leur permettant de stocker temporairement
leurs pions.

La fenêtre permettant aux joueurs de jouer réellement au jeu sera composé, en plus du plateau de jeu, d'un cimetière regroupant l'ensemble des pions
qui ont été mangés lors de la partie, d'un tableau de statistiques sur les pions du joueur courant et d'un historique tenant à jour le joueur courant
de la dernière action produite.

L'ensemble des composants et features utilisés dans cette version graphique s'inspirent, de par leurs utilisations, de ce qui avait déjà été implémenté dans
la version console.

