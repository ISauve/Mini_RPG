#include "View.h"
#include "../Controller/Controller.h"
#include "../Model/Model.h"
#include "../ConfigReader.h"

static int counter = 0;
sf::IntRect View::getPlayerImage(Sprite& s) {
    int row = s.row();
    int col = s.col();
    int width = s.width();
    int height = s.height();

    // Select the rectangle for the "default" stance
    sf::IntRect player_img = sf::IntRect(col*width, row*height, width, height);

    // Animate player movement by changing the image if a key is pressed
    bool animate = false;
    if ( sf::Keyboard::isKeyPressed(sf::Keyboard::Left) ) {
        row = row+1; animate = true;
    } else if ( sf::Keyboard::isKeyPressed(sf::Keyboard::Right) ) {
        row = row+2; animate = true;
    } else if ( sf::Keyboard::isKeyPressed(sf::Keyboard::Up) ) {
        row = row+3; animate = true;
    } else if ( sf::Keyboard::isKeyPressed(sf::Keyboard::Down) ) {
        animate = true; }

    if (animate) {
        if (counter < 5)                            // 0-4
            player_img = sf::IntRect( (col-1)*width, row*height, width, height );
        else if (counter < 10 || counter >= 15)     // 5-9, 15-19
            player_img = sf::IntRect( (col)*width, row*height, width, height );
        else                                        // 10-14
            player_img = sf::IntRect( (col+1)*width, row*height, width, height );

        counter++; if (counter == 20) counter = 0;
    }

    return player_img;
}

sf::IntRect View::getSelection(Sprite& s) {
    int row = s.row();
    int col = s.col();
    int width = s.width();
    int height = s.height();

    return sf::IntRect(col*width, row*height, width, height);
}

void View::drawPlayerWeapon(Character& c) {
    if ( sf::Keyboard::isKeyPressed(sf::Keyboard::Right) ) {
        drawSprite(c.x() + c.width() / 2, c.y(), c.weaponPathR());
        return;
    }

    drawSprite(c.x() - c.width() / 2, c.y(), c.weaponPath());
}

void View::drawActivePlayerWeapon(Character& c) {
    if ( sf::Keyboard::isKeyPressed(sf::Keyboard::Right) ) {
        drawSprite(c.x() + c.width() / 2, c.y(), c.activeWeaponPathR());
        return;
    }

    drawSprite(c.x() - c.width() / 2, c.y(), c.activeWeaponPath());
}

void View::drawEvent(Notification event) {
    switch (event.type) {
        case Notification::PLAYER_COLLISION:
            drawText(25, sf::Color::White, "OPE", false, sf::Vector2f(model_->player().x()+50, model_->player().y()-50));
            break;

        case Notification::PLAYER_ATTACK:
            if (model_->player().hasWeapon()){
                auto player = model_->player();
                drawActivePlayerWeapon(player);
            }

            if (event.hit) {   // Draw a hit above the enemy
                drawText(25, sf::Color::Black, std::to_string(event.damage), false, sf::Vector2f(event.enemy.x(), event.enemy.y() - 100));
            }
            else {      // Draw a miss above the player's sword
                drawText(25, sf::Color::White, "Miss", false, sf::Vector2f(model_->player().x() + 50, model_->player().y() - 100));
            }
            break;

        case Notification::ENEMY_ATTACK:
            // Draw the weapon as active
            if (event.enemy.hasWeapon()) {
                drawSprite(event.enemy.x() - event.enemy.width() / 2, event.enemy.y(), event.enemy.activeWeaponPath());
            }

            // Draw the damage above the player's head
            drawText(25, sf::Color::Red, std::to_string(event.damage), false, sf::Vector2f(model_->player().x(), model_->player().y()-70));
            break;

        case Notification::ENEMY_DIED:
            // Draw a crossbones where the enemy was
            drawSprite(event.enemy.x(), event.enemy.y(), "assets/Textures/Skull_crossbones.png");
            break;

        case Notification::HEALED:
            // Draw the amount healed above the player's head
            drawText(25, sf::Color::Green, "+" + std::to_string(event.healed), false, sf::Vector2f(model_->player().x()-50, model_->player().y()-80));
            break;

        case Notification::GOT_MONEY:
            // Draw the amount acquired above the player's head
            drawText(25, sf::Color::Yellow, "+" + std::to_string(event.value), false, sf::Vector2f(model_->player().x()+50, model_->player().y()-80));
            break;

        default:
            break;
    }
}

