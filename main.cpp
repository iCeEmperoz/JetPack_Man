#include "HeaderFile/Variable.h"
#include "HeaderFile/BaseObject.h"
#include "HeaderFile/MainObject.h"
#include "HeaderFile/ThreatsObject.h"
#include "HeaderFile/ExplosionObject.h"
#include "HeaderFile/TextObject.h"
#include "HeaderFile/PlayerPower.h"
#include "HeaderFile/GameUtils.h"
#include "HeaderFile/Item.h"

BaseObject g_background;
BaseObject g_backgroundTexture[BACKGROUND_LAYER];
BaseObject g_menu_screen;	

int OffsetSpeed_Ground = BASE_OFFSET_SPEED;
std::vector<double> OffsetSpeed_Bkgr(BACKGROUND_LAYER, BASE_OFFSET_SPEED);

Mix_Music *gMusic = nullptr;
Mix_Music *gMenuMusic = nullptr;
Mix_Chunk *gClick = nullptr;
Mix_Chunk *gLose = nullptr;
Mix_Chunk *gMove = nullptr;
Mix_Chunk *gShot = nullptr;
Mix_Chunk *gCoin = nullptr;
TTF_Font *font_ = NULL;

MainObject p_player;

PlayerPower player_power;
PlayerMoney player_money;

TextObject best_score;
TextObject score_game;
TextObject money_game;
bool InitData()
{
	bool success = true;
	int ret = SDL_Init(SDL_INIT_VIDEO);
	if (ret < 0)
		return false;
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

	g_window = SDL_CreateWindow("JetPackMan", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (g_window == NULL)
	{
		success = false;
	}
	else
	{
		g_screen = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
		if (g_screen == NULL)
			success = false;
		else
		{
			SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
			int imgflags = IMG_INIT_PNG;
			if (!(IMG_Init(imgflags) && imgflags))
				success = false;
		}
		if (TTF_Init() == -1)
		{
			success = false;
		}
		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 512) < 0)
		{
			success = false;
		}
		font_ = TTF_OpenFont("font//dlxfont_.ttf", 15);
		if (font_ == NULL)
		{
			success = false;
		}
	}
	return success;
}

void LoadBackground()
{
	std::vector<double> layer_speed;
	for (int i=0;i<9;i++)
	{
		layer_speed.push_back(0.0+i*0.25);
	}
	for (int i = 0; i < BACKGROUND_LAYER; ++i)
	{
		OffsetSpeed_Bkgr[i] -= layer_speed[i];
		if (OffsetSpeed_Bkgr[i] < -g_backgroundTexture[i].GetRect().w)
		{
			OffsetSpeed_Bkgr[i] = 0;
		}
		g_backgroundTexture[i].Render2(OffsetSpeed_Bkgr[i], 0, g_screen);
		g_backgroundTexture[i].Render2(OffsetSpeed_Bkgr[i] + g_backgroundTexture[i].GetRect().w, 0, g_screen);
	}
}

bool LoadMedia()
{
	bool success = true;
	for (int i = 0; i < BACKGROUND_LAYER; ++i)
	{
		if (!g_backgroundTexture[i].LoadImg(LAYER[i].c_str(), g_screen))
		{
			std::cout << "Failed to load background image" << std::endl;
			success = false;
		}
	}
	gMusic = Mix_LoadMUS("sound/Jetpack.wav");
	if (gMusic == nullptr)
	{
		success = false;
	}
	gMenuMusic = Mix_LoadMUS("sound/MusFruitKilla.wav");
	if (gMenuMusic == nullptr)
	{
		success = false;
	}

	gClick = Mix_LoadWAV("sound/mouse_click.wav");
	if (gClick == nullptr)
	{
		success = false;
	}

	gLose = Mix_LoadWAV("sound/lose_sound.wav");
	if (gLose == nullptr)
	{
		success = false;
	}
	gMove = Mix_LoadWAV("sound/mouse_move.wav");
	if (gMove == nullptr)
	{
		success = false;
	}
	gShot = Mix_LoadWAV("sound/shot.wav");
	if (gShot == nullptr)
	{
		success = false;
	}
	gCoin = Mix_LoadWAV("sound//coin.wav");
	if (gCoin == nullptr)
	{
		success = false;
	}

	p_player.LoadImg("img//PLAYER//WALK_WITH_GUN.png", g_screen);
	p_player.set_clip();

	player_money.Init(g_screen);
	player_money.SetPos(0, 40);

	best_score.SetColor(TextObject::WHITE_TEXT);
	score_game.SetColor(TextObject::WHITE_TEXT);
	money_game.SetColor(TextObject::WHITE_TEXT);
	return success;

}

