#pragma once

#include "SDL/include/SDL.h"
#include "SDL_image/include/SDL_image.h"
#include "SDL_mixer/include/SDL_mixer.h"


#include "Entity.h"
#include <iostream>

#define WINDOW_WIDTH	1024
#define WINDOW_HEIGHT	768
#define MAX_KEYS		256
#define MAX_SHOTS		32

class Game
{
public:
	Game();
	~Game();

	bool Init();
	void Release();

	bool Input();
	bool Update();
	void Draw();
	bool CheckCollision(SDL_Rect a, SDL_Rect b);

private:
	SDL_Window* Window;
	SDL_Renderer* Renderer;

	SDL_Texture* Spaceship;
	SDL_Texture* Shot;
	SDL_Texture* Background;
	SDL_Texture* EnemyShip;

	Mix_Music* Music;
	Mix_Chunk* Fx_shoot;
	

	Entity Player, Scene, Shots[MAX_SHOTS], Enemy;

	bool godMode;

	int idx_shot;


	enum KEY_STATE { KEY_IDLE, KEY_DOWN, KEY_REPEAT, KEY_UP };
	KEY_STATE keys[MAX_KEYS];
};


