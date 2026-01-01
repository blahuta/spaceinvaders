#include "bunker.h"
#include "text.h"

static SDL_Texture* TexLC[5]; //nevim zda je toto spravna moznost ulozeni textur, ale w/e
static SDL_Texture* TexFull[5];
static SDL_Texture* TexRC[5];
static SDL_Texture* TexLM[5];
static SDL_Texture* TexRM[5];
static SDL_Texture* TexE[5];
static bool Loaded = false;

void BunkersInit(Bunker bunkers[], SDL_Renderer* renderer, float windowWidth) {
    
    if (!Loaded) {
        TexLC[0] = LoadTexture(renderer, "texture/lc0.bmp");
        TexLC[1] = LoadTexture(renderer, "texture/lc1.bmp");
        TexLC[2] = LoadTexture(renderer, "texture/lc2.bmp");
        TexLC[3] = LoadTexture(renderer, "texture/lc3.bmp");
        TexLC[4] = LoadTexture(renderer, "texture/lc4.bmp");

        TexFull[0] = LoadTexture(renderer, "texture/full0.bmp");
        TexFull[1] = LoadTexture(renderer, "texture/full1.bmp");
        TexFull[2] = LoadTexture(renderer, "texture/full2.bmp");
        TexFull[3] = LoadTexture(renderer, "texture/full3.bmp");
        TexFull[4] = LoadTexture(renderer, "texture/full4.bmp");

        TexRC[0] = LoadTexture(renderer, "texture/rc0.bmp");
        TexRC[1] = LoadTexture(renderer, "texture/rc1.bmp");
        TexRC[2] = LoadTexture(renderer, "texture/rc2.bmp");
        TexRC[3] = LoadTexture(renderer, "texture/rc3.bmp");
        TexRC[4] = LoadTexture(renderer, "texture/rc4.bmp");

        TexLM[0] = LoadTexture(renderer, "texture/lm0.bmp"); 
        TexLM[1] = LoadTexture(renderer, "texture/lm1.bmp");
        TexLM[2] = LoadTexture(renderer, "texture/lm2.bmp");
        TexLM[3] = LoadTexture(renderer, "texture/lm3.bmp");
        TexLM[4] = LoadTexture(renderer, "texture/lm4.bmp");

        TexRM[0] = LoadTexture(renderer, "texture/rm0.bmp");
        TexRM[1] = LoadTexture(renderer, "texture/rm1.bmp");
        TexRM[2] = LoadTexture(renderer, "texture/rm2.bmp");
        TexRM[3] = LoadTexture(renderer, "texture/rm3.bmp");
        TexRM[4] = LoadTexture(renderer, "texture/rm4.bmp");

        TexE[0] = LoadTexture(renderer, "texture/e0.bmp");
        TexE[1] = LoadTexture(renderer, "texture/e1.bmp");
        TexE[2] = LoadTexture(renderer, "texture/e2.bmp");
        TexE[3] = LoadTexture(renderer, "texture/e3.bmp");
        TexE[4] = LoadTexture(renderer, "texture/e4.bmp");
        Loaded = true;
    }

    float spacing = windowWidth / (BunkerCount + 1);
    float bW = 3 * Size;

    for (int i = 0; i < BunkerCount; i++) {
        float startX = spacing * (i + 1) - (bW / 2.0f);
        float startY = 500.0f;


        bunkers[i].parts[0].rect = (SDL_FRect){startX, startY, Size, Size};
        bunkers[i].parts[0].textures = TexLC;
        bunkers[i].parts[0].active = true;
        bunkers[i].parts[0].damage = 0;

        bunkers[i].parts[1].rect = (SDL_FRect){startX + 32, startY, Size, Size};
        bunkers[i].parts[1].textures = TexFull;
        bunkers[i].parts[1].active = true;
        bunkers[i].parts[1].damage = 0;

        bunkers[i].parts[2].rect = (SDL_FRect){startX + 64, startY, Size, Size};
        bunkers[i].parts[2].textures = TexRC;
        bunkers[i].parts[2].active = true;
        bunkers[i].parts[2].damage = 0;

        bunkers[i].parts[3].rect = (SDL_FRect){startX, startY + 32, Size, Size};
        bunkers[i].parts[3].textures = TexLM;
        bunkers[i].parts[3].active = true;
        bunkers[i].parts[3].damage = 0;

        bunkers[i].parts[4].rect = (SDL_FRect){startX + 32, startY + 32, Size, Size};
        bunkers[i].parts[4].textures = TexE;
        bunkers[i].parts[4].active = true;
        bunkers[i].parts[4].damage = 0;

        bunkers[i].parts[5].rect = (SDL_FRect){startX + 64, startY + 32, Size, Size};
        bunkers[i].parts[5].textures = TexRM;
        bunkers[i].parts[5].active = true;
        bunkers[i].parts[5].damage = 0;
    }
}

bool BunkersCheckCollision(Bunker* bunker, SDL_FRect* bulletRect) {
    for (int i = 0; i < 6; i++) {
        if (bunker->parts[i].active) {
            SDL_FRect* r = &bunker->parts[i].rect;
            if (CheckCollision(*r, *bulletRect)) {
                
                bunker->parts[i].damage++;
                if (bunker->parts[i].damage >= 4) {
                    bunker->parts[i].active = false;
                }
                return true;
            }
        }
    }
    return false;
}

void BunkersRender(Bunker bunkers[], SDL_Renderer* renderer) {
    for (int i = 0; i < BunkerCount; i++) {
        for (int j = 0; j < 6; j++) {
            BunkerPart* p = &bunkers[i].parts[j];
            if (p->active && p->textures) {
                SDL_RenderTexture(renderer, p->textures[p->damage], NULL, &p->rect);
            }
        }
    }
}

void BunkersCleanup() {
    if (Loaded) {
        for(int i = 0; i < 5; i++) {
            SDL_DestroyTexture(TexLC[i]);
            SDL_DestroyTexture(TexFull[i]);
            SDL_DestroyTexture(TexRC[i]);
            SDL_DestroyTexture(TexE[i]);
            SDL_DestroyTexture(TexLM[i]);
            SDL_DestroyTexture(TexRM[i]);
        }
        Loaded = false;
    }
}