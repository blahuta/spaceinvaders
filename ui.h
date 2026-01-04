#pragma once
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

void UIInit(SDL_Renderer *renderer);
void UIAddScore(int score);
void UIText(SDL_Renderer *renderer, const char *message, float x, float y, SDL_Color color);
void UIHud(SDL_Renderer *renderer, int lives, int score, float windowWidth, float windowHeight);
void UIMenu(SDL_Renderer *renderer, float windowWidth, float windowHeight);
void UIGameOver(SDL_Renderer *renderer, float windowWidth, float windowHeight, int score);
void UICleanup();