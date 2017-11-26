#ifndef MINI_RPG_CHARACTER_H
#define MINI_RPG_CHARACTER_H

#include "Sprite.h"
#include "Weapon.h"

class Character : public Sprite {
    bool activeEnemy_;

    // Stats
    int health_;
    int strength_;
    int speed_;

    // Tools
    Weapon* weapon_;
    //Armor armor_;

public:
    Character();
    Character(int, int, float, float, bool, std::string, int, int);
    int attack(Character*) const;

    // Accessors
    int health() const { return health_; };
    bool isAlive() const { return health_ > 0; };
    bool isActiveEnemy() const { return activeEnemy_; };
    std::string weaponPath() const { return weapon_->path(); };

    // Mutators
    void move(float n, float m) { x_ += n; y_ += m; };
    void hit(int);
    void setActiveEnemy(bool b) { activeEnemy_ = b; };
    void equipWeapon(Weapon*);
    void removeWeapon() { hasWeapon_ = false; }

    bool operator == (const Character& c) const {
        return (this->x() == c.x() &&           // same location
                this->y() == c.y() &&
                this->path() == c.path() &&     // same image
                this->row() == c.row() &&
                this->col() == c.col());
    }
};


#endif //MINI_RPG_CHARACTER_H
