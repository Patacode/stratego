#include "piece.h"
#include "util.h"

#include <regex>

using namespace stratego::model;
using namespace stratego;

/* ========================== Piece =========================== */
const std::array<PieceInfo, Config::PIECE_BOMB_INFO.rank + 1> Piece::pieceInfo{
    stratego::Config::PIECE_FLAG_INFO,
    stratego::Config::PIECE_SPY_INFO,
    stratego::Config::PIECE_SCOUT_INFO,
    stratego::Config::PIECE_MINER_INFO,
    stratego::Config::PIECE_SERGENT_INFO,
    stratego::Config::PIECE_LIEUTENANT_INFO,
    stratego::Config::PIECE_CAPTAIN_INFO,
    stratego::Config::PIECE_MAJOR_INFO,
    stratego::Config::PIECE_COLONEL_INFO,
    stratego::Config::PIECE_GENERAL_INFO,
    stratego::Config::PIECE_MARSHAL_INFO,
    stratego::Config::PIECE_BOMB_INFO
};

Piece::operator std::string() const{
    return std::string{info_.symbol} + "-" + std::string{currentPos_};
}

Piece::Piece(const PieceInfo& info, const Position& initPos, Color color, std::function<bool(int, int)> winPredicate, Board& board, StateGraph& graph, History& hist) noexcept:
    observers_ {},
    color_ {color},
    info_ {info},
    revealed_ {true},
    winPredicate_ {winPredicate},
    currentPos_ {initPos},
    graph_ {graph},
    hist_ {hist},
    board_ {board},
    alive_ {true},
    isMovable_ {true},
    hasBeenInCombat_ {},
    recordedPos_ {0, 0},
    bnfCounter_ {},
    hasMove_{}
{}

int Piece::rank() const noexcept{
    return info_.rank;
}

Position& Piece::position() noexcept{
    return const_cast<Position&>(std::as_const(*this).position());
}

const Position& Piece::position() const noexcept{
    return currentPos_;
}

void Piece::setPosition(const Position &pos) noexcept{
    currentPos_ = pos;
}

Color Piece::color() const noexcept{
    return color_;
}

bool Piece::alive() const noexcept{
    return alive_;
}

bool Piece::revealed() const noexcept{
    return revealed_;
}

void  Piece::setRevealed(bool state) noexcept{
    revealed_ = state;
}

std::string_view& Piece::name() noexcept{
    return const_cast<std::string_view&>(std::as_const(*this).name());
}

const std::string_view& Piece::name() const noexcept{
    return info_.name;
}

std::string Piece::description() const noexcept{
    return Piece::description(info_.rank);
}

PieceInfo Piece::info() const noexcept{
    return info_;
}

bool Piece::isMovable() const noexcept{
    return isMovable_;
}

bool Piece::hasMove() const noexcept{
    return hasMove_;
}

void Piece::resetCounter() noexcept{
    bnfCounter_ = 0;
}

void Piece::move(const Position& pos) noexcept{
    if(!canMove(pos)){
        hasMove_ = false;
        hist_.addFailure("Déplacement invalide. Référez vous aux règles pour en déterminer la cause.");
        hist_.addHint("Tapez RULES pour afficher les règles ou tapez PIECE <id> pour afficher la "
                      "description d'une pièce d'id donné.");
        graph_.consume(StateGraph::FACT);
    } else{
        if(recordedPos_ == Position{0, 0}){
            recordedPos_ = currentPos_;
        } else{
            if(pos == recordedPos_){
                bnfCounter_++;
            } else if(!(currentPos_ == recordedPos_)){
                bnfCounter_ = 0;
                recordedPos_ = currentPos_;
            }
        }

        board_.getCell(currentPos_).piece = nullptr;
        board_.getCell(pos).piece = this;
        currentPos_ = pos;
        hasMove_ = true;

        hist_.addSuccess("Déplacement du pion "
                         + std::string{info_.name}
                         + " en "
                         + std::string{pos}
                         + " par le joueur "
                         + (color_ == Color::RED ? "rouge" : "bleu"));
        graph_.consume(StateGraph::ACT);

    }

    notifyObservers({this});
}

