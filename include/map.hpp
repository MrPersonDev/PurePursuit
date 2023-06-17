#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

class Map
{
    public:
        Map();
        ~Map();
        void init(SDL_Renderer *renderer);
        void render(SDL_Renderer *renderer);
        void free();

    private:
        SDL_Texture *mapTexture;
};