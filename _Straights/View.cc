#include "View.h"
#include "Controller.h"
#include "Model.h"
#include "Subject.h"
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>

// Initialize the game window: ask if players are human or computer
View::View( Controller * c, Model * m ) :
    model_(m), controller_(c), builder{nullptr},
    text{""}, main_box{ Gtk::ORIENTATION_HORIZONTAL },
    c_player_button{ "Computer" }, h_player_button{ "Human" }, game_view{nullptr}, text_entry{nullptr} {

    set_border_width( 100 );

    // Set the title
    text.set_markup("Is player 1 a human or computer?");

    // Initialize the buttons
    add( main_box );
    main_box.add( text );
    main_box.add( c_player_button );
    main_box.add( h_player_button );

    // Attach handlers to the 2 initial buttons
    c_player_button.signal_clicked().connect( sigc::mem_fun( * this, & View::onComputerButtonClicked ) );
    h_player_button.signal_clicked().connect( sigc::mem_fun( * this, & View::onHumanButtonClicked ) );

    show_all_children();

	// Register view as observer of model
	model_->subscribe( this );
}

/*************** Update ***************/

// Interpret updates from the model
void View::update() {
    // At the beginning of the game, initialize all the players
    if ( !model_->playersInitialized() ) this->promptNextPlayerSelection();

    // Once the players are initialized, model calls for the initial setup of the view/
    else if ( model_->newGame() ) this->initialSetup();

    // If the game naturally comes to the end of a round, display the appropriate information
    else if ( model_->isNewRound() ) this->displayNewRoundBegins();

    // At the beginning of a player turn, display appropriate information
    else if ( model_->isBeginningOfTurn() ) this->displayNewTurn();

    // If none of the above: a play has occurred, so we need to display this
    else {
        // Display the a card if it has been played
        if ( model_->playerHasPlayedCard() ) {
            Card * played = model_->actionCard();

            // Card ID's in the grid are = rank*10 + suit
            int position = played->rank().rank() * 10 + played->suit().suit();
            std::string id = std::to_string(position);

            Gtk::Image * card = nullptr;
            this->builder->get_widget( id, card );
            card->set( getCardPath( played ) );
        }

        updateDiscardsAndScoresCount();

        // As soon as the last play of the game has been displayed, present the winners and the scores of all players
        // End game results must be displayed immediately without waiting for unnecessary user input
        if ( model_->roundHasEnded() ) this->displayEndOfRound();
        if ( model_->gameEnd() ) this->displayGameEnd();
    }
}

/*************** Signal handlers ***************/

void View::onComputerButtonClicked() {
    controller_->invitePlayer( "c" );
};

void View::onHumanButtonClicked() {
    controller_->invitePlayer( "h" );
};

void View::onNewGameButtonClicked() {
    // Get the seed from the text entry box
    std::string seed_string = Glib::locale_from_utf8(  text_entry->get_text() );
    int seed = std::stoi( seed_string );
    resetTableCards();
    updateDiscardsAndScoresCount( true );       
    controller_->createNewDeck( seed );
    controller_->startGame();
}

void View::onPlayerCardClicked( unsigned int i ) {
    Card * card = model_->playerHand()[ i ];
    
    if ( model_->playerHasLegalPlays() ) {
        controller_->play( * card );
    } else {
        controller_->discard( * card );
    }
}

void View::onRageQuitClicked( int i ) {
    // Grey out this rage quit button
    std::string id = "rage_quit_" + std::to_string( i );

    Gtk::Button * widget = nullptr;
    builder->get_widget( id, widget );
    widget->set_sensitive( false );

    controller_->humanToComputer( i );

    // If it was their turn, make the computer finish their turn
    int curr = model_->getCurrentPlayer();
    if ( curr == i ) {
        clearPlayerHand();
        controller_->finishPlayerTurn( i );
    }
}

void View::onQuitGameButtonClicked() {
    this->close();
}

/*************** Displaying the view ***************/

