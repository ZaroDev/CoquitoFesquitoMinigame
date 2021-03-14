#include "UI.h"

UI::UI() 
{
}

void UI::UpdateScore(int pointsToAdd)
{
	if (score[3] < 10)
	{
		score[3] += pointsToAdd;
	}
	if(score[3] >= 10)
	{
		score[3] = 0;
		score[2]++;
		if (score[2] < 10 && score[3] != 0)
		{
			score[2] += pointsToAdd;
		}
		if (score[2] >= 10)
		{
			score[2] = 0;
			score[1] = 1;
			if (score[1] < 10 && score[2] != 0)
			{
				score[1] += pointsToAdd;
			}
			if (score[1] >= 10)
			{
				score[1] = 0;
				score[0] = 1;

			}

		}
	}
	num4 = score[3];
	num3 = score[2];
	num2 = score[1];
	num1 = score[0];
}

void UI::DrawScore(SDL_Rect* srcRc, SDL_Rect* srcRc1, SDL_Rect* srcRc2, SDL_Rect* srcRc3)
{
	srcRc->x = 0;
	srcRc->y = 0;
	srcRc->w = 38;
	srcRc->h = 65;

	srcRc1->x = 0;
	srcRc1->y = 0;
	srcRc1->w = 38;
	srcRc1->h = 65;


	srcRc2->x = 0;
	srcRc2->y = 0;
	srcRc2->w = 38;
	srcRc2->h = 65;

	srcRc3->x = 0;
	srcRc3->y = 0;
	srcRc3->w = 38;
	srcRc3->h = 65;
	switch (num1)
	{
	case 0:
	{
		srcRc->x = 0;
	}break;
	case 1:
	{
		srcRc->x = srcRc->w;
	}break;
	case 2:
	{
		srcRc->x = 2 * srcRc->w;
	}break;
	case 3:
	{
		srcRc->x = 3 * srcRc->w;
	}break;
	case 4:
	{
		srcRc->x = 4 * srcRc->w;
	}break;
	case 5:
	{
		srcRc->x = 5 * srcRc->w;
	}break;
	case 6:
	{
		srcRc->x = 6 * srcRc->w;
	}break;
	case 7:
	{
		srcRc->x = 7 * srcRc->w;
	}break;
	case 8:
	{
		srcRc->x = 8 * srcRc->w;
	}break;
	case 9:
	{
		srcRc->x = 9 * srcRc->w;
	}break;
	}
	switch (num2)
	{
	case 0:
	{
		srcRc1->x = 0;
	}break;
	case 1:
	{
		srcRc1->x = srcRc1->w;
	}break;
	case 2:
	{
		srcRc1->x = 2 * srcRc1->w;
	}break;
	case 3:
	{
		srcRc1->x = 3 * srcRc1->w;
	}break;
	case 4:
	{
		srcRc1->x = 4 * srcRc1->w;
	}break;
	case 5:
	{
		srcRc1->x = 5 * srcRc1->w;
	}break;
	case 6:
	{
		srcRc1->x = 6 * srcRc1->w;
	}break;
	case 7:
	{
		srcRc1->x = 7 * srcRc1->w;
	}break;
	case 8:
	{
		srcRc1->x = 8 * srcRc1->w;
	}break;
	case 9:
	{
		srcRc1->x = 9 * srcRc1->w;
	}break;
	}
	switch (num3)
	{
	case 0:
	{
		srcRc2->x = 0;
	}break;
	case 1:
	{
		srcRc2->x = srcRc2->w;
	}break;
	case 2:
	{
		srcRc2->x = 2 * srcRc2->w;
	}break;
	case 3:
	{
		srcRc2->x = 3 * srcRc2->w;
	}break;
	case 4:
	{
		srcRc2->x = 4 * srcRc2->w;
	}break;
	case 5:
	{
		srcRc2->x = 5 * srcRc2->w;
	}break;
	case 6:
	{
		srcRc2->x = 6 * srcRc2->w;
	}break;
	case 7:
	{
		srcRc2->x = 7 * srcRc2->w;
	}break;
	case 8:
	{
		srcRc2->x = 8 * srcRc2->w;
	}break;
	case 9:
	{
		srcRc2->x = 9 * srcRc2->w;
	}break;
	}
	switch (num4)
	{
	case 0:
	{
		srcRc3->x = 0;
	}break;
	case 1:
	{
		srcRc3->x = srcRc3->w;
	}break;
	case 2:
	{
		srcRc3->x = 2 * srcRc3->w;
	}break;
	case 3:
	{
		srcRc3->x = 3 * srcRc3->w;
	}break;
	case 4:
	{
		srcRc3->x = 4 * srcRc3->w;
	}break;
	case 5:
	{
		srcRc3->x = 5 * srcRc3->w;
	}break;
	case 6:
	{
		srcRc3->x = 6 * srcRc3->w;
	}break;
	case 7:
	{
		srcRc3->x = 7 * srcRc3->w;
	}break;
	case 8:
	{
		srcRc3->x = 8 * srcRc3->w;
	}break;
	case 9:
	{
		srcRc3->x = 9 * srcRc3->w;
	}break;
	}
}

void UI::ResetScore()
{
	num1 = num2 = num3 = num4 = 0;
}

