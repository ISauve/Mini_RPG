#ifndef MINI_RPG_NOTIFICATION_H
#define MINI_RPG_NOTIFICATION_H

class Character;

enum Code {PLAYER_ATTACK, PLAYER_COLLISION, ENEMY_DIED, PLAYER_DIED, PLAYER_CHANGE,
    EXIT_SPECIAL_SCREEN, QUIT_GAME};

struct Notification {
    Notification(Code n) : type(n) {};

    // Code for which animation is required
    Code type;

    // Related data
    int damage;
    bool hit;
    Character* enemy;
};

#endif //MINI_RPG_NOTIFICATION_H
