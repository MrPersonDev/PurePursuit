#include <SDL2/SDL.h>

#include "map.hpp"
#include "robot.hpp"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 800;

SDL_Window *gWindow = NULL;
SDL_Surface *gScreenSurface = NULL;
SDL_Renderer*gRenderer = NULL;

Map gMap;
Robot gRobot;

void render();
bool init();
void close();

int main()
{
    if (!init())
    {
        close();
        return 1;
    }

    SDL_Event e;
    bool quit = false;
    double frameDelta = 0.0;
    while (!quit)
    {
        long start = SDL_GetPerformanceCounter();

        while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
				quit = true;
		}

        gRobot.updatePosition(frameDelta);
        render();

        long end = SDL_GetPerformanceCounter();
		frameDelta = (end - start) / (double)(SDL_GetPerformanceFrequency() * 1000.0);
    }

    close();
}

void render()
{
    SDL_RenderClear(gRenderer);
    gMap.render(gRenderer);
    gRobot.render(gRenderer);
    SDL_RenderPresent(gRenderer);
}

bool init()
{
    gWindow = SDL_CreateWindow("Pure Pursuit", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	gRenderer = SDL_CreateRenderer(gWindow, -1, 0);
    
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        return false;
    
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags))
        return false;

    gMap.init(gRenderer);
    gRobot.init(gRenderer);
    
    return true;
}

void close()
{
    SDL_DestroyWindow(gWindow);
	SDL_DestroyRenderer(gRenderer);
	gWindow = NULL;
	gRenderer = NULL;

    gMap.free();
    gRobot.free();
    
    SDL_Quit();
}
