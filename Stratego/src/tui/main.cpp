#include <iostream>

#include "vcstuff.h"
#include <util.h>

using namespace stratego;

int main(int argc, char** argv){
    Config::setDynamicResources(argv[0]);
    Model* gameModel{};

    if(argc == 1){ // default mode
        gameModel = new Stratego{};
        Controller gameController{gameModel};
        gameController.start();
    } else{ // personalized mode
        std::regex normalPattern{"(normal|stratego|classique)", std::regex_constants::icase};
        std::regex revealPattern{"(reveal)", std::regex_constants::icase};
        bool res1 {std::regex_match(argv[1], normalPattern)};
        bool res2 {std::regex_match(argv[1], revealPattern)};

        if(res1 || res2){
            if(res1){ // normal mode
                gameModel = new Stratego{};
            } else{ // reveal mode
                gameModel = new StrategoReveal{};
            }

            Controller gameController{gameModel};
            gameController.start();
        } else{
            std::cerr << "Ancun mode de jeu correspondant n'a été trouvé.\n"
                      << "Mode de jeu disponible:\n"
                      << "\tnormal\n"
                      << "\treveal\n";

            return 1;
        }
    }

    delete gameModel;
    return 0;
}
