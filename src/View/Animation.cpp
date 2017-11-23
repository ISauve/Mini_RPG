#include "Animation.h"

#include <iostream>

Animation::Animation(sf::RenderWindow* w, Model* m, Controller* c) : window_(w), model_(m), controller_(c) {
    font.loadFromFile("View/Old_School_Adventures.ttf");
};

/***************************** Helper functions *****************************/

sf::RectangleShape Animation::drawRectangle(int w, int h, sf::Color c1, sf::Vector2f pos, int th, sf::Color c2) {
    sf::RectangleShape rectangle(sf::Vector2f(w, h));
    rectangle.setOrigin(w/2, h/2);
    rectangle.setPosition(pos);
    rectangle.setFillColor(c1);
    rectangle.setOutlineThickness(th);
    rectangle.setOutlineColor(c2);
    window_->draw(rectangle);
    return rectangle;
}

sf::RectangleShape Animation::drawRectangle(int w, int h, sf::Color c1, sf::Vector2f pos) {
    sf::RectangleShape rectangle(sf::Vector2f(w, h));
    rectangle.setOrigin(w/2, h/2);
    rectangle.setPosition(pos);
    rectangle.setFillColor(c1);
    window_->draw(rectangle);
    return rectangle;
}

sf::Text Animation::generateText(int size, sf::Color color, std::string text, bool resetOrigin) {
    sf::Text t;
    t.setFont(font);
    t.setCharacterSize(size);
    t.setFillColor(color);
    t.setStyle(sf::Text::Bold);
    t.setString(text);
    if (resetOrigin) t.setOrigin(t.getLocalBounds().width/2, t.getLocalBounds().height/2);
    return t;
}

void Animation::drawText(int size, sf::Color color, std::string text, bool origin, sf::Vector2f pos) {
    sf::Text t = generateText(size, color, text, origin);
    t.setPosition(pos);
    window_->draw(t);
}

