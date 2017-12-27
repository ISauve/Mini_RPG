#ifndef MINI_RPG_ITEM_H
#define MINI_RPG_ITEM_H

#include "Sprite.h"

// A prop is an un-acquired version of an item (ie an item on the ground, or being sold)
// It is a standalone sprite
class ItemReference;
class Prop : public Sprite {
    ItemReference* ref_;

    Prop(float x, float y, ItemReference* ref);
public:
    static Prop makeProp(std::string itemName, float x, float y);

    std::string name();
    int dropChance();
    int cost();

    bool acquirable();
    int healing();  // for hearts (immediate)
    int value();    // for money (immediate)

    // TODO add more instance-specific data (like a chest's contents)
};

#endif //MINI_RPG_ITEM_H
