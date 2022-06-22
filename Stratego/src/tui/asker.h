#ifndef ASKER_H
#define ASKER_H

#include "action.h"

namespace stratego::controller{

    /**
     * Asker utilisé pour l'intéraction utilisateur.
     */
    template<class T>
    class Asker{

        protected:

            std::string defaultMsg_;
            std::function<T(std::string&)> convertor_;
            std::function<bool(std::string&)> predicate_;

        public:

            /**
             * Construit un Asker en utilisant le message par défaut donné et les convertor et prédicat pour la conversion et validation
             * de ce que rentre l'utilisateur au clavier.
             *
             * @param defaultMsg le message par défaut a utiliser
             * @param convertor la fonction de conversion utilisé
             * @param predicate le prédicat utilisé
             */
            Asker(const std::string& defaultMsg, std::function<T(std::string&)> convertor, std::function<bool(std::string&)> predicate) noexcept;

            /**
             * Demande à l'utilisateur une valeur de type T jusqu'à ce que cette-dernière soit valide en utilisant
             * le message par défaut.
             *
             * @param tabFunc (optional) une fonction spécial qui s'exécute lors d'un double TAB.
             *
             * @return la conversion en type T de ce que l'utilisateur a rentré de valide en utilisant la fonction
             * de conversion.
             */
            virtual T ask(const std::function<void(void)>& tabFunc = [](){}) const noexcept;

            /**
             * Demande à l'utilisateur une valeur de type T jusqu'à ce que cette-dernière soit valide en utilisant
             * le message donné.
             *
             * @param msg le message à utiliser
             * @param tabFunc (optional) une fonction spécial qui s'exécute lors d'un double TAB.
             *
             * @return la conversion en type T de ce que l'utilisateur a rentré de valide en utilisant la fonction
             * de conversion.
             */
            virtual T ask(const std::string& msg, const std::function<void(void)>& tabFunc = [](){}) const noexcept;

            /**
             * Destructeur virtuel de Asker.
             */
            virtual ~Asker() {}
    };

    /**
     * Asker pour valeur booléenne.
     */
    class BoolAsker : public Asker<bool>{

        public:

            /**
             * Construit un asker pour valeur booléenne en utilisant le message par défaut donné.
             *
             * @param defaultMsg le message par défaut a utiliser
             */
            BoolAsker(const std::string& defaultMsg) noexcept;


            // --- Déjà Documenté ---
            bool ask(const std::string& msg, const std::function<void(void)>& tabFunc = [](){}) const noexcept override;
    };

    /**
     * Asker pour valeur entière.
     */
    class IntAsker : public Asker<int>{

        public:

            /**
             * Construit un asker pour valeur entière en utilisant le message par défaut donné.
             *
             * @param defaultMsg le message par défaut a utiliser
             */
            IntAsker(const std::string& defaultMsg) noexcept;
    };

    /**
     * Asker pour pseudo.
     */
    class PseudoAsker : public Asker<std::string>{

        public:

            /**
             * Construit un asker pour pseudo en utilisant le message par défaut donné.
             *
             * @param defaultMsg le message par défaut a utiliser
             */
            PseudoAsker(const std::string& defaultMsg) noexcept;


            // --- Déjà Documenté ---
            std::string ask(const std::string& msg, const std::function<void(void)>& tabFunc = [](){}) const noexcept override;
    };

    /**
     * Asker pour nom de fichier.
     */
    class FilenameAsker : public Asker<std::string>{

        public:

            /**
             * Construit un asker pour un nom de fichier en utilisant le message par défaut donné.
             *
             * @param defaultMsg le message par défaut a utiliser
             */
            FilenameAsker(const std::string& defaultMsg) noexcept;


            // --- Déjà Documenté ---
            std::string ask(const std::string& msg, const std::function<void(void)>& tabFunc = [](){}) const noexcept override;

    };

    /**
     * Asker pour une action.
     */
    class ActionAsker : public Asker<std::string>{

        public:

            /**
             * Construit un asker pour une action en utilisant le message par défaut donné.
             *
             * @param defaultMsg le message par défaut a utiliser
             */
            ActionAsker(const std::string& defaultMsg) noexcept;


            // --- Déjà Documenté ---
            std::string ask(const std::string& msg, const std::function<void(void)>& tabFunc = [](){}) const noexcept override;
    };
};


#endif // ASKER_H
