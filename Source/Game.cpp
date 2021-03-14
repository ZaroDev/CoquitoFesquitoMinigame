#include "Game.h"
#include <math.h>


Game::Game() {}
Game::~Game() {}

bool Game::Init()
{
	//Initialize SDL with all subsystems
	SDL_Init(SDL_INIT_EVERYTHING);
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}
	//Create our window: title, x, y, w, h, flags
	Window = SDL_CreateWindow("Ghost Shot", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
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

	PlayerIMG = SDL_CreateTextureFromSurface(Renderer, IMG_Load("Assets/ghost.png"));
	if (PlayerIMG == NULL)
	{
		SDL_Log("Unable to load PlayerIMG: %s", SDL_GetError());
		return false;
	}
	EnemyIMG = SDL_CreateTextureFromSurface(Renderer, IMG_Load("Assets/ghost.png"));
	if (EnemyIMG == NULL)
	{
		SDL_Log("Unable to load PlayerIMG: %s", SDL_GetError());
		return false;
	}
	Background = SDL_CreateTextureFromSurface(Renderer, IMG_Load("Assets/background.png"));
	if (Background == NULL)
	{
		SDL_Log("Unable to load Background: %s", SDL_GetError());
		return false;
	}
	ShotIMG = SDL_CreateTextureFromSurface(Renderer, IMG_Load("Assets/steps.png"));
	if (ShotIMG == NULL)
	{
		SDL_Log("Unable to load Shot: %s", SDL_GetError());
		return false;
	}
	Text = SDL_CreateTextureFromSurface(Renderer, IMG_Load("Assets/score.png"));
	if (Text == NULL)
	{
		SDL_Log("Unable to load score: %s", SDL_GetError());
		return false;
	}
	Font = SDL_CreateTextureFromSurface(Renderer, IMG_Load("Assets/numbers.png"));
	if	(Font == NULL)
	{
		SDL_Log("Unable to load font: %s", SDL_GetError());
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
	Fx_shoot = Mix_LoadWAV("Assets/fx_jump.wav");
	if (Fx_shoot == NULL)
	{
		SDL_Log("Unable to load FX_SHOT: %s", SDL_GetError());
		return false;
	}
	Fx_kill = Mix_LoadWAV("Assets/enemy-dead.wav");
	if (Fx_shoot == NULL)
	{
		SDL_Log("Unable to load FX_kill: %s", SDL_GetError());
		return false;
	}
	Fx_gameOver = Mix_LoadWAV("Assets/game-over.wav");
	if (Fx_shoot == NULL)
	{
		SDL_Log("Unable to load fx_gameover: %s", SDL_GetError());
		return false;
	}

	
	//Init variables
	//AUDIO
	Mix_PlayMusic(Music, -1);
	//VIDEO
	Player.Init(950, WINDOW_HEIGHT >> 1, 82, 82, 5, 3, 1);
	//GAME VARIABLES
	int w;
	SDL_QueryTexture(Background, NULL, NULL, &w, NULL);
	Scene.Init(0, 0, w, WINDOW_HEIGHT, 4, NULL, NULL);
	idx_shot = 0;
	idx_enemy = 0;
	godMode = false;


	return true;
}
void Game::Release()
{
	//Clean up all SDL initialized subsystems

	SDL_DestroyTexture(PlayerIMG);
	SDL_DestroyTexture(Background);

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
		//size: 56x20
		//offset from player: dx, dy = [(29, 3), (29, 59)]
		Shots[idx_shot].Init(x, y, 56, 20, 10, 1, 1);
		idx_shot++;
		idx_shot %= MAX_SHOTS;
		Mix_PlayChannel(-1, Fx_shoot, 0);
	
	}
	Player.Move(fx, fy);
	//Moving shots
	for (int i = 0; i < MAX_SHOTS; ++i)
	{
		if (Shots[i].IsAlive())
		{
			Shots[i].Move(-1, 0);
			if (Shots[i].GetX() < 0)
			{
				std::cout << "Shot shutdown" << std::endl;
				Shots[i].ShutDown();
			}
		}
	}
	//Moving Enemies
	for (int i = 0; i < MAX_ENEMIES; i++)
	{
		if (Enemy[i].IsAlive())
		{
			Enemy[i].Move(0.5f, 0);
			if (Enemy[i].GetX() > WINDOW_WIDTH)
			{
				Enemy[i].ShutDown();
				std::cout << "Enemy shutdown" << std::endl;
			}
		}
	}
	RandSpawn();
	if (!Player.IsAlive())
	{
		SpawnPlayer();
	}
	//Collisions Update
	for (int i = 0; i < MAX_SHOTS; i++)
	{
		for (int j = 0; j < MAX_SHOTS; j++)
		{
			if (CheckCollision(Enemy[i].EntityRect(), Shots[j].EntityRect()))
			{
				Shots[j].ShutDown();
				Enemy[i].ShutDown();
				std::cout << "Enemy Kill!" << std::endl;
				Mix_PlayChannel(-1, Fx_kill, 0);
			}
		}
	}
	for (int i = 0; i < MAX_ENEMIES; i++)
	{
		if (Enemy[i].IsAlive() && Player.IsAlive())
		{
			if (CheckCollision(Enemy[i].EntityRect(), Player.EntityRect()))
			{
				Player.ShutDown();
				Enemy[i].ShutDown();
				std::cout << "player collsion!" << std::endl;
				Mix_PlayChannel(-1, Fx_gameOver, 0);
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
	if (Player.IsAlive())
	{
		Player.GetRect(&dstRc.x, &dstRc.y, &dstRc.w, &dstRc.h);
		SDL_RenderCopy(Renderer, PlayerIMG, NULL, &dstRc);
		if (godMode) SDL_RenderDrawRect(Renderer, &dstRc);
	}
	
	Platform.GetRect(&dstRc.x, &dstRc.y, &dstRc.w, &dstRc.h);
	SDL_SetRenderDrawColor(Renderer, 150, 52, 7, 255);
	SDL_RenderDrawRect(Renderer, &dstRc);
	SDL_RenderFillRect(Renderer, &dstRc);
	for (int i = 0; i < MAX_SHOTS; ++i)
	{
		if (Shots[i].IsAlive())
		{
			Shots[i].GetRect(&dstRc.x, &dstRc.y, &dstRc.w, &dstRc.h);
			SDL_RenderCopy(Renderer, ShotIMG, NULL, &dstRc);
			if (godMode) SDL_RenderDrawRect(Renderer, &dstRc);
		}
	}
	for (int i = 0; i < MAX_ENEMIES; ++i)
	{
		if (Enemy[i].GetX() != NULL)
		{
			Enemy[i].GetRect(&dstRc.x, &dstRc.y, &dstRc.w, &dstRc.h);
			SDL_RenderCopy(Renderer, EnemyIMG, NULL, &dstRc);
			if (godMode)
			{
				SDL_RenderDrawRect(Renderer, &dstRc);
				SDL_SetRenderDrawColor(Renderer, 150, 52, 7, 255);
				SDL_RenderDrawRect(Renderer, &srcRc);
			}
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
void Game::RandSpawn()
{
	srand(time(NULL));
	int Randnum;
	int inity[MAX_ENEMIES];
	int initx[MAX_ENEMIES];
	Randnum = rand() % 10 + 1;
	//std::cout << Randnum << initx;
	if (Randnum > 5)
	{
		for (int i = 0; i < MAX_ENEMIES - 1; i++)
		{
			inity[i] = rand() % WINDOW_HEIGHT - 85	 /* -altura del enemigo */;
			if (Enemy[i].IsAlive() == false)
			{
				//crea enemigo
				Enemy[i].Init(0, inity[i], 85, 85, 3, 1, 1);
				std::cout << "Enemy spawn!" << std::endl;
				std::cout << "Enemy " << i << " w: " << Enemy[i].EntityRect().w << " h: "<<  Enemy[i].EntityRect().h << std::endl;
				break;
			}
		}
	}
}

void Game::SpawnPlayer()
{
	Player.Init(950, WINDOW_HEIGHT >> 1, 82, 82, 5, 3, 1);


}
