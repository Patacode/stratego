#include <catch2/catch.hpp>
#include <model.h>
#include <eventMgr.h>
#include <iostream>

using namespace stratego;
using namespace stratego::model;

void clearModel(Model& model){
    model.board().~Board();
}

void playMove(Model& model, const Position& startPos, const Position& endPos){
    model.move(startPos, endPos);
    model.nextTurn();
    model.nextPlayer();
}

TEST_CASE("model state mutation", "[model][state]"){

    Stratego stratego {};
    Model& model {stratego};

    SECTION("initial state"){
        REQUIRE(model.currentState() == StateGraph::NOT_STARTED);
    }

    SECTION("after init()"){
        model.init();
        REQUIRE(model.currentState() == StateGraph::SET_UP);
    }

    SECTION("after valid load(string, Color)"){
        model.init();
        model.load("default", Color::RED);
        REQUIRE(model.currentState() == StateGraph::SET_UP);
        model.board().~Board();
    }

    SECTION("after invalid load(string, Color)"){
        model.init();
        model.load("wrong", Color::RED);
        REQUIRE(model.currentState() == StateGraph::ERROR_SETUP);
    }

    SECTION("after setup(string, string)"){
        model.init();
        model.load("default", Color::RED);
        model.load("default", Color::BLUE);
        model.setup("max", "alex");
        REQUIRE(model.currentState() == StateGraph::PLAYER_SWAP);
        clearModel(model);
    }

    SECTION("after nextPlayer()"){
        model.init();
        model.load("default", Color::RED);
        model.load("default", Color::BLUE);
        model.setup("max", "alex");
        model.nextPlayer();
        REQUIRE(model.currentState() == StateGraph::PLAYER_TURN);
        model.board().~Board();
        clearModel(model);
    }

    SECTION("after stop()"){
        model.init();
        model.load("default", Color::RED);
        model.load("default", Color::BLUE);
        model.setup("max", "alex");
        model.nextPlayer();
        model.stop();
        REQUIRE(model.currentState() == StateGraph::EOG);
        model.board().~Board();
        clearModel(model);
    }

    SECTION("after valid move(Position, Position)"){
        model.init();
        model.load("default", Color::RED);
        model.load("default", Color::BLUE);
        model.setup("max", "alex");
        model.nextPlayer();
        model.move({model.board().size() / 2, model.board().size() / 2 + 1}, {model.board().size() / 2, model.board().size() / 2});
        REQUIRE(model.currentState() == StateGraph::GAME_TURN);
        clearModel(model);
    }

    SECTION("after invalid move(Position, Position)"){
        model.init();
        model.load("default", Color::RED);
        model.load("default", Color::BLUE);
        model.setup("max", "alex");
        model.nextPlayer();
        model.move({1, model.board().size() - 2}, {1, model.board().size() - 3});
        REQUIRE(model.currentState() == StateGraph::ERROR_ACTION);
        clearModel(model);
    }

    SECTION("after invalid move(Position, Position) due to no piece"){
        model.init();
        model.load("default", Color::RED);
        model.load("default", Color::BLUE);
        model.setup("max", "alex");
        model.nextPlayer();
        model.move({0, 0}, {1, 0});
        REQUIRE(model.currentState() == StateGraph::ERROR_ACTION);
        clearModel(model);
    }

    SECTION("after invalid move(Position, Position) due to opponent piece"){
        model.init();
        model.load("default", Color::RED);
        model.load("default", Color::BLUE);
        model.setup("max", "alex");
        model.nextPlayer();
        model.move({1, 1}, {1, 2});
        REQUIRE(model.currentState() == StateGraph::ERROR_ACTION);
        clearModel(model);
    }

    SECTION("after valid attack(Position, Position)"){
        model.init();
        model.load("default", Color::RED);
        model.load("default", Color::BLUE);
        model.setup("max", "alex");
        model.nextPlayer();
        model.attack({model.board().size() / 2, model.board().size() / 2 + 1}, {model.board().size() / 2, model.board().size() / 2 - 2});
        REQUIRE(model.currentState() == StateGraph::GAME_TURN);
        clearModel(model);
    }

    SECTION("after invalid attack(Position, Position)"){
        model.init();
        model.load("default", Color::RED);
        model.load("default", Color::BLUE);
        model.setup("max", "alex");
        model.nextPlayer();
        model.attack({model.board().size() / 2, model.board().size() / 2 + 1}, {model.board().size() / 2, model.board().size() / 2 - 1});
        REQUIRE(model.currentState() == StateGraph::ERROR_ACTION);
        clearModel(model);
    }

    SECTION("after invalid attack(Position, Position) due to no piece"){
        model.init();
        model.load("default", Color::RED);
        model.load("default", Color::BLUE);
        model.setup("max", "alex");
        model.nextPlayer();
        model.attack({0, 0}, {1, 0});
        REQUIRE(model.currentState() == StateGraph::ERROR_ACTION);
        clearModel(model);
    }

    SECTION("after invalid attack(Position, Position) due to opponent piece"){
        model.init();
        model.load("default", Color::RED);
        model.load("default", Color::BLUE);
        model.setup("max", "alex");
        model.nextPlayer();
        model.attack({1, 1}, {1, 2});
        REQUIRE(model.currentState() == StateGraph::ERROR_ACTION);
        clearModel(model);
    }

    SECTION("errorProcessed() after invalid setup()"){
        model.init();
        model.load("wrong", Color::RED);
        model.errorProcessed();
        REQUIRE(model.currentState() == StateGraph::SET_UP);
    }

    SECTION("errorProcessed() after invalid move(Position, Position)"){
        model.init();
        model.load("default", Color::RED);
        model.load("default", Color::BLUE);
        model.setup("max", "alex");
        model.nextPlayer();
        model.move({1, model.board().size() - 2}, {1, model.board().size() - 3});
        model.errorProcessed();
        REQUIRE(model.currentState() == StateGraph::PLAYER_TURN);
        clearModel(model);
    }

    SECTION("errorProcessed() after invalid attack(Position, Position)"){
        model.init();
        model.load("default", Color::RED);
        model.load("default", Color::BLUE);
        model.setup("max", "alex");
        model.nextPlayer();
        model.attack({model.board().size() / 2, model.board().size() / 2 + 1}, {model.board().size() / 2, model.board().size() / 2 - 1});
        model.errorProcessed();
        REQUIRE(model.currentState() == StateGraph::PLAYER_TURN);
        clearModel(model);
    }

    SECTION("after nextTurn() game continues"){
        model.init();
        model.load("default", Color::RED);
        model.load("default", Color::BLUE);
        model.setup("max", "alex");
        model.nextPlayer();
        model.attack({model.board().size() / 2, model.board().size() / 2 + 1}, {model.board().size() / 2, model.board().size() / 2 - 2});
        model.nextTurn();
        REQUIRE(model.currentState() == StateGraph::PLAYER_SWAP);
        clearModel(model);
    }

    SECTION("after nextTurn() game ends"){
        model.init();
        model.load("test", Color::RED);
        model.load("test", Color::BLUE);
        model.setup("max", "alex");
        model.nextPlayer();
        playMove(model, {model.board().size() / 2, model.board().size() / 2 + 1}, {model.board().size() / 2, model.board().size() / 2});
        playMove(model, {model.board().size() / 2, model.board().size() / 2 - 2}, {model.board().size() / 2, model.board().size() / 2 - 1});
        playMove(model, {model.board().size() / 2, model.board().size() / 2}, {model.board().size() / 2 - 1, model.board().size() / 2});
        playMove(model, {model.board().size() / 2, model.board().size() / 2 - 1}, {model.board().size() / 2, model.board().size() / 2});
        model.attack({model.board().size() / 2 - 1, model.board().size() / 2}, {model.board().size() / 2 - 1, model.board().size() / 2 - 2});
        model.nextTurn();
        REQUIRE(model.currentState() == StateGraph::GAME_OVER);
        clearModel(model);
    }

    SECTION("after replay(bool) true"){
        model.init();
        Board& board {model.board()};
        model.load("test", Color::RED);
        model.load("test", Color::BLUE);
        model.setup("max", "alex");
        model.nextPlayer();
        playMove(model, {model.board().size() / 2, model.board().size() / 2 + 1}, {model.board().size() / 2, model.board().size() / 2});
        playMove(model, {model.board().size() / 2, model.board().size() / 2 - 2}, {model.board().size() / 2, model.board().size() / 2 - 1});
        playMove(model, {model.board().size() / 2, model.board().size() / 2}, {model.board().size() / 2 - 1, model.board().size() / 2});
        playMove(model, {model.board().size() / 2, model.board().size() / 2 - 1}, {model.board().size() / 2, model.board().size() / 2});
        model.attack({model.board().size() / 2 - 1, model.board().size() / 2}, {model.board().size() / 2 - 1, model.board().size() / 2 - 2});
        model.nextTurn();
        model.replay(true);
        REQUIRE(model.currentState() == StateGraph::NOT_STARTED);
        board.~Board();
    }

    SECTION("after replay(bool) false"){
        model.init();
        model.load("test", Color::RED);
        model.load("test", Color::BLUE);
        model.setup("max", "alex");
        model.nextPlayer();
        playMove(model, {model.board().size() / 2, model.board().size() / 2 + 1}, {model.board().size() / 2, model.board().size() / 2});
        playMove(model, {model.board().size() / 2, model.board().size() / 2 - 2}, {model.board().size() / 2, model.board().size() / 2 - 1});
        playMove(model, {model.board().size() / 2, model.board().size() / 2}, {model.board().size() / 2 - 1, model.board().size() / 2});
        playMove(model, {model.board().size() / 2, model.board().size() / 2 - 1}, {model.board().size() / 2, model.board().size() / 2});
        model.attack({model.board().size() / 2 - 1, model.board().size() / 2}, {model.board().size() / 2 - 1, model.board().size() / 2 - 2});
        model.nextTurn();
        model.replay(false);
        REQUIRE(model.currentState() == StateGraph::EOG);
        clearModel(model);
    }
}
