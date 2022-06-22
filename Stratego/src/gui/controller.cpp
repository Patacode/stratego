#include "vcstuff.h"

using namespace stratego;

Controller::Controller(Model* model) :
    model_ {model},
    view_ {*this, model_}
{
    model_ -> addObserver(&view_);
}

void Controller::start(){
    view_.compose();
    view_.show();
    view_.update({model_});
}

void Controller::init(){
    model_ -> init();
}

void Controller::load(const std::string &filename, model::Color color, bool isAbsolutePath){
    model_ -> load(filename, color, isAbsolutePath);
}

void Controller::setup(const std::string &redPseudo, const std::string &bluePseudo){
    model_ -> setup(redPseudo, bluePseudo);
}

void Controller::errorProcessed(){
    model_ -> errorProcessed();
}

void Controller::nextPlayer(){    
    model_ -> nextPlayer();
}

void Controller::moveAttack(const model::Position &startPos, const model::Position &endPos){
    model_ -> moveAttack(startPos, endPos);
}

void Controller::nextTurn(){
    model_ -> nextTurn();
}

void Controller::replay(bool state){
    model_ -> replay(state);
}
