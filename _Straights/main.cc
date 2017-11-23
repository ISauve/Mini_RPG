// TODO remove gtkmm-3.0
#include <gtkmm-3.0/gtkmm/main.h>

#include "Model.h"
#include "Controller.h"
#include "View.h"


int main( int argc, char * argv[] ) {
	Gtk::Main kit( argc, argv );         	// Initialize gtkmm with the command line arguments

	Model model;                          	// Create model
	Controller controller( & model ); 	  	// Create controller
	View view( & controller, & model );     	// Create the view -- is passed handle to controller and model

	Gtk::Main::run( view );               	// Shows the game window and returns when it's closed

	return 0;		// controller deletes model when it goes out of scope, view deletes only itself
} // main