#include "HeaderFile/CommonFunc.h"
#include "HeaderFile/BaseObject.h"
#include "HeaderFile/game_map.h"
#include "HeaderFile/MainObject.h"
#include "HeaderFile/ImpTimer.h"
#include "HeaderFile/ThreatsObject.h"
#include "HeaderFile/ExplosionObject.h"
#include "HeaderFile/TextObject.h"
#include "HeaderFile/PlayerPower.h"
#include "HeaderFile/Geometric.h"
#include "HeaderFile/BossObject.h"
#include <SDL2/SDL_mixer.h>
#include "thread"
#include "chrono"
BaseObject g_background;
BaseObject g_backgroundTexture[BACKGROUND_LAYER];
BaseObject g_menu_screen;
int OffsetSpeed_Ground = BASE_OFFSET_SPEED;
std::vector <double> OffsetSpeed_Bkgr(BACKGROUND_LAYER, BASE_OFFSET_SPEED);	
Mix_Music *gMusic = nullptr;
Mix_Music *gMenuMusic = nullptr;
Mix_Chunk *gClick = nullptr;
Mix_Chunk *gLose = nullptr;
Mix_Chunk *gMove = nullptr;
Mix_Chunk *gShot = nullptr;
TTF_Font *font_time = NULL;
TTF_Font *g_font_MENU = NULL;
const std::string LAYER[BACKGROUND_LAYER] = {
	"img//background//1.png",
	"img//background//2.png",
	"img//background//3.png",
	"img//background//4.png",
	"img//background//5.png",
	"img//background//6.png",
	"img//background//7.png",
};
std::string ConvertWideToANSI(const wchar_t *wideStr)
{
	int len = WideCharToMultiByte(CP_ACP, 0, wideStr, -1, nullptr, 0, nullptr, nullptr);
	std::string result(len, '\0');
	WideCharToMultiByte(CP_ACP, 0, wideStr, -1, &result[0], len, nullptr, nullptr);
	return result;
}

bool InitData()
{
	bool success = true;
	int ret = SDL_Init(SDL_INIT_VIDEO);
	if (ret < 0)
		return false;
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

	g_window = SDL_CreateWindow("Game Cpp SDL 2.0", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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
		font_time = TTF_OpenFont("font//dlxfont_.ttf", 15);
		if (font_time == NULL)
		{
			success = false;
		}
	}
	return success;
}

void LoadBackground()
{
	std::vector <double> layer_speed;
	layer_speed.push_back(LAYER_1_SPEED);
	layer_speed.push_back(LAYER_2_SPEED);
	layer_speed.push_back(LAYER_3_SPEED);
	layer_speed.push_back(LAYER_4_SPEED);
	layer_speed.push_back(LAYER_5_SPEED);
	layer_speed.push_back(LAYER_6_SPEED);
	layer_speed.push_back(LAYER_7_SPEED);
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
	gMusic = Mix_LoadMUS("sound/bkgr_audio.wav");
	if (gMusic == nullptr)
	{
		success = false;
	}

	gMenuMusic = Mix_LoadMUS("sound/menu_audio.wav");
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
	return success;
}
void close()
{
	g_background.Free();
	SDL_DestroyRenderer(g_screen);
	g_screen = NULL;

	SDL_DestroyWindow(g_window);
	g_window = NULL;
	Mix_FreeMusic(gMusic);
	Mix_FreeMusic(gMenuMusic);
	Mix_FreeChunk(gClick);
	Mix_FreeChunk(gLose);
	gMusic = nullptr;
	gMenuMusic = nullptr;
	gClick = nullptr;
	gLose = nullptr;
	gMove = nullptr;
	gShot = nullptr;
	IMG_Quit();
	Mix_Quit();
	SDL_Quit();
}

