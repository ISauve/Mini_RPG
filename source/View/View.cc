#include "View.h"
#include "../Model/Model.h"
#include "../Controller/Controller.h"
#include "../ConfigReader.h"

View::View(Model* m, Controller* c) : model_(m), controller_(c), gameOver_(false) {
    // Initialize the window (can be a local var because game ends when this returns)
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Izzy's OpenGL Adventure", sf::Style::Titlebar | sf::Style::Close);
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

    //bool closedWindow = animateSplashScreen();
    //if (closedWindow) return;

    // Launch the 3 main game loops
    window.setActive(false);    // OpenGL context can only be active in 1 thread at a time
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
    auto directory = opendir("resources/Textures");
    std::vector<std::string> paths;
    while (auto file = readdir(directory)) {
        std::string name = file->d_name;
        if (name.substr(name.find_last_of(".") + 1) != "png") continue;
        paths.push_back("resources/Textures/" + name);
    }

    for (auto path : paths) {
        sf::Image image;
        image.loadFromFile(path);
        imageCache_[path] = image;
    }
};

bool View::animateSplashScreen() {
    auto start = std::chrono::high_resolution_clock::now();

    sf::Texture bgTexture;
    bgTexture.loadFromImage(imageCache_["resources/Textures/splash_screen.png"]);

    sf::Texture titleTexture;
    titleTexture.loadFromImage(imageCache_["resources/Textures/splash_screen_title.png"]);

    int transparency = 0;
    while (window_->isOpen()){
        sf::Event event;
        while (window_->pollEvent(event)){
            if (event.type == sf::Event::EventType::Closed) {
                window_->close();
                return true;
            }
        }

        auto now = std::chrono::high_resolution_clock::now();
        auto timePassed = std::chrono::duration_cast<std::chrono::seconds>(now - start);

        if (timePassed.count() > 2) return false;

        window_->clear(sf::Color::Black);

        // Draw the background
        sf::Sprite sprite;
        sprite.setTexture(bgTexture);
        sprite.setPosition(sf::Vector2f(0, 0));
        window_->draw(sprite);

        // After 0.5s, fade in the title
        if (timePassed.count() >= 0.5) {
            sf::Sprite sprite;
            sprite.setTexture(titleTexture);
            sprite.setColor(sf::Color(255, 255, 255, transparency));
            sprite.setPosition(sf::Vector2f(0, 0));
            window_->draw(sprite);

            if (transparency < 255) {
                transparency += 3;
                if (transparency > 255) transparency = 255;
            }
        }

        window_->display();
    }
    return true;
}