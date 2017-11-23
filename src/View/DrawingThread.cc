#include "View.h"

void View::render() {
    window_->setActive(true);
    while (true) {
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

        std::this_thread::sleep_for(std::chrono::milliseconds(1000/frameRate_));

        if (gameOver_) {
            // sleep briefly to allow for quitting screen to display
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            break;
        }
    }
}


void View::handleUpdate(Notification event) {
    switch (event.type) {
        case QUIT_GAME:
            specialScreen_ = QUIT;
            gameOver_ = true;   // stops drawing thread
            break;
        case PLAYER_CHANGE:
            specialScreen_ = SELECT_PLAYER;
            break;
        case EXIT_SPECIAL_SCREEN:
            specialScreen_ = NONE;
            break;
        case PLAYER_DIED:
            // Blocking? special screen? What do we want to happen?  TODO
            break;
        case PLAYER_COLLISION:
        case PLAYER_ATTACK:
        case ENEMY_DIED:
            temporaryEvents_.emplace_back(std::make_pair (event, 25));
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
        if (it.isOnCharSheet()) {
            sf::IntRect playerImage = getPlayerImage(it);
            drawSprite(it.x(), it.y(), it.path(), playerImage);
        } else {
            drawSprite(it.x(), it.y(), it.path());
        }

        if (it.hasWeapon()) {   // Can only be true for characters
            drawSprite(it.x() - it.width() / 2, it.y(), it.weaponPath());
        }
    }

    // Draw any temporary events
    for (auto it = temporaryEvents_.begin(); it != temporaryEvents_.end(); it++){
        if (it->second == 0) {
            temporaryEvents_.erase(it++);
            continue;
        }

        drawEvent(it->first);
        it->second--;
    }
}