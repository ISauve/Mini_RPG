#include "Controller.h"
#include "../Model/Model.h"

// TODO: remove
#include <iostream>
using namespace std;

Controller::Controller( Model* m ) : model_(m) {};

void Controller::handleEvent(sf::Event& event) {
    using namespace std;

    switch (event.type) {
        case sf::Event::Closed:
            model_->endGame();
            break;

        case sf::Event::KeyPressed:
            handleKeyPress(event);
            break;

        case sf::Event::MouseButtonPressed:
            handleMouseClick(event);
            break;
        default: break;
    }
}


void Controller::handleKeyPress(sf::Event& e) {
    switch (e.key.code) {
        case sf::Keyboard::Escape:
            model_->endGame();
            break;
        case sf::Keyboard::Left:
            model_->movePlayer(-10, 0);
            break;
        case sf::Keyboard::Right:
            model_->movePlayer(10, 0);
            break;
        case sf::Keyboard::Up:
            model_->movePlayer(0, -10);
            break;
        case sf::Keyboard::Down:
            model_->movePlayer(0, 10);
            break;
        case sf::Keyboard::Space:
            model_->attack();
            break;
        default:
            break;
    }
}

void Controller::handleMouseClick(sf::Event& e) {
    int x = e.mouseButton.x;
    int y = e.mouseButton.y;

    // Check if this click was on any active buttons
    for (auto it : activeButtons_) {
        if ( it.second.contains(x, y) ) {
            switch (it.first) {
                case CHANGE_PLAYER:
                    model_->changePlayer();
                    break;
                case SELECT_PLAYER_1:
                    model_->player()->setCharacter(0, 1);
                    model_->regularScreenShow();
                    break;
                case SELECT_PLAYER_2:
                    model_->player()->setCharacter(0, 4);
                    model_->regularScreenShow();
                    break;
                case SELECT_PLAYER_3:
                    model_->player()->setCharacter(4, 1);
                    model_->regularScreenShow();
                    break;
                case SELECT_PLAYER_4:
                    model_->player()->setCharacter(4, 4);
                    model_->regularScreenShow();
                    break;
                case SELECT_PLAYER_5:
                    model_->player()->setCharacter(4, 7);
                    model_->regularScreenShow();
                    break;
            }
        }
    }
}
