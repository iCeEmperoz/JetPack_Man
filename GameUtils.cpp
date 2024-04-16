#include "HeaderFile/GameUtils.h"

std::vector<ThreatsObject *> MakeThreatList(SDL_Renderer *screen)
{
	std::vector<ThreatsObject *> list_threats;

	ThreatsObject *dymaic_threats = new ThreatsObject[5];
	ThreatsObject *p_threat = (dymaic_threats);
	for (int i = 0; i < 2; i++)
	{
		p_threat = (dymaic_threats + i);
		if (p_threat != NULL)
		{
			p_threat->LoadImg("img//threat//threat2_left.png", screen);
			p_threat->set_clips();
			p_threat->set_type_threat(ThreatsObject::THREAT_2);
			p_threat->set_x_pos(rand() % (SCREEN_WIDTH + THREAT_POSITION_RANGE) + SCREEN_WIDTH);
			p_threat->set_y_pos(rand() % (SCREEN_HEIGHT - 120 - 120) + 120);
			BulletObject *p_bullet = new BulletObject();
			p_threat->InitBullet(p_bullet, screen);
			list_threats.push_back(p_threat);
		}
	}
	// plane
	for (int i = 2; i < 4; i++)
	{
		p_threat = (dymaic_threats + i);
		if (p_threat != NULL)
		{
			p_threat->LoadImg("img//threat//plane_left.png", screen);
			p_threat->set_clips();
			p_threat->set_type_threat(ThreatsObject::THREAT_PLANE);
			p_threat->set_x_pos(rand() % (SCREEN_WIDTH + THREAT_POSITION_RANGE) + SCREEN_WIDTH);
			p_threat->set_y_pos(rand() % (SCREEN_HEIGHT - 120 - 120) + 120);
			BulletObject *p_bullet = new BulletObject();
			p_threat->InitBullet(p_bullet, screen);
			list_threats.push_back(p_threat);
		}
	}
	return list_threats;
}

std::vector<Item *> MakeCoinList(int x, SDL_Renderer *screen)
{
	std::string filename = "coin//" + std::to_string(x) + ".txt";
	std::ifstream file(filename);
	int n, m, c;
	file >> n >> m;
	std::vector<Item *> coin_list;
	float xpos = rand() % (SCREEN_WIDTH + THREAT_POSITION_RANGE) + SCREEN_WIDTH;
	float ypos = rand() % (SCREEN_HEIGHT / 2);
	for (int i = 0; i < n; ++i)
	{
		ypos += 32;
		for (int j = 0; j < m; j++)
		{
			file >> c;
			if (c != 0)
			{
				Item *coin = new Item();
				coin->LoadImg("img//Item//COIN.png", screen, ITEM_FRAME_NUM);
				coin->set_clips(ITEM_FRAME_NUM);
				coin->set_x_pos(xpos + j * 32);
				coin->set_y_pos(ypos);
				coin_list.push_back(coin);
			}
		}
	}
	file.close();
	return coin_list;
}

Item *MakeRandomItem(int x, SDL_Renderer *screen)
{
	std::string name_item = "img//Item//" + std::to_string(x) + ".png";
	Item *item = new Item();
	float xpos = SCREEN_WIDTH + THREAT_POSITION_RANGE;
	float ypos = (SCREEN_HEIGHT / 2);
	item->LoadImg(name_item, screen, ITEM_FRAME_NUM);
	item->set_clips(ITEM_FRAME_NUM);
	item->set_x_pos(xpos);
	item->set_y_pos(ypos);
	item->set_y_start(ypos);
	return item;
}

void RecreateItem(Item *&item, int x, SDL_Renderer *screen)
{
	if (item != NULL && item->IsOffScreenLeft())
	{
		item->Free();
		delete item;
		item = NULL;
	}
	if (item == NULL)
	{
		item = MakeRandomItem(x, screen);
	}
}

