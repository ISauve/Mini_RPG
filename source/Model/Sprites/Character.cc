#include "Character.h"
#include "../Tools/Weapon.h"
#include <random>

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

std::string Character::weaponPathR() const {
    if (weapon_ == nullptr) return "error: no weapon set";
    return weapon_->pathR();
};

std::string Character::activeWeaponPath() const {
    if (weapon_ == nullptr) return "error: no weapon set";
    return weapon_->activePath();
};

std::string Character::activeWeaponPathR() const {
    if (weapon_ == nullptr) return "error: no weapon set";
    return weapon_->activePathR();
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
    if (weapon_ != nullptr) addTool(weapon_);   // unequip current weapon
    weapon_ = w;
};

void Character::removeWeapon() {
    if (weapon_ == nullptr) return;
    addTool(weapon_);
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

void Character::addTool(Tool* t) {
    if (quickAccess_.size() < 7) quickAccess_.push_back(t);
    else bag_.push_back(t);
}

std::vector<Prop> Character::quickAccessContents() {
    std::vector<Prop> props;
    // Quick access bar starts at 810/30 & each slot is 85 wide/90 tall
    for (int i=0; i < int(quickAccess_.size()); i++) {
        Prop prop = Prop::makeProp(quickAccess_[i]->name(), 810 + 43 + 85*i, 30 + 45);
        props.push_back(prop);
    }
    return props;
}

#include <iostream>
void Character::useQuickAccess(int slot) {
    Tool* item = quickAccess_[slot];

    // If it's a weapon, equip it
    try {
        Weapon* weapon = dynamic_cast< Weapon* >(item);
        equipWeapon(weapon);
        quickAccess_.erase(quickAccess_.begin() + slot);
        return;
    } catch (...) {};

    // handle other items       todo
}

/*
std::vector<Tool> Character::bagContents() {
    std::vector<Prop*> props;
    for (auto tool : tools_) {
        Prop* prop = Prop::makeProp(tool);
    }
}
 */