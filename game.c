#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//#include <SDL2_ttf.h>
#include <SDL2/SDL.h>
#include "game.h"


int main(){
    const int numImages = 32;
    int MAP[numImages];

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
    SDL_Surface* startButtonSurface = SDL_LoadBMP("images/start_button.bmp");
    SDL_Surface* quitButtonSurface = SDL_LoadBMP("images/quit_button.bmp");
    SDL_Surface* titleSurface = SDL_LoadBMP("images/title.bmp");
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
    SDL_Surface* initDiceSurface = SDL_LoadBMP("images/init_dice.bmp");
    startButtonTexture = SDL_CreateTextureFromSurface(renderer, startButtonSurface);
    quitButtonTexture = SDL_CreateTextureFromSurface(renderer, quitButtonSurface);
    titleTexture = SDL_CreateTextureFromSurface(renderer, titleSurface);
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
    diceTexture = SDL_CreateTextureFromSurface(renderer, initDiceSurface);
    SDL_FreeSurface(startButtonSurface);
    SDL_FreeSurface(quitButtonSurface);
    SDL_FreeSurface(titleSurface);
    SDL_FreeSurface(bagpackSurface);
    SDL_FreeSurface(gameTileSurface);
    SDL_FreeSurface(chanceTileSurface);
    SDL_FreeSurface(normalTileSurface);
    SDL_FreeSurface(sodaTileSurface);
    SDL_FreeSurface(startTileSurface);
    SDL_FreeSurface(storeTileSurface);
    SDL_FreeSurface(player1Surface);
    SDL_FreeSurface(player2Surface);
    SDL_FreeSurface(initDiceSurface);
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
        //int j = rand() % (i + 1);
        int j = 4 + rand() % (32 - 4);
        int temp = imageOrder[i];
        imageOrder[i] = imageOrder[j];
        imageOrder[j] = temp;
    }
    /* for (int i =0; i < 32; i++) {
        printf("%d ", imageOrder[i]);
    }
    printf("\n"); */
}

void initialize_player(){
    player1.name = "Player1";
    player2.name = "Player2";
    player1.position[0] = 1210;
    player1.position[1] = 665;
    player2.position[0] = 1240;
    player2.position[1] = 665;
    currentPlayer = 0;
}