void Piece::attack(const Position &pos) noexcept{
    Piece* opponentPiece {};
    std::string explicitCause {};
    if(!canAttack(pos)){
        hasMove_ = false;
        hist_.addFailure("Attaque invalide. Référez vous aux règles pour en déterminer la cause.");
        hist_.addHint("Tapez RULES pour afficher les règles ou tapez PIECE <id> pour afficher la"
                      "description d'une pièce d'id donné.");
        graph_.consume(StateGraph::FACT);
    } else{
        // to prevent pieces from going back n forth
        if(recordedPos_ == Position{0, 0}){
            recordedPos_ = currentPos_;
        } else{
            if(pos == recordedPos_){
                bnfCounter_++;
            } else if(!(currentPos_ == recordedPos_)){
                bnfCounter_ = 0;
                recordedPos_ = currentPos_;
            }
        }

        opponentPiece = board_.getPiece(pos);
        if(winPredicate_(info_.rank, opponentPiece -> info_.rank)){ // win battle
            board_.getCell(currentPos_).piece = nullptr;
            board_.getCell(pos).piece = this;
            opponentPiece -> alive_ = false;
            currentPos_ = pos;
            explicitCause.append("Le pion " + std::string{info_.name} + " du joueur "
                                 + (color_ == Color::RED ? "rouge" : "bleu")
                                 + " a gagné le combat.");
        } else{ // lose or draw
            board_.getCell(currentPos_).piece = nullptr;
            alive_ = false;

            if(opponentPiece -> info_.rank == info_.rank){ // draw
                board_.getCell(pos).piece = nullptr;
                opponentPiece -> alive_ = false;
                explicitCause.append("Les deux pions de rang égal on perdu.");
            } else{
                explicitCause.append("Le pion " + std::string{opponentPiece -> info_.name} + " du joueur "
                                     + (opponentPiece -> color_ == Color::RED ? "rouge" : "bleu")
                                     + " a gagné le combat.");
            }
        }

        hist_.addSuccess("Attaque du pion " +
            std::string{opponentPiece -> info_.name} +
            " en " +
            std::string{pos} +
            " par le pion " +
            std::string{info_.name} + " en " +
            std::string{currentPos_} +
            ". " + explicitCause);
        graph_.consume(StateGraph::ACT);
        hasBeenInCombat_ = true;
        opponentPiece -> hasBeenInCombat_ = true;
        opponentPiece -> revealed_ = true;
        revealed_ = true;
        hasMove_ = true;
    }

    notifyObservers({this, opponentPiece});
}

bool Piece::canMove(const Position &pos) const noexcept{
    return isMovable_ &&
        board_.isInside(pos) &&
        board_.walkableCell(pos) &&
        (bnfCounter_ < Config::MAX_BNF || (bnfCounter_ >= Config::MAX_BNF && !(pos == recordedPos_)));
}

bool Piece::canAttack(const Position &pos) const noexcept{
    return isMovable_ &&
        board_.isInside(pos) &&
        board_.getPiece(pos) &&
        board_.getPiece(pos) -> color() != color_;
}

int Piece::toRank(const std::string &str){
    if(std::regex_match(str, std::regex{"( (10|[1-9]|D|B))", std::regex_constants::icase})){
        throw std::invalid_argument("Invalid string to convert to rank");
    }

    int result;
    std::string op {str};
    util::strtoupper(op);
    if(op == std::string{Config::PIECE_FLAG_INFO.symbol}) result = Config::PIECE_FLAG_INFO.rank;
    else if(op == std::string{Config::PIECE_BOMB_INFO.symbol}) result = Config::PIECE_BOMB_INFO.rank;
    else result = std::stoi(op);

    return result;
}

std::string Piece::description(int rank){
    if(rank < Config::PIECE_MIN_RANK || rank > Config::PIECE_MAX_RANK){
        std::cout << rank << std::endl;
        throw std::invalid_argument("The given rank is not valid");
    }

    std::string name {pieceInfo[rank].name};
    util::strtolower(name);
    return Config::PIECE_DATA.propertyOf(name + ".desc");
}

bool Piece::hasBeenInCombat() const noexcept{
    return hasBeenInCombat_;
}

void Piece::addObserver(Observer* obs) noexcept{
    observers_.push_back(obs);
}

