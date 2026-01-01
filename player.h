#pragma once
#include <SDL3/SDL.h>
#include "projectile.h"

#define PlayerWidth 64.0f
#define PlayerHeight 32.0f
#define PlayerSpeed 400.0f
#define PlayerHP 3
typedef struct {
    SDL_FRect rect;
    float speed;
    int hp;
    Projectile bullet;
    SDL_Texture* texture;
    SDL_Texture* damage[2];
    uint64_t explosion;
    bool explode;
} Player;

void PlayerInit(Player* player, SDL_Renderer* renderer, float windowWidth, float windowHeight);
void PlayerUpdate(Player* player, const bool* keys, float dt, float windowWidth);
void PlayerRender(Player* player, SDL_Renderer* renderer);
void PlayerDamage(Player* player);
void PlayerDestroy(Player* player);