#include <catch2/catch.hpp>
#include <gamestuff.h>

using namespace stratego::model;

TEST_CASE("history constructor", "[history][constructors]"){

    History hist {50};

    SECTION("bound constructor"){
        REQUIRE(hist.size() == 0);
        REQUIRE(hist.bound() == 50);
        REQUIRE_FALSE(hist.isFull());
    }
}

TEST_CASE("history modifiers", "[history][modifiers]"){

    History hist {75};

    SECTION("history addInfo(InfoType, string)"){
        std::string info[] {"Succès ", "Échec "};
        for(int i = History::SUCCESS; i < History::HINT; i++){
            hist.clear();
            for(int j = 0; j < hist.bound(); j++){
                hist.addInfo(static_cast<History::InfoType>(i), info[i] + std::to_string(j));
                REQUIRE(hist.size() == j + 1);
                REQUIRE(!hist.lastInfo(static_cast<History::InfoType>(i)).compare(info[i] + std::to_string(j)));
            }
        }

        REQUIRE_THROWS_AS(hist.addInfo(History::SUCCESS, "Succès"), std::out_of_range);
        REQUIRE_THROWS_AS(hist.addInfo(History::FAILURE, "Échec"), std::out_of_range);
    }

    SECTION("history addSuccess(string)"){
        std::string info {"Succès "};
        for(int i = 0; i < hist.bound(); i++){
            hist.addSuccess(info + std::to_string(i));
            REQUIRE(hist.size() == i + 1);
            REQUIRE(!hist.lastInfo(History::SUCCESS).compare(info + std::to_string(i)));
            REQUIRE_THROWS_AS(hist.lastInfo(History::FAILURE), std::logic_error);
            REQUIRE_THROWS_AS(hist.lastInfo(History::HINT), std::logic_error);
        }

        REQUIRE_THROWS_AS(hist.addSuccess("Succès"), std::out_of_range);
    }

    SECTION("history addFailure(string)"){
        std::string info {"Échec "};
        for(int i = 0; i < hist.bound(); i++){
            hist.addFailure(info + std::to_string(i));
            REQUIRE(hist.size() == i + 1);
            REQUIRE(!hist.lastInfo(History::FAILURE).compare(info + std::to_string(i)));
            REQUIRE_THROWS_AS(hist.lastInfo(History::SUCCESS), std::logic_error);
            REQUIRE_THROWS_AS(hist.lastInfo(History::HINT), std::logic_error);

        }

        REQUIRE_THROWS_AS(hist.addFailure("Échec"), std::out_of_range);
    }

    SECTION("history addHint(string)"){
        std::string info {"Conseil "};
        for(int i = 0; i < hist.bound(); i++){
            hist.addHint(info + std::to_string(i));
            REQUIRE(hist.size() == i + 1);
            REQUIRE(!hist.lastInfo(History::HINT).compare(info + std::to_string(i)));
            REQUIRE_THROWS_AS(hist.lastInfo(History::FAILURE), std::logic_error);
            REQUIRE_THROWS_AS(hist.lastInfo(History::SUCCESS), std::logic_error);

        }

        REQUIRE_THROWS_AS(hist.addFailure("Échec"), std::out_of_range);
    }

    SECTION("history popInfo(InfoType)"){
        REQUIRE_THROWS_AS(hist.popInfo(History::SUCCESS), std::logic_error);
        REQUIRE_THROWS_AS(hist.popInfo(History::FAILURE), std::logic_error);
        REQUIRE_THROWS_AS(hist.popInfo(History::HINT), std::logic_error);

        std::string info {"Info "};
        for(int i = 0; i < hist.bound(); i++){
            hist.addInfo(History::SUCCESS, info + std::to_string(i));
        }

        while(hist.size() > 0){
            REQUIRE(!hist.popInfo(History::SUCCESS).compare(info + std::to_string(hist.size())));
            REQUIRE_THROWS_AS(hist.popInfo(History::FAILURE), std::logic_error);
            REQUIRE_THROWS_AS(hist.popInfo(History::HINT), std::logic_error);
        }

        REQUIRE_THROWS_AS(hist.popInfo(History::SUCCESS), std::logic_error);
        REQUIRE_THROWS_AS(hist.popInfo(History::FAILURE), std::logic_error);
        REQUIRE_THROWS_AS(hist.popInfo(History::HINT), std::logic_error);
    }

    SECTION("history popSuccess()"){
        REQUIRE_THROWS_AS(hist.popSuccess(), std::logic_error);

        std::string info {"Info "};
        for(int i = 0; i < hist.bound(); i++){
            hist.addInfo(History::SUCCESS, info + std::to_string(i));
        }

        while(hist.size() > 0){
            REQUIRE(!hist.popSuccess().compare(info + std::to_string(hist.size())));
        }

        REQUIRE_THROWS_AS(hist.popSuccess(), std::logic_error);
    }

    SECTION("history popFailure()"){
        REQUIRE_THROWS_AS(hist.popFailure(), std::logic_error);

        std::string info {"Info "};
        for(int i = 0; i < hist.bound(); i++){
            hist.addInfo(History::FAILURE, info + std::to_string(i));
        }

        while(hist.size() > 0){
            REQUIRE(!hist.popFailure().compare(info + std::to_string(hist.size())));
        }

        REQUIRE_THROWS_AS(hist.popFailure(), std::logic_error);
    }

    SECTION("history popHint()"){
        REQUIRE_THROWS_AS(hist.popHint(), std::logic_error);

        std::string info {"Info "};
        for(int i = 0; i < hist.bound(); i++){
            hist.addInfo(History::HINT, info + std::to_string(i));
        }

        while(hist.size() > 0){
            REQUIRE(!hist.popHint().compare(info + std::to_string(hist.size())));
        }

        REQUIRE_THROWS_AS(hist.popHint(), std::logic_error);
    }

    SECTION("history clear()"){
        REQUIRE(hist.size() == 0);
        hist.clear();
        REQUIRE(hist.size() == 0);

        std::string info {"Info "};
        srand((unsigned) time(0));
        int rn;
        for(int i = 0; i < hist.bound(); i++){
            rn = rand() % 3;
            hist.addInfo(static_cast<History::InfoType>(rn), info + std::to_string(i));
        }

        REQUIRE(hist.size() == hist.bound());
        hist.clear();
        REQUIRE(hist.size() == 0);
    }

    SECTION("history clear(InfoType)"){
        REQUIRE(hist.size() == 0);
        hist.clear(History::SUCCESS);
        hist.clear(History::FAILURE);
        hist.clear(History::HINT);
        REQUIRE(hist.size() == 0);

        std::string info {"Info "};
        for(int i = 0; i < hist.bound() / 3; i++){
            hist.addInfo(History::SUCCESS, info + std::to_string(i));
            hist.addInfo(History::FAILURE, info + std::to_string(i));
            hist.addInfo(History::HINT, info + std::to_string(i));
        }

        REQUIRE(hist.size() == hist.bound());
        hist.clear(History::SUCCESS);
        REQUIRE(hist.size() == 50);
        hist.clear(History::FAILURE);
        REQUIRE(hist.size() == 25);
        hist.clear(History::HINT);
        REQUIRE(hist.size() == 0);
    }

    SECTION("history clearSuccess()"){
        REQUIRE(hist.size() == 0);
        hist.clearSuccess();
        REQUIRE(hist.size() == 0);

        std::string info {"Info "};
        for(int i = 0; i < hist.bound(); i++){
            hist.addInfo(History::SUCCESS, info + std::to_string(i));
        }

        REQUIRE(hist.size() == hist.bound());
        hist.clearSuccess();
        REQUIRE(hist.size() == 0);
    }

    SECTION("history clearFailure()"){
        REQUIRE(hist.size() == 0);
        hist.clearFailure();
        REQUIRE(hist.size() == 0);

        std::string info {"Info "};
        for(int i = 0; i < hist.bound(); i++){
            hist.addInfo(History::FAILURE, info + std::to_string(i));
        }

        REQUIRE(hist.size() == hist.bound());
        hist.clearFailure();
        REQUIRE(hist.size() == 0);
    }

    SECTION("history clearHint()"){
        REQUIRE(hist.size() == 0);
        hist.clearHint();
        REQUIRE(hist.size() == 0);

        std::string info {"Info "};
        for(int i = 0; i < hist.bound(); i++){
            hist.addInfo(History::HINT, info + std::to_string(i));
        }

        REQUIRE(hist.size() == hist.bound());
        hist.clearHint();
        REQUIRE(hist.size() == 0);
    }
}