void close()
{
	for (int i = 0; i < BACKGROUND_LAYER; ++i)
	{
		g_backgroundTexture[i].Free();
	}

	g_menu_screen.Free();
	Mix_FreeMusic(gMusic);
	Mix_FreeMusic(gMenuMusic);
	Mix_FreeChunk(gClick);
	Mix_FreeChunk(gLose);
	Mix_FreeChunk(gCoin);
	Mix_FreeChunk(gMove);
	Mix_FreeChunk(gShot);
	gMusic = nullptr;
	gMenuMusic = nullptr;
	gClick = nullptr;
	gLose = nullptr;
	gMove = nullptr;
	gShot = nullptr;
	gCoin = nullptr;
	g_background.Free();
	SDL_DestroyRenderer(g_screen);
	g_screen = NULL;

	SDL_DestroyWindow(g_window);
	g_window = NULL;
	
	IMG_Quit();
	Mix_Quit();
	SDL_Quit();
}

int main(int argc, char *args[])
{
	srand(time(NULL));
	int start_tick_ = 0;
	if (InitData() == false)
		return -1;
	if (LoadMedia() == false)
		return -1;

	bool is_quit = false;
	bool Play_Again = false;
	LoadMenuStart(g_menu_screen,g_screen,gMenuMusic,gMove,gClick,Play_Again);

	while (Play_Again)
	{
		Mix_PlayMusic(gMusic, IS_REPEATITIVE);
		int time_ = 0;    
		int acceleration = 0;
		int score = 0;
		int time_item = 200;
		int money = 0;
		int num_die = 0;
		is_quit = false;
		
		player_power.Init(g_screen);
		Item *item = NULL;
		std::vector<ThreatsObject *> threats_list = MakeThreatList(g_screen);
		std::vector<Item *> coins_list = MakeCoinList(rand() % 9, g_screen);
		std::vector<ExplosionObject *> explosion_list;

		int bscore = getBestScoreFromFile();

		while (!is_quit)
		{
			UpdateGameTimeAndScore(time_, acceleration, score);
			RecreateCoinList(coins_list, rand() % (9),g_screen);
			RecreateThreatList(threats_list,g_screen);
			if (score % 300 == 0)
			{
				RecreateItem(item, 1, g_screen);
			}
			start_tick_ = SDL_GetTicks();

			while (SDL_PollEvent(&g_event) != 0)
			{
				if (g_event.type == SDL_QUIT)
				{
					is_quit = true;
					Play_Again = false;
				}
				p_player.HandelInputAction(g_event, g_screen, gShot);
				if (p_player.is_pause == true)
				{
					Mix_PauseMusic();
					LoadMenuPause(g_menu_screen,g_screen,gMenuMusic,gMove,gClick,gMusic,is_quit,Play_Again);
					p_player.is_pause = false;
				}
			}

			SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
			SDL_RenderClear(g_screen);
			LoadBackground();

			p_player.HandleBullet(g_screen, acceleration);
			p_player.DoPlayerX(acceleration);
			p_player.Show(g_screen, MAIN_FRAME);

			if (item != NULL)
			{
				item->Move_sin(acceleration);
				item->Show(g_screen, ITEM_FRAME_NUM);
				SDL_Rect rect_player = p_player.GetRectFrame();
				SDL_Rect rect_item = item->GetRectFrame();
				if (CheckCollision(rect_item, rect_player) && p_player.get_comback_time() == 0)
				{ 
					p_player.undie_time = 200;
					p_player.SPEEDING = true;
					item->Free();
					item = NULL;
				}
			}
			for (int i = 0; i < coins_list.size();)
			{
				Item *coins = coins_list.at(i);
				coins->Move(acceleration);
				coins->Show(g_screen, ITEM_FRAME_NUM);
				SDL_Rect rect_player = p_player.GetRectFrame();
				SDL_Rect rect_item = coins->GetRectFrame();
				if (CheckCollision(rect_item, rect_player) &&p_player.get_comback_time() == 0)
				{
					Mix_PlayChannel(MIX_CHANNEL, gCoin, 0);
					coins->Free();
					delete coins;
					coins_list.erase(coins_list.begin() + i);
					money++;
				}
				else
					i++;
			}
		
			player_power.Show(g_screen);
			player_money.Show(g_screen);

			// Threat
			bool quit = false;
			for (int i = 0; i < threats_list.size(); i++)
			{
				ThreatsObject *p_threat = threats_list.at(i);
				if (p_threat != NULL)
				{
					p_threat->Move(acceleration);
					p_threat->Show(g_screen,THREAT_FRAME_NUM);
					p_threat->MakeBullet(g_screen, SCREEN_WIDTH, SCREEN_HEIGHT, acceleration);
					SDL_Rect rect_player = p_player.GetRectFrame();
					bool bCol1 = false;
					std::vector<BulletObject *> tBullet_list = p_threat->get_bullet_list();
					for (int jj = 0; jj < tBullet_list.size(); ++jj)
					{
						BulletObject *pt_bullet = tBullet_list.at(jj);
						if (pt_bullet)
						{
							SDL_Rect rect_bullet = pt_bullet->GetRect();
							bCol1 = CheckCollision(rect_bullet, rect_player) && pt_bullet->get_is_move();
							if (bCol1)
							{
								break;
							}
						}
					}
					SDL_Rect rect_threat = p_threat->GetRectFrame();
					bool bCol2 = CheckCollision(rect_threat, rect_player);
					if (p_player.undie_time == 0)
					{
						if ((bCol1 || bCol2) && (p_player.SPEEDING == false))
						{
							ExplosionObject *newExplosion = new ExplosionObject();
							if (p_player.OnGround()) newExplosion->LoadImg("img//PLAYER//STAND_DIE.png", g_screen, MAIN_FRAME_EXP);
							else newExplosion->LoadImg("img//PLAYER//FLY_DIE.png",g_screen,MAIN_FRAME_EXP);
							newExplosion->set_clip(MAIN_FRAME_EXP);
							int px_pos = (p_player.GetRect().x + p_player.get_frame_width() * 0.5) - newExplosion->get_frame_width() * 0.5;
							int py_pos = (p_player.GetRect().y + p_player.get_frame_height() * 0.5) - newExplosion->get_frame_height() * 0.5;
							newExplosion->SetRect(px_pos, py_pos);
							explosion_list.push_back(newExplosion);
							num_die++;
							if (num_die < 3)
							{
								p_player.undie_time = 200;
								p_player.set_comback_time(60);
								Mix_PlayChannel(MIX_CHANNEL, gLose, NOT_REPEATITIVE);
								player_power.Decrease();
								player_power.Render(g_screen);
								continue;
							}
							else
							{
								Mix_PauseMusic();
								Mix_PlayChannel(MIX_CHANNEL, gLose, NOT_REPEATITIVE);
								LoadMenuQuit(g_menu_screen,g_screen,gMenuMusic,gMove,gClick,gMusic,is_quit,quit,Play_Again);
							}
						}
					}
				}
				if (quit == true)
					break;
			}
			// player bullet
			std::vector<BulletObject *> bullet_arr = p_player.get_bullet_list();
			for (int r = 0; r < bullet_arr.size(); r++)
			{
				BulletObject *p_bullet = bullet_arr.at(r);
				if (p_bullet != NULL)
				{
					for (int t = 0; t < threats_list.size(); ++t)
					{
						ThreatsObject *obj_threat = threats_list.at(t);
						if (obj_threat != NULL)
						{
							SDL_Rect tRect = obj_threat->GetRectFrame();
							SDL_Rect bRect = p_bullet->GetRect();

							bool bCol = CheckCollision(bRect, tRect);

							if (bCol)
							{
								ExplosionObject *newExplosion = new ExplosionObject();
								newExplosion->LoadImg("img//explosion//Explosion_two.png", g_screen, NUM_FRAME_EXP);
								newExplosion->set_clip(NUM_FRAME_EXP);
								int tx_pos = obj_threat->GetRect().x - newExplosion->get_frame_width() * 0.4;
								int ty_pos = obj_threat->GetRect().y - newExplosion->get_frame_height() * 0.4;
								newExplosion->SetRect(tx_pos, ty_pos);
								explosion_list.push_back(newExplosion);
								p_player.RemoveBullet(r);
								obj_threat->Free();
								threats_list.erase(threats_list.begin() + t);
							}
						}
					}
				}
			}
			// Load explosion
			for (int ex = 0; ex < explosion_list.size(); ex++)
			{
				ExplosionObject *newExplosion = explosion_list.at(ex);
				if (newExplosion != NULL)
				{
					newExplosion->Show(g_screen, NUM_FRAME_EXP);
					if (newExplosion->get_animation_loop() > 0)
					{
						newExplosion->Free();
						delete newExplosion;
						explosion_list.erase(explosion_list.begin() + ex);
					}
				}
			}
			// Show score and money
			ShowScore(score,score_game,font_,g_screen);
			ShowBestScore(bscore,score,best_score,font_,g_screen);
			ShowMoney(money,money_game,font_,g_screen);
			SDL_RenderPresent(g_screen);

			int real_imp_time = SDL_GetTicks() - start_tick_;
			int time_one_frame = 1000 / FRAME_PER_SECOND; // ms

			if (real_imp_time < time_one_frame)
			{
				int delay_time = time_one_frame - real_imp_time;
				if (delay_time >= 0)
				{
					SDL_Delay(delay_time);
				}
			}
		}
		// Delete Threat
		if (!Play_Again)
		{
			for (int i = 0; i < threats_list.size(); i++)
			{
				ThreatsObject *p_threat = threats_list.at(i);
				if (p_threat)
				{
					p_threat->Free();
				}
			}
			threats_list.clear();
			for (int i = 0; i < coins_list.size(); i++)
			{
				Item *coin = coins_list.at(i);
				if (coin)
				{
					coin->Free();
				}
			}
			coins_list.clear();
		}
		SaveBestScoreToFile(bscore);
	}
	close();
	return 0;
}
