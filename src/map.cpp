#include "map.hpp"

Map::Map() { }

Map::~Map()
{
    free();
}

void Map::init(SDL_Renderer *renderer)
{
    std::string path = std::string(SDL_GetBasePath()) + "assets/images/overunderfield.png";
    mapTexture = IMG_LoadTexture(renderer, path.c_str());
}

void Map::render(SDL_Renderer *renderer)
{
    SDL_RenderCopy(renderer, mapTexture, NULL, NULL);
}

void Map::free()
{
    SDL_DestroyTexture(mapTexture);
}