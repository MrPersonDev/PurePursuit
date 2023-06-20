#include <SDL2/SDL.h>

#include "map.hpp"
#include "robot.hpp"
#include "path.hpp"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 800;
const int FIELD_SIZE = 144;

double mapScale = 1.0;
bool running = false;

SDL_Window *gWindow = NULL;
SDL_Surface *gScreenSurface = NULL;
SDL_Renderer*gRenderer = NULL;

SDL_Texture *pathTexture = NULL;

Map gMap;
Robot gRobot;
Path gPath;

void render();
bool init();
void run();
void placePoint();
void clearPoints();
void smoothPoints();
void getPathTexture();
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
            else if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                if (e.button.button == 1)
                    placePoint();
                else if (e.button.button == 3)
                    clearPoints();
            }
            else if (e.type == SDL_KEYDOWN)
            {
                switch (e.key.keysym.sym)
                {
                    case SDLK_SPACE:
                        run();
                        break;
                    case SDLK_s:
                        smoothPoints();
                        break;
                }
            }
		}
        
        if (running)
        {
            Point goalPoint = gPath.getPoint(gRobot.getX(), gRobot.getY(), gRobot.getLookAheadDist());
            gPath.setGoalPoint(goalPoint.getX(), goalPoint.getY());
            gRobot.setGoalPoint(goalPoint.getX(), goalPoint.getY());

            gRobot.moveToGoal(goalPoint.getCurvature());
            gRobot.updateWheelPower(frameDelta);
            gRobot.updatePosition(frameDelta);
        }
        
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
    
    if (pathTexture != NULL)
        SDL_RenderCopy(gRenderer, pathTexture, NULL, NULL);
    else
        gPath.renderPath(gRenderer, mapScale);
    gPath.renderGoal(gRenderer, mapScale);

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

void run()
{
    if (!gPath.hasPoint())
        return;

    gRobot.reset();
    gPath.reset();
    gPath.setPointCurvature();
    running = !running;
    
    if (running)
        getPathTexture();
    else
        pathTexture = NULL;
}

void placePoint()
{
    if (running)
        return;

    int x, y;
    SDL_GetMouseState(&x, &y);
    
    double xScaled = x / mapScale;
    double yScaled = y / mapScale;

    gPath.addPoint(xScaled, yScaled);
}

void clearPoints()
{
    if (running)
        return;

    gPath.resetPath();
}

void smoothPoints()
{
    if (running)
        return;

    gPath.smoothPoints();
}

void getPathTexture()
{
    pathTexture = SDL_CreateTexture(gRenderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH, SCREEN_HEIGHT);

    SDL_SetTextureBlendMode(pathTexture, SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget(gRenderer, pathTexture);
    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0);
    SDL_RenderClear(gRenderer);
    gPath.renderPath(gRenderer, mapScale);

    SDL_SetRenderTarget(gRenderer, NULL);
}

void setScale()
{
    mapScale = SCREEN_WIDTH / (double)FIELD_SIZE;
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
