#include "robot.hpp"

const double Robot::DRIVE_WIDTH = 10;
const double Robot::WHEEL_DIAMETER = 4;
const int Robot::RPM = 200;
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
    leftPower = 0.0;
    rightPower = 0.0;
}

void Robot::updatePosition(double delta)
{
    double leftVelocity = leftPower / 100.0 * RPM / 60000.0 * WHEEL_DIAMETER * M_PI;
    double rightVelocity = rightPower / 100.0 * RPM / 60000.0 * WHEEL_DIAMETER * M_PI;

    float linear = (leftVelocity + rightVelocity) / 2.0f;
    float angular = (rightVelocity - leftVelocity) / DRIVE_WIDTH;
    
    x += cos(heading) * linear;
    y += sin(heading) * linear;
    heading -= angular;
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