#ifndef MINI_RPG_CONFIG_H
#define MINI_RPG_CONFIG_H

#include "Model/Character.h"
#include "Model/Item.h"
#include <json/json.h>
#include <fstream>
#include <iostream>

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 1024

class Config {
    Character player_;
    std::vector< Character > chars_;
    std::vector< Item > items_;

public:
    Config() = default;

    Character getPlayer() { return player_; };
    std::vector< Character > getChars() { return chars_; };
    std::vector< Item > getItems() { return items_; };

    void readConfig(std::string path) {
        // Read the config file
        std::ifstream infile( path );
        if ( not infile.is_open() ) { // Ensure file successfully opened
            std::cerr << "Failed to open file \"" << path << "\".\n";
            return;
        }

        // Parse the file
        Json::Value root;
        Json::CharReaderBuilder rbuilder;
        std::string errors;
        bool parsedOk = Json::parseFromStream( rbuilder, infile, &root, &errors );
        if (!parsedOk) {
            std::cerr << "Error in parsing JSON: " << errors << "\n";
            return;
        }

        // Read the characters from the config
        Character player;
        for (auto it : root["characters"]) {
            // Initialize char w/ the required data
            int str = it["str"].asInt();
            int sp = it["sp"].asInt();
            float x = it["x"].asFloat();
            float y = it["y"].asFloat();
            bool sheet = it["sheet"].asBool();
            std::string path = it["path"].asString();
            int w = it["w"].asInt();
            int h = it["h"].asInt();

            Character character = Character(str, sp, x, y, sheet, path, w, h);

            // Add the optional data
            if (!it["active"].isNull()) character.setActiveEnemy(it["active"].asBool());
            if (!it["row"].isNull() && !it["col"].isNull()) character.setCharacter(it["row"].asInt(), it["col"].asInt());


            if (!it["weapon"].isNull()) {
                float x = it["weapon"]["x"].asFloat();
                float y = it["weapon"]["y"].asFloat();
                std::string path = it["weapon"]["path"].asString();
                int w = it["weapon"]["w"].asInt();
                int h = it["weapon"]["h"].asInt();

                character.equipWeapon(new Weapon(x, y, x + w/2,  y, false, path, w, h));
            }

            // Store the character
            if ( !it["player"].isNull() ) {
                character.setPlayer(true);
                player_ = character;
            }
            else chars_.push_back(character);
        }

        // Read items from the config
        for (auto it : root["items"]) {
            // Initialize item w/ the required data
            auto t = static_cast<Item::Type>(it["type"].asInt());
            float x = it["x"].asFloat();
            float y = it["y"].asFloat();
            bool sheet = it["sheet"].asBool();
            std::string path = it["path"].asString();
            int w = it["w"].asInt();
            int h = it["h"].asInt();

            Item item = Item(t, x, y, sheet, path, w, h);

            items_.push_back(item);
        }

        // TODO read the background
    }
};

#endif //MINI_RPG_CONFIG_H