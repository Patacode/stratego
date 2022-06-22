#ifndef QPIECESTATS_H
#define QPIECESTATS_H

#include <QFrame>
#include <QGridLayout>
#include <QLabel>
#include <model.h>

#include "qcomponent.h"

namespace stratego::view{

    /**
     * Tableau de statistiques de pions.
     */
    class QPieceStats : public QFrame, public QComponent{

        const Model* model_;
        QGridLayout* container_;
        QLabel* title_;
        std::array<QLabel*, Config::PIECE_MAX_RANK + 1> statLabels_;

        public:

            /**
             * Construit un nouveau tableau de statistiques de pions.
             *
             * @param model le modèle utilisé
             * @param parent le parent auquel appartient ce tableau
             */
            QPieceStats(const Model* model, QWidget* parent = nullptr);


            // --- Déjà documenté ---
            void compose() override;
            void decompose() override;
            void reload() override;
            void connectSlots() override;
    };
}

#endif // QPIECESTATS_H
