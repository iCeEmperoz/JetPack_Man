#include "HeaderFile/CommonFunc.h"


bool CheckCollision(const SDL_Rect& object1, const SDL_Rect& object2) 
{
    int TRASH_PIXEL_1 = 10; // Điều chỉnh trash pixel 
    int TRASH_PIXEL_2 = 5; // Điều chỉnh trash pixel

    int left_a = object1.x - TRASH_PIXEL_1;
    int right_a = left_a + object1.w - TRASH_PIXEL_1;
    int top_a = object1.y - TRASH_PIXEL_2;
    int bottom_a = top_a + object1.h - TRASH_PIXEL_2;

    int left_b = object2.x - TRASH_PIXEL_1;
    int right_b = left_b + object2.w - TRASH_PIXEL_1;
    int top_b = object2.y - TRASH_PIXEL_2;
    int bottom_b = top_b + object2.h - TRASH_PIXEL_2;

    // Kiểm tra va chạm
    bool horizontalOverlap = left_a < right_b && right_a > left_b;
    bool verticalOverlap = top_a < bottom_b && bottom_a > top_b;

    return horizontalOverlap && verticalOverlap;
}

bool CheckMousePos(const int& x,const int& y,const SDL_Rect& Object)
{
	return (x >= Object.x && x <= Object.x + Object.w && y>= Object.y && y <= Object.y + Object.h);
}

int UpdateGameTimeAndScore(int &time, int &speed, int &score) 
{
	if (time == TIME_MAX)
	{
		speed += SPEED_INCREASEMENT;
        time = 0;
	}
    if (speed > 30) 
    {
        speed = 0;
    }
	if (time % 5 == 0)
	{
		score += SCORE_INCREASEMENT;
	}
	
	time += TIME_INCREASEMENT;

	return time;
}
