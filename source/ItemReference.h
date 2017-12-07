#ifndef MINI_RPG_ITEMREFERENCE_H
#define MINI_RPG_ITEMREFERENCE_H

#include <string>

// Item Reference class, holds all the stats relating to a particular type of item
class ItemReference {
    /**** Required for all items ****/

    // Identifiers
    const std::string name_;
    const std::string type_;

    // Stats
    const int dropChance_;
    const int cost_;

    // Image details
    const std::string path_;
    const bool sheet_;
    const int row_;
    const int col_;
    const int width_;
    const int height_;

    /**** Not required for all items ****/

    // Animation
    std::string activePath_;        // todo: create an animation class

    // Weapon/armor stats
    int str_;
    int def_;
    int wght_;
    int bonusHealth_;
    // other bonus features

    // Other
    int healing_;           // for items/props
    std::string ability_;   // for items/props
    int value_;             // for money

    // Props
    bool acquirable_;

public:
    ItemReference(std::string name, std::string type, int drop, int cost,
            std::string path, bool sheet, int row, int col, int width, int height) :

            // initialize required data
            name_(name),  type_(type),  dropChance_(drop),  cost_(cost),
            path_(path),  sheet_(sheet),  row_(row),  col_(col), width_(width), height_(height),

            // initialize optional data
            activePath_(""), str_(0), def_(0), wght_(0), bonusHealth_(0), healing_(0), ability_(""),
            value_(0), acquirable_(false) {};

    // Need both default & copy ctors so that ItemRefs can be stored in a map
    ItemReference() :
            name_(""),  type_(""),  dropChance_(0),  cost_(0),
            path_(""),  sheet_(false),  row_(0),  col_(0), width_(0), height_(0),
            activePath_(""), str_(0), def_(0), wght_(0), bonusHealth_(0), healing_(0), ability_(""),
            value_(0), acquirable_(false) {};

    ItemReference(ItemReference const& i) :
            name_(i.name()),  type_(i.type()),  dropChance_(i.dropChance()),  cost_(i.cost()),
            path_(i.path()),  sheet_(i.sheet()),  row_(i.row()),  col_(i.col()), width_(i.width()), height_(i.height()),
            activePath_(i.activePath()), str_(i.strength()), def_(i.defense()), wght_(i.weight()),
            bonusHealth_(i.bonusHealth()), healing_(i.healing()), ability_(i.ability()), value_(i.value()),
            acquirable_(i.acquirable()) {};

    // Accessors
    std::string name() const { return name_; };
    std::string type() const { return type_; };

    int dropChance() const { return dropChance_; };
    int cost() const { return cost_; };

    std::string path() const { return path_; };
    bool sheet() const { return sheet_; };
    int row() const { return row_; };
    int col() const { return col_; };
    int width() const { return width_; };
    int height() const { return height_; };

    std::string activePath() const { return activePath_; };

    int strength() const { return str_; };
    int defense() const { return def_; };
    int weight() const { return wght_; };
    int bonusHealth() const { return bonusHealth_; };

    int healing() const { return healing_; };
    std::string ability() const { return ability_; };
    int value() const { return value_; };
    bool acquirable() const { return acquirable_; };

    // Modifiers for the optional data
    // TODO
};


#endif //MINI_RPG_ITEMREFERENCE_H
