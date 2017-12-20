#include "View.h"
#include "../Model/Model.h"
#include "../ConfigReader.h"
#include <iostream>

sf::RectangleShape View::drawRectangle(int w, int h, sf::Color c1, sf::Vector2f pos, int th, sf::Color c2, bool center) {
    sf::RectangleShape rectangle(sf::Vector2f(w, h));
    if (center) rectangle.setOrigin(w/2, h/2);
    rectangle.setPosition(pos);
    rectangle.setFillColor(c1);
    rectangle.setOutlineThickness(th);
    rectangle.setOutlineColor(c2);
    window_->draw(rectangle);
    return rectangle;
}

sf::Text View::generateText(int size, sf::Color color, std::string text, bool centerOrigin) {
    sf::Text t;
    t.setFont(font_);
    t.setCharacterSize(size);
    t.setFillColor(color);
    t.setStyle(sf::Text::Bold);
    t.setString(text);
    if (centerOrigin) t.setOrigin(t.getLocalBounds().width/2, t.getLocalBounds().height/2);
    return t;
}

void View::drawText(int size, sf::Color color, std::string text, bool center_origin, sf::Vector2f pos) {
    sf::Text t = generateText(size, color, text, center_origin);
    t.setPosition(pos);
    window_->draw(t);
}

sf::FloatRect View::drawSprite(float x, float y, std::string path, sf::IntRect selection, bool center_origin, float scale) {
    sf::Texture texture;
    if (!texture.loadFromImage(imageCache_[path], selection)) {
        std::cerr << "Couldn't draw image for sprite. Image path: " << path << std::endl;
    };

    sf::Sprite sprite;
    sprite.setTexture(texture);
    sprite.setPosition(sf::Vector2f(x, y));
    sprite.setScale(scale, scale);
    if (center_origin) sprite.setOrigin(sprite.getLocalBounds().width/2, sprite.getLocalBounds().height/2);

    window_->draw(sprite);
    return sprite.getGlobalBounds();
};