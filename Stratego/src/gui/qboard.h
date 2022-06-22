#ifndef QBOARD_H
#define QBOARD_H

#include <QGridLayout>
#include <QFrame>
#include <QDropEvent>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <model.h>

#include "qcomponent.h"
#include "qcell.h"

namespace stratego::view{

    /**
     * Plateau de jeu graphique.
     */
    class QBoard : public QFrame, public QComponent{

        Q_OBJECT

        QGridLayout* container_;
        std::vector<QCell*> cells_;
        const Model* model_;
        std::vector<QCell*> lastClickedCells_;

        public:

            /**
             * Construit un nouveau plateau de jeu graphique avec le modèle et parent donné.
             *
             * @param model le modèle utilisé
             * @param parent le parent auquel appartient le plateau de jeu
             */
            QBoard(const Model* model, QWidget* parent = nullptr);

            // --- Déjà documenté ---
            void compose() override;
            void decompose() override;
            void reload() override;
            void connectSlots() override;

            /**
             * Recharge la plateau de jeu pour les pions de couleur donnée.
             *
             * @param color la couleur des pions à recharger
             * @param setup spécifie sur le rechargement à lieu lors de la phase de configuration du
             * plateau de jeu ou non.
             */
            void reload(model::Color color, bool setup = true);

            /**
             * Connecte les signaux de la cellule donnée aux slots correspondant du plateau de jeu.
             *
             * @param cell la cellule à connecter
             */
            void connectSlots(QCell* cell);

            /**
             * Nettoie les pions de couleur donnée du plateau de jeu (pour setup uniquement).
             *
             * @param color la couleur des pions à nettoyer
             */
            void clear(model::Color color);

            /**
             * Cache les pions de couleur donnée du plateau de jeu.
             *
             * @param color la couleur des pions à cacher
             */
            void hideColor(model::Color color);

            /**
             * Vérifie si le plateau de jeu est complet pour les pions de couleur donnée (pour setup uniquement).
             *
             * @param color la couleur des pions à vérifier
             * @return true si la plateau de jeu est complet pour les pions de couleur donnée, false si non.
             */
            bool isFull(model::Color color);

            /**
             * Sauvegarde le plateau de jeu dans le fichier de nom donné pour le joueur de couleur donné. (pour setup uniquement)
             *
             * @param filename le nom du fichier (ou le chemin vers le fichier) dans lequel sauvegarder
             * le plateau de jeu
             * @param color la couleur du joueur pour qui sauvegarder le plateau de jeu
             */
            void saveToFile(const std::string& filename, model::Color color);


            // --- Déjà documenté ---
            void dragEnterEvent(QDragEnterEvent* event) override;
            void dropEvent(QDropEvent* event) override;

        private:

            void highlightPossibleMoves(const model::Piece* piece);
            void clearHighlights();
            std::string adjustAbsolutePath(const std::string& filepath);

        private slots:

            void hovered(const stratego::view::QCell* cell);
            void leaved(stratego::view::QCell* cell);
            void clicked(const stratego::view::QCell* cell);

        signals:

            /**
             * Signale qu'une cellule du plateau de jeu a été survolée par la souris de
             * l'utilisateur.
             *
             * @param info les informations de la cellule survolée
             */
            void cellHovered(const QString& info);

            /**
             * Signale que la souris de l'utilisateur ne survole plus une cellule.
             */
            void cellLeaved();

            /**
             * Signale qu'une cellule a été cliquée.
             *
             * @param cell la cellule qui a été cliquée
             */
            void cellClicked(stratego::view::QCell* cell);

            /**
             * Signale qu'un fichier a été drop sur plateau de jeu. (pour setup uniquement)
             *
             * @param filepath le chemin vers le fichier qui a été drop
             */
            void fileDropped(const QString& filepath);

            /**
             * Signale qu'une erreur s'est produite sur le plateau de jeu.
             *
             * @param message le message de l'erreur
             */
            void errorOccured(const QString& message);
    };
}

#endif // QBOARD_H
