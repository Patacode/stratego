#include <stdlib.h>
#include "config.h"
#include "util.h"
#include "properties.h"

using namespace stratego;

std::string Config::EXEC_PATH;
std::string Config::RESOURCES_PATH;
std::string Config::CONFIG_PATH;
std::string Config::BOARD_CONFIG_PATH;
std::string Config::IMAGE_PATH;
Properties Config::PIECE_DATA {""};
Properties Config::ACTION_DATA {""};

void Config::setDynamicResources(const std::string& pathToExec){
    std::string resources_path {pathToExec.substr(0, pathToExec.rfind(SLASH))
                + SLASH + ".."
                + SLASH + ".."
                + SLASH + "resources"
                + SLASH};

    EXEC_PATH = pathToExec;
    RESOURCES_PATH = resources_path;
    CONFIG_PATH = resources_path + "config" + SLASH;
    BOARD_CONFIG_PATH = resources_path + "config" + SLASH + "board" + SLASH;
    IMAGE_PATH = resources_path + "images" + SLASH;
    PIECE_DATA.reload(CONFIG_PATH + "piece.properties");
    ACTION_DATA.reload(CONFIG_PATH + "action.properties");
}
