#include <iostream>
#include <sstream>
#include <SDL_mixer.h>
#include "Game.h"
#include "Texture.h"
#include "Map.h"
#include "Components.h"
#include "Vector2D.h"
#include "Collision.h"
#include "AssetManager.h"


Map* map;
Manager manager;
int Game::Timer = 0;
int Timers[10] = { 10,30,45,30,40,45,50,35,50,40 };
//int Timers[10] = { 1000,1000 ,1000 ,1000 ,1000 ,1000 ,1000 ,1000 ,1000 ,1000 }; - Testing
SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;

AssetManager* Game::assets = new AssetManager(&manager);
Vector2D Positions[20] = { {384,170},{1152,170},{128,120},{1408,120},{640,120},{896,120},{384,120},{1152,640},{128,704},{1408,120},{128,120},{640,120},{1408,696},{1408,120},{1408, 120},{128, 120},{640,370},{900,430},{128, 120},{1408, 696} };

auto& player1(manager.addEntity());
auto& player2(manager.addEntity());

auto& Start(manager.addEntity());

auto& Tutorial(manager.addEntity());
auto& WinTut1(manager.addEntity());
auto& WinTut2(manager.addEntity());

auto& Exit2(manager.addEntity());
auto& Music1(manager.addEntity());
auto& Music2(manager.addEntity());

auto& Exit(manager.addEntity());

auto& Background(manager.addEntity());
auto& Win(manager.addEntity());

auto& Watch(manager.addEntity());

bool isMenu = true; 
int level = 1;
bool Completed = false;


Game::Game() :isRunning(false), window(nullptr)
{}

Game::~Game()
{}

void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen)
{
	int flags = 0;
	SDL_Surface* aux = nullptr;
	if (fullscreen)
	{
		flags = SDL_WINDOW_FULLSCREEN;
	}
		if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
		{
			std::cout << "Subsystems initialized" << std::endl;
			auto window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
			
			try
			{
				if (window == 0)
					throw "Error creating window.";

				std::cout << "Window created.\n";
			}
			catch (const std::exception& e)
			{
				std::cout << e.what();
			}

			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

			try
			{
				if (renderer == nullptr)
					throw "Error creating renderer.";

				SDL_SetRenderDrawColor(renderer, 0xbb, 0x80, 0x44, 0x00);
				std::cout << "Renderer created\n" << std::endl;
			}
			catch (const std::exception& e)
			{
				std::cout << e.what();
			}
			
			int flags = MIX_INIT_OGG | MIX_INIT_MOD;
			Mix_Init(flags);

			Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096);

			Mix_Chunk* sample;
			sample = Mix_LoadWAV("Images/Music.wav");

			try
			{
				if (sample == 0)
					throw "Error getting music.";
			}
			catch (const std::exception& e)
			{
				std::cout << e.what();
			}

			Mix_PlayChannel(-1, sample, -1);
			Mix_Volume(-1, MIX_MAX_VOLUME / 10);

			TTF_Init();
			
			isRunning = true;
			
	}
	else
	{
		isRunning = false;
	}

		assets->AddTexture("Background", "Images/SampleE.png");
		assets->AddTexture("Win", "Images/win.png");
		assets->AddTexture("textures", "Images/Textures.png");
		assets->AddTexture("player1", "Images/P1Anim.png");
		assets->AddTexture("player2", "Images/P2Anim.png");
		
		assets->AddTexture("Start", "Images/Start.png");

		assets->AddTexture("Tutorial", "Images/tutorial.png");
		assets->AddTexture("WinTut1", "Images/WinTut1.png");
		assets->AddTexture("WinTut2", "Images/WinTut2.png");

		assets->AddTexture("Exit2", "Images/Exit2.png");
		assets->AddTexture("Music1", "Images/Music1.png");
		assets->AddTexture("Music2", "Images/Music2.png");

		assets->AddTexture("exit", "Images/exit.png");

		SDL_Color white = { 255,255,255,255 };
		Watch.addComponent<TimerComponent>(12, 8, "", 45, white);
		Watch.addGroup(groupTimer);

		Background.addComponent<TransformComponent>(0, 0, 896, 1600, 1);
		Background.addComponent<SpriteComponent>("Background", false);

		Start.addComponent<TransformComponent>(651, 294, 132, 297, 1);
		Start.addComponent<SpriteComponent>("Start", false);
		Start.addComponent<ButtonComponent>("Start");

		Tutorial.addComponent<TransformComponent>(649, 421, 126, 291, 1);
		Tutorial.addComponent<SpriteComponent>("Tutorial", false);
		Tutorial.addComponent<ButtonComponent>("Tutorial");		

		Exit2.addComponent<TransformComponent>(1547, 8, 48, 48, 1);
		Exit2.addComponent<SpriteComponent>("Exit2", false);
		Exit2.addComponent<ButtonComponent>("Exit2");

		Music1.addComponent<TransformComponent>(1480, 8, 48, 48, 1);
		Music1.addComponent<SpriteComponent>("Music1", false);
		Music1.addComponent<ButtonComponent>("Music1");

		Music2.addComponent<TransformComponent>(1547, 72, 48, 48, 1);
		Music2.addComponent<SpriteComponent>("Music2", false);
		Music2.addComponent<ButtonComponent>("Music2");

		Exit.addComponent<TransformComponent>(693, 551, 117, 213, 1);
		Exit.addComponent<SpriteComponent>("exit", false);
		Exit.addComponent<ButtonComponent>("exit");

		
		player1.addGroup(groupPlayers);
		player2.addGroup(groupPlayers);
}