TEST_CASE("history getters", "[history][getters]"){

    History hist {75};

    SECTION("history lastInfo(InfoType)"){
        REQUIRE_THROWS_AS(hist.lastInfo(History::SUCCESS), std::logic_error);

        hist.addInfo(History::SUCCESS, "Ceci est un succès");
        hist.addInfo(History::SUCCESS, "Ceci est un autre succès");
        hist.addInfo(History::FAILURE, "Ceci est un échec");
        hist.addInfo(History::HINT, "Ceci est un conseil");

        REQUIRE(!hist.lastInfo(History::SUCCESS).compare("Ceci est un autre succès"));
        REQUIRE(!hist.lastInfo(History::FAILURE).compare("Ceci est un échec"));
        REQUIRE(!hist.lastInfo(History::HINT).compare("Ceci est un conseil"));
    }

    SECTION("history lastSuccess()"){
        REQUIRE_THROWS_AS(hist.lastSuccess(), std::logic_error);

        hist.addInfo(History::SUCCESS, "Ceci est un succès");
        hist.addInfo(History::SUCCESS, "Ceci est un autre succès");
        hist.addInfo(History::FAILURE, "Ceci est un échec");
        hist.addInfo(History::HINT, "Ceci est un conseil");

        REQUIRE(!hist.lastSuccess().compare("Ceci est un autre succès"));
    }

    SECTION("history lastFailure()"){
        REQUIRE_THROWS_AS(hist.lastFailure(), std::logic_error);

        hist.addInfo(History::SUCCESS, "Ceci est un succès");
        hist.addInfo(History::SUCCESS, "Ceci est un autre succès");
        hist.addInfo(History::FAILURE, "Ceci est un échec");
        hist.addInfo(History::HINT, "Ceci est un conseil");

        REQUIRE(!hist.lastFailure().compare("Ceci est un échec"));
    }

    SECTION("history lastHint()"){
        REQUIRE_THROWS_AS(hist.lastHint(), std::logic_error);

        hist.addInfo(History::SUCCESS, "Ceci est un succès");
        hist.addInfo(History::SUCCESS, "Ceci est un autre succès");
        hist.addInfo(History::FAILURE, "Ceci est un échec");
        hist.addInfo(History::HINT, "Ceci est un conseil");

        REQUIRE(!hist.lastHint().compare("Ceci est un conseil"));
    }

    SECTION("history allInfo(InfoType)"){
        REQUIRE(hist.allInfo(History::SUCCESS).empty());
        REQUIRE(hist.allInfo(History::FAILURE).empty());
        REQUIRE(hist.allInfo(History::HINT).empty());

        for(int i = 0; i < hist.bound() / 3; i++){
            hist.addSuccess("Success " + std::to_string(i));
            hist.addFailure("Failure " + std::to_string(i));
            hist.addHint("Hint " + std::to_string(i));
        }

        for(int i = 0; i < 3; i++){
            History::InfoType infoType {static_cast<History::InfoType>(i)};
            std::vector<tmstring>& infos {hist.allInfo(infoType)};
            for(size_t j = 0; j < infos.size(); j++){
                switch(infoType){
                    case History::SUCCESS:
                        REQUIRE(!infos[j].info.compare("Success " + std::to_string(j)));
                        break;
                    case History::FAILURE:
                        REQUIRE(!infos[j].info.compare("Failure " + std::to_string(j)));
                        break;
                    case History::HINT:
                        REQUIRE(!infos[j].info.compare("Hint " + std::to_string(j)));
                }
            }
        }

        hist.clear();
        REQUIRE(hist.allInfo(History::SUCCESS).empty());
        REQUIRE(hist.allInfo(History::FAILURE).empty());
        REQUIRE(hist.allInfo(History::HINT).empty());
    }

    SECTION("history allSuccess()"){
        REQUIRE(hist.allSuccess().empty());
        for(int i = 0; i < hist.bound(); i++){
            hist.addSuccess("Success " + std::to_string(i));
        }

        std::vector<tmstring>& infos {hist.allSuccess()};
        for(size_t i = 0; i < infos.size(); i++){
            REQUIRE(!infos[i].info.compare("Success " + std::to_string(i)));
        }

        hist.clearSuccess();
        REQUIRE(hist.allSuccess().empty());
    }

    SECTION("history allFailure()"){
        REQUIRE(hist.allFailure().empty());
        for(int i = 0; i < hist.bound(); i++){
            hist.addSuccess("Failure " + std::to_string(i));
        }

        std::vector<tmstring>& infos {hist.allFailure()};
        for(size_t i = 0; i < infos.size(); i++){
            REQUIRE(!infos[i].info.compare("Failure " + std::to_string(i)));
        }

        hist.clearFailure();
        REQUIRE(hist.allFailure().empty());
    }

    SECTION("history allHint()"){
        REQUIRE(hist.allHint().empty());
        for(int i = 0; i < hist.bound(); i++){
            hist.addSuccess("Hint " + std::to_string(i));
        }

        std::vector<tmstring>& infos {hist.allHint()};
        for(size_t i = 0; i < infos.size(); i++){
            REQUIRE(!infos[i].info.compare("Hint " + std::to_string(i)));
        }

        hist.clearHint();
        REQUIRE(hist.allHint().empty());
    }

    SECTION("history bound()"){
        REQUIRE(hist.bound() == 75);
    }

    SECTION("history size()"){
        REQUIRE(hist.size() == 0);
        for(int i = 0; i < hist.bound(); i++){
            hist.addInfo(History::SUCCESS, "Succès");
            REQUIRE(hist.size() == i + 1);
        }
    }

    SECTION("history isFull()"){
        for(int i = 0; i < hist.bound(); i++){
            hist.addInfo(History::SUCCESS, "Succès");
        }

        REQUIRE(hist.isFull());
    }
}
