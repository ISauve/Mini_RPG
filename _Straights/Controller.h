#ifndef CS_247_GAME_H
#define CS_247_GAME_H

#include "Command.h"
#include <vector>
#include <random>

// Controller: mediates between model & view
//  -> translates UI events into call to model

class Model;

class Controller {
    Model * model_;

public:
    Controller( Model * );
    ~Controller();

    void invitePlayer( std::string );
    void createNewDeck( int );
    void startGame();
    const int getCurrentPlayer() const;
    void play( Card & );
    void discard( Card & );
    void humanToComputer(int);
    void finishPlayerTurn(int i);
};

#endif //CS_247_GAME_H
