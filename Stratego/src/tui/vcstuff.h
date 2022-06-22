#ifndef VCSTUFF_H
#define VCSTUFF_H

#include "model.h"
#include "action.h"

namespace stratego{

    class Controller;

    /**
     * Vue console observer pour l'application permettant de construire
     * le visuel du jeu.
     */
    class View : public stratego::Observer{

        const Model* model_;
        Controller& controller_;

        public:

            /**
             * Construit une vue console communiquant avec le contrôleur et utilisant
             * le modèle donné.
             *
             * @param model le modèle de jeu à utilisé
             * @param controller le contrôleur de jeu à utilisé
             */
            View(Model* model, Controller& controller);


            // --- Déjà documenté ---
            void update(std::initializer_list<Observable*> args) override;

            /**
             * Affiche l'introduction du jeu.
             */
            void displayIntro() const noexcept;

            /**
             * Affiche le plateau de jeu.
             */
            void displayBoard() const noexcept;

            /**
             * Affiche les pièces mangées pour le joueur courant.
             */
            void displayEatenPieces() const noexcept;

            /**
             * Traite la chaîne de caractère donné pour exécuter une action.
             *
             * @param input le chaîne de caractère à traiter.
             */
            void processAction(const std::string& input) noexcept;

        private:

            std::pair<model::Position, model::Position> processMoveAttackCommand(view::ActionMatcher& actionMatcher);
            int processPieceStatCommand(view::ActionMatcher& actionMatcher);

            void displayNormalCell(const model::Cell& cell, int width) const;
            void displayWallCell(int width) const;
            void displayWaterCell(int width) const;
            void displayEmptyRow(int leftoffs, int width) const;
            void displayBoardHead(int leftoffs, int cellwidth, int sepwidth) const ;
            void displayBoardOutline(int leftoffs, int cellwidth, int sepwidth) const;
    };

    /**
     * Contrôleur de jeu intéragissant avec les utilisateurs et servant d'intermédiaire
     * à la transmission d'infos entre la vue et le modèle.
     */
    class Controller{

        Model* model_;
        View view_;

        public:

            /**
             * Construit un contrôleur en utilisant le modèle donné. Une vue par défaut
             * lui sera également donné.
             *
             * @param model le modèle de jeu à utiliser
             */
            Controller(Model* model) noexcept;

            /**
             * Commence une nouvelle partie de jeu. Uniquement cette méthode devrait être invoquée
             * pour commencer une partie, toutes les autres seront implicitement invoquées lors de
             * l'exécution de l'application.
             */
            void start() noexcept;

            /**
             * Signale le modèle qu'une partie de jeu veut être lancée.
             */
            void init() noexcept;

            /**
             * Charge une partie de jeu. Le contrôleur demande un pseudo pour chaque joueur
             * et les fournis au modèle.
             */
            void load() noexcept;

            /**
             * Charge un fichier de configuration de plateau de jeu. Le contrôleur demande
             * un fichier de configuration de plateau de jeu pour le joueur de couleur donné et le
             * fournis au modèle.
             *
             * @param color la couleur du joueur pour qui chargé le fichier
             */
            void load(model::Color color) noexcept;

            /**
             * Exécute une action. Le contrôleur demande une action à réaliser au joueur courant et la
             * transmet à la vue pour qu'elle l'éxecute. L'exécution de cette action sur la vue pourra
             * provoquer un appel en chaîne d'autres méthodes de ce contrôleur (comme move() et attack())
             * impliquant une transmission d'infos au modèle.
             */
            void processAction() noexcept;

            /**
             * Signale le modèle qu'un déplacement veut être effectué.
             *
             * @param startPos la position de départ
             * @param endPos la position d'arrivé
             */
            void move(const model::Position& startPos, const model::Position& endPos) noexcept;

            /**
             * Signale le modèle qu'une attaque veut être effectué.
             *
             * @param startPos la position de départ
             * @param endPos la position d'arrivé
             */
            void attack(const model::Position& startPos, const model::Position& endPos) noexcept;

            /**
             * Signale le modèle qu'un passage au joueur suivant est souhaité.
             */
            void nextPlayer() noexcept;

            /**
             * Signale le modèle que les joueurs veulent arrêter la partie de jeu courante.
             */
            void stop() noexcept;

            /**
             * Signale le modèle que la vue a traité la dernière erreur qui a eu lieu.
             */
            void errorProcessed() noexcept;

            /**
             * Signale le modèle que la partie de jeu courante doit être testé pour déterminer
             * si cette-dernière est terminée ou non.
             */
            void nextTurn() noexcept;

            /**
             * Demande aux joueurs s'ils veulent rejouer une partie et transmet leurs décision au modèle.
             */
            void replay() noexcept;
    };
};

#endif // VCSTUFF_H
