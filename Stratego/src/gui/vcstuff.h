#ifndef VCSTUFF_H
#define VCSTUFF_H

#include <QMainWindow>
#include <QStatusBar>
#include <designpatt.h>
#include <model.h>

#include "qappdialog.h"
#include "qappwindow.h"
#include "qcomponent.h"

namespace stratego{

    class Controller;

    /**
     * Construit une vue pour l'application chargé de mettre correctement à jour l'interface.
     */
    class View : public QMainWindow, public Observer, public view::QComponent{

        Q_OBJECT

        private:

            Controller& controller_;
            const Model* model_;
            view::QStartAppDialog* startAppDialog_;
            view::QConfigWindow* configWindow_;
            view::QStartGameDialog* startGameDialog_;
            view::QGameWindow* gameWindow_;
            view::QEndGameDialog* endGameDialog_;
            QStatusBar* statusBar_;

        public:

            /**
             * Construit une nouvelle vue avec le contrôleur et modèle donnés.
             *
             * @param controller le contrôleur à utiliser
             * @param model le modèle à utiliser
             */
            View(Controller& controller, Model* model);


            // --- Déjà documenté ---
            void update(std::initializer_list<Observable*> args) override;


            // -- Déjà documenté ---
            void compose() override;
            void decompose() override;
            void reload() override;
            void connectSlots() override;
            void connectSlotsConfigWindow();
            void connectSlotsGameWindow();

        private:

            void swapToGameWindow();
            void swapToConfigWindow();

        private slots:

            void displayError(const QString& error);
            void displaySuccess(const QString& error);
    };

    /**
     * Contrôleur de jeu pour transmettre des informations de la vue au modèle.
     */
    class Controller : public QObject{

        Q_OBJECT

        Model* model_;
        View view_;

        public:

            /**
             * Construit un nouveau contrôleur avec le modèle donné. Une vue par défaut
             * lui sera également donné.
             *
             * @param model le modèle à utiliser
             */
            Controller(Model* model);

            /**
             * Commence une nouvelle partie de jeu. Uniquement cette méthode devrait être invoquée
             * pour commencer une partie, toutes les autres seront implicitement invoquées lors de
             * l'exécution de l'application.
             */
            void start();

        public slots:

            /**
             * Signale le modèle qu'une partie de jeu veut être lancée.
             */
            void init();

            /**
             * Signale le modèle qu'un fichier de configuration pour le joueur de couleur donné voudrait être chargé.
             *
             * @param filename le nom du fichier de configuration à charger
             * @param color la couleur du joueur pour lequel charger le fichier de configuration
             * @param isAbsolutePath flag pour déterminer si le nom du fichier donné représente un chemin
             * absolu
             */
            void load(const std::string& filename, model::Color color, bool isAbsolutePath = false);

            /**
             * Signale le modèle qu'une partie de jeu aimerait être lancé en utilisant les pseudos donnés pour les deux joueurs.
             *
             * @param redPseudo le pseudo du joueur rouge
             * @param bluePseudo le pseudo du joueur bleu
             */
            void setup(const std::string& redPseudo, const std::string& bluePseudo);

            /**
             * Signale le modèle que la vue a traité la dernière erreur qui a eu lieu.
             */
            void errorProcessed();

            /**
             * Signale le modèle qu'un passage au joueur suivant est souhaité.
             */
            void nextPlayer();

            /**
             * Signale le modèle qu'un mouvement ou une attaque veut être effectué.
             *
             * @param startPos la position de départ
             * @param endPos la position d'arrivé
             */
            void moveAttack(const model::Position& startPos, const model::Position& endPos);

            /**
             * Signale le modèle que la partie de jeu courante devrait être testé pour déterminer
             * si cette-dernière est terminée ou non.
             */
            void nextTurn();

            /**
             * Signale au modèle le souhait qu'ont les joueurs à vouloir refaire une partie de jeu.
             *
             * @param state l'état de la décision des joueurs à vouloir refaire une partie
             */
            void replay(bool state);            
    };
}
#endif // VCSTUFF_H
