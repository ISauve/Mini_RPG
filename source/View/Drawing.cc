#include "View.h"

void View::drawPlayerSelection() {
    std::string sheet_path = "resources/Textures/Character_set_2.png";
    sf::IntRect selection = sf::IntRect(1*80, 0*80, 80, 80);
    sf::FloatRect rectangle_1 = drawSprite(100, SCREEN_HEIGHT/2, sheet_path, selection);

    selection = sf::IntRect(4*80, 0*80, 80, 80);
    sf::FloatRect rectangle_2 = drawSprite(300, SCREEN_HEIGHT/2, sheet_path, selection);

    selection = sf::IntRect(1*80, 4*80, 80, 80);
    sf::FloatRect rectangle_3 = drawSprite(500, SCREEN_HEIGHT/2, sheet_path, selection);

    selection = sf::IntRect(4*80, 4*80, 80, 80);
    sf::FloatRect rectangle_4 = drawSprite(700, SCREEN_HEIGHT/2, sheet_path, selection);

    selection = sf::IntRect(7*80, 4*80, 80, 80);
    sf::FloatRect rectangle_5 = drawSprite(900, SCREEN_HEIGHT/2, sheet_path, selection);

    controller_->clearActiveButtons();
    controller_->addActiveButton(Controller::SELECT_PLAYER_1, rectangle_1);
    controller_->addActiveButton(Controller::SELECT_PLAYER_2, rectangle_2);
    controller_->addActiveButton(Controller::SELECT_PLAYER_3, rectangle_3);
    controller_->addActiveButton(Controller::SELECT_PLAYER_4, rectangle_4);
    controller_->addActiveButton(Controller::SELECT_PLAYER_5, rectangle_5);
}

void View::drawQuitScreen() {
    drawText(50, sf::Color::Cyan, "Quitting...", true, sf::Vector2f(SCREEN_WIDTH/2, SCREEN_HEIGHT/2));
};

void View::drawPlayerDied() {
    drawText(35, sf::Color::Red, "You are dead!", true, sf::Vector2f(SCREEN_WIDTH/2, SCREEN_HEIGHT/2));

    sf::RectangleShape r = drawRectangle(100, 50, sf::Color(52, 152, 219), sf::Vector2f(100, SCREEN_HEIGHT - 75), 10, sf::Color(41, 128, 185));
    drawText(17, sf::Color::White, "Reset", true, sf::Vector2f(100, SCREEN_HEIGHT - 75));

    controller_->clearActiveButtons();
    controller_->addActiveButton(Controller::RESET, r.getGlobalBounds());
};

void View::drawBackground() {
    // Draw the status bar
    sf::Text text = generateText(25, sf::Color::White, "Monsieur Moustache: " + std::to_string(model_->player().health()), false);
    //text.setPosition(sf::Vector2f(SCREEN_WIDTH/2 - 100 - text.getLocalBounds().width, 100));
    text.setPosition(sf::Vector2f(100, 100));
    window_->draw(text);

    // Add buttons
    sf::RectangleShape r1 = drawRectangle(250, 50, sf::Color(52, 152, 219), sf::Vector2f(175, SCREEN_HEIGHT - 75), 10, sf::Color(41, 128, 185));
    drawText(17, sf::Color::White, "Change character", true, sf::Vector2f(175, SCREEN_HEIGHT - 75));

    sf::RectangleShape r2 = drawRectangle(100, 50, sf::Color(52, 152, 219), sf::Vector2f(415, SCREEN_HEIGHT - 75), 10, sf::Color(41, 128, 185));
    drawText(17, sf::Color::White, "Reset", true, sf::Vector2f(415, SCREEN_HEIGHT - 75));

    controller_->clearActiveButtons();
    controller_->addActiveButton(Controller::CHANGE_PLAYER, r1.getGlobalBounds());
    controller_->addActiveButton(Controller::RESET, r2.getGlobalBounds());
}


