#ifndef MINI_RPG_VIEW_H
#define MINI_RPG_VIEW_H

#include "../Observer/Observer.h"
#include "../Channel.h"
#include "../Model/Notification.h"

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <algorithm>
#include <thread>
#include <mutex>
#include <memory>
#include <string>
#include <list>
#include <dirent.h>

// View: displays the game to the user
//  -> observes the model
//  -> can query the model for information (ie character positions)

class Model;
class Controller;
class Character;

class View : public Observer {
    Model* model_;
    Controller* controller_;

    sf::RenderWindow* window_;
    Channel<Notification> eventsChannel_;
    sf::Font font_;

    // Images used (stored in memory for fast access)
    std::map<std::string, sf::Image> imageCache_;
    void loadImages();

    bool animateSplashScreen();

    bool gameOver_;
    enum ViewState {PLAYING, SELECT_PLAYER, QUIT, DEAD, VIEW_STATS};
    ViewState viewState_;
    std::list<std::pair<Notification, int>> temporaryEvents_;

    // Observer pattern: concrete callback for when subject (model) sends a notification
    void update(Notification) override;

    // Interprets notifications the view receives when the model sends an update
    void handleUpdate(Notification);

    // Draws the current state on the window
    void drawFrame();

    // Determines which image to use from a char sheet based on direction char is moving
    // (reads input from the keyboard)
    sf::IntRect getPlayerImage(Sprite&);

    // Grabs the correct image to use off a character sheet based on a sprite's row/col
    // Returns
    sf::IntRect getSelection(Sprite& s);

    // Determines which side to draw the weapon on base on direction char is moving
    // (reads input from the keyboard)
    void drawPlayerWeapon(Character&);
    void drawActivePlayerWeapon(Character&);

    // Drawing functions
    void drawEvent(Notification);
    void drawBackground();
    void drawPlayerSelection();
    void drawQuitScreen();
    void drawPlayerDied();
    void drawViewStats();

    // Helpers
    sf::RectangleShape drawRectangle(int, int, sf::Color, sf::Vector2f, int, sf::Color, bool = true);
    sf::Text generateText(int, sf::Color, std::string, bool);
    void drawText(int, sf::Color, std::string, bool, sf::Vector2f);
    sf::FloatRect drawSprite(float, float, std::string, sf::IntRect = sf::IntRect(), bool = true, float scale = 1);

public:
    View(Model*, Controller*);
    ~View() = default;

    // Main game loop for rendering the window
    void render();
};


#endif //MINI_RPG_VIEW_H