std::vector<ThreatsObject *> MakeThreatList()
{
	std::vector<ThreatsObject *> list_threats;

	ThreatsObject *dymaic_threats = new ThreatsObject[75];

	for (int i = 0; i < 30; i++)
	{
		ThreatsObject *p_threat = (dymaic_threats + i);
		if (p_threat != NULL)
		{
			p_threat->LoadImg("img//threat//suriken_2_left.png", g_screen);
			p_threat->set_clips();
			p_threat->set_type_threat(ThreatsObject::THREAT_SURIKEN);
			p_threat->set_type_move(ThreatsObject::MOVE_IN_SPACE_THREAT);
			p_threat->set_x_pos(500 + i * 600);
			p_threat->set_y_pos(500);
			p_threat->set_input_left(1);
			int pos1 = p_threat->get_x_pos() - 100;
			int pos2 = p_threat->get_x_pos() + 100;
			p_threat->SetAnimationPos(pos1, pos2);
			list_threats.push_back(p_threat);
		}
	}
	// second dynamic
	for (int i = 30; i < 60; i++)
	{
		ThreatsObject *p_threat = (dymaic_threats + i);
		if (p_threat != NULL)
		{
			p_threat->LoadImg("img//threat//threat2_left.png", g_screen);
			p_threat->set_clips();
			p_threat->set_type_threat(ThreatsObject::THREAT_2);
			p_threat->set_type_move(ThreatsObject::MOVE_IN_SPACE_THREAT);
			p_threat->set_x_pos(700 + (i - 30) * 500);
			p_threat->set_y_pos(200);
			p_threat->set_input_left(1);
			int pos1 = p_threat->get_x_pos() - 60;
			int pos2 = p_threat->get_x_pos() + 60;
			BulletObject *p_bullet = new BulletObject();
			p_threat->InitBullet(p_bullet, g_screen);
			p_threat->SetAnimationPos(pos1, pos2);
			list_threats.push_back(p_threat);
		}
	}
	// plane threat
	for (int i = 60; i < 75; i++)
	{
		ThreatsObject *p_threat = (dymaic_threats + i);
		if (p_threat != NULL)
		{
			p_threat->LoadImg("img//threat//plane_left.png", g_screen);
			p_threat->set_clips();
			p_threat->set_type_threat(ThreatsObject::THREAT_PLANE);
			p_threat->set_type_move(ThreatsObject::MOVE_IN_SPACE_THREAT);
			p_threat->set_x_pos(700 + (i - 60) * 1280);
			p_threat->set_y_pos(100);
			p_threat->set_input_left(1);
			int pos1 = p_threat->get_x_pos() - 150;
			int pos2 = p_threat->get_x_pos() + 150;
			BulletObject *p_bullet = new BulletObject();
			p_threat->InitBullet(p_bullet, g_screen);
			p_threat->SetAnimationPos(pos1, pos2);
			list_threats.push_back(p_threat);
		}
	}
	return list_threats;
}


