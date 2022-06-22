#ifndef EVENTMGR_H
#define EVENTMGR_H

/* ============================================================
 * Regroupe l'ensemble des classes intervenant dans la gestion
 * des événements de l'application.
 * ============================================================
 */

#include <map>

namespace stratego::model{

    /**
     * Graphe d'état utilisé par les Observables pour passer d'un état à un
     * autre en fonction d'événements consumés.
     */
    class StateGraph{

        public:

            /**
             * Regroupe l'ensemble des états dans lesquels
             * l'application peut se retrouver.
             */
            enum State{

                /**
                 * Le jeu n'a pas encore commencé.
                 */
                NOT_STARTED,

                /**
                 * Les joueurs configurent leur plateau de jeu pour commencer
                 * une partie.
                 */
                SET_UP,

                /**
                 * La configuration du plateau de jeu par les joueurs est erroné.
                 */
                ERROR_SETUP,

                /**
                 * Les joueurs performent une action de jeu.
                 */
                PLAYER_TURN,

                /**
                 * Le joueur courant a fini son tour et le système passe au joueur suivant.
                 */
                PLAYER_SWAP,

                /**
                 * Le système vérifie si la partie de jeu courante est terminé.
                 */
                GAME_TURN,

                /**
                 * Le dernier joueur à performé une action invalide.
                 */
                ERROR_ACTION,

                /**
                 * La partie de jeu courante est terminé.
                 */
                GAME_OVER,

                /**
                 * L'application est terminé, les joueurs ont décidés d'arrêter la
                 * partie ou de ne pas rejouer.
                 */
                EOG,

                /**
                 * Un changement d'état invalide à eu lieu.
                 */
                ERROR
            };

            /**
             * Regroupe l'ensemble des événements qui peuvent avoir lieu lors
             * d'une partie de jeu. Un événement permet aux Observables de changer
             * l'état du modèle.
             */
            enum Event{

                /**
                 * Commence une nouvelle partie de jeu.
                 */
                INI,

                /**
                 * Charge un fichier de configuration de plateau de jeu.
                 */
                LOAD,

                /**
                 * Configure la partie de jeu courante.
                 */
                SET,

                /**
                 * La configuration de la partie de jeu est erronée.
                 */
                FLOAD,

                /**
                 * Réalise une action de jeu valide.
                 */
                ACT,

                /**
                 * L'action de jeu réalisée n'est pas valide.
                 */
                FACT,

                /**
                 * La partie de jeu courante n'est pas terminé.
                 */
                CHK,

                /**
                 * La partie de jeu courante est terminé.
                 */
                FCHK,

                /**
                 * Passage au joueur suivant.
                 */
                NEXT,

                /**
                 * La dernière erreur ayant eu lieu a été traitée.
                 */
                ERRCS,

                /**
                 * Recommence une nouvel partie de jeu.
                 */
                RWD,

                /**
                 * L'application se termine.
                 */
                END,

                /**
                 * La partie de jeu courante s'arrête.
                 */
                STOP
            };

            /**
             * Construit un graphe d'état initialisé à l'état NOT_STARTED.
             */
            StateGraph() noexcept;

            /**
             * Consume l'événement donné pour passer à un nouvel état. Si l'état
             * courant ne permet pas la consommation de l'événement donné en
             * paramètre (canConsume(Event) renvoit false), l'état du graphe passe
             * à ERROR.
             *
             * @param event l'événement à consumer
             */
            void consume(Event event) noexcept;

            /**
             * Vérifie si l'événement donné peut être consumé dans l'état actuel
             * du graphe.
             *
             * @param event l'événement à testé pour consommation
             * @return true si l'événement peut être consumé, false si non.
             */
            bool canConsume(Event event) const noexcept;

            /**
             * Récupère l'état courant du graphe.
             *
             * @return l'état courant du graphe.
             */
            State state() const noexcept;

            /**
             * Change l'état courant du graphe en utilisant l'état donné
             * en paramètre.
             *
             * @param state le nouvel état
             */
            void setState(State state) noexcept; // pour tests unitaires uniquement

        private:

            std::map<State, std::map<Event, State>> graph_;
            State state_;
    };
};

#endif

