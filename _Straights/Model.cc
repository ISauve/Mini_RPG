#include "Model.h"
#include "Computer.h"
#include "Human.h"
#include <string>


Model::Model() : currPlayer_(0),
                 firstRoundOfGame_(true),
                 endOfRound_(false),
                 newGame_(true),
                 playersInitialized_(false),
                 gameEnd_(false),
                 newRoundBegins_( true ),
                 beginningOfTurn_( true ),
                 playerHasPlayedCard_( true ),
                 currentPlayerHasRageQuitted_( false ),
                 seed_(0){};



/*************** Beginning of game/round functions ***************/

void Model::createPlayer( std::string playerType ) {
    Player * newPlayer;
    if( !playerType.compare( "h" ) ) {
        newPlayer = new Human();
    }
    else {
        newPlayer = new Computer();
    }

    players_.push_back( newPlayer );

    // Check if that was the last player that has to be created
    if (players_.size() == 4) {
        playersInitialized_ = true;
    }

    notify();
}

void Model::initializeDeck( int seed ) {
    // sets seed value for shuffling
    seed_ = seed;

    // fills in deck in the "start order"
    for ( int suite = 0; suite < Card::Suit::MAX_SUIT; suite++ ) {
        for ( int rank = 0; rank < Card::Rank::MAX_RANK; rank++ ) {
            Card c = Card( rank, suite );
            deck_.push_back( c );
        }
    }
}

void Model::shuffle() {
    static std::mt19937 rng( seed_ );

    int n = 52;

    while ( n > 1 ) {
        int k = ( int ) ( rng() % n );
        --n;
        Card c = deck_[ n ];
        deck_[ n ] = deck_[ k ];
        deck_[ k ] = c;
    }
}

void Model::deal() {
    // Reset game state:
    clearTable();               // Erase history of discards from previous players
    newRoundBegins_ = true;     // Cards are only redealt every new round
    beginningOfTurn_ = true;    // A new turn always begins after dealing
    gameEnd_ = false;           // Game cannot end as soon as a new round starts
    newGame_ = false;           // Setup for a new game is complete for now

    // After cards are shuffled, each group of 13 consecutive cards is assigned to a unique player
    for( int i = 0; i < 4; i++ ) {
        players_[ i ]->deal( deck_, i );
    }

    // Set first player of the game, the player with 7S
    for( int i = 0; i < deck_.size(); i++ ) {
        if ( deck_[ i ].suit().suit()  == 3  && deck_[ i ].rank().rank() == 6 ) {
            int j = floor( i / 13 ) ;
            currPlayer_ = j;
            break;
        }
    }

    // Legal plays must be computed before any turn
    this->computeLegalPlays();
    notify();
    newRoundBegins_ = false;
}

void Model::startGame() {
    if ( !firstRoundOfGame_ ) {
        for ( auto it : players_ ) it->clearScore();
    }
    
    newRoundBegins_ = false;
    firstRoundOfGame_ = false;

    if ( !currentPlayerisHuman() ) {
        playComputer();
    } else {
        notify();
    }
}


/*************** Main actions ***************/

// Gets called after a player completes a move
void Model::endTurn() {
    // Update game state
    beginningOfTurn_ = false;

    // Check for end of game
    if ( endOfRound() ) {
        if( newRound() ) {
            notify();
            endOfRound_ = false;

            // Reshuffle and redeal cards for the next round
            // A new round begins everytime cards are redealt
            shuffle();
            deal();

            if ( !players_[ currPlayer_ ]->isHuman() ) {
                this->playComputer();
            }
        } else {
            gameEnd_ = true;
            int minScore = players_[0]->oldScore() + players_[0]->score();
            // First pass: Calculate smallest score
            for( int i = 0; i < 4; i++ ) {
                currPlayer_ = i;
                if( players_[i]->oldScore() + players_[i]->score() < minScore ){
                    minScore = players_[i]->oldScore() + players_[i]->score();
                }
            }
            // Second pass: Record all players that held the smallest score so far
            for( int i = 0; i < 4; i++ ) {
                if( ( players_[i]->oldScore() + players_[i]->score() ) == minScore ) {
                    winners_.push_back(i);
                }
            }
            notify();
        }
    }
    else {
        // Display end of turn results
        notify();

        // Get next player
        incrementPlayer();
        bool nextPlayerType = players_[ currPlayer_ ]->isHuman();

        // Refresh turn state of game
        beginningOfTurn_ = true;
        computeLegalPlays();

        // Starts the next turn if the player is human
        notify();

        // Starts the next turn if the player is a computer
        if ( !nextPlayerType  ) playComputer();
    }
}

