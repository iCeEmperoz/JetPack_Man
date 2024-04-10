#include "HeaderFile/ThreatsObject.h"

ThreatsObject::ThreatsObject()
{
    width_frame_ = 0;
    height_frame_ = 0;

    come_back_time_ = 0;
    frame_ = 0;
}

ThreatsObject::~ThreatsObject()
{
}

bool ThreatsObject::LoadImg(std::string path, SDL_Renderer *screen)
{
    bool ret = BaseObject::LoadImg(path, screen);
    if (ret)
    {
        width_frame_ = rect_.w / THREAT_FRAME_NUM;
        height_frame_ = rect_.h;
    }
    return ret;
}

SDL_Rect ThreatsObject::GetRectFrame()
{
    SDL_Rect rect;
    rect.x = rect_.x;
    rect.y = rect_.y;
    rect.w = width_frame_;
    rect.h = height_frame_;

    return rect;
}
void ThreatsObject::set_clips()
{
    if (width_frame_ > 0 && height_frame_ > 0)
    {
        for (int i = 0; i < THREAT_FRAME_NUM; i++)
        {
            frame_clip_[i].x = i * width_frame_;
            frame_clip_[i].y = 0;
            frame_clip_[i].w = width_frame_;
            frame_clip_[i].h = height_frame_;
        }
    }
}

void ThreatsObject::Show(SDL_Renderer *des)
{
    rect_.x = x_pos_;
    rect_.y = y_pos_;
    frame_++;
    if (frame_ >= THREAT_FRAME_NUM)
    {
        frame_ = 0;
    }

    SDL_Rect *currentClip = &frame_clip_[frame_];
    SDL_Rect rendQuad = {rect_.x, rect_.y, width_frame_, height_frame_};
    SDL_RenderCopy(des, p_object_, currentClip, &rendQuad);
}

void ThreatsObject::Move(const int &acceleration)
{
    x_pos_ -= (THREAT_SPEED + acceleration);
}
bool ThreatsObject::IsOffScreen()
{
    return x_pos_ < 0;
}
void ThreatsObject::ImpMoveType(SDL_Renderer *screen)
{

    if (type_threat_ == THREAT_PLANE)
    {
        LoadImg("img//threat//plane_left.png", screen);
    }
    else if (type_threat_ == THREAT_2)
        LoadImg("img//threat//threat2_left.png", screen);
    else if (type_threat_ == THREAT_SURIKEN)
        LoadImg("img//threat//suriken_2_left.png", screen);
}

void ThreatsObject::InitBullet(BulletObject *p_bullet, SDL_Renderer *screen)
{
    if (p_bullet != NULL)
    {
        if (type_threat_ != THREAT_PLANE)
            p_bullet->set_bullet_type(BulletObject::LASER_BULLET_02);
        else
            p_bullet->set_bullet_type(BulletObject::BOMB_BULLET);
        bool ret = p_bullet->LoadImgBullet(screen);
        if (ret)
        {
            p_bullet->set_is_move(true);
            p_bullet->set_bullet_dir(BulletObject::DIR_LEFT);
            p_bullet->SetRect(x_pos_ + width_frame_ / 2, y_pos_ + height_frame_ / 2);
            p_bullet->set_x_val(15);
            p_bullet->set_y_val(5);
            bullet_list_.push_back(p_bullet);
        }
    }
}

void ThreatsObject::MakeBullet(SDL_Renderer *screen, const int &x_limit, const int &y_limit, int acceleration)
{
    for (int i = 0; i < bullet_list_.size(); i++)
    {
        BulletObject *p_bullet = bullet_list_.at(i);
        if (p_bullet != NULL)
        {
            if (p_bullet->get_is_move())
            {
                p_bullet->HandleMove(x_limit, y_limit, acceleration);

                p_bullet->Render(screen);
            }
            else
            {
                p_bullet->SetRect(x_pos_ + width_frame_ / 2, y_pos_ + height_frame_ / 2);
                p_bullet->set_bullet_dir(BulletObject::DIR_LEFT);
                p_bullet->set_is_move(true); // Thiết lập viên đạn để di chuyển lại
            }
        }
    }
}

void ThreatsObject::RemoveBullet(const int &idx)
{
    int size = bullet_list_.size();
    if (size > 0 && idx < size)
    {
        BulletObject *p_bullet = bullet_list_.at(idx);
        bullet_list_.erase(bullet_list_.begin() + idx);

        if (p_bullet)
        {
            delete p_bullet;
            p_bullet = NULL;
        }
    }
}