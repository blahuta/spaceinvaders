#pragma once
#include <SDL3/SDL.h>

#define BunkerCount 4
#define Size 32.0f 

typedef struct {
    SDL_FRect rect;
    int damage;  
    bool active;
    SDL_Texture** textures;
} BunkerPart;

typedef struct {

    BunkerPart parts[6];  // Indexy: 0=LC, 1=Full, 2=RC, 3=LM, 4=Empty, 5=RM
} Bunker;

void BunkersInit(Bunker bunkers[], SDL_Renderer* renderer, float windowWidth);
void BunkersRender(Bunker bunkers[], SDL_Renderer* renderer);
bool BunkersCheckCollision(Bunker* bunker, SDL_FRect* bulletRect);
void BunkersCleanup();