void Model::play( Card & c ) {

    // Check legality of play only for Human players
    if ( players_[ currPlayer_]->isHuman() ) {
        // Player cannot put any card on the table with no legal plays
        if( legalPlays_.empty() ) {
            throw IllegalPlayException();
        }
        // Player must play a card that is a legal play
        for( auto it :legalPlays_ ) {
            if( c.rank().rank() == it->rank().rank() && c.suit().suit() == it->suit().suit()) break;
            // Card is not legal play
            if( it == legalPlays_[ legalPlays_.size() - 1 ] ) throw IllegalPlayException();
        }
    }

    // Played card must be put on the table
    Card* played = players_[currPlayer_]->play( c );
    switch ( played->suit().suit() ) {
        case 0: // clubs
            playedClubs_.push_back( played );
            break;
        case 1: // diamonds
            playedDiamonds_.push_back( played );
            break;
        case 2: // hearts
            playedHearts_.push_back( played );
            break;
        case 3: // spades
            playedSpades_.push_back( played );
            break;
    }
    // Update card to be presented
    actionCard_ = played;
    playerHasPlayedCard_ = true;
}

void Model::discard( Card & c ) {
    // User can't discard when there are legal plays
    if( !legalPlays_.empty() ) {
        throw IllegalDiscardException();
    }
    actionCard_ = players_[currPlayer_]->discard( c );
    playerHasPlayedCard_ = false;
}

void Model::printDeck() const {
    for( int i = 0; i < 4 ; i++ ) {
        for ( int j = 0; j < 13; j++ ) {
            std::cout << deck_[ 13 * i + j ] << " ";
        }
        std::cout << std::endl;
    }
}

void Model::quit() {
    gameEnd_ = true;
}

void Model::ragequit() {
    // Temporary solution
    players_[ currPlayer_ ]->changeType();
    currentPlayerHasRageQuitted_ = true;
    this->notify();

    // Reset ragequit status of current player
    currentPlayerHasRageQuitted_ = false;
    this->playComputer();
}

const std::vector < Card * > Model::playerHand() const {
    return players_[ currPlayer_ ]->hand();
};



/*************** Helper functions ***************/

void Model::playComputer() {

    // Discards the lowest card in the hand, or plays the highest
    if ( legalPlays_.empty() ) {
        Card* discarded =  players_[ currPlayer_]->discard();

        // Indicate that player has discarded
        playerHasPlayedCard_ = false;
        actionCard_ = discarded;
    } 
    else {
        Card* played = highestLegalCard();

        // Update table with card played
        this->play( *played );
        actionCard_ = played;
        playerHasPlayedCard_ = true;
    }

    // End of turn events will be printed
    endTurn();
}

