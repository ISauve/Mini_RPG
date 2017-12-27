#include "Model.h"

Model::Model() {
    std::lock_guard<std::mutex> lock(charsLock_);
    resetState();
};

void Model::setChannel(Channel<EventPackage>* ch) {
    eventsChannel_ = ch;
};

Character Model::player() {
    std::lock_guard<std::mutex> lock(charsLock_);
    return player_;
};

std::vector< Character > Model::getChars() {
    std::lock_guard<std::mutex> lock(charsLock_);
    return chars_;
};

std::vector< Prop > Model::getProps() {
    // todo add this lock in other places where props are used (or remove it)
    std::lock_guard<std::mutex> lock(propsLock_);
    return props_;
};

std::string Model::getBackground() {
    return background_;
}

int Model::playerMoney() {
   return playerMoney_.load();
};