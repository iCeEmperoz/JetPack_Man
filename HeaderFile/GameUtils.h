#ifndef GAME_UTILS_H_
#define GAME_UITLS_H_

#include "CommonFunc.h"
#include "BaseObject.h"
#include "ThreatsObject.h"
#include "Item.h"
#include "BulletObject.h"

std::vector<ThreatsObject *> MakeThreatList(SDL_Renderer *screen);

std::vector<Item *> MakeCoinList(int x, SDL_Renderer *screen);

Item *MakeRandomItem(int x, SDL_Renderer *screen);

void RecreateItem(Item *&item, int x, SDL_Renderer *screen);

void RecreateCoinList(std::vector<Item *> &coins_list, int x, SDL_Renderer *screen);

void RecreateThreatList(std::vector<ThreatsObject *> &threats_list, SDL_Renderer *screen);

void LoadMenuStart(BaseObject g_menu_screen, 
                    SDL_Renderer *screen, 
                    Mix_Music *&gMenuMusic, 
                    Mix_Chunk *&gMove, 
                    Mix_Chunk *&gClick, 
                    bool &Play_Again);

void LoadMenuPause(BaseObject g_menu_screen, 
                    SDL_Renderer *screen, 
                    Mix_Music *&gMenuMusic, 
                    Mix_Chunk *&gMove, 
                    Mix_Chunk *&gClick, 
                    Mix_Music *&gMusic,
                    bool &is_quit,
                    bool &Play_Again);

void LoadMenuQuit(BaseObject g_menu_screen, 
                    SDL_Renderer *screen, 
                    Mix_Music *&gMenuMusic, 
                    Mix_Chunk *&gMove, 
                    Mix_Chunk *&gClick, 
                    Mix_Music *&gMusic,
                    bool &is_quit,
                    bool &quit,
                    bool &Play_Again);

#endif // !GAME_UTILS_H_