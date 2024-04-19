
#include "HeaderFile/MainObject.h"

MainObject::MainObject()
{
    frame_ = 0;
    x_pos_ = SCREEN_WIDTH - 1100;
    y_pos_ = GROUND;
    width_frame_ = 0;
    height_frame_ = 0;
    status_ = WALK_RIGHT;

    come_back_time_ = 0;
    undie_time = 200;
    is_pause = false;
    SPEEDING = false;
}

MainObject::~MainObject()
{
}

bool MainObject::LoadImg(std::string path, SDL_Renderer *screen)
{
    bool ret = BaseObject::LoadImg(path, screen);

    if (ret == true)
    {
        width_frame_ = rect_.w / MAIN_FRAME;
        height_frame_ = rect_.h;
    }

    return ret;
}

SDL_Rect MainObject::GetRectFrame()
{
    SDL_Rect rect;
    rect.x = rect_.x;
    rect.y = rect_.y;
    rect.w = width_frame_;
    rect.h = height_frame_;

    return rect;
}

void MainObject::set_clip()
{
    if (width_frame_ > 0 && height_frame_ > 0)
    {
        for (int i = 0; i < MAIN_FRAME; i++)
        {
            frame_clip_[i].x = i * width_frame_;
            frame_clip_[i].y = 0;
            frame_clip_[i].w = width_frame_;
            frame_clip_[i].h = height_frame_;
        }
    }
}

void MainObject::HandelInputAction(SDL_Event events, SDL_Renderer *screen, Mix_Chunk *mChunk)
{
    if (come_back_time_ == 0)
    {
        if (events.type == SDL_KEYDOWN)
        {
            switch (events.key.keysym.sym)
            {
            case SDLK_SPACE:
            {
                status_ = FLY;
            }
            break;
            case SDLK_ESCAPE:
            {
                is_pause = true;
            }
            break;
            }
        }
        else if (events.type == SDL_KEYUP)
        {
            switch (events.key.keysym.sym)
            {
            case SDLK_SPACE:
            {
                status_ = FALL;
            }
            break;
            }
        }

        if (events.type == SDL_MOUSEBUTTONDOWN)
        {
            if (events.button.button == SDL_BUTTON_LEFT)
            {
                Mix_PlayChannel(MIX_CHANNEL, mChunk, 0);
                ShootNormal(screen);
            }
        }
    }
}

void MainObject::Show(SDL_Renderer *des, int num_frame)
{
    UpdateImagePlayer(des);
    frame_++;
    if (frame_ >= num_frame)
    {
        frame_ = 0;
    }

    if (come_back_time_ == 0)
    {
        rect_.x = x_pos_;
        rect_.y = y_pos_;
        SDL_Rect *current_clip = &frame_clip_[frame_];

        SDL_Rect renderQuad = {rect_.x, rect_.y, width_frame_, height_frame_};

        SDL_RenderCopy(des, p_object_, current_clip, &renderQuad);
    }
}

void MainObject::HandleBullet(SDL_Renderer *des, int acceleration)
{
    for (int i = 0; i < p_bullet_list_.size(); i++)
    {
        BulletObject *p_bullet = p_bullet_list_.at(i);
        if (p_bullet != NULL)
        {
            if (p_bullet->get_is_move() == true)
            {
                int bullet_distance = rect_.x + width_frame_ - p_bullet->GetRect().x;
                if (bullet_distance < 500 && bullet_distance > (-500))
                {
                    p_bullet->HandleMove(SCREEN_WIDTH, SCREEN_HEIGHT, acceleration);
                    p_bullet->Render(des);
                }
                else
                {
                    p_bullet->set_is_move(false);
                }
            }
            else
            {
                p_bullet_list_.erase(p_bullet_list_.begin() + i);
                if (p_bullet != NULL)
                {
                    delete p_bullet;
                    p_bullet = NULL;
                }
            }
        }
    }
}

void MainObject::RemoveBullet(const int &idx)
{
    int size = p_bullet_list_.size();
    if (size > 0 && idx < size)
    {
        BulletObject *p_bullet = p_bullet_list_.at(idx);
        p_bullet_list_.erase(p_bullet_list_.begin() + idx);

        if (p_bullet)
        {
            delete p_bullet;
            p_bullet = NULL;
        }
    }
}

void MainObject::DoPlayerX(int &acceleration)
{
    if (!SPEEDING)
    {
        if (come_back_time_ == 0)
        {
            if (status_ == FLY)
            {
                y_pos_ -= (PLAYER_JUMP_VAL + acceleration);
            }
            if (y_pos_ <= 0)
            {
                y_pos_ = 0;
                status_ == FALL;
            }
            if (status_ == FALL && y_pos_ < GROUND)
            {
                y_pos_ += GRAVITY_SPEED + acceleration;
            }
            if (y_pos_ > GROUND)
            {
                y_pos_ = GROUND;
            }
        }
    }
    if (SPEEDING == true && time_item > 0)
    {
        time_item--;
        acceleration = ACCEL_SPEED;
    }
    else if (time_item == 0)
    {
        SPEEDING = false;
        time_item = 200;
        acceleration = 0;
        undie_time = 200;
    }
    if (undie_time > 0)
    {
        undie_time--;
    }
    if (come_back_time_ > 0)
    {
        come_back_time_--;
        if (come_back_time_ == 0) // Reset again
        {
            x_pos_ = SCREEN_WIDTH - 1100;
            y_pos_ = GROUND;
        }
    }
}

void MainObject::ShootNormal(SDL_Renderer *screen)
{
    BulletObject *p_bullet = new BulletObject();
    p_bullet->set_bullet_type(BulletObject::LASER_BULLET);
    p_bullet->LoadImgBullet(screen);

    p_bullet->set_bullet_dir(BulletObject::DIR_RIGHT);
    p_bullet->SetRect(this->rect_.x + width_frame_ - 20, this->rect_.y + height_frame_ * 0.5);

    p_bullet->set_x_val(20);
    p_bullet->set_y_val(20);
    p_bullet->set_is_move(true);

    p_bullet_list_.push_back(p_bullet);
}

void MainObject::UpdateImagePlayer(SDL_Renderer *des)
{
    if (OnGround() == false)
    {
        if (undie_time != 0)
        {
            LoadImg("img//PLAYER//FLY_WITH_GUN_N.png", des);
            set_clip();
        }
        else
        {
            LoadImg("img//PLAYER//FLY_WITH_GUN.png", des);
            set_clip();
        }
    }
    else
    {
        if (undie_time != 0)
        {
            LoadImg("img//PLAYER//WALK_WITH_GUN_N.png", des);
            set_clip();
        }
        else
        {
            LoadImg("img//PLAYER//WALK_WITH_GUN.png", des);
            set_clip();
        }
    }
}