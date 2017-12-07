#ifndef MINI_RPG_ARMOR_H
#define MINI_RPG_ARMOR_H

#include "Tool.h"
#include "../../ItemReference.h"

class Armor : public Tool {

public:
    explicit Armor(ItemReference* ref) : Tool(ref) {};

    std::string type() { return "armor"; };

    int defense() { return ref_->defense(); };
    int weight() { return ref_->weight(); };
    int bonusHealth() { return ref_->bonusHealth(); };
};

#endif //MINI_RPG_ARMOR_H
