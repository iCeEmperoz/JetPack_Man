#ifndef MAIN_OBJECT_H_
#define MAIN_OBJECT_H_

#include "CommonFunc.h"
#include "BaseObject.h"
#include "BulletObject.h"
#include <vector>


class MainObject : public BaseObject
{
public:
    MainObject();
    ~MainObject();

    enum STATUS
    {
        WALK_NONE,
        WALK_RIGHT,
        SHOT,
        FLY,
        FALL,
    };
    bool LoadImg(std::string path, SDL_Renderer *screen);
    void Show(SDL_Renderer *des, int num_frame);
    void HandelInputAction(SDL_Event envents, SDL_Renderer *screen, Mix_Chunk* mChunk);
    void set_clip();

    void DoPlayerX(Map &map_data);
    void CheckToMap(Map &map_data);
    void SetMapXY(const int map_x, const int map_y)
    {
        map_x_ = map_x;
        map_y_ = map_y;
    };
    void set_x_pos(const float &xp) { x_pos_ = xp; }
    void set_y_pos(const float &yp) { y_pos_ = yp; }
    float get_x_pos() const {return x_pos_;}
    float get_y_pos() const {return y_pos_;} 
    void CenterEntityOnMap(Map &map_data);
    void UpdateImagePlayer(SDL_Renderer *des);
    SDL_Rect GetRectFrame();

    void set_bullet_list(std::vector<BulletObject *> bullet_list)
    {
        p_bullet_list_ = bullet_list;
    }

    std::vector<BulletObject *> get_bullet_list() const { return p_bullet_list_; }
    void HandleBullet(SDL_Renderer *des);
    void RemoveBullet(const int &idx);
    void ShootNormal(SDL_Renderer *screen);
    void ShootR(SDL_Renderer *screen);
    void IncreaseMoney();
    void IncreaseScore();
    int get_frame_width() const { return width_frame_; }
    int get_frame_height() const { return height_frame_; }
    void set_comback_time(const int &cb_time) { come_back_time_ = cb_time; }
    int GetMoneyCount() const { return money_count; }
    int GetScoreCount() const {return score_count;}
    void InitScoreCount() 
    {
        score_count = 0;
    } 
    void InitMoneyCount() 
    {
        money_count = 0;
    }
    Input input_type_;
    bool OnGround() {
        return y_pos_ == GROUND;
    };
    bool is_pause;
private:
    int money_count;
    int score_count;
    std::vector<BulletObject *> p_bullet_list_;
    float x_val_;
    float y_val_;

    float x_pos_;
    float y_pos_;

    int width_frame_;
    int height_frame_;

    SDL_Rect frame_clip_[20];
    int frame_;
    int status_;
    int status_front;
    bool on_ground_;

    int map_x_;
    int map_y_;
    int come_back_time_;
};

#endif