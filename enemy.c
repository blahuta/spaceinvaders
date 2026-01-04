#include "enemy.h"
#include "text.h"
#include "projectile.h"
#include <stdlib.h>

static SDL_Texture *Textures[7];
static SDL_Texture *EnemyBulletTextures[ProjAnimFrames];
static bool TexturesLoaded = false;
static SDL_Texture *UfoExplosion;

static uint64_t LastMoveTime = 0;
static uint64_t LastEnemyShotTime = 0;
static int Wave = 0;
static uint64_t MoveInterval = 400;
static const uint64_t ShotInterval = 800;
static const uint64_t ExplosionDuration = 500;

static bool IsTimerReady(uint64_t *lastTime, uint64_t intervalMs, uint64_t currentTime)
{
    uint64_t freq = SDL_GetPerformanceFrequency();
    if (currentTime > *lastTime + (intervalMs * freq) / 1000)
    {
        *lastTime = currentTime;
        return true;
    }
    return false;
}

void EnemiesTexture(SDL_Renderer *renderer)
{
    if (!TexturesLoaded)
    {
        Textures[0] = LoadTexture(renderer, "texture/alien0.bmp");
        Textures[1] = LoadTexture(renderer, "texture/alienp0.bmp");
        Textures[2] = LoadTexture(renderer, "texture/alien1.bmp");
        Textures[3] = LoadTexture(renderer, "texture/alienp1.bmp");
        Textures[4] = LoadTexture(renderer, "texture/alien2.bmp");
        Textures[5] = LoadTexture(renderer, "texture/alienp2.bmp");
        Textures[6] = LoadTexture(renderer, "texture/exp.bmp");

        EnemyBulletTextures[0] = LoadTexture(renderer, "texture/shot1.bmp");
        EnemyBulletTextures[1] = LoadTexture(renderer, "texture/shot2.bmp");
        EnemyBulletTextures[2] = LoadTexture(renderer, "texture/shot3.bmp");
        EnemyBulletTextures[3] = LoadTexture(renderer, "texture/shot4.bmp");

        SetProjectileTextures(EnemyBulletTextures);
        TexturesLoaded = true;
    }
}
void EnemiesBox(Enemy enemies[])
{
    float startX = 50.0f;
    float startY = 100.0f;

    for (int j = 0; j < Rows; j++)
    {
        for (int i = 0; i < Cols; i++)
        {
            int idx = i + j * Cols;

            enemies[idx].rect.w = EnemyWidth;
            enemies[idx].rect.h = EnemyHeight;
            enemies[idx].rect.x = startX + i * (EnemyWidth + 5.0f);
            enemies[idx].rect.y = startY + j * (EnemyHeight + 5.0f);

            enemies[idx].hp = 1;
            enemies[idx].speed = EnemyStartSpeed;
            enemies[idx].direction = 1;
            enemies[idx].exploding = false;
            enemies[idx].explosionStart = 0;
            enemies[idx].frame = 0;

            if (j == 0)
            {
                enemies[idx].type = 0;
            }
            else if (j < 3)
            {
                enemies[idx].type = 1;
            }
            else
            {
                enemies[idx].type = 2;
            }

            enemies[idx].texture = Textures[enemies[idx].type * 2];
            ProjectileInit(&enemies[idx].bullet, 0, 0, 0, OwnerEnemy);
            enemies[idx].bullet.active = false;
        }
    }
}

void EnemiesReset(Enemy enemies[])
{
    Wave += 1;
    int NewInt = 450 - (Wave * 50);
    if (NewInt < 50)
    {
        NewInt = 50;
    }
    MoveInterval = (uint64_t)NewInt;
    EnemiesBox(enemies);
    LastMoveTime = SDL_GetPerformanceCounter();
    LastEnemyShotTime = SDL_GetPerformanceCounter();
}

void EnemiesInit(Enemy enemies[], SDL_Renderer *renderer)
{
    EnemiesTexture(renderer);
    EnemiesReset(enemies);
}

void EnemiesUpdate(Enemy enemies[], uint64_t currentTime, float windowWidth, float dt)
{
    for (int i = 0; i < EnemyCount; i++)
    {
        if (enemies[i].exploding)
        {
            uint64_t temp = SDL_GetPerformanceFrequency();
            uint64_t elapsed = (currentTime - enemies[i].explosionStart) * 1000 / temp;

            if (elapsed > 300)
            {
                enemies[i].exploding = false;
                enemies[i].rect.x = -1000.0f;
            }
        }
        else if (enemies[i].hp <= 0)
        {
            enemies[i].exploding = true;
            enemies[i].texture = Textures[6];
            enemies[i].explosionStart = currentTime;
        }
    }

    if (IsTimerReady(&LastMoveTime, MoveInterval, currentTime))
    {
        LastMoveTime = currentTime;
        bool hit = false;

        for (int i = 0; i < EnemyCount; i++)
        {
            if (enemies[i].hp <= 0)
                continue;
            enemies[i].rect.x += enemies[i].speed * enemies[i].direction;
            enemies[i].frame = (enemies[i].frame == 0) ? 1 : 0;
            enemies[i].texture = Textures[(enemies[i].type * 2) + enemies[i].frame];
            if (enemies[i].rect.x <= 0 || enemies[i].rect.x + enemies[i].rect.w >= windowWidth)
            {
                hit = true;
            }
        }

        if (hit)
        {
            for (int i = 0; i < EnemyCount; i++)
            {
                if (enemies[i].hp <= 0)
                    continue;
                enemies[i].direction *= -1;
                enemies[i].rect.y += 20.0f;
                enemies[i].rect.x += enemies[i].speed * enemies[i].direction;
            }
        }
    }

    for (int i = 0; i < EnemyCount; i++)
    {
        if (enemies[i].bullet.active)
        {
            ProjectileUpdate(&enemies[i].bullet, dt);
        }
    }

    if (IsTimerReady(&LastEnemyShotTime, ShotInterval, currentTime))
    {
        LastEnemyShotTime = currentTime;
        bool OneBullet = false;

        for (int i = 0; i < EnemyCount; i++)
        {
            if (enemies[i].bullet.active)
            {
                OneBullet = true;
                break;
            }
        }

        int aliveEnemies[EnemyCount];
        int AliveCount = 0;
        if (!OneBullet)
        {
            for (int i = 0; i < EnemyCount; i++)
            {
                if (enemies[i].hp > 0)
                {
                    aliveEnemies[AliveCount++] = i;
                }
            }
        }

        if (AliveCount > 0)
        {
            int randomIdx = aliveEnemies[rand() % AliveCount];
            float bulletX = enemies[randomIdx].rect.x + (enemies[randomIdx].rect.w * 2.0f) - (ProjWidth * 2.0f);
            float bulletY = enemies[randomIdx].rect.y + enemies[randomIdx].rect.h;
            ProjectileInit(&enemies[randomIdx].bullet, bulletX, bulletY, ProjSpeed / 2.0f, OwnerEnemy);
        }
    }
}

