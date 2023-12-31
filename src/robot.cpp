#include "robot.hpp"

const double Robot::LOOK_AHEAD_DIST = 15.0;
const double Robot::DRIVE_WIDTH = 18;
const double Robot::WHEEL_DIAMETER = 4;
const double Robot::ACCELERATION = 400.0; // power / second
const int Robot::RPM = 200;
const double Robot::ANGULAR_RESPONSE = 50.0;
const double Robot::LINEAR_RESPONSE = 50.0;
const int Robot::LOOK_AHEAD_LINE_WIDTH = 2;
const SDL_Color Robot::LOOK_AHEAD_CIRCLE_COLOR = {255, 255, 0, 255};
const SDL_Color Robot::LOOK_AHEAD_LINE_COLOR = {0, 255, 255, 255};

Robot::Robot()
{
    goal = {0.0, 0.0};
}

Robot::~Robot()
{
    free();
}

void Robot::init(SDL_Renderer *renderer)
{
    std::string path = std::string(SDL_GetBasePath()) + "assets/images/squirrel.png";
    robotTexture = IMG_LoadTexture(renderer, path.c_str());
    
    reset();
}

void Robot::reset()
{
    x = DRIVE_WIDTH/2;
    y = DRIVE_WIDTH/2;
    heading = 0.0;
    desiredLeftPower = 0.0;
    desiredRightPower = 0.0;
    leftPower = 0.0;
    rightPower = 0.0;
}

int Robot::oppositeSign(double n)
{
    if (n == 0)
        return 0;
    return n < 0 ? 1 : -1;
}

void Robot::moveToGoal(double curvature)
{
    double absTargetAngle = atan2(goal.getY() - y, goal.getX() - x);
    if (absTargetAngle < 0)
        absTargetAngle += M_PI*2;
    
    double minAngle = absTargetAngle - heading;
    if (minAngle > M_PI || minAngle < -M_PI)
        minAngle = oppositeSign(minAngle) * (M_PI*2 - abs(minAngle));
    
    double turnVel = ANGULAR_RESPONSE * minAngle;
    double linearVel = LINEAR_RESPONSE * (1 - curvature) * (1 - abs(minAngle/M_PI));
    if (goal.isStopping()) {
        double dist = sqrt(pow(goal.getY() - y, 2) + pow(goal.getX() - x, 2));
        linearVel *= dist / LOOK_AHEAD_DIST;
    }
    
    desiredLeftPower = linearVel + turnVel;
    desiredRightPower = linearVel - turnVel;
    
    if (desiredLeftPower < -100)
        desiredLeftPower = -100;
    else if (desiredLeftPower > 100)
        desiredLeftPower = 100;

    if (desiredRightPower < -100)
        desiredRightPower = -100;
    else if (desiredRightPower > 100)
        desiredRightPower = 100;
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

void Robot::setGoalPoint(Point point)
{
    goal = point;
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
    circleRGBA(renderer, x * scale, y * scale, LOOK_AHEAD_DIST * scale, LOOK_AHEAD_CIRCLE_COLOR.r, LOOK_AHEAD_CIRCLE_COLOR.g, LOOK_AHEAD_CIRCLE_COLOR.b, LOOK_AHEAD_CIRCLE_COLOR.a);
    thickLineRGBA(renderer, x * scale, y * scale, goal.getX() * scale, goal.getY() * scale, LOOK_AHEAD_LINE_WIDTH, LOOK_AHEAD_LINE_COLOR.r, LOOK_AHEAD_LINE_COLOR.g, LOOK_AHEAD_LINE_COLOR.b, LOOK_AHEAD_LINE_COLOR.a);

    SDL_Rect renderQuad = {(x-DRIVE_WIDTH/2) * scale, (y-DRIVE_WIDTH/2) * scale, DRIVE_WIDTH * scale, DRIVE_WIDTH * scale};
    SDL_RenderCopyEx(renderer, robotTexture, NULL, &renderQuad, heading * (180.0 / M_PI), NULL, SDL_FLIP_NONE);
}

void Robot::free()
{
    SDL_DestroyTexture(robotTexture);
}
