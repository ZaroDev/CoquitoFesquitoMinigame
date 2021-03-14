#pragma once
#include <SDL.h>

class UI
{
public:
	UI();
	void UpdateScore(int pointsToAdd);
	void DrawScore(SDL_Rect* srcRc, SDL_Rect* srcRc1, SDL_Rect* srcRc2, SDL_Rect* srcRc3);
	void ResetScore();

private:
	int num1;
	int num2;
	int num3;
	int num4;
	int score[4] = { 0 };
};

