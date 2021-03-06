#ifndef MINI_RPG_REFERENCE_H
#define MINI_RPG_REFERENCE_H

#include "ItemReference.h"
#include <yaml-cpp/yaml.h>
#include <string>
#include <map>
#include <dirent.h>

#include <iostream>

// Reads configuration files for items & stores references for them
// Singleton design pattern: one of these is created when the game starts, & that's it
class ItemReader {
    static ItemReader* instance_;
    std::map<std::string, ItemReference> references_;

    ItemReader() = default;
    ItemReader(ItemReader const&){};            // copy ctor
    ItemReader&operator=(ItemReader const&){};  // assignment operator
public:
    static ItemReader* instance() {
        if (!instance_) instance_ = new ItemReader;
        return instance_;
    };

    ItemReference* getReference(std::string itemName) {
        return &(references_[itemName]);
    }

    // Fills the references_ map
    void readItemConfigurations() {
        // Fetch the paths to all the Item configuration files
        auto directory = opendir("assets/Items");
        std::vector<std::string> confPaths;
        while (auto file = readdir(directory)) {
            std::string name = file->d_name;
            if (name.substr(name.find_last_of(".") + 1) != "yaml") continue;
            confPaths.push_back("assets/Items/" + name);
        }

        for (auto path : confPaths) {
            YAML::Node config = YAML::LoadFile(path);
            std::string name = config["name"].as<std::string>();

            // Initialize the reference with the required data
            ItemReference ref(name,
                          config["type"].as<std::string>(),
                          config["drop_chance"].as<int>(),
                          config["cost"].as<int>(),
                          config["image"]["path"].as<std::string>(),
                          config["image"]["sheet"].as<bool>(),
                          config["image"]["row"].as<int>(),
                          config["image"]["col"].as<int>(),
                          config["image"]["w"].as<int>(),
                          config["image"]["h"].as<int>());

            // Add any extra data
            if (config["acquirable"]) ref.setAcquirable(config["acquirable"].as<bool>());
            if (config["effect"]["healing"]) ref.setHealing(config["effect"]["healing"].as<int>());
            if (config["effect"]["value"]) ref.setValue(config["effect"]["value"].as<int>());

            // Animations should be dealt with in a better way  TODO
            if (config["animations"]["attack"]["activePath"]) {
                ref.setActivePath(config["animations"]["attack"]["activePath"].as<std::string>());
            }
            if (config["animations"]["attack"]["activePathR"]) {
                ref.setActivePathR(config["animations"]["attack"]["activePathR"].as<std::string>());
            }

            if (config["image"]["pathR"]) ref.setPathR(config["image"]["pathR"].as<std::string>());
            if (config["strength"]) ref.setStrength(config["strength"].as<int>());
            if (config["weight"]) ref.setWeight(config["weight"].as<int>());

            //   defense
            //   bonus health
            //   other bonus features
            //   ability

            references_[name] =  ref;

            // For debugging
            //ref.print();
        }
    };

};

#endif //MINI_RPG_REFERENCE_H
