#include "ConfigReader.h"
#include "Model/Tools/Weapon.h"

#include <iostream>

void ConfigReader::readConfig(std::string path) {
    YAML::Node config = YAML::LoadFile(path);

    // Read the characters
    Character player;
    for (auto it : config["characters"]) {
        // Initialize char w/ the required data
        int str = it["str"].as<int>();
        int sp = it["sp"].as<int>();
        float x = it["x"].as<float>();
        float y = it["y"].as<float>();
        bool sheet = it["sheet"].as<bool>();;
        std::string path = it["path"].as<std::string>();
        int w = it["w"].as<int>();
        int h = it["h"].as<int>();

        Character character = Character(str, sp, x, y, sheet, path, w, h);

        // Add the optional data
        if (it["active"]) character.setActiveEnemy(it["active"].as<bool>());
        if (it["row"] && it["col"]) character.setSheetPosition(it["row"].as<int>(), it["col"].as<int>());
        if (it["weapon"]) character.equipWeapon(Weapon::makeWeapon(it["weapon"].as<std::string>()));

        // Store the character
        if ( it["player"] ) {
            character.setPlayer(true);
            player_ = character;
        }
        else chars_.push_back(character);
    }

    // Read the props
    for (auto it : config["props"]) {
        // Read the required data
        std::string name = it["name"].as<std::string>();
        float x =  it["x"].as<float>();
        float y =  it["y"].as<float>();

        // TODO add any other instance-specific data (ie a chest's contents)

        props_.push_back(Prop::makeProp(name, x, y));
    }

    // TODO read the background
}
