#ifndef COMMON_FUNCTION_H_
#define COMMON_FUNCTION_H_

#include <windows.h>
#include <string>
#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <fstream>
static SDL_Window* g_window = NULL;
static SDL_Renderer* g_screen = NULL;
static SDL_Event g_event;
static SDL_Event two_event;

static Mix_Chunk* g_sound_bullet[2];
static Mix_Chunk* g_sound_exp[2];
//Screen
const int FRAME_PER_SECOND = 45; // fps
const int SCREEN_WIDTH=1280;
const int SCREEN_HEIGHT=640;
const int SCREEN_BPP = 32;

const int COLOR_KEY_R=167;
const int COLOR_KEY_G=175;
const int COLOR_KEY_B=180;

const int RENDER_DRAW_COLOR = 0xff;
const int BACKGROUND_LAYER = 7;
const double LAYER_1_SPEED = 0.0;
const double LAYER_2_SPEED = 0.25; 
const double LAYER_3_SPEED = 0.5;
const double LAYER_4_SPEED = 0.75;
const double LAYER_5_SPEED = 1.0;
const double LAYER_6_SPEED = 1.25;
const double LAYER_7_SPEED = 1.5;
const double LAYER_8_SPEED = 1.75;
const double LAYER_9_SPEED = 2.0;
#define BLANK_TILE 0
#define TILE_SIZE 64

#define MAX_MAP_X 400
#define MAX_MAP_Y 10

#define TIME_MAX 1000
#define GROUND 520

#define PLAYER_SPEED 5
#define STEP_SPEED 0.5
#define MAX_SPEED 10

#define PLAYER_SPEED_FLY 2
#define PLAYER_JUMP_VAL 4

#define MAIN_FRAME 10
#define STATE_MONEY 4
#define MIX_CHANNEL -1

#define IS_REPEATITIVE -1
#define NOT_REPEATITIVE 0

#define BASE_OFFSET_SPEED 0

struct Input
{
    int left_;
    int right_;
    int up_;
    int down_;
    int jump_;
    int shot_;
    int fly_;
};

struct Map 
{
    int start_x_;
    int start_y_;

    int max_x_;
    int max_y_;

    int tile[MAX_MAP_Y][MAX_MAP_X];
    std::string file_name_;
    
};

namespace SDLCommonFunc
{
    bool CheckCollision(const SDL_Rect& object1, const SDL_Rect& object2);
    bool CheckMousePos(const int& x,const int& y,const SDL_Rect& Obj_);
}
#endif