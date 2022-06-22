#ifndef MODEL_H
#define MODEL_H

/* ==========================================
 * Regroupe la partie modèle de l'application
 * ==========================================
 */

#include "gamestuff.h"

namespace stratego{

    /**
     * Modèle observable (et observer) du jeu s'occupant de la logique des actions performés.
     */
    class Model : public Observable, public Observer{

        public:

            // ==========================================
            // --- Méthode modifiant l'état du modèle ---
            // ==========================================

            /**
             * Initialise une nouvelle partie de jeu. L'état passe de NOT_STARTED à SET_UP via
             * l'événement INI.
             *
             * L'ensemble des attributs propre à la nouvel partie de jeu sur le point de commencé sont
             * initalisés dans cette méthode.
             *
             * @throw std::logic_error si l'état courant du modèle l'empêche de consumer l'événement INI.
             */
            virtual void init() = 0;

            /**
             * Charge un fichier de configuration de plateau de jeu pour un joueur de couleur donné. L'état
             * reste à SET_UP en cas de succès via l'événement LOAD mais passera de SET_UP à ERROR_SETUP
             * via l'événement FLOAD si le chargement du fichier pose probléme.
             *
             * @throw std::logic_error si l'état courant du modèle l'empêche de consumer l'événement LOAD ou FLOAD.
             *
             * @param filename le nom du fichier à charger
             * @param color la couleur du joueur pour lequel charger le fichier
             * @param isPathAbsolute flag pour déterminer si le nom du fichier donné représente un chemin
             * absolu
             */
            virtual void load(const std::string& filename, model::Color color, bool isPathAbsolute = false) = 0;

            /**
             * Termine la configuration d'une partie de jeu en instançiant les deux joueurs. L'état passe de
             * SET_UP à PLAYER_SWAP via l'événement SET.
             *
             * @throw std::logic_error si l'état courant du modèle l'empêche de consumer l'événement SET.
             *
             * @param redPseudo le pseudo du joueur rouge
             * @param bluePseudo le pseudo du joueur bleu
             */
            virtual void setup(const std::string& redPseudo, const std::string& bluePseudo) = 0;

            /**
             * Déplace le pion se trouvant à la position de départ donnée vers la position de fin donnée. L'état
             * passe, en cas de succès, de PLAYER_TURN à GAME_TURN via l'événement ACT, ou passe, en cas d'échec,
             * de l'état PLAYER_TURN à ERROR_ACTION via l'événement FACT.
             *
             * @throw std::logic_error si l'état courant du modèle l'empêche de consumer l'événement ACT ou FACT.
             *
             * @param startPos la position de départ du pion à déplacer
             * @param endPos la position de fin du pion à déplacer
             */
            virtual void move(const model::Position& startPos, const model::Position& endPos) = 0;

            /**
             * Attaque le pion qui se trouve en position de fin donnée en partant de position de départ
             * donnée. L'état passe de PLAYER_TURN à GAME_TURN via l'événement ACT, ou passe, en cas d'échec,
             * de l'état PLAYER_TURN à ERROR_ACTION via l'événement FACT.
             *
             * @throw std::logic_error si l'état courant du modèle l'empêche de consumer l'événement ACT ou FACT.
             *
             * @param startPos la position du pion attaquant
             * @param endPos la position du pion attaqué
             */
            virtual void attack(const model::Position& startPos, const model::Position& endPos) = 0;

            /**
             * Déplace le pion se trouvant à la position de départ donnée vers la position de fin donnée ou attaque
             * le pion qui se trouve en position de fin donnée en partant de position de départ
             * donnée (en fonction de la situation). Si un pion se trouve sur la position de fin donnée, alors
             * cette méthode choisira l'attaque, si non, le mouvement sera choisit.
             *
             * L'état passe de PLAYER_TURN à GAME_TURN via l'événement ACT, ou passe, en cas d'échec, de l'état
             * PLAYER_TURN à ERROR_ACTION via l'événement FACT.
             *
             * @throw std::logic_error si l'état courant du modèle l'empêche de consumer l'événement ACT ou FACT.
             *
             * @param startPos la position de départ du pion
             * @param endPos la position de fin du pion
             */
            virtual void moveAttack(const model::Position& startPos, const model::Position& endPos) = 0;

            /**
             * Vérifie si la partie de jeu courante est terminée. L'état passe de GAME_TURN à GAME_OVER via
             * l'événement CHK si la partie de jeu courante est terminée, ou passse de GAME_TURN à PLAYER_SWAP
             * via l'événement FCHK dans le cas contraire.
             *
             * @throw std::logic_error si l'état courant du modèle l'empêche de consumer l'événement CHK ou FCHK.
             */
            virtual void nextTurn() = 0;

