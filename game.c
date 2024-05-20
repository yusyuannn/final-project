#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//#include <SDL2_ttf.h>
#include <SDL2/SDL.h>
#include "game.h"

// gcc -Isrc/Include -Lsrc/lib -o game game.c -lmingw32 -lSDL2main -lSDL2

int main( int argc, char *argv[] ){
    const int numImages = 32; 
    int MAP[32]; 

    if(!GAME_init(MAP, numImages)){
        printf("game initialization failed");
    }

    GAME_end();

    return 0;
}

int GAME_init(int* MAP, int numImages){
    initialize_menu();
    initialize_texture();
    initialize_map(MAP, numImages);
    initialize_player();
    render_map_and_player(MAP);

    return 1;
}

void GAME_end(){
    if(!close()){
        printf("game close failed\n");
    }
}

// 初始化menu
void initialize_menu(){ 
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Ginger Soda", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(renderer, menuBackgrounColor.r, menuBackgrounColor.g, menuBackgrounColor.b, menuBackgrounColor.a);
}

// 載入圖片為texture
void initialize_texture(){  
    // MAIN_MENU    
    SDL_Surface* startButtonSurface = SDL_LoadBMP("images/start_button.bmp");
    SDL_Surface* quitButtonSurface = SDL_LoadBMP("images/quit_button.bmp");
    SDL_Surface* titleSurface = SDL_LoadBMP("images/title.bmp");
    // GAME_SCREEN   
    SDL_Surface* bagpackSurface = SDL_LoadBMP("images/bagpack.bmp");
    SDL_Surface* homepageSurface = SDL_LoadBMP("images/homepage.bmp");
    SDL_Surface* gameTileSurface = SDL_LoadBMP("images/gameTile.bmp");
    SDL_Surface* chanceTileSurface = SDL_LoadBMP("images/chanceTile.bmp");
    SDL_Surface* normalTileSurface = SDL_LoadBMP("images/normalTile.bmp");
    SDL_Surface* sodaTileSurface = SDL_LoadBMP("images/sodaTile.bmp");
    SDL_Surface* startTileSurface = SDL_LoadBMP("images/startTile.bmp");
    SDL_Surface* storeTileSurface = SDL_LoadBMP("images/storeTile.bmp");
    SDL_Surface* player1Surface = SDL_LoadBMP("images/black_circle.bmp");
    SDL_Surface* player2Surface = SDL_LoadBMP("images/white_circle.bmp");
    // BAGPACK_SCREEN
    SDL_Surface* player1TitleSurface = SDL_LoadBMP("images/Player1.bmp");
    SDL_Surface* player2TitleSurface = SDL_LoadBMP("images/Player2.bmp");
    SDL_Surface* toolIllustreSurface = SDL_LoadBMP("images/toolIilustration.bmp");
    SDL_Surface* returnButtonSurface = SDL_LoadBMP("images/ReturnMap.bmp");
    SDL_Surface* moneySurface = SDL_LoadBMP("images/money.bmp");
    SDL_Surface* moneyPrintSurface = SDL_LoadBMP("images/print.bmp");
    SDL_Surface* gindersodaSurface = SDL_LoadBMP("images/money.bmp");
    SDL_Surface* gingersodaPrintSurface = SDL_LoadBMP("images/print.bmp");

    // MAIN_MENU
    startButtonTexture = SDL_CreateTextureFromSurface(renderer, startButtonSurface);
    quitButtonTexture = SDL_CreateTextureFromSurface(renderer, quitButtonSurface);
    titleTexture = SDL_CreateTextureFromSurface(renderer, titleSurface);
    // GAME_SCREEN 
    bagpackTexture = SDL_CreateTextureFromSurface(renderer, bagpackSurface);
    homepageTexture = SDL_CreateTextureFromSurface(renderer, homepageSurface);
    square[SQUARE_normal] = SDL_CreateTextureFromSurface(renderer, normalTileSurface);
    square[SQUARE_chance] = SDL_CreateTextureFromSurface(renderer, chanceTileSurface);
    square[SQUARE_game] = SDL_CreateTextureFromSurface(renderer, gameTileSurface);
    square[SQUARE_store] = SDL_CreateTextureFromSurface(renderer, storeTileSurface);
    square[SQUARE_ginger_soda] = SDL_CreateTextureFromSurface(renderer, sodaTileSurface);
    square[SQUARE_start] = SDL_CreateTextureFromSurface(renderer, startTileSurface);
    player[0] = SDL_CreateTextureFromSurface(renderer, player1Surface);
    player[1] = SDL_CreateTextureFromSurface(renderer, player2Surface);
    // BAGPACK_SCREEN
    player1TitleTexture = SDL_CreateTextureFromSurface(renderer, player1TitleSurface);
    player2TitleTexture = SDL_CreateTextureFromSurface(renderer, player2TitleSurface);
    toolIllustreTexture = SDL_CreateTextureFromSurface(renderer, toolIllustreSurface);
    returnButtonTexture = SDL_CreateTextureFromSurface(renderer, returnButtonSurface);
    moneyTexture = SDL_CreateTextureFromSurface(renderer, moneySurface);
    moneyPrintTexture = SDL_CreateTextureFromSurface(renderer, moneyPrintSurface);
    gindersodaTexture = SDL_CreateTextureFromSurface(renderer, gindersodaSurface);
    gingersodaPrintTexture = SDL_CreateTextureFromSurface(renderer, gingersodaPrintSurface);
    
    // MAIN_MENU  
    SDL_FreeSurface(startButtonSurface);
    SDL_FreeSurface(quitButtonSurface);
    SDL_FreeSurface(titleSurface);
    // GAME_SCREEN 
    SDL_FreeSurface(bagpackSurface);
    SDL_FreeSurface(gameTileSurface);
    SDL_FreeSurface(chanceTileSurface);
    SDL_FreeSurface(normalTileSurface);
    SDL_FreeSurface(sodaTileSurface);
    SDL_FreeSurface(startTileSurface);
    SDL_FreeSurface(storeTileSurface);
    SDL_FreeSurface(player1Surface);
    SDL_FreeSurface(player2Surface);
    // BAGPACK_SCREEN
    SDL_FreeSurface(player1TitleSurface);
    SDL_FreeSurface(player2TitleSurface);
    SDL_FreeSurface(toolIllustreSurface);
    SDL_FreeSurface(returnButtonSurface);
    SDL_FreeSurface(moneySurface);
    SDL_FreeSurface(moneyPrintSurface);
    SDL_FreeSurface(gindersodaSurface);
    SDL_FreeSurface(gingersodaPrintSurface);
    
    // DICE
    for (int i = 0; i < 6; ++i) {
        char filename[20];
        snprintf(filename, sizeof(filename), "images/dice_%d.bmp", i + 1);
        SDL_Surface* surface = SDL_LoadBMP(filename);
        diceTextures[i] = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }
}