void View::displayGameView() {
    // Remove the earlier widgets
    main_box.remove( text );
    main_box.remove( c_player_button );
    main_box.remove( h_player_button );
    this->set_border_width( 0 );

    // Read in the Glade file
    this->builder = Gtk::Builder::create_from_file( "straights.glade" );
    builder->get_widget( "game_view", game_view );
    main_box.add( * game_view );

    // Disabled the rage quit buttons for the computer players
    auto players = model_->getPlayers();
    for ( int i = 0; i < 4; i++ ) {
        if ( !players[i]->isHuman() ) {
            std::string id = "rage_quit_" + std::to_string( i );

            Gtk::Button * widget = nullptr;
            this->builder->get_widget( id, widget );
            widget->set_sensitive( false );
        }
    }

    // Attach a handler to the new game button
    Gtk::Button * new_game_with_seed = nullptr;
    this->builder->get_widget( "new_game_with_seed", new_game_with_seed );
    new_game_with_seed->signal_clicked().connect( sigc::mem_fun( * this, & View::onNewGameButtonClicked ) );

    // Setup buttons to handle actions on player's hand of cards
    for ( int i = 0; i < 13; i++ ) {
        std::string id = "00" + std::to_string( i + 1 );
        Gtk::Button * card_button = nullptr;
        this->builder->get_widget( id, card_button );
        // Set signal handler for each button  
        card_button->signal_clicked().connect( 
            sigc::bind<unsigned int>( sigc::mem_fun( * this, & View::onPlayerCardClicked ), i ) 
        );
    }

    // Attach rage quit button handlers
    for (int i = 0; i < 4; i++) {
        std::string id = "rage_quit_" + std::to_string( i );
        Gtk::Button * rage_quit_button = nullptr;
        builder->get_widget( id, rage_quit_button );

        rage_quit_button->signal_clicked().connect(
                sigc::bind<unsigned int>( sigc::mem_fun( * this, & View::onRageQuitClicked ), i )
        );
    }

    // Initialize other widgets
    builder->get_widget( "text_entry", text_entry );
    Gtk::Button * quit = nullptr;
    this->builder->get_widget( "quit_game", quit );
    quit->signal_clicked().connect( sigc::mem_fun( * this, & View::onQuitGameButtonClicked ));

    game_view->show();

    // Set the images of the buttons to display no cards in the beginning
    this->clearPlayerHand();
}


void View::displayNewRoundBegins() {
    if ( !model_->firstRoundOfGame() ) this->resetTableCards();
    Gtk::MessageDialog dialog( * this, "A new round begins, click to continue!");
    dialog.set_secondary_text(
            "It's player's " + std::to_string(controller_->getCurrentPlayer() + 1) + "'s turn to play.");
    dialog.run();      
}

void View::displayNewTurn() {
    // Allow human to play
    if( model_->currentPlayerisHuman() ) {   
        this->clearPlayerHand();
        // Display their hand
        const std::vector < Card * > playerHand = model_->playerHand();
        const std::vector < Card * > legalPlays = model_->legalPlays();
 
        for ( int i = 0; i < playerHand.size(); i++ ) {
            std::string id = "00" + std::to_string( i + 1 );
    
            Gtk::Button * card_button = nullptr;
            builder->get_widget( id, card_button );
        
            Gtk::Image * card_img = Gtk::manage( new Gtk::Image( getCardPath( playerHand[ i ] ) ) );
            card_button->set_image( * card_img );
           
            // Allow only legal plays (if there are any)
            if ( !legalPlays.empty() ) {
                for (auto it : legalPlays) {
                    if ( * playerHand[ i ] == * it ) {
                        card_button->set_sensitive( true );
                    }
                }
            }
            else {
                // If no legal plays, every card is a valid discard
                card_button->set_sensitive( true );
            }
        }
    }
}