void View::drawEvent(Notification event) {
    switch (event.type) {
        case Notification::PLAYER_COLLISION:
            drawText(25, sf::Color::White, "OPE", false, sf::Vector2f(model_->player().x()+50, model_->player().y()-50));
            break;

        case Notification::PLAYER_ATTACK:
            // Draw the player's active sword
            drawSprite(model_->player().x() + model_->player().width() / 2, model_->player().y(), model_->player().activeWeaponPath());

            if (event.hit) {   // Draw a hit above the enemy
                drawText(25, sf::Color::Magenta, std::to_string(event.damage), false, sf::Vector2f(event.enemy.x(), event.enemy.y() - 100));
            }
            else {      // Draw a miss above the player's sword
                drawText(25, sf::Color::White, "Miss", false, sf::Vector2f(model_->player().x() + 50, model_->player().y() - 100));
            }
            break;

        case Notification::ENEMY_ATTACK:
            // Draw the weapon as active
            drawSprite(event.enemy.x() - event.enemy.width() / 2, event.enemy.y(), event.enemy.activeWeaponPath());

            // Draw the damage above the player's head
            drawText(25, sf::Color::Red, std::to_string(event.damage), false, sf::Vector2f(model_->player().x(), model_->player().y()-70));
            break;

        case Notification::ENEMY_DIED:
            // Draw a crossbones where the enemy was
            drawSprite(event.enemy.x(), event.enemy.y(), "resources/Textures/Skull_crossbones.png");
            break;

        case Notification::GOT_HEART:
            // Draw +50 above the player's head
            drawText(25, sf::Color::Green, "+50", false, sf::Vector2f(model_->player().x()-50, model_->player().y()-80));

        default:
            break;
    }
}


/*************************************************************************
                            Helper functions
 *************************************************************************/

sf::RectangleShape View::drawRectangle(int w, int h, sf::Color c1, sf::Vector2f pos, int th, sf::Color c2) {
    sf::RectangleShape rectangle(sf::Vector2f(w, h));
    rectangle.setOrigin(w/2, h/2);
    rectangle.setPosition(pos);
    rectangle.setFillColor(c1);
    rectangle.setOutlineThickness(th);
    rectangle.setOutlineColor(c2);
    window_->draw(rectangle);
    return rectangle;
}

sf::Text View::generateText(int size, sf::Color color, std::string text, bool resetOrigin) {
    sf::Text t;
    t.setFont(font_);
    t.setCharacterSize(size);
    t.setFillColor(color);
    t.setStyle(sf::Text::Bold);
    t.setString(text);
    if (resetOrigin) t.setOrigin(t.getLocalBounds().width/2, t.getLocalBounds().height/2);
    return t;
}

void View::drawText(int size, sf::Color color, std::string text, bool origin, sf::Vector2f pos) {
    sf::Text t = generateText(size, color, text, origin);
    t.setPosition(pos);
    window_->draw(t);
}

sf::FloatRect View::drawSprite(float x, float y, std::string path) {
    sf::Texture texture;
    texture.loadFromFile( path );

    sf::Sprite sprite;
    sprite.setTexture(texture);
    sprite.setPosition(sf::Vector2f(x, y));
    sprite.setOrigin(sprite.getLocalBounds().width/2, sprite.getLocalBounds().height/2);

    window_->draw(sprite);
    return sprite.getGlobalBounds();
}

// Overloaded version which takes only a section of an image as the texture
sf::FloatRect View::drawSprite(float x, float y, std::string path, sf::IntRect selection) {
    sf::Texture texture;
    texture.loadFromFile(path, selection);

    sf::Sprite sprite;
    sprite.setTexture(texture);
    sprite.setPosition(sf::Vector2f(x, y));
    sprite.setOrigin(sprite.getLocalBounds().width/2, sprite.getLocalBounds().height/2);

    window_->draw(sprite);
    return sprite.getGlobalBounds();
};