#include "Controller.h"

#include "../Model/Model.h"

Controller::Controller( Model* m ) : gameOver_(false) {
    m->setChannel(&modelChannel_);
};

void Controller::setWindow(sf::RenderWindow* window) {
    window_ = window;
}

void Controller::clearActiveButtons() {
    std::lock_guard<std::mutex> lock(buttonsLock_);
    activeButtonAreas_.clear();
};

void Controller::addActiveButton(Button b, sf::FloatRect f) {
    std::lock_guard<std::mutex> lock(buttonsLock_);
    activeButtonAreas_.emplace_back(std::make_pair(b, f));
}


void Controller::handleEvents() {
    while (!gameOver_) {
        auto start = std::chrono::high_resolution_clock::now();

        // Check if the user is pressing a "movement" key
        checkPlayerMovement();

        // Pop any events that have occurred off the top of the event queue
        sf::Event windowEvent;
        while ( window_->pollEvent(windowEvent) ) { // returns false when queue is empty
            handleEvent(windowEvent);
        }

        // This loop should run exactly once every 100ms (to get a consistent movement speed)
        std::this_thread::sleep_until(start + std::chrono::milliseconds(100));
    }
}


void Controller::checkPlayerMovement() {
    int x = 0;
    int y = 0;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) x -= 10;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) x += 10;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) y -= 10;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) y += 10;

    if (x != 0 || y != 0) modelChannel_.send( EventPackage(EventPackage::MOVE_PLAYER, x, y) );
}


void Controller::handleEvent(sf::Event& event) {
    // Handle "exit" events first
    if (event.type == sf::Event::Closed ||
        (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) {
        gameOver_ = true;   // ends this loop
        modelChannel_.send(EventPackage(EventPackage::QUIT)); // ends the model's game loop -> view gets notified to close too
        return;
    }

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
        case sf::Keyboard::Space:
            modelChannel_.send(EventPackage(EventPackage::PLAYER_ATTACK));
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
    for (auto it : activeButtonAreas_) {
        if ( it.second.contains(x, y) ) {
            switch (it.first) {
                case CHANGE_PLAYER:
                    modelChannel_.send(EventPackage(EventPackage::CHANGE_PLAYER));
                    break;
                case SELECT_PLAYER_1:
                    modelChannel_.send(EventPackage(EventPackage::SELECT_PLAYER, 0, 1));
                    break;
                case SELECT_PLAYER_2:
                    modelChannel_.send(EventPackage(EventPackage::SELECT_PLAYER, 0, 4));
                    break;
                case SELECT_PLAYER_3:
                    modelChannel_.send(EventPackage(EventPackage::SELECT_PLAYER, 4, 1));
                    break;
                case SELECT_PLAYER_4:
                    modelChannel_.send(EventPackage(EventPackage::SELECT_PLAYER, 4, 4));
                    break;
                case SELECT_PLAYER_5:
                    modelChannel_.send(EventPackage(EventPackage::SELECT_PLAYER, 4, 7));
                    break;
                case VIEW_STATS:
                    modelChannel_.send(EventPackage(EventPackage::VIEW_STATS));
                    break;
                case EXIT_SPECIAL_SCREEN:
                    modelChannel_.send(EventPackage(EventPackage::EXIT_SPECIAL_SCREEN));
                    break;
                case RESET:
                    modelChannel_.send(EventPackage(EventPackage::RESET));
                    break;
            }
        }
    }
}
