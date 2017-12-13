#include "View.h"
#include "../Model/Model.h"
#include "../Controller/Controller.h"
#include "../ConfigReader.h"

View::View(Model* m, Controller* c) : model_(m), controller_(c), gameOver_(false) {
    // Initialize the window (can be a local var because game ends when this returns)
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Izzy's OpenGL Adventure", sf::Style::Default);

    window.setActive(false);        // OpenGL context can only be active in 1 thread at a time
    window_ = &window;
    controller_->setWindow(window_);

    // Register view as observer of model
    model_->subscribe( this );

    // Initialize the view data
    loadImages();
    font_.loadFromFile("resources/Old_School_Adventures.ttf");
    sf::Music music;
    music.openFromFile("resources/AMemoryAway.ogg");
    music.play();
    viewState_ = PLAYING;

    // Launch the 3 main game loops
    std::thread render_thread(&View::render, this);
    std::thread game_thread(&Model::startGameLoop, model_);
    // Note: "main" thread has to be the controller because SFML does not allow window
    // event processing in threads (only rendering)
    controller_->handleEvents();

    // Wait for all threads before exiting (will return once the game is over)
    render_thread.join();
    game_thread.join();
    window_->close();
};

void View::update(Notification n) {
    // Pass the notification into the events channel for the drawing thread to safely process
    eventsChannel_.send(n);
};

void View::loadImages() {
    // TODO read all the images present in the Textures directory
    std::vector<std::string> paths = {"resources/Textures/Character_set_5.png",
                                       "resources/Textures/Character_set_2.png",
                                       "resources/Textures/Enemy_1.png",
                                       "resources/Textures/null.png",
                                       "resources/Textures/Enemy_Sword.png",
                                       "resources/Textures/Enemy_Sword_Active.png",
                                       "resources/Textures/Skull_crossbones.png",
                                       "resources/Textures/scratch.png",
    };

    for (auto path : paths) {
        sf::Image image;
        image.loadFromFile(path);
        imageCache_[path] = image;
    }
};