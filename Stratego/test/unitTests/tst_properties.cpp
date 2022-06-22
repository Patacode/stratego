#include <catch2/catch.hpp>
#include <properties.h>
#include <config.h>
#include <gamestuff.h>
#include <util.h>

using namespace stratego;
using namespace stratego::model;

TEST_CASE("Properties constructor", "[properties][constructor]"){

    SECTION("constructor(string) valid file"){
        Properties props {Config::CONFIG_PATH + "action.properties"};
        REQUIRE_FALSE(props.propertyOf("help.syntax").empty());
    }
}

TEST_CASE("Properties getter", "[properties][getter]"){

    Properties props {Config::CONFIG_PATH + "piece.properties"};

    SECTION("Properties propertyOf(string)"){
        for(size_t i = 0; i < Piece::pieceInfo.size(); i++){
            std::string name {Piece::pieceInfo[i].name};
            util::strtolower(name);
            REQUIRE_FALSE(props.propertyOf(name + ".desc").empty());
        }
    }
}
