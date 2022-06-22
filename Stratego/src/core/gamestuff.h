#ifndef GAMESTUFF_H
#define GAMESTUFF_H

/* ============================================================
 * Regroupe l'ensemble des classes représentant les composants 
 * du jeu (pièce de jeu, plateau de jeu, joueur, ...).
 * ============================================================
 */

#include <fstream>
#include <ctime>
#include <functional>

#include "config.h"
#include "designpatt.h"
#include "eventMgr.h"
#include "properties.h"

namespace stratego::model{

    /**
     * Regroupe l'ensemble des couleurs disponible
     * pour jouer au jeu.
     */
    enum class Color : char{

        /**
         * La couleur rouge.
         */
        RED,

        /**
         * La couleur bleu.
         */
        BLUE
    };

    /**
     * Position de déplacement à deux dimensions.
     */
    struct Position{
        int x, y;

        /**
         * Surchage d'opérateur de conversion permettant
         * à une Position d'être convertit explicitement
         * ou implicitement en string.
         */
        operator std::string() const;

        /**
         * Convertit la chaîne de caractères donné en
         * Position. Le format de conversion utilisé est [1-10][a-j]. Ce format
         * reste insensible à la casse.
         *
         * @throw std::invalid_argument si la chaîne de caractères donné
         * ne peut être convertit en Position.
         *
         * @param str la chaîne de caractères à convertir
         * @return une Position équivalente à la chaîne de caractère donné.
         */
        static Position from(const std::string& str);
    };

    /**
     * Direction cardinal de déplacement.
     */
    class Direction{

        public:

            /**
             * Valeurs que peut prendre une direction.
             */
            enum Value{

                /**
                 * Vers le haut.
                 */
                UP,

                /**
                 * Vers le bas.
                 */
                DOWN,

                /**
                 * Vers la gauche.
                 */
                LEFT,

                /**
                 * Vers la droite.
                 */
                RIGHT
            };

            /**
             * Construit une position en utilisant la direction donnée.
             *
             * @param v la direction souhaitée
             */
            Direction(Value v) noexcept;

            /**
             * Surchage d'opérateur de direction en sa valeur d'énumeration correspondante.
             * Cette surcharge d'opérateur permet à la classe Direction d'être contextuellement
             * implicitement convertible en type d'énumeration, lui permettant d'être utilisé dans
             * un switch.
             */
            constexpr operator Value() const {return v_;}

            /**
             * Convertit la chaîne de caractères donné en
             * Direction. Le format de conversion utilisé est (up|down|left|right). Ce format
             * reste insensible à la casse.
             *
             * @throw std::invalid_argument si la chaîne de caractères donné
             * ne peut être convertit en Direction.
             *
             * @param str la chaîne de caractères à convertir
             * @return une Direction équivalente à la chaîne de caractère donné.
             */
            static Direction from(const std::string& str);

        private:

            Value v_;
    };

    class Piece;

    /**
     * Cellule du plateau de jeu identifée par une
     * position, un type et un pion se trouvant sur
     * la cellule.
     */
    struct Cell{

        /**
         * Regroupe l'ensemble des types de cellules
         * existant sur un plateau de jeu.
         */
        enum Type : char{

            /**
             * Cellule normale sur laquelle un pion peut se déplacer.
             */
            NORMAL,

            /**
             * Cellule représentant un mur sur laquelle aucun pion ne peut se
             * déplacer.
             */
            WALL,

            /**
             * Cellule représentant de l'eau sur laquelle, comme pour un mur,
             * aucun pion ne peut se déplacer.
             */
            WATER
        };

        static const std::array<std::string, 3> typeNames;

        Position pos;
        Type type;
        Piece* piece;
    };

    /**
     * Chaîne de caractère temporel.
     */
    struct tmstring{
        std::string info;
        std::tm time;

        /**
         * Construit une chaîne de caractère temporel via l'information donnée.
         *
         * @param information la chaîne de caractère à temporaliser
         */
        tmstring(const std::string& information) noexcept;

