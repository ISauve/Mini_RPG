#include "Model/Model.h"
#include "Controller/Controller.h"
#include "View/View.h"
#include "ItemReader.h"

// Initialize the singleton ItemReader instance
ItemReader* ItemReader::instance_ = nullptr;

int main() {
    // Read in the item configuration files
    ItemReader::instance()->readItemConfigurations();

    // Initialize the main components of the game
    Model model;
    Controller controller( &model );
    View view( &model, &controller );   // returns when window exits

    return 0;
}