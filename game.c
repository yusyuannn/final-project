#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>
#include "game.h"

// gcc -Isrc/Include -Lsrc/lib -o game game.c -lmingw32 -lSDL2main -lSDL2

int main( int argc, char *argv[] ){ 
    int MAP[32];

    if(!GAME_init(MAP)){
        printf("game initialization failed");
    }
    
    render_map_and_player(MAP);

    GAME_end();

    return 0;
}

int GAME_init(int* MAP){
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    font = TTF_OpenFont("fonts/Cubic_11_1.100_R.ttf", 24);  // 可替換字體
    
    initialize_menu();
    initialize_texture();
    initialize_map(MAP);
    initialize_player();

    return 1;
}

// 初始化menu
void initialize_menu(){ 
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
    SDL_Surface* gameTileSurface = SDL_LoadBMP("images/tile_game.bmp");
    SDL_Surface* chanceTileSurface = SDL_LoadBMP("images/tile_chance.bmp");
    SDL_Surface* normalTileSurface = SDL_LoadBMP("images/tile_normal.bmp");
    SDL_Surface* sodaTileSurface = SDL_LoadBMP("images/tile_soda.bmp");
    SDL_Surface* startTileSurface = SDL_LoadBMP("images/tile_start.bmp");
    SDL_Surface* storeTileSurface = SDL_LoadBMP("images/tile_store.bmp");
    SDL_Surface* player1Surface = SDL_LoadBMP("images/circle_black.bmp");
    SDL_Surface* player2Surface = SDL_LoadBMP("images/circle_white.bmp");
    // BAGPACK_SCREEN
    SDL_Surface* player1TitleSurface = SDL_LoadBMP("images/packbagImage/Player1.bmp");
    SDL_Surface* player2TitleSurface = SDL_LoadBMP("images/packbagImage/Player2.bmp");
    SDL_Surface* toolTileSurface = SDL_LoadBMP("images/packbagImage/toolTile.bmp");
    SDL_Surface* illustrationDecreaseSodaSurface = SDL_LoadBMP("images/packbagImage/illustrationDecreaseSoda.bmp");
    SDL_Surface* illustrationIncreaseSodaSurface = SDL_LoadBMP("images/packbagImage/illustrationIncreaseSoda.bmp");
    SDL_Surface* illustrationGamblingRouletteSurface = SDL_LoadBMP("images/packbagImage/illustrationGamblingRoulette.bmp");
    SDL_Surface* illustrationUnknownSodaSurface = SDL_LoadBMP("images/packbagImage/illustrationUnknownSoda.bmp"); 
    SDL_Surface* decreasingSodaSurface = SDL_LoadBMP("images/packbagImage/decreasingSoda.bmp");
    SDL_Surface* increasingSodaSurface = SDL_LoadBMP("images/packbagImage/increasingSoda.bmp");
    SDL_Surface* gambleRouletteSurface = SDL_LoadBMP("images/packbagImage/gambleRoulette.bmp");
    SDL_Surface* unknownSodaSurface = SDL_LoadBMP("images/packbagImage/unknownSoda.bmp");
    SDL_Surface* returnButtonSurface = SDL_LoadBMP("images/packbagImage/ReturnMap.bmp");
    SDL_Surface* moneySurface = SDL_LoadBMP("images/packbagImage/money.bmp");
    SDL_Surface* moneyPrintSurface = SDL_LoadBMP("images/packbagImage/printMoney.bmp");
    SDL_Surface* ginderSodaSurface = SDL_LoadBMP("images/packbagImage/gingerSoda.bmp");
    SDL_Surface* gingerSodaPrintSurface = SDL_LoadBMP("images/packbagImage/printGingerSoda.bmp");
    
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
    toolTileTexture = SDL_CreateTextureFromSurface(renderer, toolTileSurface);  
    illustrationDecreaseSodaTexture = SDL_CreateTextureFromSurface(renderer, illustrationDecreaseSodaSurface);
    illustrationIncreaseSodaTexture = SDL_CreateTextureFromSurface(renderer, illustrationIncreaseSodaSurface);
    illustrationGamblingRouletteTexture = SDL_CreateTextureFromSurface(renderer, illustrationGamblingRouletteSurface);
    illustrationUnknownSodaTexture = SDL_CreateTextureFromSurface(renderer, illustrationUnknownSodaSurface);   
    decreasingSodaTexture = SDL_CreateTextureFromSurface(renderer, decreasingSodaSurface);
    increasingSodaTexture = SDL_CreateTextureFromSurface(renderer, increasingSodaSurface);
    gambleRouletteTexture = SDL_CreateTextureFromSurface(renderer, gambleRouletteSurface);
    unknownSodaTexture = SDL_CreateTextureFromSurface(renderer, unknownSodaSurface);    
    returnButtonTexture = SDL_CreateTextureFromSurface(renderer, returnButtonSurface); 
    moneyTexture = SDL_CreateTextureFromSurface(renderer, moneySurface);    
    moneyPrintTexture = SDL_CreateTextureFromSurface(renderer, moneyPrintSurface);    
    ginderSodaTexture = SDL_CreateTextureFromSurface(renderer, ginderSodaSurface);
    gingerSodaPrintTexture = SDL_CreateTextureFromSurface(renderer, gingerSodaPrintSurface);
    
    // MAIN_MENU  
    SDL_FreeSurface(startButtonSurface);
    SDL_FreeSurface(quitButtonSurface);
    SDL_FreeSurface(titleSurface);
    // GAME_SCREEN 
    SDL_FreeSurface(bagpackSurface);
    SDL_FreeSurface(homepageSurface);
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
    SDL_FreeSurface(toolTileSurface);    
    SDL_FreeSurface(illustrationDecreaseSodaSurface);
    SDL_FreeSurface(illustrationIncreaseSodaSurface);
    SDL_FreeSurface(illustrationGamblingRouletteSurface);
    SDL_FreeSurface(illustrationUnknownSodaSurface);    
    SDL_FreeSurface(decreasingSodaSurface);
    SDL_FreeSurface(increasingSodaSurface);
    SDL_FreeSurface(gambleRouletteSurface);
    SDL_FreeSurface(unknownSodaSurface);
    SDL_FreeSurface(returnButtonSurface);
    SDL_FreeSurface(moneySurface);
    SDL_FreeSurface(moneyPrintSurface);
    SDL_FreeSurface(ginderSodaSurface);
    SDL_FreeSurface(gingerSodaPrintSurface);
    
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
void initialize_map(int* imageOrder){
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
    // 隨機交換imageOrder中的元素
    for (int i = numTiles - 1; i >= 4; --i) {
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
    player1.numDecreaseSoda = 0;
    player2.numDecreaseSoda = 0;
    player1.numIncreaseSoda = 0;
    player2.numIncreaseSoda = 0;
    player1.numGambleRoulette = 0;
    player2.numGambleRoulette = 0;
    player1.numUnknownSoda = 0;
    player2.numUnknownSoda = 0;
    currentPlayer = 0;
    player1.dir = RIGHT;
    player2.dir = RIGHT;
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
    int currentScreen = MAIN_MENU;
    int steps = 1;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {                    // close window by the "X"
                running = 0;
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {  // pressed mouse
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);
                if (currentScreen == MAIN_MENU) {
                    if (mouse_is_above(mouseX, mouseY, startButtonRect) ) {      // press start
                        currentScreen = GAME_SCREEN;
                    } else if (mouse_is_above(mouseX, mouseY, quitButtonRect)) { // press exit
                        running = 0;
                    }
                } else if (currentScreen == GAME_SCREEN) {                   
                    if (mouse_is_above(mouseX, mouseY, homepageRect)) {          // press home
                        currentScreen = MAIN_MENU;
                    } else if (mouse_is_above(mouseX, mouseY, bagpackRect)) {    // press bagpack
                        currentScreen = BAGPACK_SCREEN;
                    } else if (mouse_is_above(mouseX, mouseY, diceRect)) {       // press dice
                        steps = roll_dice();      
                        renderDiceAnimation(steps, MAP);                   
                        updatePlayerPosition(steps, currentPlayer, MAP);   
                        square_event(MAP, currentPlayer);                  
                        currentPlayer = (currentPlayer + 1) % player_num;
                        if (game_round == 0) {
                            currentScreen = GAME_OVER_SCREEN;
                        }
                    }
                } else if (currentScreen == BAGPACK_SCREEN) {                    
                    if (mouse_is_above(mouseX, mouseY, returnButtonRect)) {      // press return
                        currentScreen = GAME_SCREEN;
                    }
                } else if (currentScreen == GAME_OVER_SCREEN) {                 
                    if (mouse_is_above(mouseX, mouseY, restartRect)) {          // press restart
                        game_round = 3;
                        //GAME_init(MAP);
                        initialize_map(MAP);
                        initialize_player();
                        render_map_and_player(MAP);
                    } else if (mouse_is_above(mouseX, mouseY, exitRect)) {       // press exit
                        running = 0;
                    }
                }
            } else if (event.type == SDL_WINDOWEVENT && newWindow != NULL) {     // 處理新視窗的事件
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
            renderBagpackScreen();
        } else if (currentScreen == GAME_OVER_SCREEN) {
            renderGameOverScreen();
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
    
    // 剩餘回合數
    char roundText[20];
    snprintf(roundText, sizeof(roundText), "Round: %d", game_round);
    SDL_Texture* roundTextTexture = renderText(roundText,textColor);
    SDL_Rect roundTextRect = {100, 120, 200, 70};
    SDL_RenderCopy(renderer, roundTextTexture, NULL, &roundTextRect);
    // 文字提示玩家順序
    SDL_Texture* textTexture = renderText("Current Player", textColor);
    SDL_Rect textRect = {100, 200, 350, 70};
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    // 玩家
    SDL_Rect player1Rect = {160, 290, 50, 50};
    SDL_RenderCopy(renderer, player[0], NULL, &player1Rect);
    SDL_Rect player2Rect = {160, 360, 50, 50};
    SDL_RenderCopy(renderer, player[1], NULL, &player2Rect);
    // 箭頭
    if (currentPlayer == 0) {
        SDL_Texture* arrowTexture = renderText("->", textColor);
        SDL_Rect arrowRect = {100, 290, 50, 50};
        SDL_RenderCopy(renderer, arrowTexture, NULL, &arrowRect);
    } else {
        SDL_Texture* arrowTexture = renderText("->", textColor);
        SDL_Rect arrowRect = {100, 360, 50, 50};
        SDL_RenderCopy(renderer, arrowTexture, NULL, &arrowRect);
    }
}

void renderBagpackScreen() {
    // 顯示背包
    for (int i = 0; i < 10; i++) {
        SDL_Rect toolTileRect_size = {toolTileRect[i][0], toolTileRect[i][1], 90, 90};
        SDL_RenderCopy(renderer, toolTileTexture, NULL, &toolTileRect_size);
    }
    SDL_RenderCopy(renderer, returnButtonTexture, NULL, &returnButtonRect);
    SDL_RenderCopy(renderer, moneyTexture, NULL, &moneyRect);
    SDL_RenderCopy(renderer, ginderSodaTexture, NULL, &ginderSodaRect);
    SDL_RenderCopy(renderer, moneyPrintTexture, NULL, &moneyPrintRect);
    SDL_RenderCopy(renderer, gingerSodaPrintTexture, NULL, &gingerSodaPrintRect);
    if (currentPlayer == 0) { // 玩家1的背包
        SDL_RenderCopy(renderer, player1TitleTexture, NULL, &playerTitleRect);
        int p1_num1 = player1.numDecreaseSoda;
        int p1_num2 = player1.numDecreaseSoda +  player1.numIncreaseSoda;
        int p1_num3 = player1.numDecreaseSoda +  player1.numIncreaseSoda + player1.numGambleRoulette;
        int p1_num4 = player1.numDecreaseSoda +  player1.numIncreaseSoda + player1.numGambleRoulette + player1.numUnknownSoda;
            
        for (int i = 0; i < p1_num1; i++) {
            SDL_Rect toolRect_size = {toolRect[i][0], toolRect[i][1], 80, 80};
            SDL_RenderCopy(renderer, decreasingSodaTexture, NULL, &toolRect_size);
        }
        for (int i = p1_num1; i < p1_num2; i++) {
            SDL_Rect toolRect_size = {toolRect[i][0], toolRect[i][1], 80, 80};
            SDL_RenderCopy(renderer, increasingSodaTexture, NULL, &toolRect_size);
        }
        for (int i = p1_num2; i < p1_num3; i++) {
            SDL_Rect toolRect_size = {toolRect[i][0], toolRect[i][1], 80, 80};
            SDL_RenderCopy(renderer, gambleRouletteTexture, NULL, &toolRect_size);
        }
        for (int i = p1_num3; i < p1_num4; i++) {
            SDL_Rect toolRect_size = {toolRect[i][0], toolRect[i][1], 80, 80};
            SDL_RenderCopy(renderer, unknownSodaTexture, NULL, &toolRect_size);
        }
    }
            
            else if (currentPlayer == 1) { // 玩家2的背包
                SDL_RenderCopy(renderer, player2TitleTexture, NULL, &playerTitleRect);
                int p2_num1 = player2.numDecreaseSoda;
                int p2_num2 = player2.numDecreaseSoda +  player2.numIncreaseSoda;
                int p2_num3 = player2.numDecreaseSoda +  player2.numIncreaseSoda + player2.numGambleRoulette;
                int p2_num4 = player2.numDecreaseSoda +  player2.numIncreaseSoda + player2.numGambleRoulette + player2.numUnknownSoda;
            
                for (int i = 0; i < p2_num1; i++) {
                    SDL_Rect toolRect_size = {toolRect[i][0], toolRect[i][1], 80, 80};
                    SDL_RenderCopy(renderer, decreasingSodaTexture, NULL, &toolRect_size);
                }
                for (int i = p2_num1; i < p2_num2; i++) {
                    SDL_Rect toolRect_size = {toolRect[i][0], toolRect[i][1], 80, 80};
                    SDL_RenderCopy(renderer, increasingSodaTexture, NULL, &toolRect_size);
                }
                for (int i = p2_num2; i < p2_num3; i++) {
                    SDL_Rect toolRect_size = {toolRect[i][0], toolRect[i][1], 80, 80};
                    SDL_RenderCopy(renderer, gambleRouletteTexture, NULL, &toolRect_size);
                }
                for (int i = p2_num3; i < p2_num4; i++) {
                    SDL_Rect toolRect_size = {toolRect[i][0], toolRect[i][1], 80, 80};
                    SDL_RenderCopy(renderer, unknownSodaTexture, NULL, &toolRect_size);
                }
}

void renderGameOverScreen() {
    // 背景顏色
    //SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    //SDL_RenderClear(renderer);

    // 玩家資訊
    char player1Info[100];
    snprintf(player1Info, sizeof(player1Info), "Player1: Money: %d, Ginger Soda: %d", player1.money, player1.ginger_soda);
    SDL_Surface* player1Surface = TTF_RenderText_Solid(font, player1Info, textColor);
    SDL_Texture* player1Texture = SDL_CreateTextureFromSurface(renderer, player1Surface);
    SDL_Rect player1Rect = {100, 100, player1Surface->w, player1Surface->h};
    SDL_RenderCopy(renderer, player1Texture, NULL, &player1Rect);

    char player2Info[100];
    snprintf(player2Info, sizeof(player2Info), "Player2: Money: %d, Ginger Soda: %d", player2.money, player2.ginger_soda);
    SDL_Surface* player2Surface = TTF_RenderText_Solid(font, player2Info, textColor);
    SDL_Texture* player2Texture = SDL_CreateTextureFromSurface(renderer, player2Surface);
    SDL_Rect player2Rect = {100, 200, player2Surface->w, player2Surface->h};
    SDL_RenderCopy(renderer, player2Texture, NULL, &player2Rect);

    SDL_Texture* restartTexture = renderText("RESTRT", textColor);
    SDL_RenderCopy(renderer, restartTexture, NULL, &restartRect);
    SDL_Texture* exitTexture = renderText("EXIT", textColor);
    SDL_RenderCopy(renderer, exitTexture, NULL, &exitRect);
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

SDL_Texture* renderText(const char* message, SDL_Color color) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, message, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

// 擲骰子
int roll_dice(){
    return rand() % 6 + 1;
}

// 根據骰子點數移動玩家位置
void updatePlayerPosition(int steps, int currentPlayer, int* MAP) {
    int finalRoll = steps;
    int* position;
    int* direction;
    
    if (currentPlayer == 0) {
        position = player1.position;
        direction = &player1.dir;
    } else {
        position = player2.position;
        direction = &player2.dir;
        game_round--;
    }

    while (steps) {
        if ((position[0] == 1210 && position[1] == 665) || 
            (position[0] == 1210 && position[1] == 25) ||
            (position[0] == 570 && position[1] == 25) ||
            (position[0] == 570 && position[1] == 665) ||
            (position[0] == 1240 && position[1] == 665) || 
            (position[0] == 1240 && position[1] == 25) ||
            (position[0] == 600 && position[1] == 25) ||
            (position[0] == 600 && position[1] == 665)) {
            *direction = (*direction + 1) % 4;
        }
        
        switch (*direction) {
            case UP:
                position[1] -= 80;
                break;
            case LEFT:
                position[0] -= 80;
                break;
            case DOWN:
                position[1] += 80;
                break;
            case RIGHT:
                position[0] += 80;
                break;
        }

        // 顯示當前移動情況
        SDL_RenderClear(renderer);
        renderGameScreen(MAP, finalRoll);
        SDL_RenderPresent(renderer);
        SDL_Delay(500);
        steps--;
    }
}

// 根據玩家位置行動
void square_event(int* MAP, int currentPlayer){
    Player* currentPlayerPtr;
    if (currentPlayer == 0) {
        currentPlayerPtr = &player1;
    } else {
        currentPlayerPtr = &player2;
    }

    int index = getSquareTypeFromPosition(currentPlayerPtr->position[0], currentPlayerPtr->position[1]);
    int squareType = MAP[index];

    switch (squareType) {
        case SQUARE_normal:
            currentPlayerPtr->money += 100; // Temporarily set to 100 for convenience
            printf("player%d now has %d\n", currentPlayer + 1, currentPlayerPtr->money);
            break;
        case SQUARE_chance:
            printf("event\n");
            chance();
            break;
        case SQUARE_game:
            printf("game\n");
            // Commented out code for minigames can be uncommented as needed
            /*
            int the_game = rand() % 6;
            switch (the_game) {
                case GAME_Minesweeper:
                    minigame_minesweeper();
                    break;
                case GAME_Dice_number:
                    minigame_Dice_number();
                    break;
                case GAME_Timer_Challenge:
                    minigame_Timer_Challenge();
                    break;
                case GAME_Flip_Card:
                    minigame_Flip_Card();
                    break;
                case GAME_Coin_catcher:
                    minigame_Coin_catcher();
                    break;
                default:
                    break;
            }
            */
            break;
        case SQUARE_store:
            printf("store\n");
            store();
            break;
        case SQUARE_ginger_soda:
            printf("sell ginger soda\n");
            ginger_soda();
            // Swap ginger soda square with a random square on the map
            int randomSquare = 4 + rand() % (32 - 4);
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

void GAME_end(){
    if(!close()){
        printf("game close failed\n");
    }
}

// 釋放資源
int close() {
    if (newRenderer) SDL_DestroyRenderer(newRenderer);
    if (newWindow) SDL_DestroyWindow(newWindow);
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    if (font) TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();
    return 1;
}
