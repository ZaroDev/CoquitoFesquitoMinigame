#include "Entity.h"


Entity::Entity(std::string _name)
{
	is_alive = false;
}
Entity::Entity() {}
Entity::~Entity() {}
void Entity::Init(int posx, int posy, int w, int h, int s, int l, int d)
{
	x = posx;
	y = posy;
	width = w;
	height = h;
	speed = s;
	is_alive = true;
	entityCollider.w = width;
	entityCollider.h = height;
	entityCollider.x = x;
	entityCollider.y = y;
}
void Entity::GetRect(int* posx, int* posy, int* w, int* h)
{
	*posx = x;
	*posy = y;
	*w = width;
	*h = height;
}
int Entity::GetX()
{
	return x;
}

int Entity::GetY()
{
	return y;
}

void Entity::SetX(int posx)
{
	x = posx;
}
void Entity::SetY(int posy)
{
	y = posy;
}
int Entity::GetWidth()
{
	return width;
}
void Entity::ShutDown()
{
	is_alive = false;
	entityCollider.w = 0;
	entityCollider.h = 0;
}
bool Entity::IsAlive()
{
	return is_alive;
}

void Entity::Move(float dx, float dy)
{
	x += dx * speed;
	y += (dy * speed) + 1;
}

SDL_Rect Entity::EntityRect()
{
	return entityCollider;
}


