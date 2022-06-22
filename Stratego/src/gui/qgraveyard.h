#ifndef QGRAVEYARD_H
#define QGRAVEYARD_H

#include <QGridLayout>
#include <QFrame>
#include <QLabel>

#include "qcomponent.h"
#include "qcell.h"

namespace stratego::view{

    /**
     * Cimetière pour regrouper l'ensemble des pions perdus lors
     * d'une partie.
     */
    class QGraveyard : public QFrame, public QComponent{

        Q_OBJECT

        const Model* model_;
        model::Color color_;
        QLabel* title_;
        QVBoxLayout* container_;
        QGridLayout* storage_;
        int lastGraveyardSize_;

        static constexpr int MAX_LINE_PIECES = 4;

        public:

            /**
             * Construit un nouveau cimetière avec le modèle et parent donnés.
             *
             * @param model le modèle à utiliser
             * @param color la couleur des pions contenant dans le cimetière
             * @param parent le parent auquel appartient le cimetière
             */
            QGraveyard(const Model* model, model::Color color, QWidget* parent = nullptr);

            // --- Déjà documenté ---
            void compose() override;
            void decompose() override;
            void reload() override;
            void connectSlots() override;

            /**
             * Connecte les signaux de la cellule donnée aux slots correspondant du cimetière.
             *
             * @param cell la cellule à connecter
             */
            void connectSlots(QCell* cell);

            /**
             * Remplie le cimetière avec l'ensemble des pions donnés pour le joueur courant.
             *
             * @param pieces l'ensemble des pions à ajouter
             */
            void fillin(const std::vector<model::Piece*>& pieces);

            /**
             * Nettoie le cimetière en supprimant l'ensemble des pions qu'il contient.
             */
            void clear();

        private slots:

            void hovered(const stratego::view::QCell* cell);
            void leaved();

        signals:

            /**
             * Signale qu'une cellule du cimetière a été survolée par la souris de
             * l'utilisateur.
             *
             * @param info les informations de la cellule survolée
             */
            void cellHovered(const QString& info);

            /**
             * Signale que la souris de l'utilisateur ne survole plus une cellule.
             */
            void cellLeaved();
    };
}

#endif // QGRAVEYARD_H
