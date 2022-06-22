#include <cstddef>
#include "qcomponent.h"

using namespace stratego::view;


/* QComponent related (inherited from it) */
void QComponent::compose(){
    connectSlots();
    for(QComponent* component : children_){
        component -> compose();
    }
}

void QComponent::decompose(){
    for(QComponent* component : children_){
        component -> decompose();
    }
}

void QComponent::reload(){
    for(QComponent* component : children_){
        component -> reload();
    }
}


/* Related to this class only */
void QComponent::addChild(QComponent *child){
    children_.push_back(child);
}

void QComponent::addChildren(std::initializer_list<QComponent *> children){
    for(QComponent* child : children){
        children_.push_back(child);
    }
}

void QComponent::removeChild(QComponent *child){
    size_t i {};
    while(i < children_.size() && children_[i] != child) i++;
    if(i < children_.size()){
        children_.erase(children_.begin() + i);
    }
}
