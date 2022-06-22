#ifndef LINEEDIT_H
#define LINEEDIT_H

#include <QDropEvent>
#include <QLineEdit>

#include "qcomponent.h"

namespace stratego::view{

    /**
     * Champs de texte accompagné d'un prédicat de validation d'entrée.
     */
    class QPredicateLineEdit : public QLineEdit, public QComponent{

        std::function<bool(const QString&)> predicate_;

        public:

            /**
             * Construit un nouveau champs de texte avec le prédicat et parent donné.
             *
             * @param predicate le prédicat de validation d'entrée
             * @param parent le parent auquel appartient le champs de texte
             */
            QPredicateLineEdit(std::function<bool(const QString&)> predicate, QWidget* parent = nullptr);

            /**
             * Vérifie ce qui a été entrée dans le champs de texte.
             *
             * @return true si ce qui a été entré en valide, false si non.
             */
            virtual bool isValid() const;
    };

    /**
     * Champs de texte pour entrer un pseudo.
     */
    class QPseudoLineEdit : public QPredicateLineEdit{

        public:

            /**
             * Construit un nouveau champs de texte pour entrer son pseudo avec le parent donné.
             *
             * @param parent le parent auquel appartient le champs de texte
             */
            QPseudoLineEdit(QWidget* parent = nullptr);


            // --- Déjà documenté ---
            void compose() override;
            void decompose() override;
            void reload() override;
            void connectSlots() override;
    };

    /**
     * Champs de texte pour entrer un nom de fichier de configuration.
     */
    class QFilenameLineEdit : public QPredicateLineEdit{

        public:

            /**
             * Construit un nouveau champs de texte pour entrer un nom de fichier
             * de configuration avec le parent donné.
             *
             * @param parent le parent auquel appartient le champs de texte
             */
            QFilenameLineEdit(QWidget* parent = nullptr);


            // --- Déjà documenté ---
            void compose() override;
            void decompose() override;
            void reload() override;
            void connectSlots() override;
    };
}

#endif // LINEEDIT_H
