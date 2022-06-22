#include "model.h"
#include "piece.h"
#include "pieceFactory.h"

#include <iostream>

using namespace stratego;
using namespace stratego::model;


/* ========================== ModelAdapter =========================== */
ModelAdapter::ModelAdapter() :
    observers_ {},
    removedPieces_ {},
    players_ {},
    playerPointer_ {-1},
    board_ {},
    history_ {1024},
    graph_ {}
{}

void ModelAdapter::init(){
    if(!graph_.canConsume(StateGraph::INI)){
        throw std::logic_error("Current model's state doesn't allow this method to be called");
    }

    for(Piece*& p : removedPieces_){
        delete p;
    }
    removedPieces_.clear();
    board_.~Board();
    history_.clear();
    playerPointer_ = -1;
    for(Player* player : players_){
        if(player)
            delete player;
    }

    graph_.consume(StateGraph::INI);
    notifyObservers({this});
}

void ModelAdapter::load(const std::string &filename, Color color, bool isPathAbsolute){
    if(!graph_.canConsume(StateGraph::LOAD) || !graph_.canConsume(StateGraph::FLOAD)){
        throw std::logic_error("Current model's state doesn't allow this method to be called");
    }

    parseFor(filename, color, isPathAbsolute);
    notifyObservers({this});
}

void ModelAdapter::setup(const std::string& redPseudo, const std::string& bluePseudo){
    if(!graph_.canConsume(StateGraph::SET)){
        throw std::logic_error("Current model's state doesn't allow this method to be called");
    }

    players_[0] = new Player {{redPseudo, Color::RED}};
    players_[1] = new Player {{bluePseudo, Color::BLUE}};

    for(int i = 1; i < board_.size() - 1; i++){
        for(int j = 1; j < board_.size() - 1; j++){
            Cell& cell {board_.getCell(j, i)};
            if(cell.piece){
                cell.piece -> addObserver(players_[1]);
                cell.piece -> addObserver(players_[0]);
                cell.piece -> addObserver(this);
            }
        }
    }

    history_.addSuccess("Partie correctement configurée. Bon jeu !");
    graph_.consume(StateGraph::SET);
    notifyObservers({this});
}

void ModelAdapter::nextPlayer(){
    if(!graph_.canConsume(StateGraph::NEXT)){
        throw std::logic_error("Current model's state doesn't allow this method to be called");
    }

    playerPointer_ = (playerPointer_ + 1) % players_.size();
    for(int i = 1; i < board_.size() - 1; i++){
        for(int j = 1; j < board_.size() - 1; j++){
            Cell& cell {board_.getCell(j, i)};
            if(cell.piece){
                if(cell.piece -> color() == currentPlayer().color())
                    cell.piece -> setRevealed(true);
                else
                    cell.piece -> setRevealed(false);
            }
        }
    }

    graph_.consume(StateGraph::NEXT);
    notifyObservers({this});
}

void ModelAdapter::stop(){
    if(!graph_.canConsume(StateGraph::STOP)){
        throw std::logic_error("Current model's state doesn't allow this method to be called");
    }

    history_.addSuccess("Partie terminée. À très bientôt !");
    graph_.consume(StateGraph::STOP);
    notifyObservers({this});
}

void ModelAdapter::errorProcessed(){
    if(!graph_.canConsume(StateGraph::ERRCS)){
        throw std::logic_error("Current model's state doesn't allow this method to be called");
    }

    graph_.consume(StateGraph::ERRCS);
    notifyObservers({this});
}