        /**
         * Convertit la temporalité de la chaîne de caractère en un format lisible.
         *
         * @return une chaîne de caractère contenant la temporalité de la chaîne de caractère
         * en un format lisible.
         */
        std::string toReadableTime() const noexcept;
    };

    /**
     * Plateau de jeu sur lequel se déroule la bataille.
     */
    class Board{

        std::array<std::array<Cell, Config::BOARD_SIZE>, Config::BOARD_SIZE> board_;

        public:

            Board(const Board& board) = delete;
            Board(Board&& board) = delete;
            Board& operator=(const Board& board) = delete;
            Board& operator=(Board&& board) = delete;

            /**
             * Construit un plateau de jeu à deux dimensions de taille Config::BOARD_SIZE.
             */
            Board() noexcept;

            /**
             * Récupère le pion se trouvant à la position donnée.
             *
             * @throw std::out_of_range si la position indiquée se trouve en dehors du plateau
             *
             * @param x la coordonnée x du pion à récupérer
             * @param y la coordonnée y du pion à récupérer
             * @return le pion se trouvant à la position donnée ou nullptr si aucun pion ne se trouve
             * à cette position.
             */
            Piece* getPiece(int x, int y);

            /**
             * Récupère le pion se trouvant à la position donnée (version read-only).
             *
             * @throw std::out_of_range si la position indiquée se trouve en dehors du plateau
             *
             * @param x la coordonnée x du pion à récupérer
             * @param y la coordonnée y du pion à récupérer
             * @return le pion se trouvant à la position donnée ou nullptr si aucun pion ne se trouve
             * à cette position.
             */
            const Piece* getPiece(int x, int y) const;

            /**
             * Récupère le pion se trouvant à la position donnée.
             *
             * @throw std::out_of_range si la position indiquée se trouve en dehors du plateau
             *
             * @param pos la position du pion à récupérer.
             * @return le pion se trouvant à la position donnée ou nullptr si aucun pion ne se trouve
             * à cette position.
             */
            Piece* getPiece(const Position& pos);

            /**
             * Récupère le pion se trouvant à la position donnée (version read-only).
             *
             * @throw std::out_of_range si la position indiquée se trouve en dehors du plateau
             *
             * @param pos la position du pion à récupérer.
             * @return le pion se trouvant à la position donnée ou nullptr si aucun pion ne se trouve
             * à cette position.
             */
            const Piece* getPiece(const Position& pos) const;

            /**
             * Récupère une référence vers la cellule se trouvant à la position donnée.
             *
             * @throw std::out_of_range si la position indiquée se trouve en dehors du plateau
             *
             * @param x la coordonnée x de la cellule a récupérer
             * @param y la coordonnée y de la cellule a récupérer
             * @return la cellule se trouvant à la position donnée.
             */
            Cell& getCell(int x, int y);

            /**
             * Récupère une référence vers la cellule se trouvant à la position donnée (version read-only).
             *
             * @throw std::out_of_range si la position indiquée se trouve en dehors du plateau
             *
             * @param x la coordonnée x de la cellule a récupérer
             * @param y la coordonnée y de la cellule a récupérer
             * @return la cellule se trouvant à la position donnée.
             */
            const Cell& getCell(int x, int y) const;

            /**
             * Récupère une référence vers la cellule se trouvant à la position donnée.
             *
             * @throw std::out_of_range si la position indiquée se trouve en dehors du plateau
             *
             * @param pos la position de la cellule a récupérer
             * @return la cellule se trouvant à la position donnée.
             */
            Cell& getCell(const Position& pos);

            /**
             * Récupère une référence vers la cellule se trouvant à la position donnée (version read-only).
             *
             * @throw std::out_of_range si la position indiquée se trouve en dehors du plateau
             *
             * @param pos la position de la cellule a récupérer
             * @return la cellule se trouvant à la position donnée.
             */
            const Cell& getCell(const Position& pos) const;

