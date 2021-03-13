#pragma once
#include "SDL/include/SDL.h"
#define MAX_SHOTS		32
class Entity
{
public:
	Entity();
	~Entity();
	void Init(int posx, int posy, int w, int h, int s, int l, int d);
	void GetRect(int* posx, int* posy, int* w, int* h);
	int GetX();
	int GetY();
	void SetX(int posx);
	void SetY(int posy);
	int GetWidth();
	void ShutDown();
	bool IsAlive();
	void Move(int dx, int dy);
	SDL_Rect EntityRect();
	void DealDamage(Entity obj);


private:
	int x, y;
	int width, height;
	int speed;
	bool is_alive;
	int life;
	int damage;
	SDL_Rect entityCollider;
};