void ModelAdapter::nextTurn(){
    if(!graph_.canConsume(StateGraph::CHK) || !graph_.canConsume(StateGraph::FCHK)){
        throw std::logic_error("Current model's state doesn't allow this method to be called");
    }

    if(players_[0] -> hasLost() && players_[1] -> hasLost()){
        graph_.consume(StateGraph::CHK);
        history_.addSuccess("Les deux joueurs ont gagnés");
    }else if(players_[0] -> hasLost() || players_[1] -> hasLost()){
        graph_.consume(StateGraph::CHK);
        if(players_[0] -> hasLost()){
             history_.addSuccess(players_[1] -> pseudo() + " a gagné");
        }else{
            history_.addSuccess(players_[0] -> pseudo() + " a gagné");
        }
    }else{
        bool p0CanMove {playerCanMove(players_[0] -> color())};
        bool p1CanMove {playerCanMove(players_[1] -> color())};

        if(!p0CanMove && !p1CanMove){
            graph_.consume(StateGraph::CHK);
            history_.addSuccess("Les deux joueurs ont gagnés car ils ne peuvent plus se déplacer");
        } else if(!p0CanMove || !p1CanMove){
            graph_.consume(StateGraph::CHK);
            if(!p0CanMove){
                history_.addSuccess(players_[1] -> pseudo() + " a gagné car le joueur adverse ne peut plus se déplacer");
            } else if(!p1CanMove){
                history_.addSuccess(players_[0] -> pseudo() + " a gagné car le joueur adverse ne peut plus se déplacer");
            }
        } else{ // game isn't over
            graph_.consume(StateGraph::FCHK);
        }
    }

    notifyObservers({this});
}

void ModelAdapter::replay(bool state){
    if(!graph_.canConsume(StateGraph::RWD) || !graph_.canConsume(StateGraph::END)){
        throw std::logic_error("Current model's state doesn't allow this method to be called");
    }

    if(state){
        graph_.consume(StateGraph::RWD);
    }else{
        graph_.consume(StateGraph::END);
        history_.addSuccess("Fin de partie!");
    }

    notifyObservers({this});
}

Player& ModelAdapter::currentPlayer(){
    return const_cast<Player&>(std::as_const(*this).currentPlayer());
}

const Player& ModelAdapter::currentPlayer() const{
    if(graph_.canConsume(StateGraph::INI) || graph_.canConsume(StateGraph::LOAD) || graph_.canConsume(StateGraph::SET))
        throw std::logic_error("Current model's state doesn't allow this method to be called");

    return *players_[playerPointer_];
}

StateGraph::State ModelAdapter::currentState() const{
    return graph_.state();
}

Board& ModelAdapter::board(){
    if(graph_.canConsume(StateGraph::INI))
        throw std::logic_error("Current model's state doesn't allow this method to be called");

    return board_;
}

const Board& ModelAdapter::board() const noexcept{
    return board_;
}

History& ModelAdapter::history(){
    return const_cast<History&>(std::as_const(*this).history());
}

const History& ModelAdapter::history() const{
    if(graph_.canConsume(StateGraph::INI))
        throw std::logic_error("Current model's state doesn't allow this method to be called");

    return history_;
}

std::array<Player*, stratego::Config::PLAYER_COUNT>& ModelAdapter::players(){
    return const_cast<std::array<Player*, stratego::Config::PLAYER_COUNT>&>(std::as_const(*this).players());
}

const std::array<Player*, stratego::Config::PLAYER_COUNT>& ModelAdapter::players() const{
    if(graph_.canConsume(StateGraph::INI) || graph_.canConsume(StateGraph::LOAD) || graph_.canConsume(StateGraph::SET))
        throw std::logic_error("Current model's state doesn't allow this method to be called");

    return players_;
}

void ModelAdapter::update(std::initializer_list<Observable*> args){
    // filter args to delete (args processed by the players -> Pieces)
    Piece* p;
    for(Observable* obs : args){
        if((p = dynamic_cast<Piece*>(obs)) && !p -> alive()){
            removedPieces_.push_back(p);
        }
    }

    notifyObservers(args);
}

void ModelAdapter::addObserver(Observer* obs) noexcept{
    observers_.push_back(obs);
}