void Piece::removeObserver(Observer* obs) noexcept{
    size_t i {};
    while(i < observers_.size() && observers_[i] != obs) i++;
    if(i < observers_.size())
        observers_.erase(observers_.begin() + i);

}

void Piece::notifyObservers(std::initializer_list<Observable*> infos) const noexcept{
    for(auto& obs : observers_){
        obs -> update(infos);
    }
}

/* ========================== Marshal =========================== */
Marshal::Marshal(const Position& initPos, Color color, Board& board, StateGraph& graph, History& hist):
    Piece {Config::PIECE_MARSHAL_INFO,
        initPos,
        color,
        [](int crank, int orank){return crank > orank;},
        board,
        graph,
        hist}
{}

bool Marshal::canMove(const Position& pos) const noexcept{
    return Piece::canMove(pos) &&
        ((std::abs(currentPos_.y - pos.y) == 1 && currentPos_.x == pos.x) ||
        (std::abs(currentPos_.x - pos.x) == 1 && currentPos_.y == pos.y));
}

bool Marshal::canAttack(const Position& pos) const noexcept{
    return Piece::canAttack(pos) &&
        ((std::abs(currentPos_.y - pos.y) == 1 && currentPos_.x == pos.x) ||
        (std::abs(currentPos_.x - pos.x) == 1 && currentPos_.y == pos.y));
}


/* ========================== General =========================== */
General::General(const Position& initPos, Color color, Board& board, StateGraph& graph, History& hist):
    Piece {Config::PIECE_GENERAL_INFO,
        initPos,
        color,
        [](int crank, int orank){return crank > orank;},
        board,
        graph,
        hist}
{}

bool General::canMove(const Position& pos) const noexcept{
     return Piece::canMove(pos)&&
        ((std::abs(currentPos_.y - pos.y) == 1 && currentPos_.x == pos.x) ||
        (std::abs(currentPos_.x - pos.x) == 1 && currentPos_.y == pos.y));
}

bool General::canAttack(const Position& pos) const noexcept{
    return Piece::canAttack(pos)&&
        ((std::abs(currentPos_.y - pos.y) == 1 && currentPos_.x == pos.x) ||
        (std::abs(currentPos_.x - pos.x) == 1 && currentPos_.y == pos.y));
}


/* ========================== Colonel =========================== */
Colonel::Colonel(const Position& initPos, Color color, Board& board, StateGraph& graph, History& hist):
    Piece {Config::PIECE_COLONEL_INFO,
        initPos,
        color,
        [](int crank, int orank){return crank > orank;},
        board,
        graph,
        hist}
{}

bool Colonel::canMove(const Position& pos) const noexcept{
    return Piece::canMove(pos)&&
        ((std::abs(currentPos_.y - pos.y) == 1 && currentPos_.x == pos.x) ||
        (std::abs(currentPos_.x - pos.x) == 1 && currentPos_.y == pos.y));
}

bool Colonel::canAttack(const Position& pos) const noexcept{
    return Piece::canAttack(pos)&&
        ((std::abs(currentPos_.y - pos.y) == 1 && currentPos_.x == pos.x) ||
        (std::abs(currentPos_.x - pos.x) == 1 && currentPos_.y == pos.y));
}


/* ========================== Major =========================== */
Major::Major(const Position& initPos, Color color, Board& board, StateGraph& graph, History& hist):
    Piece {Config::PIECE_MAJOR_INFO,
        initPos,
        color,
        [](int crank, int orank){return crank > orank;},
        board,
        graph,
        hist}
{}

bool Major::canMove(const Position& pos) const noexcept{
    return Piece::canMove(pos)&&
        ((std::abs(currentPos_.y - pos.y) == 1 && currentPos_.x == pos.x) ||
        (std::abs(currentPos_.x - pos.x) == 1 && currentPos_.y == pos.y));
}

bool Major::canAttack(const Position& pos) const noexcept{
    return Piece::canAttack(pos)&&
        ((std::abs(currentPos_.y - pos.y) == 1 && currentPos_.x == pos.x) ||
        (std::abs(currentPos_.x - pos.x) == 1 && currentPos_.y == pos.y));
}


