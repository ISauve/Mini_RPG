#ifndef MVC_MODEL_H
#define MVC_MODEL_H

#include "Subject.h"
#include "Human.h"
#include "Computer.h"
#include "CardPlayExceptions.h"
#include <cassert>
#include <vector>
#include <random>

// Model: game logic & data
//  -> the players, the deck, and the commands that alter their state
//  -> knows nothing about views/controllers (but both can access it)

class Model : public Subject {

    // Game data
    std::vector< Card > deck_;
    std::vector< Player * > players_;
    int currPlayer_;
    std::vector < int > winners_;
    Card * actionCard_;

    // Game state
    bool endOfRound_;
    bool firstRoundOfGame_;
    bool newGame_;
    bool playersInitialized_;
    bool gameEnd_;
    bool newRoundBegins_;
    bool beginningOfTurn_;
    bool playerHasPlayedCard_;
    bool currentPlayerHasRageQuitted_;
    int seed_;
    std::vector< Card * > playedDiamonds_;
    std::vector< Card * > playedHearts_;
    std::vector< Card * > playedClubs_;
    std::vector< Card * > playedSpades_;
    std::vector< Card * > legalPlays_;

    // Helper functions
    void computeLegalPlays();
    void incrementPlayer();
    bool endOfRound();
    bool newRound();
    void clearTable();

public:
    Model();

    // Regular actions
    void play( Card & );
    void discard( Card & );
    void printDeck() const ;
    void quit();
    void ragequit();
    void endTurn();
    void playComputer();

    // Beginning of the game/round
    void startGame();
    void shuffle();
    void deal();
    void initializeDeck( int );
    void createPlayer( std::string );

    // Accessors for the view
    const std::vector < Card * > & legalPlays() const { return legalPlays_; };
    const std::vector < Card * > & playedDiamonds() const { return playedDiamonds_; };
    const std::vector < Card * > & playedHearts() const { return playedHearts_; };
    const std::vector < Card * > & playedClubs() const  { return playedClubs_; };
    const std::vector < Card * > & playedSpades() const { return playedSpades_; };
    Card * actionCard() const { return actionCard_; };

    bool roundHasEnded() const { return endOfRound_; };
    bool firstRoundOfGame() const { return firstRoundOfGame_; };
    bool newGame() const { return newGame_; };
    bool playersInitialized() const { return playersInitialized_; };
    bool isBeginningOfTurn() const { return beginningOfTurn_; };
    bool gameEnd() const { return gameEnd_; };
    bool currentPlayerisHuman() const { return players_[ currPlayer_ ]->isHuman(); };
    bool playerHasPlayedCard() const { return playerHasPlayedCard_; };
    bool isNewRound() const { return newRoundBegins_; };
    bool currentPlayerhasRageQuitted() { return currentPlayerHasRageQuitted_; };
    bool playerHasLegalPlays() const { return !legalPlays_.empty(); };
    int getCurrentPlayer() const { return currPlayer_ ; };
    const std::vector < Card * > playerHand() const;
    std::vector< int > & winner()  { return winners_;};
    std::vector< Card * > & getCurrentPlayerDiscards();
    std::vector < Card > & deck() { return deck_; };
    std::vector< Player* > getPlayers() const { return players_; };
    Card* highestLegalCard();

    // Mutators
    void humanToComputer(int);
    void finishPlayerTurn(int);
};


#endif
