#include "View.h"

void View::drawPlayerSelection() {
    std::string sheet_path = "View/Textures/Character_set_2.png";
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
    controller_->addActiveButton(SELECT_PLAYER_1, rectangle_1);
    controller_->addActiveButton(SELECT_PLAYER_2, rectangle_2);
    controller_->addActiveButton(SELECT_PLAYER_3, rectangle_3);
    controller_->addActiveButton(SELECT_PLAYER_4, rectangle_4);
    controller_->addActiveButton(SELECT_PLAYER_5, rectangle_5);
}

void View::drawQuitScreen() {
    drawText(50, sf::Color::Cyan, "Quitting...", true, sf::Vector2f(SCREEN_WIDTH/2, SCREEN_HEIGHT/2));
};

void View::drawPlayerDied() {
    drawText(50, sf::Color::Red, "You are dead!", true, sf::Vector2f(SCREEN_WIDTH/2, SCREEN_HEIGHT/2));
};

void View::drawBackground() {
    // Draw the status bar
    sf::Text text = generateText(25, sf::Color::White, "Monsieur Moustache", false);
    text.setPosition(sf::Vector2f(SCREEN_WIDTH/2 - 100 - text.getLocalBounds().width, 100));
    window_->draw(text);

    text.setString( std::to_string(model_->player()->health() ) );
    text.setPosition(sf::Vector2f(SCREEN_WIDTH/2 - 100 - text.getLocalBounds().width, 150));
    window_->draw(text);

    drawText(25, sf::Color::White, "The Big Evil", false, sf::Vector2f(SCREEN_WIDTH/2 + 100, 100));

    sf::Vector2f position(SCREEN_WIDTH/2 + 100, 150);
    drawText(25, sf::Color::White, std::to_string(model_->enemy()->health()), false, position);

    // Draw buttons (for now, we only have the "change character" button - later we might have more)
    sf::Vector2f pos(175, SCREEN_HEIGHT - 75);
    sf::RectangleShape r = drawRectangle(250, 50, sf::Color(52, 152, 219), pos, 10, sf::Color(41, 128, 185));

    drawText(17, sf::Color::White, "Change character", true, sf::Vector2f(175, SCREEN_HEIGHT - 75));

    controller_->clearActiveButtons();
    controller_->addActiveButton(CHANGE_PLAYER, r.getGlobalBounds());
}


void View::drawEvent(Notification event) {
    switch (event.type) {
        case PLAYER_COLLISION:
            drawText(25, sf::Color::White, "OPE", false, sf::Vector2f(model_->player()->x()+50, model_->player()->y()-50));
            break;

        case PLAYER_ATTACK:
            drawPlayerAttack(event.hit, event.damage);
            break;

        case ENEMY_DIED:
            // probably want to change this to something better todo
            drawText(32, sf::Color::Blue, "Congrats, you win! Press r to restart", true, sf::Vector2f(SCREEN_WIDTH/2, SCREEN_HEIGHT/2 + 200));
            break;

        //case ENEMY_ATTACK:
        //   drawSprite(event.enemy->x() - event.enemy->width()/2, event.enemy->y(), "View/Textures/Enemy_Sword_1.png");
        //   drawText(25, sf::Color::Red, std::to_string(event.damage), false, sf::Vector2f(event.enemy->x() + 50, event.enemy->y() - 100));
        //   break;

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

sf::RectangleShape View::drawRectangle(int w, int h, sf::Color c1, sf::Vector2f pos) {
    sf::RectangleShape rectangle(sf::Vector2f(w, h));
    rectangle.setOrigin(w/2, h/2);
    rectangle.setPosition(pos);
    rectangle.setFillColor(c1);
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

// Determine which image to use from a char sheet based on direction char is moving
static int counter = 0;
sf::IntRect View::getPlayerImage(Sprite& s) {
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

void View::drawPlayerAttack(bool hit, int damage) {
    // Draw the sword
    float x = model_->player()->x() + model_->player()->width() / 2;
    drawSprite(x, model_->player()->y(), "View/Textures/Sword_1.png");

    // Draw the hit or miss
    sf::Vector2f position(model_->player()->x() + 50, model_->player()->y() - 100);
    if (hit) drawText(25, sf::Color::Red, std::to_string(damage), false, position);
    else drawText(25, sf::Color::White, "Miss", false, position);
}