auto& tiles(manager.getGroup(Game::groupMap));
auto& players(manager.getGroup(Game::groupPlayers));
auto& colliders(manager.getGroup(Game::groupColliders));
auto& Portal(manager.getGroup(Game::groupPortal));
auto& Watchu(manager.getGroup(Game::groupTimer));


void Game::handleEvents()
{
	SDL_PollEvent(&event);

	switch (event.type)
	{
	case SDL_QUIT: {
		isRunning = false;
		break;
	}
	case SDL_KEYDOWN:
	{
		if (event.key.keysym.sym == SDLK_ESCAPE)
			isRunning = false;
		break;
	}
	default:
		break;
	}
	
}

int win1 = 0, win2 = 0;
int justWon = 0;
bool ok = false;
bool isMusic = false;
bool isShowed = true;

void Game::update()
{
	std::stringstream WT;

	int TimerS = Timers[level - 1] - Timer/60;
	int TimerM = TimerS / 60;

	TimerS = TimerS - TimerM * 60;

	WT << std::to_string(TimerM) + ":" + std::to_string(TimerS);
	Watch.getComponent<TimerComponent>().SetLabelText(WT.str());
	WT.clear();

	if(isMenu == false)
		Game::Timer++;

	std::cout << Timer << "\n";
	int xMouse, yMouse;

	SDL_Rect playerCol1;
	Vector2D playerPos1;

	SDL_Rect playerCol2;
	Vector2D playerPos2;

	if (Exit2.getComponent<ButtonComponent>().alreadyPressed)
	{
		Game::isRunning = 0;
		isMenu = false;
	}

	if (Music1.getComponent<ButtonComponent>().alreadyPressed)
	{
		Mix_Pause(-1);
		isMusic = true;
	}

	if (Music2.getComponent<ButtonComponent>().alreadyPressed)
	{
		Mix_Resume(-1);
		isMusic = false;
	}

	if(isMenu == true)
	{

		if (Exit.getComponent<ButtonComponent>().alreadyPressed)
		{
			Game::isRunning = 0;
			isMenu = false;
		}

		if (Start.getComponent<ButtonComponent>().alreadyPressed)
		{
			ok = true;
			isShowed = false;
			Background.destroy();
			Start.destroy();
			Tutorial.destroy();
			Exit.destroy();
			WinTut1.destroy();
			WinTut2.destroy();

			map = new Map("textures", 1, 64);
			map->LoadMap("Images/Levels/Lvl1/Lvl1.map", 25, 14);
			Game::Timer = 0;

			player1.addComponent<TransformComponent>(384, 170, 128, 128, 2);
			player1.addComponent<SpriteComponent>("player1", true);
			player1.addComponent<KeyboardController>();
			player1.addComponent<ColliderComponent>("Player1");

			player2.addComponent<TransformComponent>(1152, 170, 128, 128, 2);
			player2.addComponent<SpriteComponent>("player2", true);
			player2.addComponent<KeyboardController>();
			player2.addComponent<ColliderComponent>("Player2");

			isMenu = false;
		}

	}

	if (isShowed == true)
	{
		if (Tutorial.getComponent<ButtonComponent>().alreadyPressed)
		{

			WinTut1.addComponent<TransformComponent>(200, 300, 500, 400, 1);
			WinTut1.addComponent<SpriteComponent>("WinTut1", false);

			WinTut2.addComponent<TransformComponent>(1005, 300, 500, 400, 1);
			WinTut2.addComponent<SpriteComponent>("WinTut2", false);

			isShowed = false;
		}
	}

	if (ok == true)
	{
		playerCol1 = player1.getComponent<ColliderComponent>().collider;
		playerPos1 = player1.getComponent<TransformComponent>().position;

		playerCol2 = player2.getComponent<ColliderComponent>().collider;
		playerPos2 = player2.getComponent<TransformComponent>().position;
	}

	manager.refresh();
	manager.update();

	if (ok == true)
	{
		
		for (auto cc : colliders)
		{
			SDL_Rect cCol = cc->getComponent<ColliderComponent>().collider;
			if (SDL_HasIntersection(&cCol, &playerCol1))
			{
				player1.getComponent<TransformComponent>().position = playerPos1;
			}
		}
		
		for (auto cc : colliders)
		{
			SDL_Rect cCol = cc->getComponent<ColliderComponent>().collider;
			if (SDL_HasIntersection(&cCol, &playerCol2))
			{
				player2.getComponent<TransformComponent>().position = playerPos2;
			}
		}


		for (auto t : Portal)
		{
			ColliderComponent tCol = t->getComponent<ColliderComponent>();
			if (SDL_HasIntersection(&tCol.collider, &playerCol1) && tCol.tag == "portalAlbastru")
			{
				win1 = 1;
			}

			if (SDL_HasIntersection(&tCol.collider, &playerCol2) && tCol.tag == "portalVerde")
			{
				win2 = 1;
			}
		}

		if (win1 && win2 && justWon == 0)
		{
			std::cout << level << '\n';
			isWinning = true;
			justWon = 1;
			level++;
			player1.getComponent<TransformComponent>().setPos(-100, -100);
			player2.getComponent<TransformComponent>().setPos(-100, -100);
		}
		else isWinning = false;

		if (win1 == 0 || win2 == 0)
			justWon = 0;

		/*
		std::cout << player1.getComponent<TransformComponent>().position.x << "," << player1.getComponent<TransformComponent>().position.y << "\n";
		std::cout << std::endl;
		std::cout << player2.getComponent<TransformComponent>().position.x << "," << player2.getComponent<TransformComponent>().position.y << "\n";*/

		if(isWinning == true)
		{
			
			if (level == 2)
			{
				Game::Remove();
				Game::Timer = 0;
				map->LoadNewMap("Images/Levels/Lvl2/Lvl2.map", 25, 14); 
				player1.getComponent<TransformComponent>().setPos(128, 120);
				player2.getComponent<TransformComponent>().setPos(1408, 120);
			}
			else if (level == 3)
			{
				Game::Remove();
				Game::Timer = 0;
				map->LoadNewMap("Images/Levels/Lvl3/Lvl3.map", 25, 14);
				player1.getComponent<TransformComponent>().setPos(640, 120);
				player2.getComponent<TransformComponent>().setPos(896, 120);
			}
			else if (level == 4)
			{
				Game::Remove();
				Game::Timer = 0;
				map->LoadNewMap("Images/Levels/Lvl4/Lvl4.map", 25, 14);
				player1.getComponent<TransformComponent>().setPos(384, 120);
				player2.getComponent<TransformComponent>().setPos(1152, 640);
			}
			else if (level == 5)
			{
				Game::Remove();
				Game::Timer = 0;
				map->LoadNewMap("Images/Levels/Lvl5/Lvl5.map", 25, 14);
				player1.getComponent<TransformComponent>().setPos(128, 704);
				player2.getComponent<TransformComponent>().setPos(1408, 120);
			}
			else if (level == 6)
			{
				Game::Remove();
				Game::Timer = 0;
				map->LoadNewMap("Images/Levels/Lvl6/Lvl6.map", 25, 14);
				player1.getComponent<TransformComponent>().setPos(128, 120);
				player2.getComponent<TransformComponent>().setPos(640, 120);
			}
			else if (level == 7)
			{
				Game::Remove();
				Game::Timer = 0;
				map->LoadNewMap("Images/Levels/Lvl7/Lvl7.map", 25, 14);
				player1.getComponent<TransformComponent>().setPos(1408, 696); 
				player2.getComponent<TransformComponent>().setPos(1408, 120);
			}
			else if (level == 8)
			{
				Game::Remove();
				Game::Timer = 0;
				map->LoadNewMap("Images/Levels/Lvl8/Lvl8.map", 25, 14);
				player1.getComponent<TransformComponent>().setPos(1408, 120);
				player2.getComponent<TransformComponent>().setPos(128, 120);
			}
			
			else if (level == 9)
			{
				Game::Remove();
				Game::Timer = 0;
				map->LoadNewMap("Images/Levels/Lvl9/Lvl9.map", 25, 14);
				player1.getComponent<TransformComponent>().setPos(640,370);
				player2.getComponent<TransformComponent>().setPos(900,430);
			}
			else if (level ==10)
			{
				Game::Remove();
				Game::Timer = 0;
				map->LoadNewMap("Images/Levels/Lvl10/Lvl10.map", 25, 14);
				player1.getComponent<TransformComponent>().setPos(128, 120);
				player2.getComponent<TransformComponent>().setPos(1408, 696);
			}

		}

		win1 = win2 = 0;
	}
	
	if (Timer > Timers[level - 1]* 60)
	{
		player1.getComponent<TransformComponent>().setPos(Positions[(level-1)*2].x, (Positions[(level - 1) * 2].y));
		player2.getComponent<TransformComponent>().setPos(Positions[(level - 1) * 2 + 1].x, (Positions[(level - 1) * 2 + 1].y));
		Timer = 0;
	}

		if (level == 11)
		{
			level = -1;
			Timer = 0;
			Completed = true;

			Game::Remove();
			Win.addComponent<TransformComponent>(0, 0, 896, 1600, 1);
			Win.addComponent<SpriteComponent>("Win", false);
		}

	
}


void Game::render()
{
	SDL_RenderClear(renderer);
	// this is where we would add stuff to render

	if (isMenu == true)
	{
		Background.draw();
		Start.draw();
		Tutorial.draw();
		Exit.draw();
	}

	if (isShowed == false)
	{
		WinTut1.draw();
		WinTut2.draw();
	}

	for (auto& t : tiles)
	{
		t->draw();
	}
	
	for (auto& c : colliders)
	{
		c->draw();
	}
	
	for (auto& p : players)
	{
		p->draw();
	}

	for (auto& t : Portal)
		t->draw();

	if(isMenu == false)
		for (auto& w : Watchu)
			w->draw();


	if (Completed == true)
		Win.draw();

	if(isMusic == false)
		Music1.draw();
	else
		Music2.draw();

	Exit2.draw();


	SDL_RenderPresent(renderer);
}

void Game::clean()
{
	Mix_CloseAudio();
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	Mix_Quit();
	TTF_Quit();
	SDL_QUIT;

	std::cout << "Game cleaned" << std::endl;
}

void Game::Remove(void)
{
	std::cout << "map cleaned!\n";	

	for (auto cc : colliders)
		cc->destroy();

	for (auto t : Portal)
		t->destroy();
}
