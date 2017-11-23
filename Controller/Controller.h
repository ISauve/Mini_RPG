#ifndef MINI_RPG_CONTROLLER_H
#define MINI_RPG_CONTROLLER_H

#include <SFML/Window.hpp>
#include <unordered_map>
#include <SFML/Graphics.hpp>

// Controller: mediates between model & view
//  -> translates UI events into call to model

class Model;

enum Button {CHANGE_PLAYER, SELECT_PLAYER_1, SELECT_PLAYER_2, SELECT_PLAYER_3, SELECT_PLAYER_4, SELECT_PLAYER_5};
class Controller {
    Model* model_;

    void handleKeyPress(sf::Event&);
    void handleMouseClick(sf::Event&);

public:
    Controller( Model* );
    //~Controller();

    std::unordered_map <Button, sf::FloatRect> activeButtons_;       // modified by the view
    void handleEvent(sf::Event&);
};

#endif //MINI_RPG_CONTROLLER_H