void RecreateCoinList(std::vector<Item *> &coins_list, int x, SDL_Renderer *screen)
{
	// Kiểm tra xem danh sách có đồng xu nào đi ra khỏi màn hình không
	for (int i = 0; i < coins_list.size();)
	{
		Item *coin = coins_list.at(i);
		if (coin->IsOffScreenLeft())
		{
			coins_list.erase(coins_list.begin() + i);
			coin->Free();
		}
		else
			i++;
	}
	if (coins_list.empty())
	{
		coins_list = MakeCoinList(x, screen);
	}
}

void RecreateThreatList(std::vector<ThreatsObject *> &threats_list, SDL_Renderer *screen)
{
	for (int i = 0; i < threats_list.size();)
	{
		ThreatsObject *threat = threats_list.at(i);
		if (threat->IsOffScreenLeft())
		{
			threat->Free();
			threats_list.erase(threats_list.begin() + i);
		}
		else
			i++;
	}
	if (threats_list.empty())
	{
		threats_list = MakeThreatList(screen);
	}
}

void LoadMenuStart(BaseObject g_menu_screen,
				   SDL_Renderer *screen,
				   Mix_Music *&gMenuMusic,
				   Mix_Chunk *&gMove,
				   Mix_Chunk *&gClick,
				   bool &Play_Again)
{
	bool showControls = false;
	g_menu_screen.LoadImg("img//menu//menu.png", screen);
	BaseObject PlayButton[2];
	BaseObject ExitButton[2];
	BaseObject ControlButton[2];

	PlayButton[0].LoadImg("img//menu//PLAY_N.png", screen);
	PlayButton[1].LoadImg("img//menu//PLAY.png", screen);
	ExitButton[0].LoadImg("img//menu//EXIT_N.png", screen);
	ExitButton[1].LoadImg("img//menu//EXIT.png", screen);
	ControlButton[0].LoadImg("img//menu//CONTROL_N.png", screen);
	ControlButton[1].LoadImg("img//menu//CONTROL.png", screen);
	PlayButton[0].SetRect(360, 260);
	PlayButton[1].SetRect(360, 260);

	ControlButton[0].SetRect(360, 350);
	ControlButton[1].SetRect(360, 350);

	ExitButton[0].SetRect(360, 440);
	ExitButton[1].SetRect(360, 440);

	int MousePosX = 0;
	int MousePosY = 0;
	bool menu_quit = false;
	Mix_PlayMusic(gMenuMusic, IS_REPEATITIVE);
	while (!menu_quit)
	{
		SDL_RenderClear(screen);
		if (!showControls)
		{
			g_menu_screen.Render(screen, NULL);
			while (SDL_PollEvent(&g_event) != 0)
			{
				if (g_event.type == SDL_MOUSEMOTION)
				{
					MousePosX = g_event.motion.x;
					MousePosY = g_event.motion.y;
					if (CheckMousePos(MousePosX, MousePosY, PlayButton[0].GetRect()) == true)
					{
						Mix_PlayChannel(MIX_CHANNEL, gMove, 0);
					}
					if (CheckMousePos(MousePosX, MousePosY, ExitButton[0].GetRect()) == true)
					{
						Mix_PlayChannel(MIX_CHANNEL, gMove, 0);
					}
					if (CheckMousePos(MousePosX, MousePosY, ControlButton[0].GetRect()) == true)
					{
						Mix_PlayChannel(MIX_CHANNEL, gMove, 0);
					}
				}
				if (g_event.type == SDL_MOUSEBUTTONDOWN)
				{
					if (g_event.button.button == SDL_BUTTON_LEFT)
					{
						if (CheckMousePos(MousePosX, MousePosY, PlayButton[0].GetRect()) == true)
						{
							menu_quit = true;
							Play_Again = true;
							Mix_PlayChannel(MIX_CHANNEL, gClick, 0);
						}
						if (CheckMousePos(MousePosX, MousePosY, ControlButton[0].GetRect()) == true)
						{
							Mix_PlayChannel(MIX_CHANNEL, gClick, 0);
							showControls = true;
						}
						if (CheckMousePos(MousePosX, MousePosY, ExitButton[0].GetRect()) == true)
						{
							menu_quit = true;
							Mix_PlayChannel(MIX_CHANNEL, gClick, 0);
						}
					}
				}
			}

			PlayButton[0].Render(screen);
			ControlButton[0].Render(screen);
			ExitButton[0].Render(screen);
			if (CheckMousePos(MousePosX, MousePosY, PlayButton[0].GetRect()) == true)
			{
				PlayButton[1].Render(screen);
			}
			if (CheckMousePos(MousePosX, MousePosY, ControlButton[0].GetRect()) == true)
			{
				ControlButton[1].Render(screen);
			}
			if (CheckMousePos(MousePosX, MousePosY, ExitButton[0].GetRect()) == true)
			{
				ExitButton[1].Render(screen);
			}
		}
		else
		{
			g_menu_screen.LoadImg("img//menu//g_control.png", screen);
			g_menu_screen.Render(screen);
			BaseObject BackButton[2];
			BackButton[0].LoadImg("img//menu//BACK_N.png", screen);
			BackButton[1].LoadImg("img//menu//BACK.png", screen);
			BackButton[0].SetRect(900, 500);
			BackButton[1].SetRect(900, 500);
			while (SDL_PollEvent(&g_event) != 0)
			{
				if (g_event.type == SDL_MOUSEMOTION)
				{
					MousePosX = g_event.motion.x;
					MousePosY = g_event.motion.y;
					if (CheckMousePos(MousePosX, MousePosY, BackButton[0].GetRect()) == true)
					{
						Mix_PlayChannel(MIX_CHANNEL, gMove, 0);
					}
				}
				if (g_event.type == SDL_MOUSEBUTTONDOWN)
				{
					if (g_event.button.button == SDL_BUTTON_LEFT)
					{
						if (CheckMousePos(MousePosX, MousePosY, BackButton[0].GetRect()) == true)
						{
							showControls = false;
							Mix_PlayChannel(MIX_CHANNEL, gClick, 0);
							g_menu_screen.LoadImg("img//menu//menu.png", screen);
						}
					}
				}
			}

			BackButton[0].Render(screen);

			if (CheckMousePos(MousePosX, MousePosY, BackButton[0].GetRect()) == true)
			{
				BackButton[1].Render(screen);
			}
		}
		SDL_RenderPresent(screen);
	}
}

