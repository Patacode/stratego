#ifndef DESIGNPATT_H
#define DESIGNPATT_H

#include <initializer_list>

/* ====================================================
 * Regroupe l'ensemble des classes abstraites propre à
 * l'implémentation des design patterns utilisés par
 * l'application.
 * ====================================================
 */

namespace stratego{

    /**
     * Parser pour convertir un fichier d'un certain format à un autre.
     */
    template<class T>
    class Parser{

        public:

            /**
             * Convertit un fichier en objet de type T.
             */
            virtual void parse() = 0;

            /**
             * Vérifie que le fichier à convertir est valide.
             *
             * @return true si le fichier à convertir est valide, false si non.
             */
            virtual bool canParse() noexcept = 0;

            /**
             * Récupère le résultat de la conversion après avoir appelé la
             * méthode parse().
             *
             * @return un objet de type T représentant le résultat de la conversion
             * du fichier.
             */
            virtual T& result() noexcept = 0;

            /**
             * Destructeur virtuel de Parser.
             */
            virtual ~Parser(){}
    };

    /**
     * Commande exécutable par les joueurs.
     */
    class Command{

        public:

            /**
             * Exécute la commande.
             */
            virtual void exec() noexcept = 0;

            /**
             * Annule la commande.
             */
            virtual void undo() noexcept = 0;

            /**
             * Destructeur virtuel de Command.
             */
            virtual ~Command(){}
    };

    class Observable;

    /**
     * Observeur d'événements produits par un Observable.
     */
    class Observer{

        public:

            /**
             * Met à jour l'observeur en fonction des actions performées par les observables.
             *
             * @param args liste d'Observables reçu par les Observers
             */
            virtual void update(std::initializer_list<Observable*> args) = 0;

            /**
             *  Destructeur virtual de Observer.
             */
            virtual ~Observer(){}
    };

    /**
     * Entitée observable par un ou plusieurs Observers.
     */
    class Observable{

        public:

            /**
             * Ajoute un observeur à cette observable.
             *
             * @param obs l'observeur à ajouter
             */
            virtual void addObserver(Observer* obs) noexcept = 0;

            /**
             * Supprime un observeur de cette observable.
             *
             * @param obs l'observeur à supprimer
             */
            virtual void removeObserver(Observer* obs) noexcept = 0;

            /**
             * Notifie l'ensemble des observeurs ajoutés à cette observable.
             *
             * @param infos liste d'Observables à envoyer aux Observers
             */
            virtual void notifyObservers(std::initializer_list<Observable*> infos) const noexcept = 0;

            /**
             *  Destructeur virtuel de Observer.
             */
            virtual ~Observable(){}
    };
};

#endif

