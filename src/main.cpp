#include "Model/Model.h"
#include "Controller/Controller.h"
#include "View/View.h"

int main( int argc, char * argv[] ) {

    // Initialize the main components of the game
    Model model;
    Controller controller( &model );
    View view( &model, &controller );      // Ctor draws the first window & begins event loop

    return 0;
}