// 顯示畫面
void render_map_and_player(int* MAP){
    // 用SDL render東西這樣
    SDL_Event event;
    int running = 1;
    float game_round = 20; // 行動回數
    int currentScreen = MAIN_MENU;
    while (running && game_round) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                // 得到滑鼠位置
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);
                if (currentScreen == MAIN_MENU) {
                    if (mouseX >= startButtonRect.x && mouseX < startButtonRect.x + startButtonRect.w &&
                        mouseY >= startButtonRect.y && mouseY < startButtonRect.y + startButtonRect.h) {
                        currentScreen = GAME_SCREEN;
                    } else if (mouseX >= quitButtonRect.x && mouseX < quitButtonRect.x + quitButtonRect.w &&
                               mouseY >= quitButtonRect.y && mouseY < quitButtonRect.y + quitButtonRect.h) {
                        running = 0;
                    }
                } else if (currentScreen == GAME_SCREEN) {
                    if (mouseX >= homepageRect.x && mouseX < homepageRect.x + homepageRect.w &&
                        mouseY >= homepageRect.y && mouseY < homepageRect.y + homepageRect.h &&
                        event.button.button == SDL_BUTTON_LEFT) {
                        currentScreen = MAIN_MENU;
                    } else if (mouseX >= bagpackRect.x && mouseX < bagpackRect.x + bagpackRect.w &&
                               mouseY >= bagpackRect.y && mouseY < bagpackRect.y + bagpackRect.h &&
                               event.button.button == SDL_BUTTON_LEFT) {
                        //initBag(mouseX, mouseY);
                    } else if (mouseX >= diceRect.x && mouseX < diceRect.x + diceRect.w &&
                               mouseY >= diceRect.y && mouseY < diceRect.y + diceRect.h &&
                               event.button.button == SDL_BUTTON_LEFT) {
                        int steps = roll_dice();
                        //printf("STEPS: %d\n", steps);
                        if (currentPlayer == 0) {
                            updatePlayerPosition(steps, currentPlayer); // 更新玩家1位置
                            square_event(MAP, currentPlayer);
                            //game_round -= 0.5;
                        } else {
                            updatePlayerPosition(steps, currentPlayer); // 更新玩家2位置
                            square_event(MAP, currentPlayer);
                            //game_round -= 0.5;
                        }
                        currentPlayer = (currentPlayer + 1) % player_num;
                    }
                }
            } else if (event.type == SDL_WINDOWEVENT && newWindow != NULL) {
                // 處理新視窗的事件
                if (event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(newWindow)) {
                    // 如果新視窗關閉了，則銷毀它
                    SDL_DestroyRenderer(newRenderer);
                    SDL_DestroyWindow(newWindow);
                    newRenderer = NULL;
                    newWindow = NULL;
                }
            }
        }
        SDL_RenderClear(renderer);
        if (currentScreen == MAIN_MENU) {
            SDL_RenderCopy(renderer, startButtonTexture, NULL, &startButtonRect);
            SDL_RenderCopy(renderer, quitButtonTexture, NULL, &quitButtonRect);
            SDL_RenderCopy(renderer, titleTexture, NULL, &titleRect);
        } else if (currentScreen == GAME_SCREEN) {
            // 顯示地圖
            for (int i = 0; i < 32; i++) {
                SDL_Rect rect = {mapRect[i][0], mapRect[i][1], 80, 80};
                int randomIndex = MAP[i];
                SDL_RenderCopy(renderer, square[randomIndex], NULL, &rect);
            }
            // 顯示玩家
            SDL_Rect rect1 = {player1.position[0], player1.position[1], 30, 30};
            SDL_RenderCopy(renderer, player[0], NULL, &rect1);
            SDL_Rect rect2 = {player2.position[0], player2.position[1], 30, 30};
            SDL_RenderCopy(renderer, player[1], NULL, &rect2);
            // 顯示骰子
            SDL_RenderCopy(renderer, diceTexture, NULL, &diceRect);
            // 顯示返回主頁按鈕
            SDL_RenderCopy(renderer, homepageTexture, NULL, &homepageRect);
            //SDL_RenderCopy(renderer, bagpackTexture, NULL, &bagpackRect);
        }
        SDL_RenderPresent(renderer);
    }
}

int roll_dice(){
    return rand() % 6 + 1;
}

void updatePlayerPosition(int steps, int currentPlayer) {
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
            //printf("Player1's position: %d %d\n", player1.position[0], player1.position[1]);
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
            //printf("Player2's position: %d %d\n", player2.position[0], player2.position[1]);
            steps--;
        }
    }
}

Square_type getSquareTypeFromPosition(int x, int y, int* MAP) {
    for (int i = 0; i < 32; ++i) {
        int squareX = mapRect[i][0];
        int squareY = mapRect[i][1];
        if (x >= squareX && x < squareX + 80 && y >= squareY && y < squareY + 80) {
            //printf("%d %d\n", squareX, squareY);
            return MAP[i];
        }
    }
    return -1;
}
 
void square_event(int* MAP, int currentPlayer){
    // 根據玩家位置行動
    int squareType;
    if (currentPlayer == 0) {
        squareType = getSquareTypeFromPosition(player1.position[0], player1.position[1], MAP);
    }else {
        squareType = getSquareTypeFromPosition(player2.position[0], player2.position[1], MAP);
    }
    
    //printf("return square: %d\n", squareType);
    //int the_game = rand() % 6;;
    switch (squareType) {
        case SQUARE_normal:
            // 玩家拿錢
            printf("$$$\n");
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
            break;
        default:
            break;
    }
}

void chance() {
    newWindow = SDL_CreateWindow("Chance", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    newRenderer = SDL_CreateRenderer(newWindow, -1, SDL_RENDERER_ACCELERATED);
}

void store() {
    newWindow = SDL_CreateWindow("Store", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    newRenderer = SDL_CreateRenderer(newWindow, -1, SDL_RENDERER_ACCELERATED);
}

void ginger_soda() {
    newWindow = SDL_CreateWindow("Ginger Soda Store", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    newRenderer = SDL_CreateRenderer(newWindow, -1, SDL_RENDERER_ACCELERATED);
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
