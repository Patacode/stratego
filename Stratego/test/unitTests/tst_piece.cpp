#include <catch2/catch.hpp>
#include <piece.h>
#include <gamestuff.h>
#include <pieceFactory.h>

using namespace stratego::model;
using namespace stratego;

Piece* toPiece(int rank, int x, int y, Color color,  Board& board, StateGraph& graph, History& hist) {
    Piece* piece {};

    PieceFactory pfactory {};
    piece = pfactory.createPiece(rank, {x, y}, color, board, graph, hist);
    return piece;
}


TEST_CASE("piece constructor", "[piece][constructor]"){
    Board board {};
    History hist {50};
    StateGraph graph {};

    for(int i=0; i <= Config::PIECE_MAX_RANK; i++){
        Piece* pieceBlue{} ;
        pieceBlue= toPiece(i, 0 ,i, Color::BLUE,  board, graph, hist);

        board.getCell(0,i).piece = pieceBlue;

        REQUIRE(board.getPiece(0,i)==pieceBlue);

        Piece* pieceRed{} ;
        pieceRed= toPiece(i, board.size()-1 ,i, Color::RED,  board, graph, hist);

        board.getCell(board.size()-1,i).piece= pieceRed;

        REQUIRE(board.getPiece(board.size()-1,i)==pieceRed);

        delete pieceBlue;
        delete pieceRed;
        board.getCell(0,i).piece = nullptr;
        board.getCell(board.size()-1,i).piece = nullptr;
    }
 }

TEST_CASE("piece move", "[piece][move]"){
    Board board {};
    History hist {50};
    StateGraph graph {};

    SECTION("move scout three squares "){

        Piece* pieceBlue{new Scout{{1,1}, Color::BLUE,  board, graph, hist}} ;
        board.getCell(1,1).piece= pieceBlue;

        Position position{4,1};
        pieceBlue->move(position);

        REQUIRE(board.getCell(4,1).piece==pieceBlue);

        delete pieceBlue;
        board.getCell(4,1).piece = nullptr;
    }

    SECTION("move piece one square "){

        Piece* pieceBlue{new Scout{{1,1}, Color::BLUE,  board, graph, hist}} ;
        board.getCell(1,1).piece= pieceBlue;

        Position position{2,1};
        pieceBlue->move(position);

        REQUIRE(board.getCell(2,1).piece==pieceBlue);

        delete pieceBlue;
        board.getCell(2,1).piece = nullptr;

    }

    SECTION("move bomb "){
        Position position{2,1};
        Piece* pieceBlue{new Bomb{{1,1}, Color::BLUE,  board, graph, hist}} ;
        board.getCell(1,1).piece= pieceBlue;

        graph.setState(StateGraph::PLAYER_TURN);
        pieceBlue->move(position);

        REQUIRE(graph.state() == StateGraph::ERROR_ACTION);

        delete pieceBlue;
        board.getCell(1,1).piece = nullptr;
    }

    SECTION("move flag "){
        Position position{2,1};
        Piece* pieceBlue{new Flag{{1,1}, Color::BLUE,  board, graph, hist}} ;
        board.getCell(1,1).piece= pieceBlue;

        graph.setState(StateGraph::PLAYER_TURN);
        pieceBlue->move(position);

        REQUIRE(graph.state() == StateGraph::ERROR_ACTION);

        delete pieceBlue;
        board.getCell(1,1).piece = nullptr;
    }

    SECTION("move scout three squares not valid(piece) "){
        Piece* pieceBlue{new Scout{{1,1}, Color::BLUE,  board, graph, hist}} ;
        board.getCell(1,1).piece= pieceBlue;

        Piece* pieceRed{new Scout{{3,1}, Color::RED,  board, graph, hist}} ;
        board.getCell(3,1).piece= pieceRed;

        Position position{4,1};

        graph.setState(StateGraph::PLAYER_TURN);
        pieceBlue->move(position);

        REQUIRE(graph.state() == StateGraph::ERROR_ACTION);

        delete pieceBlue;
        delete pieceRed;
        board.getCell(1,1).piece = nullptr;
        board.getCell(3,1).piece = nullptr;
    }

    SECTION("move but attack "){
        Piece* pieceBlue{new Captain{{1,1}, Color::BLUE,  board, graph, hist}} ;
        board.getCell(1,1).piece= pieceBlue;

        Piece* pieceRed{new Marshal{{2,1}, Color::RED,  board, graph, hist}} ;
        board.getCell(2,1).piece= pieceRed;

        Position position{2,1};
        graph.setState(StateGraph::PLAYER_TURN);
        pieceBlue->move(position);

        REQUIRE(graph.state() == StateGraph::ERROR_ACTION);

        delete pieceBlue;
        delete pieceRed;
        board.getCell(1,1).piece = nullptr;
        board.getCell(2,1).piece = nullptr;
    }

     SECTION("piece move water"){
         Position position{3,6};
         Piece* pieceBlue{new Spy{{2,6}, Color::BLUE,  board, graph, hist}} ;
         board.getCell(2,6).piece= pieceBlue;

         graph.setState(StateGraph::PLAYER_TURN);
         pieceBlue->move(position);

         REQUIRE(graph.state() == StateGraph::ERROR_ACTION);

         delete pieceBlue;
         board.getCell(2,6).piece = nullptr;
     }

     SECTION("move piece off the board"){
         Position position{0,1};
         Piece* pieceBlue{new Flag{{1,1}, Color::BLUE,  board, graph, hist}} ;
         board.getCell(1,1).piece= pieceBlue;

         graph.setState(StateGraph::PLAYER_TURN);
         pieceBlue->move(position);

         REQUIRE(graph.state() == StateGraph::ERROR_ACTION);
         delete pieceBlue;
         board.getCell(1,1).piece = nullptr;
     }
}

