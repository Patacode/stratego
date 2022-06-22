#include "gamestuff.h"

using namespace stratego::model;

Player::Player(const PlayerInfo& info) noexcept:
    pseudo_ {info.pseudo},
    color_ {info.color},
    aliveCounter_ {Config::ARMY_SIZE},
    eatenPiecesCounter_ {},
    armyStat_ {},
    battleStat_ {},
    lastMovedPiece_ {}
{
    for(int i = Config::PIECE_MIN_RANK; i <= Config::PIECE_MAX_RANK; i++){
        armyStat_[i] = Piece::pieceInfo[i].count;
    }

    for(int i = Config::PIECE_MIN_RANK; i <= Config::PIECE_MAX_RANK; i++){
        battleStat_[i] = 0;
    }
}

bool Player::hasPiece(int rank) const noexcept{
    return armyStat_.at(rank);
}

std::string& Player::pseudo() noexcept{
    return const_cast<std::string&>(std::as_const(*this).pseudo());
}

const std::string& Player::pseudo() const noexcept{
    return pseudo_;
}

int Player::remainingPieces() const noexcept{
    return aliveCounter_;
}

int Player::remainingPieces(int rank) const{
    if(rank < Config::PIECE_MIN_RANK || rank > Config::PIECE_MAX_RANK)
        throw std::invalid_argument("The given rank is not valid");

    return armyStat_.at(rank);
}

int Player::eatenPieces() const noexcept{
    return eatenPiecesCounter_;
}

int Player::eatenPieces(int rank) const{
    if(rank < Config::PIECE_MIN_RANK || rank > Config::PIECE_MAX_RANK)
        throw std::invalid_argument("The given rank is not valid");

    return battleStat_.at(rank);
}

Color Player::color() const noexcept{
    return color_;
}

std::map<int, int>& Player::stats() noexcept{
    return const_cast<std::map<int, int>&>(std::as_const(*this).stats());
}

const std::map<int, int>& Player::stats() const noexcept{
    return armyStat_;
}

void Player::update(std::initializer_list<Observable*> args){
    Piece* p;
    for(Observable* obsv : args){
        if((p = dynamic_cast<Piece*>(obsv))){
            if(p -> color() == color_){
                if(lastMovedPiece_ && p != lastMovedPiece_ && p -> hasMove()){
                    lastMovedPiece_ -> resetCounter();
                }

                if(!p -> alive()){
                    --aliveCounter_;
                    --armyStat_[p -> rank()];
                }

                if(p -> hasMove())
                    lastMovedPiece_ = p;
            } else if(p -> color() != color_ && !(p -> alive())){
                eatenPiecesCounter_ += 1;
                battleStat_[p -> rank()] += 1;
            }
        }
    }
}

bool Player::hasLost() const noexcept{
    return !hasPiece(Config::PIECE_FLAG_INFO.rank) ||
           remainingPieces() == remainingPieces(Config::PIECE_BOMB_INFO.rank)
            + remainingPieces(Config::PIECE_FLAG_INFO.rank);
}