            /**
             * Passe au joueur suivant. L'état passe de PLAYER_SWAP à PLAYER_TURN via l'événement NEXT.
             *
             * @throw std::logic_error si l'état courant du modèle l'empêche de consumer l'événement NEXT.
             */
            virtual void nextPlayer() = 0;

            /**
             * Met fin à la partie de jeu courante. L'état passe de PLAYER_TURN à EOG via
             * l'événement STOP.
             *
             * @throw std::logic_error si l'état courant du modèle l'empêche de consumer l'événement STOP.
             */
            virtual void stop() = 0;

            /**
             * Indique que la dernière erreur qui s'est produit dans le jeu à été traité. L'état passe de
             * ERROR_ACTION à PLAYER_TURN via l'événement ERRCS.
             *
             * @throw std::logic_error si l'état courant du modèle l'empêche de consumer l'événement ERRCS.
             */
            virtual void errorProcessed() = 0;

            /**
             * Rejoue une partie en fonction de l'état passé en paramètre. L'état passe de GAME_OVER à NOT_STARTED si les
             * joueurs veulent recommencer une nouvelle partie via l'événement RWD, ou passe de l'état GAME_OVER à EOG
             * via l'événement END dans le cas contraire.
             *
             * @throw std::logic_error si l'état courant du modèle l'empêche de consumer l'événement RWD ou END.
             *
             * @param state l'état de la décision des joueurs à vouloir rejouer
             */
            virtual void replay(bool state) = 0;




            // ========================================================
            // --- Methods to retrieve informations about the model ---
            // ========================================================

            /**
             * Récupère une référence vers la joueur courant.
             *
             * @throw std::logic_error si l'état courant du modèle lui permet de consumer l'événement INI, LOAD ou SET.
             *
             * @return le joueur courant.
             */
            virtual model::Player& currentPlayer() = 0;

            /**
             * Récupère une référence vers la joueur courant (version read-only).
             *
             * @throw std::logic_error si l'état courant du modèle lui permet de consumer l'événement INI, LOAD ou SET.
             *
             * @return le joueur courant.
             */
            virtual const model::Player& currentPlayer() const = 0;

            /**
             * Récupère l'état courant du modèle.
             *
             * @return l'état courant du modèle.
             */
            virtual model::StateGraph::State currentState() const = 0;

            /**
             * Récupère une référence vers le plateau de jeu utilisé par le modèle.
             *
             * @throw std::logic_error si l'état courant du modèle lui permet de consumer l'événement INI.
             *
             * @return le plateau de jeu.
             */
            virtual model::Board& board() = 0;

            /**
             * Récupère une référence vers le plateau de jeu utilisé par le modèle (version read-only).
             *
             * @return le plateau de jeu.
             */
            virtual const model::Board& board() const noexcept = 0;

            /**
             * Récupère une référence vers l'historique de jeu utilisé par le modèle.
             *
             * @throw std::logic_error si l'état courant du modèle lui permet de consumer l'événement INI.
             *
             * @return l'historique de jeu.
             */
            virtual model::History& history() = 0;

            /**
             * Récupère une référence vers l'historique de jeu utilisé par le modèle (version read-only).
             *
             * @throw std::logic_error si l'état courant du modèle lui permet de consumer l'événement INI.
             *
             * @return l'historique de jeu.
             */
            virtual const model::History& history() const = 0;

            /**
             * Récupère les joueurs de la partie courante.
             *
             * @throw std::logic_error si l'état courant du modèle lui permet de consumer l'événement INI, LOAD ou SET.
             *
             * @return un tableau contenant les joueurs de la partie courante.
             */
            virtual std::array<model::Player*, stratego::Config::PLAYER_COUNT>& players() = 0;

            /**
             * Récupère les joueurs de la partie courante (version read-only).
             *
             * @throw std::logic_error si l'état courant du modèle lui permet de consumer l'événement INI, LOAD ou SET.
             *
             * @return un tableau contenant les joueurs de la partie courante.
             */
            virtual const std::array<model::Player*, stratego::Config::PLAYER_COUNT>& players() const = 0;

            /**
             * Récupère un vecteur de pions à disposer sur la plateau de jeu pour un joueur
             * de couleur donné.
             *
             * @throw std::logic_error si l'état courant du modèle ne lui permet pas de consommer
             * l'événement LOAD.
             *
             * @param color la couleur des pions
             * @return un vecteur de pions à disposer.
             */
            virtual std::vector<model::Piece*> piecesOf(model::Color color) = 0; // pour GUI

