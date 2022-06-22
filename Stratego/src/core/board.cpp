#include "gamestuff.h"

using namespace stratego::model;
Board::Board() noexcept{
    int bs {Config::BOARD_SIZE};

    /* top and bottom walls */
    for(int i = 0; i < bs; i += bs - 1){
        for(int j = 0; j < bs; j++){
            board_[i][j] = {{j, i}, Cell::WALL, nullptr};
        }
    }

    /* left and right walls */
    for(int i = 1; i < bs - 1; i++){
        for(int j = 0; j < bs; j += bs - 1){
            board_[i][j] = {{j, i}, Cell::WALL, nullptr};
        }
    }

    /* normal cells */
    for(int i = 1; i < bs - 1; i++){
        for(int j = 1; j < bs - 1; j++){
            board_[i][j] = {{j, i}, Cell::NORMAL, nullptr};
        }
    }

    /* water cells */
    board_[bs / 2 - 1][3].type = Cell::WATER;
    board_[bs / 2 - 1][4].type = Cell::WATER;
    board_[bs / 2][3].type = Cell::WATER;
    board_[bs / 2][4].type = Cell::WATER;

    board_[bs / 2 - 1][bs - 4].type = Cell::WATER;
    board_[bs / 2 - 1][bs - 5].type = Cell::WATER;
    board_[bs / 2][bs - 4].type = Cell::WATER;
    board_[bs / 2][bs - 5].type = Cell::WATER;
}

Piece* Board::getPiece(int x, int y){
    return const_cast<Piece*>(std::as_const(*this).getPiece(x, y));
}

const Piece* Board::getPiece(int x, int y) const{
    if(x < 0 || x >= size())
        throw std::out_of_range("x coordinate out of range");
    if(y < 0 || y >= size())
        throw std::out_of_range("y coordinate out of range");

    return board_[y][x].piece;
}

Piece* Board::getPiece(const Position &pos){
    return const_cast<Piece*>(std::as_const(*this).getPiece(pos));
}

const Piece* Board::getPiece(const Position &pos) const{
    if(pos.x < 0 || pos.x >= size())
        throw std::out_of_range("x coordinate out of range");
    if(pos.y < 0 || pos.y >= size())
        throw std::out_of_range("y coordinate out of range");

    return board_[pos.y][pos.x].piece;
}

Cell& Board::getCell(int x, int y){
    return const_cast<Cell&>(std::as_const(*this).getCell(x, y));
}

const Cell& Board::getCell(int x, int y) const{
    if(x < 0 || x >= size())
        throw std::out_of_range("x coordinate out of range");
    if(y < 0 || y >= size())
        throw std::out_of_range("y coordinate out of range");

    return board_[y][x];
}

Cell& Board::getCell(const Position& pos){
    return const_cast<Cell&>(std::as_const(*this).getCell(pos));
}

const Cell& Board::getCell(const Position &pos) const{
    if(pos.x < 0 || pos.x >= size())
        throw std::out_of_range("x coordinate out of range");
    if(pos.y < 0 || pos.y >= size())
        throw std::out_of_range("y coordinate out of range");

    return board_[pos.y][pos.x];
}

bool Board::walkableCell(const Position &pos) const{
    if(pos.x < 0 || pos.x >= size())
        throw std::out_of_range("x coordinate out of range");
    if(pos.y < 0 || pos.y >= size())
        throw std::out_of_range("y coordinate out of range");

    return !board_[pos.y][pos.x].piece && board_[pos.y][pos.x].type == Cell::NORMAL;
}

bool Board::walkableCell(int x, int y) const{
    if(x < 0 || x >= size())
        throw std::out_of_range("x coordinate out of range");
    if(y < 0 || y >= size())
        throw std::out_of_range("y coordinate out of range");

    return walkableCell({x, y});
}

int Board::size() const noexcept{
    return board_.size();
}

const std::array<Cell, stratego::Config::BOARD_SIZE>* Board::begin() const{
    return &board_[0];
}

const std::array<Cell, stratego::Config::BOARD_SIZE>* Board::end() const{
    return &board_[board_.size()];
}

Board::~Board(){
    for(int i = 0; i < size(); i++){
        for(int j = 0; j < size(); j++){
            if(board_[i][j].piece){
                delete board_[i][j].piece;
                board_[i][j].piece = nullptr;
            }
        }
    }
}

bool Board::isInside(const Position& pos) const noexcept{
    return pos.x >= 0 &&
           pos.x < size() &&
           pos.y >= 0 &&
           pos.y <  size();
}
