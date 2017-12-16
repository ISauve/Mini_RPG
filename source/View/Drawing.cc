#include "View.h"
#include "../Controller/Controller.h"
#include "../Model/Model.h"
#include "../ConfigReader.h"

void View::drawPlayerSelection() {
    std::string sheet_path = "resources/Textures/Character_set_2.png";
    drawText(30, sf::Color::White, "Select your character", false, sf::Vector2f(50, 50));

    sf::IntRect selection = sf::IntRect(1*80, 0*80, 80, 80);
    sf::FloatRect rectangle_1 = drawSprite(100, 200, sheet_path, selection);
    drawText(25, sf::Color::White, "Business-y Bob: Not sure how he ended up here. Is late for a meeting.",
             false, sf::Vector2f(175, 200));

    selection = sf::IntRect(4*80, 0*80, 80, 80);
    sf::FloatRect rectangle_2 = drawSprite(100, 400, sheet_path, selection);
    drawText(25, sf::Color::White, "Beeb (generic): Very cuddly & loves back massages.",
             false, sf::Vector2f(175, 400));

    selection = sf::IntRect(1*80, 4*80, 80, 80);
    sf::FloatRect rectangle_3 = drawSprite(100, 600, sheet_path, selection);
    drawText(25, sf::Color::White, "Monsieur Moustache: A classic hero, this great lad is prepared for all sorts of adventure.",
             false, sf::Vector2f(175, 600));

    selection = sf::IntRect(4*80, 4*80, 80, 80);
    sf::FloatRect rectangle_4 = drawSprite(100, 800, sheet_path, selection);
    drawText(25, sf::Color::White, "Jean-Pierre-Marie-Cloud: Has never been seen without his beret, and scoffs at anyone \nwho says 'Chocolatine'. It's Pain au Chocolate... putain.",
             false, sf::Vector2f(175, 775));

    selection = sf::IntRect(7*80, 4*80, 80, 80);
    sf::FloatRect rectangle_5 = drawSprite(100, 1000, sheet_path, selection);
    drawText(25, sf::Color::White, "Gerard: A black belt, but a pacifist at heart. One time he almost got in a fight, when someone \nsaid his kitten wasn't THAT cute.",
             false, sf::Vector2f(175, 975));

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

void View::drawViewStats() {
    // Draw the player's image
    Sprite player = model_->player();
    sf::IntRect playerImage = getCharImage(player);
    sf::Texture texture;
    texture.loadFromImage(imageCache_[model_->player().path()], playerImage);
    sf::Sprite sprite;
    sprite.setTexture(texture);
    sprite.setPosition(sf::Vector2f(SCREEN_WIDTH/2 + 100, 200));
    sprite.setScale(3, 3);
    window_->draw(sprite);
    sf::FloatRect bounds = sprite.getGlobalBounds();

    // Add stats
    drawText(25, sf::Color::White, "STATS", true,
             sf::Vector2f(SCREEN_WIDTH/2 + 100 + bounds.width/2, bounds.top +  bounds.height + 100));
    drawText(25, sf::Color::White, "Strength: " + std::to_string(model_->player().strength()), true,
             sf::Vector2f(SCREEN_WIDTH/2 + 100 + bounds.width/2, bounds.top +  bounds.height + 150));
    drawText(25, sf::Color::White, "Speed: " + std::to_string(model_->player().speed()), true,
             sf::Vector2f(SCREEN_WIDTH/2 + 100 + bounds.width/2, bounds.top + bounds.height + 200));

    // Draw the player's weapon, blown up 3x
    if (model_->player().hasWeapon()) {
        sf::Texture texture;
        texture.loadFromImage(imageCache_[model_->player().weaponPath()]);
        sf::Sprite sprite;
        sprite.setTexture(texture);
        sprite.setScale(3, 3);
        sprite.setPosition(sf::Vector2f(SCREEN_WIDTH/2 - 100 - sprite.getGlobalBounds().width, 200));
        window_->draw(sprite);
        sf::FloatRect bounds = sprite.getGlobalBounds();

        // Add stats
        drawText(25, sf::Color::White, "STATS", true,
                 sf::Vector2f(SCREEN_WIDTH/2 - 100 - bounds.width/2, bounds.top +  bounds.height + 100));
        drawText(25, sf::Color::White, "Strength: " + std::to_string(model_->player().weaponStrength()), true,
                 sf::Vector2f(SCREEN_WIDTH/2 - 100 - bounds.width/2, bounds.top +  bounds.height + 150));
        drawText(25, sf::Color::White, "Weight: " + std::to_string(model_->player().weaponWeight()), true,
                 sf::Vector2f(SCREEN_WIDTH/2 - 100 - bounds.width/2, bounds.top + bounds.height + 200));
    } else {
        drawText(25, sf::Color::White, "No weapon equipped", true, sf::Vector2f(SCREEN_WIDTH/2 - 200, SCREEN_HEIGHT/2));
    }

    // Draw the bag contents
    // TODO

    // Draw "return" button
    sf::FloatRect arrow = drawSprite(100, SCREEN_HEIGHT - 75, "resources/Textures/return.png");
    controller_->clearActiveButtons();
    controller_->addActiveButton(Controller::EXIT_SPECIAL_SCREEN, arrow);
}

sf::IntRect cropImage(int percentage, int width, int height) {
    return sf::IntRect(0, 0, width * percentage / 100, height);
}

#include <iostream>

void View::drawBackground() {
    controller_->clearActiveButtons();

    // Health
    auto empty_bar = drawSprite(50, 50, "resources/Textures/empty_bar.png", sf::IntRect(), false);
    drawSprite(50, 50, "resources/Textures/green_bar.png", cropImage(model_->player().health(), 425, 57), false);

    // Money
    drawSprite(50 + empty_bar.width + 50, 30, "resources/Textures/coin_pile.png", sf::IntRect(), false);
    sf::Text money = generateText(25, sf::Color::White, std::to_string(model_->playerMoney()), false);
    money.setPosition(sf::Vector2f(50 + empty_bar.width + 125, 60));
    window_->draw(money);

    // Backpack button
    sf::FloatRect backpack = drawSprite(50 + empty_bar.width + 240, 75, "resources/Textures/pack.png");
    controller_->addActiveButton(Controller::VIEW_STATS, backpack);

    // Quick-access items
    sf::FloatRect slots = drawSprite(backpack.left + backpack.width + 50, 30, "resources/Textures/slots.png", sf::IntRect(), false);
    std::vector<Prop> quickAccessItems = model_->player().quickAccessContents();
    for (int i=0; i < int(quickAccessItems.size()); i++) {
        auto it = quickAccessItems[i];
        if (it.isOnSheet()) {
            sf::IntRect playerImage = getCharImage(it);
            sf::FloatRect item = drawSprite(it.x(), it.y(), it.path(), playerImage);
            controller_->addActiveButton(static_cast<Controller::Button>(i), item);
        } else {
            sf::FloatRect item =  drawSprite(it.x(), it.y(), it.path());
            controller_->addActiveButton(static_cast<Controller::Button>(i), item);
        }
    }

    // Change character
    sf::RectangleShape change = drawRectangle(255, 50, sf::Color(52, 152, 219),
                                          sf::Vector2f(slots.left + slots.width + 50, 50), 10,
                                          sf::Color(41, 128, 185), false);
    drawText(17, sf::Color::White, "Change character", false,
             sf::Vector2f(slots.left + slots.width + 60, 65));
    controller_->addActiveButton(Controller::CHANGE_PLAYER, change.getGlobalBounds());

    // Reset
    sf::RectangleShape reset = drawRectangle(100, 50, sf::Color(52, 152, 219),
                                          sf::Vector2f(change.getGlobalBounds().left + change.getGlobalBounds().width + 50, 50), 10,
                                          sf::Color(41, 128, 185), false);
    drawText(17, sf::Color::White, "Reset", false,
             sf::Vector2f(change.getGlobalBounds().left + change.getGlobalBounds().width + 60, 65));
    controller_->addActiveButton(Controller::RESET, reset.getGlobalBounds());
}


void View::drawEvent(Notification event) {
    switch (event.type) {
        case Notification::PLAYER_COLLISION:
            drawText(25, sf::Color::White, "OPE", false, sf::Vector2f(model_->player().x()+50, model_->player().y()-50));
            break;

        case Notification::PLAYER_ATTACK:
            if (model_->player().hasWeapon()){
                auto player = model_->player();
                drawActivePlayerWeapon(player);
            }

            if (event.hit) {   // Draw a hit above the enemy
                drawText(25, sf::Color::Magenta, std::to_string(event.damage), false, sf::Vector2f(event.enemy.x(), event.enemy.y() - 100));
            }
            else {      // Draw a miss above the player's sword
                drawText(25, sf::Color::White, "Miss", false, sf::Vector2f(model_->player().x() + 50, model_->player().y() - 100));
            }
            break;

        case Notification::ENEMY_ATTACK:
            // Draw the weapon as active
            if (event.enemy.hasWeapon()) {
                drawSprite(event.enemy.x() - event.enemy.width() / 2, event.enemy.y(), event.enemy.activeWeaponPath());
            }

            // Draw the damage above the player's head
            drawText(25, sf::Color::Red, std::to_string(event.damage), false, sf::Vector2f(model_->player().x(), model_->player().y()-70));
            break;

        case Notification::ENEMY_DIED:
            // Draw a crossbones where the enemy was
            drawSprite(event.enemy.x(), event.enemy.y(), "resources/Textures/Skull_crossbones.png");
            break;

        case Notification::HEALED:
            // Draw the amount healed above the player's head
            drawText(25, sf::Color::Green, "+" + std::to_string(event.healed), false, sf::Vector2f(model_->player().x()-50, model_->player().y()-80));
            break;

        case Notification::GOT_MONEY:
            // Draw the amount acquired above the player's head
            drawText(25, sf::Color::Yellow, "+" + std::to_string(event.value), false, sf::Vector2f(model_->player().x()+50, model_->player().y()-80));
            break;

        default:
            break;
    }
}


/*************************************************************************
                            Helper functions
 *************************************************************************/

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

sf::FloatRect View::drawSprite(float x, float y, std::string path, sf::IntRect selection, bool center_origin) {
    sf::Texture texture;
    texture.loadFromImage(imageCache_[path], selection);

    sf::Sprite sprite;
    sprite.setTexture(texture);
    sprite.setPosition(sf::Vector2f(x, y));
    if (center_origin) sprite.setOrigin(sprite.getLocalBounds().width/2, sprite.getLocalBounds().height/2);

    window_->draw(sprite);
    return sprite.getGlobalBounds();
};