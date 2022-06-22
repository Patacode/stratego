#ifndef COMMAND_H
#define COMMAND_H

#include "vcstuff.h"

namespace stratego::view{

    /**
     * Commande de déplacement.
     */
    class MoveCommand : public Command{

        Controller& controller_;
        model::Position startPos_;
        model::Position endPos_;

        public:

            /**
             * Construit une commande de déplacement en utilisant le contrôleur et les positions données.
             *
             * @param controller le contrôleur
             * @param startPos la position de départ
             * @param endPos la position de fin
             */
            MoveCommand(Controller& controller, const model::Position& startPos, const model::Position& endPos);


            // --- Déjà Documenté ---
            void exec() noexcept;
            void undo() noexcept;
    };

    /**
     * Commande d'attaque.
     */
    class AttackCommand : public Command{

        Controller& controller_;
        model::Position startPos_;
        model::Position endPos_;

        public:

            /**
             * Construit une commande d'attaque en utilisant le contrôleur et les positions données.
             *
             * @param controller le contrôleur
             * @param startPos la position de départ
             * @param endPos la position de fin
             */
            AttackCommand(Controller& controller, const model::Position& startPos, const model::Position& endPos);


            // --- Déjà Documenté ---
            void exec() noexcept;
            void undo() noexcept;
    };

    /**
     * Commande d'aide.
     */
    class HelpCommand : public Command{

        Action* action_;

        public:

            /**
             * Construit une commande d'aide pour l'action donné ou pour toutes les actions si l'action
             * donné est nullptr.
             *
             * @param action l'action à utiliser
             */
            HelpCommand(Action* action = nullptr);


            // --- Déjà Documenté ---
            void exec() noexcept;
            void undo() noexcept;
    };

    /**
     * Commande de règle.
     */
    class RulesCommand : public Command{

        std::string filename_;

        public:

            /**
             * Construit une commande de règle en utilisant le nom de fichier donné.
             *
             * @param filename le nom du fichier contennant les règles du jeu.
             */
            RulesCommand(const std::string& filename);


            // --- Déjà Documenté ---
            void exec() noexcept;
            void undo() noexcept;
    };

    /**
     * Commande de pions.
     */
    class PieceCommand : public Command{

        int rank_;

        public:

            /**
             * Construit une commande de pions pour le rang donné ou pour tous les pions
             * si le rand donné est -1.
             *
             * @param rank le rang d'un pion
             */
            PieceCommand(int rank = -1);


            // --- Déjà Documenté ---
            void exec() noexcept;
            void undo() noexcept;
    };

    /**
     * Commande d'arrêt.
     */
    class StopCommand : public Command{

        Controller& controller_;

        public:

            /**
             * Construit une commande d'arrêt en utilisant le contrôleur donné.
             *
             * @param controller le contrôleur a utiliser
             */
            StopCommand(Controller& controller);


            // --- Déjà Documenté ---
            void exec() noexcept;
            void undo() noexcept;
    };

    /**
     * Commande de stats.
     */
    class StatCommand : public Command{

        const model::Player& player_;
        int rank_;

        public:

            /**
             * Construit une commande de stats pour le joueur et le pion de rang donné ou pour le joueur
             * et tous les pions si rank vaut -1.
             *
             * @param player le joueur à utiliser
             * @param rank le rang du pion
             */
            StatCommand(const model::Player& player, int rank = -1);


            // --- Déjà Documenté ---
            void exec() noexcept;
            void undo() noexcept;
    };

    /**
     * Commande d'historique.
     */
    class HistoryCommand : public Command{

        const model::History& hist_;

        public:

            /**
             * Construit une commande d'historique en utilisant l'historique donné.
             *
             * @param hist l'historique utilisé
             */
            HistoryCommand(const model::History& hist);


            // --- Déjà Documenté ---
            void exec() noexcept;
            void undo() noexcept;
    };

    /**
     * Commande de nettoyage.
     */
    class ClearCommand : public Command{

        View& view_;
        model::Color color_;
        std::string pseudo_;

        public:

            /**
             * Commande de nettoyage pour clear la vue.
             *
             * @param view la vue à clear
             * @param color la couleur du joueur courant
             * @param pseudo le pseudo du joueur courant
             */
            ClearCommand(View& view, model::Color color, const std::string& pseudo);


            // --- Déjà Documenté ---
            void exec() noexcept;
            void undo() noexcept;
    };
}

#endif // COMMAND_H
