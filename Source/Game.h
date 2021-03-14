#pragma once

#include "SDL/include/SDL.h"
#include "SDL_image/include/SDL_image.h"
#include "SDL_mixer/include/SDL_mixer.h"


#include "Entity.h"
#include "UI.h"
#include <iostream>

#define WINDOW_WIDTH	1024
#define WINDOW_HEIGHT	768
#define MAX_KEYS		256
#define MAX_SHOTS		32
#define MAX_ENEMIES		5

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
	void RandSpawn();
	void SpawnPlayer();

private:
	SDL_Window* Window;
	SDL_Renderer* Renderer;

	SDL_Texture* PlayerIMG;
	SDL_Texture* Background;
	SDL_Texture* ShotIMG;
	SDL_Texture* EnemyIMG;
	SDL_Texture* Text;
	SDL_Texture* Font;


	Mix_Music* Music;
	Mix_Chunk* Fx_shoot;
	Mix_Chunk* Fx_kill;
	Mix_Chunk* Fx_gameOver;


	SDL_Rect scrRc;
	SDL_Rect scrRc1;
	SDL_Rect scrRc2;
	SDL_Rect scrRc3;

	SDL_Rect dsrRc;
	SDL_Rect dsrRc1;
	SDL_Rect dsrRc2;
	SDL_Rect dsrRc3;

	UI UI;
	

	Entity Player, Scene, Platform, Shots[MAX_SHOTS], Enemy[MAX_ENEMIES];

	bool godMode;
	int idx_shot;
	int idx_enemy;
	enum KEY_STATE { KEY_IDLE, KEY_DOWN, KEY_REPEAT, KEY_UP };
	KEY_STATE keys[MAX_KEYS];
};