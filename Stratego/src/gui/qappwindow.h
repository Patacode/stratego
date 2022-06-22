#ifndef QAPPWINDOW_H
#define QAPPWINDOW_H

#include <QFrame>
#include <QVBoxLayout>
#include <QThread>
#include <model.h>

#include "qcomponent.h"
#include "qcell.h"
#include "qpanel.h"
#include "qinputconfig.h"

namespace stratego::view{

    /**
     * Fenêtre de configuration du plateau de jeu (avant le démarrage d'une partie).
     */
    class QConfigWindow : public QFrame, public QComponent{

        Q_OBJECT

        const Model* model_;
        model::Color playerColor_;
        int playerPointer_;
        std::array<std::string, 2> playerNames_;

        QVBoxLayout* container_;
        QLabel* title_;
        QConfigPanel* configPanel_;
        QPushButton* saveButton_;
        QInputConfig* userInput_;
        QCell* lastClickedBoardCell_;
        QCell* lastClickedStorageCell_;

        public:

            /**
             * Construit une nouvelle fenêtre de configuration du plateau de jeu avec le modèle, la couleur d'un joueur et
             * le parent donné.
             *
             * La couleur du joueur permet à la fenêtre de déterminer quel joueur commence à configurer son plateau de jeu.
             *
             * @param model le modèle utilisé
             * @param playerColor la couleur du joueur qui commence à configurer son plateau de jeu
             * @param parent le parent auquel appartient la fenêtre
             */
            QConfigWindow(const Model* model, model::Color playerColor = model::Color::RED, QWidget* parent = nullptr);


            // --- Déjà documenté ---
            void compose() override;
            void decompose() override;
            void reload() override;
            void connectSlots() override;

            /**
             * Récupère la couleur du joueur courant configurant son plateau de jeu.
             *
             * @param la couleur du joueur courant configurant son plateau de jeu.
             */
            model::Color playerColor() const;

        private:

            bool validRow(int row) const;
            void updateTitle();

        private slots:

            void loaded(const QString& configFilename);
            void dropLoaded(const QString& filepath);
            void finished(const QString& pseudo);
            void error(const QString& error);
            void hovered(const QString& info);
            void leaved();

            void boardClicked(stratego::view::QCell* cell);
            void storageClicked(stratego::view::QCell* cell);
            void saved();

        signals:

            /**
             * Signale que le bouton de chargement "load" à été cliqué.
             *
             * @param configFilename le nom du fichier de configuration entré lors du
             * clique sur le bouton
             * @param color la couleur du joueur pour lequel charger le fichier
             * @param isAbsolutePath flag pour déterminer si le nom du fichier donné représente un chemin
             * absolu
             */
            void loadClicked(const std::string& configFilename, model::Color color, bool isAbsolutePath = false);

            /**
             * Signale que le bouton de terminaison "finish" a été cliqué. Ce signale est émit
             * uniquement lorsque le dernier joueur à configurer son plateau de jeu clique
             * sur le bouton "finish".
             *
             * @param redPseudo le pseudo du joueur rouge
             * @param bluePseudo le pseudo du joueur bleu
             */
            void finishClicked(const std::string& redPseudo, const std::string& bluePseudo);

            /**
             * Signale qu'une erreur s'est produite.
             *
             * @param message le message de l'erreur
             */
            void errorOccured(const QString& message);

            /**
             * Signale qu'un succès à eu lieu.
             *
             * @param message le message de succès
             */
            void successOccured(const QString& message);

            /**
             * Signale qu'une cellule du plateau de jeu a été survolée par la souris de
             * l'utilisateur.
             *
             * @param info les informations de la cellule survolée
             * @param timeout le temps d'affichage des informations propres
             * à la cellule survolée (référez vous à la documentation du slot "showMessage()" de la classe
             * QStatusBar)
             */
            void cellHovered(const QString& info, int timeout = 0);

            /**
             * Signale que la souris de l'utilisateur ne survole plus une cellule.
             */
            void cellLeaved();
    };

    /**
     * Fenêtre pour jouer au jeu (après la configuration du plateau de jeu).
     */
    class QGameWindow : public QFrame, public QComponent{

        Q_OBJECT

        const Model* model_;
        QVBoxLayout* container_;
        QLabel* title_;
        QPushButton* nextButton_;
        QGamePanel* gamePanel_;
        QCell* lastClickedBoardCell_;

        public:

            /**
             * Construit une nouvelle fenêtre pour jouer au jeu avec le modèle et parent donné.
             *
             * @param model le modèle utilisé
             * @param parent le parent auquel appartient la fenêtre
             */
            QGameWindow(const Model* model, QWidget* parent = nullptr);

            // --- Déjà documenté ---
            void compose() override;
            void decompose() override;
            void reload() override;
            void connectSlots() override;

            /**
             * Recharge la fenêtre pour le joueur de couleur donné uniquement.
             *
             * @param color la couleur du joueur pour qui recharger la fenêtre
             */
            void reload(model::Color color);

            /**
             * Intervertit la désactivation des composants de la fenêtre (soit bouton next, soit plateau de jeu)
             */
            void swapDisability();

            /**
             * Cache les pions du joueur courant.
             */
            void hideCurrentPlayerColor();

        private:

            void updateTitle();

        private slots:

            void hovered(const QString& info);
            void leaved();
            void error(const QString& error);
            void next();
            void clicked(stratego::view::QCell* cell);

        signals:

            /**
             * Signale qu'une cellule du plateau de jeu a été survolée par la souris de
             * l'utilisateur.
             *
             * @param info les informations de la cellule survolée
             * @param timeout le temps d'affichage des informations propres
             * à la cellule survolée (référez vous à la documentation du slot "showMessage()" de la classe
             * QStatusBar)
             */
            void cellHovered(const QString& info, int timeout = 0);

            /**
             * Signale que la souris de l'utilisateur ne survole plus une cellule.
             */
            void cellLeaved();

            /**
             * Signale qu'une erreur s'est produite.
             *
             * @param message le message de l'erreur
             */
            void errorOccured(const QString& message);

            /**
             * Signale qu'un pion s'est déplacé.
             *
             * @param startPos la position de départ du pion
             * @param endPos la position de destination du pion
             */
            void pieceMove(const model::Position& startPos, const model::Position& endPos);

            /**
             * Signale que le bouton "next" a été cliqué.
             */
            void nextClicked();
    };
}
#endif // QAPPWINDOW_H