// 初始化地圖
void initialize_map(int* imageOrder, int numImages){
    /* 隨機交換imageOrder中的元素 */
    srand(time(NULL));
    int imageCounts[] = {9, 12, 6, 0, 1, 0}; // square各自的數量
    imageOrder[0] = SQUARE_store;
    imageOrder[1] = SQUARE_store;
    imageOrder[2] = SQUARE_store;
    imageOrder[3] = SQUARE_start;
    int index = 4;
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < imageCounts[i]; ++j) {
            imageOrder[index++] = i;
        }
    }
    for (int i = numImages - 1; i >= 4; --i) {
        int j = 4 + rand() % (32 - 4);      // random number btw 4 and 28
        int temp = imageOrder[i];
        imageOrder[i] = imageOrder[j];
        imageOrder[j] = temp;
    }
}

// 初始化玩家
void initialize_player(){
    player1.name = "Player1";
    player2.name = "Player2";
    player1.position[0] = 1210;
    player1.position[1] = 665;
    player2.position[0] = 1240;
    player2.position[1] = 665;
    player1.money = 100;
    player2.money = 100;
    player1.ginger_soda = 0;
    player2.ginger_soda = 0;
    currentPlayer = 0;
}

// 檢查滑鼠點擊位置
int mouse_is_above( int mouse_X, int mouse_Y, SDL_Rect rect ){
    if( mouse_X >= rect.x && mouse_X < rect.x + rect.w &&
        mouse_Y >= rect.y && mouse_Y < rect.y + rect.h ){
            return 1;
    }

    return 0;
}