void LoadMenuPause(BaseObject g_menu_screen,
				   SDL_Renderer *screen,
				   Mix_Music *&gMenuMusic,
				   Mix_Chunk *&gMove,
				   Mix_Chunk *&gClick,
				   Mix_Music *&gMusic,
				   bool &is_quit,
				   bool &Play_Again)
{
	g_menu_screen.LoadImg("img//menu//menu.png", screen);
	BaseObject ResumeButton[2];
	BaseObject ExitButton[2];
	ResumeButton[0].LoadImg("img//menu//RESUME_N.png", screen);
	ResumeButton[1].LoadImg("img//menu//RESUME.png", screen);
	ExitButton[0].LoadImg("img//menu//EXIT_N.png", screen);
	ExitButton[1].LoadImg("img//menu//EXIT.png", screen);
	ResumeButton[0].SetRect(360, 260);
	ResumeButton[1].SetRect(360, 260);
	ExitButton[0].SetRect(360, 350);
	ExitButton[1].SetRect(360, 350);
	int MousePosX = 0;
	int MousePosY = 0;
	bool menu_p_quit = false;
	while (!menu_p_quit)
	{
		SDL_RenderClear(screen);
		g_menu_screen.Render(screen, NULL);
		while (SDL_PollEvent(&two_event) != 0)
		{
			if (two_event.type == SDL_MOUSEMOTION)
			{
				MousePosX = two_event.motion.x;
				MousePosY = two_event.motion.y;
				if (CheckMousePos(MousePosX, MousePosY, ResumeButton[0].GetRect()) == true)
				{
					Mix_PlayChannel(MIX_CHANNEL, gMove, 0);
				}
				if (CheckMousePos(MousePosX, MousePosY, ExitButton[0].GetRect()) == true)
				{
					Mix_PlayChannel(MIX_CHANNEL, gMove, 0);
				}
			}
			if (two_event.type == SDL_MOUSEBUTTONDOWN)
			{
				if (two_event.button.button == SDL_BUTTON_LEFT)
				{
					if (CheckMousePos(MousePosX, MousePosY, ResumeButton[0].GetRect()) == true)
					{
						menu_p_quit = true;
						Mix_PlayChannel(MIX_CHANNEL, gClick, 0);
						Mix_PlayMusic(gMusic, IS_REPEATITIVE);
						// g_menu_screen.Free();
					}
					if (CheckMousePos(MousePosX, MousePosY, ExitButton[0].GetRect()) == true)
					{
						menu_p_quit = true;
						is_quit = true;
						Play_Again = false;
						Mix_PlayChannel(MIX_CHANNEL, gClick, 0);
						// g_menu_screen.Free();
					}
				}
			}
		}
		ResumeButton[0].Render(screen);
		ExitButton[0].Render(screen);
		if (CheckMousePos(MousePosX, MousePosY, ResumeButton[0].GetRect()) == true)
		{
			ResumeButton[1].Render(screen);
		}
		if (CheckMousePos(MousePosX, MousePosY, ExitButton[0].GetRect()) == true)
		{
			ExitButton[1].Render(screen);
		}
		SDL_RenderPresent(screen);
	}
}

