#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

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
    //int effect;
} Player;

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
    BAGPACK_SCREEN
} Screen;

typedef enum {
    UP,
    LEFT,
    DOWN,
    RIGHT
} Movement;

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

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
// GAME_SCREEN
SDL_Texture* bagpackTexture;
SDL_Texture* homepageTexture;
SDL_Texture* square[6];
SDL_Texture* player[player_num];
SDL_Texture* diceTextures[6];
// BAGPACK_SCREEN
SDL_Texture* player1TitleTexture;
SDL_Texture* player2TitleTexture;
SDL_Texture* toolTileTexture; 
SDL_Texture* illustrationDecreaseSodaTexture;
SDL_Texture* illustrationIncreaseSodaTexture;
SDL_Texture* illustrationGamblingRouletteTexture;
SDL_Texture* illustrationUnknownSodaTexture;
SDL_Texture* decreasingSodaTexture;
SDL_Texture* increasingSodaTexture;
SDL_Texture* gambleRouletteTexture;
SDL_Texture* unknownSodaTexture;
SDL_Texture* returnButtonTexture;
SDL_Texture* moneyTexture;
SDL_Texture* moneyPrintTexture;    
SDL_Texture* ginderSodaTexture;
SDL_Texture* gingerSodaPrintTexture;
// GAME_END_SCREEN
SDL_Texture* restartTexture;
SDL_Texture* exitTexture;

// 位置
// MAIN_MENU 
SDL_Rect startButtonRect = { 760, 200, 335, 135 };
SDL_Rect quitButtonRect = { 780, 400, 284, 120 };
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
// BAGPACK_SCREEN
SDL_Rect playerTitleRect = {250, 100, 200, 50};
SDL_Rect toolIllustrationRect = {100, 450, 500, 200};
SDL_Rect returnButtonRect = {700, 100, 350, 50};
SDL_Rect moneyRect = {700, 200, 150, 150};
SDL_Rect moneyPrintRect = {950, 250, 100, 100};
SDL_Rect ginderSodaRect = {700, 450, 150, 150};
SDL_Rect gingerSodaPrintRect = {950, 500, 100, 100};
int toolTileRect[10][2] = {
    {100, 200}, {200, 200}, {300, 200}, {400, 200}, {500, 200},
    {100, 300}, {200, 300}, {300, 300}, {400, 300}, {500, 300}
};
int toolRect[10][2] = {
    {100 + 5, 200 + 5}, {200 + 5, 200 + 5}, {300 + 5, 200 + 5}, {400 + 5, 200 + 5}, {500 + 5, 200 + 5},
    {100 + 5, 300 + 5}, {200 + 5, 300 + 5}, {300 + 5, 300 + 5}, {400 + 5, 300 + 5}, {500 + 5, 300 + 5}
};
// GAME_END_SCREEN
SDL_Rect restartRect = {465, 335, 350, 50};
SDL_Rect exitRect = {465, 395, 350, 50};

// 背景顏色
SDL_Color menuBackgrounColor = {255, 248, 220 ,0xFF};

// 字體
TTF_Font* font;

// 字體顏色
SDL_Color textColor = {0,0,0,0};

// 玩家設定
int currentPlayer;
Player player1;
Player player2;
//int dir[2];      // move direction

// 局數
int game_round = 3;

const int numTiles = 32;

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
Square_type getSquareTypeFromPosition(int, int);
void square_event(int*, int);
void ginger_soda();
void store();
void chance();
void renderMenu();
void renderGameScreen(int*, int);
void renderGameOverScreen();
void renderBagpackScreen();
void renderDiceAnimation(int, int*);
SDL_Texture* renderText(const char* message, SDL_Color color);
