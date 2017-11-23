#ifndef CS_247_PLAYER_H
#define CS_247_PLAYER_H

#include "Card.h"
#include <vector>
#include <cassert>

class Player {

public:
    virtual Card * discard();
    Card * play( Card & );
    Card * discard( Card & );
    void deal( std::vector < Card > & deck, int player );

    // Mutators
    void changeType() { isHuman_ = !isHuman_; };
    void clearScore() { oldScore_ = 0; score_ = 0; };

    // Accessors
    int score() const { return score_; };
    int oldScore() const { return oldScore_;};
    bool isHuman() const { return isHuman_; };
    bool handEmpty() const { return hand_.empty(); }
    bool discardEmpty() const { return discards_.empty(); }
    std::vector < Card * > hand() { return hand_; };
    std::vector< Card * > & fetchDiscards(){ return discards_; }
    
protected:                                                   
    Player( bool );
    void updateCurrentRoundScore( int );
    std::vector< Card * > hand_;
    std::vector< Card * > discards_;

private:
    bool isHuman_;
    int score_;
    int oldScore_;
};

#endif //CS_247_PLAYER_H
