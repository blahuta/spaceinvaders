#include "ui.h"
#include "text.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static TTF_Font *Font = NULL;
static SDL_Texture *PlayerTexture = NULL;

static const int FontSize = 32;
static int Records[10] = {0};
const char *filepath = "leaderboard.txt";

static SDL_Color white = {255, 255, 255, 255};
static SDL_Color green = {0, 255, 0, 255};
static SDL_Color red = {255, 0, 0, 255};

static void LoadScores()
{
    FILE *file = fopen(filepath, "r");

    if (file)
    {
        for (int i = 0; i < 10; i++)
        {
            if (fscanf(file, "%d", &Records[i]) != 1)
            {
                Records[i] = 0;
            }
        }
        fclose(file);
    }
}

static void SaveScores()
{
    FILE *file = fopen(filepath, "w");
    if (file)
    {
        for (int i = 0; i < 10; i++)
        {
            fprintf(file, "%d\n", Records[i]);
        }
        fclose(file);
    }
}

void UIInit(SDL_Renderer *renderer)
{
    if (!TTF_Init())
    {
        SDL_Log("Chyba init TTF: %s", SDL_GetError());
        return;
    }
    Font = TTF_OpenFont("texture/space_invaders.ttf", FontSize);
    PlayerTexture = LoadTexture(renderer, "texture/play.bmp");
    if (!Font)
    {
        SDL_Log("Chyba nacitani fontu: %s", SDL_GetError());
    }
    LoadScores();
}

void UIAddScore(int score)
{
    if (score < Records[9])
        return;

    int idx = -1;
    for (int i = 0; i < 10; i++)
    {
        if (score > Records[i])
        {
            idx = i;
            break;
        }
    }

    for (int i = 10 - 1; i > idx; i--)
    {
        Records[i] = Records[i - 1];
    }

    Records[idx] = score;
    SaveScores();
}

void UIText(SDL_Renderer *renderer, const char *message, float x, float y, SDL_Color color)
{
    SDL_Surface *surface = TTF_RenderText_Blended(Font, message, 0, color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FRect dstRect = {x, y, (float)surface->w, (float)surface->h};
    SDL_RenderTexture(renderer, texture, NULL, &dstRect);

    SDL_DestroyTexture(texture);
    SDL_DestroySurface(surface);
}

void UIHud(SDL_Renderer *renderer, int lives, int score, float windowWidth, float windowHeight)
{
    char scoreText[32];
    snprintf(scoreText, sizeof(scoreText), "SCORE: %d", score);

    UIText(renderer, scoreText, 20.0f, 20.0f, white);

    SDL_FRect line = {0.0f, windowHeight - 20.0f, windowWidth, 10.0f};
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &line);
    for (int i = 0; i < lives; i++)
    {
        SDL_FRect destRect = {10.0f + i * 36.0f, windowHeight - 50.0f, 32.0f, 16.0f};
        SDL_RenderTexture(renderer, PlayerTexture, NULL, &destRect);
    }
}

void UIMenu(SDL_Renderer *renderer, float windowWidth, float windowHeight)
{
    UIText(renderer, "SPACE INVADERS", windowWidth / 2.5f, windowHeight / 3.0f, green);
    UIText(renderer, "PRESS ENTER TO START", windowWidth / 2.5f, windowHeight / 2.0f, white);
    UIText(renderer, "Leaderboard", windowWidth / 20.0f, windowHeight * 0.2f, red);

    char buf[64];
    for (int i = 0; i < 10; i++)
    {
        if (Records[i] > 0)
        {
            snprintf(buf, sizeof(buf), "%d.  %d", i + 1, Records[i]);
            UIText(renderer, buf, windowWidth / 10.0f, windowHeight * 0.4f + (i * 40.0f), white);
        }
    }
}

void UIGameOver(SDL_Renderer *renderer, float windowWidth, float windowHeight, int score)
{
    char scoreText[32];
    snprintf(scoreText, sizeof(scoreText), "FINAL SCORE: %d", score);

    UIText(renderer, "GAME OVER", windowWidth / 2.5f, windowHeight / 3.0f, red);

    UIText(renderer, scoreText, windowWidth / 2.5f, windowHeight / 2.0f, white);

    UIText(renderer, "PRESS ENTER TO RESTART", windowWidth / 2.5f, windowHeight * 0.66f, white);
}

void UICleanup()
{
    TTF_CloseFont(Font);
    SDL_DestroyTexture(PlayerTexture);
    TTF_Quit();
}