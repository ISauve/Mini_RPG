#include "Character.h"
#include "Prop.h"
#include "../Tools/Weapon.h"
#include "../Tools/Item.h"
#include "../Notification.h"
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

std::string Character::weaponName() const {
    if (weapon_ == nullptr) return "error: no weapon set";
    return weapon_->name();
}

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
    unequipWeapon();
    weapon_ = w;
};

void Character::unequipWeapon() {
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
    if (quickAccess_.size() < 8) quickAccess_.push_back(t);
    else bag_.push_back(t);
}

std::vector<Prop> Character::quickAccessContents() {
    // TODO: scale the items so that they're the correct size to fit in the slot

    std::vector<Prop> props;
    // Quick access bar starts at 810/30 & each slot is 100 wide/103 tall
    for (int i=0; i < int(quickAccess_.size()); i++) {
        Prop prop = Prop::makeProp(quickAccess_[i]->name(), 810 + 50 + 100*i, 30 + 52.5);
        props.push_back(prop);
    }
    return props;
}

std::vector<Prop> Character::items() {
    // TODO: scale the items so that they're the correct size to fit in the slot

    std::vector<Prop> props;
    // Backpack: starts at 1148/200, each slot is 100 wide/100 tall, 3 rows of 8
    for (int i=0; i < int(bag_.size()); i++) {
        int top = 50;
        if (i >=8 && i < 16) top = 150;
        else if (i >= 16) top = 250;

        int left = 50 + 100 * (i % 8);

        Prop prop = Prop::makeProp(bag_[i]->name(), 1148 + left, 200 + top);
        props.push_back(prop);
    }
    // Belt: starts at 1148/700, each slot is 100 wide/103 tall, 1 row of 7
    for (int i=0; i < int(quickAccess_.size()); i++) {
        Prop prop = Prop::makeProp(quickAccess_[i]->name(), 1148 + 50 + 100*i, 700 + 52.5);
        props.push_back(prop);
    }
    return props;
}

Notification Character::useQuickAccess(int slot) {
    Tool* tool = quickAccess_[slot];
    Notification notification;

    // If it's a weapon, equip it
    if (tool->type() == "weapon") {
        Weapon* weapon = dynamic_cast< Weapon* >(tool);
        equipWeapon(weapon);
    } else if (tool->type() == "item") {
        Item* item = dynamic_cast< Item* >(tool);
        if (item->healing() > 0) {
            addHealth(item->healing());
            notification = Notification::HEALED;
            notification.healed = item->healing();
        }
    }

    quickAccess_.erase(quickAccess_.begin() + slot);
    return notification;
}