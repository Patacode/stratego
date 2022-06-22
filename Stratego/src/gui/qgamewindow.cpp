#include "qappwindow.h"

using namespace stratego::view;

/* ========== QGameWindow ========== */
QGameWindow::QGameWindow(const Model* model, QWidget* parent) :
    QFrame{parent},
    model_ {model},
    container_ {new QVBoxLayout},
    title_ {new QLabel},
    nextButton_ {new QPushButton{"&Next"}},
    gamePanel_ {new QGamePanel{model_}},
    lastClickedBoardCell_ {}
{
    addChildren({gamePanel_});
    setLayout(container_);
}


/* QComponent related (inherited from it) */
void QGameWindow::compose(){
    model::Player player {model_ -> currentPlayer()};
    std::string title {"<h3>Au tour de <strong><font color='" +
                       std::string{player.color() == model::Color::RED ? "red" : "blue"} +
                       "'>" + model_ -> currentPlayer().pseudo() + "</font></strong></h3>"};

    title_ -> setTextFormat(Qt::RichText);
    title_ -> setText(title.c_str());
    title_ -> setAlignment(Qt::AlignCenter);

    container_ -> addWidget(title_);
    container_ -> addWidget(nextButton_);
    container_ -> addWidget(gamePanel_);

    nextButton_ -> setDisabled(true);

    QComponent::compose();
}

void QGameWindow::decompose(){
    QComponent::decompose();
}

void QGameWindow::reload(){
    updateTitle();
    QComponent::reload();
}

void QGameWindow::reload(model::Color color){
    updateTitle();
    gamePanel_ -> stats() -> reload();
    gamePanel_ -> board() -> reload(color, false);
}


void QGameWindow::connectSlots(){
    QObject::connect(gamePanel_ -> board(), &QBoard::cellHovered, this, &QGameWindow::hovered);
    QObject::connect(gamePanel_ -> graveyardRed(), &QGraveyard::cellHovered, this, &QGameWindow::hovered);
    QObject::connect(gamePanel_ -> graveyardBlue(), &QGraveyard::cellHovered, this, &QGameWindow::hovered);


    QObject::connect(gamePanel_ -> board(), &QBoard::cellLeaved, this, &QGameWindow::leaved);
    QObject::connect(gamePanel_ -> graveyardRed(), &QGraveyard::cellLeaved, this, &QGameWindow::leaved);
    QObject::connect(gamePanel_ -> graveyardBlue(), &QGraveyard::cellLeaved, this, &QGameWindow::leaved);

    QObject::connect(gamePanel_ -> board(), &QBoard::cellClicked, this, &QGameWindow::clicked);
    QObject::connect(nextButton_, &QPushButton::clicked, this, &QGameWindow::next);
}


/* Related to this class only */
void QGameWindow::swapDisability(){
   gamePanel_ -> board() -> setDisabled(!(gamePanel_ -> board() -> isEnabled() ^ true));
   nextButton_ -> setDisabled(!(nextButton_ -> isEnabled() ^ true));
}

void QGameWindow::hideCurrentPlayerColor(){
    gamePanel_ -> board() -> hideColor(model_ -> currentPlayer().color());
}

void QGameWindow::updateTitle(){
    model::Player player {model_ -> currentPlayer()};
    std::string title {"<h3>Au tour de <strong><font color='" +
                       std::string{player.color() == model::Color::RED ? "red" : "blue"} +
                       "'>" + model_ -> currentPlayer().pseudo() + "</font></strong></h3>"};
    title_ -> setText(title.c_str());
}



/* Slots */
void QGameWindow::hovered(const QString& info){
    emit cellHovered(info);
}

void QGameWindow::leaved(){
    emit cellLeaved();
}

void QGameWindow::error(const QString& message){
    emit errorOccured(message);
}

void QGameWindow::clicked(QCell* cell){
    if(lastClickedBoardCell_ && lastClickedBoardCell_ != cell){
        int prevRow {lastClickedBoardCell_ -> row()}, prevCol {lastClickedBoardCell_ -> col()};
        int cellRow {cell -> row()}, cellCol {cell -> col()};
        lastClickedBoardCell_ = nullptr;
        emit pieceMove(model::Position{prevCol, prevRow}, model::Position{cellCol, cellRow});
    } else if(cell -> qpiece().piece() && cell -> qpiece().piece() -> color() == model_ -> currentPlayer().color()){
        lastClickedBoardCell_ = cell;
    }
}

void QGameWindow::next(){
    emit nextClicked();
}
