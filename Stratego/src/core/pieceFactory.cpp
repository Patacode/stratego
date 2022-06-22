#include "pieceFactory.h"
#include "piece.h"

using namespace stratego::model;

PieceFactory::PieceFactory() noexcept{}

Piece* PieceFactory::createPiece(int rank, const Position& initPos, Color color, Board& board, StateGraph& graph, History& hist) noexcept{
    Piece* piece {};
    switch(rank){ // rank
        case Config::PIECE_BOMB_INFO.rank:
            piece = new Bomb{initPos, color, board, graph, hist};
            break;
        case Config::PIECE_MARSHAL_INFO.rank:
            piece = new Marshal{initPos, color, board, graph, hist};
            break;
        case Config::PIECE_GENERAL_INFO.rank:
            piece = new General{initPos, color, board, graph, hist};
            break;
        case Config::PIECE_COLONEL_INFO.rank:
            piece = new Colonel{initPos, color, board, graph, hist};
            break;
        case Config::PIECE_MAJOR_INFO.rank:
            piece = new Major{initPos, color, board, graph, hist};
            break;
        case Config::PIECE_CAPTAIN_INFO.rank:
            piece = new Captain{initPos, color, board, graph, hist};
            break;
        case Config::PIECE_LIEUTENANT_INFO.rank:
            piece = new Lieutenant{initPos, color, board, graph, hist};
            break;
        case Config::PIECE_SERGENT_INFO.rank:
            piece = new Sergent{initPos, color, board, graph, hist};
            break;
        case Config::PIECE_MINER_INFO.rank:
            piece = new Miner{initPos, color, board, graph, hist};
            break;
        case Config::PIECE_SCOUT_INFO.rank:
            piece = new Scout{initPos, color, board, graph, hist};
            break;
        case Config::PIECE_SPY_INFO.rank:
            piece = new Spy{initPos, color, board, graph, hist};
            break;
        case Config::PIECE_FLAG_INFO.rank:
            piece = new Flag{initPos, color, board, graph, hist};
    }

    return piece;
}
