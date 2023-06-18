#include "path.hpp"

const int Path::LINE_WIDTH = 3;
const int Path::POINT_SIZE = 5;
const SDL_Color Path::LINE_COLOR = {255, 0, 0, 255};
const SDL_Color Path::POINT_COLOR = {255, 255, 0, 255};

Path::Path()
{ 
}

Path::~Path()
{
    free();
}

void Path::addPoint(SDL_Point point)
{
    points.push_back(point);
}

void Path::render(SDL_Renderer *renderer)
{
    for (int i = 0; i < points.size()-1; i++)
    {
        SDL_Point curPoint = points[i];
        SDL_Point nextPoint = points[i+1];
        thickLineRGBA(renderer, curPoint.x, curPoint.y, nextPoint.x, nextPoint.y, LINE_WIDTH, LINE_COLOR.r, LINE_COLOR.g, LINE_COLOR.b, LINE_COLOR.a);
    }

    for (SDL_Point point : points)
        filledCircleRGBA(renderer, point.x, point.y, POINT_SIZE, POINT_COLOR.r, POINT_COLOR.g, POINT_COLOR.b, POINT_COLOR.a);
}

void Path::free()
{
    
}
