#ifndef ACTION_H
#define ACTION_H

#include <regex>

#include "properties.h"

namespace stratego::view{

    /**
     * Action que les joueurs peuvent réaliser.
     */
    class Action{

        public:

            /**
             * Regroupe l'ensemble des actions réalisables par
             * les joueurs depuis la vue.
             */
            enum Value{

                /**
                 * Action de mouvement pour déplacer un pion.
                 */
                MOVE,

                /**
                 * Action d'attaque pour attaquer un pion.
                 */
                ATTACK,

                /**
                 * Action d'aide pour afficher l'ensemble des actions
                 * réalisables.
                 */
                HELP,

                /**
                 * Action de règle pour afficher les règles du jeu.
                 */
                RULES,

                /**
                 * Action de pièce pour obtenir des infos propre aux pions.
                 */
                PIECE,

                /**
                 * Action d'arrêt pour mettre fin à l'application.
                 */
                STOP,

                /**
                 * Action de statistique pour afficher des données
                 * analytiques propre au joueur courant.
                 */
                STAT,

                /**
                 * Action d'historique pour obtenir une vue d'ensemble
                 * sur ce qui s'est passé pendant une partie de jeu.
                 */
                HISTORY,

                /**
                 * Action de clear pour nettoyer l'écran du terminal.
                 */
                CLEAR
            };

            /**
             * Tableau de noms permettant de récupérer la valeur textuelle d'une action.
             */
            static const std::array<std::string, 9> valueNames;

            /**
             * Construit une action de valeur donnée.
             *
             * @param value la valeur de l'action
             */
            Action(Value value) noexcept;

            /**
             * Construit une action à partir d'une chaîne donnée. Seul la première partie
             * de la chaîne sera utilisé (celle avant le premier espace blanc).
             *
             * @throw std::invalid_argument si aucune action ne correspond à la chaîne donnée.
             *
             * @param str la chaîne à convertir en action
             */
            Action(const std::string& str);

            /**
             * Récupère la description de l'action.
             *
             * @return la description de l'action.
             */
            std::string description() const noexcept;

            /**
             * Récupère la syntaxe de l'action.
             *
             * @return la syntaxe de l'action.
             */
            std::string syntax() const noexcept;

            /**
             * Récupère la regex de l'action.
             *
             * @return la regex de l'action.
             */
            std::regex regex() const noexcept;

            /**
             * Change la valeur de l'action.
             *
             * @param value la nouvel valeur à prendre
             */
            void setAction(Value value) noexcept;

            /**
             * Change la valeur de l'action à partir d'une chaîne donnée. Seul la première partie
             * de la chaîne sera utilisé (celle avant le premier espace blanc).
             *
             * @throw std::invalid_argument si aucune action ne correspond à la chaîne donnée.
             *
             * @param str la chaîne à convertir
             */
            void setAction(const std::string& str);

            /**
             * Surchage d'opérateur de action en sa valeur d'énumeration correspondante.
             * Cette surcharge d'opérateur permet à la classe Action d'être contextuellement
             * implicitement convertible en type d'énumeration, lui permettant d'être utilisé dans
             * un switch.
             */
            constexpr operator Value() const {return action_;}

        private:

            Value action_;

            enum Info{
                DESC,
                SYNTAX,
                REGEX
            };

            std::string fetchInfo(Value value, Info info) const;

            static const std::array<std::string, 3> infoNames_;
            static const std::map<std::string, Action::Value> nameActionMap_;
    };

    /**
     * Matcher pour la réalisation d'une action.
     */
    class ActionMatcher{

        Action action_;
        std::smatch matcher_;

        public:

            /**
             * Construit un matcher par défaut. Ce matcher utilisera la première
             * action définie dans l'énumeration utilisée par la classe Action.
             */
            ActionMatcher() noexcept;

            /**
             * Construit un matcher pour l'action donnée.
             *
             * @param action l'action à matcher
             */
            ActionMatcher(Action::Value action) noexcept;

            /**
             * Récupère l'action courante du matcher.
             *
             * @return l'action du matcher.
             */
            Action::Value action() const noexcept;

            /**
             * Change l'action du matcher en utilisant la chaîne donnée. Seul la première partie
             * de la chaîne sera utilisé (celle avant le premier espace blanc).
             *
             * @throw std::invalid_argument si aucune action ne correspond à la chaîne donnée.
             *
             * @param str la chaîne à utiliser
             */
            void setAction(const std::string& str);

            /**
             * Récupère une référence vers le string matcher utilisé (suite à l'appel d'un match(std::string)).
             *
             * @return le string matcher utilisé.
             */
            std::smatch& matcher() noexcept;

            /**
             * Détermine si la chaîne donnée match avec la regex définis pour l'action courante du matcher.
             *
             * @param str la chaîne à vérifier
             * @return true si la chaîne donnée match, false si non
             */
            bool match(const std::string& str) noexcept;
    };
}

#endif // ACTION_H
