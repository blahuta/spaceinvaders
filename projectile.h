#pragma once
#include <SDL3/SDL.h>

#define ProjWidth 6.0f
#define ProjHeight 15.0f
#define ProjSpeed 600.0f
#define OwnerPlayer 0
#define OwnerEnemy 1
#define ProjAnimFrames 4

typedef struct
{
    SDL_FRect rect;
    bool active;
    float speed;
    int owner;
    uint64_t spawnTime;
    int frame;
} Projectile;

void ProjectileInit(Projectile *proj, float x, float y, float speed, int owner);
void ProjectileUpdate(Projectile *proj, float dt);
void ProjectileRender(const Projectile *proj, SDL_Renderer *renderer);
bool ProjectileCheckCollision(const Projectile *proj, SDL_FRect target);
void ProjectileDeactivate(Projectile *proj);
void SetProjectileTextures(SDL_Texture **enemyTextures);