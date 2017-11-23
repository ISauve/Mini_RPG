#ifndef MINI_RPG_SPRITE_H
#define MINI_RPG_SPRITE_H

#include <string>


class Sprite {
protected:
    float x_;
    float y_;
    bool character_sheet_;
    std::string path_;           // Path to image
    int row_;                    // Row at which it appears on sheet
    int col_;                    // Col which it appears on sheet
    int width_;
    int height_;

protected:
    bool hasWeapon_;
    std::string weaponPath_;

public:
    Sprite(float, float, bool, std::string, int, int);

    // Accessor
    float x() const { return x_; };
    float y() const { return y_; };
    bool isOnCharSheet() const { return character_sheet_; };
    std::string path() const { return path_; }
    int row() const { return row_; };
    int col() const { return col_; };
    int width() const { return width_; };
    int height() const { return height_; };
    bool hasWeapon() const { return hasWeapon_; };
    std::string weaponPath() const { return weaponPath_; };

    // Mutators
    void setCharacter(int, int);
    void setImage(std::string p) { path_ = p; };
};


#endif //MINI_RPG_SPRITE_H