void LoadMenuQuit(BaseObject g_menu_screen, 
                    SDL_Renderer *screen, 
                    Mix_Music *&gMenuMusic, 
                    Mix_Chunk *&gMove, 
                    Mix_Chunk *&gClick, 
                    Mix_Music *&gMusic,
                    bool &is_quit,
					bool &quit,
                    bool &Play_Again)
{
	g_menu_screen.LoadImg("img//menu//o_menu.png", screen);
	BaseObject ExitButton[2];
	ExitButton[0].LoadImg("img//menu//EXIT_N.png", screen);
	ExitButton[1].LoadImg("img//menu//EXIT.png", screen);
	ExitButton[0].SetRect(360, 350);
	ExitButton[1].SetRect(360, 350);
	BaseObject NewGameButton[2];
	NewGameButton[0].LoadImg("img//menu//NEWGAME_N.png", screen);
	NewGameButton[1].LoadImg("img//menu//NEWGAME.png", screen);
	NewGameButton[0].SetRect(360, 260);
	NewGameButton[1].SetRect(360, 260);
	int MousePosX = 0;
	int MousePosY = 0;
	bool menu_p_quit = false;
	while (!menu_p_quit)
	{
		SDL_RenderClear(screen);
		g_menu_screen.Render(screen);
		while (SDL_PollEvent(&two_event) != 0)
		{
			if (two_event.type == SDL_MOUSEMOTION)
			{
				MousePosX = two_event.motion.x;
				MousePosY = two_event.motion.y;
				if (CheckMousePos(MousePosX, MousePosY, ExitButton[0].GetRect()) == true)
				{
					Mix_PlayChannel(MIX_CHANNEL, gMove, 0);
				}
				if (CheckMousePos(MousePosX, MousePosY, NewGameButton[0].GetRect()) == true)
				{
					Mix_PlayChannel(MIX_CHANNEL, gMove, 0);
				}
			}
			if (two_event.type == SDL_MOUSEBUTTONDOWN)
			{
				if (two_event.button.button == SDL_BUTTON_LEFT)
				{
					if (CheckMousePos(MousePosX, MousePosY, ExitButton[0].GetRect()) == true)
					{
						menu_p_quit = true;
						is_quit = true;
						Play_Again = false;
						quit = true;
					}
					if (CheckMousePos(MousePosX, MousePosY, NewGameButton[0].GetRect()) == true)
					{
						menu_p_quit = true;
						is_quit = true;
						quit = false;
						Play_Again = true;
					}
				}
			}
		}
		ExitButton[0].Render(screen);
		NewGameButton[0].Render(screen);
		if (CheckMousePos(MousePosX, MousePosY, ExitButton[0].GetRect()) == true)
		{
			ExitButton[1].Render(screen);
		}
		if (CheckMousePos(MousePosX, MousePosY, NewGameButton[0].GetRect()) == true)
		{
			NewGameButton[1].Render(screen);
		}
		SDL_RenderPresent(screen);
	}
}