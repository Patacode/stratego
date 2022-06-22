#include <util.h>

#include "vcstuff.h"
#include "ansiColor.h"
#include "asker.h"
#include "console.h"

using namespace stratego::view;
using namespace stratego::model;
using namespace stratego::controller;
using namespace stratego;

Controller::Controller(Model* model) noexcept:
    model_ {model},
    view_ {model,*this}

{
    model_->addObserver(&view_);
}

void Controller::start() noexcept{
    while(model_->currentState()!=StateGraph::EOG){
        view_.update({model_});
    }
}

void Controller::init() noexcept{
#if defined __unix__ || defined __APPLE__
    Console::getch();
    std::cout << std::endl;
#else
    std::cin.ignore();
#endif
    model_->init();
}

void Controller::load() noexcept{
    PseudoAsker pseudoAskerRed("["+AnsiColor::colorText("Joueur rouge",AnsiColor::RED)
                               +"] Pseudo pour joueur rouge:");
    PseudoAsker pseudoAskerBlue("["+AnsiColor::colorText("Joueur bleu",AnsiColor::BLUE)
                                +"] Pseudo pour joueur bleu:");
    Asker<std::string>& askerRed{pseudoAskerRed};
    Asker<std::string>& askerBlue{pseudoAskerBlue};
    std::string pseudoRed{askerRed.ask()};
    std::string pseudoBlue{askerBlue.ask()};

    model_->setup(pseudoRed,pseudoBlue);
}

void Controller::load(Color color) noexcept{
    std::function<void(void)> tabFunc {[](){
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
    }};
    std::string player_color {color == Color::RED ? "rouge" : "bleu"};
    AnsiColor prompt_color {color == Color::RED ? AnsiColor::RED : AnsiColor::BLUE};
    FilenameAsker filenameAsker {"["+prompt_color.colorize("Joueur " + player_color)
                +"] Entrez un nom de fichier de configuration de plateau de jeu (tapez <enter> pour choisir celui par défaut):"};
    Asker<std::string>& asker{filenameAsker};
    model_->load(asker.ask(tabFunc), color);
}

void Controller::processAction() noexcept{
    ActionAsker actionAsker {"Entrez une commande:"};
    Asker<std::string>& ask{actionAsker};
    std::function<void(void)> tabFunc {[&](){
        std::cout << std::endl;
        view_.processAction("help");
        std::cout << "-> ";
    }};

    view_.processAction(ask.ask(tabFunc));
}

void Controller::move(const model::Position& startPos, const model::Position& endPos) noexcept{
    model_->move(startPos,endPos);
}

void Controller::attack(const model::Position& startPos, const model::Position& endPos) noexcept{
    model_->attack(startPos,endPos);
}

void Controller::nextPlayer() noexcept{
#if defined __unix__ || defined __APPLE__
    Console::getch();
    std::cout << std::endl;
#else
    std::cin.ignore();
#endif
    Console ::clear();
    model_->nextPlayer();
}

void Controller::stop() noexcept{
    model_->stop();
}

void Controller::errorProcessed() noexcept{
    model_->errorProcessed();
}

void Controller::nextTurn() noexcept{
    model_->nextTurn();
}

void Controller::replay() noexcept{
    BoolAsker boolAsker{"Voulez-vous rejouer y(oui) et n(non): "};
    Asker<bool>& boolState{boolAsker};
    model_->replay(boolState.ask());
}
