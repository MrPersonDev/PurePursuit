#include "robot.hpp"

const int Robot::SIZE = 100;

Robot::Robot() { }

Robot::~Robot()
{
    free();
}

void Robot::init(SDL_Renderer *renderer)
{
    std::string path = std::string(SDL_GetBasePath()) + "assets/images/squirrel.png";
    robotTexture = IMG_LoadTexture(renderer, path.c_str());
    
    x = 0.0;
    y = 0.0;
    heading = 0.0;
}

void Robot::render(SDL_Renderer *renderer)
{
    SDL_Rect renderQuad = {x, y, SIZE, SIZE};
    SDL_RenderCopyEx(renderer, robotTexture, NULL, &renderQuad, heading * (180.0 / M_PI), NULL, SDL_FLIP_NONE);
}

void Robot::free()
{
    SDL_DestroyTexture(robotTexture);
}