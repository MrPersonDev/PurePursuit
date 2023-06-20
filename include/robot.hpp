#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <string>

class Robot
{
    public:
        Robot();
        ~Robot();
        void init(SDL_Renderer *renderer);
        void reset();
        int oppositeSign(double n);
        void moveToGoal(double curvature);
        void updateWheelPower(double delta);
        void updatePosition(double delta);
        void setGoalPoint(double x, double y);
        double getX();
        double getY();
        double getHeading();
        double getLookAheadDist();
        void render(SDL_Renderer *renderer, double scale);
        void free();

    private:
        SDL_Texture *robotTexture;
        static const double LOOK_AHEAD_DIST;
        static const double DRIVE_WIDTH;
        static const double WHEEL_DIAMETER;
        static const double ACCELERATION;
        static const int RPM;
        static const double ANGULAR_RESPONSE;
        static const double LINEAR_RESPONSE;
        static const SDL_Color LOOK_AHEAD_CIRCLE_COLOR;
        static const SDL_Color LOOK_AHEAD_LINE_COLOR;
        static const int LOOK_AHEAD_LINE_WIDTH;
        double x, y;
        double goalX, goalY;
        double heading;
        double desiredLeftPower, desiredRightPower;
        double leftPower, rightPower;
        bool running;
};