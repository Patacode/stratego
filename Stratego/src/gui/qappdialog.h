#ifndef QAPPDIALOG_H
#define QAPPDIALOG_H

#include <QMessageBox>
#include <QGridLayout>

#include "qcomponent.h"

namespace stratego::view{

    /**
     * Dialogue pour le démarrage de l'application.
     */
    class QStartAppDialog : public QMessageBox, public QComponent{

        public:

            /**
             * Construit un nouveau dialogue pour le démarrage de l'application.
             *
             * @param parent le parent auquel appartient le dialogue
             */
            QStartAppDialog(QWidget* parent = nullptr);


            // --- Déjà documenté ---
            void compose() override;
            void decompose() override;
            void reload() override;
            void connectSlots() override;
    };

    /**
     * Dialogue pour le démarrage du jeu.
     */
    class QStartGameDialog : public QMessageBox, public QComponent{

        public:

            /**
             * Construit un nouveau dialogue pour le démarrage du jeu.
             *
             * @param parent le parent auquel appartient le dialogue
             */
            QStartGameDialog(QWidget* parent = nullptr);


            // --- Déjà documenté ---
            void compose() override;
            void decompose() override;
            void reload() override;
            void connectSlots() override;
    };

    /**
     * Construit un nouveau dialogue pour la fin d'une partie de jeu.
     */
    class QEndGameDialog : public QMessageBox, public QComponent{

        QString message_;

        public:

            /**
             * Construit un nouveau dialogue pour la fin d'une partie de jeu.
             *
             * @param message le message affiché dans le dialogue
             * @param parent le parent auquel appartient le dialogue
             */
            QEndGameDialog(const QString& message, QWidget* parent = nullptr);


            // --- Déjà documenté ---
            void compose() override;
            void decompose() override;
            void reload() override;
            void connectSlots() override;
    };

}

#endif // QAPPDIALOG_H
