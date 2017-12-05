#include "Character.h"

Character::Character() : Sprite(0, 0, false, "", 0, 0),
                         activeEnemy_(false), isPlayer_(false), health_(100), strength_(0), speed_(0), weapon_(nullptr) {};

// Note that row/col don't need to be in the ctor because their only needed for char sheets
Character::Character(int str, int sp, float x, float y, bool sheet, std::string path, int w, int h) :
        Sprite(x, y, sheet, path, w, h),
        activeEnemy_(false), isPlayer_(false), health_(100), strength_(str), speed_(sp), weapon_(nullptr) {};

std::string Character::weaponPath() const {
    if (weapon_ == nullptr) return "error: no weapon set";
    return weapon_->path();
};

std::string Character::activeWeaponPath() const {
    if (weapon_ == nullptr) return "error: no weapon set";
    return weapon_->activePath();
};

int Character::weaponStrength() const {
    if (weapon_ == nullptr) return 0;
    return weapon_->strength();
};

int Character::weaponWeight() const {
    if (weapon_ == nullptr) return 0;
    return weapon_->weight();
};

int Character::attack(Character* c) const {
    std::random_device rd;
    std::mt19937 generate_rand(rd());;
    std::uniform_int_distribution<> distr(1, 5);

    int str = strength_;
    if (weapon_ != nullptr) str += weapon_->strength();
    int damage = int(floor(distr(generate_rand) * str));
    c->hit(damage);
    return damage;
};

void Character::hit(int n) {
    health_ -= n;
    if (health_ < 0) health_ = 0;
};

void Character::equipWeapon(Weapon* w) {
    if (weapon_ != nullptr) delete weapon_;
    weapon_ = w;
};

void Character::removeWeapon() {
    if (weapon_ == nullptr) return;
    delete weapon_;
    weapon_ = nullptr;
};

int Character::timeOut() {
    int maxTimeout = 30; // 1.5s

    std::random_device rd;
    std::mt19937 generate_rand(rd());;
    std::uniform_int_distribution<> distr(1, 3);

    // Note: max speed is 5 (or else we might get a negative timeout)
    int spd = speed_;
    if (weapon_ != nullptr) spd -= weapon_->weight();
    return maxTimeout - int(floor(distr(generate_rand) * speed_));
}