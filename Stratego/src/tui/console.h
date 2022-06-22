#ifndef CONSOLE_H
#define CONSOLE_H

#include <string>
#include <functional>

namespace stratego::view{

    /**
     * Classe statique permettant d'intéragir avec le terminal. En fonction de l'os,
     * l'implémentation des méthodes de cette classe fournis aux utilisateurs plus ou moins de
     * fonctionnalitées.
     *
     * Sous linux et apple, la méthode readline(), en combinaison avec getch(), permet aux utilisateurs
     * de bénéficier d'un prompt personnalisé, avec un système de buffer et des raccourcis bien pratique.
     *
     * Référez vous à la documentation du projet pour en savoir plus.
     */
    class Console{

        static std::array<std::string, 5092> history_;
        static int hist_p_;

        Console();
        static void replace(std::string& input, const std::string& replacement, int input_p);

        public:

            /**
             * Nettoie la console. Exécute la commande 'clear' sous linux et apple, ou
             * la commande 'cls' sous windows.
             */
            static void clear() noexcept;

            /**
             * Lis une ligne au clavier jusqu'à la rencontre du caractère '\n'.
             *
             * @param specialTabFunc fonction spécial exécuté lorsque l'utilisateur
             * clique 2 fois sur tab.
             */
            static std::string readline(const std::function<void(void)>& specialTabFunc = [](){}) noexcept;

            /**
             * Réplique très simplifié de getch() définit dans 'curses.h'. Lis un caractère au clavier sans le traiter
             * et sans attendre un caractère de terminaison de ligne.
             *
             * @return le code ascii du caractère lu.
             */
            static int getch() noexcept;

            /**
             * Vérifie si le caractère de code donné est considéré comme une flèche (haut, bas, gauche, droite).
             *
             * @param ch le code du caractère
             * @return true si le caractère donné est une flèche, false si non.
             */
            static bool isArrowKey(int ch) noexcept;
    };

    /**
     * Regroupe le code ascii pour les touches du clavier.
     */
    class KeyCode{

        public:

            /**
             * Code ascii de la touche ESC.
             */
            static constexpr int VK_ESC = 0x1B;

            /**
             * Code ascii de la touche crochet gauche.
             */
            static constexpr int VK_LSQUARE_BRACKET = 0x5B;

            /**
             * Code ascii de la touche flèche du haut.
             */
            static constexpr int VK_ARROW_UP = 0x41;

            /**
             * Code ascii de la touche flèche du bas.
             */
            static constexpr int VK_ARROW_DOWN = 0x42;

            /**
             * Code ascii de la touche flèche de droite.
             */
            static constexpr int VK_ARROW_RIGHT = 0x43;

            /**
             * Code ascii de la touche flèche de gauche.
             */
            static constexpr int VK_ARROW_LEFT = 0x44;

            /**
             * Code ascii de la touche DEL (backspace).
             */
            static constexpr int VK_DEL = 0x7F;

            /**
             * Code ascii de la touche tabulation horizontal.
             */
            static constexpr int VK_HTAB = 0x09;

            /**
             * Code ascii de la touche dollar.
             */
            static constexpr int VK_DOLLAR = 0x24;

            /**
             * Code ascii de la touche accent circonflexe.
             */
            static constexpr int VK_CIRCUMFLEX = 0x5E;

    };

}

#endif // CONSOLE_H
