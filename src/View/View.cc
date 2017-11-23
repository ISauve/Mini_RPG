#include "View.h"
#include <iostream>

View::View(Model* m, Controller* c) : model_(m), controller_(c), gameOver_(false) {
    // Initialize the window (can be a local var because game ends when this returns)
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Izzy's OpenGL Adventure", sf::Style::Default);

    window.setActive(false);        // OpenGL context can only be active in 1 thread at a time
    window_ = &window;
    controller_->setWindow(window_);

    // Register view as observer of model
    model_->subscribe( this );

    // Initialize the view data
    font_.loadFromFile("src/View/Old_School_Adventures.ttf");
    /*   TODO
    sf::Music music;
    music.openFromFile("nice_music.ogg");
    music.play();
     */
    specialScreen_ = NONE;

    // Launch the 2 main game loops
    std::thread render_thread(&View::render, this);
    controller_->handleEvents();

    // Wait for both threads before returning (both will return once the game is over)
    render_thread.join();
    window_->close();
};


void View::update(Notification n) {
    // Pass the notification into the events channel for the drawing thread to safely process
    eventsChannel_.send(n);
};