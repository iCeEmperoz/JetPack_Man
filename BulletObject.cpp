#include "HeaderFile/BulletObject.h"

BulletObject::BulletObject()
{
    x_val_ = 0;
    y_val_ = 0;
    is_move_ = false;
    bullet_type_ = LASER_BULLET;
    x_pos_ = 0;
    y_pos_ = 0;
    map_x_ = 0;
    map_y_ = 0;
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
void BulletObject::HandelMove()
{
    if(is_move_ == true)
    {
        if(bullet_dir_ == DIR_RIGHT)
        {
            x_pos_ += x_val_;
            // int x1 = x_pos_ / TILE_SIZE;
            // int x2 = (x_pos_ + rect_.w - 1) / TILE_SIZE;

            // int y1 = y_pos_ / TILE_SIZE;
            // int y2 = (y_pos_ + rect_.h - 1) / TILE_SIZE;

            // map_data.tile[y1][x2] == BLANK_TILE || map_data.tile[y2][x2] == BLANK_TILE 
            distance_ += x_val_; 
            if(distance_ > 300)
            {
                is_move_ = false;
                distance_ = 0;
            }
        }
        if(bullet_dir_ == DIR_LEFT)
        {
            x_pos_ -= x_val_;
            distance_ += x_val_; 
            if(distance_ > 300)
            {
                is_move_ = false;
                distance_ = 0;
            }
        }
        if(bullet_dir_ == DIR_DOWN)
        {
            y_pos_ += y_val_;
            distance_ += y_val_;
            if(distance_ > 300)
            {
                is_move_ = false;
                distance_ = 0;
            }
        }
    }
    rect_.x = x_pos_ - map_x_;
    rect_.y = y_pos_ - map_y_;
}
