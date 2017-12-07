#include "../../ItemReader.h"
#include "../Sprites/Prop.h"
#include "Weapon.h"
#include "Armor.h"
#include "Item.h"

Tool* Tool::makeTool(std::string itemName) {
    ItemReference* ref = ItemReader::instance()->getReference(itemName);

    if (ref->type() == "weapon") {
        return new Weapon(ref);
    } else if (ref->type() == "armor") {
        return new Armor(ref);
    } else if (ref->type() == "item") {
        return new Item(ref);
    }
};

std::string Tool::name() { return ref_->name(); };
std::string Tool::path() { return ref_->path(); };
bool Tool::sheet() { return ref_->sheet(); };
int Tool::row() { return ref_->row(); };
int Tool::col() { return ref_->col(); };
int Tool::width() { return ref_->width(); };
int Tool::height() { return ref_->height(); };
int Tool::dropChance() { return ref_->dropChance(); };
int Tool::cost() { return ref_->cost(); };
