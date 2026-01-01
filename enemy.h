#pragma once
#include <SDL3/SDL.h>
#include "projectile.h"

#define Cols 11
#define Rows 5
#define EnemyCount (Cols * Rows)
#define EnemyWidth 48.0f
#define EnemyHeight 24.0f
#define EnemyStartSpeed 10.0f

#define UfoWidth 64.0f
#define UfoHeight 32.0f
#define UfoSpeed 150.0f
#define UfoSpawnInterval 40000
typedef struct {
    SDL_FRect rect;
    float speed;
    int direction;
    int hp;
    int type;
    int frame;
    bool exploding;
    uint64_t explosionEnd;
    SDL_Texture* texture;
    Projectile bullet;
} Enemy;

typedef struct {
    SDL_FRect rect;
    float speed;
    int direction;
    int lastdirection;
    int hp;
    bool active;
    bool exploding;
    SDL_Texture* texture;
    uint64_t lastSpawnTime;
} Ufo;

void EnemiesTexture(SDL_Renderer* renderer);
void EnemiesBox(Enemy enemies[]);
void EnemiesReset(Enemy enemies[]);
void EnemiesInit(Enemy enemies[], SDL_Renderer* renderer);
void EnemiesUpdate(Enemy enemies[], uint64_t currentTime, float windowWidth, float dt);
void EnemiesRender(Enemy enemies[], SDL_Renderer* renderer);
void EnemiesRenderBullets(Enemy enemies[], SDL_Renderer* renderer);
bool EnemiesDead(Enemy enemies[]);
bool EnemiesLow(Enemy enemies[], float y);
void EnemiesCleanup();

void UfoInit(Ufo* ufo, SDL_Renderer* renderer);
void UfoUpdate(Ufo* ufo, uint64_t currentTime, float windowWidth, float dt);
void UfoRender(Ufo* ufo, SDL_Renderer* renderer);
void UfoCleanup();