#include <fstream>
#include "properties.h"

using namespace stratego;

Properties::Properties(const std::string& filename){
    reload(filename);
}

void Properties::reload(const std::string& filename){
    std::ifstream ifs {filename};
    if(ifs.good()){
        keyPropertiesMap_.clear();
        std::string line {};
        while(std::getline(ifs, line)){
            keyPropertiesMap_[line.substr(0, line.find("="))] = line.substr(line.find("=") + 1);
        }
    }
}

std::string Properties::propertyOf(const std::string &key) const noexcept{
    if(keyPropertiesMap_.find(key) == keyPropertiesMap_.end())
        return {};

    return keyPropertiesMap_.at(key);
}
