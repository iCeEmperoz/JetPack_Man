
#ifndef EXPLOSION_OBJECT_H_
#define EXPLOSION_OBJECT_H_

#include "CommonFunc.h"
#include "BaseObject.h"

#define  NUM_FRAME_EXP 20
#define  MAIN_FRAME_EXP 5

class ExplosionObject : public BaseObject
{

public:
    ExplosionObject();
    ~ExplosionObject();

    void set_clip(const int num_frame);
    void set_frame(const int& fr) { frame_ = fr; }
    virtual bool LoadImg(std::string path, SDL_Renderer* screen, const int num_frame);
    void Show(SDL_Renderer* screen, const int num_frame);
    int get_frame_width() const { return frame_width_; }
    int get_frame_height() const { return frame_height_; }
    
    void set_x_val(const float& xVal){x_val_ = xVal;}
    void set_y_val(const float& yVal){y_val_ = yVal;}
    void set_x_pos(const float& xPos){x_pos_ = xPos;}
    void set_y_pos(const float& yPos){y_pos_ = yPos;}

    float get_x_val() const {return x_val_;}
    float get_y_val() const {return y_val_;}
    float get_x_pos() const {return x_pos_;}
    float get_y_pos() const {return y_pos_;}

    int get_animation_loop() const {return animation_loop_;}
private:
    int frame_width_;
    int frame_height_;
    unsigned int frame_;

    float x_pos_;
    float y_pos_;
    float x_val_;
    float y_val_;
    int animation_loop_;
    SDL_Rect frame_clip_[NUM_FRAME_EXP];

};


#endif
