#include <catch2/catch.hpp>
#include <gamestuff.h>
#include <piece.h>
#include <pieceFactory.h>

using namespace stratego::model;
using namespace stratego;
Piece* toPieces(int rank, int x, int y, Color color,  Board& board, StateGraph& graph, History& hist) {
    Piece* piece {};

    PieceFactory pfactory {};
    piece = pfactory.createPiece(rank, {x, y}, color, board, graph, hist);
    return piece;
}

TEST_CASE("player constructor"){

        std::string pseudoRed{"Alex"};
        Color colorRed{Color::RED};
        std::string pseudoBlue{"Max"};
        Color colorBlue{Color::BLUE};


        PlayerInfo infoRed{
            pseudoRed,
            colorRed
        };

        PlayerInfo infoBlue{
            pseudoBlue,
            colorBlue
        };

        Player player1 = Player(infoRed);
        Player player2 =Player(infoBlue);

        REQUIRE(player1.color()==Color::RED);
        REQUIRE(player1.pseudo()=="Alex");

        REQUIRE(player2.color()==Color::BLUE);
        REQUIRE(player2.pseudo()=="Max");

}

TEST_CASE("player hast lost"){
    Board board {};
    History hist {50};
    StateGraph graph {};

    std::string pseudoRed{"Alex"};
    Color colorRed{Color::RED};
    std::string pseudoBlue{"Max"};
    Color colorBlue{Color::BLUE};


    PlayerInfo infoRed{
        pseudoRed,
        colorRed
    };

    PlayerInfo infoBlue{
        pseudoBlue,
        colorBlue
    };

    SECTION("catch flag"){

        Player player1 =  Player(infoRed);
        Player player2 =  Player(infoBlue);

        Position position{2,1};

        Piece* pieceRedFlag{new Flag{{10,10}, Color::RED,  board, graph, hist}} ;
        Piece* pieceRed{new Captain{{1,1}, Color::RED,  board, graph, hist}} ;
        Piece* pieceBlueFlag{new Flag{position, Color::BLUE,  board, graph, hist}} ;
        Piece* pieceBlue{new Miner{{2,2}, Color::BLUE,  board, graph, hist}} ;

        pieceRedFlag->addObserver(&player1);
        pieceRed->addObserver(&player1);
        pieceBlueFlag->addObserver(&player1);
        pieceBlue->addObserver(&player1);
        pieceRedFlag->addObserver(&player2);
        pieceRed->addObserver(&player2);
        pieceBlueFlag->addObserver(&player2);
        pieceBlue->addObserver(&player2);

        board.getCell(10,10).piece=pieceRedFlag;
        board.getCell(1,1).piece= pieceRed;
        board.getCell(position).piece= pieceBlueFlag;
        board.getCell(2,2).piece= pieceBlue;


        board.getPiece(1,1)->attack(position);

        REQUIRE(player2.hasLost());
        REQUIRE(!player1.hasLost());


        delete pieceBlue;
        delete pieceRed;
        delete pieceBlueFlag;
        delete pieceRedFlag;
        board.getCell(2,2).piece = nullptr;
        board.getCell(position).piece = nullptr;
        board.getCell(1,1).piece = nullptr;
        board.getCell(10,10).piece = nullptr;
    }

    SECTION("false because bomb and flag"){
        Player player1 =  Player(infoRed);
        Player player2 =  Player(infoBlue);
        for(int i=1; i<Config::PIECE_MAX_RANK;i++){
            while( player1.hasPiece(i)&& player2.hasPiece(i)){
                Piece* pieceBlue{} ;
                Piece* pieceRed{} ;
                Position position{2,1};
                pieceBlue= toPieces(i, 1 ,1, Color::BLUE,  board, graph, hist);
                pieceRed= toPieces(i, 2,1, Color::RED,  board, graph, hist);

                pieceRed->addObserver(&player1);
                pieceBlue->addObserver(&player1);
                pieceRed->addObserver(&player2);
                pieceBlue->addObserver(&player2);


                board.getCell(1,1).piece = pieceBlue;
                board.getCell(2,1).piece= pieceRed;

               board.getPiece(1,1)->attack(position);

               delete pieceBlue;
               delete pieceRed;

            }

        }
        REQUIRE(player1.hasLost());
        REQUIRE(player2.hasLost());
    }

}

TEST_CASE("count their pieces and the enemy's pieces"){
    Board board {};
    History hist {50};
    StateGraph graph {};

    std::string pseudoRed{"Alex"};
    Color colorRed{Color::RED};
    std::string pseudoBlue{"Max"};
    Color colorBlue{Color::BLUE};


    PlayerInfo infoRed{
        pseudoRed,
        colorRed
    };

    PlayerInfo infoBlue{
        pseudoBlue,
        colorBlue
    };

    Player player1 =  Player(infoRed);
    Player player2 =  Player(infoBlue);

    SECTION("Flag"){

        Position position{2,1};

        Piece* pieceRedFlag{new Flag{{10,10}, Color::RED,  board, graph, hist}} ;
        Piece* pieceRed{new Captain{{1,1}, Color::RED,  board, graph, hist}} ;
        Piece* pieceBlueFlag{new Flag{position, Color::BLUE,  board, graph, hist}} ;
        Piece* pieceBlue{new Miner{{2,2}, Color::BLUE,  board, graph, hist}} ;

        pieceRedFlag->addObserver(&player1);
        pieceRed->addObserver(&player1);
        pieceBlueFlag->addObserver(&player1);
        pieceBlue->addObserver(&player1);
        pieceRedFlag->addObserver(&player2);
        pieceRed->addObserver(&player2);
        pieceBlueFlag->addObserver(&player2);
        pieceBlue->addObserver(&player2);

        board.getCell(10,10).piece=pieceRedFlag;
        board.getCell(1,1).piece= pieceRed;
        board.getCell(position).piece= pieceBlueFlag;
        board.getCell(2,2).piece= pieceBlue;


        board.getPiece(1,1)->attack(position);

        REQUIRE(player2.remainingPieces(0)==0);
        REQUIRE(player1.remainingPieces(0)==1);
        REQUIRE(player1.remainingPieces()==40);
        REQUIRE(player2.remainingPieces()==39);
        REQUIRE(player1.eatenPieces(0)==1);
        REQUIRE(player2.eatenPieces(0)==0);
        REQUIRE(player1.eatenPieces()==1);
        REQUIRE(player2.eatenPieces()==0);
    }
}
