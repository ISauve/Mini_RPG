#ifndef MINI_RPG_NOTIFICATION_H
#define MINI_RPG_NOTIFICATION_H

#include "Sprites/Character.h"

struct Notification {
    enum Code {PLAYER_ATTACK, ENEMY_ATTACK, ENEMY_DIED, PLAYER_DIED, PLAYER_COLLISION, CHANGE_PLAYER,
        EXIT_SPECIAL_SCREEN, HEALED, GOT_MONEY, QUIT, RESET, VIEW_STATS};

    Notification(){};
    Notification(Code n) : type(n) {};

    // Code for which animation is required
    Code type;

    // Related data
    int damage;
    bool hit;
    Character enemy;
    int healed;
    int value;
};

#endif //MINI_RPG_NOTIFICATION_H
