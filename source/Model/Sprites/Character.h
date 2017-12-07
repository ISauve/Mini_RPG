#ifndef MINI_RPG_CHARACTER_H
#define MINI_RPG_CHARACTER_H

#include "Sprite.h"
#include <vector>

class Tool;
class Weapon;
class Armor;
class Character : public Sprite {
    bool activeEnemy_;
    bool isPlayer_;

    // Base stats
    int health_;
    int strength_;
    int speed_;

    // Tools owned by this character
    std::vector< Tool* > tools_;

    // Equipped items
    Weapon* weapon_;
    Armor* armor_;

public:
    Character();
    Character(int, int, float, float, bool, std::string, int, int);

    // Attack another player - damage based on strength & weapon strength
    int attack(Character*) const;

    // Determine timeout before next hit based on speed & weapon weight
    int timeOut();

    // Accessors
    bool isActiveEnemy() const { return activeEnemy_; };
    bool isPlayer() const { return isPlayer_; };
    int health() const { return health_; };
    int strength() const { return strength_; };
    int speed() const { return speed_; };
    bool isAlive() const { return health_ > 0; };

    // Mutators
    void move(float n, float m) { x_ += n; y_ += m; };
    void hit(int);
    void setActiveEnemy(bool b) { activeEnemy_ = b; };
    void addHealth(int h) { health_ += h; };
    void setPlayer(bool b) { isPlayer_ = b; };

    // Weapon-related functions
    bool hasWeapon() const { return weapon_ != nullptr; };
    std::string weaponPath() const;
    std::string activeWeaponPath() const;
    int weaponStrength() const;
    int weaponWeight() const;
    void equipWeapon(Weapon*);
    void removeWeapon();

    // Tool-related functions
    // ie drop, add, etc        todo

    bool operator == (const Character& c) const {
        return (this->x() == c.x() &&           // same location
                this->y() == c.y() &&
                this->path() == c.path() &&     // same image
                this->row() == c.row() &&
                this->col() == c.col());
    }
};


#endif //MINI_RPG_CHARACTER_H
