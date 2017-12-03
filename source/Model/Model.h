#ifndef MINI_RPG_MODEL_H
#define MINI_RPG_MODEL_H

#include "../Observer/Subject.h"
#include "Character.h"
#include "Item.h"
#include "../Channel.h"
#include "EventPackage.h"
#include "../Config.h"

#include <queue>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>

struct Attack {
    int damage;
    Character* enemy;

    Attack(int d, Character* e) : damage(d), enemy(e) {};
};

class Model : public Subject {
    std::mutex charsLock_;
    std::vector< Character > chars_;
    Character player_;

    std::vector< Item > items_;

    // One entry exists in this vector for each char in chars_
    // Each entry corresponds to the timeout before that character can attack again
    std::vector< int > charTimeouts_;
    int playerTimeout_;

    // Flag for when we need to ignore (certain) calls from the controller
    bool specialScreen_;

    // Channel & handler for receiving calls from the controller
    Channel<EventPackage>* eventsChannel_;
    void handleEvent(EventPackage);

    // Handles enemy attacks
    void checkActiveEnemies();

    // Helpers
    void movePlayer(int x, int y);
    void playerAttack();
    void resetState();

public:
    Model();
    ~Model() = default;

    // Modifier for the controller to pass the shared channel to the model
    void setChannel(Channel<EventPackage>*);

    // Main game loop for updating the state of the game
    void startGameLoop();

    // Accessors for the view - return copies, not references, so as to be thread-safe
    Character player();
    std::vector< Character > getChars();
    std::vector< Item > getItems();
};


#endif //MINI_RPG_MODEL_H
