#ifndef MINI_RPG_TOOL_H
#define MINI_RPG_TOOL_H

// A Tool is an acquired version of an item (ie a potion in a bag, or an equipped weapon)
// It is not a standalone sprite, but it is "drawable" (the reference contains the information
// necessary to render an image of it)
class Prop;
class ItemReference;
class Tool {
protected:
    ItemReference* ref_;

    explicit Tool(ItemReference* ref) : ref_(ref) {};
public:
    // Factory method: defer instantiation to subclasses
    Tool* makeTool(std::string itemName);

    // Accessor for the fields all Tools have
    std::string name();
    virtual std::string type() = 0;
    int dropChance();
    int cost();
    std::string path();
    bool sheet();
    int row();
    int col();
    int width();
    int height();
};

#endif //MINI_RPG_TOOL_H
