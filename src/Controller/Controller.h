#ifndef MINI_RPG_CONTROLLER_H
#define MINI_RPG_CONTROLLER_H

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <unordered_map>

// Controller: mediates between model & view
//  -> translates UI events into call to model

class Model;

enum Button {CHANGE_PLAYER, SELECT_PLAYER_1, SELECT_PLAYER_2, SELECT_PLAYER_3, SELECT_PLAYER_4, SELECT_PLAYER_5};
class Controller {
    Model* model_;

    sf::RenderWindow* window_;
    std::unordered_map <Button, sf::FloatRect> activeButtons_;
    bool ignoreEvents_;
    bool gameOver_;

    // Helper functions for interpreting events
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
