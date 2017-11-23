#include "Character.h"

// Note that row/col don't need to be in the ctor because their only needed for char sheets
Character::Character(int str, int sp, float x, float y, bool sheet, std::string path, int w, int h) :
        Sprite(x, y, sheet, path, w, h),
        activeEnemy_(false), health_(100), strength_(str), speed_(sp), weapon_(nullptr) {};

int Character::attack(Character* c) const {
    auto generate_rand = std::bind(std::uniform_int_distribution<float>(1,5), std::mt19937(time(0)));

    int damage = floor(generate_rand() * strength_);
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