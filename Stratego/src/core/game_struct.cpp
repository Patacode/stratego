#include "gamestuff.h"
#include "util.h"

#include <regex>

using namespace stratego::model;


/* ========================== Position =========================== */
bool stratego::model::operator==(const Position& pos1, const Position& pos2){
    return pos1.x == pos2.x && pos1.y == pos2.y;
}

Position stratego::model::operator+(const Position& pos1, const Position& pos2){
    return {pos1.x + pos2.x, pos1.y + pos2.y};
}

Position stratego::model::operator+(const Position& pos, Direction::Value direction){
    switch(direction){
        case Direction::UP: return {pos.x, pos.y - 1};
        case Direction::DOWN: return {pos.x, pos.y + 1};
        case Direction::LEFT: return {pos.x - 1, pos.y};
        case Direction::RIGHT: return {pos.x + 1, pos.y};
    }

    return {};
}

Position::operator std::string() const{
    return std::to_string(y) + std::string{static_cast<char>(x + ('A' - 1))};
}

Position Position::from(const std::string &str){
    if(std::regex_match(str, std::regex{"( (10|[1-9]{1})[a-j]{1})", std::regex_constants::icase})){
        throw std::invalid_argument("Invalid string to convert to position");
    }

    auto it = std::find_if(str.begin(), str.end(), [](unsigned char c){return !std::isdigit(c);});
    int y {std::stoi(str.substr(0, it - str.begin()))};
    int x {std::tolower(str.substr(it - str.begin()).at(0)) - ('a' - 1)};
    return {x, y};
}


/* ========================== Direction =========================== */
Direction::Direction(Direction::Value v) noexcept:
    v_ {v}
{}

Direction Direction::from(const std::string& str){
    if(std::regex_match(str, std::regex{"( (up|down|left|right))", std::regex_constants::icase})){
        throw std::invalid_argument("Invalid string to convert to direction");
    }

    std::string op {str};
    util::strtoupper(op);
    if(op == "UP") return {Direction::UP};
    if(op == "DOWN") return {Direction::DOWN};
    if(op == "LEFT") return {Direction::LEFT};
    if(op == "RIGHT") return {Direction::RIGHT};

    return {static_cast<Direction::Value>(0)};
}


/* ========================== Cell =========================== */
const std::array<std::string, 3> Cell::typeNames {"Normale", "Mur", "Eau"};

bool operator==(Cell& c1, Cell& c2){
    return c1.type == c2.type && c1.pos == c2.pos && c1.piece == c2.piece;
}


/* ========================== tmstring =========================== */
tmstring::tmstring(const std::string& information) noexcept : info {information}{
    std::time_t t = std::time(0);
    time = *std::localtime(&t);
}

std::string tmstring::toReadableTime() const noexcept{
    std::string date_sep {"-"};
    std::string time_sep {":"};
    std::string whts {" "};

    return std::to_string(time.tm_mday)        + date_sep +
           std::to_string(time.tm_mon + 1)     + date_sep +
           std::to_string(time.tm_year + 1900) + whts     +
           std::to_string(time.tm_hour)        + time_sep +
           std::to_string(time.tm_min)         + time_sep +
           std::to_string(time.tm_sec);
}