void EnemiesRender(Enemy enemies[], SDL_Renderer *renderer)
{
    for (int i = 0; i < EnemyCount; i++)
    {
        if (enemies[i].hp > 0 || enemies[i].exploding)
        {
            SDL_RenderTexture(renderer, enemies[i].texture, NULL, &enemies[i].rect);
        }
    }
}

void EnemiesRenderBullets(Enemy enemies[], SDL_Renderer *renderer)
{
    for (int i = 0; i < EnemyCount; i++)
    {
        if (enemies[i].bullet.active)
        {
            ProjectileRender(&enemies[i].bullet, renderer);
        }
    }
}

bool EnemiesDead(Enemy enemies[])
{
    for (int i = 0; i < EnemyCount; i++)
    {
        if (enemies[i].hp > 0)
        {
            return false;
        }
    }
    return true;
}

bool EnemiesLow(Enemy enemies[], float y)
{
    for (int i = 0; i < EnemyCount; i++)
    {
        if (enemies[i].hp > 0 && (enemies[i].rect.y + enemies[i].rect.h >= y))
        {
            return true;
        }
    }
    return false;
}

void EnemiesCleanup()
{
    if (TexturesLoaded)
    {
        for (int i = 0; i < 7; i++)
        {
            if (Textures[i])
                SDL_DestroyTexture(Textures[i]);
        }
        for (int i = 0; i < ProjAnimFrames; i++)
        {
            if (EnemyBulletTextures[i])
                SDL_DestroyTexture(EnemyBulletTextures[i]);
        }
        TexturesLoaded = false;
    }
}

void UfoInit(Ufo *ufo, SDL_Renderer *renderer)
{
    ufo->rect.w = UfoWidth;
    ufo->rect.h = UfoHeight;
    ufo->rect.x = -UfoWidth;
    ufo->rect.y = 30.0f;
    ufo->speed = UfoSpeed;
    ufo->direction = 1;
    ufo->lastdirection = 1;
    ufo->hp = 1;
    ufo->active = false;
    ufo->exploding = false;
    ufo->texture = LoadTexture(renderer, "texture/ufo.bmp");
    ufo->lastSpawnTime = 0;
    UfoExplosion = LoadTexture(renderer, "texture/ufoexp.bmp");
}

void UfoUpdate(Ufo *ufo, uint64_t currentTime, float windowWidth, float dt)
{
    if (!ufo->active)
    {
        if (IsTimerReady(&ufo->lastSpawnTime, UfoSpawnInterval, currentTime))
        {
            ufo->active = true;
            ufo->rect.x = (ufo->lastdirection == 1) ? -UfoWidth : windowWidth;
            ufo->direction = (ufo->lastdirection == 1) ? 1 : -1;
            ufo->lastdirection = ufo->direction;
            ufo->lastSpawnTime = currentTime;
            ufo->exploding = false;
            ufo->hp = 1;
        }
        return;
    }

    if (ufo->exploding)
    {
        uint64_t freq = SDL_GetPerformanceFrequency();
        uint64_t elapsed = (currentTime - ufo->lastSpawnTime) * 1000 / freq;

        if (elapsed > ExplosionDuration)
        {
            ufo->exploding = false;
            ufo->active = false;
            ufo->rect.x = -1000.0f;
        }
        return;
    }

    ufo->rect.x += ufo->speed * ufo->direction * dt;

    if (ufo->rect.x > windowWidth || ufo->rect.x + ufo->rect.w < 0)
    {
        ufo->active = false;
    }
}

void UfoRender(Ufo *ufo, SDL_Renderer *renderer)
{
    if (ufo->active)
    {
        if (ufo->exploding)
        {
            SDL_RenderTexture(renderer, UfoExplosion, NULL, &ufo->rect);
            return;
        }
        SDL_RenderTexture(renderer, ufo->texture, NULL, &ufo->rect);
    }
}

void UfoCleanup(Ufo *ufo)
{
    SDL_DestroyTexture(ufo->texture);
    SDL_DestroyTexture(UfoExplosion);
}