// 顯示畫面
void render_map_and_player(int* MAP){
    SDL_Event event;
    int running = 1;
    float game_round = 20; // 行動回數
    int currentScreen = MAIN_MENU;
    int steps = 1;

    while (running && game_round) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {       // close window by the "X"
                running = 0;
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {     // pressed mouse
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);
                if (currentScreen == MAIN_MENU) {
                    if (mouse_is_above(mouseX, mouseY, startButtonRect) ) {
                        currentScreen = GAME_SCREEN;
                    } else if (mouse_is_above(mouseX, mouseY, quitButtonRect)) {
                        running = 0;
                    }
                } else if (currentScreen == GAME_SCREEN) {
                    if (mouse_is_above(mouseX, mouseY, homepageRect) && event.button.button == SDL_BUTTON_LEFT) {
                        currentScreen = MAIN_MENU;
                    } else if (mouse_is_above(mouseX, mouseY, bagpackRect) && event.button.button == SDL_BUTTON_LEFT) {
                        currentScreen = BAGPACK_SCREEN;
                    } else if (mouse_is_above(mouseX, mouseY, diceRect) && event.button.button == SDL_BUTTON_LEFT) {
                        steps = roll_dice();
                        renderDiceAnimation(steps, MAP);                   
                        updatePlayerPosition(steps, currentPlayer, MAP);   
                        square_event(MAP, currentPlayer);                  
                        currentPlayer = (currentPlayer + 1) % player_num;  
                    }
                } else if (currentScreen == BAGPACK_SCREEN) {
                    if (mouse_is_above(mouseX, mouseY, returnButtonRect) && event.button.button == SDL_BUTTON_LEFT) {
                        currentScreen = GAME_SCREEN;
                    }
                }
            } else if (event.type == SDL_WINDOWEVENT && newWindow != NULL) {  // 處理新視窗的事件
                if (event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(newWindow)) {  // 關閉新視窗
                    SDL_DestroyRenderer(newRenderer);
                    SDL_DestroyWindow(newWindow);
                    newRenderer = NULL;
                    newWindow = NULL;
                }
            }
        }
        SDL_RenderClear(renderer);
        if (currentScreen == MAIN_MENU) {
            renderMenu();
        } else if (currentScreen == GAME_SCREEN) {
            renderGameScreen(MAP, steps);
        } else if (currentScreen == BAGPACK_SCREEN) {
            // 顯示背包
            SDL_RenderCopy(renderer, player1TitleTexture, NULL, &playerTitleRect);
            SDL_RenderCopy(renderer, toolIllustreTexture, NULL, &toolIllustreRect);
            SDL_RenderCopy(renderer, returnButtonTexture, NULL, &returnButtonRect);

            SDL_RenderCopy(renderer, moneyTexture, NULL, &moneyRect);
            SDL_RenderCopy(renderer, moneyPrintTexture, NULL, &moneyPrintRect);
            SDL_RenderCopy(renderer, gindersodaTexture, NULL, &gindersodaRect);
            SDL_RenderCopy(renderer, gingersodaPrintTexture, NULL, &gingersodaPrintRect);
            }
        SDL_RenderPresent(renderer);
    }
}

void renderMenu() {
    SDL_RenderCopy(renderer, startButtonTexture, NULL, &startButtonRect);
    SDL_RenderCopy(renderer, quitButtonTexture, NULL, &quitButtonRect);
    SDL_RenderCopy(renderer, titleTexture, NULL, &titleRect);
}

