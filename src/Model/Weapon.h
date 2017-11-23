#ifndef MINI_RPG_WEAPON_H
#define MINI_RPG_WEAPON_H

#include <string>
#include "Sprite.h"

class Weapon: public Sprite {
    int strength_;
    int weight_;

public:
    Weapon(int str, int wght, float x, float y, bool sheet, std::string path, int w, int h) :
            Sprite(x, y, sheet, path, w, h), strength_(str), weight_(wght) {};

    int strength() { return strength_; };
    int weight() { return weight_; };
};


#endif //MINI_RPG_WEAPON_H
