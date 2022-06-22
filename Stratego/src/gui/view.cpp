#include <QApplication>
#include <eventMgr.h>
#include <config.h>
#include <QResizeEvent>

#include "vcstuff.h"

using namespace stratego;

View::View(Controller& controller, Model* model) :
    controller_ {controller},
    model_ {model},
    startAppDialog_ {new view::QStartAppDialog{this}},
    configWindow_ {},
    startGameDialog_ {new view::QStartGameDialog{this}},
    gameWindow_ {},
    endGameDialog_ {},
    statusBar_ {new QStatusBar}

{
    addChildren({startAppDialog_, startGameDialog_});
}

void View::compose(){
    resize(800, 800);
    setWindowTitle("Stratego");
    setStatusBar(statusBar_);

    QFont font {};
    font.setPointSize(8);
    statusBar_ -> setFont(font);

    startAppDialog_ -> compose();
    startGameDialog_ -> compose();
}

void View::decompose(){
    QComponent::decompose();
}

void View::reload(){
    QComponent::reload();
}

void View::connectSlots(){
    connectSlotsConfigWindow();
    connectSlotsGameWindow();
}

void View::connectSlotsConfigWindow(){
    QObject::connect(configWindow_, &view::QConfigWindow::errorOccured, this, &View::displayError);
    QObject::connect(configWindow_, &view::QConfigWindow::successOccured, this, &View::displaySuccess);

    QObject::connect(configWindow_, &view::QConfigWindow::cellHovered, statusBar_, &QStatusBar::showMessage);
    QObject::connect(configWindow_, &view::QConfigWindow::cellLeaved, statusBar_, &QStatusBar::clearMessage);

    QObject::connect(configWindow_, &view::QConfigWindow::loadClicked, &controller_, &Controller::load);
    QObject::connect(configWindow_, &view::QConfigWindow::finishClicked, &controller_, &Controller::setup);
}

void View::connectSlotsGameWindow(){
    QObject::connect(gameWindow_, &view::QGameWindow::cellHovered, statusBar_, &QStatusBar::showMessage);
    QObject::connect(gameWindow_, &view::QGameWindow::cellLeaved, statusBar_, &QStatusBar::clearMessage);

    QObject::connect(gameWindow_, &view::QGameWindow::pieceMove, &controller_, &Controller::moveAttack);
    QObject::connect(gameWindow_, &view::QGameWindow::nextClicked, &controller_, &Controller::nextTurn);
}

void View::update(std::initializer_list<Observable *> args){
    model::StateGraph::State state {model_ -> currentState()};
    static bool firstRound {true}, setup {true}, requestReload {true};

    switch(state){
        case model::StateGraph::NOT_STARTED:
            controller_.init();
            break;
        case model::StateGraph::SET_UP:
            if(setup){
                swapToConfigWindow();
                setup = false;
            }

            configWindow_ -> reload();
            break;
        case model::StateGraph::ERROR_SETUP:
            displayError(model_ -> history().lastFailure().c_str());
            controller_.errorProcessed();
            break;
        case model::StateGraph::PLAYER_TURN:
            if(firstRound){
                swapToGameWindow();
                firstRound = false;
            }

            if(requestReload){
                gameWindow_ -> reload(model_ -> currentPlayer().color());
            } else{
                requestReload = true;
            }

            break;
        case model::StateGraph::PLAYER_SWAP:
            if(gameWindow_)
                gameWindow_ -> swapDisability();

            controller_.nextPlayer();
            break;
        case model::StateGraph::ERROR_ACTION:
            displayError(model_ -> history().lastFailure().c_str());
            requestReload = false;
            controller_.errorProcessed();
            break;
        case model::StateGraph::GAME_TURN:
            gameWindow_ -> swapDisability();
            gameWindow_ -> reload();
            gameWindow_ -> hideCurrentPlayerColor();
            break;
        case model::StateGraph::GAME_OVER:
            endGameDialog_ = new view::QEndGameDialog{model_ -> history().lastSuccess().c_str(), this};
            endGameDialog_ -> compose();
            firstRound = true;
            setup = true;
            controller_.replay(endGameDialog_ -> exec() == QMessageBox::AcceptRole);
            break;
        case model::StateGraph::EOG:
            QApplication::exit();
            break;
        default: throw std::logic_error{"Unknown state"};
    }
}

void View::swapToGameWindow(){
    gameWindow_ = new view::QGameWindow{model_};
    removeChild(configWindow_);
    addChild(gameWindow_);

    if(configWindow_){
        takeCentralWidget() -> deleteLater();
        configWindow_ = nullptr;
    }

    setCentralWidget(gameWindow_);
    connectSlotsGameWindow();

    gameWindow_ -> compose();
    adjustSize();
    startGameDialog_ -> exec();
}

void View::swapToConfigWindow(){
    configWindow_ = new view::QConfigWindow{model_};
    removeChild(gameWindow_);
    addChild(configWindow_);

    if(gameWindow_){
        takeCentralWidget() -> deleteLater();
        gameWindow_ = nullptr;
    }

    setCentralWidget(configWindow_);
    connectSlotsConfigWindow();

    configWindow_ -> compose();
    adjustSize();
    startAppDialog_ -> exec();
}


/* ======================== SLOTS ======================== */
void View::displayError(const QString& error){
    QMessageBox::warning(this, "Erreur", error);
}

void View::displaySuccess(const QString& success){
    QMessageBox::information(this, "Succès", success);
}
