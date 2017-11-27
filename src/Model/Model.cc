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

std::vector< Item > Model::getItems() {
    return items_;
};