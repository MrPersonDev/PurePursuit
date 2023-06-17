#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

class Robot
{
    public:
        Robot();
        ~Robot();
        void init(SDL_Renderer *renderer);
        void updateWheelPower(double delta);
        void updatePosition(double delta);
        void render(SDL_Renderer *renderer);
        void free();

    private:
        SDL_Texture *robotTexture;
        const static double DRIVE_WIDTH;
        const static double WHEEL_DIAMETER;
        const static double ACCELERATION;
        const static int RPM;
        const static int SIZE;
        double x, y;
        double heading;
        double desiredLeftPower, desiredRightPower;
        double leftPower, rightPower;
};