            /**
             * Vérifie s'il est possible de se déplacer sur la cellule se trouvant
             * à la position donnée.
             *
             * @throw std::out_of_range si la position indiquée se trouve en dehors du plateau
             *
             * @param pos la position de la cellule à vérifier
             * @return true s'il est possible de se déplacer à la position donnée, false si non.
             */
            bool walkableCell(const Position& pos) const;

            /**
             * Vérifie s'il est possible de se déplacer sur la cellule se trouvant
             * à la position donnée.
             *
             * @throw std::out_of_range si la position indiquée se trouve en dehors du plateau
             *
             * @param x la coordonnée x de la cellule à verifier
             * @param y la coordonnée y de la cellule à verifier
             * @return true s'il est possible de se déplacer à la position donnée, false si non.
             */
            bool walkableCell(int x, int y) const;

            /**
             * Détermine si la position donnée est dans le plateau.
             *
             * @param pos la position de la cellule à vérifier
             * @return true si la position se trouve sur le plateau de jeu, false si non.
             */
            bool isInside(const Position& pos) const noexcept;

            /**
             * Récupère la taille du plateau de jeu.
             *
             * @param la taille du plateau de jeu.
             */
            int size() const noexcept;

            /**
             * Retourne un pointeur constant vers la première ligne du plateau de jeu.
             *
             * @return un pointeur constant vers la première ligne du plateau de jeu.
             */
            const std::array<Cell, Config::BOARD_SIZE>* begin() const;

            /**
             * Retourne un pointeur constant vers la dernière ligne du plateau de jeu.
             *
             * @return un pointeur constant vers la dernière ligne du plateau de jeu.
             */
            const std::array<Cell, Config::BOARD_SIZE>* end() const;

            /**
             * Déstructeur du plateau de jeu. Détruit tout les pions
             * se trouvant sur le plateau de jeu.
             */
            ~Board();
    };

    /**
     * Historique d'informations de jeu.
     */
    class History{

        public:

            /**
             * Le type d'informations possible identifiant une information
             * ajoutée à l'historique.
             */
            enum InfoType{

                /**
                 * Un succès.
                 */
                SUCCESS,

                /**
                 * Un échec.
                 */
                FAILURE,

                /**
                 * Un conseil.
                 */
                HINT
            };


            /**
             * Construit un historique de jeu de taille limite
             * donnée.
             *
             * @param bound la taille maximum de l'historique
             */
            History(int bound) noexcept;

            /**
             * Ajoute l'information donnée de type donné à l'historique.
             *
             * @throw std::out_of_range si il n'y a plus assez de place dans l'historique.
             *
             * @param type le type de l'information
             * @param info l'info à ajouter
             */
            void addInfo(InfoType type, const std::string& info);

            /**
             * Ajoute le succès à l'historique.
             *
             * @throw std::out_of_range si il n'y a plus assez de place dans l'historique.
             *
             * @param info l'info à ajouter
             */
            void addSuccess(const std::string& info);

            /**
             * Ajoute l'échec à l'historique.
             *
             * @throw std::out_of_range si il n'y a plus assez de place dans l'historique.
             *
             * @param info l'info à ajouter
             */
            void addFailure(const std::string& info);

            /**
             * Ajoute le conseil à l'historique.
             *
             * @throw std::out_of_range si il n'y a plus assez de place dans l'historique.
             *
             * @param info l'info à ajouter
             */
            void addHint(const std::string& info);

            /**
             * Récupère la dernière information de type donné.
             *
             * @throw std::invalid_argument si aucune information n'est associé avec
             * le type d'info donné.
             *
             * @param type le type d'information à récupérer
             * @return la dernière information de type donné.
             */
            std::string& lastInfo(InfoType type);

