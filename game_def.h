#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "game.h"

#ifndef __GAME_DEF_H_
#define __GAME_DEF_H_

#define player_num 2

// 目前螢幕標籤
int currentScreen;

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const int INPUT_BOX_WIDTH =  400;
const int INPUT_BOX_HEIGHT = 50;

// 視窗
SDL_Window* window;
SDL_Window* newWindow;

// Render
SDL_Renderer* renderer;
SDL_Renderer* newRenderer;

// 圖片
// MAIN_MENU 
SDL_Texture* startButtonTexture;
SDL_Texture* quitButtonTexture;
SDL_Texture* titleTexture;
SDL_Texture *InputBoxTexture;
// GAME_SCREEN
SDL_Texture* bagpack_iconTexture;
SDL_Texture* homepageTexture;
SDL_Texture* square[6];
SDL_Texture* player[player_num];
SDL_Texture* diceTextures[6];
// BAGPACK_SCREEN
SDL_Texture* bagpackWindowTexture;
// GAME_END_SCREEN
SDL_Texture* restartTexture;
SDL_Texture* exitTexture;
SDL_Texture* bgTexture;

// 位置
// MAIN_MENU 
SDL_Rect startButtonRect = { 760, 150, 233, 57 };
SDL_Rect quitButtonRect = { 760, 580, 182, 56 };
SDL_Rect backButtonRect = { 10, 5, 40, 40 };
SDL_Rect titleRect = { 120, 120, 551,  444};
// GAME_SCREEN
SDL_Rect bagpackRect = {160, 30, 50, 50};
SDL_Rect homepageRect = {100, 30, 50, 50};
int mapRect[32][2] = {
        {560,0}, {1200,0}, {560,640}, {1200,640},
        {640, 0}, {720, 0}, {800, 0}, {880, 0}, {960, 0}, {1040, 0}, {1120, 0},                  // up
        {640, 640}, {720, 640}, {800, 640}, {880, 640}, {960, 640}, {1040, 640}, {1120, 640},    // down
        {560, 80}, {560, 160}, {560, 240}, {560, 320}, {560, 400}, {560, 480}, {560, 560},       // left
        {1200, 80}, {1200, 160}, {1200, 240}, {1200, 320}, {1200, 400}, {1200, 480}, {1200, 560} // right
};
SDL_Rect diceRect = {880, 320, 80, 80};

SDL_Rect restartRect = {360, 420, 200, 50};
SDL_Rect exitRect = {810, 420, 100, 50};
SDL_Rect bgRect = {300, 150, 660, 360};

// 背景顏色
SDL_Color menuBackgrounColor = {253, 245, 230 ,0xFF};

// 字體
TTF_Font* font;
TTF_Font* font_player;
TTF_Font* font_tool;

// 字體顏色
SDL_Color textColor = {0,0,0,0};

// 玩家設定
int currentPlayer;
Player player1;
Player player2;
//int dir[2];      // move direction

// 局數
int game_round = 1;
// 地圖格數
const int numTiles = 32;
// 輸入框
InputBox input_box1;
InputBox input_box2;
InputBox input_box3;
Uint32 CURSOR_BLINK_INTERVAL = 500;  // 鼠標閃爍間隔時間（毫秒）
Uint32 cursor_last_time;
bool show_cursor = true;
// 骰子結果
char diceEvent[30];
// 道具解釋
const char* tool[4] = {
    "Decreasing Soda: ",
    "Increasing Soda: ",
    "Gamble Roulette: ",
    "Unknown Soda: ",
};

#endif