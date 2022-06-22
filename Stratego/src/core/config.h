#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include "properties.h"

namespace stratego{

    struct PieceInfo{
        const int rank;
        std::string_view name;
        std::string_view symbol;
        const int count;
    };

    /**
     * Centralise l'ensemble des données propre à la configuration de l'application
     */
    class Config{

        public:

            /**
             * Taille par défaut du plateau de jeu. Le plateau de jeu est
             * représenté par une matrice à deux dimensions où chacune des
             * dimensions possède une taille équivalente à BOARD_SIZE.
             */
            static constexpr int BOARD_SIZE = 12;

            /**
             * Nombre de joueurs par défaut participant à une partie de Stratego.
             */
            static constexpr int PLAYER_COUNT = 2;

            /**
             * Respectivement le rang, nom et symbole du pion Maréchal.
             */
            static constexpr PieceInfo PIECE_MARSHAL_INFO {10, "Maréchal", "10", 1};

            /**
             * Respectivement le rang, nom et symbole du pion Général.
             */
            static constexpr PieceInfo PIECE_GENERAL_INFO {9, "Général", "9", 1};

            /**
             * Respectivement le rang, nom et symbole du pion Colonel.
             */
            static constexpr PieceInfo PIECE_COLONEL_INFO {8, "Colonel", "8", 2};

            /**
             * Respectivement le rang, nom et symbole du pion Major.
             */
            static constexpr PieceInfo PIECE_MAJOR_INFO {7, "Major", "7", 3};

            /**
             * Respectivement le rang, nom et symbole du pion Commandant.
             */
            static constexpr PieceInfo PIECE_CAPTAIN_INFO {6, "Commandant", "6", 4};

            /**
             * Respectivement le rang, nom et symbole du pion Lieutenant.
             */
            static constexpr PieceInfo PIECE_LIEUTENANT_INFO {5, "Lieutenant", "5", 4};

            /**
             * Respectivement le rang, nom et symbole du pion Sergent.
             */
            static constexpr PieceInfo PIECE_SERGENT_INFO {4, "Sergeant", "4", 4};

            /**
             * Respectivement le rang, nom et symbole du pion Démineur.
             */
            static constexpr PieceInfo PIECE_MINER_INFO {3, "Démineur", "3", 5};

            /**
             * Respectivement le rang, nom et symbole du pion Maréchal.
             */
            static constexpr PieceInfo PIECE_SCOUT_INFO {2, "Eclaireur", "2", 8};

            /**
             * Respectivement le rang, nom et symbole du pion Éclaireur.
             */
            static constexpr PieceInfo PIECE_SPY_INFO {1, "Espionne", "1", 1};

            /**
             * Respectivement le rang, nom et symbole du pion Bombe.
             */
            static constexpr PieceInfo PIECE_BOMB_INFO {11, "Bombe", "B", 6};

            /**
             * Respectivement le rang, nom et symbole du pion Drapeau.
             */
            static constexpr PieceInfo PIECE_FLAG_INFO {0, "Drapeau", "D", 1};

            /**
             * Le plus petit rang des pions.
             */
            static constexpr int PIECE_MIN_RANK = PIECE_FLAG_INFO.rank;

            /**
             * Le plus grand rang des pions.
             */
            static constexpr int PIECE_MAX_RANK = PIECE_BOMB_INFO.rank;

            /**
             * Taille initial que fait une armée de pions.
             */
            static constexpr int ARMY_SIZE = PIECE_FLAG_INFO.count
                    + PIECE_BOMB_INFO.count
                    + PIECE_SPY_INFO.count
                    + PIECE_SCOUT_INFO.count
                    + PIECE_MINER_INFO.count
                    + PIECE_SERGENT_INFO.count
                    + PIECE_LIEUTENANT_INFO.count
                    + PIECE_CAPTAIN_INFO.count
                    + PIECE_MAJOR_INFO.count
                    + PIECE_COLONEL_INFO.count
                    + PIECE_GENERAL_INFO.count
                    + PIECE_MARSHAL_INFO.count;

            /**
             * Symbole utilisé pour représenter un mur dans le terminal.
             */
            static constexpr char SYMBOL_WALL = '#';

            /**
             * Symbole utilisé pour représenter de l'eau dans le terminal.
             */
            static constexpr char SYMBOL_WATER = '~';

            /**
             * Symbole utilisé pour représenter une case vide dans le terminal.
             */
            static constexpr char SYMBOL_EMPTY = '.';

            /**
             * Symbole utilisé pour représenter espace blanc horizontal dans le terminal.
             */
            static constexpr char SYMBOL_HWSPACE = '-';

            /**
             * Symbole utilisé pour représenter un espace blanc vertical dans le terminal.
             */
            static constexpr char SYMBOL_VWSPACE = '|';

            /**
             * Symbole utilisé pour représenter un pion face caché le terminal.
             */
            static constexpr std::string_view SYMBOL_HIDDEN_PIECE = "[^_^]";

            /**
             * Nom du projet.
             */
            static constexpr std::string_view PROJECT_NAME = "dev4-stratego-pha-01";

            /**
             * Nom par défault pour le joueur rouge.
             */
            static constexpr std::string_view DEFAULT_NAME_RPLAYER = "JoueurRouge";

            /**
             * Nom par défault pour le joueur bleu.
             */
            static constexpr std::string_view DEFAULT_NAME_BPLAYER = "JoueurBleu";

            /**
             * Chemin relatif vers les ressources du projet.
             */
            static std::string RESOURCES_PATH;

            /**
             * Chemin relatif vers les fichiers de configurations du projet.
             */
            static std::string CONFIG_PATH;

            /**
             * Chemin relatif vers les fichiers de configurations de plateau de jeu utilisés par
             * l'application.
             */
            static std::string BOARD_CONFIG_PATH;

            /**
             * Chemin relatif vers les images utilisées par le projet.
             */
            static std::string IMAGE_PATH;

            /**
             * Données propres aux pions.
             */
            static Properties PIECE_DATA;

            /**
             * Données propres aux actions réalisées par les utilisateurs.
             */
            static Properties ACTION_DATA;

            /**
             * Chemin relatif vers l'exécutable.
             */
            static std::string EXEC_PATH;

            /**
             * Maximum d'allers retours vers une même case qu'un pion peut réaliser.
             */
            static constexpr int MAX_BNF = 3;

            /**
             * Initialise les ressources utilisées par le programme (_PATH et _DATA typiquement).
             *
             * @param pathToExec chemin vers l'exécutable
             */
            static void setDynamicResources(const std::string& pathToExec);
    };
};

#endif

