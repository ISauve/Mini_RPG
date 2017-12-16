#ifndef MINI_RPG_CONTROLLER_H
#define MINI_RPG_CONTROLLER_H

#include "../Channel.h"
#include "../Model/EventPackage.h"

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <list>
#include <thread>
#include <mutex>

// Controller: mediates between model & view
//  -> translates UI events into call to model

class Model;
class Controller {
public:
    enum Button {
        QUICK_ACCESS_1, QUICK_ACCESS_2, QUICK_ACCESS_3, QUICK_ACCESS_4, QUICK_ACCESS_5, QUICK_ACCESS_6, QUICK_ACCESS_7,
        CHANGE_PLAYER, SELECT_PLAYER_1, SELECT_PLAYER_2, SELECT_PLAYER_3, SELECT_PLAYER_4, SELECT_PLAYER_5,
        VIEW_STATS, RESET, EXIT_SPECIAL_SCREEN
    };

private:
    sf::RenderWindow* window_;
    std::mutex buttonsLock_;
    std::list<std::pair<Button, sf::FloatRect>> activeButtonAreas_;
    bool gameOver_;

    // Thread-safe channel to send messages to the model
    Channel<EventPackage> modelChannel_;

    // Helper functions for interpreting events
    void checkPlayerMovement();
    void handleEvent(sf::Event&);
    void handleKeyPress(sf::Event&);
    void handleMouseClick(sf::Event&);

public:
    explicit Controller( Model* );
    ~Controller() = default;

    // Main game loop for handling events
    void handleEvents();

    // Modifiers for the view
    void setWindow(sf::RenderWindow* window);
    void clearActiveButtons();                      // thread safe
    void addActiveButton(Button, sf::FloatRect);    // thread safe

};

#endif //MINI_RPG_CONTROLLER_H
