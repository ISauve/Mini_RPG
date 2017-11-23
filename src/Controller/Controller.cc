#include "Controller.h"
#include "../Model/Model.h"

// TODO: remove
#include <iostream>
using namespace std;

Controller::Controller( Model* m ) : model_(m), ignoreEvents_(false), gameOver_(false) {};

void Controller::setWindow(sf::RenderWindow* window) { window_ = window; }

void Controller::clearActiveButtons() {
    std::lock_guard<std::mutex> lock(buttonsLock_);
    activeButtons_.clear();
};

void Controller::addActiveButton(Button b, sf::FloatRect f) {
    std::lock_guard<std::mutex> lock(buttonsLock_);
    activeButtons_.push_back(std::make_pair(b, f));
}

void Controller::handleEvents() {
    while (!gameOver_) {
        // Pop any events that have occurred off the top of the event queue
        sf::Event windowEvent;
        while ( window_->pollEvent(windowEvent) ) { // returns false when queue is empty
            handleEvent(windowEvent);
        }

        // Alternative: (this is blocking, ideal for if we don't have to process anything
        // else in this loop)
        /*
        sf::Event windowEvent;
        if (window.waitEvent(windowEvent)) {
           handleEvent(windowEvent);
        }
         */

        /*
        // Poll for information from the attack thread
        // do in model??? this doesn't seem like the place for it...
        while ( !model_->enemyAttacks_.empty() ) {      // todo: need lockguard?
            Attack newAttack = model_->enemyAttacks_.front();
            model_->enemyAttacks_.pop();

            enemyAttack(newAttack.enemy, newAttack.damage);
        }

        // Check if player was killed
        if ( !model_->player()->isAlive() ) {

        }
         */

    }
}

// Handles events that come from the window
void Controller::handleEvent(sf::Event& event) {
    // Always handle "exit" events
    if (event.type == sf::Event::Closed ||
        (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) {
        gameOver_ = true;   // ends this loop
        model_->endGame();  // triggers the model to clean up, which notifies the view to close as well
    }

    if (ignoreEvents_) return;

    switch (event.type) {
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
            // ignore events for 1.5s?
            break;
        case sf::Keyboard::R:
            model_->resetState();
            break;
        default:
            break;
    }
}

void Controller::handleMouseClick(sf::Event& e) {
    std::lock_guard<std::mutex> lock(buttonsLock_);
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