            /**
             * Récupère la dernière information de type donné (version read-only).
             *
             * @throw std::invalid_argument si aucune information n'est associé avec
             * le type d'info donné.
             *
             * @param type le type d'information à récupérer
             * @return la dernière information de type donné.
             */
            const std::string& lastInfo(InfoType type) const;

            /**
             * Récupère le dernier succès ajoutée à l'historique.
             *
             * @throw std::logic_error si aucun succès n'a été rajouté
             *
             * @return la dernier succès ajoutée à l'historique.
             */
            std::string& lastSuccess();

            /**
             * Récupère le dernier succès ajoutée à l'historique (version read-only).
             *
             * @throw std::logic_error si aucun succès n'a été rajouté
             *
             * @return la dernier succès ajoutée à l'historique.
             */
            const std::string& lastSuccess() const;

            /**
             * Récupère le dernier échec ajoutée à l'historique.
             *
             * @throw std::logic_error si aucun échec n'a été rajouté
             *
             * @return la dernier échec ajoutée à l'historique.
             */
            std::string& lastFailure();

            /**
             * Récupère le dernier échec ajoutée à l'historique (version read-only).
             *
             * @throw std::logic_error si aucun échec n'a été rajouté
             *
             * @return la dernier échec ajoutée à l'historique.
             */
            const std::string& lastFailure() const;

            /**
             * Récupère le dernier conseil ajoutée à l'historique.
             *
             * @throw std::logic_error si aucun conseil n'a été rajouté
             *
             * @return la dernier conseil ajoutée à l'historique.
             */
            std::string& lastHint();

            /**
             * Récupère le dernier conseil ajoutée à l'historique (version read-only).
             *
             * @throw std::logic_error si aucun conseil n'a été rajouté
             *
             * @return la dernier conseil ajoutée à l'historique.
             */
            const std::string& lastHint() const;

            /**
             * Récupère tous les succès de l'historique.
             *
             * @return l'ensemble des succès de l'historique.
             */
            std::vector<tmstring>& allSuccess() noexcept;

            /**
             * Récupère tous les succès de l'historique (version read-only).
             *
             * @return l'ensemble des succès de l'historique.
             */
            const std::vector<tmstring>& allSuccess() const noexcept;

            /**
             * Récupère tous les échecs de l'historique.
             *
             * @return l'ensemble des échecs de l'historique.
             */
            std::vector<tmstring>& allFailure() noexcept;

            /**
             * Récupère tous les échecs de l'historique (version read-only).
             *
             * @return l'ensemble des échecs de l'historique.
             */
            const std::vector<tmstring>& allFailure() const noexcept;

            /**
             * Récupère tous les conseils de l'historique.
             *
             * @return l'ensemble des conseils de l'historique.
             */
            std::vector<tmstring>& allHint() noexcept;

            /**
             * Récupère tous les conseils de l'historique (version read-only).
             *
             * @return l'ensemble des conseils de l'historique.
             */
            const std::vector<tmstring>& allHint() const noexcept;

            /**
             * Récupère toutes les infos de type donné de l'historique.
             *
             * @return l'ensemble des infos de type donné de l'historique.
             */
            std::vector<tmstring>& allInfo(InfoType type) noexcept;

            /**
             * Récupère toutes les infos de type donné de l'historique (version read-only).
             *
             * @return l'ensemble des infos de type donné de l'historique.
             */
            const std::vector<tmstring>& allInfo(InfoType type) const noexcept;


            /**
             * Supprime la dernière information de type donné.
             *
             * @throws std::logic_error s'il ne reste plus d'infos associés au type
             * d'info donné
             *
             * @param type le type d'information à supprimer
             * @return l'information supprimée.
             */
            std::string popInfo(InfoType type);

            /**
             * Supprime le dernier succès de l'historique.
             *
             * @throw std::logic_error s'il ne reste plus de succès
             *
             * @return le succès supprimé.
             */
            std::string popSuccess();

