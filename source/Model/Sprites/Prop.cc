#include "Prop.h"

#include "../Tools/Tool.h"
#include "../../ItemReader.h"

Prop::Prop(float x, float y, ItemReference* ref) :
            Sprite(x, y, ref->sheet(), ref->path(), ref->width(), ref->height()), ref_(ref) {
    if (ref->sheet()) setSheetPosition(ref->row(), ref->col());
};

Prop Prop::makeProp(std::string itemName, float x, float y) {
    return Prop(x, y, ItemReader::instance()->getReference(itemName));
};

std::string Prop::name() { return ref_->name(); };
int Prop::dropChance() { return ref_->dropChance(); };
int Prop::cost() { return ref_->cost(); };
bool Prop::acquirable() { return ref_->acquirable(); };
int Prop::healing() { return ref_->healing(); };
int Prop::value() { return ref_->value(); };
