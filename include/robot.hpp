#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

class Robot
{
    public:
        Robot();
        ~Robot();
        void init(SDL_Renderer *renderer);
        void render(SDL_Renderer *renderer);
        void free();

    private:
        SDL_Texture *robotTexture;
        const static int SIZE;
        double x, y;
        double heading;
};