void ModelAdapter::removeObserver(Observer* obs) noexcept{
    size_t i {};
    while(i < observers_.size() && observers_[i] != obs) i++;
    if(i < observers_.size())
        observers_.erase(observers_.begin() + i);

}

void ModelAdapter::notifyObservers(std::initializer_list<Observable*> infos) const noexcept{
    for(auto& obs : observers_){
        obs -> update(infos);
    }
}

ModelAdapter::~ModelAdapter(){
    for(Player*& player : players_){
        if(player){
            delete player;
            player = nullptr;
        }
    }

    for(Piece*& p : removedPieces_){
        delete p;
    }
}

bool ModelAdapter::playerCanMove(Color color) noexcept{
    for(int i = 0; i < board_.size(); i++){
        for(int j = 0; j < board_.size(); j++){
            Piece* piece {board_.getPiece(j, i)};
            if(piece && piece -> color() == color && pieceCanMove(piece))
                return true;
        }
    }

    return false;
}

bool ModelAdapter::playerCanMove_startGame(Color color) const noexcept{
    int y {color == Color::BLUE ? 4 : board_.size() / 2 + 1};
    for(int i = 0; i < board_.size(); i++){
        const Piece* piece {board_.getPiece(i, y)};
        if(piece && piece -> color() == color && pieceCanMove(piece))
            return true;
    }

    return false;
}

bool ModelAdapter::pieceCanMove(const Piece* piece) const noexcept{
    return piece -> canMove(piece -> position() + Position{0, 1})    ||
           piece -> canMove(piece -> position() + Position{0, -1})   ||
           piece -> canMove(piece -> position() + Position{1, 0})    ||
           piece -> canMove(piece -> position() + Position{-1, 0})   ||
           piece -> canAttack(piece -> position() + Position{0, 1})  ||
           piece -> canAttack(piece -> position() + Position{0, -1}) ||
           piece -> canAttack(piece -> position() + Position{1, 0})  ||
           piece -> canAttack(piece -> position() + Position{-1, 0});
}

void ModelAdapter::parseFor(const std::string& filename, Color color, bool isPathAbsolute){
    ParseInfo info {board_, graph_, history_, color};
    Parser<std::vector<Piece*>>* parser = new ConfigFileParser(info, isPathAbsolute ? filename : std::string{Config::BOARD_CONFIG_PATH} + filename);
    std::vector<Piece*> result;
    char buffer[200];

    if(parser -> canParse()){
        parser -> parse();
        result = std::move(parser -> result());

        for(Piece* piece : result){
            if(board_.getCell(piece -> position()).piece){
                delete board_.getCell(piece -> position()).piece;
            }

            board_.getCell(piece -> position()).piece = piece;
        }

        if(!playerCanMove_startGame(color)){
            sprintf(buffer, "Le fichier de configuration du joueur %s est valide "
                            "mais le positionnement n'est pas valide (aucun pions ne peuvent bougés).",
                    color == Color::RED ? "rouge" : "bleu");
            history_.addFailure(buffer);
            graph_.consume(StateGraph::FLOAD);
        } else{
            sprintf(buffer, "Le fichier de configuration du joueur %s a été chargé avec succès.", color == Color::RED ? "rouge" : "bleu");
            history_.addSuccess(buffer);
            graph_.consume(StateGraph::LOAD);
        }
    } else{
        sprintf(buffer, "Le fichier de configuration du joueur %s est erroné.", color == Color::RED ? "rouge" : "bleu");
        history_.addFailure(buffer);
        graph_.consume(StateGraph::FLOAD);
    }

    delete parser;
}

std::vector<Piece*> ModelAdapter::piecesOf(Color color){
    if(!graph_.canConsume(StateGraph::LOAD)){
        throw std::logic_error("Current model's state doesn't allow this method to be called");
    }

    std::vector<Piece*> army {};
    for(int i = Config::PIECE_MIN_RANK; i <= Config::PIECE_MAX_RANK; i++){
        for(int j = 0; j < Piece::pieceInfo[i].count; j++){
            army.push_back(toPiece(i, color));
        }
    }

    return army;
}

