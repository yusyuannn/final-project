#ifndef __BACKPACK_H_
#define __BACKPACK_H_

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

typedef enum {
    NonEffect, // 0

    decreaseEffect, // 1 oppo
    increaseEffect, // 2 me
    gambleEffect, // 3 oppo
    // unkown Soda
    increaseOwnMoneyEffect, // 4 me
    increaseOppoMoneyEffect, // 5 oppo
} toolType;

// window
extern SDL_Window* backpackWindow;

// Render
extern SDL_Renderer* backpackRenderer;

// texture
extern SDL_Texture* backpackWindowTexture;
extern SDL_Texture* useButtonTexture;

// define place
extern SDL_Rect playerTitleRect;
extern SDL_Rect toolIllustrationRect;
extern SDL_Rect returnButtonRect;
extern SDL_Rect toolRect;
extern SDL_Rect toolTitleRect;
extern SDL_Rect tool_quanityRect;
extern SDL_Rect toolIconRects[4];
extern SDL_Rect Text_numToolRect[4];
extern SDL_Rect moneyPrintRect;
extern SDL_Rect gingerSodaPrintRect;
extern SDL_Rect backpackWindowRect;
extern SDL_Rect useButtonRect;

// Illustration for tools
extern char toolDescriptions[4][1000];

int backpack_main(int);

void renderTextCentered(SDL_Renderer*, TTF_Font* , const char*, SDL_Color, SDL_Rect);
void renderTextWrapped(SDL_Renderer*, TTF_Font*, const char*, SDL_Color, SDL_Rect*);
void initialize_backpack();
void backpack_mouseEvent(int);
void backpack_initialize_texture();
// int mouse_is_above( int, int, SDL_Rect);
int close_backpack();
int unknownSoda_effect();
void toolEffect(int, int);
void renderBackpackScreen();
void renderItemDescription(int);

#endif