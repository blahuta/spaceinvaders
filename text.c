#include "text.h"

SDL_Texture* LoadTexture(SDL_Renderer* renderer, const char* path) {
    SDL_Surface* surface = SDL_LoadBMP(path);

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);

    return texture;
}

SDL_Surface* LoadSurface(const char* path) {
    SDL_Surface* surface = SDL_LoadBMP(path);
    return surface;
}

bool CheckCollision(SDL_FRect a, SDL_FRect b) {
    return (a.x < b.x + b.w && a.x + a.w > b.x && a.y < b.y + b.h && a.y + a.h > b.y);
}

/*bool CheckCollision(SDL_FRect a, SDL_FRect b) { //lol sdl ma builtin aabb
    return SDL_HasRectIntersection(&a, &b);
}*/