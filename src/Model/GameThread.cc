#include "Model.h"

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
        for (int i = 0; i < int(charTimeouts_.size()); i++) {
            if (charTimeouts_[i] > 0) charTimeouts_[i]--;
        }

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

        case EventPackage::SELECT_PLAYER:
            // Set the new character
            chars_[0].setCharacter(e.row, e.col);
            // Notify the view that player selection is complete
            notify(Notification::EXIT_SPECIAL_SCREEN);
            specialScreen_ = false;
            break;

        case EventPackage::ADD_HEALTH:      // for testing - need to remove  TODO
            chars_[0].addHealth(100);

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
    notify(Notification::RESET);

    // Read in default configuration
    Config conf;
    conf.readConfig("resources/Confs/default.json");
    chars_ = conf.getChars();
    for (int i = 0; i < int(chars_.size()); i++) { charTimeouts_.push_back(0); }
    items_ = conf.getItems();   // TODO: add items

    // todo: get a ptr to the player & save it separately from the array
}

// Always called from within a function with a charsLock_
void Model::movePlayer(int x, int y) {
    if (specialScreen_) return;

    chars_[0].move(x, y);

    // Check for player collision
    for (int i = 1; i < int(chars_.size()); i++) {
        float dist_x = std::abs(chars_[0].x() - chars_[i].x());
        float dist_y = std::abs(chars_[0].y() - chars_[i].y());

        if ( dist_x < chars_[0].width()/1.5 && dist_y < chars_[0].height()/1.5) {
            chars_[0].move(-x, -y);       // don't allow them to move
            notify(Notification::PLAYER_COLLISION);
        }
    }
}

// Always called from within a function with a charsLock_
void Model::playerAttack() {
    if (specialScreen_) return;

    // We only allow 1 attack per 500ms
    if (charTimeouts_[0] > 0) return;

    // idea: only show sword swinging if weapon is wielded, otherwise show a slap  todo
    // idea: show sword on L/R depending on which side enemy is standing           todo
    bool hit = false;

    // Check if any enemies are in range - if so, attack them
    for (int i = 1; i < int(chars_.size()); i++ ) {
        float dist_x = std::abs(chars_[0].x() - chars_[i].x());
        float dist_y = std::abs(chars_[0].y() - chars_[i].y());

        if ( dist_x < chars_[0].width() + 20 && dist_y < chars_[0].height() + 20) {
            int d = chars_[0].attack( &chars_[i] );

            Notification attack(Notification::PLAYER_ATTACK);
            attack.damage = d;
            attack.hit = true;
            attack.enemy = chars_[i];
            notify( attack );

            hit = true;

            // If this is the first hit, make enemy draw weapon & start fighting
            if ( !chars_[i].hasWeapon() ) {
                Weapon* enemy_sword = new Weapon(10, 10, chars_[i].x() + chars_[i].width()/2,  chars_[i].y(), false,
                                                 "resources/Textures/Enemy_Sword_2.png", 50, 54);
                chars_[i].equipWeapon(enemy_sword);
                chars_[i].setActiveEnemy(true);
                charTimeouts_[i] = 20;      // add a small delay before he hits back
            }
        }
    }

    if (!hit) {
        Notification attack(Notification::PLAYER_ATTACK);
        attack.hit = false;
        notify( attack );
    }

    // Check if any enemies died
    for (int i = 1; i < int(chars_.size()); ) {     // start at 1 to skip the player
        if (!chars_[i].isAlive()) {
            Notification death(Notification::ENEMY_DIED);
            death.enemy = chars_[i];
            notify(death);

            chars_.erase(chars_.begin() + i);
        } else i++;
    }

    charTimeouts_[0] = 10;
}

void Model::checkActiveEnemies() {
    if (specialScreen_) return;
    std::lock_guard<std::mutex> lock(charsLock_);

    for (int i = 1; i < int(chars_.size()); i++ ) {
        float dist_x = std::abs(chars_[0].x() - chars_[i].x());
        float dist_y = std::abs(chars_[0].y() - chars_[i].y());
        bool inRange = dist_x < chars_[0].width() + 20 && dist_y < chars_[0].height() + 20;

        if (inRange && chars_[i].isActiveEnemy() && charTimeouts_[i] == 0 ) {
            Notification attack(Notification::ENEMY_ATTACK);
            attack.damage = -1 * chars_[i].attack( &chars_[0] );   // display as negative damage
            attack.enemy = chars_[i];
            notify(attack);

            // 1s before this enemy can attack again
            charTimeouts_[i] = 20;
        }
    }

    if ( !chars_[0].isAlive() ) {
        notify(Notification::PLAYER_DIED);
        specialScreen_ = true;
    }
}