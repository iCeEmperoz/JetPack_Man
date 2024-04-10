#include "HeaderFile/CommonFunc.h"

bool SDLCommonFunc::CheckCollision(const SDL_Rect& object1, const SDL_Rect& object2) 
{
    // int left_a = object1.x;
    // int right_a = object1.x + object1.w;
    // int top_a = object1.y;
    // int bottom_a = object1.y + object1.h;
 
    // int left_b = object2.x;
    // int right_b = object2.x + object2.w;
    // int top_b = object2.y;
    // int bottom_b = object2.y + object2.h;
    
    // // Case 1: size object 1 < size object 2
    // if (left_a > left_b && left_a < right_b)
    // {
    //     if (top_a > top_b && top_a < bottom_b)
    //     {
    //     return true;
    //     }
    // }
    
    // if (left_a > left_b && left_a < right_b)
    // {
    //     if (bottom_a > top_b && bottom_a < bottom_b)
    //     {
    //     return true;
    //     }
    // }
    
    // if (right_a > left_b && right_a < right_b)
    // {
    //     if (top_a > top_b && top_a < bottom_b)
    //     {
    //     return true;
    //     }
    // }
    
    // if (right_a > left_b && right_a < right_b)
    // {
    //     if (bottom_a > top_b && bottom_a < bottom_b)
    //     {
    //     return true;
    //     }
    // }
    
    // // Case 2: size object 1 > size object 2
    // if (left_b > left_a && left_b < right_a)
    // {
    //     if (top_b > top_a && top_b < bottom_a)
    //     {
    //     return true;
    //     }
    // }
    
    // if (left_b > left_a && left_b < right_a)
    // {
    //     if (bottom_b > top_a && bottom_b < bottom_a)
    //     {
    //     return true;
    //     }
    // }
    
    // if (right_b > left_a && right_b < right_a)
    // {
    //     if (top_b > top_a && top_b < bottom_a)
    //     {
    //     return true;
    //     }
    // }
    
    // if (right_b > left_a && right_b < right_a)
    // {
    //     if (bottom_b > top_a && bottom_b < bottom_a)
    //     {
    //     return true;
    //     }
    // }
    
    // // Case 3: size object 1 = size object 2
    // if (top_a == top_b && right_a == right_b && bottom_a == bottom_b)
    // {
    //     return true;
    // }

    // return false;
    int TRASH_PIXEL_1 = 10; // Điều chỉnh trash pixel tùy theo yêu cầu của bạn
    int TRASH_PIXEL_2 = 5; // Điều chỉnh trash pixel tùy theo yêu cầu của bạn

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
bool SDLCommonFunc::CheckMousePos(const int& x,const int& y,const SDL_Rect& Obj_)
{
	return (x >= Obj_.x && x <= Obj_.x + Obj_.w && y>= Obj_.y && y <= Obj_.y + Obj_.h);
}
int SDLCommonFunc::UpdateGameTimeAndScore(int &time, int &speed, int &score) 
{
	if (time == TIME_MAX)
	{
		speed += SPEED_INCREASEMENT;
	}

	if (time > TIME_MAX)
	{
		time = 0;
	}
	if (time % 5 == 0)
	{
		score += SCORE_INCREASEMENT;
	}
	
	time += TIME_INCREASEMENT;

	return time;
}
