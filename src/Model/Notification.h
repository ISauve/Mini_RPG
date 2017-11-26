#ifndef MINI_RPG_NOTIFICATION_H
#define MINI_RPG_NOTIFICATION_H

#include "Character.h"

enum Code {PLAYER_ATTACK, ENEMY_DIED, ENEMY_ATTACK, PLAYER_DIED, PLAYER_COLLISION, PLAYER_CHANGE,
    EXIT_SPECIAL_SCREEN, QUIT_GAME, RESET};

struct Notification {
    Notification(){};
    Notification(Code n) : type(n) {};

    // Code for which animation is required
    Code type;

    // Related data
    int damage;
    bool hit;
    Character enemy;
};

#endif //MINI_RPG_NOTIFICATION_H
