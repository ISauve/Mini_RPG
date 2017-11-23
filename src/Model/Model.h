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
    std::mutex charsLock_;
    std::vector< Character > chars_;

    //void enemyAttack();
    //std::queue< Attack > enemyAttacks_;     // Public queue of messages for the view
    //std::thread attackThread_;

public:
    Model();
    ~Model() = default;

    // Modifiers for the controller to call
    void resetState();
    void endGame();
    void changePlayer() { notify(PLAYER_CHANGE); };
    void regularScreenShow() { notify(EXIT_SPECIAL_SCREEN); };
    void attack();
    void movePlayer(int, int);

    // Accessors for the view (all thread-safe)
    Character* player();
    Character* enemy();
    std::vector< Character > getChars();     // Doesn't return a reference
};


#endif //MINI_RPG_MODEL_H
