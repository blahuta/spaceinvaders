#include "text.h"

SDL_Surface *LoadSurface(const char *path)
{
    SDL_Surface *surface = SDL_LoadBMP(path);
    return surface;
}

SDL_Texture *LoadTexture(SDL_Renderer *renderer, const char *path)
{
    SDL_Surface *surface = LoadSurface(path);

    if (!surface)
    {
        SDL_Log("Failed to load %s - texture", path);
        return NULL;
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);

    if (!texture)
    {
        SDL_Log("Failed to create %s - texture from surface ", path);
    }
    return texture;
}

bool CheckCollision(SDL_FRect a, SDL_FRect b)
{
    return (a.x < b.x + b.w && a.x + a.w > b.x && a.y < b.y + b.h && a.y + a.h > b.y);
}