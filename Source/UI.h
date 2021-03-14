#pragma once
#include <SDL.h>
class UI
{
public:
	UI();
	void UpdateScore(int pointsToAdd);
	void DrawScore();

private:
	int score;
};