            /**
             * Supprime le dernier échec de l'historique.
             *
             * @throw std::logic_error s'il ne reste plus d'échecs
             *
             * @return l'échec supprimé.
             */
            std::string popFailure();

            /**
             * Supprime le dernier conseil de l'historique.
             *
             * @throw std::logic_error s'il ne reste plus de conseils
             *
             * @return le conseil supprimé.
             */
            std::string popHint();

            /**
             * Supprime l'ensemble des informations présentes dans l'historique.
             */
            void clear() noexcept;

            /**
             * Supprime l'ensemble des informations de type donné.
             *
             * @param type le type d'information à supprimer
             */
            void clear(InfoType type) noexcept;

            /**
             * Supprime l'ensemble des succès.
             */
            void clearSuccess() noexcept;

            /**
             * Supprime l'ensemble des échecs.
             */
            void clearFailure() noexcept;

            /**
             * Supprime l'ensemble des conseils.
             */
            void clearHint() noexcept;

            /**
             * Sauvegarde l'ensemble des informations de type donné dans le fichier
             * identifié par le nom donné. Si le fichier identifié par le nom donné
             * n'existe pas, ce-dernier sera créé.
             *
             * @param type le type d'informations à sauvegarder
             * @param filename le nom du fichier dans lequel sauvegarder l'historique
             */
            void saveToFile(InfoType type, const std::string& filename) const noexcept;

            /**
             * Sauvegarde l'ensemble des succès de l'historique dans le fichier
             * identifié par le nom donné. Si le fichier identifié par le nom donné
             * n'existe pas, ce-dernier sera créé.
             *
             * @param filename le nom du fichier dans lequel sauvegarder l'historique
             */
            void saveSuccess(const std::string& filename) const noexcept;

            /**
             * Sauvegarde l'ensemble des échecs de l'historique dans le fichier
             * identifié par le nom donné. Si le fichier identifié par le nom donné
             * n'existe pas, ce-dernier sera créé.
             *
             * @param filename le nom du fichier dans lequel sauvegarder l'historique
             */
            void saveFailure(const std::string& filename) const noexcept;

            /**
             * Sauvegarde l'ensemble des conseils de l'historique dans le fichier
             * identifié par le nom donné. Si le fichier identifié par le nom donné
             * n'existe pas, ce-dernier sera créé.
             *
             * @param filename le nom du fichier dans lequel sauvegarder l'historique
             */
            void saveHint(const std::string& filename) const noexcept;

            /**
             * Récupère la taille maximum de l'historique.
             *
             * @return le taille maximum de l'historique.
             */
            int bound() const noexcept;

            /**
             * Récupère la taille courante de l'historique.
             *
             * @return la taille courante de l'historique.
             */
            int size() const noexcept;

            /**
             * Vérifie si l'historique est complet.
             *
             * @return true si l'historique est complet, false si non.
             */
            bool isFull() const noexcept;

        private:

            std::map<InfoType, std::vector<tmstring>> history_;
            int counter_;
            const int bound_;
    };

    /**
     * Pion de jeu observable que les joueurs manipulent.
     */
    class Piece : public Observable{

        std::vector<Observer*> observers_;
        Color color_;
        PieceInfo info_;
        bool revealed_;
        std::function<bool(int, int)> winPredicate_;

        protected:

            Position currentPos_;
            StateGraph& graph_;
            History& hist_;
            Board& board_;
            bool alive_;
            bool isMovable_;
            bool hasBeenInCombat_;
            Position recordedPos_;
            int bnfCounter_;
            bool hasMove_;

        public:

            /**
             * Tableau d'informations permettant de récupérer les informations propre à un pion de rang donné.
             */
            static const std::array<PieceInfo, Config::PIECE_BOMB_INFO.rank + 1> pieceInfo;

            /**
             * Récupère le rang sous forme d'entier d'une chaîne de caractères donné.
             *
             * @throw std::invalid_argument si la chaîne donné ne peut être convertit en rang.
             *
             * @param str la chaîne de caractères à convertir
             * @return le rang correspondant à la chaîne de caractères donné.
             */
            static int toRank(const std::string& str);

