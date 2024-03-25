#include "HeaderFile/BulletObject.h"

BulletObject::BulletObject()
{
    x_val_ = 0;
    y_val_ = 0;
    is_move_ = false;
    bullet_type_ = LASER_BULLET;
}

BulletObject::~BulletObject()
{
}

bool BulletObject::LoadImgBullet(SDL_Renderer* des)
{
    bool ret = false;
    switch (bullet_type_) {
    case SPHERE_BULLET:
        ret = LoadImg("img//sphere_bullet.png", des);
        break;
    case LASER_BULLET:
        ret = LoadImg("img//laser_bullet.png", des);
        break;
    case FIRE_BULLET:
        ret = LoadImg("img//fire_bullet.png", des);
        break;
    case R_BULLET:
        ret = LoadImg("img//R_bullet.png", des);
        break;
    case LASER_BULLET_02:
        ret = LoadImg("img//laser_bullet_02.png", des);
        break;
    case BOMB_BULLET:
        ret = LoadImg("img//bomb_bullet.png",des);
        break;
    default:
        break;
}

    return ret;
}
void BulletObject::HandleMove(const int &x_border, const int &y_border)
{
    switch (bullet_dir_)
    {
    case DIR_RIGHT:
        {
            rect_.x += x_val_;
            if (rect_.x > x_border)
            {
                is_move_ = false;
            }
        } break;
    case DIR_LEFT:
        {
            rect_.x -= x_val_;
            if (rect_.x < 0) 
            {
                is_move_ = false;
            }
        } break;
    case DIR_UP_LEFT:
        {
            rect_.x -= x_val_;
            if (rect_.x < 0) 
            {
                is_move_ = false;
            }
            rect_.y -= y_val_;
            if (rect_.y < 0) 
            {
                is_move_ = false;
            }
        } break;
    case DIR_UP_RIGHT:
        {
            rect_.x += x_val_;
            if (rect_.x > x_border)
            {
                is_move_ = false;
            }

            rect_.y -= y_val_;
            if (rect_.y < 0) 
            {
                is_move_ = false;
            }
        } break;
    case DIR_DOWN:
        {
            rect_.y += y_val_;
            if (rect_.y > y_border) 
            {
                is_move_ = false;
            }
        } break;
    case DIR_DOWN_LEFT:
        {
            rect_.x -= x_val_;
            if (rect_.x < 0)
            {
                is_move_ = false;
            }
            rect_.y += y_val_;
            if (rect_.y > y_border)
            {
                is_move_ = false;
            }
        }
    default:
        break;
    }

}

