#pragma once
#include "SDL/include/SDL.h"
#include <iostream>

class Entity
{
public:
	Entity(std::string _name);
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
	void Move(float dx, float dy);
	SDL_Rect EntityRect();


private:
	float x, y;
	int width, height;
	int speed;
	bool is_alive;
	
	SDL_Rect entityCollider;

};