sf::FloatRect Animation::drawSprite(float x, float y, std::string path) {
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
sf::FloatRect Animation::drawSprite(float x, float y, std::string path, sf::IntRect selection) {
    sf::Texture texture;
    texture.loadFromFile(path, selection);

    sf::Sprite sprite;
    sprite.setTexture(texture);
    sprite.setPosition(sf::Vector2f(x, y));
    sprite.setOrigin(sprite.getLocalBounds().width/2, sprite.getLocalBounds().height/2);

    window_->draw(sprite);
    return sprite.getGlobalBounds();
};

// Determine which image to use from a char sheet based on direction char is moving
static int counter = 0;
sf::IntRect Animation::getPlayerImage(Sprite& s) {
    int row = s.row();
    int col = s.col();
    int width = s.width();
    int height = s.height();

    // Select the rectangle for the "default" stance
    sf::IntRect player_img = sf::IntRect(col*width, row*height, width, height);

    // Animate player movement by changing the image if a key is pressed
    bool animate = false;
    if ( sf::Keyboard::isKeyPressed(sf::Keyboard::Left) ) {
        row = row+1; animate = true;
    } else if ( sf::Keyboard::isKeyPressed(sf::Keyboard::Right) ) {
        row = row+2; animate = true;
    } else if ( sf::Keyboard::isKeyPressed(sf::Keyboard::Up) ) {
        row = row+3; animate = true;
    } else if ( sf::Keyboard::isKeyPressed(sf::Keyboard::Down) ) {
        animate = true; }

    if (animate) {
        if (counter < 10)
            player_img = sf::IntRect( (col-1)*width, row*height, width, height );
        else if (counter < 20 || (counter >= 30 && counter < 40))
            player_img = sf::IntRect( (col)*width, row*height, width, height );
        else
            player_img = sf::IntRect( (col+1)*width, row*height, width, height );

        counter++; if (counter == 40) counter = 0;
    }

    return player_img;
}


/***************************** Background *****************************/

void Animation::drawStatusBar() {
    sf::Text text = generateText(25, sf::Color::White, "Monsieur Moustache", false);
    text.setPosition(sf::Vector2f(SCREEN_WIDTH/2 - 100 - text.getLocalBounds().width, 100));
    window_->draw(text);

    text.setString( std::to_string(model_->player()->health() ) );
    text.setPosition(sf::Vector2f(SCREEN_WIDTH/2 - 100 - text.getLocalBounds().width, 150));
    window_->draw(text);

    drawText(25, sf::Color::White, "The Big Evil", false, sf::Vector2f(SCREEN_WIDTH/2 + 100, 100));

    sf::Vector2f position(SCREEN_WIDTH/2 + 100, 150);
    drawText(25, sf::Color::White, std::to_string(model_->enemy()->health()), false, position);
}

void Animation::drawButtons() {
    sf::Vector2f pos(175, SCREEN_HEIGHT - 75);
    sf::RectangleShape r = drawRectangle(250, 50, sf::Color(52, 152, 219), pos, 10, sf::Color(41, 128, 185));

    drawText(17, sf::Color::White, "Change character", true, sf::Vector2f(175, SCREEN_HEIGHT - 75));

    controller_->activeButtons_.clear();
    controller_->activeButtons_[CHANGE_PLAYER] = r.getGlobalBounds();
}

/***************************** Special screens *****************************/

void Animation::drawPlayerSelection() {
    controller_->activeButtons_.clear();

    std::string sheet_path = "View/Textures/Character_set_2.png";
    sf::IntRect selection = sf::IntRect(1*80, 0*80, 80, 80);
    sf::FloatRect rectangle_1 = drawSprite(100, SCREEN_HEIGHT/2, sheet_path, selection);
    controller_->activeButtons_[SELECT_PLAYER_1] = rectangle_1;

    selection = sf::IntRect(4*80, 0*80, 80, 80);
    sf::FloatRect rectangle_2 = drawSprite(300, SCREEN_HEIGHT/2, sheet_path, selection);
    controller_->activeButtons_[SELECT_PLAYER_2] = rectangle_2;

    selection = sf::IntRect(1*80, 4*80, 80, 80);
    sf::FloatRect rectangle_3 = drawSprite(500, SCREEN_HEIGHT/2, sheet_path, selection);
    controller_->activeButtons_[SELECT_PLAYER_3] = rectangle_3;

    selection = sf::IntRect(4*80, 4*80, 80, 80);
    sf::FloatRect rectangle_4 = drawSprite(700, SCREEN_HEIGHT/2, sheet_path, selection);
    controller_->activeButtons_[SELECT_PLAYER_4] = rectangle_4;

    selection = sf::IntRect(7*80, 4*80, 80, 80);
    sf::FloatRect rectangle_5 = drawSprite(900, SCREEN_HEIGHT/2, sheet_path, selection);
    controller_->activeButtons_[SELECT_PLAYER_5] = rectangle_5;
}


/***************************** Events *****************************/

void Animation::playerAttack(bool hit, int damage) {
    // Draw the sword
    float x = model_->player()->x() + model_->player()->width()/2;
    drawSprite(x, model_->player()->y(), "View/Textures/Sword_1.png");

    // Draw the hit or miss
    sf::Vector2f position(model_->player()->x() + 50, model_->player()->y() - 100);
    if (hit)    drawText(25, sf::Color::Red, std::to_string(damage), false,  position);
    else        drawText(25, sf::Color::White, "Miss", false, position);

    // Pause for 0.5s to view
    window_->display();
    sf::sleep(sf::seconds(0.5f));
}

void Animation::enemyAttack(Character* c, int d) {
    drawSprite(c->x() - c->width()/2, c->y(), "View/Textures/Enemy_Sword_1.png");
    drawText(25, sf::Color::Red, std::to_string(d), false, sf::Vector2f(c->x() + 50, c->y() - 100));

    window_->display();
    sf::sleep(sf::seconds(0.5f));
}

void Animation::playerCollision() {
    drawText(25, sf::Color::White, "OPE", false, sf::Vector2f(model_->player()->x()+50, model_->player()->y()-50));

    window_->display();
    sf::sleep(sf::seconds(0.5f));
}

void Animation::playerDied() {
    drawText(50, sf::Color::Red, "You are dead!", true, sf::Vector2f(SCREEN_WIDTH/2, SCREEN_HEIGHT/2));

    window_->display();
    sf::sleep(sf::seconds(1.f));
};

void Animation::quitGame() {
    drawText(50, sf::Color::Cyan, "Quitting...", true, sf::Vector2f(SCREEN_WIDTH/2, SCREEN_HEIGHT/2));
    window_->display();     // Don't need to pause bc draw() never gets called again
};