Piece* ModelAdapter::toPiece(int rank, Color color){
    Piece* piece {};

    PieceFactory pfactory {};
    piece = pfactory.createPiece(rank, {0, 0}, color, board_, graph_, history_);
    return piece;
}

const std::vector<Piece*>& ModelAdapter::removedPieces() const noexcept{
    return removedPieces_;
}


/* ========================== Stratego =========================== */
Stratego::Stratego() : ModelAdapter {}
{}

void Stratego::move(const Position &startPos, const Position &endPos){
    if(!graph_.canConsume(StateGraph::ACT) || !graph_.canConsume(StateGraph::FACT)){
        throw std::logic_error("Current model's state doesn't allow this method to be called");
    }

    Piece* piece;
    if((piece = board_.getPiece(startPos))){
        if(piece -> color() == currentPlayer().color()){
            piece -> move(endPos);
        } else{
            history_.addFailure("La pièce indiquée ne vous appartient pas.");
            history_.addHint(std::string{"Sélectionnez une pièce "} + (currentPlayer().color() == Color::RED ? "rouge" : "bleu")
                             + " face visible.");
            graph_.consume(StateGraph::FACT);
            notifyObservers({this});
        }
    } else{
        history_.addFailure("Aucune pièce ne se trouve à la position de départ indiquée.");
        history_.addHint("La case sélectionné estvide");
        graph_.consume(StateGraph::FACT);
        notifyObservers({this});
    }
}

void Stratego::attack(const Position &startPos, const Position &endPos){
    if(!graph_.canConsume(StateGraph::ACT) || !graph_.canConsume(StateGraph::FACT)){
        throw std::logic_error("Current model's state doesn't allow this method to be called");
    }

    Piece* piece;
    if((piece = board_.getPiece(startPos))){
        if(piece -> color() == currentPlayer().color()){
            piece -> attack(endPos);
        } else{
            history_.addFailure("La pièce indiquée ne vous appartient pas.");
            history_.addHint(std::string{"Sélectionnez une pièce "} + (currentPlayer().color() == Color::RED ? "rouge" : "bleu")
                             + " face visible.");
            graph_.consume(StateGraph::FACT);
            notifyObservers({this});
        }
    } else{
        history_.addFailure("Aucune pièce ne se trouve à la position de départ indiquée.");
        history_.addHint("La case sélectionné est vide");
        graph_.consume(StateGraph::FACT);
        notifyObservers({this});
    }
}

void Stratego::moveAttack(const Position &startPos, const Position &endPos){
    if(!graph_.canConsume(StateGraph::ACT) || !graph_.canConsume(StateGraph::FACT)){
        throw std::logic_error("Current model's state doesn't allow this method to be called");
    }

    Piece* piece;
    if((piece = board_.getPiece(startPos))){
        if(piece -> color() == currentPlayer().color()){
            if(board_.getPiece(endPos)){
                piece -> attack(endPos);
            } else{
                piece -> move(endPos);
            }
        } else{
            history_.addFailure("La pièce indiquée ne vous appartient pas.");
            history_.addHint(std::string{"Sélectionnez une pièce "} + (currentPlayer().color() == Color::RED ? "rouge" : "bleu")
                             + " face visible.");
            graph_.consume(StateGraph::FACT);
            notifyObservers({this});
        }
    } else{
        history_.addFailure("Aucune pièce ne se trouve à la position de départ indiquée.");
        history_.addHint("La case sélectionné est vide");
        graph_.consume(StateGraph::FACT);
        notifyObservers({this});
    }
}


/* ========================== StrategoReveal =========================== */
StrategoReveal::StrategoReveal() : ModelAdapter {}
{}

