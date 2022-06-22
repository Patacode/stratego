#include "eventMgr.h"

using namespace stratego::model;

StateGraph::StateGraph() noexcept:
    graph_ {{NOT_STARTED, {{INI, SET_UP}}},
            {SET_UP, {{LOAD, SET_UP}, {SET, PLAYER_SWAP}, {FLOAD, ERROR_SETUP}}},
            {ERROR_SETUP, {{ERRCS, SET_UP}}},
            {PLAYER_TURN, {{ACT, GAME_TURN}, {FACT, ERROR_ACTION}, {STOP, EOG}}},
            {GAME_TURN, {{CHK, GAME_OVER}, {FCHK, PLAYER_SWAP}}},
            {PLAYER_SWAP, {{NEXT, PLAYER_TURN}}},
            {ERROR_ACTION, {{ERRCS, PLAYER_TURN}}},
            {GAME_OVER, {{RWD, NOT_STARTED}, {END, EOG}}},
            {EOG, {}},
            {ERROR, {}}},
    state_ {NOT_STARTED}
{}

void StateGraph::consume(Event event) noexcept{
    if(!canConsume(event)){
        state_ = ERROR;
        return;
    }

    state_ = graph_.at(state_)[event];
}

bool StateGraph::canConsume(Event event) const noexcept{
    return graph_.at(state_).find(event) != graph_.at(state_).end();
}

StateGraph::State StateGraph::state() const noexcept{
    return state_;
}

void StateGraph::setState(State state) noexcept{
    state_ = state;
}