            /**
             * Récupère la description d'un pion de rang donné.
             *
             * @throw std::invalid_argument si la rang donné n'est pas valide
             *
             * @param rank le rang du pion
             * @return la description du pion de rang donné.
             */
            static std::string description(int rank);

            /**
             * Construit un pion d'un certain rang et couleur se trouvant à une position initale donnée. Chaque
             * pion possède également une référence vers le plateau de jeu et le graphe d'état provisionnés par le
             * modèle pour pouvoir se déplacer correctement et changer l'état du modèle en fonction de l'action
             * performée (attaque ou déplacement). De plus, chaque pion possédera un nom unique, une référence vers
             * l'historique utilisé par le modèle et un prédicat de victoire pour les combats.
             *
             * @param info information à propos du pion
             * @param initPos la position initiale du pion
             * @param color la couleur du pion
             * @param winPredicate le prédicat de victoire
             * @param board la référence vers le plateau de jeu sur lequel se trouve le pion
             * @param graph la référence vers le graphe d'état utilisé par le modèle
             * @param hist la référence vers l'historique utilisé par le modèle
             */
            Piece(const PieceInfo& info, const Position& initPos, Color color, std::function<bool(int, int)> winPredicate, Board& board, StateGraph& graph, History& hist) noexcept;

            /**
             * Déplace le pion à la position donnée.
             *
             * @param pos la position à laquelle déplacer le pion
             */
            virtual void move(const Position& pos) noexcept;

            /**
             * Attaque le pion se trouvant à la position donnée.
             *
             * @param pos la position du pion à attaqué
             */
            virtual void attack(const Position& pos) noexcept;

            /**
             * Vérifie si le pion peut se déplacer à la position donnée.
             *
             * @param pos la position à vérifier pour un déplacement
             * @return true si le pion peut se déplacer à la position donnée, false si non.
             */
            virtual bool canMove(const Position& pos) const noexcept = 0;

            /**
             * Vérifie si le pion peut attaquer un pion à la position donnée.
             *
             * @param pos la position à vérifier pour une attaque
             * @return true si le pion peut attaquer à la position donnée, false si non.
             */
            virtual bool canAttack(const Position& pos) const noexcept = 0;

            /**
             * Récupère le rang du pion.
             *
             * @return le rang du pion.
             */
            int rank() const noexcept;

            /**
             * Récupère une référence vers la position à laquelle se trouve le pion.
             *
             * @return la position à laquelle se trouve le pion.
             */
            Position& position() noexcept;

            /**
             * Récupère une référence vers la position à laquelle se trouve le pion (version read-only).
             *
             * @return la position à laquelle se trouve le pion.
             */
            const Position& position() const noexcept;

            /**
             * Déplace le pion à la position donné.
             *
             * @param pos la position à laquell déplacer le pion
             */
            void setPosition(const Position& pos) noexcept; // for GUI on setup

            /**
             * Récupère la couleur du pion.
             *
             * @return la couleur du pion.
             */
            Color color() const noexcept;

            /**
             * Récupère des informations à propos du pion.
             *
             * @return informations à propos du pion.
             */
            PieceInfo info() const noexcept;

            /**
             * Récupère l'état de vie du pion.
             *
             * @return true si le pion est encore en vie, false si non.
             */
            bool alive() const noexcept;

            /**
             * Détermine si le pion a été révélé.
             *
             * @return true si le pion a été révélé, false si non.
             */
            bool revealed() const noexcept;

            /**
             * Change l'état de révélation du pion via le paramètre donné.
             *
             * @param state le nouvel état de révélation
             */
            void setRevealed(bool state) noexcept;

            /**
             * Récupère une référence vers le nom du pion.
             *
             * @return le nom du pion.
             */
            std::string_view& name() noexcept;

