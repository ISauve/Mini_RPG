#include "Model.h"

// TODO: remove
#include <iostream>

static std::mutex ATTACK_THREAD_MUTEX;

Model::Model() : gameOver_(false) {
    attackThread_ = std::thread(&Model::enemyAttack, this);

    reset();
};

void Model::reset() {           // todo: need lockguard?
    chars_.clear();

    float x, y;
    x = SCREEN_WIDTH/2 - PLAYER_WIDTH;
    y = SCREEN_HEIGHT/2;
    // Note: Character sheet is 960px x 12 sprites   -> 80px width
    //                          640px x 8 sprites    -> 80px height
    Character player(10, 10, x, y, true, "View/Textures/Character_set_2.png", 80, 80);
    player.setCharacter(4, 1);      // Default char = moustached man
    chars_.push_back(player);

    x = SCREEN_WIDTH/2 + PLAYER_WIDTH;
    chars_.push_back( Character(10, 10, x, y, false, "View/Textures/Enemy_1.png", 100, 110) );    // 1st enemy
}

// Handles player quitting (via escape or clicking X)
void Model::endGame() {
    notify(QUIT_GAME);
    gameOver_ = true;
    attackThread_.join();           // synchronize the threads

    // probably want to save game data to some external file
}

// Checks if player is in range of any active enemies, and attacks if yes
void Model::enemyAttack() {

    // Reads: gameOver_, chars_
    ATTACK_THREAD_MUTEX.lock();
    if (gameOver_) return;

    for (int i = 1; i < chars_.size(); i++ ) {
        float dist_x = std::abs(chars_[0].x() - chars_[i].x());
        float dist_y = std::abs(chars_[0].y() - chars_[i].y());
        bool inRange = dist_x < chars_[0].width() + 20 && dist_y < chars_[0].height() + 20;


        if (inRange && chars_[i].isActiveEnemy()) {
            int damage = chars_[i].attack( &chars_[0] );
            enemyAttacks_.push( Attack(damage, &chars_[i]) );
        }
    }

    if ( !chars_[0].isAlive() ) {
        //reset();          probably can't call reset from this thread...
    }
    ATTACK_THREAD_MUTEX.unlock();

    // call back fcn after a 3 second wait
    std::this_thread::sleep_for(std::chrono::seconds(2));
    enemyAttack();
}

void Model::attack() {
    std::lock_guard<std::mutex> lock(ATTACK_THREAD_MUTEX);

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

    for (int i = 1; i < chars_.size(); i++ ) {
        if ( !chars_[i].isAlive() ) {
            chars_[i].setImage("View/Textures/Dead.png");
            chars_[i].removeWeapon();

            //return reset();
        }
    }
}

void Model::movePlayer(int x, int y) {
    std::lock_guard<std::mutex> lock(ATTACK_THREAD_MUTEX);
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
