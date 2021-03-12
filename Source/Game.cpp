#include "Game.h"
#include <math.h>

Game::Game() {}
Game::~Game() {}

bool Game::Init()
{
	//Initialize SDL with all subsystems
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}
	//Create our window: title, x, y, w, h, flags
	Window = SDL_CreateWindow("Spaceship: arrow keys + space", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	if (Window == NULL)
	{
		SDL_Log("Unable to create window: %s", SDL_GetError());
		return false;
	}
	//Create a 2D rendering context for a window: window, device index, flags
	Renderer = SDL_CreateRenderer(Window, -1, 0);
	if (Renderer == NULL)
	{
		SDL_Log("Unable to create rendering context: %s", SDL_GetError());
		return false;
	}
	//Initialize keys array
	for (int i = 0; i < MAX_KEYS; ++i)
		keys[i] = KEY_IDLE;
	//Initialize sprites
	IMG_Init(IMG_INIT_PNG);
	if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
	{
		SDL_Log("Unable to initialize images: %s", SDL_GetError());
		return false;
	}

	Spaceship = SDL_CreateTextureFromSurface(Renderer, IMG_Load("Assets/spaceship.png"));
	if (Spaceship == NULL)
	{
		SDL_Log("Unable to load Spaceship: %s", SDL_GetError());
		return false;
	}


	Shot = SDL_CreateTextureFromSurface(Renderer, IMG_Load("Assets/shot.png"));
	if (Shot == NULL)
	{
		SDL_Log("Unable to load Shots: %s", SDL_GetError());
		return false;
	}

	Background = SDL_CreateTextureFromSurface(Renderer, IMG_Load("Assets/background.png"));
	if (Background == NULL)
	{
		SDL_Log("Unable to load Background: %s", SDL_GetError());
		return false;
	}
	EnemyShip = SDL_CreateTextureFromSurface(Renderer, IMG_Load("Assets/ship.png"));
	if (EnemyShip == NULL)
	{
		SDL_Log("Unable to load Enemyship: %s", SDL_GetError());
		return false;
	}
	//Initialize Music
	Mix_Init(MIX_INIT_OGG);
	Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024);

	Music = Mix_LoadMUS("Assets/music.ogg");
	if (Music == NULL)
	{
		SDL_Log("Unable to load Music: %s", SDL_GetError());
		return false;
	}
	Fx_shoot = Mix_LoadWAV("Assets/laser.wav");
	if (Fx_shoot == NULL)
	{
		SDL_Log("Unable to load FX_SHOT: %s", SDL_GetError());
		return false;
	}


	
	//Init variables
	//AUDIO
	Mix_PlayMusic(Music, -1);
	//VIDEO
	Player.Init(20, WINDOW_HEIGHT >> 1, 104, 82, 5, 3, 1);
	Enemy.Init(650, WINDOW_HEIGHT >> 1, 82, 82, 5, 3, 1);
	//GAME VARIABLES
	int w;
	SDL_QueryTexture(Background, NULL, NULL, &w, NULL);
	Scene.Init(0, 0, w, WINDOW_HEIGHT, 4, NULL, NULL);
	idx_shot = 0;
	godMode = false;


	return true;
}
void Game::Release()
{
	//Clean up all SDL initialized subsystems
	SDL_DestroyTexture(Spaceship);
	SDL_DestroyTexture(Shot);
	SDL_DestroyTexture(Background);
	SDL_DestroyTexture(EnemyShip);
	Mix_FreeMusic(Music);
	Mix_FreeChunk(Fx_shoot);
	Mix_CloseAudio();
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}
bool Game::Input()
{
	SDL_Event event;
	if (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)	return false;
	}

	SDL_PumpEvents();
	const Uint8* keyboard = SDL_GetKeyboardState(NULL);
	for (int i = 0; i < MAX_KEYS; ++i)
	{
		if (keyboard[i])
			keys[i] = (keys[i] == KEY_IDLE) ? KEY_DOWN : KEY_REPEAT;
		else
			keys[i] = (keys[i] == KEY_REPEAT || keys[i] == KEY_DOWN) ? KEY_UP : KEY_IDLE;
	}

	return true;
}
bool Game::Update()
{
	//Read Input
	if (!Input())	return true;

	//Process Input
	int fx = 0, fy = 0;
	if (keys[SDL_SCANCODE_ESCAPE] == KEY_DOWN)	return true;
	if (keys[SDL_SCANCODE_F1] == KEY_DOWN) godMode = !godMode;
	if (keys[SDL_SCANCODE_UP] == KEY_REPEAT)	fy = -1;
	if (keys[SDL_SCANCODE_DOWN] == KEY_REPEAT)	fy = 1;
	if (keys[SDL_SCANCODE_LEFT] == KEY_REPEAT)	fx = -1;
	if (keys[SDL_SCANCODE_RIGHT] == KEY_REPEAT)	fx = 1;
	if (keys[SDL_SCANCODE_SPACE] == KEY_DOWN)
	{
		int x, y, w, h;
		Player.GetRect(&x, &y, &w, &h);
		Shots[idx_shot].Init(x + w - 75, y + h - 79, 56, 20, 10, 1, 1);
		idx_shot++;
		idx_shot %= MAX_SHOTS;
		Shots[idx_shot].Init(x + w - 75, y + h - 23, 56, 20, 10, 1, 1);
		idx_shot++;
		idx_shot %= MAX_SHOTS;
		Mix_PlayChannel(-1, Fx_shoot, 0);

	}


	//Logic
	//Player update
	Scene.Move(-1, 0);
	if (Scene.GetX() <= -Scene.GetWidth()) Scene.SetX(0);
	Player.Move(fx, fy);
	Enemy.Move(0, 0);
	//Shots update
	for (int i = 0; i < MAX_SHOTS; ++i)
	{
		if (Shots[i].IsAlive())
		{
			Shots[i].Move(1, 0);
			if (Shots[i].GetX() > WINDOW_WIDTH)	Shots[i].ShutDown();
			if ((Shots[i].GetX() == Enemy.GetX()) && (Shots[i].GetY() == Enemy.GetY()))	Shots[i].ShutDown();
			if (CheckCollision(Enemy.EntityRect(), Shots[i].EntityRect()))
			{
				Enemy.DealDamage(Shots[i]);
			}
		}

	}

	return false;
}
void Game::Draw()
{
	//Set the color used for drawing operations
	SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
	//Clear rendering target
	SDL_RenderClear(Renderer);
	if (godMode) SDL_SetRenderDrawColor(Renderer, 192, 0, 0, 255);
	//Draw player
	SDL_Rect srcRc;
	SDL_Rect dstRc;



	Scene.GetRect(&dstRc.x, &dstRc.y, &dstRc.w, &dstRc.h);
	SDL_RenderCopy(Renderer, Background, NULL, &dstRc);
	dstRc.x += dstRc.w;
	SDL_RenderCopy(Renderer, Background, NULL, &dstRc);

	Player.GetRect(&dstRc.x, &dstRc.y, &dstRc.w, &dstRc.h);
	SDL_RenderCopy(Renderer, Spaceship, NULL, &dstRc);
	if (godMode) SDL_RenderDrawRect(Renderer, &dstRc);
	if (Enemy.IsAlive())
	{
		Enemy.GetRect(&dstRc.x, &dstRc.y, &dstRc.w, &dstRc.h);
		SDL_RenderCopy(Renderer, EnemyShip, NULL, &dstRc);
		if (godMode) SDL_RenderDrawRect(Renderer, &dstRc);
	}

	//Draw shots
	SDL_SetRenderDrawColor(Renderer, 192, 0, 0, 255);
	for (int i = 0; i < MAX_SHOTS; ++i)
	{
		if (Shots[i].IsAlive())
		{
			Shots[i].GetRect(&dstRc.x, &dstRc.y, &dstRc.w, &dstRc.h);
			SDL_RenderCopy(Renderer, Shot, NULL, &dstRc);
			if (godMode) SDL_RenderDrawRect(Renderer, &dstRc);
		}
	}

	//Update screen
	SDL_RenderPresent(Renderer);

	SDL_Delay(10);	// 1000/10 = 100 fps max
}

bool Game::CheckCollision(SDL_Rect a, SDL_Rect b)
{
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

	leftA = a.x;
	rightA = a.x + a.w;
	topA = a.y;
	bottomA = a.y + a.h;

	leftB = b.x;
	rightB = b.x + b.w;
	topB = b.y;
	bottomB = b.y + b.h;

	if (bottomA <= topB) {
		return false;
	}
	if (topA >= bottomB) {
		return false;
	}
	if (rightA <= leftB) {
		return false;
	}
	if (leftA >= rightB) {
		return false;
	}
	return true;
}
