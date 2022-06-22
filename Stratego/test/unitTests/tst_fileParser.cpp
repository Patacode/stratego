#include <catch2/catch.hpp>
#include <gamestuff.h>
#include <iostream>

using namespace stratego::model;
using namespace stratego;


TEST_CASE("ConfigFileParser constructor", "[parser][constructor]"){

    Board board {};
    StateGraph graph {};
    History hist {100};
    ParseInfo infos {board, graph, hist, Color::RED};

    SECTION("constructor(ParseInfo, string) wrong filename"){
        REQUIRE_THROWS_AS((ConfigFileParser {infos, ""}), std::invalid_argument);
    }

    SECTION("constructor(ParseInfo, string) invalid file"){
        ConfigFileParser fileParser {infos, Config::BOARD_CONFIG_PATH + "wrong"};
        Parser<std::vector<Piece*>>& parser {fileParser};

        REQUIRE_FALSE(parser.canParse());
    }

    SECTION("constructor(ParseInfo, string) valid file"){
        ConfigFileParser fileParser {infos, Config::BOARD_CONFIG_PATH + "default"};
        Parser<std::vector<Piece*>>& parser {fileParser};

        REQUIRE(parser.canParse());
    }
}

TEST_CASE("ConfigFileParser parsing", "[parser][parsing]"){

    Board board {};
    StateGraph graph {};
    History hist {100};
    ParseInfo infos {board, graph, hist, Color::RED};

    SECTION("parse() invalid"){
        ConfigFileParser fileParser {infos, Config::BOARD_CONFIG_PATH + "wrong"};
        Parser<std::vector<Piece*>>& parser {fileParser};

        REQUIRE_THROWS_AS(parser.parse(), std::logic_error);
        REQUIRE(parser.result().empty());
    }

    SECTION("parse() valid"){
        ConfigFileParser fileParser {infos, Config::BOARD_CONFIG_PATH + "default"};
        Parser<std::vector<Piece*>>& parser {fileParser};

        parser.parse();
        std::vector<Piece*>& pieces {parser.result()};
        REQUIRE_FALSE(pieces.empty());
        for(int i = Config::PIECE_MIN_RANK; i <= Config::PIECE_MAX_RANK; i++){
            int count {};
            for(size_t j = 0; j < pieces.size(); j++){
                if(pieces[j] -> rank() == i)
                    count++;
            }

            REQUIRE(count == Piece::pieceInfo[i].count);
        }


        for(Piece*& p : pieces){
            delete p;
            p = nullptr;
        }
    }
}