// Display discards and scores of all players
void View::displayGameEnd() {
    // Display winners
    std::string output = "";
    std::vector< int > winners = model_->winner();
    for ( int i = 0; i < winners.size(); i++ ) {
        output += "Player " + std::to_string( winners[ i ] + 1 ) + " wins!\n";
    }

    Gtk::MessageDialog dialog( * this, output );
    dialog.run();       // Returns once player is ready to start the game
}

void View::displayEndOfRound() {
    std::string output = "";

    std::vector< Player * > players = model_->getPlayers();

    // Display discards and scores of all players
    for (int i = 0; i < players.size(); i++ ) {
        // Displays discards
        output +=  "Player " + std::to_string( i + 1 ) + "'s discards:";

        std::vector< Card * > discards = players[ i ]->fetchDiscards();
        std::stringstream ss;
        for( auto it: discards ) {
            ss << ' ' << * it ;
        }
        output += ss.str() + "\n";

        // Displays score
        output += "Player " + std::to_string( i + 1 ) + "'s score: ";
        output += std::to_string( players[ i ]->oldScore() ) + " + "
                  + std::to_string( players[ i ]->score() ) + " = "
                  + std::to_string( players[ i ] ->oldScore() + players[ i ]->score() ) + "\n";

        this->updateDiscardsAndScoresCount();
    }

    Gtk::MessageDialog dialog( * this, output );
    dialog.run();
}

/*************** Other helpers ***************/

void View::promptNextPlayerSelection() {
    text.set_markup( "Is player " + std::to_string(model_->getPlayers().size() + 1) + " a human or computer?" );
}

void View::initialSetup() {

    controller_->createNewDeck( 0 );
    this->displayGameView();            
    controller_->startGame();
}

void View::resetTableCards() {
    for ( int i = 0; i < 13; i++ ) {
        for ( int j = 0; j < 4; j++ ) {
            int position = i * 10 + j;
            std::string id = std::to_string( position );

            Gtk::Image * card = nullptr;
            builder->get_widget( id, card );
            card->set( "cards_png/back_1.png" );
        }
    }
}

std::string View::getCardPath( Card * card ) const {
    std::string image_id = "cards_png/" + std::to_string( card->suit().suit() ) + "_" ;
    std::string rank_s;
    switch ( card->rank().rank() ) {
        case 10:
            rank_s = "j"; break;
        case 11:
            rank_s = "q"; break;
        case 12:
            rank_s = "k"; break;
        default:
            rank_s = std::to_string( card->rank().rank() ); break;
    }
    image_id += rank_s + ".png";
    return image_id;
}

void View::clearPlayerHand() {
    for ( int i = 1; i < 14; i++ ) {

        std::string id = "00" + std::to_string( i );
        Gtk::Button * card_button = nullptr;
        this->builder->get_widget( id, card_button );

        Gtk::Image * card_img = Gtk::manage( new Gtk::Image( "cards_png/nothing.png" ) );
        card_button->set_image( * card_img ) ;
        card_button->set_sensitive( false );
    }
}

void View::updateDiscardsAndScoresCount( bool resetToZero ) {     // defaulted to false
    // Update all the discards and scores
    std::vector< Player * > players = model_->getPlayers();
    for (int i = 0; i < players.size(); i++ ) {
        // Fetch the widgets
        Gtk::Label * discards_label = nullptr;
        std::string id_discards = "player_" + std::to_string( i + 1 ) + "_discards";
        this->builder->get_widget( id_discards, discards_label );

        Gtk::Label * points_label = nullptr;
        std::string id_points = "player_" + std::to_string( i + 1 ) + "_points";
        this->builder->get_widget( id_points, points_label );

        if ( resetToZero ) {
            discards_label->set_text( "0 discards" );
            points_label->set_text( "0 points" );
        } else {
            discards_label->set_text( std::to_string( players[ i ]->fetchDiscards().size() ) + " discards" );
            points_label->set_text( std::to_string( players[ i ] ->oldScore() + players[ i ]->score() ) + " points" );
        }
    }
}