int main(int argc, char *args[])
{
	ImpTimer fps_timer;

	if (InitData() == false)
		return -1;
	if (LoadMedia() == false)
		return -1;
	// Make menu game
	GameMap game_map;
	

	MainObject p_player;
	p_player.LoadImg("img//PLAYER//WALK_RIGHT.png", g_screen);
	p_player.set_clip();
	

	PlayerPower player_power;

	PlayerMoney player_money;
	player_money.Init(g_screen);
	player_money.SetPos(SCREEN_WIDTH * 0.5 - 300, 8);

	// Boss Threat
	BossObject boss_object;
	boss_object.LoadImg("img//boss_object.png", g_screen);
	

	// Explosision
	ExplosionObject exp_threat;
	bool tRet = exp_threat.LoadImg("img//explosion//Explosion_two.png", g_screen, NUM_FRAME_EXP);
	if (!tRet)
		return -1;
	ExplosionObject exp_main;
	bool mRet = exp_main.LoadImg("img//PLAYER//DEAD_RIGHT.png", g_screen, MAIN_FRAME_EXP);
	exp_main.set_clip(MAIN_FRAME_EXP);
	if (!mRet)
		return -1;
	// Time text
	TextObject time_game;
	time_game.SetColor(TextObject::WHITE_TEXT);
	// Score text
	TextObject score_game;
	score_game.SetColor(TextObject::WHITE_TEXT);

	TextObject money_game;
	money_game.SetColor(TextObject::WHITE_TEXT);
	// BEST_SCORE

	// menu game
	bool is_quit = false;

	g_menu_screen.LoadImg("img//menu//menu.png", g_screen);
	BaseObject PlayButton[2];
	BaseObject ExitButton[2];
	BaseObject ContinueButton[2];

	PlayButton[0].LoadImg("img//menu//PLAY_N.png", g_screen);
	PlayButton[1].LoadImg("img//menu//PLAY.png", g_screen);
	ContinueButton[0].LoadImg("img//menu//CONTINUE_N.png", g_screen);
	ContinueButton[1].LoadImg("img//menu//CONTINUE.png", g_screen);
	ExitButton[0].LoadImg("img//menu//EXIT_N.png", g_screen);
	ExitButton[1].LoadImg("img//menu//EXIT.png", g_screen);

	PlayButton[0].SetRect(360, 260);
	PlayButton[1].SetRect(360, 260);

	ContinueButton[0].SetRect(360, 350);
	ContinueButton[1].SetRect(360, 350);

	ExitButton[0].SetRect(360, 440);
	ExitButton[1].SetRect(360, 440);

	int MousePosX = 0;
	int MousePosY = 0;
	bool menu_quit = false;
	bool Play_Again = false;
	Mix_PlayMusic(gMenuMusic, IS_REPEATITIVE);
	while (!menu_quit)
	{
		SDL_RenderClear(g_screen);
		g_menu_screen.Render(g_screen, NULL);
		while (SDL_PollEvent(&g_event) != 0)
		{
			if (g_event.type == SDL_MOUSEMOTION)
			{
				MousePosX = g_event.motion.x;
				MousePosY = g_event.motion.y;
				if (SDLCommonFunc::CheckMousePos(MousePosX, MousePosY, PlayButton[0].GetRect()) == true)
				{
					Mix_PlayChannel(MIX_CHANNEL, gMove, 0);
				}
				if (SDLCommonFunc::CheckMousePos(MousePosX, MousePosY, ContinueButton[0].GetRect()) == true)
				{
					Mix_PlayChannel(MIX_CHANNEL, gMove, 0);
				}
				if (SDLCommonFunc::CheckMousePos(MousePosX, MousePosY, ExitButton[0].GetRect()) == true)
				{
					Mix_PlayChannel(MIX_CHANNEL, gMove, 0);
				}
			}
			if (g_event.type == SDL_MOUSEBUTTONDOWN)
			{
				if (g_event.button.button == SDL_BUTTON_LEFT)
				{
					if (SDLCommonFunc::CheckMousePos(MousePosX, MousePosY, PlayButton[0].GetRect()) == true)
					{
						menu_quit = true;
						// g_menu_screen.Free();
						Play_Again = true;
						Mix_PlayChannel(MIX_CHANNEL, gClick, 0);
					}

					if (SDLCommonFunc::CheckMousePos(MousePosX, MousePosY, ExitButton[0].GetRect()) == true)
					{
						menu_quit = true;
						is_quit = true;
						Mix_PlayChannel(MIX_CHANNEL, gClick, 0);
						g_menu_screen.Free();
					}
				}
			}
		}
		PlayButton[0].Render(g_screen);
		ContinueButton[0].Render(g_screen);
		ExitButton[0].Render(g_screen);
		if (SDLCommonFunc::CheckMousePos(MousePosX, MousePosY, PlayButton[0].GetRect()) == true)
		{
			PlayButton[1].Render(g_screen);
		}
		if (SDLCommonFunc::CheckMousePos(MousePosX, MousePosY, ContinueButton[0].GetRect()) == true)
		{
			ContinueButton[1].Render(g_screen);
		}
		if (SDLCommonFunc::CheckMousePos(MousePosX, MousePosY, ExitButton[0].GetRect()) == true)
		{
			ExitButton[1].Render(g_screen);
		}
		SDL_RenderPresent(g_screen);
	}

	while (Play_Again)
	{
		Mix_PlayMusic(gMusic, IS_REPEATITIVE);
		game_map.Load_Map("map//map01.dat");
		game_map.LoadTiles(g_screen);

		p_player.InitMoneyCount();
		p_player.InitScoreCount();
		
		int num_die = 0;
		is_quit = false;

		p_player.set_x_pos(0);
		p_player.set_y_pos(GROUND);
		player_power.Init(g_screen);

		std::vector<ThreatsObject *> threats_list = MakeThreatList();

		boss_object.set_clips();
		int xPosBoss = MAX_MAP_X * TILE_SIZE - SCREEN_WIDTH * 0.6;
		boss_object.set_xpos(xPosBoss);
		boss_object.set_ypos(10);

		std::ifstream file("BEST_SCORE.txt");
		int bscore;
		file >> bscore;
		TextObject best_score;
		best_score.SetColor(TextObject::WHITE_TEXT);
		std::string b_score = std::to_string(bscore);
		std::string strb_score("BEST: ");
		strb_score += b_score;
		std::vector<ExplosionObject *> explosion_list;
		// map
		
		while (!is_quit)
		{
			fps_timer.start();
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
					g_menu_screen.LoadImg("img//menu//menu.png", g_screen);
					BaseObject ResumeButton[2];
					BaseObject ExitButton[2];
					ResumeButton[0].LoadImg("img//menu//RESUME_N.png", g_screen);
					ResumeButton[1].LoadImg("img//menu//RESUME.png", g_screen);
					ExitButton[0].LoadImg("img//menu//EXIT_N.png", g_screen);
					ExitButton[1].LoadImg("img//menu//EXIT.png", g_screen);
					SDL_Rect Button = ResumeButton[0].GetRect();
					ResumeButton[0].SetRect(360, 260);
					ResumeButton[1].SetRect(360, 260);
					Button = ExitButton[0].GetRect();
					ExitButton[0].SetRect(360, 350);
					ExitButton[1].SetRect(360, 350);
					int MousePosX = 0;
					int MousePosY = 0;
					bool menu_p_quit = false;
					while (!menu_p_quit)
					{
						SDL_RenderClear(g_screen);
						g_menu_screen.Render(g_screen, NULL);
						while (SDL_PollEvent(&two_event) != 0)
						{
							if (two_event.type == SDL_MOUSEMOTION)
							{
								MousePosX = two_event.motion.x;
								MousePosY = two_event.motion.y;
								if (SDLCommonFunc::CheckMousePos(MousePosX, MousePosY, ResumeButton[0].GetRect()) == true)
								{
									Mix_PlayChannel(MIX_CHANNEL, gMove, 0);
								}
								if (SDLCommonFunc::CheckMousePos(MousePosX, MousePosY, ExitButton[0].GetRect()) == true)
								{
									Mix_PlayChannel(MIX_CHANNEL, gMove, 0);
								}
							}
							if (two_event.type == SDL_MOUSEBUTTONDOWN)
							{
								if (two_event.button.button == SDL_BUTTON_LEFT)
								{
									if (SDLCommonFunc::CheckMousePos(MousePosX, MousePosY, ResumeButton[0].GetRect()) == true)
									{
										menu_p_quit = true;
										Mix_PlayChannel(MIX_CHANNEL, gClick, 0);
										Mix_PlayMusic(gMusic, IS_REPEATITIVE);
										g_menu_screen.Free();
									}
									if (SDLCommonFunc::CheckMousePos(MousePosX, MousePosY, ExitButton[0].GetRect()) == true)
									{
										menu_p_quit = true;
										is_quit = true;
										Play_Again = false;
										Mix_PlayChannel(MIX_CHANNEL, gClick, 0);
										g_menu_screen.Free();
									}
								}
							}
						}
						ResumeButton[0].Render(g_screen);
						ExitButton[0].Render(g_screen);
						if (SDLCommonFunc::CheckMousePos(MousePosX, MousePosY, ResumeButton[0].GetRect()) == true)
						{
							ResumeButton[1].Render(g_screen);
						}
						if (SDLCommonFunc::CheckMousePos(MousePosX, MousePosY, ExitButton[0].GetRect()) == true)
						{
							ExitButton[1].Render(g_screen);
						}
						SDL_RenderPresent(g_screen);
					}
					p_player.is_pause = false;
				}
			}
			SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
			SDL_RenderClear(g_screen);
			
			LoadBackground();
			Map map_data = game_map.getMap();

			p_player.HandleBullet(g_screen);
			p_player.SetMapXY(map_data.start_x_, map_data.start_y_);
			p_player.DoPlayerX(map_data);
			p_player.Show(g_screen, MAIN_FRAME);

			game_map.SetMap(map_data);
			game_map.DrawMap(g_screen);

			// Draw Geometric
			GeometricFormat rectangle_size(0, 0, SCREEN_WIDTH, 40);
			ColorData color_data(36, 36, 36);
			Geometric::RenderRectangle(rectangle_size, color_data, g_screen);

			GeometricFormat outLineSize(1, 1, SCREEN_WIDTH - 1, 38);
			ColorData color_data2(255, 255, 255);

			Geometric::RenderOutline(outLineSize, color_data2, g_screen);

			player_power.Show(g_screen);
			player_money.Show(g_screen);

			// Threat
			bool quit = false;
			for (int i = 0; i < threats_list.size(); i++)
			{
				ThreatsObject *p_threat = threats_list.at(i);
				if (p_threat != NULL)
				{
					p_threat->SetMapXY(map_data.start_x_, map_data.start_y_);
					p_threat->ImpMoveType(g_screen);
					p_threat->DoPlayer(map_data);

					p_threat->Show(g_screen);
					p_threat->MakeBullet(g_screen, SCREEN_WIDTH, SCREEN_HEIGHT);

					SDL_Rect rect_player = p_player.GetRectFrame();
					bool bCol1 = false;
					std::vector<BulletObject *> tBullet_list = p_threat->get_bullet_list();
					for (int jj = 0; jj < tBullet_list.size(); ++jj)
					{
						BulletObject *pt_bullet = tBullet_list.at(jj);
						if (pt_bullet)
						{
							SDL_Rect rect_bullet = pt_bullet->GetRect();
							bCol1 = SDLCommonFunc::CheckCollision(rect_bullet, rect_player) && pt_bullet->get_is_move();
							if (bCol1)
							{
								// pt_bullet->set_is_move(false);
								break;
							}
						}
					}
					SDL_Rect rect_threat = p_threat->GetRectFrame();
					bool bCol2 = SDLCommonFunc::CheckCollision(rect_player, rect_threat);

					// if (bCol1 || bCol2)
					// {
					// 	ExplosionObject *newExplosion = new ExplosionObject();
					// 	newExplosion->LoadImg("img//PLAYER//DEAD_RIGHT.png", g_screen, MAIN_FRAME_EXP);
					// 	newExplosion->set_clip(MAIN_FRAME_EXP);
					// 	int px_pos = (p_player.GetRect().x + p_player.get_frame_width() * 0.5) - exp_main.get_frame_width() * 0.5;
					// 	int py_pos = (p_player.GetRect().y + p_player.get_frame_height() * 0.5) - exp_main.get_frame_height() * 0.5;
					// 	newExplosion->SetRect(px_pos, py_pos);
					// 	explosion_list.push_back(newExplosion);
					// 	num_die++;
					// 	if (num_die < 3)
					// 	{
					// 		p_player.SetRect(0, 0);
					// 		p_player.set_comback_time(60);
					// 		// SDL_Delay(1000);
					// 		Mix_PlayChannel(MIX_CHANNEL, gLose, NOT_REPEATITIVE);
					// 		player_power.Decrease();
					// 		player_power.Render(g_screen);
					// 		continue;
					// 	}
					// 	else
					// 	{
					// 		Mix_PauseMusic();
					// 		Mix_PlayChannel(MIX_CHANNEL, gLose, NOT_REPEATITIVE);
					// 		g_menu_screen.LoadImg("img//menu//o_menu.png", g_screen);
					// 		BaseObject ExitButton[2];
					// 		ExitButton[0].LoadImg("img//menu//EXIT_N.png", g_screen);
					// 		ExitButton[1].LoadImg("img//menu//EXIT.png", g_screen);
					// 		ExitButton[0].SetRect(360, 350);
					// 		ExitButton[1].SetRect(360, 350);
					// 		BaseObject NewGameButton[2];
					// 		NewGameButton[0].LoadImg("img//menu//NEWGAME_N.png", g_screen);
					// 		NewGameButton[1].LoadImg("img//menu//NEWGAME.png", g_screen);
					// 		NewGameButton[0].SetRect(360, 260);
					// 		NewGameButton[1].SetRect(360, 260);
					// 		int MousePosX = 0;
					// 		int MousePosY = 0;
					// 		bool menu_p_quit = false;
					// 		while (!menu_p_quit)
					// 		{
					// 			SDL_RenderClear(g_screen);
					// 			g_menu_screen.Render(g_screen, NULL);
					// 			while (SDL_PollEvent(&two_event) != 0)
					// 			{
					// 				if (two_event.type == SDL_MOUSEMOTION)
					// 				{
					// 					MousePosX = two_event.motion.x;
					// 					MousePosY = two_event.motion.y;
					// 					if (SDLCommonFunc::CheckMousePos(MousePosX, MousePosY, ExitButton[0].GetRect()) == true)
					// 					{
					// 						Mix_PlayChannel(MIX_CHANNEL, gMove, 0);
					// 					}
					// 					if (SDLCommonFunc::CheckMousePos(MousePosX, MousePosY, NewGameButton[0].GetRect()) == true)
					// 					{
					// 						Mix_PlayChannel(MIX_CHANNEL, gMove, 0);
					// 					}
					// 				}
					// 				if (two_event.type == SDL_MOUSEBUTTONDOWN)
					// 				{
					// 					if (two_event.button.button == SDL_BUTTON_LEFT)
					// 					{
					// 						if (SDLCommonFunc::CheckMousePos(MousePosX, MousePosY, ExitButton[0].GetRect()) == true)
					// 						{
					// 							menu_p_quit = true;
					// 							is_quit = true;
					// 							Play_Again = false;
					// 							g_menu_screen.Free();
					// 							quit = true;
					// 							// p_threat->Free();
					// 							// close();
					// 							// SDL_Quit();
					// 							// return 0;
					// 						}
					// 						if (SDLCommonFunc::CheckMousePos(MousePosX, MousePosY, NewGameButton[0].GetRect()) == true)
					// 						{
					// 							menu_p_quit = true;
					// 							is_quit = true;
					// 							quit = false;
					// 							Play_Again = true;
					// 						}
					// 					}
					// 				}
					// 			}
					// 			ExitButton[0].Render(g_screen);
					// 			NewGameButton[0].Render(g_screen);
					// 			if (SDLCommonFunc::CheckMousePos(MousePosX, MousePosY, ExitButton[0].GetRect()) == true)
					// 			{
					// 				ExitButton[1].Render(g_screen);
					// 			}
					// 			if (SDLCommonFunc::CheckMousePos(MousePosX, MousePosY, NewGameButton[0].GetRect()) == true)
					// 			{
					// 				NewGameButton[1].Render(g_screen);
					// 			}
					// 			SDL_RenderPresent(g_screen);
					// 		}
					// 	}
					// }
				}
				if (quit == true) break;
			}

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
							SDL_Rect tRect;
							tRect.x = obj_threat->GetRect().x;
							tRect.y = obj_threat->GetRect().y;
							tRect.w = obj_threat->get_width_frame();
							tRect.h = obj_threat->get_height_frame();

							SDL_Rect bRect = p_bullet->GetRect();

							bool bCol = SDLCommonFunc::CheckCollision(bRect, tRect);

							if (bCol)
							{
								ExplosionObject *newExplosion = new ExplosionObject();
								newExplosion->LoadImg("img//explosion//Explosion_two.png", g_screen, NUM_FRAME_EXP);
								newExplosion->set_clip(NUM_FRAME_EXP);
								int tx_pos = obj_threat->GetRect().x - exp_threat.get_frame_width() * 0.4;
								int ty_pos = obj_threat->GetRect().y - exp_threat.get_frame_height() * 0.4;
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
					newExplosion->SetMapXY(map_data.start_x_, map_data.start_y_);

					newExplosion->Show(g_screen, NUM_FRAME_EXP);
					if (newExplosion->get_animation_loop() > 0)
					{
						newExplosion->Free();
						explosion_list.erase(explosion_list.begin() + ex);
					}
				}
			}
			// Show game time
			std::string str_time = "TIME: ";
			Uint32 time_val = SDL_GetTicks() / 1000;
			Uint32 val_time = 300 - time_val;
			if (val_time <= 0)
			{
				if (MessageBox(NULL, ConvertWideToANSI(L"GAME OVER").c_str(), ConvertWideToANSI(L"Info").c_str(), (MB_OK | MB_ICONSTOP)) == IDOK)
				{
					is_quit = true;
					break;
				}
			}
			else
			{
				std::string str_val = std::to_string(val_time);
				str_time += str_val;

				time_game.SetText(str_time);
				time_game.LoadFromRenderText(font_time, g_screen);
				time_game.RenderText(g_screen, SCREEN_WIDTH - 200, 15);
			}
			// Show score and money
			int score = p_player.GetScoreCount();
			std::string score_str = std::to_string(score);
			std::string strScore("SCORE: ");
			strScore += score_str;
			score_game.SetText(strScore);
			score_game.LoadFromRenderText(font_time, g_screen);
			score_game.RenderText(g_screen, SCREEN_WIDTH * 0.5 - 50, 15);

			if (score > bscore)
			{
				bscore = score;
				b_score = std::to_string(bscore);
				strb_score = "BEST: ";
				strb_score += b_score;
			}
			best_score.SetText(strb_score);
			best_score.LoadFromRenderText(font_time, g_screen);
			best_score.RenderText(g_screen, SCREEN_WIDTH * 0.5 + 200, 15);
			int money_count = p_player.GetMoneyCount();
			std::string money_str = std::to_string(money_count);

			money_game.SetText(money_str);
			money_game.LoadFromRenderText(font_time, g_screen);
			money_game.RenderText(g_screen, SCREEN_WIDTH * 0.5 - 250, 15);

			// Show boss
			int val = MAX_MAP_X * TILE_SIZE - (map_data.start_x_ + p_player.GetRect().x);
			if (val <= SCREEN_WIDTH)
			{
				boss_object.SetMapXY(map_data.start_x_, map_data.start_y_);
				boss_object.DoPlayer(map_data);
				boss_object.MakeBullet(g_screen, SCREEN_WIDTH, SCREEN_HEIGHT);
				boss_object.Show(g_screen);
			}
			SDL_RenderPresent(g_screen);

			int real_imp_time = fps_timer.get_ticks();
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
		for (int i = 0; i < threats_list.size(); i++)
			{
				ThreatsObject *p_threat = threats_list.at(i);
				if (p_threat)
				{
					p_threat->Free();
					p_threat = NULL;
				}
			}
		threats_list.clear();
		file.close();
		std::ofstream outfile("BEST_SCORE.txt");
		outfile << bscore;
		outfile.close();
	}
	close();
	return 0;
}
