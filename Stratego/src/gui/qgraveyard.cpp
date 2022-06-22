#include "qgraveyard.h"

using namespace stratego::view;

QGraveyard::QGraveyard(const Model* model, model::Color color, QWidget* parent) :
    QFrame{parent},
    model_ {model},
    color_ {color},
    title_ {new QLabel{(std::string{"Cimetière "} + (color == model::Color::RED ? "rouge" : "bleu")).c_str()}},
    container_ {new QVBoxLayout},
    storage_ {new QGridLayout},
    lastGraveyardSize_ {}
{
    for(int i = 0; i < Config::ARMY_SIZE; i++){
        QCell* qcell {new QCell{model::Cell::NORMAL, 0, 0, nullptr}};
        addChild(qcell);
    }

    setLayout(container_);
}


/* QComponent related (inherited from it) */
void QGraveyard::compose(){
    QFont font{};
    font.setPointSize(12);
    font.setBold(true);
    title_ -> setFont(font);
    title_ -> setAlignment(Qt::AlignCenter);

    setFrameShadow(QFrame::Raised);
    setFrameShape(QFrame::Panel);
    container_ -> addWidget(title_);
    container_ -> addLayout(storage_);
    storage_ -> setSpacing(5);

    size_t rows {children_.size() / MAX_LINE_PIECES}, cols {MAX_LINE_PIECES};
    size_t colsLastLine {children_.size() % MAX_LINE_PIECES};
    int counter {};
    while(rows > 0){
        while(cols > 0){
            QCell* cell {dynamic_cast<QCell*>(children_[counter])};
            cell -> compose();
            connectSlots(cell);

            storage_ -> addWidget(cell, rows, cols);
            cols--;
            counter++;
        }

        cols = MAX_LINE_PIECES;
        rows--;
    }

    while(colsLastLine > 0){
        QCell* cell {dynamic_cast<QCell*>(children_[counter])};
        cell -> compose();
        connectSlots(cell);

        storage_ -> addWidget(cell, rows, colsLastLine);
        counter++;
        colsLastLine--;
    }

    QComponent::compose();
}

void QGraveyard::decompose(){
    QComponent::decompose();
}

void QGraveyard::reload(){
    fillin(model_ -> removedPieces());
    QComponent::reload();
}

void QGraveyard::connectSlots(){
    for(QComponent* component : children_){
        QCell* cell {dynamic_cast<QCell*>(component)};
        QObject::connect(cell, &QCell::cellHovered, this, &QGraveyard::hovered);
        QObject::connect(cell, &QCell::cellLeaved, this, &QGraveyard::leaved);
    }
}

void QGraveyard::connectSlots(QCell *cell){
    QObject::connect(cell, &QCell::cellHovered, this, &QGraveyard::hovered);
    QObject::connect(cell, &QCell::cellLeaved, this, &QGraveyard::leaved);
}


/* Related to this class only */
void QGraveyard::fillin(const std::vector<model::Piece*>& pieces){
    int counter {};
    for(model::Piece* piece : pieces){
        if(piece -> color() == color_){
            QCell* cell {dynamic_cast<QCell*>(children_[counter++])};
            cell -> qpiece().setPiece(piece);
            cell -> reload();
        }
    }

    lastGraveyardSize_ = pieces.size();
}

void QGraveyard::clear(){
    for(int i = 0; i < lastGraveyardSize_; i++){
        QCell* cell {dynamic_cast<QCell*>(children_[i])};
        cell -> qpiece().setPiece(nullptr);
        cell -> reload();
    }
}


/* Slots */
void QGraveyard::hovered(const QCell* cell){
    const model::Piece* piece {cell -> qpiece().piece()};
    if(piece){
        emit cellHovered(("Pièce " + std::string{piece -> name()} + " de rang "
                          + std::to_string(piece -> rank()) + " - " + piece -> description()).c_str());
    } else{
        emit cellHovered("Aucune pion à cet emplacement");
    }
}

void QGraveyard::leaved(){
    emit cellLeaved();
}
