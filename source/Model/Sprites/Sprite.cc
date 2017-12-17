#include "Sprite.h"

Sprite::Sprite(float x, float y, bool sheet, std::string path, int w, int h) :
        x_(x), y_(y), zIndex_(0), path_(path), sheet_(sheet), width_(w), height_(h) {};

void Sprite::setSheetPosition(int r, int c) {
    row_ = r;
    col_ = c;
};