TEST_CASE("piece attack", "[piece][attack]"){
    Board board {};
    History hist {50};
    StateGraph graph {};

     SECTION("attack equal rank"){
        Position position{2,1};
        Piece* pieceBlue{new Marshal{{1,1}, Color::BLUE,  board, graph, hist}} ;
        board.getCell(1,1).piece= pieceBlue;

        Piece* pieceRed{new Marshal{position, Color::RED,  board, graph, hist}} ;
        board.getCell(position).piece= pieceRed;


        pieceBlue->attack(position);

        REQUIRE(board.getPiece(position)==nullptr);

        delete pieceBlue;
        delete pieceRed;
        board.getCell(1,1).piece = nullptr;
        board.getCell(position).piece = nullptr;
     }

     SECTION("attack Marshal -> Spy"){
        Position position{2,1};
        Piece* pieceBlue{new Marshal{{1,1}, Color::BLUE,  board, graph, hist}} ;
        board.getCell(1,1).piece= pieceBlue;

        Piece* pieceRed{new Spy{position, Color::RED,  board, graph, hist}} ;
        board.getCell(position).piece= pieceRed;


        pieceBlue->attack(position);

        REQUIRE(board.getPiece(position)==pieceBlue);

        delete pieceBlue;
        delete pieceRed;
        board.getCell(1,1).piece = nullptr;
        board.getCell(position).piece = nullptr;
     }

     SECTION("attack Spy -> Marshal"){
        Position position{2,1};

        Piece* pieceBlue{new Spy{{1,1}, Color::BLUE,  board, graph, hist}} ;
        board.getCell(1,1).piece= pieceBlue;

        Piece* pieceRed{new Marshal{position, Color::RED,  board, graph, hist}} ;
        board.getCell(position).piece= pieceRed;

        pieceBlue->attack(position);

        REQUIRE(board.getPiece(position)==pieceBlue);

        delete pieceBlue;
        delete pieceRed;
        board.getCell(1,1).piece = nullptr;
        board.getCell(position).piece = nullptr;
     }

     SECTION("attack Miner -> Bomb"){
        Position position{2,1};
        Piece* pieceBlue{new Miner{{1,1}, Color::BLUE,  board, graph, hist}} ;
        board.getCell(1,1).piece= pieceBlue;

        Piece* pieceRed{new Bomb{position, Color::RED,  board, graph, hist}} ;
        board.getCell(position).piece= pieceRed;

        pieceBlue->attack(position);

        REQUIRE(board.getPiece(position)==pieceBlue);

        delete pieceBlue;
        delete pieceRed;
        board.getCell(1,1).piece = nullptr;
        board.getCell(position).piece = nullptr;
     }

     SECTION("attack Scout -> Bomb"){
        Position position{4,1};
        Piece* pieceBlue{new Scout{{1,1}, Color::BLUE,  board, graph, hist}} ;
        board.getCell(1,1).piece= pieceBlue;

        Piece* pieceRed{new Bomb{position, Color::RED,  board, graph, hist}} ;
        board.getCell(position).piece= pieceRed;


        pieceBlue->attack(position);

        REQUIRE(board.getPiece(position)==pieceRed);

        delete pieceBlue;
        delete pieceRed;
        board.getCell(1,1).piece = nullptr;
        board.getCell(position).piece = nullptr;
     }

     SECTION(" Major attack captain"){
        Position position{2,1};
        Piece* pieceBlue{new Major{{1,1}, Color::BLUE,  board, graph, hist}} ;
        board.getCell(1,1).piece= pieceBlue;

        Piece* pieceRed{new Captain{position, Color::RED,  board, graph, hist}} ;
        board.getCell(position).piece= pieceRed;


        pieceBlue->attack(position);

        REQUIRE(board.getPiece(position)==pieceBlue);

        delete pieceBlue;
        delete pieceRed;
        board.getCell(1,1).piece = nullptr;
        board.getCell(position).piece = nullptr;
     }

     SECTION("Sergent attack Lieutenant"){
        Position position{2,1};
        Piece* pieceBlue{new Sergent{{1,1}, Color::BLUE,  board, graph, hist}} ;
        board.getCell(1,1).piece= pieceBlue;

        Piece* pieceRed{new Lieutenant{position, Color::RED,  board, graph, hist}} ;
        board.getCell(position).piece= pieceRed;


        pieceBlue->attack(position);

        REQUIRE(board.getPiece(position)==pieceRed);

        delete pieceBlue;
        delete pieceRed;
        board.getCell(1,1).piece = nullptr;
        board.getCell(position).piece = nullptr;
     }

     SECTION("Flag attack General"){
        Position position{2,1};
        Piece* pieceBlue{new Flag{{1,1}, Color::BLUE,  board, graph, hist}} ;
        board.getCell(1,1).piece= pieceBlue;

        Piece* pieceRed{new General{position, Color::RED,  board, graph, hist}} ;
        board.getCell(position).piece= pieceRed;


        graph.setState(StateGraph::PLAYER_TURN);
        pieceBlue->attack(position);

        REQUIRE(graph.state() == StateGraph::ERROR_ACTION);

        delete pieceBlue;
        delete pieceRed;
        board.getCell(1,1).piece = nullptr;
        board.getCell(position).piece = nullptr;
     }

     SECTION("Bomb attack General"){
        Position position{2,1};
        Piece* pieceBlue{new Bomb{{1,1}, Color::BLUE,  board, graph, hist}} ;
        board.getCell(1,1).piece= pieceBlue;

        Piece* pieceRed{new General{position, Color::RED,  board, graph, hist}} ;
        board.getCell(position).piece= pieceRed;


        graph.setState(StateGraph::PLAYER_TURN);
        pieceBlue->attack(position);

        REQUIRE(graph.state() == StateGraph::ERROR_ACTION);

        delete pieceBlue;
        delete pieceRed;
        board.getCell(1,1).piece = nullptr;
        board.getCell(position).piece = nullptr;
     }

     SECTION("Spy red attack General red"){
         Position position{2,1};
        Piece* pieceBlue{new Spy{{1,1}, Color::RED,  board, graph, hist}} ;
        board.getCell(1,1).piece= pieceBlue;

        Piece* pieceRed{new General{position, Color::RED,  board, graph, hist}} ;
        board.getCell(position).piece= pieceRed;

        graph.setState(StateGraph::PLAYER_TURN);
        pieceBlue->attack(position);

        REQUIRE(graph.state() == StateGraph::ERROR_ACTION);

        delete pieceBlue;
        delete pieceRed;
        board.getCell(1,1).piece = nullptr;
        board.getCell(position).piece = nullptr;
     }

     SECTION("Sergent attack nothing"){

        Piece* pieceBlue{new Sergent{{1,1}, Color::BLUE,  board, graph, hist}} ;
        board.getCell(1,1).piece= pieceBlue;

        Position position{2,1};

        graph.setState(StateGraph::PLAYER_TURN);
        pieceBlue->attack(position);

        REQUIRE(graph.state() == StateGraph::ERROR_ACTION);

        delete pieceBlue;
        board.getCell(1,1).piece = nullptr;
     }

     SECTION("Cpatain attack flage two squares away"){

        Position position{3,1};
        Piece* pieceBlue{new Captain{{1,1}, Color::BLUE,  board, graph, hist}} ;
        board.getCell(1,1).piece= pieceBlue;

        Piece* pieceRed{new Flag{position, Color::RED,  board, graph, hist}} ;
        board.getCell(position).piece= pieceRed;


        graph.setState(StateGraph::PLAYER_TURN);
        pieceBlue->attack(position);

        REQUIRE(graph.state() == StateGraph::ERROR_ACTION);

        delete pieceBlue;
        delete pieceRed;
        board.getCell(1,1).piece = nullptr;
        board.getCell(position).piece = nullptr;
     }

     SECTION("Scout attack flage but water"){

        Piece* pieceBlue{new Scout{{2,6}, Color::BLUE,  board, graph, hist}} ;
        board.getCell(2,6).piece= pieceBlue;

        Piece* pieceRed{new Flag{{7,6}, Color::RED,  board, graph, hist}} ;
        board.getCell(7,6).piece= pieceRed;

        Position position{7,6};

        graph.setState(StateGraph::PLAYER_TURN);
        pieceBlue->attack(position);

        REQUIRE(graph.state() == StateGraph::ERROR_ACTION);
        delete pieceBlue;
        delete pieceRed;
        board.getCell(2,6).piece = nullptr;
        board.getCell(7,6).piece = nullptr;
     }
}
