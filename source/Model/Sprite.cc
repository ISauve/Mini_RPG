#include "Sprite.h"


Sprite::Sprite(float x, float y, bool sheet, std::string path, int w, int h) :
        x_(x), y_(y), path_(path),  character_sheet_(sheet), width_(w), height_(h), zIndex_(0) {};

void Sprite::setCharacter(int r, int c) {
    row_ = r;
    col_ = c;
};