#ifndef MINI_RPG_MODEL_H
#define MINI_RPG_MODEL_H

#include "../Observer/Subject.h"
#include "Character.h"
#include <queue>
#include <vector>
#include <thread>
#include <mutex>

// TODO: keep this somewhere better (also, make screen proportions not modifyable)
#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 1024
#define PLAYER_WIDTH 80
#define PLAYER_HEIGHT 80

struct Attack {
    int damage;
    Character* enemy;

    Attack(int d, Character* e) : damage(d), enemy(e) {};
};

class Model : public Subject {
    std::vector< Character > chars_;
    bool gameOver_;
    void enemyAttack();

public:
    Model();

    std::queue< Attack > enemyAttacks_;     // Public queue of messages for the view
    std::thread attackThread_;

    // Functions for the controller
    void endGame();
    void attack();
    void movePlayer(int, int);
    void changePlayer() { notify(PLAYER_CHANGE); };
    void regularScreenShow() { notify(EXIT_SPECIAL_SCREEN); };
    void reset();

    // Accessors for the view TODO: make thread-safe
    bool gameOver() { return gameOver_; };
    Character* player() { return &chars_[0]; };
    Character* enemy() { return &chars_[1]; };
    std::vector< Character > getChars() { return chars_; };     // Doesn't return a reference
};


#endif //MINI_RPG_MODEL_H
