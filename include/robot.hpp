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
        void updateWheelPower(double delta);
        void updatePosition(double delta);
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
        static const SDL_Color LOOK_AHEAD_CIRCLE_COLOR;
        double x, y;
        double heading;
        double desiredLeftPower, desiredRightPower;
        double leftPower, rightPower;
};