/* ========================== Captain =========================== */
Captain::Captain(const Position& initPos, Color color, Board& board, StateGraph& graph, History& hist):
    Piece {Config::PIECE_CAPTAIN_INFO,
        initPos,
        color,
        [](int crank, int orank){return crank > orank;},
        board,
        graph,
        hist}
{}

bool Captain::canMove(const Position& pos) const noexcept{
    return Piece::canMove(pos) &&
        ((std::abs(currentPos_.y - pos.y) == 1 && currentPos_.x == pos.x) ||
        (std::abs(currentPos_.x - pos.x) == 1 && currentPos_.y == pos.y));
}

bool Captain::canAttack(const Position& pos) const noexcept{
    return Piece::canAttack(pos)&&
        ((std::abs(currentPos_.y - pos.y) == 1 && currentPos_.x == pos.x) ||
        (std::abs(currentPos_.x - pos.x) == 1 && currentPos_.y == pos.y));
}


/* ========================== Lieutenant =========================== */
Lieutenant::Lieutenant(const Position& initPos, Color color, Board& board, StateGraph& graph, History& hist):
    Piece {Config::PIECE_LIEUTENANT_INFO,
        initPos,
        color,
        [](int crank, int orank){return crank > orank;},
        board,
        graph,
        hist}
{}

bool  Lieutenant::canMove(const Position& pos) const noexcept{
    return Piece::canMove(pos)&&
        ((std::abs(currentPos_.y - pos.y) == 1 && currentPos_.x == pos.x) ||
        (std::abs(currentPos_.x - pos.x) == 1 && currentPos_.y == pos.y));
}

bool  Lieutenant::canAttack(const Position& pos) const noexcept{
    return Piece::canAttack(pos) &&
        ((std::abs(currentPos_.y - pos.y) == 1 && currentPos_.x == pos.x) ||
        (std::abs(currentPos_.x - pos.x) == 1 && currentPos_.y == pos.y));
}


/* ========================== Sergent =========================== */
Sergent::Sergent(const Position& initPos, Color color, Board& board, StateGraph& graph, History& hist):
    Piece {Config::PIECE_SERGENT_INFO,
        initPos,
        color,
        [](int crank, int orank){return crank > orank;},
        board,
        graph,
        hist}
{}

bool Sergent::canMove(const Position& pos) const noexcept{
    return Piece::canMove(pos)&&
        ((std::abs(currentPos_.y - pos.y) == 1 && currentPos_.x == pos.x) ||
        (std::abs(currentPos_.x - pos.x) == 1 && currentPos_.y == pos.y));
}

bool Sergent::canAttack(const Position& pos) const noexcept{
    return Piece::canAttack(pos)&&
        ((std::abs(currentPos_.y - pos.y) == 1 && currentPos_.x == pos.x) ||
        (std::abs(currentPos_.x - pos.x) == 1 && currentPos_.y == pos.y));
}


/* ========================== Miner =========================== */
Miner::Miner(const Position& initPos, Color color, Board& board, StateGraph& graph, History& hist):
    Piece {Config::PIECE_MINER_INFO,
        initPos,
        color,
        [](int crank, int orank){return crank > orank || orank == Config::PIECE_BOMB_INFO.rank;},
        board,
        graph,
        hist}
{}

bool Miner::canMove(const Position& pos) const noexcept{
    return Piece::canMove(pos) &&
        ((std::abs(currentPos_.y - pos.y) == 1 && currentPos_.x == pos.x) ||
        (std::abs(currentPos_.x - pos.x) == 1 && currentPos_.y == pos.y));
}

bool Miner::canAttack(const Position& pos) const noexcept{
    return Piece::canAttack(pos)&&
        ((std::abs(currentPos_.y - pos.y) == 1 && currentPos_.x == pos.x) ||
        (std::abs(currentPos_.x - pos.x) == 1 && currentPos_.y == pos.y));
}


/* ========================== Scout =========================== */
Scout::Scout(const Position& initPos, Color color, Board& board, StateGraph& graph, History& hist):
    Piece {Config::PIECE_SCOUT_INFO,
        initPos,
        color,
        [](int crank, int orank){return crank > orank;},
        board,
        graph,
        hist}
{}

