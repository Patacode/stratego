#include <catch2/catch.hpp>
#include <gamestuff.h>

using namespace stratego::model;

TEST_CASE("board constructor", "[board][constructor]"){

    Board board {};

    SECTION("default constructor"){
        for(int i = 0; i < board.size(); i += board.size() - 1){
            for(int j = 0; j < board.size(); j++){
                REQUIRE(board.getCell(j ,i).type == Cell::WALL);
            }
        }

        for(int i = 1; i < board.size() - 1; i++){
            for(int j = 0; j < board.size(); j += board.size() - 1){
                REQUIRE(board.getCell(j ,i).type == Cell::WALL);
            }
        }

        for(int i = 1; i < board.size() / 2 - 1; i++){
            for(int j = 1; j < board.size() - 1; j++){
                REQUIRE(board.getCell(j ,i).type == Cell::NORMAL);
            }
        }

        for(int i = board.size() / 2 + 1; i < board.size() - 1; i++){
            for(int j = 1; j < board.size() - 1; j++){
                REQUIRE(board.getCell(j ,i).type == Cell::NORMAL);
            }
        }

        REQUIRE(board.getCell(3, board.size() / 2 - 1).type == Cell::WATER);
        REQUIRE(board.getCell(4, board.size() / 2 - 1).type == Cell::WATER);
        REQUIRE(board.getCell(3, board.size() / 2).type == Cell::WATER);
        REQUIRE(board.getCell(4, board.size() / 2).type == Cell::WATER);

        REQUIRE(board.getCell(board.size() - 4, board.size() / 2 - 1).type == Cell::WATER);
        REQUIRE(board.getCell(board.size() - 5, board.size() / 2 - 1).type == Cell::WATER);
        REQUIRE(board.getCell(board.size() - 4, board.size() / 2).type == Cell::WATER);
        REQUIRE(board.getCell(board.size() - 5, board.size() / 2).type == Cell::WATER);

        for(int i = board.size() / 2 - 1; i < board.size() / 2 + 1; i++){
            for(int j = 1; j < board.size() - 1; j++){
                if(j < 3 || (j > 4 && j < board.size() - 5) || j > board.size() - 4){
                    REQUIRE(board.getCell(j, i).type == Cell::NORMAL);
                }
            }
        }
    }
}

TEST_CASE("board getters", "[board][getters]"){

    Board board {};

    SECTION("board size()"){
        REQUIRE(board.size() == static_cast<int>(stratego::Config::BOARD_SIZE));
    }

    SECTION("board getCell(int, int)"){
        REQUIRE((board.getCell(0, 0).type == Cell::WALL
                && board.getCell(0, 0).pos == Position{0, 0}
                && board.getCell(0, 0).piece == nullptr));
        REQUIRE_THROWS_AS(board.getCell(-1, 0), std::out_of_range);
        REQUIRE_THROWS_AS(board.getCell(-1, -1), std::out_of_range);
        REQUIRE_THROWS_AS(board.getCell(0, -1), std::out_of_range);
        REQUIRE_THROWS_AS(board.getCell(board.size(), 0), std::out_of_range);
        REQUIRE_THROWS_AS(board.getCell(board.size(), board.size()), std::out_of_range);
        REQUIRE_THROWS_AS(board.getCell(0, board.size()), std::out_of_range);
    }

    SECTION("board getCell(Position, Position)"){
        REQUIRE((board.getCell({0, 0}).type == Cell::WALL
                && board.getCell({0, 0}).pos == Position{0, 0}
                && board.getCell({0, 0}).piece == nullptr));
        REQUIRE_THROWS_AS(board.getCell({-1, 0}), std::out_of_range);
        REQUIRE_THROWS_AS(board.getCell({-1, -1}), std::out_of_range);
        REQUIRE_THROWS_AS(board.getCell({0, -1}), std::out_of_range);
        REQUIRE_THROWS_AS(board.getCell({board.size(), 0}), std::out_of_range);
        REQUIRE_THROWS_AS(board.getCell({board.size(), board.size()}), std::out_of_range);
        REQUIRE_THROWS_AS(board.getCell({0, board.size()}), std::out_of_range);
    }

    SECTION("board getPiece(int, int)"){
        REQUIRE(board.getPiece(0, 0) == nullptr);
        REQUIRE_THROWS_AS(board.getPiece(-1, 0), std::out_of_range);
        REQUIRE_THROWS_AS(board.getPiece(-1, -1), std::out_of_range);
        REQUIRE_THROWS_AS(board.getPiece(0, -1), std::out_of_range);
        REQUIRE_THROWS_AS(board.getPiece(board.size(), 0), std::out_of_range);
        REQUIRE_THROWS_AS(board.getPiece(board.size(), board.size()), std::out_of_range);
        REQUIRE_THROWS_AS(board.getPiece(0, board.size()), std::out_of_range);
    }

    SECTION("board getPiece(Position, Position)"){
        REQUIRE(board.getPiece({0, 0}) == nullptr);
        REQUIRE_THROWS_AS(board.getPiece({-1, 0}), std::out_of_range);
        REQUIRE_THROWS_AS(board.getPiece({-1, -1}), std::out_of_range);
        REQUIRE_THROWS_AS(board.getPiece({0, -1}), std::out_of_range);
        REQUIRE_THROWS_AS(board.getPiece({board.size(), 0}), std::out_of_range);
        REQUIRE_THROWS_AS(board.getPiece({board.size(), board.size()}), std::out_of_range);
        REQUIRE_THROWS_AS(board.getPiece({0, board.size()}), std::out_of_range);
    }
}

TEST_CASE("board predicate", "[board][predicate]"){

    Board board {};

    SECTION("board walkableCell(int, int)"){
        REQUIRE_FALSE(board.walkableCell(0, 0)); // wall
        REQUIRE_FALSE(board.walkableCell(3, board.size() / 2)); // wate
        REQUIRE(board.walkableCell(1, 1)); // normal
    }

    SECTION("board isInside(Position)"){
        REQUIRE(board.isInside({0, 0}));
        REQUIRE_FALSE(board.isInside({-1, 0}));
    }
}
