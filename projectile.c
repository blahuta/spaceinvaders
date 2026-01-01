#include "projectile.h"
#include "text.h"

static SDL_Texture* EnemyBulletTextures[ProjAnimFrames] = {NULL};
static const uint64_t AnimFrameTime = 100;

void ProjectileInit(Projectile* proj, float x, float y, float speed, int owner) {
    proj->rect.w = ProjWidth;
    proj->rect.h = ProjHeight;
    proj->rect.x = x;
    proj->rect.y = y;
    proj->speed = (owner == OwnerPlayer) ? -speed : speed;
    proj->owner = owner;
    proj->active = true;
    proj->spawnTime = SDL_GetPerformanceCounter();
    proj->frame = 0;
}

void ProjectileUpdate(Projectile* proj, float dt) {
    if (!proj->active) return;
    proj->rect.y += proj->speed * dt;

    if (proj->owner == OwnerEnemy) {
        uint64_t elapsed = SDL_GetPerformanceCounter() - proj->spawnTime;
        uint64_t elapsedMs = (elapsed * 1000) / SDL_GetPerformanceFrequency();
        proj->frame = (elapsedMs / AnimFrameTime) % ProjAnimFrames;
    }

    if (proj->rect.y < -50.0f || proj->rect.y > 2000.0f) {
        proj->active = false;
    }   
}

void ProjectileRender(const Projectile* proj, SDL_Renderer* renderer) {
    if (proj->active) {
        if (proj->owner == OwnerPlayer) {
                SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                SDL_RenderFillRect(renderer, &proj->rect);
        } else {
            if (EnemyBulletTextures[proj->frame]) {
                SDL_RenderTexture(renderer, EnemyBulletTextures[proj->frame], NULL, &proj->rect);
            } 
        }
    }
}

bool ProjectileCheckCollision(const Projectile* proj, SDL_FRect target) {
    if (!proj->active) return false;
    return CheckCollision(proj->rect, target);
}

void ProjectileDeactivate(Projectile* proj) {
    proj->active = false;
    proj->rect.x = -1000.0f; 
}

void SetProjectileTextures(SDL_Texture** enemyTextures) {
    for (int i = 0; i < ProjAnimFrames; i++) {
        EnemyBulletTextures[i] = enemyTextures[i];
    }
}