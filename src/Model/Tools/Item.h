#ifndef MINI_RPG_ITEM_H_H
#define MINI_RPG_ITEM_H_H

#include "Tool.h"
#include "../../ItemReference.h"

class Item : public Tool {

public:
    explicit Item(ItemReference* ref) : Tool(ref) {};

    std::string type() { return "item"; };

    int healing() { return ref_->healing(); };
    std::string ability() { return ref_->ability(); };
};

#endif //MINI_RPG_ITEM_H_H
