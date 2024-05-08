#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "game.h"


int main(int argc, char** argv){
    const int numImages = 28;
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
    SDL_Surface* player1TileSurface = SDL_LoadBMP("images/black_circle.bmp");
    SDL_Surface* player2TileSurface = SDL_LoadBMP("images/white_circle.bmp");
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
    player[0] = SDL_CreateTextureFromSurface(renderer, player1TileSurface);
    player[1] = SDL_CreateTextureFromSurface(renderer, player2TileSurface);
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
    SDL_FreeSurface(player1TileSurface);
    SDL_FreeSurface(player2TileSurface);
}

// 初始化地圖
void initialize_map(int* imageOrder, int numImages){
    /* 隨機交換imageOrder中的元素 */
    srand(time(NULL));
    int imageCounts[] = {9, 12, 6, 0, 1, 0}; // square各自的數量
    int index = 0;
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < imageCounts[i]; ++j) {
            imageOrder[index++] = i;
        }
    }
    for (int i = numImages - 1; i > 0; --i) {
        int j = rand() % (i + 1);
        int temp = imageOrder[i];
        imageOrder[i] = imageOrder[j];
        imageOrder[j] = temp;
    }
}

void initialize_player(){
    // 玩家資料清乾淨，然後給玩家輸入名字
    char* buffer = (char*)malloc(21*sizeof(char));
    scanf("%20s", buffer);
    // 然後給玩家這個名字這樣
}

// 顯示畫面
void render_map_and_player(int* MAP){
    // 用SDL render東西這樣
    SDL_Event event;
    int running = 1;
    int currentScreen = MAIN_MENU;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
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
                    }
                }
            } else if (event.type == SDL_WINDOWEVENT && bagWindow != NULL) {
                // 處理bag視窗的事件
                if (event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(bagWindow)) {
                    // 如果新視窗關閉了，則銷毀它
                    SDL_DestroyRenderer(bagRenderer);
                    SDL_DestroyWindow(bagWindow);
                    bagRenderer = NULL;
                    bagWindow = NULL;
                }
            }
        }
        SDL_RenderClear(renderer);
        if (currentScreen == MAIN_MENU) {
            SDL_RenderCopy(renderer, startButtonTexture, NULL, &startButtonRect);
            SDL_RenderCopy(renderer, quitButtonTexture, NULL, &quitButtonRect);
            SDL_RenderCopy(renderer, titleTexture, NULL, &titleRect);
        } else if (currentScreen == GAME_SCREEN) {
            for (int i = 0; i < 4; i++) {
                SDL_Rect rect = {mapFixedPositions[i][0], mapFixedPositions[i][1], 80, 80};
                SDL_RenderCopy(renderer, square[SQUARE_store], NULL, &rect);
                if (i == 3) {
                    SDL_Rect rect = {mapFixedPositions[i][0], mapFixedPositions[i][1], 80, 80};
                    SDL_RenderCopy(renderer, square[SQUARE_start], NULL, &rect);
                    break;
                }
            }
            for (int i = 0; i < 28; i++) {
                SDL_Rect rect = {mapPositions[i][0], mapPositions[i][1], 80, 80};
                int randomIndex = MAP[i];
                SDL_RenderCopy(renderer, square[randomIndex], NULL, &rect);
            }
            SDL_RenderCopy(renderer, homepageTexture, NULL, &homepageRect);
            SDL_RenderCopy(renderer, bagpackTexture, NULL, &bagpackRect);
        }
        SDL_RenderPresent(renderer);
    }
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