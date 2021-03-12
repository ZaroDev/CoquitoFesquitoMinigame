#pragma once
class Entity
{
public:
	Entity();
	~Entity();
	void Init(int posx, int posy, int w, int h, int s);
	void GetRect(int* posx, int* posy, int* w, int* h);
	int  GetX();
	void SetX(int posx);
	int  GetWidth();
	void ShutDown();
	bool IsAlive();
	void Move(int dx, int dy);

private:
	int x, y;
	int width, height;
	int speed;
	bool is_alive;
};

