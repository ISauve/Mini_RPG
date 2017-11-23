#ifndef MINI_RPG_ANIMATIONS_H
#define MINI_RPG_ANIMATIONS_H

#include "../Model/Model.h"
#include "../Controller/Controller.h"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

class Model;
class Controller;

// Class which handles all the drawing
class Animation {
    sf::RenderWindow* window_;
    Model* model_;
    Controller* controller_;

    sf::Font font;
public:
    Animation(sf::RenderWindow*, Model*, Controller*);

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

    void playerAttack(bool, int);
    void enemyAttack(Character*, int);
    void playerCollision();
    void playerDied();
    void quitGame();
};


#endif //MINI_RPG_ANIMATIONS_H
