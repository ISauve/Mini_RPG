#include "Model.h"

Model::Model() {
    //attackThread_ = std::thread(&Model::enemyAttack, this);

    resetState();
};

void Model::resetState() {
    std::lock_guard<std::mutex> lock(charsLock_);
    chars_.clear();

    float x, y;
    x = SCREEN_WIDTH/2 - PLAYER_WIDTH;
    y = SCREEN_HEIGHT/2;

    // Note: Character sheet is 960px x 12 sprites   -> 80px width
    //                          640px x 8 sprites    -> 80px height

    // Set default char (moustached man)
    Character player(10, 10, x, y, true, "View/Textures/Character_set_2.png", 80, 80);
    player.setCharacter(4, 1);
    chars_.push_back(player);

    // Set 1st enemy
    x = SCREEN_WIDTH/2 + PLAYER_WIDTH;
    chars_.push_back( Character(10, 10, x, y, false, "View/Textures/Enemy_1.png", 100, 110) );
}


void Model::endGame() {
    notify(QUIT_GAME);

    // attackThread_.join();
    // probably want to save game data to some external file
}


void Model::attack() {
    std::lock_guard<std::mutex> lock(charsLock_);

    // idea: only show sword swinging if weapon is wielded, otherwise show a slap  todo
    // idea: show sword on L/R depending on which side enemy is standing           todo
    Notification attack(PLAYER_ATTACK);
    attack.damage = 0;
    attack.hit = false;

    // Check if any enemies are in range - if so, attack them
    for (int i = 1; i < chars_.size(); i++ ) {
        float dist_x = std::abs(chars_[0].x() - chars_[i].x());
        float dist_y = std::abs(chars_[0].y() - chars_[i].y());

        if ( dist_x < chars_[0].width() + 20 && dist_y < chars_[0].height() + 20) {
            int d = chars_[0].attack( &chars_[1] );

            attack.damage = d;
            attack.hit = true;

            // If this is the first hit, make enemy draw weapon & start fighting
            if ( !chars_[i].hasWeapon() ) {
                Weapon* enemy_sword = new Weapon(10, 10, chars_[i].x() + chars_[i].width()/2,  chars_[i].y(), false,
                                                 "View/Textures/Enemy_Sword_2.png", 50, 54);
                chars_[i].equipWeapon(enemy_sword);
                chars_[i].setActiveEnemy(true);
            }
        }
    }

    notify( attack );

    // Check if any enemies died
    for (int i = 1; i < chars_.size(); i++ ) {
        if (!chars_[i].isAlive()) {
            chars_[i].setImage("View/Textures/Dead.png");
            chars_[i].removeWeapon();

            Notification death(ENEMY_DIED);
            death.enemy = &chars_[i];
            notify( death );
        }
    }
}

void Model::movePlayer(int x, int y) {
    std::lock_guard<std::mutex> lock(charsLock_);

    chars_[0].move(x, y);

    // Check for player collision
    for (int i = 1; i < chars_.size(); i++) {
        float dist_x = std::abs(chars_[0].x() - chars_[i].x());
        float dist_y = std::abs(chars_[0].y() - chars_[i].y());

        if ( dist_x < chars_[0].width()/1.5 && dist_y < chars_[0].height()/1.5) {
            chars_[0].move(-x, -y);       // don't allow them to move
            notify(PLAYER_COLLISION);
        }
    }
}

Character* Model::player() {
    std::lock_guard<std::mutex> lock(charsLock_);
    return &chars_[0];
};

Character* Model::enemy() {
    std::lock_guard<std::mutex> lock(charsLock_);
    return &chars_[1];
};

std::vector< Character > Model::getChars() {
    std::lock_guard<std::mutex> lock(charsLock_);
    return chars_;
};
