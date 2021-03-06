#ifndef MINI_RPG_ITEMREFERENCE_H
#define MINI_RPG_ITEMREFERENCE_H

#include <string>
#include <iostream>

// Item Reference class, holds all the stats relating to a particular type of item
class ItemReference {
    /**** Required for all items ****/

    // Identifiers
    std::string name_;
    std::string type_;

    // Stats
    int dropChance_;
    int cost_;

    // Image details
    std::string path_;
    bool sheet_;
    int row_;
    int col_;
    int width_;
    int height_;

    /**** Not required for all items ****/

    // Animation
    std::string activePath_;        // todo: create an animation class
    std::string activePathR_;

    // Weapon/armor stats
    std::string pathR_;
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
            activePath_(""), activePathR_(""), pathR_(""), str_(0), def_(0), wght_(0), bonusHealth_(0),
            healing_(0), ability_(""), value_(0), acquirable_(false) {};

    // Need both default & copy ctors so that ItemRefs can be stored in a map
    ItemReference() :
            name_(""),  type_(""),  dropChance_(0),  cost_(0),
            path_(""),  sheet_(false),  row_(0),  col_(0), width_(0), height_(0),
            activePath_(""), activePathR_(""), pathR_(""), str_(0), def_(0), wght_(0), bonusHealth_(0),
            healing_(0), ability_(""), value_(0), acquirable_(false) {};
    ItemReference(const ItemReference& i) :
            name_(i.name()),  type_(i.type()),  dropChance_(i.dropChance()),  cost_(i.cost()),
            path_(i.path()),  sheet_(i.sheet()),  row_(i.row()),  col_(i.col()), width_(i.width()), height_(i.height()),
            activePath_(i.activePath()), activePathR_(i.activePathR()), pathR_(i.pathR()), str_(i.strength()),
            def_(i.defense()), wght_(i.weight()), bonusHealth_(i.bonusHealth()), healing_(i.healing()),
            ability_(i.ability()), value_(i.value()), acquirable_(i.acquirable()) {};

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
    std::string activePathR() const { return activePathR_; };
    std::string pathR() const { return pathR_; };
    int strength() const { return str_; };
    int defense() const { return def_; };
    int weight() const { return wght_; };
    int bonusHealth() const { return bonusHealth_; };

    int healing() const { return healing_; };
    std::string ability() const { return ability_; };
    int value() const { return value_; };
    bool acquirable() const { return acquirable_; };

    // Modifiers for the optional data
    // These should only be called when the item reference is being initialized
    void setActivePath(std::string a) { activePath_ = a; };
    void setActivePathR(std::string a) { activePathR_ = a; };
    void setPathR(std::string p) { pathR_ = p; };
    void setStrength(int s) { str_ = s; };
    void setDefense(int d) { def_ = d; };
    void setWeight(int w) { wght_ = w; };
    void setBonusHealth(int b) { bonusHealth_ = b; };
    void setHealing(int h) { healing_ = h; };
    void setAbility(std::string a) { ability_ = a; };
    void setValue(int v) { value_ = v; };
    void setAcquirable(bool a) { acquirable_ = a; };

    // for debugging
    void print() {
        using namespace std;

        cout << "Printing item reference for " << name_ << endl;
        cout << "\t Type: " << type_ << endl;
        cout << "\t Drop chance: " << dropChance_ << endl;
        cout << "\t Acquirable: " << acquirable_ << endl;
        cout << "\t Image " << endl;
        cout << "\t\t Path: " << path_ << endl;
        cout << "\t\t Path (r): " << pathR_ << endl;
        cout << "\t\t Sheet: " << sheet_ << endl;
        cout << "\t\t Row: " << row_ << endl;
        cout << "\t\t Col: " << col_ << endl;
        cout << "\t\t Width: " << width_ << endl;
        cout << "\t\t Height: " << height_ << endl;
        cout << "\t Active path: " << activePath_ << endl;
        cout << "\t Active path (r): " << activePathR_ << endl;
        cout << "\t Strength: " << str_ << endl;
        cout << "\t Strength: " << str_ << endl;
        cout << "\t Defence: " << def_ << endl;
        cout << "\t Weight: " << wght_ << endl;
        cout << "\t Bonus health: " << bonusHealth_ << endl;
        cout << "\t Healing: " << healing_ << endl;
        cout << "\t Value: " << value_ << endl;
        cout << "\t Ability: " << ability_ << endl;
    }
};


#endif //MINI_RPG_ITEMREFERENCE_H
