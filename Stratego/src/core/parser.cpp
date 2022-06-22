#include "gamestuff.h"
#include "piece.h"
#include "util.h"
#include "pieceFactory.h"

#include <regex>

using namespace stratego::model;

ConfigFileParser::ConfigFileParser(ParseInfo& info, const std::string& filename) :
    file_ {filename},
    result_ {},
    info_ {info}
{
    if(file_.fail())
        throw std::invalid_argument("Cannot open the given file");
}

void ConfigFileParser::parse(){
    if(!canParse())
        throw std::logic_error("Your file cannot be parsed");

    file_.clear();
    file_.seekg(0);
    result_.clear();

    std::string line {};
    std::smatch matcher {};
    std::regex regex {"(10|[1-9]|D|B)" + util::strrepeat("[[:space:]]+(10|[1-9]|D|B)", 9)};
    int y {info_.color == Color::RED ? info_.board.size() - 2 : 1};
    while(std::getline(file_, line) && std::regex_match(line, matcher, regex)){
        for(int x = 1; x < 11; x++){
            int rank = Piece::toRank(matcher[x].str());
            result_.push_back(toPiece(rank, x, y));
        }

        y += info_.color == Color::RED ? -1 : 1;
    }
}

bool ConfigFileParser::canParse() noexcept{
    file_.clear();
    file_.seekg(0);

    return canParse(file_);
}

bool ConfigFileParser::canParse(std::ifstream& file_){
    std::string line {};
    std::map<int, int> hist {};
    std::smatch matcher {};
    std::regex regex {"(10|[1-9]|D|B)" + util::strrepeat("[[:space:]]+(10|[1-9]|D|B)", 9)};

    while(std::getline(file_, line) && std::regex_match(line, matcher, regex)){
        for(int i = 1; i < 11; i++){
            int rank = Piece::toRank(matcher[i].str());
            if(rank < Config::PIECE_MIN_RANK || rank > Config::PIECE_MAX_RANK)
                return false;

            hist[rank] = hist.find(rank) != hist.end() ? hist[rank] + 1 : 1;
        }
    }

    if(line.empty()){
        for(const auto& [key, value] : hist){
            int count {Piece::pieceInfo[key].count};
            if(value != count){
                return false;
            }
        }

        return true;
    }

    return false;
}

std::vector<Piece*>& ConfigFileParser::result() noexcept{
    return result_;
}

Piece* ConfigFileParser::toPiece(int rank, int x, int y) const{
    Piece* piece {};

    PieceFactory pfactory {};
    piece = pfactory.createPiece(rank, {x, y}, info_.color, info_.board, info_.graph, info_.hist);
    return piece;
}
