#include "View.h"
#include "../Model/Model.h"

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

        // 30 fps
        std::this_thread::sleep_until(start + std::chrono::milliseconds(33));

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
            viewState_ = QUIT;
            gameOver_ = true;   // stops drawing thread
            break;
        case Notification::RESET:
            viewState_ = PLAYING;
            temporaryEvents_.clear();
            break;
        case Notification::CHANGE_PLAYER:
            viewState_ = SELECT_PLAYER;
            break;
        case Notification::EXIT_SPECIAL_SCREEN:
            viewState_ = PLAYING;
            break;
        case Notification::PLAYER_DIED:
            viewState_ = DEAD;
            break;
        case Notification::VIEW_STATS:
            viewState_ = VIEW_STATS;
            break;
        case Notification::ENEMY_DIED:
            temporaryEvents_.emplace_back(std::make_pair (event, 60));
            break;
        default:
            temporaryEvents_.emplace_back(std::make_pair (event, 15));
            break;
    }
}

void View::drawFrame() {
    switch (viewState_) {
        case SELECT_PLAYER:
            drawPlayerSelection();
            return;
        case QUIT:
            drawQuitScreen();
            return;
        case DEAD:
            drawPlayerDied();
            return;
        case VIEW_STATS:
            drawViewStats();
            return;
        case PLAYING:
        default:
            break;
    }

    drawBackground();

    // Grab all the sprites we have to draw
    std::vector< std::reference_wrapper<Sprite> > sprites;

    std::vector< Character > chars = model_->getChars();
    for (int i=0; i < int(chars.size()); i++) sprites.emplace_back(chars[i]);

    Character player = model_->player();
    sprites.emplace_back(player);

    std::vector< Prop > props = model_->getProps();
    for (int i=0; i < int(props.size()); i++) sprites.emplace_back(props[i]);

    // Order gets determined by y coordinates of the bottom of the sprite & by z-index
    std::sort (sprites.begin(), sprites.end(), [] (Sprite& a, Sprite& b) -> bool {
        if (a.zIndex() > b.zIndex()) return false;

        float a_bottom = a.y() + a.height()/2;
        float b_bottom = b.y() + b.height()/2;

        return a_bottom < b_bottom;
    });

    // Draw the sprites
    for (auto& it : sprites) {
        try {  // Attempt to downcast sprite to character
            auto character = dynamic_cast<Character&>(it.get());

            // The player gets drawn differently (image used depends on the keys being pressed)
            if (character.isPlayer()) {
                sf::IntRect playerImage = getPlayerImage(it);
                drawSprite(character.x(), character.y(), character.path(), playerImage);

                if (character.hasWeapon()) drawPlayerWeapon(character);
                continue;
            }

            // Only characters can have weapons
            if (character.hasWeapon()) {
                drawSprite(character.x() - character.width() / 2, character.y(), character.weaponPath());
            }
        } catch (std::bad_cast &e) {};

        auto sprite = it.get();
        if (sprite.isOnSheet()) {
            sf::IntRect selection = getSelection(it);
            drawSprite(sprite.x(), sprite.y(), sprite.path(), selection);
        } else {
            drawSprite(sprite.x(), sprite.y(), sprite.path());
        }
    }

    // Draw any temporary events
    // BUG: these shouldn't all necessarily be the highest (ie crossbones...)       todo - fix
    for (auto it = temporaryEvents_.begin(); it != temporaryEvents_.end(); it++){
        drawEvent(it->first);
        it->second--;
    }

    // Remove any "expired" temporary events
    temporaryEvents_.erase(std::remove_if(temporaryEvents_.begin(), temporaryEvents_.end(),
        [](std::pair<Notification, int> event) { return event.second <= 0; }), temporaryEvents_.end());
}