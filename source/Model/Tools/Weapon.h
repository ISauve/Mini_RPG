#ifndef MINI_RPG_WEAPON_H
#define MINI_RPG_WEAPON_H

#include "Tool.h"
#include "../../ItemReference.h"

class Weapon : public Tool {

public:
    explicit Weapon(ItemReference* ref) : Tool(ref) {};

    std::string type() { return "weapon"; };

    int strength() { return ref_->strength(); };
    int weight() { return ref_->weight(); };
    int bonusHealth() { return ref_->bonusHealth(); };

    std::string activePath() { return ref_->activePath(); };
};

#endif //MINI_RPG_WEAPON_H