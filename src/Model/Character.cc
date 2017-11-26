#include "Character.h"

Character::Character() : Sprite(0, 0, false, "", 0, 0),
                         activeEnemy_(false), health_(100), strength_(0), speed_(0), weapon_(nullptr) {};

// Note that row/col don't need to be in the ctor because their only needed for char sheets
Character::Character(int str, int sp, float x, float y, bool sheet, std::string path, int w, int h) :
        Sprite(x, y, sheet, path, w, h),
        activeEnemy_(false), health_(100), strength_(str), speed_(sp), weapon_(nullptr) {};

int Character::attack(Character* c) const {
    std::random_device rd;
    std::mt19937 generate_rand(rd());;
    std::uniform_int_distribution<> distr(1, 5);

    int damage = int(floor(distr(generate_rand) * strength_));
    c->hit(damage);
    return damage;
};

void Character::hit(int n) {
    health_ -= n;
    if (health_ < 0) health_ = 0;
};

void Character::equipWeapon(Weapon* w) {
    weapon_ = w;
    hasWeapon_ = true;
    weaponPath_ = w->path();
};