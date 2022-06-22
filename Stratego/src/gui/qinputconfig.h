#ifndef QINPUTCONFIG_H
#define QINPUTCONFIG_H

#include <QLineEdit>
#include <QHBoxLayout>
#include <QPushButton>

#include "qcomponent.h"
#include "lineedit.h"

namespace stratego::view{

    /**
     * Conteneur utilisé pour permettre à l'utilisateur de configurer son plateau de jeu
     * lors de la phase de mise en place.
     */
    class QInputConfig : public QFrame, public QComponent{

        Q_OBJECT

        QHBoxLayout* container_;
        QPseudoLineEdit* pseudoLineEdit_;
        QFilenameLineEdit* configLineEdit_;
        QPushButton* loadButton_;
        QPushButton* finishButton_;

        public:

            /**
             * Construit un nouveau conteneur de configuration avec le parent donné.
             *
             * @param parent le parent auquel appartient ce conteneur
             */
            QInputConfig(QWidget* parent = nullptr);


            // --- Déjà documenté ---
            void compose() override;
            void decompose() override;
            void reload() override;
            void connectSlots() override;

        private slots:

            void loaded();
            void finished();

        signals:

            /**
             * Signale que le bouton de chargement "load" à été cliqué.
             *
             * @param configFilename le nom du fichier de configuration entré lors du
             * clique sur le bouton
             */
            void loadClicked(const QString& configFilename);

            /**
             * Signale que le bouton de terminaison "finish" a été cliqué.
             *
             * @param pseudo le pseudo entré lors du clique sur le bouton
             */
            void finishClicked(const QString& pseudo);

            /**
             * Signale qu'une erreur s'est produite.
             *
             * @param message le message de l'erreur
             */
            void errorOccured(const QString& message);
    };
}

#endif // QINPUTCONFIG_H
