#include "player.h"
#include "text.h"

static const uint64_t ExplosionDuration = 1000;
void PlayerInit(Player *player, SDL_Renderer *renderer, float windowWidth, float windowHeight)
{
    player->rect.w = PlayerWidth;
    player->rect.h = PlayerHeight;
    player->rect.x = (windowWidth - PlayerWidth) / 2.0f;
    player->rect.y = windowHeight - 100.0f;
    player->speed = PlayerSpeed;
    player->hp = PlayerHP;
    player->explode = false;
    player->explosion = 0;
    ProjectileInit(&player->bullet, 0, 0, 0, OwnerPlayer);
    player->bullet.active = false;
    player->texture = LoadTexture(renderer, "texture/play.bmp");
    player->damage[0] = LoadTexture(renderer, "texture/playexp0.bmp");
    player->damage[1] = LoadTexture(renderer, "texture/playexp1.bmp");
}

void PlayerUpdate(Player *player, const bool *keys, float dt, float windowWidth)
{
    if (keys[SDL_SCANCODE_LEFT])
        player->rect.x -= player->speed * dt;
    if (keys[SDL_SCANCODE_RIGHT])
        player->rect.x += player->speed * dt;

    if (player->rect.x < 0)
        player->rect.x = 0;
    if (player->rect.x + player->rect.w > windowWidth)
        player->rect.x = windowWidth - player->rect.w;

    if (keys[SDL_SCANCODE_SPACE] && !player->bullet.active)
    {
        float bulletX = player->rect.x + (PlayerWidth / 2.0f) - (ProjWidth / 2.0f);
        float bulletY = player->rect.y;
        ProjectileInit(&player->bullet, bulletX, bulletY, ProjSpeed, OwnerPlayer);
    }
    ProjectileUpdate(&player->bullet, dt);

    if (player->explode)
    {
        uint64_t now = SDL_GetPerformanceCounter();
        uint64_t elapsed = (now - player->explosion) * 1000 / SDL_GetPerformanceFrequency();

        if (elapsed > ExplosionDuration)
        {
            player->explode = false;

            player->rect.x = (windowWidth - PlayerWidth) / 2.0f;
        }
        return;
    }
}

void PlayerDamage(Player *player)
{
    if (player->explode)
        return;
    player->hp--;
    player->explode = true;
    player->explosion = SDL_GetPerformanceCounter();
    player->bullet.active = false;
}

void PlayerRender(Player *player, SDL_Renderer *renderer)
{
    if (player->hp <= 0 && !player->explode)
        return;

    if (player->texture)
    {
        SDL_RenderTexture(renderer, player->texture, NULL, &player->rect);
    }

    if (player->explode)
    {
        uint64_t now = SDL_GetPerformanceCounter();
        uint64_t freq = SDL_GetPerformanceFrequency();
        uint64_t elapsed = (now - player->explosion) * 1000 / freq;
        int frameIndex = (elapsed < (ExplosionDuration / 2)) ? 0 : 1;

        if (player->damage[frameIndex])
        {
            SDL_RenderTexture(renderer, player->damage[frameIndex], NULL, &player->rect);
        }
        return;
    }
    ProjectileRender(&player->bullet, renderer);
}

void PlayerDestroy(Player *player)
{
    SDL_DestroyTexture(player->texture);
    for (int i = 0; i < 2; i++)
    {
        if (player->damage[i])
            SDL_DestroyTexture(player->damage[i]);
    }
}