void renderGameScreen(int* MAP, int steps) {
    // 地圖
    for (int i = 0; i < 32; i++) {
        SDL_Rect rect = {mapRect[i][0], mapRect[i][1], 80, 80};
        int randomIndex = MAP[i];
        SDL_RenderCopy(renderer, square[randomIndex], NULL, &rect);
    }
    // 玩家
    SDL_Rect rect1 = {player1.position[0], player1.position[1], 30, 30};
    SDL_RenderCopy(renderer, player[0], NULL, &rect1);
    SDL_Rect rect2 = {player2.position[0], player2.position[1], 30, 30};
    SDL_RenderCopy(renderer, player[1], NULL, &rect2);
    // 骰子
    SDL_RenderCopy(renderer, diceTextures[steps - 1], NULL, &diceRect);
    // 返回主頁
    SDL_RenderCopy(renderer, homepageTexture, NULL, &homepageRect);
    // 背包
    SDL_RenderCopy(renderer, bagpackTexture, NULL, &bagpackRect);
}

void renderDiceAnimation(int finalRoll, int* MAP) {
    Uint32 startTime = SDL_GetTicks();
    Uint32 animation_duration = 500;      // 0.5 second
    Uint32 animation_frame_duration = 50; // 50 ms per frame
    int frame = 0;
    while (SDL_GetTicks() - startTime < animation_duration) {
        frame = (frame + 1) % 6;
        SDL_RenderClear(renderer);
        renderGameScreen(MAP, finalRoll);
        SDL_RenderCopy(renderer, diceTextures[frame], NULL, &diceRect);
        SDL_RenderPresent(renderer);
        SDL_Delay(animation_frame_duration);
    }
}

// 擲骰子
int roll_dice(){
    return rand() % 6 + 1;
}

// 根據骰子點數移動玩家位置
void updatePlayerPosition(int steps, int currentPlayer, int* MAP) {
    int finalRoll = steps;
    if (currentPlayer == 0) {
        while (steps){
            if ((player1.position[0]==1210 && player1.position[1]==665) || 
                (player1.position[0]==1210 && player1.position[1]==25) ||
                (player1.position[0]==570 && player1.position[1]==25) ||
                (player1.position[0]==570 && player1.position[1]==665) ) {
                dir[0] = (dir[0] + 1) % 4;
            } 
            if (dir[0] == UP) {
                player1.position[1] -= 80;
            } else if (dir[0] == LEFT) {
                player1.position[0] -= 80;
            } else if (dir[0] == DOWN) {
                player1.position[1] += 80;
            } else if (dir[0] == RIGHT) {
                player1.position[0] += 80;
            }
            // 顯示當前移動情況
            SDL_RenderClear(renderer);
            renderGameScreen(MAP, finalRoll);
            SDL_RenderPresent(renderer);
            SDL_Delay(500);
            steps--;
        }
    } else {
        while (steps){
            if ((player2.position[0]==1240 && player2.position[1]==665) || 
                (player2.position[0]==1240 && player2.position[1]==25) ||
                (player2.position[0]==600 && player2.position[1]==25) ||
                (player2.position[0]==600 && player2.position[1]==665) ) {
                dir[1] = (dir[1] + 1) % 4;
            }
            if (dir[1] == UP) {
                player2.position[1] -= 80;
            } else if (dir[1] == LEFT) {
                player2.position[0] -= 80;
            } else if (dir[1] == DOWN) {
                    player2.position[1] += 80;
            } else if (dir[1] == RIGHT) {
                player2.position[0] += 80;
            }
            // 顯示當前移動情況
            SDL_RenderClear(renderer);
            renderGameScreen(MAP, finalRoll);
            SDL_RenderPresent(renderer);
            SDL_Delay(500);
            steps--;
        }
    }
}

