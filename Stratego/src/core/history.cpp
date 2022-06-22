#include "gamestuff.h"

using namespace stratego::model;

History::History(int bound) noexcept:
    history_ {{SUCCESS, {}}, {FAILURE, {}}, {HINT, {}}},
    counter_ {},
    bound_ {bound}
{}

void History::addInfo(InfoType type, const std::string &info){
    if(counter_ >= bound_)
        throw std::out_of_range("Cannot add more elements of this type");

    history_[type].push_back(info);
    ++counter_;
}

void History::addSuccess(const std::string& info){
    addInfo(SUCCESS, info);
}

void History::addFailure(const std::string& info){
    addInfo(FAILURE, info);
}

void History::addHint(const std::string &info){
    addInfo(HINT, info);
}

std::string& History::lastInfo(InfoType type){
    return const_cast<std::string&>(std::as_const(*this).lastInfo(type));
}

const std::string& History::lastInfo(InfoType type) const{
    if(history_.at(type).empty())
        throw std::logic_error("No informations associated with the given type");

    return history_.at(type).back().info;
}

std::string& History::lastSuccess(){
    return const_cast<std::string&>(std::as_const(*this).lastSuccess());
}

const std::string& History::lastSuccess() const{
    return lastInfo(SUCCESS);
}

std::string& History::lastFailure(){
    return const_cast<std::string&>(std::as_const(*this).lastFailure());
}

const std::string& History::lastFailure() const{
    return lastInfo(FAILURE);
}

std::string& History::lastHint(){
    return const_cast<std::string&>(std::as_const(*this).lastHint());
}

const std::string& History::lastHint() const{
    return lastInfo(HINT);
}

std::string History::popInfo(InfoType type){
    if(history_[type].empty())
        throw std::logic_error("No informations associated with the given type");

    std::string& info {history_[type].back().info};
    history_[type].pop_back();
    --counter_;
    return info;
}

std::string History::popSuccess(){
    return popInfo(SUCCESS);
}

std::string History::popFailure(){
    return popInfo(FAILURE);
}

std::string History::popHint(){
    return popInfo(HINT);
}

void History::clear() noexcept{
    for(int i = SUCCESS; i <= HINT; i++){
        clear(static_cast<InfoType>(i));
    }
}

void History::clear(InfoType type) noexcept{
    counter_ -= history_[type].size();
    history_[type].clear();
}

void History::clearSuccess() noexcept{
    clear(SUCCESS);
}

void History::clearFailure() noexcept{
    clear(FAILURE);
}

void History::clearHint() noexcept{
    clear(HINT);
}

void History::saveToFile(InfoType type, const std::string &filename) const noexcept{
    std::ofstream file {filename};
    if(history_.at(type).empty())
        file << "Aucunes informations de type donné n'a été trouvé." << "\n";

    for(size_t i = 0; i < history_.at(type).size(); i++){
        file << "[" << history_.at(type)[i].toReadableTime() << "] "
             << history_.at(type)[i].info << "\n";
    }
}

void History::saveSuccess(const std::string &filename) const noexcept{
    saveToFile(SUCCESS, filename);
}

void History::saveFailure(const std::string &filename) const noexcept{
    saveToFile(FAILURE, filename);
}

void History::saveHint(const std::string &filename) const noexcept{
    saveToFile(HINT, filename);
}

std::vector<tmstring>& History::allSuccess() noexcept{
    return const_cast<std::vector<tmstring>&>(std::as_const(*this).allSuccess());
}

const std::vector<tmstring>& History::allSuccess() const noexcept{
    return allInfo(SUCCESS);
}

std::vector<tmstring>& History::allFailure() noexcept{
    return const_cast<std::vector<tmstring>&>(std::as_const(*this).allFailure());
}

const std::vector<tmstring>& History::allFailure() const noexcept{
    return allInfo(FAILURE);
}

std::vector<tmstring>& History::allHint() noexcept{
    return const_cast<std::vector<tmstring>&>(std::as_const(*this).allHint());
}

const std::vector<tmstring>& History::allHint() const noexcept{
    return allInfo(HINT);
}

std::vector<tmstring>& History::allInfo(InfoType type) noexcept{
    return const_cast<std::vector<tmstring>&>(std::as_const(*this).allInfo(type));
}

const std::vector<tmstring>& History::allInfo(InfoType type) const noexcept{
    return history_.at(type);
}

int History::bound() const noexcept{
    return bound_;
}

int History::size() const noexcept{
    return counter_;
}

bool History::isFull() const noexcept{
    return bound_ == counter_;
}
