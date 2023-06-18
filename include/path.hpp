#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <vector>

class Path
{
    public:
        Path();
        ~Path();
        void addPoint(SDL_Point point);
        void render(SDL_Renderer *renderer, double scale);
        void free();
        
    private:
        static const int LINE_WIDTH;
        static const int POINT_SIZE;
        static const SDL_Color LINE_COLOR;
        static const SDL_Color POINT_COLOR;
        std::vector<SDL_Point> points;
};