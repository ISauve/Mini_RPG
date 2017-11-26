#include "Model/Model.h"
#include "Controller/Controller.h"
#include "View/View.h"

int main() {
    // Read in the confs file to the "conf" class todo

    // Initialize the main components of the game
    Model model;
    Controller controller( &model );
    View view( &model, &controller );   // returns when window exits

    return 0;
}