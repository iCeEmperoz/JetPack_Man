#include "HeaderFile/Item.h"

Item::Item()
{
    frame_ = 0;
}

Item::~Item()
{

}
void Item::Show(SDL_Renderer *des, int FRAME)
{
    if (p_object_ != NULL) {
    rect_.x = x_pos_;
    rect_.y = y_pos_; 
    frame_++;
    if (frame_ >= FRAME)
    {
        frame_ = 0;
    }

    SDL_Rect *currentClip = &frame_clip_[frame_];
    SDL_Rect rendQuad = {rect_.x, rect_.y, width_frame_, height_frame_};
    SDL_RenderCopy(des, p_object_, currentClip, &rendQuad);
    }
}
SDL_Rect Item::GetRectFrame()
{
    SDL_Rect rect;
    rect.x = rect_.x;
    rect.y = rect_.y;
    rect.w = width_frame_;
    rect.h = height_frame_;

    return rect;
}
void Item::Move(int acceleration)
{
    x_pos_ -= (ITEM_SPEED+acceleration);
}

void Item::Move_sin(int acceleration)
{
    x_pos_ -= (ITEM_SPEED+acceleration);
    y_pos_ = y_start_- (rand() % (1) - 1)*1.0f * AMPLITUDE * sin(FREQUENCY * x_pos_);
}
bool Item::IsOffScreenLeft() {
    return x_pos_ < 0;
}
bool Item::IsOffScreenRight() 
{
    return x_pos_ > SCREEN_WIDTH;
}
bool Item::LoadImg(std::string path, SDL_Renderer *screen, int FRAME)
{
    bool ret = BaseObject::LoadImg(path, screen);
    if (ret)
    {
        width_frame_ = rect_.w / FRAME;
        height_frame_ = rect_.h;
    }
    return ret;
}
void Item::set_clips(int FRAME)
{
    if (width_frame_ > 0 && height_frame_ > 0)
    {
        for (int i = 0; i < FRAME; i++)
        {
            frame_clip_[i].x = i * width_frame_;
            frame_clip_[i].y = 0;
            frame_clip_[i].w = width_frame_;
            frame_clip_[i].h = height_frame_;
        }
    }
}