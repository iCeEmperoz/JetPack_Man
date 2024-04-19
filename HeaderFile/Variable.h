#ifndef VARIABLE_H_
#define VARIABLE_H_

#include <windows.h>
#include <string>
#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <fstream>
#include <cstdlib>
#include <ctime>
static SDL_Window* g_window = NULL;
static SDL_Renderer* g_screen = NULL;
static SDL_Event g_event;
static SDL_Event two_event;

//Screen
const int FRAME_PER_SECOND = 45; // fps
const int SCREEN_WIDTH=1280;
const int SCREEN_HEIGHT=640;
const int SCREEN_BPP = 32;

const int COLOR_KEY_R=167;
const int COLOR_KEY_G=175;
const int COLOR_KEY_B=180;

const int RENDER_DRAW_COLOR = 0xff;
const int BACKGROUND_LAYER = 9;

#define ACCEL_SPEED 50
#define GRAVITY_SPEED 3
#define MAX_FALL_SPEED 6

#define TIME_MAX 1000
#define GROUND 520

#define SPEED_INCREASEMENT 2
#define SCORE_INCREASEMENT 1
#define TIME_INCREASEMENT 1

#define PLAYER_SPEED_FLY 2
#define PLAYER_JUMP_VAL 5

#define MAIN_FRAME 15
#define STATE_MONEY 4
#define MIX_CHANNEL -1

#define IS_REPEATITIVE -1
#define NOT_REPEATITIVE 0

#define BASE_OFFSET_SPEED 0

const std::string LAYER[BACKGROUND_LAYER] = {
	"img//background//cave//1.png",
	"img//background//cave//2.png",
	"img//background//cave//3.png",
	"img//background//cave//4.png",
	"img//background//cave//5.png",
	"img//background//cave//6.png",
	"img//background//cave//7.png",
	"img//background//cave//8.png",
	"img//background//cave//9.png",
};


#endif