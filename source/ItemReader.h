#ifndef MINI_RPG_REFERENCE_H
#define MINI_RPG_REFERENCE_H

#include "ItemReference.h"
#include <string>
#include <map>

// Reads configuration files for items & stores references for them
// Singleton design pattern: one of these is created when the game starts, & that's it
class ItemReader {
    static ItemReader* instance_;
    std::map<std::string, ItemReference> references_;

    ItemReader() = default;
    ItemReader(ItemReader const&){};            // copy ctor
    ItemReader&operator=(ItemReader const&){};  // assignment operator
public:
    static ItemReader* instance() {
        if (!instance_) instance_ = new ItemReader;
        return instance_;
    };

    void readItemConfigurations() {
        // TODO: fill the ItemReference map
    };

    ItemReference* getReference(std::string itemName) {
        return &(references_[itemName]);
    }
};

#endif //MINI_RPG_REFERENCE_H
