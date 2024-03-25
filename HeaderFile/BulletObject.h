#ifndef BULLET_OBJECT_H_
#define BULLET_OBJECT_H_

#include "BaseObject.h"
#include "CommonFunc.h"

class BulletObject : public BaseObject
{
public:
    BulletObject();
    ~BulletObject();

    enum BulletDir
    {
        DIR_RIGHT,
        DIR_LEFT,
        DIR_UP,
        DIR_UP_LEFT,
        DIR_UP_RIGHT, 
        DIR_DOWN_LEFT, 
        DIR_DOWN_RIGHT, 
        DIR_DOWN
    };

    enum BulletType
    {
        SPHERE_BULLET,
        LASER_BULLET,
        FIRE_BULLET,
        LASER_BULLET_02,
        BOMB_BULLET,
        R_BULLET
    };

    void set_x_val(const int &xVal) { x_val_ = xVal; }
    void set_y_val(const int &yVal) { y_val_ = yVal; }
    int get_x_val() const { return x_val_; }
    int get_y_val() const { return y_val_; }

    void set_is_move(const bool &isMove) { is_move_ = isMove; }
    bool get_is_move() const { return is_move_; }

    void set_bullet_dir(const unsigned int &bulletDir) { bullet_dir_ = bulletDir; }
    int get_bullet_dir() const { return bullet_dir_; }

    void set_bullet_type(const unsigned int &bulletType) { bullet_type_ = bulletType; }
    unsigned int get_bullet_type() const { return bullet_type_; }

    void HandleMove(const int &x_border, const int &y_border);
    void setMapXY(const int& mp_x,const int& mp_y)
    {
        map_x_ = mp_x;
        map_y_ = mp_y;
    }
    bool LoadImgBullet(SDL_Renderer *des);
private:
    int map_x_;
    int map_y_;
    int x_val_;
    int y_val_;
    bool is_move_;
    unsigned int bullet_dir_;
    unsigned int bullet_type_;
};
#endif