#ifndef QCELL_H
#define QCELL_H

#include <QPushButton>
#include <QGridLayout>
#include <model.h>
#include <QMouseEvent>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDropEvent>
#include <QDrag>
#include <QFocusEvent>

#include "qcomponent.h"
#include "qpiece.h"

namespace stratego::view{

    /**
     * Cellule graphique pouvant contenir un pion.
     */
    class QCell : public QPushButton, public QComponent{

        Q_OBJECT

        model::Cell::Type type_;
        QPiece piece_;
        int row_, col_;

        public:

            /**
             * Construit une nouvelle cellule de type donné, se trouvant à une certaine position et dans laquelle
             * se trouve possiblement un pion.
             *
             * @param type le type de la cellule
             * @param row la rangée où se trouve la cellule
             * @param col la colonne où se trouve la cellule
             * @param piece le pion qui se trouve dans la cellule
             * @param parent le parent auquel appartient la cellule
             */
            QCell(model::Cell::Type type, int row, int col, model::Piece* piece, QWidget* parent = nullptr);


            // --- Déjà documenté ---
            void compose() override;
            void decompose() override;
            void reload() override;
            void connectSlots() override;

            /**
             * Récupère la rangée où se trouve la cellule.
             *
             * @return la rangée où se trouve la cellule.
             */
            int row() const;

            /**
             * Change la rangée où se trouve la cellule.
             *
             * @param row la rangée où se trouve la cellule.
             */
            void setRow(int row);

            /**
             * Récupère la colonne où se trouve la cellule.
             *
             * @return la colonne où se trouve la cellule.
             */
            int col() const;

            /**
             * Change la colonne où se trouve la cellule.
             *
             * @param col la colonne où se trouve la cellule.
             */
            void setCol(int col);

            /**
             * Récupère la qpiece qui se trouve dans cette cellule. Cette objet
             * représente une icône vide ou pleine en fonction de son état (donc en fonction
             * de si il se trouve un pion sur cette cellule ou non)
             *
             * @return la qpiece qui se trouve dans cette cellule.
             */
            QPiece& qpiece();

            /**
             * Récupère la qpiece qui se trouve dans cette cellule. Cette objet
             * représente une icône vide ou pleine en fonction de son état (donc en fonction
             * de si il se trouve un pion sur cette cellule ou non). (version read-only)
             *
             * @return la qpiece qui se trouve dans cette cellule.
             */
            const QPiece& qpiece() const;

            /**
             * Récupère le type de la cellule.
             *
             * @return le type de la cellule.
             */
            model::Cell::Type type() const;

            /**
             * Change l'état de surbrillance de la cellule.
             *
             * @param state l'état de surbrillance
             */
            void highlight(bool state);


            // --- Déjà documenté ---
            void mouseMoveEvent(QMouseEvent* event) override;
            void leaveEvent(QEvent* event) override;

        signals:

            /**
             * Signale que la cellule a été survolée par la souris de
             * l'utilisateur.
             *
             * @param cell la cellule survolée
             */
            void cellHovered(const stratego::view::QCell* cell);

            /**
             * Signale que la souris de l'utilisateur ne survole plus la cellule.
             *
             * @param cell la cellule n'étant plus survolée
             */
            void cellLeaved(stratego::view::QCell* cell);

            /**
             * Signale que la cellule a été cliquée.
             *
             * @param cell la cellule qui a été cliquée
             */
            void cellClicked(const stratego::view::QCell* cell);

        private slots:

            void buttonClicked(bool checked = false);
    };
}

#endif // QCELL_H
