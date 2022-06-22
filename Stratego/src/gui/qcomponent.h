#ifndef QCOMPONENT_H
#define QCOMPONENT_H

#include <iostream>
#include <QDebug>

#include <vector>
#include <QFrame>

namespace stratego::view{

    class QComponent{

        protected:

            /**
             * Ensemble des enfants du composant.
             */
            std::vector<QComponent*> children_;

        public:

            /**
             * Met en place le composant (initialisation de base).
             */
            virtual void compose() = 0;

            /**
             * Décompose le composant.
             */
            virtual void decompose() = 0;

            /**
             * Recharge le composant pour le mettre à jour.
             */
            virtual void reload() = 0;

            /**
             * Connecte l'ensemble des widgets du composant aux slots définis.
             */
            virtual void connectSlots() = 0;

            /**
             * Ajoute un enfant à la liste des enfants de ce composant.
             *
             * @param child l'enfant à ajouter
             */
            virtual void addChild(QComponent* child);

            /**
             * Ajoute l'ensemble des enfants données à la liste des enfants de ce composant.
             *
             * @param children l'ensemble des enfants à ajouter
             */
            virtual void addChildren(std::initializer_list<QComponent*> children);

            /**
             * Supprime l'enfant donné de la liste des enfants du composant. Cette méthode
             * ne supprime rien si l'enfant donné n'est pas trouvé.
             *
             * @param child l'enfant à vouloir supprimer
             */
            virtual void removeChild(QComponent* child);

            /**
             * Destructeur virtuel d'un QComponent.
             */
            virtual ~QComponent(){}
    };
}

#endif // QCOMPONENT_H
