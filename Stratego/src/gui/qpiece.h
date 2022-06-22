#ifndef QPIECE_H
#define QPIECE_H

#include <QWidget>
#include <QIcon>
#include <model.h>

namespace stratego::view {

    /**
     * Pions de jeu graphique.
     */
    class QPiece : public QWidget{

        QIcon icon_;
        model::Piece* piece_;

        public:

            /**
             * Construit un nouveau pion graphique contenant le pion donné.
             *
             * @param piece le pion utilisé
             */
            QPiece(model::Piece* piece);

            /**
             * Récupère l'icône représentant le pion.
             *
             * @return l'icône représentant le pion.
             */
            QIcon& icon();

            /**
             * Récupère l'icône représentant le pion. (version read-only)
             *
             * @return l'icône représentant le pion.
             */
            const QIcon& icon() const;

            /**
             * Récupère le pion utilisé.
             *
             * @return le pion utilisé.
             */
            model::Piece* piece();

            /**
             * Récupère le pion utilisé. (version read-only)
             *
             * @return le pion utilisé.
             */
            const model::Piece* piece() const;

            /**
             * Change le pion utilisé.
             *
             * @param piece le nouveau pion à utiliser
             */
            void setPiece(model::Piece* piece);
    };
}


#endif // QPIECE_H
