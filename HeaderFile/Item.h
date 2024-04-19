#ifndef ITEM_H_
#define ITEM_H_

#include "Variable.h"
#include "BaseObject.h"
#include "cmath"
#define ITEM_FRAME_NUM 5
#define ITEM_SPEED 5
const float AMPLITUDE = 250.0f; // Biên độ của đường cong sin
const float FREQUENCY = 0.01f; // Tần số của đường cong sin

class Item : public BaseObject
{
public:
    Item();
    ~Item();

    void set_x_pos(const float &xp) { x_pos_ = xp;}
    void set_y_pos(const float &yp) { y_pos_ = yp; }
    void set_y_start(const float &ys) {y_start_ = ys;}
    float get_x_pos() const { return x_pos_; }
    float get_y_pos() const { return y_pos_; }

    void set_clips(int FRAME);
    bool LoadImg(std::string path, SDL_Renderer *screen, int FRAME);

    int get_width_frame() const { return width_frame_; }
    int get_height_frame() const { return height_frame_; }
    SDL_Rect GetRectFrame();

    void Show(SDL_Renderer *des, int FRAME);
    void Move(int acceleration);
    void Move_sin(int acceleration);
    bool IsOffScreenLeft();
    bool IsOffScreenRight();
private:
    float x_pos_;
    float y_pos_;
    float y_start_;
    SDL_Rect frame_clip_[ITEM_FRAME_NUM];
    int width_frame_;
    int height_frame_;
    int frame_;
};

#endif