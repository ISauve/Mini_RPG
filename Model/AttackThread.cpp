#include "Model.h"

// Checks if player is in range of any active enemies, and attacks if yes
// TODO: add some kind of synchronization to make this thread quit
/*
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
 */