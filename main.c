#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "player.h"
#include "enemy.h"
#include "bunker.h"
#include "projectile.h"
#include "text.h"
#include "ui.h"

#define WindowWidth 1000.0f
#define WindowHeight 800.0f

typedef enum
{
    menu,
    game,
    end
} GameState;

int main(int _argc, char *_argv[])
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("Init fail: %s", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Space Invaders", WindowWidth, WindowHeight, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);
    SDL_SetRenderVSync(renderer, 1);
    if (!window || !renderer)
    {
        SDL_Log("Creation fail: %s", SDL_GetError());
        return 1;
    }

    UIInit(renderer);
    Player player;
    Enemy enemies[EnemyCount];
    Bunker bunkers[BunkerCount];
    Ufo ufo;

    GameState state = menu;
    int score = 0;
    bool ScoreSaved = false;
    uint64_t LastTime = SDL_GetPerformanceCounter();
    bool Running = true;
    SDL_Event Event;

    while (Running)
    {
        uint64_t Now = SDL_GetPerformanceCounter();
        float Dt = (float)((Now - LastTime) / (double)SDL_GetPerformanceFrequency());
        LastTime = Now;

        while (SDL_PollEvent(&Event))
        {
            if (Event.type == SDL_EVENT_QUIT)
            {
                Running = false;
            }
            if (Event.type == SDL_EVENT_KEY_DOWN)
            {
                if (Event.key.key == SDLK_RETURN && state == menu)
                {
                    state = game;
                    PlayerInit(&player, renderer, WindowWidth, WindowHeight);
                    EnemiesInit(enemies, renderer);
                    BunkersInit(bunkers, renderer, WindowWidth);
                    UfoInit(&ufo, renderer);
                    score = 0;
                }
                if (Event.key.key == SDLK_RETURN && state == end)
                {
                    state = menu;
                    ScoreSaved = false;
                }
            }
        }

        const bool *Keys = SDL_GetKeyboardState(NULL);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        if (state == menu)
        {
            UIMenu(renderer, WindowWidth, WindowHeight);
        }
        else if (state == game)
        {
            PlayerUpdate(&player, Keys, Dt, WindowWidth);
            EnemiesUpdate(enemies, Now, WindowWidth, Dt);
            UfoUpdate(&ufo, Now, WindowWidth, Dt);

            if (player.bullet.active)
            {
                for (int i = 0; i < EnemyCount; i++)
                {
                    if (enemies[i].hp > 0)
                    {
                        if (CheckCollision(player.bullet.rect, enemies[i].rect))
                        {
                            player.bullet.active = false;
                            enemies[i].hp--;
                            if (enemies[i].type == 0)
                            {
                                score += 30;
                            }
                            else if (enemies[i].type == 2)
                            {
                                score += 20;
                            }
                            else
                            {
                                score += 10;
                            }
                        }
                    }
                }

                if (player.bullet.active)
                {
                    for (int i = 0; i < BunkerCount; i++)
                    {
                        if (BunkersCheckCollision(&bunkers[i], &player.bullet.rect))
                        {
                            player.bullet.active = false;
                            break;
                        }
                    }
                }
            }

            for (int i = 0; i < EnemyCount; i++)
            {
                if (enemies[i].bullet.active)
                {
                    if (ProjectileCheckCollision(&enemies[i].bullet, player.rect))
                    {
                        enemies[i].bullet.active = false;
                        PlayerDamage(&player);
                    }
                }
            }

            for (int e = 0; e < EnemyCount; e++)
            {
                if (enemies[e].hp > 0 && enemies[e].bullet.active)
                {
                    for (int b = 0; b < BunkerCount; b++)
                    {
                        if (BunkersCheckCollision(&bunkers[b], &enemies[e].bullet.rect))
                        {
                            enemies[e].bullet.active = false;
                            break;
                        }
                    }
                }
            }

            if (player.bullet.active && ufo.active)
            {
                if (CheckCollision(player.bullet.rect, ufo.rect))
                {
                    player.bullet.active = false;
                    ufo.hp--;
                    if (ufo.hp <= 0)
                    {
                        ufo.exploding = true;
                        ufo.lastSpawnTime = Now;
                        score += 100;
                    }
                }
            }

            if (EnemiesDead(enemies))
            {
                EnemiesReset(enemies);
                score += 100;
            }

            PlayerRender(&player, renderer);
            EnemiesRender(enemies, renderer);
            EnemiesRenderBullets(enemies, renderer);
            BunkersRender(bunkers, renderer);
            UfoRender(&ufo, renderer);
            UIHud(renderer, player.hp, score, WindowWidth, WindowHeight);

            if (player.hp <= 0)
            {
                state = end;
            }

            if (EnemiesLow(enemies, 500))
            {
                state = end;
            }
        }
        else if (state == end)
        {
            if (score > 0 && !ScoreSaved)
            {
                UIAddScore(score);
                ScoreSaved = true;
            }
            UIGameOver(renderer, WindowWidth, WindowHeight, score);
        }

        SDL_RenderPresent(renderer);
    }

    PlayerDestroy(&player);
    EnemiesCleanup();
    BunkersCleanup();
    UfoCleanup(&ufo);
    UICleanup();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}