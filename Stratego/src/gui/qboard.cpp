#include <QPushButton>
#include <QMimeData>
#include <regex>
#include <../core/util.h>

#include "qboard.h"

using namespace stratego::view;

QBoard::QBoard(const Model* model, QWidget* parent) :
    QFrame{parent},
    container_ {new QGridLayout},
    cells_ {},
    model_ {model},
    lastClickedCells_ {}
{
    const model::Board& board {model_ -> board()};
    for(int i = 0; i < board.size(); i++){
        for(int j = 0; j < board.size(); j++){
            model::Cell boardCell {board.getCell(j, i)};
            QCell* qcell {new QCell{boardCell.type, i, j, boardCell.piece}};

            if(boardCell.type == model::Cell::NORMAL){
                cells_.push_back(qcell);
            }

            addChild(qcell);
        }
    }

    setLayout(container_);
    setAcceptDrops(true);
}


/* QComponent related (inherited from it) */
void QBoard::compose(){
    setContentsMargins(20, 0, 20, 0);
    container_ -> setSpacing(3);
    for(QComponent* component : children_){
        QCell* cell {dynamic_cast<QCell*>(component)};
        container_ -> addWidget(cell, cell -> row(), cell -> col());
        cell -> compose();
        connectSlots(cell);
    }
}

void QBoard::decompose(){
    QComponent::decompose();
}

void QBoard::reload(){
    const model::Board& board {model_ -> board()};
    if(!lastClickedCells_.empty()){
        for(QCell* cell : lastClickedCells_){
            model::Cell currentCell {board.getCell(cell -> col(), cell -> row())};
            cell -> qpiece().setPiece(currentCell.piece);
            cell -> reload();
        }

        lastClickedCells_.clear();
    } else{
        for(QCell* cell : cells_){
            model::Cell currentCell {board.getCell(cell -> col(), cell -> row())};
            cell -> qpiece().setPiece(currentCell.piece);
            cell -> reload();
        }
    }
}

void QBoard::reload(model::Color color, bool setup){
    const model::Board& board {model_ -> board()};
    if(setup){
        int bound {color == model::Color::BLUE ? Config::ARMY_SIZE : static_cast<int>(cells_.size())};
        for(int i {color == model::Color::BLUE ? 0 : Config::ARMY_SIZE + 12}; i < bound; i++){
            QCell* cell {cells_[i]};
            model::Cell currentCell {board.getCell(cell -> col(), cell -> row())};

            cell -> qpiece().setPiece(currentCell.piece);
            cell -> reload();
        }
    } else{
        for(QCell* cell : cells_){
            if(cell -> qpiece().piece() && cell -> qpiece().piece() -> color() == color){
                model::Cell currentCell {board.getCell(cell -> col(), cell -> row())};

                cell -> qpiece().setPiece(currentCell.piece);
                cell -> reload();
            }
        }
    }
}

void QBoard::connectSlots(){
    for(QComponent* component : children_){
        QCell* cell {dynamic_cast<QCell*>(component)};
        QObject::connect(cell, &QCell::cellHovered, this, &QBoard::hovered);
        QObject::connect(cell, &QCell::cellLeaved, this, &QBoard::leaved);
        QObject::connect(cell, &QCell::cellClicked, this, &QBoard::clicked);
    }
}

void QBoard::connectSlots(QCell* cell){
    QObject::connect(cell, &QCell::cellHovered, this, &QBoard::hovered);
    QObject::connect(cell, &QCell::cellLeaved, this, &QBoard::leaved);
    QObject::connect(cell, &QCell::cellClicked, this, &QBoard::clicked);
}


void QBoard::dragEnterEvent(QDragEnterEvent *event){
    event -> accept();
}

void QBoard::dropEvent(QDropEvent *event){
    QString filepath {event -> mimeData() -> text()};
    std::string filename {util::getFileName(filepath.toStdString())};
    std::string extension {util::getFileExtension(filepath.toStdString())};

    if(!std::regex_match(filename, std::regex{"[[:alnum:]]+"})){
        emit errorOccured("Nom de fichier invalide. Seul les caractères alphanumériques sont acceptés.");
    } else if(!extension.empty() && extension != ".txt"){
        emit errorOccured("Fichier invalide. L'extension du fichier n'est pas correct.");
    } else{
        filepath = adjustAbsolutePath(filepath.toStdString()).c_str();
        emit fileDropped(filepath);
    }
}


