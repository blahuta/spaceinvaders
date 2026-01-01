#pragma once
#include <SDL3/SDL.h>

SDL_Texture* LoadTexture(SDL_Renderer* renderer, const char* path);

SDL_Surface* LoadSurface(const char* path);

bool CheckCollision(SDL_FRect a, SDL_FRect b);