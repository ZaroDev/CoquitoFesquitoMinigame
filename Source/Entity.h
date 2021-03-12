#pragma once
#include "SDL/include/SDL.h"
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

