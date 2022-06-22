#ifndef QPIECESTORAGE_H
#define QPIECESTORAGE_H

#include <QLabel>
#include <QVBoxLayout>
#include <QScrollArea>
#include <model.h>

#include "qcomponent.h"
#include "qcell.h"

namespace stratego::view{

    /**
     * Stockage de pions.
     */
    class QPieceStorage : public QFrame, public QComponent{

        Q_OBJECT

        QLabel* title_;
        QVBoxLayout* container_;
        QGridLayout* storage_;

        static constexpr int MAX_LINE_PIECES = 4;

        public:

            /**
             * Construit un nouveau stockage de pions avec le parent donné.
             *
             * @param parent le parent auquel appartient le stockage
             */
            QPieceStorage(QWidget* parent = nullptr);

            // --- Déjà documenté ---
            void compose() override;
            void decompose() override;
            void reload() override;
            void connectSlots() override;

            /**
             * Connecte les signaux de la cellule donnée aux slots correspondant du stockage.
             *
             * @param cell la cellule à connecter
             */
            void connectSlots(QCell* cell);

            /**
             * Remplie le stockage avec l'ensemble des pions donnés.
             *
             * @param pieces l'ensemble des pions à ajouter
             */
            void fillin(const std::vector<model::Piece*>& pieces);

            /**
             * Nettoie le stockage en supprimant l'ensemble des pions qu'il contient.
             */
            void clear();

            /**
             * Vérifie si le stockage est vide.
             *
             * @return true si le stockage est vide, false si non.
             */
            bool isEmpty();

        private slots:

            void hovered(const stratego::view::QCell* cell);
            void leaved();
            void clicked(const stratego::view::QCell* cell);

        signals:

            /**
             * Signale qu'une cellule du stockage de pions a été survolée par la souris de
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
    };
}

#endif // QPIECESTORAGE_H
