#include "View.h"

void View::render() {
    window_->setActive(true);
    while (true) {
        auto start = std::chrono::high_resolution_clock::now();

        // Check for updates from the model
        // deal with multiple updates at once? or is 1/100ms fast enough?  TODO
        Notification update;
        bool updateOccurred = eventsChannel_.receive(update);
        if (updateOccurred) {
            handleUpdate(update);
        }

        // Draw the next frame
        window_->clear(sf::Color::Black);
        drawFrame();
        window_->display();

        // 10 fps
        std::this_thread::sleep_until(start + std::chrono::milliseconds(100));

        if (gameOver_) {
            // sleep briefly (1s) to allow for quitting screen to be seen
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            break;
        }
    }
}


void View::handleUpdate(Notification event) {
    switch (event.type) {
        case Notification::QUIT:
            specialScreen_ = QUIT;
            gameOver_ = true;   // stops drawing thread
            break;
        case Notification::RESET:
            specialScreen_ = NONE;
            temporaryEvents_.clear();
            break;
        case Notification::CHANGE_PLAYER:
            specialScreen_ = SELECT_PLAYER;
            break;
        case Notification::EXIT_SPECIAL_SCREEN:
            specialScreen_ = NONE;
            break;
        case Notification::PLAYER_DIED:
            specialScreen_ = DEAD;
            break;
        case Notification::PLAYER_COLLISION:
        case Notification::PLAYER_ATTACK:
        case Notification::ENEMY_ATTACK:
            temporaryEvents_.emplace_back(std::make_pair (event, 5));
            break;
        case Notification::ENEMY_DIED:
            temporaryEvents_.emplace_back(std::make_pair (event, 20));
            break;
        default:
            break;
    }
}

void View::drawFrame() {
    switch (specialScreen_) {
        case SELECT_PLAYER:
            drawPlayerSelection();
            return;
        case QUIT:
            drawQuitScreen();
            return;
        case DEAD:
            drawPlayerDied();
            return;
        case NONE:
        default:
            break;
    }

    drawBackground();

    // Grab all the sprites we have to draw
    std::vector< Sprite > sprites;
    std::vector< Character > chars = model_->getChars();
    for (auto it : chars) sprites.push_back(it);

    // Order gets determined by y coordinates of the bottom of the sprite & by z-index
    std::sort (sprites.begin(), sprites.end(), [] (Sprite& a, Sprite& b) -> bool {
        // idea: add z-index quality to sprites     todo
        //if (a.z() < b.z()) return true

        float a_bottom = a.y() + a.height()/2;
        float b_bottom = b.y() + b.height()/2;

        return a_bottom < b_bottom;
    });

    // Draw the sprites
    for (auto it : sprites) {
        if (it.isPlayer()) {
            sf::IntRect playerImage = getPlayerImage(it);
            drawSprite(it.x(), it.y(), it.path(), playerImage);
        } else if (it.isOnCharSheet()) {
            sf::IntRect playerImage = getCharImage(it);
            drawSprite(it.x(), it.y(), it.path(), playerImage);
        } else drawSprite(it.x(), it.y(), it.path());

        if (it.hasWeapon()) {   // Can only be true for characters
            drawSprite(it.x() - it.width() / 2, it.y(), it.weaponPath());
        }
    }

    // Draw any temporary events
    for (auto it = temporaryEvents_.begin(); it != temporaryEvents_.end(); it++){
        drawEvent(it->first);
        it->second--;
    }

    // Remove any "expired" temporary events
    temporaryEvents_.erase(std::remove_if(temporaryEvents_.begin(), temporaryEvents_.end(),
        [](std::pair<Notification, int> event) { return event.second <= 0; }), temporaryEvents_.end());
}

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
        if (counter < 5)
            player_img = sf::IntRect( (col-1)*width, row*height, width, height );
        else if (counter < 10 || (counter >= 15 && counter < 20))
            player_img = sf::IntRect( (col)*width, row*height, width, height );
        else
            player_img = sf::IntRect( (col+1)*width, row*height, width, height );

        counter++; if (counter == 20) counter = 0;
    }

    return player_img;
}

sf::IntRect View::getCharImage(Sprite& s) {
    int row = s.row();
    int col = s.col();
    int width = s.width();
    int height = s.height();

    return sf::IntRect(col*width, row*height, width, height);
}
