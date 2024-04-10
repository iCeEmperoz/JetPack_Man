#ifndef THREATS_OBJECT_H_
#define THREATS_OBHECT_H_

#include "CommonFunc.h"
#include "BaseObject.h"
#include "BulletObject.h"
#define THREAT_MAX_HEIGHT 300
#define THREAT_MIN_HEIGHT 330

#define THREAT_POSITION_RANGE 250
#define THREAT_MAX_FALL_SPEED 10
#define THREAT_GRAVITY_SPEED 0.2
#define THREAT_FRAME_NUM 8
#define THREAT_SPEED 5
class ThreatsObject : public BaseObject
{
public:
    ThreatsObject();
    ~ThreatsObject();

    enum TypeMove
    {
        STATIC_THREAT = 0,
        MOVE_IN_SPACE_THREAT = 1,

    };
    enum TypeThreat
    {
        THREAT_2,
        THREAT_SURIKEN,        
        THREAT_PLANE,
    };

    void set_x_pos(const float &xp) { x_pos_ = xp; }
    void set_y_pos(const float &yp) { y_pos_ = yp; }
    float get_x_pos() const { return x_pos_; }
    float get_y_pos() const { return y_pos_; }
    
    void set_clips();
    bool LoadImg(std::string path, SDL_Renderer *screen);
    void Show(SDL_Renderer *des);
    int get_width_frame() const { return width_frame_; }
    int get_height_frame() const { return height_frame_; }
    void Move(const int& acceleration);

    void set_type_threat(const int& typeThreat) {type_threat_ = typeThreat;}
    void set_input_left(const int &ipLeft) { input_type_.left_ = ipLeft; }
    void ImpMoveType(SDL_Renderer *screen);
    SDL_Rect GetRectFrame();

    std::vector<BulletObject*> get_bullet_list() const {return bullet_list_;}
    void set_bullet_list(const std::vector<BulletObject*>& bl_list) {bullet_list_ = bl_list;}
    
    void InitBullet(BulletObject* p_bullet, SDL_Renderer* screen);
    void MakeBullet(SDL_Renderer* screen, const int& x_limit, const int& y_limit, int acceleration);
    void RemoveBullet(const int& idx);
    bool IsOffScreen();
    
private:
    float x_pos_;
    float y_pos_;
    int come_back_time_;
    SDL_Rect frame_clip_[THREAT_FRAME_NUM];
    int width_frame_;
    int height_frame_;
    int frame_;

    Input input_type_;
    int type_threat_;
    std::vector<BulletObject*>  bullet_list_;
};

#endif