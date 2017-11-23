#ifndef MINI_RPG_VIEW_H
#define MINI_RPG_VIEW_H

#include "../Observer.h"
#include "Animation.h"
#include <algorithm>

// View: how the game interacts with the user
//	-> observes the model
//  -> sends messages to controller

class Model;
class Controller;
class Character;

enum SpecialScreen {SELECT_PLAYER, QUIT};

class View : public Observer {
    Model* model_;
    Controller* controller_;
    sf::RenderWindow* window_;

    bool specialScreenShow_;
    SpecialScreen specialScreen_;
    std::vector< Sprite > buttons_;

    virtual void update(Notification) override;
    void runControlLoop();
    void draw();

public:
    View(Model*, Controller*);
};


#endif //MINI_RPG_VIEW_H