bool Scout::canMove(const Position& pos) const noexcept{
    if(!Piece::canMove(pos) ||
        (std::abs(currentPos_.x - pos.x) && currentPos_.y != pos.y) ||
        (std::abs(currentPos_.y - pos.y) && currentPos_.x != pos.x))
    {
        return false;
    }

    int coord; // 0 for y, 1 for x
    int inc;
    if(currentPos_.y - pos.y != 0){ // move up or down
        coord = 0;
        inc = currentPos_.y - pos.y < 0 ? 1 : -1;
    } else{ // move left or right
        coord = 1;
        inc = currentPos_.x - pos.x < 0 ? 1 : -1;
    }

    if(coord){
        for(int i = currentPos_.x + inc; i != pos.x; i += inc){
            if(!board_.walkableCell(i, currentPos_.y))
                return false;
        }
    } else{
        for(int i = currentPos_.y + inc; i != pos.y; i += inc){
            if(!board_.walkableCell(currentPos_.x, i))
                return false;
        }
    }

    return  true;
}

bool Scout::canAttack(const Position& pos) const noexcept{
    if(!Piece::canAttack(pos) ||
        (std::abs(currentPos_.x - pos.x) && currentPos_.y != pos.y) ||
        (std::abs(currentPos_.y - pos.y) && currentPos_.x != pos.x))
    {
        return false;
    }

    int coord; // 0 for y, 1 for x
    int inc;
    if(currentPos_.y - pos.y != 0){ // move up or down
        coord = 0;
        inc = currentPos_.y - pos.y < 0 ? 1 : -1;
    } else{ // move left or right
        coord = 1;
        inc = currentPos_.x - pos.x < 0 ? 1 : -1;
    }

    if(coord){
        for(int i = currentPos_.x + inc; i != pos.x; i += inc){
            if(!board_.walkableCell(i, currentPos_.y))
                return false;
        }
    } else{
        for(int i = currentPos_.y + inc; i != pos.y; i += inc){
            if(!board_.walkableCell(currentPos_.x, i))
                return false;
        }
    }

    return true;
}


/* ========================== Spy =========================== */
Spy::Spy(const Position& initPos, Color color, Board& board, StateGraph& graph, History& hist):
    Piece {Config::PIECE_SPY_INFO,
        initPos,
        color,
        [](int crank, int orank){return crank > orank || orank == Config::PIECE_MARSHAL_INFO.rank;},
        board,
        graph,
        hist}
{}

bool Spy::canMove(const Position& pos) const noexcept{
    return Piece::canMove(pos)&&
        ((std::abs(currentPos_.y - pos.y) == 1 && currentPos_.x == pos.x) ||
        (std::abs(currentPos_.x - pos.x) == 1 && currentPos_.y == pos.y));
}

bool Spy::canAttack(const Position& pos) const noexcept{
    return Piece::canAttack(pos)&&
        ((std::abs(currentPos_.y - pos.y) == 1 && currentPos_.x == pos.x) ||
        (std::abs(currentPos_.x - pos.x) == 1 && currentPos_.y == pos.y));
}


/* ========================== Bomb =========================== */
Bomb::Bomb(const Position& initPos, Color color, Board& board, StateGraph& graph, History& hist):
    Piece {Config::PIECE_BOMB_INFO,
        initPos,
        color,
        [](int crank, int orank){return crank > orank;},
        board,
        graph,
        hist}
{
    isMovable_ = false;
}

bool Bomb::canMove(const Position& pos) const noexcept{
     return Piece::canMove(pos);
}

bool Bomb::canAttack(const Position& pos) const noexcept{
    return Piece::canAttack(pos);
}


/* ========================== Flag =========================== */
Flag::Flag(const Position& initPos, Color color, Board& board, StateGraph& graph, History& hist):
    Piece {Config::PIECE_FLAG_INFO,
        initPos,
        color,
        [](int crank, int orank){return crank > orank;},
        board,
        graph,
        hist}
{
    isMovable_ = false;
}

bool Flag::canMove(const Position& pos) const noexcept{
    return Piece::canMove(pos);
}

bool Flag::canAttack(const Position& pos) const noexcept{
    return Piece::canAttack(pos);
}
