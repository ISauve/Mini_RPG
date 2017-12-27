#ifndef MINI_RPG_CONFIG_H
#define MINI_RPG_CONFIG_H

#define TOP_BAR_HEIGHT 150
#define SCREEN_WIDTH 2048   // 32 tiles
#define SCREEN_HEIGHT 1174  // 16 tiles

#include "Model/Sprites/Character.h"
#include "Model/Sprites/Prop.h"
#include <vector>
#include <yaml-cpp/yaml.h>

// Reads configuration files for game areas
class ConfigReader {
    Character player_;
    std::vector< Character > chars_;
    std::vector< Prop > props_;
    std::string background_;

public:
    explicit ConfigReader() = default;

    void readConfig(std::string path);

    Character getPlayer() { return player_; };
    std::vector< Character > getChars() { return chars_; };
    std::vector< Prop > getProps() { return props_; };
    std::string getBackground() { return background_; };
};

#endif //MINI_RPG_CONFIG_H
