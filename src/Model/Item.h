#ifndef MINI_RPG_ITEM_H
#define MINI_RPG_ITEM_H

#include "Sprite.h"

class Item : public Sprite {
public:
    enum Type {HEART};

private:
    Type t_;

public:
    Item(Type t, float x, float y, bool sheet, std::string path, int w, int h) :
            Sprite(x, y, sheet, path, w, h), t_(t) {};

    // Accessors
    Type type() { return t_; };
};


#endif //MINI_RPG_ITEM_H
