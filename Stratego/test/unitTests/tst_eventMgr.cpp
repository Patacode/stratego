#include <catch2/catch.hpp>
#include <eventMgr.h>

using namespace stratego::model;

TEST_CASE("stategraph modifiers", "[stategraph][modifiers]"){

    StateGraph graph {};

    REQUIRE(graph.state() == StateGraph::NOT_STARTED);

    SECTION("stategraph consume(event) - INI"){
        graph.consume(StateGraph::INI);
        REQUIRE(graph.state() == StateGraph::SET_UP);
    }

    SECTION("stategraph consume(event) - LOAD"){
        graph.consume(StateGraph::INI);
        graph.consume(StateGraph::LOAD);
        REQUIRE(graph.state() == StateGraph::SET_UP);
    }

    SECTION("stategraph consume(event) - SET"){
        graph.consume(StateGraph::INI);
        graph.consume(StateGraph::SET);
        REQUIRE(graph.state() == StateGraph::PLAYER_SWAP);
    }

    SECTION("stategraph consume(event) - FLOAD"){
        graph.consume(StateGraph::INI);
        graph.consume(StateGraph::FLOAD);
        REQUIRE(graph.state() == StateGraph::ERROR_SETUP);
    }

    SECTION("stategraph consume(event) - NEXT"){
        graph.consume(StateGraph::INI);
        graph.consume(StateGraph::SET);
        graph.consume(StateGraph::NEXT);
        REQUIRE(graph.state() == StateGraph::PLAYER_TURN);
    }

    SECTION("stategraph consume(event) - ACT"){
        graph.consume(StateGraph::INI);
        graph.consume(StateGraph::SET);
        graph.consume(StateGraph::NEXT);
        graph.consume(StateGraph::ACT);
        REQUIRE(graph.state() == StateGraph::GAME_TURN);
    }

    SECTION("stategraph consume(event) - FACT"){
        graph.consume(StateGraph::INI);
        graph.consume(StateGraph::SET);
        graph.consume(StateGraph::NEXT);
        graph.consume(StateGraph::FACT);
        REQUIRE(graph.state() == StateGraph::ERROR_ACTION);
    }

    SECTION("stategraph consume(event) - CHK"){
        graph.consume(StateGraph::INI);
        graph.consume(StateGraph::SET);
        graph.consume(StateGraph::NEXT);
        graph.consume(StateGraph::ACT);
        graph.consume(StateGraph::CHK);
        REQUIRE(graph.state() == StateGraph::GAME_OVER);
    }

    SECTION("stategraph consume(event) - FCHK"){
        graph.consume(StateGraph::INI);
        graph.consume(StateGraph::SET);
        graph.consume(StateGraph::NEXT);
        graph.consume(StateGraph::ACT);
        graph.consume(StateGraph::FCHK);
        REQUIRE(graph.state() == StateGraph::PLAYER_SWAP);
    }

    SECTION("stategraph consume(event) - ERRCS"){
        graph.consume(StateGraph::INI);
        graph.consume(StateGraph::SET);
        graph.consume(StateGraph::NEXT);
        graph.consume(StateGraph::FACT);
        graph.consume(StateGraph::ERRCS);
        REQUIRE(graph.state() == StateGraph::PLAYER_TURN);
    }

    SECTION("stategraph consume(event) - RWD"){
        graph.consume(StateGraph::INI);
        graph.consume(StateGraph::SET);
        graph.consume(StateGraph::NEXT);
        graph.consume(StateGraph::ACT);
        graph.consume(StateGraph::CHK);
        graph.consume(StateGraph::RWD);
        REQUIRE(graph.state() == StateGraph::NOT_STARTED);
    }

    SECTION("stategraph consume(event) - END"){
        graph.consume(StateGraph::INI);
        graph.consume(StateGraph::SET);
        graph.consume(StateGraph::NEXT);
        graph.consume(StateGraph::ACT);
        graph.consume(StateGraph::CHK);
        graph.consume(StateGraph::END);
        REQUIRE(graph.state() == StateGraph::EOG);
    }

    SECTION("stategraph consume(event) - STOP"){
        graph.consume(StateGraph::INI);
        graph.consume(StateGraph::SET);
        graph.consume(StateGraph::NEXT);
        graph.consume(StateGraph::STOP);
        REQUIRE(graph.state() == StateGraph::EOG);
    }

    SECTION("stategraph consume(event) - Error in NOT_STARTED"){
        for(int i = StateGraph::LOAD; i <= StateGraph::STOP; i++){
            REQUIRE(!graph.canConsume(static_cast<StateGraph::Event>(i)));
            graph.consume(static_cast<StateGraph::Event>(i));
            REQUIRE(graph.state() == StateGraph::ERROR);
            graph.setState(StateGraph::NOT_STARTED);
        }
    }

    SECTION("stategraph consume(event) - Error in SET_UP"){
        graph.setState(StateGraph::SET_UP);

        REQUIRE(!graph.canConsume(StateGraph::INI));
        graph.consume(StateGraph::INI);
        REQUIRE(graph.state() == StateGraph::ERROR);

        graph.setState(StateGraph::SET_UP);
        for(int i = StateGraph::ACT; i <= StateGraph::STOP; i++){
            REQUIRE(!graph.canConsume(static_cast<StateGraph::Event>(i)));
            graph.consume(static_cast<StateGraph::Event>(i));
            REQUIRE(graph.state() == StateGraph::ERROR);
            graph.setState(StateGraph::SET_UP);
        }
    }

    SECTION("stategraph consume(event) - Error in ERROR_SETUP"){
        graph.setState(StateGraph::ERROR_SETUP);

        for(int i = StateGraph::INI; i <= StateGraph::NEXT; i++){
            REQUIRE(!graph.canConsume(static_cast<StateGraph::Event>(i)));
            graph.consume(static_cast<StateGraph::Event>(i));
            REQUIRE(graph.state() == StateGraph::ERROR);
            graph.setState(StateGraph::ERROR_SETUP);
        }

        for(int i = StateGraph::RWD; i <= StateGraph::STOP; i++){
            REQUIRE(!graph.canConsume(static_cast<StateGraph::Event>(i)));
            graph.consume(static_cast<StateGraph::Event>(i));
            REQUIRE(graph.state() == StateGraph::ERROR);
            graph.setState(StateGraph::ERROR_SETUP);
        }
    }

    SECTION("stategraph consume(event) - Error in PLAYER_TURN"){
        graph.setState(StateGraph::PLAYER_TURN);

        for(int i = StateGraph::INI; i <= StateGraph::FLOAD; i++){
            REQUIRE(!graph.canConsume(static_cast<StateGraph::Event>(i)));
            graph.consume(static_cast<StateGraph::Event>(i));
            REQUIRE(graph.state() == StateGraph::ERROR);
            graph.setState(StateGraph::PLAYER_TURN);
        }

        for(int i = StateGraph::CHK; i < StateGraph::STOP; i++){
            REQUIRE(!graph.canConsume(static_cast<StateGraph::Event>(i)));
            graph.consume(static_cast<StateGraph::Event>(i));
            REQUIRE(graph.state() == StateGraph::ERROR);
            graph.setState(StateGraph::PLAYER_TURN);
        }
    }

    SECTION("stategraph consume(event) - Error in GAME_TURN"){
        graph.setState(StateGraph::GAME_TURN);

        for(int i = StateGraph::INI; i <= StateGraph::FACT; i++){
            REQUIRE(!graph.canConsume(static_cast<StateGraph::Event>(i)));
            graph.consume(static_cast<StateGraph::Event>(i));
            REQUIRE(graph.state() == StateGraph::ERROR);
            graph.setState(StateGraph::GAME_TURN);
        }

        for(int i = StateGraph::NEXT; i <= StateGraph::STOP; i++){
            REQUIRE(!graph.canConsume(static_cast<StateGraph::Event>(i)));
            graph.consume(static_cast<StateGraph::Event>(i));
            REQUIRE(graph.state() == StateGraph::ERROR);
            graph.setState(StateGraph::GAME_TURN);
        }
    }

    SECTION("stategraph consume(event) - Error in PLAYER_SWAP"){
        graph.setState(StateGraph::PLAYER_SWAP);

        for(int i = StateGraph::INI; i <= StateGraph::FCHK; i++){
            REQUIRE(!graph.canConsume(static_cast<StateGraph::Event>(i)));
            graph.consume(static_cast<StateGraph::Event>(i));
            REQUIRE(graph.state() == StateGraph::ERROR);
            graph.setState(StateGraph::PLAYER_SWAP);
        }

        for(int i = StateGraph::ERRCS; i <= StateGraph::STOP; i++){
            REQUIRE(!graph.canConsume(static_cast<StateGraph::Event>(i)));
            graph.consume(static_cast<StateGraph::Event>(i));
            REQUIRE(graph.state() == StateGraph::ERROR);
            graph.setState(StateGraph::PLAYER_SWAP);
        }
    }

    SECTION("stategraph consume(event) - Error in ERROR_ACTION"){
        graph.setState(StateGraph::ERROR_ACTION);

        for(int i = StateGraph::INI; i <= StateGraph::NEXT; i++){
            REQUIRE(!graph.canConsume(static_cast<StateGraph::Event>(i)));
            graph.consume(static_cast<StateGraph::Event>(i));
            REQUIRE(graph.state() == StateGraph::ERROR);
            graph.setState(StateGraph::ERROR_ACTION);
        }

        for(int i = StateGraph::RWD; i <= StateGraph::STOP; i++){
            REQUIRE(!graph.canConsume(static_cast<StateGraph::Event>(i)));
            graph.consume(static_cast<StateGraph::Event>(i));
            REQUIRE(graph.state() == StateGraph::ERROR);
            graph.setState(StateGraph::ERROR_ACTION);
        }
    }

    SECTION("stategraph consume(event) - Error in GAME_OVER"){
        graph.setState(StateGraph::GAME_OVER);

        for(int i = StateGraph::INI; i <= StateGraph::ERRCS; i++){
            REQUIRE(!graph.canConsume(static_cast<StateGraph::Event>(i)));
            graph.consume(static_cast<StateGraph::Event>(i));
            REQUIRE(graph.state() == StateGraph::ERROR);
            graph.setState(StateGraph::GAME_OVER);
        }

        REQUIRE(!graph.canConsume(StateGraph::STOP));
        graph.consume(StateGraph::STOP);
        REQUIRE(graph.state() == StateGraph::ERROR);
    }

    SECTION("stategraph consume(event) - Error in EOG"){
        graph.setState(StateGraph::EOG);

        for(int i = StateGraph::INI; i <= StateGraph::STOP; i++){
            REQUIRE(!graph.canConsume(static_cast<StateGraph::Event>(i)));
            graph.consume(static_cast<StateGraph::Event>(i));
            REQUIRE(graph.state() == StateGraph::ERROR);
            graph.setState(StateGraph::EOG);
        }
    }

    SECTION("stategraph consume(event) - Error in ERROR"){
        graph.setState(StateGraph::ERROR);

        for(int i = StateGraph::INI; i <= StateGraph::STOP; i++){
            REQUIRE(!graph.canConsume(static_cast<StateGraph::Event>(i)));
            graph.consume(static_cast<StateGraph::Event>(i));
            REQUIRE(graph.state() == StateGraph::ERROR);
            graph.setState(StateGraph::ERROR);
        }
    }
}
