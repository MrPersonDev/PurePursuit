#include <SDL2/SDL.h>

#include "map.hpp"
#include "robot.hpp"
#include "path.hpp"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 800;
const int FIELD_SIZE = 144;

double mapScale = 1.0;

SDL_Window *gWindow = NULL;
SDL_Surface *gScreenSurface = NULL;
SDL_Renderer*gRenderer = NULL;

Map gMap;
Robot gRobot;
Path gPath;

void render();
bool init();
void setScale();
void setPath();
void close();

int main()
{
    if (!init())
    {
        close();
        return 1;
    }
    
    setScale();
    setPath();

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

        gRobot.updateWheelPower(frameDelta);
        gRobot.updatePosition(frameDelta);

        std::pair<double, double> goalPoint = gPath.getPoint(gRobot.getX(), gRobot.getY(), gRobot.getLookAheadDist());
        gPath.setGoalPoint(goalPoint.first, goalPoint.second);
        gRobot.setGoalPoint(goalPoint.first, goalPoint.second);
        
        render();

        long end = SDL_GetPerformanceCounter();
		frameDelta = (end - start) * 1000.0 / (double)SDL_GetPerformanceFrequency();
    }

    close();
}

void render()
{
    SDL_RenderClear(gRenderer);
    gMap.render(gRenderer);
    gRobot.render(gRenderer, mapScale);
    gPath.render(gRenderer, mapScale);
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

void setScale()
{
    mapScale = SCREEN_WIDTH / (double)FIELD_SIZE;
}

void setPath()
{
    gPath.addPoint(36, 12);
    gPath.addPoint(36, 132);
    gPath.addPoint(108, 132);
    gPath.addPoint(108, 12);
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