            /**
             * Récupère une référence vers le nom du pion (version read-only).
             *
             * @return le nom du pion.
             */
            const std::string_view& name() const noexcept;

            /**
             * Récupère la description du pion.
             *
             * @return la description du pion.
             */
            std::string description() const noexcept;


            /**
             * Vérifie si le pion est déplaçable.
             *
             * @return true si le pion peut se déplacer, false si non.
             */
            bool isMovable() const noexcept;

            /**
             * Vérifie si la dernière action réalisé par le pion, l'a fait bougé.
             *
             * @return true si la dernière action réalisé par le pion l'a fait bougé, false si non.
             */
            bool hasMove() const noexcept;

            /**
             * Vérifie si le pion a été impliqué au moins une fois dans un combat.
             *
             * @return true si le pion a été impliqué au moins une fois dans un combat, false si non.
             */
            bool hasBeenInCombat() const noexcept;

            /**
             * Réinitialise le compteur d'aller retours du pion.
             */
            void resetCounter() noexcept;

            /**
             * Surchage d'opérateur de conversion permettant
             * à une Pièce d'être convertit explicitement
             * ou implicitement en string.
             */
            operator std::string() const;


            // --- Déjà documenté ---
            void addObserver(Observer* obs) noexcept override;
            void removeObserver(Observer* obs) noexcept override;
            void notifyObservers(std::initializer_list<Observable*> infos) const noexcept override;
    };

    /**
     * Décrit les informations utilisées par un joueur.
     */
    struct PlayerInfo{
        std::string pseudo;
        Color color;
    };

    /**
     * Joueur de Stratego.
     */
    class Player : public Observer{

        std::string pseudo_;
        Color color_;
        int aliveCounter_;
        int eatenPiecesCounter_;
        std::map<int, int> armyStat_;
        std::map<int, int> battleStat_;
        Piece* lastMovedPiece_;

        public:

            /**
             * Construit un joueur identifié par un nom et une couleur.
             *
             * @param info les informations utilisés par le joueur
             */
            Player(const PlayerInfo& info) noexcept;

            /**
             * Vérifie si le joueur possède au moins un pion de rang donné.
             *
             * @param rank le rang du pion à vérifier
             * @return true si le joueur possède au moins un pion de rang donné, false si non.
             */
            bool hasPiece(int rank) const noexcept;

            /**
             * Détermine si le joueur a perdu. Un joueur aura perdu si il ne posséde plus
             * de drapeau ou si il ne lui reste que des pions ne pouvant pas se déplacer.
             *
             * @return true si le joueur a perdu, false si non.
             */
            bool hasLost() const noexcept;

            /**
             * Récupère le pseudo du joueur.
             *
             * @return le pseudo du joueur.
             */
            std::string& pseudo() noexcept;

            /**
             * Récupère le pseudo du joueur (version read-only).
             *
             * @return le pseudo du joueur.
             */
            const std::string& pseudo() const noexcept;

            /**
             * Récupère le nombre de pions qu'il reste au joueur.
             *
             * @return le nombre de pions qu'il reste au joueur.
             */
            int remainingPieces() const noexcept;

            /**
             * Récupère le nombre de pions de rang donné qu'il reste au joueur.
             *
             * @throw std::invalid_argument si le rang donné n'est pas valide
             *
             * @param rank le rang du pion
             * @return le nombre de pion de rang donné qu'il reste au joueur.
             */
            int remainingPieces(int rank) const;

            /**
             * Récupère le nombre de pions ennemis mangés.
             *
             * @return le nombre de pions mangés.
             */
            int eatenPieces() const noexcept;

            /**
             * Récupère le nombre de pions ennemis mangés possédant le rang indiqué.
             *
             * @throw std::invalid_argument si le rang donné n'est pas valide
             *
             * @param rank le rang du pion
             * @return le nombre de pions mangés du rang donné.
             */
            int eatenPieces(int rank) const;

