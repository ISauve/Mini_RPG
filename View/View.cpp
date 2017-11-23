#include "View.h"

// TODO remove
#include <iostream>

View::View(Model* m, Controller* c) : model_(m), controller_(c), specialScreenShow_(false) {

    // Initialize the window (can be a local var because game ends when this returns)
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Izzy's OpenGL Adventure", sf::Style::Default);
    window_ = &window;

    // Register view as observer of model
    model_->subscribe( this );

    /*
    // Load music to play
    sf::Music music;
    music.openFromFile("nice_music.ogg");
    music.play();
     */

    // Start the event control loop - returns once window is closed
    runControlLoop();
};

/*/
 * This is the main control loop. It has 2 functions: draw the view, and poll for events.
 * When no events are occurring, it just repeatedly draws the view based on current data.
 * Once an event occurs:
 *      1. The event is passed off to the controller
 *      2. Controller interprets the event and calls changes in the model accordingly
 *      3. Model executes the changes, then calls notify()
 *      4. Notify() causes update() to be called in the view
 *      5. The view updates its data
 * Once an event calls for the window to close, this loop breaks and the program ends.
/*/
void View::runControlLoop() {
    while ( window_->isOpen() ) {
        // Check if an event has occurred
        sf::Event windowEvent;
        while ( window_->pollEvent(windowEvent) ) {     // pollEvent returns T while event is pending
            controller_->handleEvent(windowEvent);
        }

        // Check if window has been exited (via escape key or clicking X)
        if ( model_->gameOver() ) {
            window_->close();
            break;
        }

        Animation a(window_, model_, controller_);

        // Poll for information from the attack thread
        while ( !model_->enemyAttacks_.empty() ) {      // todo: need lockguard?
            Attack newAttack = model_->enemyAttacks_.front();
            model_->enemyAttacks_.pop();

            a.enemyAttack(newAttack.enemy, newAttack.damage);
        }
        if ( !model_->player()->isAlive() ) {       // Check if player was killed
            // Display the updated health before showing death
            window_->clear(sf::Color::Black);
            draw();

            a.playerDied();
            model_->reset();                        // TODO: this shouldn't be in the view...
        }

        // Draw the next frame
        window_->clear(sf::Color::Black);    // Clear the window with black color
        draw();                              // Draw the frame (based on current data)
        window_->display();                  // Update the screen to display current frame
    }
}

// Observer pattern: callback for when model notifies of a change in the game data
void View::update(Notification n) {
    Animation a(window_, model_, controller_);

    switch (n.type) {
        case PLAYER_ATTACK:
            // idea: only show sword swinging if weapon is wielded, otherwise show a slap (how?)
            a.playerAttack(n.hit, n.damage);
            break;
        case PLAYER_COLLISION:
            a.playerCollision();
            break;
        case ENEMY_DIED:


            window_->clear(sf::Color::Black);
            draw();
            break;
        case PLAYER_DIED:
            a.playerDied();
            break;
        case QUIT_GAME:
            a.quitGame();
            break;
        case PLAYER_CHANGE:
            specialScreenShow_ = true;
            specialScreen_ = SELECT_PLAYER;
            break;
        case EXIT_SPECIAL_SCREEN:
            specialScreenShow_ = false;
            break;
        default:
            break;
    }
};

// Draw the items that should always appear (characters, status)
void View::draw() {
    Animation a(window_, model_, controller_);
    if (specialScreenShow_) {
        switch (specialScreen_) {
            case SELECT_PLAYER:
                a.drawPlayerSelection();
                break;
            default:
                break;
        }
        return;
    }

    // Draw the background
    a.drawStatusBar();
    a.drawButtons();

    // Draw the sprites
    std::vector< Sprite > sprites;
    std::vector< Character > chars = model_->getChars();
    for (auto it : chars) sprites.push_back(it);
        // when we have more to draw (ie background items, weapons, etc) we can add them to sprites

    // Order gets determined by y coordinates of the bottom of the sprite
    std::sort (sprites.begin(), sprites.end(), [] (Sprite& a, Sprite& b) -> bool {
        float a_bottom = a.y() + a.height()/2;
        float b_bottom = b.y() + b.height()/2;

        return a_bottom < b_bottom;
    });
    for (auto it : sprites) {
        if (it.isOnCharSheet()) {
            sf::IntRect playerImage = a.getPlayerImage(it);
            a.drawSprite(it.x(), it.y(), it.path(), playerImage);
        } else {
            a.drawSprite(it.x(), it.y(), it.path());
        }

        if (it.hasWeapon()) {   // Can only be true for characters
            a.drawSprite(it.x() - it.width() / 2, it.y(), it.weaponPath());
        }
    }
}