#ifndef __SHOP_H_
#define __SHOP_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "game.h"

// #define SCREEN_WIDTH 1280
// #define SCREEN_HEIGHT 720

typedef struct {
    int price;
    int imageIndex;
    SDL_Rect rect;
    SDL_Texture *texture;
    SDL_Texture *image;
} Product;


void market(Product [], SDL_Renderer *, TTF_Font *);

void renderTextWrapped(SDL_Renderer *, TTF_Font *, const char *, SDL_Color , SDL_Rect *, int );

void displayDialog(SDL_Renderer *, TTF_Font *, const char *, ...);

void itemDepiction(int );

void displayMoney(SDL_Renderer *, TTF_Font *);

// void cleanup(SDL_Window **, SDL_Renderer **, SDL_Surface **, SDL_Texture **, Product *);

int shop_main();

#endif