#include "Controller.h"
#include "Model.h"


Controller::Controller( Model * m) : model_( m ) {}

Controller::~Controller() {
    // deletes the data in the model
    //      Note that this includes deleting all the players
    // TODO
}


void Controller::invitePlayer( std::string playerType ) {
    model_->createPlayer( playerType );
}

void Controller::createNewDeck( int seed ) {
    model_->initializeDeck( seed );
    model_->shuffle();
    model_->deal();           
}

const int Controller::getCurrentPlayer() const { 
    return model_->getCurrentPlayer(); 
}

void Controller::startGame() {
    model_->startGame();        
}

void Controller::play( Card & c ) {
    model_->play( c );
    model_->endTurn();
}

// Interpreting commands from the view
void Controller::discard( Card & c ) {
    model_->discard( c );
    model_->endTurn();
}

void Controller::humanToComputer(int i) {
    model_->humanToComputer(i);
}


void Controller::finishPlayerTurn(int i) {
    model_->playComputer();
}