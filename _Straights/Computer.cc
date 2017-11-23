#include "Computer.h"

Computer::Computer(std::vector< Card *> newHand, std::vector< Card * > newDiscards) : Player( false ) {
    for (auto it : newHand) {
        hand_.push_back(it);
    }
    for (auto it : newDiscards) {
        discards_.push_back(it);
    }
}


Computer::Computer() : Player( false ) {}

Card * Computer::discard() {
    // Requires: there are no legal plays for computer player this turn

    // Discards lowest card in hand
    Card lowest = this->lowest();

    Card* discarded = nullptr;
    for (auto it = hand_.begin(); it != hand_.end(); it++) {
        if (*(*it) == lowest) {
            discarded = *it;
            hand_.erase( it );
            break;
        }
    }

    // Update discards
    discards_.push_back( discarded );

    // Update score
    this->updateCurrentRoundScore( discarded->rank().rank() + 1 );
    return discarded;
}

Card Computer::lowest() {
    Card l = *hand_[0];
    for (auto it : hand_) {
        // this comes from the given boilerplate code
        if ( it->rank().rank() < l.rank().rank() ) l = *it;
    }

    // return a new card with the same values
    return Card( l.rank(), l.suit() );
};
