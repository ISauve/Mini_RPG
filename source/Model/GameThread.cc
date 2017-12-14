#include "Model.h"
#include "../ConfigReader.h"
#include "Tools/Tool.h"
#include "Tools/Weapon.h"
#include <cmath>

void Model::startGameLoop() {
    while(true) {
        auto start = std::chrono::high_resolution_clock::now();

        // Check for calls coming from the controller
        EventPackage pkg;
        bool eventOccurred = eventsChannel_->receive(pkg);
        if (eventOccurred) {
            if (pkg.type == EventPackage::QUIT) {
                notify(Notification::QUIT);
                // probably want to save game data to some external file        todo
                return;
            }

            handleEvent(pkg);
        }

        // Decrease any timeouts we have counting
        for (auto& it : charTimeouts_) if (it > 0) it--;
        if (playerTimeout_ > 0) playerTimeout_--;

        // Check if player is in the range of any active enemies
        // Resets the game if the player dies
        checkActiveEnemies();

        // Add a delay so that this loop runs at a consistent interval
        // (not too slow though because we can receive many events from the controller
        // in a short times span)
        std::this_thread::sleep_until(start + std::chrono::milliseconds(50));
    }
}

void Model::handleEvent(EventPackage e) {
    std::lock_guard<std::mutex> lock(charsLock_);       // most of these functions modify chars_
    switch (e.type) {
        case EventPackage::MOVE_PLAYER:
            movePlayer(e.x, e.y);
            break;

        case EventPackage::PLAYER_ATTACK:
            playerAttack();
            break;

        case EventPackage::RESET:
            resetState();
            break;

        case EventPackage::CHANGE_PLAYER:
            notify(Notification::CHANGE_PLAYER);
            specialScreen_ = true;
            return;

        case EventPackage::VIEW_STATS:
            notify(Notification::VIEW_STATS);
            specialScreen_ = true;
            return;

        case EventPackage::SELECT_PLAYER:
            // Set the new character
            player_.setCharacter(e.row, e.col);
            // Notify the view that player selection is complete
            notify(Notification::EXIT_SPECIAL_SCREEN);
            specialScreen_ = false;
            break;

        case EventPackage::EXIT_SPECIAL_SCREEN:
            notify(Notification::EXIT_SPECIAL_SCREEN);
            specialScreen_ = false;
            return;

        default:
            break;
    }
}

// Always called from within a function with a charsLock_
void Model::resetState() {
    // Set the state
    chars_.clear();
    charTimeouts_.clear();
    specialScreen_ = false;
    playerMoney_ = 0;
    notify(Notification::RESET);

    // Read in default configuration
    ConfigReader conf;
    conf.readConfig("resources/Areas/start.yaml");

    // Fetch the characters from the config
    chars_ = conf.getChars();
    for (auto it : chars_) charTimeouts_.push_back(0);
    player_ = conf.getPlayer();
    playerTimeout_ = 0;

    // Fetch other data from the config
    props_ = conf.getProps();
}

// Always called from within a function with a charsLock_
void Model::movePlayer(int x, int y) {
    if (specialScreen_) return;

    player_.move(x, y);

    // Check for player collision
    for (auto character : chars_) {
        float dist_x = std::abs(player_.x() - character.x());
        float dist_y = std::abs(player_.y() - character.y());

        if ( dist_x < player_.width()/1.5 && dist_y < player_.height()/1.5) {
            player_.move(-x, -y);       // don't allow them to move
            notify(Notification::PLAYER_COLLISION);
        }
    }

    // Check for item collisions
    for (auto it = props_.begin(); it != props_.end(); ) {
        float dist_x = std::abs(player_.x() - (*it).x());
        float dist_y = std::abs(player_.y() - (*it).y());

        if ( dist_x < player_.width()/2 && dist_y < player_.height()/2) {
            if ((*it).acquirable()) {
                // TODO acquire the item
            } else {
                if ((*it).healing() > 0) {
                    player_.addHealth((*it).healing());
                    Notification effect(Notification::HEALED);
                    effect.healed = (*it).healing();
                    notify(effect);
                }
                if ((*it).value() > 0) {
                    playerMoney_ += (*it).value();
                    Notification event(Notification::GOT_MONEY);
                    event.value = (*it).value();
                    notify(event);
                }
            }
            props_.erase(it);
        } else it++;
    }
}

// Always called from within a function with a charsLock_
void Model::playerAttack() {
    if (specialScreen_) return;

    // We only allow 1 attack per 500ms
    if (playerTimeout_ > 0) return;

    // idea: only show sword swinging if weapon is wielded, otherwise show a hit  todo
    // idea: show sword on L/R depending on which side enemy is standing          todo

    // Check if any enemies are in range - if so, attack them
    bool hit = false;
    for (int i = 0; i < int(chars_.size()); i++ ) {
        float dist_x = std::abs(player_.x() - chars_[i].x());
        float dist_y = std::abs(player_.y() - chars_[i].y());

        if ( dist_x < player_.width() + 20 && dist_y < player_.height() + 20) {
            int d = player_.attack( &chars_[i] );

            Notification attack(Notification::PLAYER_ATTACK);
            attack.damage = d;
            attack.hit = true;
            attack.enemy = chars_[i];
            notify( attack );

            hit = true;

            // If this is the first hit, activate the enemy
            if ( !chars_[i].hasWeapon() ) {
                chars_[i].equipWeapon(Weapon::makeWeapon("default enemy sword"));
            }
            if (!chars_[i].isActiveEnemy()) {
                chars_[i].setActiveEnemy(true);
                charTimeouts_[i] = 20;      // add a small delay before enemy hits back
            }
        }
    }

    if (!hit) {
        Notification attack(Notification::PLAYER_ATTACK);
        attack.hit = false;
        notify( attack );
    }

    // Check if any enemies died
    for (auto it = chars_.begin(); it != chars_.end(); ) {
        if (!(*it).isAlive()) {
            Notification death(Notification::ENEMY_DIED);
            death.enemy = (*it);
            notify(death);

            chars_.erase(it);
        } else it++;
    }

    playerTimeout_ = 10;
}

void Model::checkActiveEnemies() {
    if (specialScreen_) return;
    std::lock_guard<std::mutex> lock(charsLock_);

    for (int i = 0; i < int(chars_.size()); i++ ) {
        float dist_x = std::abs(player_.x() - chars_[i].x());
        float dist_y = std::abs(player_.y() - chars_[i].y());
        bool inRange = dist_x < player_.width() + 20 && dist_y < player_.height() + 20;

        if (inRange && chars_[i].isActiveEnemy() && charTimeouts_[i] == 0 ) {
            Notification attack(Notification::ENEMY_ATTACK);
            attack.damage = -1 * chars_[i].attack( &player_ );   // display as negative damage
            attack.enemy = chars_[i];
            notify(attack);

            // Set a timeout before they can attack again (based on speed)
            charTimeouts_[i] = chars_[i].timeOut();
        }
    }

    if ( !player_.isAlive() ) {
        notify(Notification::PLAYER_DIED);
        specialScreen_ = true;
    }
}