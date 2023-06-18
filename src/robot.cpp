#include "robot.hpp"

const double Robot::LOOK_AHEAD_DIST = 30.0;
const double Robot::DRIVE_WIDTH = 18;
const double Robot::WHEEL_DIAMETER = 4;
const double Robot::ACCELERATION = 100.0; // power / second
const int Robot::RPM = 200;

Robot::Robot() { }

Robot::~Robot()
{
    free();
}

void Robot::init(SDL_Renderer *renderer)
{
    std::string path = std::string(SDL_GetBasePath()) + "assets/images/squirrel.png";
    robotTexture = IMG_LoadTexture(renderer, path.c_str());
    
    x = DRIVE_WIDTH/2;
    y = DRIVE_WIDTH/2 + 80;
    heading = 0.0;
    desiredLeftPower = 0.0;
    desiredRightPower = 0.0;
    leftPower = 0.0;
    rightPower = 0.0;
}

void Robot::updateWheelPower(double delta)
{
    double change = ACCELERATION / 1000.0 * delta;

    if (leftPower < desiredLeftPower)
        leftPower = std::min(leftPower + change, desiredLeftPower);
    else if (leftPower > desiredLeftPower)
        leftPower = std::max(leftPower - change, desiredLeftPower);
    
    if (rightPower < desiredRightPower)
        rightPower = std::min(rightPower + change, desiredRightPower);
    else if (rightPower > desiredRightPower)
        rightPower = std::max(rightPower - change, desiredRightPower);
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

double Robot::getX()
{
    return x;
}

double Robot::getY()
{
    return y;
}

double Robot::getHeading()
{
    return heading;
}

double Robot::getLookAheadDist()
{
    return LOOK_AHEAD_DIST;
}

void Robot::render(SDL_Renderer *renderer, double scale)
{
    SDL_Rect renderQuad = {(x-DRIVE_WIDTH/2) * scale, (y-DRIVE_WIDTH/2) * scale, DRIVE_WIDTH * scale, DRIVE_WIDTH * scale};
    SDL_RenderCopyEx(renderer, robotTexture, NULL, &renderQuad, heading * (180.0 / M_PI), NULL, SDL_FLIP_NONE);
}

void Robot::free()
{
    SDL_DestroyTexture(robotTexture);
}