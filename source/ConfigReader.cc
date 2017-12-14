#include "ConfigReader.h"

#include <json/json.h>
#include <fstream>
#include <iostream>

void ConfigReader::readConfig(std::string path) {
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

    // Read the characters
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
            int str = it["weapon"]["str"].asInt();
            int weight = it["weapon"]["weight"].asInt();
            std::string path = it["weapon"]["path"].asString();
            std::string activePath = it["weapon"]["activePath"].asString();

            // TODO
            //character.equipWeapon(new Weapon(str, weight, path, activePath));
        }

        // Store the character
        if ( !it["player"].isNull() ) {
            character.setPlayer(true);
            player_ = character;
        }
        else chars_.push_back(character);
    }

    // Read the props
    for (auto it : root["props"]) {
        // Read the required data
        std::string name = it["name"].asString();
        float x =  it["x"].asFloat();
        float y =  it["y"].asFloat();

        // TODO add any other instance-specific data (ie a chest's contents)

        props_.push_back(Prop::makeProp(name, x, y));
    }

    // TODO read the background
}
