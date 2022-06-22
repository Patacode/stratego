#ifndef QPANEL_H
#define QPANEL_H

#include <QFrame>
#include <QHBoxLayout>

#include "qcomponent.h"
#include "qboard.h"
#include "qpiecestorage.h"
#include "qgraveyard.h"
#include "qpiecestats.h"

namespace stratego::view{

    /**
     * Panel de composants horizontals pour la phase de configuration du plateau de jeu.
     */
    class QConfigPanel : public QFrame, public QComponent{

        QHBoxLayout* container_;
        QBoard* board_;
        QPieceStorage* storage_;

        public:

            /**
             * Construit un nouveau panel de composants avec le modèle et parent donnés.
             *
             * @param model le modèle utilisé
             * @param parent le parent auquel appartient le panel
             */
            QConfigPanel(const Model* model, QWidget* parent = nullptr);


            // --- Déjà documenté ---
            void compose() override;
            void decompose() override;
            void reload() override;
            void connectSlots() override;

            /**
             * Récupère le plateau de jeu contenu dans ce panel.
             *
             * @return le plateau de jeu contenu dans ce panel.
             */
            QBoard* board();

            /**
             * Récupère le stockage de pions du panel.
             *
             * @return le stockage de pions du panel.
             */
            QPieceStorage* storage();
    };

    /**
     * Panel de composants horizontals pour la phase de jeu.
     */
    class QGamePanel : public QFrame, public QComponent{

        QHBoxLayout* container_;
        QGraveyard* graveyardRed_;
        QGraveyard* graveyardBlue_;
        QBoard* board_;
        QPieceStats* stats_;

        public:

            /**
             * Construit un nouveau panel de composants avec le modèle et parent donnés.
             *
             * @param model le modèle utilisé
             * @param parent le parent auquel appartient le panel
             */
            QGamePanel(const Model* model, QWidget* parent = nullptr);


            // --- Déjà documenté ---
            void compose() override;
            void decompose() override;
            void reload() override;
            void connectSlots() override;

            /**
             * Récupère le cimetière du panel propre au joueur rouge.
             *
             * @return le cimetière du panel.
             */
            QGraveyard* graveyardRed();

            /**
             * Récupère le cimetière du panel propre au joueur bleu.
             *
             * @return le cimetière du panel.
             */
            QGraveyard* graveyardBlue();

            /**
             * Récupère le plateau de jeu du panel.
             *
             * @return le plateau de jeu du panel.
             */
            QBoard* board();

            /**
             * Récupère les statistiques de pions du panel.
             *
             * @return les statistiques de pions du panel.
             */
            QPieceStats* stats();
    };
}

#endif // QPANEL_H