/* Related to this class only */
void QBoard::saveToFile(const std::string& filename, model::Color color){
    std::ofstream ofs {filename};
    int bound {color == model::Color::BLUE ? -4 : Config::BOARD_SIZE - 5};
    for(int i {color == model::Color::BLUE ? -1 : Config::BOARD_SIZE - 2}; i >= bound; i--){
        for(int j = 1; j < Config::BOARD_SIZE - 1; j++){
            QWidget* item {container_ -> itemAtPosition(std::abs(i), j) -> widget()};
            QCell* cell {dynamic_cast<QCell*>(item)};
            std::string pieceSymbol {cell -> qpiece().piece() -> info().symbol};
            ofs << pieceSymbol << (j == Config::BOARD_SIZE - 2 ? "" : " ");
        }
        ofs << "\n";
    }
}

std::string QBoard::adjustAbsolutePath(const std::string& filepath){
    size_t pointer {filepath.find(':') + 1};
    while(filepath[pointer] == '/') pointer++;
#ifdef __WIN32
    return filepath.substr(pointer);
#else
    return filepath.substr(pointer - 1);
#endif
}


void QBoard::clear(model::Color color){
    model::Board& board {const_cast<model::Board&>(model_ -> board())};
    int bound {color == model::Color::BLUE ? Config::ARMY_SIZE : static_cast<int>(cells_.size())};
    for(int i {color == model::Color::BLUE ? 0 : Config::ARMY_SIZE + 12}; i < bound; i++){
        QCell* cell {cells_[i]};

        board.getCell(cell -> col(), cell -> row()).piece = nullptr;
        cell -> decompose();
        cell -> reload();
    }
}

void QBoard::hideColor(model::Color color){
    for(QCell* cell : cells_){
        if(cell -> qpiece().piece() && cell -> qpiece().piece() -> color() == color){
            cell -> qpiece().piece() -> setRevealed(false);
            cell -> reload();
        }
    }
}

void QBoard::highlightPossibleMoves(const model::Piece* piece){
    for(int i = model::Direction::UP; i <= model::Direction::RIGHT; i++){
        model::Direction::Value currentDirection {static_cast<model::Direction::Value>(i)};
        model::Position currentPos {piece -> position()};
        while(piece -> canMove(currentPos + currentDirection)){
            currentPos = currentPos + currentDirection;
            QCell* currentCell {dynamic_cast<QCell*>(container_ -> itemAtPosition(currentPos.y, currentPos.x) -> widget())};
            currentCell -> highlight(true);
        }
    }
}

void QBoard::clearHighlights(){
    for(QCell* cell : cells_){
        if(cell -> type() == model::Cell::NORMAL && !cell -> qpiece().piece()){
            cell -> highlight(false);
        }
    }
}

bool QBoard::isFull(model::Color color){
    int i {color == model::Color::BLUE ? 0 : Config::ARMY_SIZE + 12};
    int bound {color == model::Color::BLUE ? Config::ARMY_SIZE : static_cast<int>(cells_.size())};
    while(i < bound && cells_[i] -> qpiece().piece()) i++;
    return i == bound;
}


/* Slots */
void QBoard::hovered(const QCell* cell){
    const model::Piece* piece {cell -> qpiece().piece()};
    model::Cell::Type type {cell -> type()};

    // highlighting of possible pieces move
    if(piece && piece -> revealed()){
        highlightPossibleMoves(piece);
    }

    // hover info
    if(piece && piece -> revealed()){
        emit cellHovered(("Pièce " + std::string{piece -> name()} + " de rang "
                          + std::to_string(piece -> rank()) + " - " + piece -> description()).c_str());
    } else if(piece && !piece -> revealed()){
        std::string color {piece -> color() == model::Color::RED ? "rouge" : "bleu"};
        emit cellHovered(("Pièce caché de couleur " + color).c_str());
    } else{
        emit cellHovered(("Case de type " + model::Cell::typeNames[type]).c_str());
    }
}

void QBoard::leaved(QCell *cell){
    const model::Piece* piece {cell -> qpiece().piece()};
    if(piece && piece -> revealed()){
        clearHighlights();
    }
    emit cellLeaved();
}

void QBoard::clicked(const QCell* cell){
    lastClickedCells_.push_back(const_cast<QCell*>(cell));
    emit cellClicked(const_cast<QCell*>(cell));
}
