#include <iostream>
#include <util.h>
#include <gamestuff.h>

#include "asker.h"
#include "console.h"
#include "config.h"
#include "ansiColor.h"

using namespace stratego::controller;
using namespace stratego::model;
using namespace stratego::view;
using namespace stratego;

/* ========================== Asker =========================== */
template<class T>
Asker<T>::Asker(const std::string& defaultMsg, std::function<T(std::string&)> convertor, std::function<bool(std::string&)> predicate) noexcept:
    defaultMsg_ {defaultMsg},
    convertor_ {convertor},
    predicate_ {predicate}
{}

template<class T>
T Asker<T>::ask(const std::function<void(void)>& tabFunc) const noexcept{
    return ask(defaultMsg_, tabFunc);
}

template<class T>
T Asker<T>::ask(const std::string& msg, const std::function<void(void)>& tabFunc) const noexcept{
    std::cout << msg << std::endl << "-> ";
    std::string input {};
    input = Console::readline(tabFunc);
    util::trim(input);
    while(!predicate_(input)){
        std::cout << "Mauvaise entrée. Recommencez !" << std::endl << "-> ";
        input = Console::readline(tabFunc);
        util::trim(input);
    }

    return convertor_(input);
}


/* ========================== BoolAsker =========================== */
BoolAsker::BoolAsker(const std::string& defaultMsg) noexcept:
    Asker{defaultMsg,
        [](std::string& input){
              return std::regex_match(input, std::regex{"(y|o|yes|oui)", std::regex_constants::icase});
        },
        [](std::string& input){
            return std::regex_match(input, std::regex{"(y|o|n|yes|no|oui|non)", std::regex_constants::icase});
        }}
{}

bool BoolAsker::ask(const std::string& msg, const std::function<void(void)>& tabFunc) const noexcept{
    std::cout << msg << std::endl << "-> ";
    std::string input {};
    input = Console::readline(tabFunc);
    util::trim(input);
    while(!predicate_(input)){
        std::cout << "Mauvaise entrée. Recommencez !" << std::endl;
        std::cout << "Tapez soit (y,o,yes,oui) pour confirmer ou (n,no,non) pour décliner."
                  << std::endl << "-> ";
        input = Console::readline(tabFunc);
        util::trim(input);
    }

    return convertor_(input);
}


/* ========================== IntAsker =========================== */
IntAsker::IntAsker(const std::string& defaultMsg) noexcept:
    Asker{defaultMsg,
        [](std::string& input){
            return std::stoi(input);
        },[](std::string& input){
            return util::isint(input);
        }
    }
{}


/* ========================== PseudoAsker =========================== */
PseudoAsker::PseudoAsker(const std::string& defaultMsg) noexcept:
    Asker{defaultMsg,
        [](std::string& input){
            return input;
        },[](std::string& input){
            return std::regex_match(input, std::regex{"[[:alpha:]]{3,}"});
        }
    }
{}

std::string PseudoAsker::ask(const std::string& msg, const std::function<void(void)>& tabFunc) const noexcept{
    std::cout << msg << std::endl << "-> ";
    std::string input {};
    input = Console::readline(tabFunc);
    util::trim(input);
    while(!predicate_(input)){
        std::cout << "Mauvaise entrée. Recommencez !" << std::endl;
        std::cout << "Le pseudo ne peut contenir que des lettres (majuscules ou miniscules) et doit être "
                  << "de longueur 3 minimum." << std::endl << "-> ";
        input = Console::readline(tabFunc);
        util::trim(input);
    }

    return convertor_(input);
}


/* ========================== FilenameAsker =========================== */
FilenameAsker::FilenameAsker(const std::string& defaultMsg) noexcept:
    Asker{defaultMsg,
        [](std::string& input){
            return input;
        },
        [](std::string& input){
            util::trim(input);
            if(input.empty())
                return true;

            std::ifstream fstream {std::string{Config::BOARD_CONFIG_PATH} + input};
            bool state {fstream.good() && ConfigFileParser::canParse(fstream)};
            fstream.close();
            return state;
        }
    }
{}

std::string FilenameAsker::ask(const std::string& msg, const std::function<void(void)>& tabFunc) const noexcept{
    std::cout << msg << std::endl << "-> ";
    std::string input {};
    input = Console::readline(tabFunc);
    util::trim(input);
    while(!predicate_(input)){
        std::cout << "Fichier invalide ou inexistant. Recommencez !" << std::endl;
#ifdef _WIN32
        std::ifstream ifs {};
        std::cout << std::endl<< "Fichiers disponibles (sous "
                  << AnsiColor::colorText(Config::BOARD_CONFIG_PATH, AnsiColor::BOLD)
                  << "):" << std::endl;
        for(std::string& filename : util::filesOf(Config::BOARD_CONFIG_PATH)){
            ifs.open(Config::BOARD_CONFIG_PATH + filename);
            std::cout << "\t" << filename << " " << (ConfigFileParser::canParse(ifs) ? "*" : "") << std::endl;
            ifs.close();
        }
        std::cout << "Les fichiers valides sont annotés d'une étoile (*)." << std::endl;
        std::cout << "-> ";
#endif
        std::cout << "-> ";
        input = Console::readline(tabFunc);
        util::trim(input);
    }

    return input.empty() ? "default" : input;
}


/* ========================== ActionAsker =========================== */
ActionAsker::ActionAsker(const std::string& defaultMsg) noexcept:
    Asker{defaultMsg, {},{}}
{}

std::string ActionAsker::ask(const std::string& msg, const std::function<void(void)>& tabFunc) const noexcept{
    bool sflag {}; // success flag
    ActionMatcher matcher {};
    std::cout << msg << std::endl;
    std::string input {};
    while(!sflag){
        try{
            std::cout << "-> ";
            input = Console::readline(tabFunc);
            util::trim(input);
            matcher.setAction(input);
            if(!matcher.match(input)){
                std::cout << "Erreur de syntaxe. Tapez HELP <cmd> pour obtenir de l'aide." << std::endl;
            } else sflag = true;
        } catch(std::invalid_argument& exc){
            std::cout << "Action invalide. Tapez HELP pour voir les actions disponibles." << std::endl;
        }
    }

    return input;
}



