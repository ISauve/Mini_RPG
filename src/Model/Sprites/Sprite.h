#ifndef MINI_RPG_SPRITE_H
#define MINI_RPG_SPRITE_H

#include <string>

// Sprites are entities that get drawn
class Sprite {
protected:
    float x_;
    float y_;
    int zIndex_;

    std::string path_;      // Path to image
    bool sheet_;            // Whether or not the image is on a sheet
    int row_;               // Row at which it appears on sheet
    int col_;               // Col which it appears on sheet
    int width_;             // Width of the image
    int height_;            // Height of the image

public:
    Sprite(float, float, bool, std::string, int, int);
    virtual ~Sprite() = default;

    // Accessors
    float x() const { return x_; };
    float y() const { return y_; };
    std::string path() const { return path_; }
    bool isOnSheet() const { return sheet_; };
    int row() const { return row_; };
    int col() const { return col_; };
    int width() const { return width_; };
    int height() const { return height_; };
    int zIndex() const { return zIndex_; };

    // Mutators
    void setSheetPosition(int, int);
    void setZIndex(int z) { zIndex_ = z; };
};


#endif //MINI_RPG_SPRITE_H
