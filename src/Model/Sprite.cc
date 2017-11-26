#include "Sprite.h"


Sprite::Sprite(float x, float y, bool sheet, std::string path, int w, int h) :
        x_(x), y_(y), character_sheet_(sheet), isPlayer_(false), path_(path), width_(w), height_(h),
        hasWeapon_(false), weaponPath_("") {};

void Sprite::setCharacter(int r, int c) {
    row_ = r;
    col_ = c;
};