void View::drawTopBar() {
    controller_->clearActiveButtons();

    // Health
    auto emptyBar = drawSprite(50, 50, "assets/Textures/empty_bar.png", sf::IntRect(), false);
    int width = 425;
    int height = 57;
    auto croppedImage = sf::IntRect(0, 0, model_->player().health() * width / 100, height);
    drawSprite(50, 50, "assets/Textures/green_bar.png", croppedImage, false);

    // Money
    drawSprite(50 + emptyBar.width + 50, 30, "assets/Textures/coin_pile.png", sf::IntRect(), false);
    sf::Text money = generateText(25, sf::Color::White, std::to_string(model_->playerMoney()), false);
    money.setPosition(sf::Vector2f(50 + emptyBar.width + 125, 60));
    window_->draw(money);

    // Backpack button
    sf::FloatRect backpack = drawSprite(50 + emptyBar.width + 240, 75, "assets/Textures/pack.png");
    controller_->addActiveButton(Controller::VIEW_STATS, backpack);

    // Quick-access items
    sf::FloatRect slots = drawSprite(backpack.left + backpack.width + 50, 25, "assets/Textures/slots.png", sf::IntRect(), false);
    std::vector<Prop> quickAccessItems = model_->player().quickAccessContents();
    for (int i=0; i < int(quickAccessItems.size()); i++) {
        auto it = quickAccessItems[i];
        if (it.isOnSheet()) {
            sf::IntRect playerImage = getSelection(it);
            sf::FloatRect item = drawSprite(it.x(), it.y(), it.path(), playerImage);
            controller_->addActiveButton(static_cast<Controller::Button>(i), item);
        } else {
            sf::FloatRect item =  drawSprite(it.x(), it.y(), it.path());
            controller_->addActiveButton(static_cast<Controller::Button>(i), item);
        }
    }

    // Change character
    sf::RectangleShape change = drawRectangle(160, 65, sf::Color(52, 152, 219),
                                              // middle of slots = 81.5 from the top
                                              sf::Vector2f(slots.left + slots.width + 50, 46.5), 10,
                                              sf::Color(41, 128, 185), false);
    drawText(17, sf::Color::White, "   Change\ncharacter", false,
             sf::Vector2f(slots.left + slots.width + 64, 58));
    controller_->addActiveButton(Controller::CHANGE_PLAYER, change.getGlobalBounds());

    // Reset
    sf::RectangleShape reset = drawRectangle(100, 45, sf::Color(52, 152, 219),
                                             sf::Vector2f(change.getGlobalBounds().left + change.getGlobalBounds().width + 50, 56.5), 10,
                                             sf::Color(41, 128, 185), false);
    drawText(17, sf::Color::White, "Reset", false,
             sf::Vector2f(change.getGlobalBounds().left + change.getGlobalBounds().width + 64, 71.5));
    controller_->addActiveButton(Controller::RESET, reset.getGlobalBounds());
}

void View::drawBackground() {
    std::string background = model_->getBackground();

    drawSprite(0, TOP_BAR_HEIGHT, background, sf::IntRect(), false);
}

void View::drawPlayerSelection() {
    std::string sheet_path = "assets/Textures/Character_set_2.png";
    drawText(30, sf::Color::White, "Select your character", false, sf::Vector2f(50, 50));

    sf::IntRect selection = sf::IntRect(1*80, 0*80, 80, 80);
    sf::FloatRect rectangle_1 = drawSprite(100, 200, sheet_path, selection);
    drawText(25, sf::Color::White, "Business-y Bob: Not sure how he ended up here. Is late for a meeting.",
             false, sf::Vector2f(175, 200));

    selection = sf::IntRect(4*80, 0*80, 80, 80);
    sf::FloatRect rectangle_2 = drawSprite(100, 400, sheet_path, selection);
    drawText(25, sf::Color::White, "Beeb (generic): Very cuddly & loves back massages.",
             false, sf::Vector2f(175, 400));

    selection = sf::IntRect(1*80, 4*80, 80, 80);
    sf::FloatRect rectangle_3 = drawSprite(100, 600, sheet_path, selection);
    drawText(25, sf::Color::White, "Monsieur Moustache: A classic hero, this great lad is prepared for all sorts of adventure.",
             false, sf::Vector2f(175, 600));

    selection = sf::IntRect(4*80, 4*80, 80, 80);
    sf::FloatRect rectangle_4 = drawSprite(100, 800, sheet_path, selection);
    drawText(25, sf::Color::White, "Jean-Pierre-Marie-Cloud: Has never been seen without his beret, and scoffs at anyone \nwho says 'Chocolatine'. It's Pain au Chocolate... putain.",
             false, sf::Vector2f(175, 775));

    selection = sf::IntRect(7*80, 4*80, 80, 80);
    sf::FloatRect rectangle_5 = drawSprite(100, 1000, sheet_path, selection);
    drawText(25, sf::Color::White, "Gerard: A black belt, but a pacifist at heart. One time he almost got in a fight, when someone \nsaid his kitten wasn't THAT cute.",
             false, sf::Vector2f(175, 975));

    controller_->clearActiveButtons();
    controller_->addActiveButton(Controller::SELECT_PLAYER_1, rectangle_1);
    controller_->addActiveButton(Controller::SELECT_PLAYER_2, rectangle_2);
    controller_->addActiveButton(Controller::SELECT_PLAYER_3, rectangle_3);
    controller_->addActiveButton(Controller::SELECT_PLAYER_4, rectangle_4);
    controller_->addActiveButton(Controller::SELECT_PLAYER_5, rectangle_5);
}

