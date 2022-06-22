#include "qpiecestats.h"

using namespace stratego::view;

QPieceStats::QPieceStats(const Model* model, QWidget* parent) :
    QFrame{parent},
    model_ {model},
    container_ {new QGridLayout},
    title_ {new QLabel{"Stats"}},
    statLabels_ {}
{
    setLayout(container_);
}

void QPieceStats::compose(){
    setFrameShadow(QFrame::Sunken);
    setFrameShape(QFrame::WinPanel);
    setStyleSheet("padding: 5 5");
    model::Player player {model_ -> currentPlayer()};
    container_ -> addWidget(title_, 0, 0);
    for(size_t i = 0; i < statLabels_.size(); i++){
        statLabels_[i] = new QLabel{std::to_string(player.remainingPieces(i)).c_str()};
        container_ -> addWidget(new QLabel{std::string{model::Piece::pieceInfo[i].name}.c_str()}, i + 1, 0);
        container_ -> addWidget(statLabels_[i], i + 1, 1);
    }

    container_ -> setSpacing(4);
    QComponent::compose();
}

void QPieceStats::decompose(){
    QComponent::decompose();
}

void QPieceStats::reload(){
    model::Player player {model_ -> currentPlayer()};
    for(size_t i = 0; i < statLabels_.size(); i++){
        statLabels_[i] -> setText(std::to_string(player.remainingPieces(i)).c_str());
    }

    QComponent::reload();
}

void QPieceStats::connectSlots(){}