            /**
             * Récupère l'ensemble des pions qui ont été retiré du plateau de jeu.
             *
             * @return un vecteur de pions retirés du plateau de jeu.
             */
            virtual const std::vector<model::Piece*>& removedPieces() const noexcept = 0;

            /**
             * Vérifie si le joueur de couleur donnée dispose d'un plateau de jeu
             * valide en début de partie.
             *
             * @param color la couleur du joueur
             * @return true si le joueur possède un plateau de jeu valide, faux si non.
             */
            virtual bool playerCanMove_startGame(model::Color color) const noexcept = 0;

            /**
             * Destructeur virtuel de Model.
             */
            virtual ~Model(){};
    };

    /**
     * Adapteur du modèle de jeu sur base duquel d'autres mode de jeu peuvent
     * se construire (seule les méthode move(), attack() et moveAttack()) ne sont pas implémenter
     * par cet adapteur.
     */
    class ModelAdapter : public Model{

        std::vector<Observer*> observers_;
        std::vector<model::Piece*> removedPieces_;

        protected:

            std::array<model::Player*, Config::PLAYER_COUNT> players_;
            int playerPointer_;
            model::Board board_;
            model::History history_;
            model::StateGraph graph_;

        public:

            /**
             * Construit un modèle de jeu.
             */
            ModelAdapter();


            // --- Déjà documenté ---
            void init() override;
            void load(const std::string& filename, model::Color color, bool isPathAbsolute = false) override;
            void setup(const std::string& redInfo, const std::string& blueInfo) override;
            void nextTurn() override;
            void nextPlayer() override;
            void stop() override;
            void errorProcessed() override;
            void replay(bool state) override;


            // --- Déjà documenté ---
            model::Player& currentPlayer() override;
            const model::Player& currentPlayer() const override;
            std::array<model::Player*, stratego::Config::PLAYER_COUNT>& players() override;
            const std::array<model::Player*, stratego::Config::PLAYER_COUNT>& players() const override;
            model::StateGraph::State currentState() const override;
            model::Board& board() override;
            const model::Board& board() const noexcept override;
            model::History& history() override;
            const model::History& history() const override;
            std::vector<model::Piece*> piecesOf(model::Color color) override;
            const std::vector<model::Piece*>& removedPieces() const noexcept override;
            bool playerCanMove_startGame(model::Color color) const noexcept override;


            // --- Déjà documenté ---
            void addObserver(Observer* obs) noexcept override;
            void removeObserver(Observer* obs) noexcept override;
            void notifyObservers(std::initializer_list<Observable*> infos) const noexcept override;
            void update(std::initializer_list<Observable*> args) override;

            /**
             * Destructeur virtuel de ModelAdapter. Supprime les joueurs
             * uniquement.
             */
            virtual ~ModelAdapter();

        private:

            void parseFor(const std::string& filename, model::Color color, bool isPathAbsolute);
            model::Piece* toPiece(int rank, model::Color color);
            bool playerCanMove(model::Color color) noexcept;
            bool pieceCanMove(const model::Piece* piece) const noexcept;
    };

    /**
     * Modèle de jeu pour jouer à la version classique de Stratego.
     */
    class Stratego : public ModelAdapter{

        public:

            /**
             * Construit un modèle de jeu classique.
             */
            Stratego();


            // --- Déjà documenté ---
            void move(const model::Position& startPos, const model::Position& endPos) override;
            void attack(const model::Position& startPos, const model::Position& endPos) override;
            void moveAttack(const model::Position& startPos, const model::Position& endPos) override;
    };

    /**
     * Modèle de jeu pour joueur à la variante Reveal de Stratego. Dans cette version,
     * l'attaque d'un pion de force supérieur révèlera son identité jusqu'à la fin
     * de la partie. Une fois l'attaque terminé le pion attaqué restera visible pour les deux
     * joueurs.
     */
    class StrategoReveal : public ModelAdapter{

        public:

            /**
             * Construit une variante du modèle de jeu.
             */
            StrategoReveal();


            // --- Déjà documenté ---
            void move(const model::Position& startPos, const model::Position& endPos) override;
            void attack(const model::Position& startPos, const model::Position& endPos) override;
            void moveAttack(const model::Position& startPos, const model::Position& endPos) override;
            void nextPlayer() override;
    };
};


#endif

