# Stratego

![strategoImage](/resources/images/stratego-header.png)

Implémentation classique du jeu de plateau **Stratego**. Le but de ce jeu
est de capturer le drapeau ennemi ou d'empêcher le joueur adverse de déplacer
ces pions.

<details>
    <summary>Table of Contents</summary>
    <ol>
        <li>
            <a href="#à-propos-du-projet">À propos du projet</a>
            <ul>
                <li><a href="#techno-utilisée">Techno utilisée</a></li>
            </ul>
        </li>
        <li>
            <a href="#commencer">Commencer</a>
            <ul>
                <li><a href="#prérequis">Prérequis</a></li>
                <li><a href="#installation">Installation</a></li>
                <li><a href="#exécution">Exécution</a></li>
            </ul>
        </li>
        <li><a href="#utilisation">Utilisation</a></li>
        <li><a href="#contribution">Contribution</a></li>
    </ol>
</details>

## À propos du projet

Ce projet consiste à digitaliser le jeu de **Stratego** pour pouvoir y jouer dans le terminal et via
une interface graphique. Le jeu sera disponible en locale pour 2 joueurs, un joueur rouge et un joueur
bleu, en utilisant les règles de jeu classiques.

L'implémentation classique de ce jeu ne tient cependant pas compte des règles contraignantes empêchant de
poursuivre indéfiniment un pion adverse et obligeant d'attaquer un pion adverse en prise.

Un mode de jeu supplémentaire sera également disponible aux joueurs, nommé *StrategoReveal* permettant de 
laisser visible l’identité d’un pion préalablement révélée lors d’un combat.

Les règles du jeu sont disponible dans le dossier `resources/` que vous pouvez trouver à la 
racine du projet au format **`.pdf`** ou directement depuis le jeu.

En plus de ces règles de jeu, la documentation propre à l'interface console et graphique vous êtes
accessible depuis le fichier `DOC.md` se trouvant à la racine du projet.

### Techno utilisée

* [C++](https://www.cplusplus.com/)
* [QtCreator](https://www.qt.io/product/development-tools)
* [catch2](https://github.com/catchorg/Catch2/tree/v2.x)
* [Qt5](https://doc.qt.io/qt-5/)

## Commencer

Pour mettre en place le projet localement et l'exécuter dans votre terminal ou depuis 
une interface graphique, vous pouvez suivre les instructions ci-dessous.

### Prérequis

Pour compiler et exécuter le projet vous devez disposer d'un compilateur C++ du style `g++`. Sous windows, une installation
de `MinGW` devrait faire l'affaire.

### Installation

Le projet a été réalisé en utilisant **QtCreator**, pour le compiler il vous faudra donc vous rendre
dans l'IDE. Ouvrez le projet et compiler le en cliquant sur le marteau en bas à gauche (Ctrl+b):

![qtcompiling](/resources/gif/qt_compiling.gif)

Pour copier automatiquement les ressources du projet dans le dossier de compilation (`build*/`) créé par qt, de sorte à pouvoir
les rendre accessible à l'application, il est nécessaire d'ajouter une étape de compilation au projet via qt:

![qtcompiling](/resources/gif/qt_buildStep.gif)

Cette étape peut être réalisée directement en ligne de commande suite à la compilation du projet:

```
[~/stratego] cd build*/
[~/stratego] make install
```

### Exécution

Pour exécuter le projet, vous pouvez soit l'exécuter via l'IDE, soit en ligne de commande via
l'exécutable fournis par qt lors de la compilation.

En ligne de commande (pour l'application en terminal):

```
[~/stratego] ./build*/src/tui/tui
```

Et via l'IDE (toujours pour l'application en terminal):

![qtrunning](/resources/gif/qt_running.gif)

L'application supporte également un mode de jeu supplémentaire auquel vous pouvez jouer. Pour ce faire il
vous faut rajouter un argument de ligne de commande. Soit via la terminal:

```
[~/stratego] ./build*/src/tui/tui reveal
```

Soit via l'IDE:

![qtcmdline](/resources/images/qt_cmdline.png)

## Utilisation

L'implémentation fournie pour les utilisateurs de l'application leur permettra, depuis la version
console, de jouer au jeu de Stratego via certaines commandes mises à leur disposition.

Pour mettre en place son armé de pions, la version console propose d'utiliser des fichiers de configuration de plateau
de jeu. Ces fichiers permettront de décrire le positionnement des pions de son armé et se trouveront dans
le dossier `/board` que l'on retrouve sous `/resources/config` dans le dossier `Stratego` du projet. Ces fichiers pourront être
sélectionné en début de partie.

Voici l'interface de base, lorsque les joueurs jouent au jeu (on observe que c'est au joueur rouge de jouer):

![strategoRedTurn](/resources/images/strategoRedTurn.png)

Les joueurs pourront intéragir avec le plateau de jeu et le système de la sorte:

![strategoDemoConsole](/resources/gif/strategoDemoConsole.gif)

Depuis la version GUI du jeu les utilisateurs pourront intéragir avec l'application directement via leur
clavier et souris et disposeront d'une interface leur permettant de visualiser en temps réel toute une
série d'informations (comme leurs pions ou les dernières actions qui se sont produites).

La version graphique permet aux utilisateurs de charger un fichier de configuration pour mettre en place leurs pions (comme pour la version
console) et leur permet également de les disposer à la main ou depuis un fichier en mode Drag and Drop.

Voici une demo de comment l'application graphique s'exécute:

![strategoDemoGui](/resources/gif/strategoDemoGui.gif)

## Contribution

Ce projet a été réalisé par:

* Maximilien Ballesteros
* Alexandra Demarteau
