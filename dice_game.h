#ifndef DICE_GAME_H
#define DICE_GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 1000
#define DICE_SCREEN_WIDTH 1280
#define DICE_SCREEN_HEIGHT 720
#define DICE_SIZE 150

void dicerenderTextCentered(SDL_Renderer* renderer, TTF_Font* font, const char* text, SDL_Color textColor, SDL_Rect rect);
void renderText(SDL_Renderer* renderer, TTF_Font* font, const char* text, int x, int y, SDL_Color color);
SDL_Texture* loadBackground(SDL_Renderer* renderer, int num);
SDL_Texture* loadDiceTexture(SDL_Renderer* renderer, int value);
void renderDiceAnimation(SDL_Renderer* renderer, SDL_Texture* diceTextures[], SDL_Rect dice1Rect, SDL_Rect dice2Rect, int* currentFrame1, int* currentFrame2);

void run_dice_game();

#endif
