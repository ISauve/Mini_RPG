#ifndef MINI_RPG_CONFIG_H
#define MINI_RPG_CONFIG_H

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 1024

#include "Model/Sprites/Character.h"
#include "Model/Sprites/Prop.h"
#include <vector>

// Reads configuration files for game areas
class ConfigReader {
    Character player_;
    std::vector< Character > chars_;
    std::vector< Prop > props_;

public:
    explicit ConfigReader() = default;

    void readConfig(std::string path);

    Character getPlayer() { return player_; };
    std::vector< Character > getChars() { return chars_; };
    std::vector< Prop > getProps() { return props_; };
};

#endif //MINI_RPG_CONFIG_H
