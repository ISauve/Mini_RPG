#ifndef MINI_RPG_CHARACTER_H
#define MINI_RPG_CHARACTER_H

#include "Sprite.h"

struct Weapon {
private:
    int strength_;
    int weight_;
    std::string path_;
    std::string activePath_;

public:
    Weapon(int str, int wght, std::string path, std::string activePath) :
            strength_(str), weight_(wght), path_(path), activePath_(activePath) {};

    int strength() { return strength_; };
    int weight() { return weight_; };
    std::string path() { return path_; };
    std::string activePath() { return activePath_; };
};

class Character : public Sprite {
    bool activeEnemy_;
    bool isPlayer_;

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
    bool hasWeapon() const { return weapon_ != nullptr; };

    std::string weaponPath() const;
    std::string activeWeaponPath() const;
    int weaponStrength() const;
    int weaponWeight() const;
    bool isAlive() const { return health_ > 0; };

    // Mutators
    void move(float n, float m) { x_ += n; y_ += m; };
    void hit(int);
    void setActiveEnemy(bool b) { activeEnemy_ = b; };
    void equipWeapon(Weapon*);
    void removeWeapon();
    void addHealth(int h) { health_ += h; };
    void setPlayer(bool b) { isPlayer_ = b; };

    bool operator == (const Character& c) const {
        return (this->x() == c.x() &&           // same location
                this->y() == c.y() &&
                this->path() == c.path() &&     // same image
                this->row() == c.row() &&
                this->col() == c.col());
    }
};


#endif //MINI_RPG_CHARACTER_H
