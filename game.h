#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>


#ifndef __GAME_H_
#define __GAME_H_

#define player_num 2

typedef struct {
    char* name;
    int position[2];
    int dir;
    int money;
    int ginger_soda;
    int numDecreaseSoda;
    int numIncreaseSoda;
    int numGambleRoulette;
    int numUnknownSoda;
    int effect_steps;
    int cannotMove;
    //int effect;
} Player;

typedef struct {
    SDL_Rect rect;
    bool has_focus;
    char text[256];
} InputBox;

typedef enum {
    SQUARE_normal,
    SQUARE_chance,
    SQUARE_game,
    SQUARE_store,
    SQUARE_ginger_soda,
    SQUARE_start
} Square_type;

typedef enum {
    GAME_Minesweeper,
    GAME_Dice_number,
    GAME_Timer_Challenge,
    GAME_Flip_Card,
    GAME_Coin_catcher
} game_square_type;

typedef enum {
    MAIN_MENU,
    GAME_SCREEN,
    GAME_OVER_SCREEN,
    BACKPACK_SCREEN
} Screen;

// 目前螢幕標籤
extern int currentScreen;

typedef enum {
    UP,
    LEFT,
    DOWN,
    RIGHT
} Movement;

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern const int INPUT_BOX_WIDTH;
extern const int INPUT_BOX_HEIGHT;

// 視窗
extern SDL_Window* window;
extern SDL_Window* newWindow;

// Render
extern SDL_Renderer* renderer;
extern SDL_Renderer* newRenderer;

// 圖片
// MAIN_MENU 
extern SDL_Texture* startButtonTexture;
extern SDL_Texture* quitButtonTexture;
extern SDL_Texture* titleTexture;
extern SDL_Texture *InputBoxTexture;
// GAME_SCREEN
extern SDL_Texture* bagpack_iconTexture;
extern SDL_Texture* homepageTexture;
extern SDL_Texture* square[6];
extern SDL_Texture* player[player_num];
extern SDL_Texture* diceTextures[6];
// BAGPACK_SCREEN
extern SDL_Texture* bagpackWindowTexture;
// GAME_END_SCREEN
extern SDL_Texture* restartTexture;
extern SDL_Texture* exitTexture;
extern SDL_Texture* bgTexture;

// 位置
// MAIN_MENU 
extern SDL_Rect startButtonRect;
extern SDL_Rect quitButtonRect;
extern SDL_Rect backButtonRect;
extern SDL_Rect titleRect;
// GAME_SCREEN
extern SDL_Rect bagpackRect;
extern SDL_Rect homepageRect;
extern int mapRect[32][2];
extern SDL_Rect diceRect;

extern SDL_Rect restartRect;
extern SDL_Rect exitRect;
extern SDL_Rect bgRect;

// 背景顏色
extern SDL_Color menuBackgrounColor;

// 字體
extern TTF_Font* font;
extern TTF_Font* font_player;
extern TTF_Font* font_tool;

// 字體顏色
extern SDL_Color textColor;

// 玩家設定
extern int currentPlayer;
extern Player player1;
extern Player player2;
//int dir[2];      // move direction

// 局數
extern int game_round;
// 地圖格數
extern const int numTiles;
// 輸入框
extern InputBox input_box1;
extern InputBox input_box2;
extern InputBox input_box3;
extern Uint32 CURSOR_BLINK_INTERVAL;  // 鼠標閃爍間隔時間（毫秒）
extern Uint32 cursor_last_time;
extern bool show_cursor;
// 骰子結果
extern char diceEvent[30];
// 道具解釋
extern const char* tool[4];

int GAME_init(int*);
void GAME_end();
void initialize_menu();
void initialize_texture();
void initialize_map(int*);
void initialize_player();
int mouse_is_above( int, int, SDL_Rect);
void render_map_and_player(int*);
int close();
int roll_dice();
void updatePlayerPosition(int, int, int*);
int getSquareTypeFromPosition(int, int);
void square_event(int*, int);
void ginger_soda();
void store();
void chance();
void renderMenu();
void renderGameScreen(int*, int);
void renderGameOverScreen();
void renderDiceAnimation(int, int*);
void draw_input_box(SDL_Renderer *renderer, TTF_Font *font, InputBox *input_box, bool show_cursor);
SDL_Texture* renderText(const char* message, SDL_Color color);

#endif