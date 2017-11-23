#ifndef MINI_RPG_VIEW_H
#define MINI_RPG_VIEW_H

#include "../Observer/Observer.h"
#include "../Model/Model.h"
#include "../Controller/Controller.h"
#include "Channel.h"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <thread>
#include <future>

#include <iostream>

// View: displays the game to the user
//	-> observes the mode
//  -> can query the model for information (ie character positions)

class Model;
class Controller;
class Character;

enum SpecialScreen {NONE, SELECT_PLAYER, QUIT};

class View : public Observer {
    Model* model_;
    Controller* controller_;

    sf::RenderWindow* window_;
    Channel<Notification> eventsChannel_;
    sf::Font font_;
    const int frameRate_ = 10; //fps

    bool gameOver_;
    SpecialScreen specialScreen_;
    std::list<std::pair<Notification, int>> temporaryEvents_;

    // Observer pattern: concrete callback for when subject (model) sends a notification
    void update(Notification) override;

    // Interprets notifications the view receives when the model sends an update
    void handleUpdate(Notification);

    // Draws the current state on the window
    void drawFrame();

    // Helper drawing functions
    Sprite getSprite(Notification);
    sf::RectangleShape drawRectangle(int, int, sf::Color, sf::Vector2f, int, sf::Color);
    sf::RectangleShape drawRectangle(int, int, sf::Color, sf::Vector2f);
    sf::Text generateText(int, sf::Color, std::string, bool);
    void drawText(int, sf::Color, std::string, bool, sf::Vector2f);
    sf::FloatRect drawSprite(float, float, std::string);
    sf::FloatRect drawSprite(float, float, std::string, sf::IntRect);
    sf::IntRect getPlayerImage(Sprite&);
    void drawStatusBar();
    void drawButtons();
    void drawPlayerSelection();
    void drawQuitScreen();
    void drawPlayerAttack(bool, int);
    void drawEnemyAttack(Character*, int);
    void drawPlayerCollision();
    void drawPlayerDied();

public:
    View(Model*, Controller*);
    ~View() = default;

    // Main game loop for rendering the window
    void render();
};


#endif //MINI_RPG_VIEW_H