void View::drawQuitScreen() {
    drawText(50, sf::Color::Cyan, "Quitting...", true, sf::Vector2f(SCREEN_WIDTH/2, SCREEN_HEIGHT/2));
};

void View::drawPlayerDied() {
    drawText(35, sf::Color::Red, "You are dead!", true, sf::Vector2f(SCREEN_WIDTH/2, SCREEN_HEIGHT/2));

    sf::RectangleShape r = drawRectangle(100, 50, sf::Color(52, 152, 219), sf::Vector2f(100, SCREEN_HEIGHT - 75), 10, sf::Color(41, 128, 185));
    drawText(17, sf::Color::White, "Reset", true, sf::Vector2f(100, SCREEN_HEIGHT - 75));

    controller_->clearActiveButtons();
    controller_->addActiveButton(Controller::RESET, r.getGlobalBounds());
};

void View::drawViewStats() {
    // Draw the player's image @ 3x size
    Character player = model_->player();
    sf::IntRect selection = getSelection(player);
    sf::FloatRect bounds = drawSprite(200, 200, player.path(), selection, false, 3);

    // Add stats
    drawText(25, sf::Color::White, "Player Stats", true,
             sf::Vector2f(bounds.left + bounds.width/2, bounds.top +  bounds.height + 100));
    drawText(25, sf::Color::White, "Strength: " + std::to_string(model_->player().strength()), true,
             sf::Vector2f(bounds.left + bounds.width/2, bounds.top +  bounds.height + 150));
    drawText(25, sf::Color::White, "Speed: " + std::to_string(model_->player().speed()), true,
             sf::Vector2f(bounds.left + bounds.width/2, bounds.top + bounds.height + 200));

    // Draw the player's weapon @ 3x size
    if (player.hasWeapon()) {
        bounds = drawSprite(700, 200, player.weaponPath(), sf::IntRect(), false, 3);

        // Add stats
        drawText(25, sf::Color::White, model_->player().weaponName(), true,
                 sf::Vector2f(bounds.left + bounds.width/2, bounds.top +  bounds.height + 100));
        drawText(25, sf::Color::White, "Strength: " + std::to_string(model_->player().weaponStrength()), true,
                 sf::Vector2f(bounds.left + bounds.width/2, bounds.top +  bounds.height + 150));
        drawText(25, sf::Color::White, "Weight: " + std::to_string(model_->player().weaponWeight()), true,
                 sf::Vector2f(bounds.left + bounds.width/2, bounds.top + bounds.height + 200));
    } else {
        drawText(25, sf::Color::White, "No weapon equipped", true, sf::Vector2f(800, SCREEN_HEIGHT/2));
    }

    // Item slots
    sf::FloatRect big_slots = drawSprite(SCREEN_WIDTH - 900, 200,
                                         "assets/Textures/big_slots.png", sf::IntRect(), false);
    drawText(25, sf::Color::White, "Backpack", true, sf::Vector2f(big_slots.left + big_slots.width/2, big_slots.top - 50));
    sf::FloatRect slots = drawSprite(SCREEN_WIDTH - 900, 700,
                                     "assets/Textures/slots.png", sf::IntRect(), false);
    drawText(25, sf::Color::White, "Belt", true, sf::Vector2f(slots.left + slots.width/2, slots.top - 50));

    // Items
    std::vector<Prop> items = model_->player().items();
    for (int i=0; i < int(items.size()); i++) {
        auto it = items[i];
        if (it.isOnSheet()) {
            sf::IntRect selection = getSelection(it);
            sf::FloatRect item = drawSprite(it.x(), it.y(), it.path(), selection);
        } else sf::FloatRect item =  drawSprite(it.x(), it.y(), it.path());

        // Add button for each item:    TODO
        //      - on hover, display item's name/stats
        //      - can click & drag item to/from quick slots
    }

    // Draw "return" button
    sf::FloatRect arrow = drawSprite(100, SCREEN_HEIGHT - 75, "assets/Textures/return.png");
    controller_->clearActiveButtons();
    controller_->addActiveButton(Controller::EXIT_SPECIAL_SCREEN, arrow);
}