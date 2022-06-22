#include <config.h>
#include <util.h>

#include "action.h"

using namespace stratego::view;

const std::array<std::string, 3> Action::infoNames_ {"desc", "syntax", "regex"};
const std::array<std::string, 9> Action::valueNames {"move", "attack", "help", "rules", "piece", "stop", "stat", "history", "clear"};
const std::map<std::string, Action::Value> Action::nameActionMap_ {
    {"move", MOVE},
    {"attack", ATTACK},
    {"help", HELP},
    {"rules", RULES},
    {"piece", PIECE},
    {"stop", STOP},
    {"stat", STAT},
    {"history", HISTORY},
    {"clear", CLEAR}
};

Action::Action(Value value) noexcept :
    action_ {value}
{}

Action::Action(const std::string& str) :
    action_ {static_cast<Value>(0)}
{
    setAction(str);
}

std::string Action::fetchInfo(Value value, Info info) const{
    std::string key {valueNames[value] + "." + infoNames_[info]};
    return Config::ACTION_DATA.propertyOf(key);
}

std::string Action::description() const noexcept{
    return fetchInfo(action_, DESC);
}

std::string Action::syntax() const noexcept{
    return fetchInfo(action_, SYNTAX);
}

std::regex Action::regex() const noexcept{
    return std::regex{fetchInfo(action_, REGEX), std::regex_constants::icase};
}

void Action::setAction(Value value) noexcept{
    action_ = value;
}

void Action::setAction(const std::string &str){
    std::string tmp {str};
    util::strtolower(tmp);
    tmp = tmp.substr(0, str.find(' '));

    if(nameActionMap_.find(tmp) == nameActionMap_.end())
        throw std::invalid_argument("No matching action");

    action_ = nameActionMap_.at(tmp);
}


/* ========================== ActionMatcher =========================== */
ActionMatcher::ActionMatcher() noexcept:
    ActionMatcher {static_cast<Action::Value>(0)}
{}

ActionMatcher::ActionMatcher(Action::Value action) noexcept:
    action_ {action},
    matcher_ {}
{}

bool ActionMatcher::match(const std::string& str) noexcept{
    return std::regex_match(str, matcher_, action_.regex());
}

Action::Value ActionMatcher::action() const noexcept{
    return action_;
}

void ActionMatcher::setAction(const std::string &str){
    action_.setAction(str);
}

std::smatch& ActionMatcher::matcher() noexcept{
    return matcher_;
}
