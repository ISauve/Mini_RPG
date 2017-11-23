#include "Player.h"

Player::Player( bool isHuman ) : isHuman_( isHuman ),
 score_( 0 ),
 oldScore_( 0 ){}
 
void Player::deal( std::vector < Card > & deck, int player ) {

    // For every new game, empty the player's hand before adding new cards
    if( ! hand_.empty() ) {
        while( ! hand_.empty() ) {
            hand_.erase( hand_.begin() );
        }
    }

    // Fill hand of player with new cards
    for( int i = 0; i < 13; i++ ) {
        hand_.push_back( &( deck[ 13 * player + i ] ) );
    }

    // Empty discards list from previous round, if any
    if( ! discardEmpty() ) {
        while( ! discardEmpty() ) {
            discards_.erase( discards_.begin() );
        }
    }

    // Update total score of the player from previous rounds
    oldScore_ += score_;
    score_ = 0;
}

Card* Player::play( Card & c ) {
    for ( auto it = hand_.begin(); it != hand_.end(); it++ ) {
        if ( * ( * it ) == c ) {
            Card * temp = * it;
            hand_.erase( it );
            return temp;
        }
    }
}

Card * Player::discard( Card & c ) {
    bool cardExists = false;
    Card * temp;
    for ( auto it = hand_.begin(); it != hand_.end(); it++ ) {
        if ( *( * it ) == c ) {
            cardExists = true;
            temp = * it;
            discards_.push_back( * it );
            hand_.erase( it );
            break;
        }
    }
    // Assert that the card the player wishes to discard is in the player's hand
    assert( cardExists );
    // Add discarded card to score of player
    this->updateCurrentRoundScore( c.rank().rank() + 1 );
    return temp;
}

Card * Player::discard() {
    return nullptr;
}

void Player::updateCurrentRoundScore( int valueOfDiscard ) {
    score_+= valueOfDiscard;
}