
#include "HeaderFile/ExplosionObject.h"

ExplosionObject::ExplosionObject()
{
    frame_height_ = 0;
    frame_width_ = 0;
    frame_ = 0;
    animation_loop_ = 0;
}

ExplosionObject::~ExplosionObject()
{

}

bool ExplosionObject::LoadImg(std::string path, SDL_Renderer* screen, const int num_frame)
{
    bool ret = BaseObject::LoadImg(path, screen);

    if (ret)
    {
        frame_width_ = rect_.w / num_frame;
        frame_height_ = rect_.h;
    }

    return ret;
}

void ExplosionObject::set_clip(const int num_frame)
{
    if (frame_width_ > 0 && frame_height_ > 0)
    {
        for (int i = 0; i < num_frame; i++)
        {
            frame_clip_[i].x = i * frame_width_;
            frame_clip_[i].y = 0;
            frame_clip_[i].w = frame_width_;
            frame_clip_[i].h = frame_height_;
        }
    }
}

void ExplosionObject::Show(SDL_Renderer* screen, const int num_frame)
{
    SDL_Rect* current_clip = &frame_clip_[frame_];
    // rect_.x = x_pos_ - map_x_;
    // rect_.y = y_pos_ - map_y_;
    if(frame_ >= num_frame)
    {
        frame_ = 0;
        animation_loop_ ++ ;
    }
    SDL_Rect RenderQuard = {rect_.x,rect_.y,frame_width_,frame_height_};
    SDL_RenderCopy(screen, p_object_, current_clip, &RenderQuard);
    frame_++;
    // SDL_Rect* current_clip = &frame_clip_[frame_];
    // SDL_Rect render_quad = { rect_.x, rect_.y, frame_width_, frame_height_ };
    // if (current_clip != NULL)
    // {
    //     render_quad.w = current_clip->w;
    //     render_quad.h = current_clip->h;
    // }

    // SDL_RenderCopy(screen, p_object_, current_clip, &render_quad);
}