void Model::computeLegalPlays() {
    // Clear last player's set of legal plays 
    while ( !legalPlays_.empty() ) {
        legalPlays_.erase( legalPlays_.begin() );
    }
    std::vector< Card * > hand = playerHand();
    if( !hand.empty() ) {
        for ( auto it : hand ) {
            // Check for 7S
            if( it->rank().rank() == 6 && it->suit().suit() == 3 ) {
                // Clear previously calculated legal plays
                while ( !legalPlays_.empty() ) {
                    legalPlays_.erase( legalPlays_.begin() );
                } 
                // 7S should be the only valid play
                legalPlays_.push_back( it ); 
                break;
            }
            // Any card with rank 7 is a legal play
            if( it->rank().rank() == 6 ) {
                legalPlays_.push_back( it );
            }
            else {
                // Look for potential adjacency in Spades Suit
                switch ( it->suit().suit() ) {
                    case 3: {
                        for ( auto spadesIt : playedSpades_ ) {
                            if( ( spadesIt->rank().rank() == it->rank().rank() + 1 ) || ( spadesIt->rank().rank() == it->rank().rank() - 1 ) ) {
                                legalPlays_.push_back( it );
                                break;
                            }
                        }  
                        break; 
                    }
                    // Look for potential adjacency in Hearts Suit
                    case 2: {
                        for ( auto heartsIt : playedHearts_ ) {
                            if( ( heartsIt->rank().rank() == it->rank().rank() + 1 ) || ( heartsIt->rank().rank() == it->rank().rank() - 1 ) ) {
                                legalPlays_.push_back( it );
                                break;
                            }
                        }
                        break;
                    }
                    // Look for potential adjacency in Diamonds Suit
                    case 1 : {
                        for ( auto diamondsIt : playedDiamonds_ ) {
                            if( ( diamondsIt->rank().rank() == it->rank().rank() + 1 ) || ( diamondsIt->rank().rank() == it->rank().rank() - 1 ) ) {
                                legalPlays_.push_back( it );
                                break;
                            }
                        }
                        break;
                    }
                    // Look for potential adjacency in Clubs Suit
                    case 0 : {
                        for ( auto clubsIt : playedClubs_ ) {
                            if( ( clubsIt->rank().rank() == it->rank().rank() + 1 ) || ( clubsIt->rank().rank() == it->rank().rank() - 1 ) ) {
                                legalPlays_.push_back( it );
                                break;
                            }
                        }
                        break;
                    }
                } 

            }
        }
    }
    // fills in legalPlays based on currPlayers hand and all the discarded cards
}

void Model::incrementPlayer() {
    if (currPlayer_ != 3) {
        currPlayer_ ++;
    }
    else {
        currPlayer_ = 0;
    }    
}

bool Model::endOfRound() {
    endOfRound_ = players_[0]->handEmpty() && players_[1]->handEmpty() &&
                  players_[2]->handEmpty() && players_[3]->handEmpty();

    return endOfRound_;
}

bool Model::newRound() {
    return ((players_[0]->score() + players_[0]->oldScore()) <= 80) &&
           ((players_[1]->score() + players_[1]->oldScore()) <= 80) &&
           ((players_[2]->score() + players_[2]->oldScore()) <= 80) &&
           ((players_[3]->score() + players_[3]->oldScore()) <= 80);
}

std::vector< Card * > & Model::getCurrentPlayerDiscards() {
    return players_[ currPlayer_ ]->fetchDiscards();
}

void Model::clearTable() {
    //Before starting a new round, clear all informaiton about cards played from previous rounds
    while ( !playedDiamonds_.empty() ) {
        playedDiamonds_.erase( playedDiamonds_.begin() );
    }
    while ( !playedClubs_.empty() ) {
        playedClubs_.erase( playedClubs_.begin() );
    }
     while ( !playedHearts_.empty() ) {
        playedHearts_.erase( playedHearts_.begin() );
    }
    while ( !playedSpades_.empty() ) {
        playedSpades_.erase( playedSpades_.begin() );
    }
    while ( !playedClubs_.empty() ) {
        playedDiamonds_.erase( playedClubs_.begin() );
    }

}

void Model::humanToComputer(int i) {
    Player* old = players_[i];

    std::vector< Card * > newHand = old->hand();
    std::vector< Card * > newDiscards = old->fetchDiscards();
    Player* newComp = new Computer(newHand, newDiscards);

    players_[i] = newComp;
}


Card* Model::highestLegalCard() {
    Card* h = legalPlays_[0];
    for (auto it : legalPlays_) {
        // this comes from the given boilerplate code
        if ( it->rank().rank() > h->rank().rank() ) h = it;
    }

    // return a new card with the same values
    return h;
}

