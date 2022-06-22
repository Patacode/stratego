#include "qpiecestorage.h"

using namespace stratego::view;

QPieceStorage::QPieceStorage(QWidget* parent) :
    QFrame{parent},
    title_ {new QLabel{"Stockage"}},
    container_ {new QVBoxLayout},
    storage_ {new QGridLayout}
{
    for(int i = 0; i < Config::ARMY_SIZE; i++){
        QCell* qcell {new QCell{model::Cell::NORMAL, 0, 0, nullptr}};
        addChild(qcell);
    }

    setLayout(container_);
}


/* QComponent related (inherited from it) */
void QPieceStorage::compose(){
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
}

void QPieceStorage::decompose(){
    QComponent::decompose();
}

void QPieceStorage::reload(){
    QComponent::reload();
}

void QPieceStorage::connectSlots(){
    for(QComponent* component : children_){
        QCell* cell {dynamic_cast<QCell*>(component)};
        QObject::connect(cell, &QCell::cellHovered, this, &QPieceStorage::hovered);
        QObject::connect(cell, &QCell::cellLeaved, this, &QPieceStorage::leaved);
        QObject::connect(cell, &QCell::cellClicked, this, &QPieceStorage::clicked);
    }
}

void QPieceStorage::connectSlots(QCell *cell){
    QObject::connect(cell, &QCell::cellHovered, this, &QPieceStorage::hovered);
    QObject::connect(cell, &QCell::cellLeaved, this, &QPieceStorage::leaved);
    QObject::connect(cell, &QCell::cellClicked, this, &QPieceStorage::clicked);
}


/* Related to this class only */
void QPieceStorage::fillin(const std::vector<model::Piece*>& pieces){
    int counter {};
    for(model::Piece* piece : pieces){
        QCell* cell {dynamic_cast<QCell*>(children_[counter++])};
        cell -> qpiece().setPiece(piece);
        cell -> reload();
    }
}

void QPieceStorage::clear(){
    for(QComponent* component : children_){
        component -> decompose();
        component -> reload();
    }
}

bool QPieceStorage::isEmpty(){
    for(QComponent* component : children_){
        QCell* cell {dynamic_cast<QCell*>(component)};
        if(cell -> qpiece().piece())
            return false;
    }

    return true;
}


/* Slots */
void QPieceStorage::hovered(const QCell* cell){
    const model::Piece* piece {cell -> qpiece().piece()};
    if(piece){
        emit cellHovered(("Pièce " + std::string{piece -> name()} + " de rang "
                          + std::to_string(piece -> rank()) + " - " + piece -> description()).c_str());
    } else{
        emit cellHovered("Aucune pion à cet emplacement");
    }
}

void QPieceStorage::leaved(){
    emit cellLeaved();
}

void QPieceStorage::clicked(const QCell* piece){
    emit cellClicked(const_cast<QCell*>(piece));
}
