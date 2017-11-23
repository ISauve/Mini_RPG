#ifndef MVC_VIEW_H
#define MVC_VIEW_H

#include "Observer.h"
#include "Card.h"
#include <vector>
#include <algorithm>

#include <gtkmm.h>
#include <gtkmm/window.h>
#include <gtkmm/button.h>
#include <gtkmm/image.h>
#include <gtkmm/messagedialog.h>

class Controller;
class Model;

// View: how the game interacts with the user
//	-> for now, just reads from cin and prints to cout
// 	-> later, will have sub classes for the different components of the GUI
//	-> observes the model (& can query model for state information to print when necessary)

class View : public Gtk::Window, public Observer {

public:
	View();
	View( Controller * , Model * );
	bool readInput();
	bool hasGameWithHumanPlayers() const { return gameIncludesHumanPlayers_; };

private:
	// Observer Pattern: to access Model accessors without having to downcast subject
	Model * model_;

	// Strategy Pattern member (plus signal handlers)
	Controller * controller_;

	// Member to indicate if any human players exist
	bool gameIncludesHumanPlayers_;

	// Observer Pattern: concrete update() method which subject calls
	virtual void update() override;

	// Helper functions
	void promptNextPlayerSelection();
	void initialSetup();
	void resetTableCards();
	std::string getCardPath( Card * card ) const;
	void updateDiscardsAndScoresCount(bool resetToZero = false);
	void clearPlayerHand();

	// Display functions
	void displayGameView();
	void displayNewTurn();
	void displayGameEnd();
	void displayNewRoundBegins();
	void displayEndOfRound();

	// Signal handlers
	void onComputerButtonClicked();
	void onHumanButtonClicked();
	void onNewGameButtonClicked();
	void onPlayerCardClicked( unsigned int );
	void onRageQuitClicked( int );
	void onQuitGameButtonClicked();

	// Member widgets
	Glib::RefPtr< Gtk::Builder > builder;
	Gtk::Label text;
	Gtk::ButtonBox main_box;
	Gtk::Button c_player_button;
	Gtk::Button h_player_button;
	Gtk::Box * game_view;
	Gtk::Entry * text_entry;
};

#endif
