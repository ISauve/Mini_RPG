#ifndef MINI_RPG_CONFIG_H
#define MINI_RPG_CONFIG_H

#include <json/json.h>
#include <fstream>
#include <iostream>

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 1024
#define PLAYER_WIDTH 80
#define PLAYER_HEIGHT 80

class Config {
    std::vector< Character > chars_;
    std::vector< Sprite > items_;

public:
    Config() = default;

    std::vector< Character > getChars() { return chars_; };
    std::vector< Sprite > getItems() { return items_; };

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

            // Store the character
            if ( !it["player"].isNull() ) {
                player = character;
                player.setPlayer(true);
            }
            else chars_.push_back(character);
        }

        // Put the player at the front
        // TODO: we have the "isPlayer" flag, no need to ensure player is at the front any more
        chars_.insert(chars_.begin(), player);

        // TODO: read items
    }
};

#endif //MINI_RPG_CONFIG_H