void StrategoReveal::move(const Position &startPos, const Position &endPos){
    if(!graph_.canConsume(StateGraph::ACT) || !graph_.canConsume(StateGraph::FACT)){
        throw std::logic_error("Current model's state doesn't allow this method to be called");
    }

    Piece* piece;
    if((piece = board_.getPiece(startPos))){
        if(piece -> color() == currentPlayer().color()){
            piece -> move(endPos);
        } else{
            history_.addFailure("La pièce indiquée ne vous appartient pas.");
            history_.addHint(std::string{"Sélectionnez une pièce "} + (currentPlayer().color() == Color::RED ? "rouge" : "bleu")
                             + " face visible.");
            graph_.consume(StateGraph::FACT);
            notifyObservers({this});
        }
    } else{
        history_.addFailure("Aucune pièce ne se trouve à la position de départ indiquée.");
        history_.addHint("La case sélectionné est vide");
        graph_.consume(StateGraph::FACT);
        notifyObservers({this});
    }
}

void StrategoReveal::attack(const Position &startPos, const Position &endPos){
    if(!graph_.canConsume(StateGraph::ACT) || !graph_.canConsume(StateGraph::FACT)){
        throw std::logic_error("Current model's state doesn't allow this method to be called");
    }

    Piece* piece;
    if((piece = board_.getPiece(startPos))){
        if(piece -> color() == currentPlayer().color()){
            piece -> attack(endPos);
        } else{
            history_.addFailure("La pièce indiquée ne vous appartient pas.");
            history_.addHint(std::string{"Sélectionnez une pièce "} + (currentPlayer().color() == Color::RED ? "rouge" : "bleu")
                             + " face visible.");
            graph_.consume(StateGraph::FACT);
            notifyObservers({this});
        }
    } else{
        history_.addFailure("Aucune pièce ne se trouve à la position de départ indiquée.");
        history_.addHint("La case sélectionné est vide");
        graph_.consume(StateGraph::FACT);
        notifyObservers({this});
    }
}

void StrategoReveal::moveAttack(const Position &startPos, const Position &endPos){
    if(!graph_.canConsume(StateGraph::ACT) || !graph_.canConsume(StateGraph::FACT)){
        throw std::logic_error("Current model's state doesn't allow this method to be called");
    }

    Piece* piece;
    if((piece = board_.getPiece(startPos))){
        if(piece -> color() == currentPlayer().color()){
            if(board_.getPiece(endPos)){
                piece -> attack(endPos);
            } else{
                piece -> move(endPos);
            }
        } else{
            history_.addFailure("La pièce indiquée ne vous appartient pas.");
            history_.addHint(std::string{"Sélectionnez une pièce "} + (currentPlayer().color() == Color::RED ? "rouge" : "bleu")
                             + " face visible.");
            graph_.consume(StateGraph::FACT);
            notifyObservers({this});
        }
    } else{
        history_.addFailure("Aucune pièce ne se trouve à la position de départ indiquée.");
        history_.addHint("La case sélectionné est vide");
        graph_.consume(StateGraph::FACT);
        notifyObservers({this});
    }
}
void StrategoReveal::nextPlayer(){
    if(!graph_.canConsume(StateGraph::NEXT)){
        throw std::logic_error("Current model's state doesn't allow this method to be called");
    }

    playerPointer_ = (playerPointer_ + 1) % players_.size();
    for(int i = 1; i < board_.size() - 1; i++){
        for(int j = 1; j < board_.size() - 1; j++){
            Cell& cell {board_.getCell(j, i)};
            if(cell.piece){
                if(cell.piece -> color() == currentPlayer().color() || cell.piece -> hasBeenInCombat()){
                    cell.piece -> setRevealed(true);
                } else{
                    cell.piece -> setRevealed(false);
                }
            }
        }
    }

    graph_.consume(StateGraph::NEXT);
    notifyObservers({this});
}



















