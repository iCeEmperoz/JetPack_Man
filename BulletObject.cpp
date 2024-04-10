#include "HeaderFile/BulletObject.h"

BulletObject::BulletObject()
{
    x_val_ = 0;
    y_val_ = 0;
    is_move_ = false;
    bullet_type_ = LASER_BULLET;
    x_pos_ = 0;
    y_pos_ = 0;
    distance_ = 0;
}

BulletObject::~BulletObject()
{
}

bool BulletObject::LoadImgBullet(SDL_Renderer* des)
{
    bool ret = false;
    switch (bullet_type_) {
    case SPHERE_BULLET:
        ret = LoadImg("img//bullet//sphere_bullet.png", des);
        break;
    case LASER_BULLET:
        ret = LoadImg("img//bullet//laser_bullet.png", des);
        break;
    case FIRE_BULLET:
        ret = LoadImg("img//bullet//fire_bullet.png", des);
        break;
    case R_BULLET:
        ret = LoadImg("img//bullet//R_bullet.png", des);
        break;
    case LASER_BULLET_02:
        ret = LoadImg("img//bullet//laser_bullet_02.png", des);
        break;
    case BOMB_BULLET:
        ret = LoadImg("img//bullet//bomb_bullet.png",des);
        break;
    default:
        break;
}

    return ret;
}
void BulletObject::HandleMove(const int &x_border, const int &y_border, int acceleration)
{
    switch (bullet_dir_)
    {
    case DIR_RIGHT:
        {
            rect_.x += (x_val_ + acceleration);
            if (rect_.x > x_border)
            {
                is_move_ = false;
            }
        } break;
    case DIR_LEFT:
        {
            rect_.x -=( x_val_ + acceleration);
            if (rect_.x < 0) 
            {
                is_move_ = false;
            }
        } break;
    case DIR_DOWN:
        {
            rect_.y += (y_val_ + acceleration);
            if (rect_.y > y_border) 
            {
                is_move_ = false;
            }
        } break;
    default:
        break;
    }

}