            /**
             * Récupère la couleur du joueur.
             *
             * @return la couleur du joueur.
             */
            Color color() const noexcept;

            /**
             * Récupère une map retraçant les statistiques du joueur. Cette map
             * regroupe l'ensemble des pions du jeu par nom (clé) avec le nombre
             * de pions qu'il reste au joueur (valeur).
             *
             * @return une map retraçant les statistiques du joueur.
             */
            std::map<int, int>& stats() noexcept;

            /**
             * Récupère une map retraçant les statistiques du joueur. Cette map
             * regroupe l'ensemble des pions du jeu par nom (clé) avec le nombre
             * de pions qu'il reste au joueur (valeur) (version read-only).
             *
             * @return une map retraçant les statistiques du joueur.
             */
            const std::map<int, int>& stats() const noexcept;


            // --- Déjà documenté ---
            void update(std::initializer_list<Observable*> args) override;
    };

    /**
     * Informations utilisés pour un parsing
     * de fichier de configuration.
     */
    struct ParseInfo{
        Board& board;
        StateGraph& graph;
        History& hist;
        Color color;
    };

    /**
     * Parser pour les fichiers de configurations définis par les joueurs
     * pour initialiser leur disposition de plateau de jeu.
     */
    class ConfigFileParser : public Parser<std::vector<Piece*>>{

        public:

            /**
             * Construit un parser de fichier de configuration depuis son nom de fichier
             * donné en paramètre.
             *
             * @throw std::invalid_argument si le nom de fichier donné ne correspond pas à un fichier valide
             *
             * @param info les informations utilisées pour le parsing
             * @param filename le nom du fichier de configuration à parser
             */
            ConfigFileParser(ParseInfo& info, const std::string& filename);

            /**
             * Détermine s'il est possible de parser le fichier identifié par le stream
             * donné en paramètre indépendamment du Parser en lui même.
             *
             * @param ifs le stream de fichier à utilisé
             * @return true s'il est possible de parser le fichier sans erreur, false si non.
             */
            static bool canParse(std::ifstream& ifs);


            // --- Déjà documenté ---
            void parse() override;
            bool canParse() noexcept override;
            std::vector<Piece*>& result() noexcept override;

        private:

            std::ifstream file_;
            std::vector<Piece*> result_;
            ParseInfo& info_;

            Piece* toPiece(int rank, int x, int y) const;
    };

    /**
     * Vérifie si les deux positions sont équivalentes. Elles seront
     * équivalentes si elles possèdent les mêmes coordonnées x et y.
     *
     * @param pos1 la première position
     * @param pos2 la deuxième position
     *
     * @return true si les deux positions sont équivalentes, false si non.
     */
    bool operator==(const Position& pos1, const Position& pos2);

    /**
     * Vérifie si les deux cellules sont équivalentes. Elles seront
     * équivalentes si elles possèdent la même position, le même type et la
     * même référence de pion.
     *
     * @param cell1 la première cellule
     * @param cell2 la deuxième cellule
     *
     * @return true si les deux cellules sont équivalentes, false si non.
     */
    bool operator==(Cell& cell1, Cell& cell2);

    /**
     * Additionne deux positions. Somme les coordonnées x et y des deux
     * positions et retourne une nouvelle position.
     *
     * @param pos1 la première position
     * @param pos2 la deuxième position
     *
     * @return le résultat de la somme des deux positions.
     */
    Position operator+(const Position& pos1, const Position& pos2);

    /**
     * Additionne une position avec une direction. Une direction UP et DOWN
     * décremente et incrémente, respectivement, la coordonnée y de la position. De même
     * pour une direction LEFT et RIGHT qui affecte la coordonéé x.
     *
     * @param pos la position de départ
     * @param direction la direction à prendre
     *
     * @return une position qui repose sur celle de départ mais modifié par la direction prise.
     */
    Position operator+(const Position& pos, Direction::Value direction);
};


#endif