// 根據玩家位置行動
void square_event(int* MAP, int currentPlayer){
    int squareType;
    int index;
    if (currentPlayer == 0) {
        index = getSquareTypeFromPosition(player1.position[0], player1.position[1]);
        squareType = MAP[index];
    }else {
        index = getSquareTypeFromPosition(player2.position[0], player2.position[1]);
        squareType = MAP[index];
    }

    //int the_game = rand() % 6;;
    switch (squareType) {
        case SQUARE_normal:
            // 玩家拿錢
            if (currentPlayer == 0) {
                player1.money += 100;    // temperarily set to 100 for convenience
                printf("player1 now has %d\n", player1.money);
            }else {
                player2.money += 100;
                printf("player2 now has %d\n", player2.money);
            }
            break;
        case SQUARE_chance:
            // 進機會格的event
            printf("event\n");
            chance();
            break;
        case SQUARE_game:
            // 用rand跑出0~5看要進什麼遊戲
            /* switch (the_game){
                case GAME_Minesweeper:
                    //minigame_minesweeper();
                    break;
                case GAME_Dice_number:
                    //minigame_Dice_number();
                    break;
                case GAME_Timer_Challenge:
                    //minigame_Timer_Challenge();
                    break;
                case GAME_Flip_Card:
                    //minigame_Flip_Card();
                    break;    
                case GAME_Coin_catcher:
                    //minigame_Coin_catcher();
                    break;
                default:
                    break;
            }
            */
            printf("game\n");
            break;
        case SQUARE_store:
            // 進商店
            printf("store\n");
            store();
            break;
        case SQUARE_ginger_soda:
            // 進薑汁汽水販賣機
            printf("sell ginger soda\n");
            ginger_soda();
            // 汽水格和地圖上隨機一格交換位置
            int randomSquare = 4 + rand() % (32 - 4);      // random number btw 4 and 28
            int temp = MAP[randomSquare];
            MAP[randomSquare] = MAP[index];
            MAP[index] = temp;
            break;
        default:
            break;
    }
}

// 從格子位置得知格子類型
Square_type getSquareTypeFromPosition(int x, int y) {
    for (int i = 0; i < 32; ++i) {
        int squareX = mapRect[i][0];
        int squareY = mapRect[i][1];
        if (x >= squareX && x < squareX + 80 && y >= squareY && y < squareY + 80) {
            return i;
        }
    }
    return -1;
}

void chance() {
    // 檢查上一次開啟的新視窗是否有關閉
    if (newWindow != NULL) {
        SDL_DestroyRenderer(newRenderer);
        SDL_DestroyWindow(newWindow);
        newRenderer = NULL;
        newWindow = NULL;
    }
    //SDL_Delay(1000);  // 延遲1秒
    newWindow = SDL_CreateWindow("Chance", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    newRenderer = SDL_CreateRenderer(newWindow, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(newRenderer, 255, 255, 255, 255);
    SDL_RenderClear(newRenderer); 
    SDL_RenderPresent(newRenderer);
}

void store() {
    // 檢查上一次開啟的新視窗是否有關閉
    if (newWindow != NULL) {
        SDL_DestroyRenderer(newRenderer);
        SDL_DestroyWindow(newWindow);
        newRenderer = NULL;
        newWindow = NULL;
    }
    //SDL_Delay(1000);  // 延遲1秒
    newWindow = SDL_CreateWindow("Store", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    newRenderer = SDL_CreateRenderer(newWindow, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(newRenderer, 255, 255, 255, 255);
    SDL_RenderClear(newRenderer); 
    SDL_RenderPresent(newRenderer);
}

void ginger_soda() {
    // 檢查上一次開啟的新視窗是否有關閉
    if (newWindow != NULL) {
        SDL_DestroyRenderer(newRenderer);
        SDL_DestroyWindow(newWindow);
        newRenderer = NULL;
        newWindow = NULL;
    }
    //SDL_Delay(1000);  // 延遲1秒
    newWindow = SDL_CreateWindow("Ginger Soda Store", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    newRenderer = SDL_CreateRenderer(newWindow, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(newRenderer, 255, 255, 255, 255);
    SDL_RenderClear(newRenderer); 
    SDL_RenderPresent(newRenderer);
}

// 釋放資源
int close() {
    SDL_DestroyTexture(startButtonTexture);
    SDL_DestroyTexture(quitButtonTexture);
    SDL_DestroyTexture(titleTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    for (int i = 0; i < 6; ++i) {
        SDL_DestroyTexture(square[i]);
    } 
    SDL_Quit();
    return 1;
}
