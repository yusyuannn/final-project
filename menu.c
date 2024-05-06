#include <SDL2/SDL.h>
#include "menu.h"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

SDL_Window* window;           // 主視窗
SDL_Window* bagWindow;        // 背包視窗

SDL_Renderer* renderer;       // 主視窗的render
SDL_Renderer* bagRenderer;    // 背包視窗的render

SDL_Texture* startButtonTexture;
SDL_Texture* quitButtonTexture;
SDL_Texture* titleTexture;
SDL_Texture* bagpackTexture;
SDL_Texture* homepageTexture;
SDL_Texture* images[6];
SDL_Texture* gameTileTexture;
SDL_Texture* chanceTileTexture;
SDL_Texture* normalTileTexture;
SDL_Texture* sodaTileTexture;
SDL_Texture* startTileTexture;
SDL_Texture* shopTileTexture;

// 各個物件的位置
SDL_Rect startButtonRect = { 760, 200, 335, 135 };
SDL_Rect quitButtonRect = { 780, 400, 284, 120 };
SDL_Rect backButtonRect = { 10, 5, 40, 40 };
SDL_Rect titleRect = { 120, 120, 551,  444};
SDL_Rect bagpackRect = {180, 120, 50, 50};
SDL_Rect homepageRect = {120, 120, 50, 50};

// 初始背景顏色
SDL_Color menuBackgrounColor = {254,252,251,0xFF};

enum Screen { MAIN_MENU, GAME_SCREEN };

int init() {
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Ginger Soda", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(renderer, menuBackgrounColor.r, menuBackgrounColor.g, menuBackgrounColor.b, menuBackgrounColor.a);
    
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
    SDL_Surface* shopTileSurface = SDL_LoadBMP("images/shopTile.bmp");
    startButtonTexture = SDL_CreateTextureFromSurface(renderer, startButtonSurface);
    quitButtonTexture = SDL_CreateTextureFromSurface(renderer, quitButtonSurface);
    titleTexture = SDL_CreateTextureFromSurface(renderer, titleSurface);
    bagpackTexture = SDL_CreateTextureFromSurface(renderer, bagpackSurface);
    homepageTexture = SDL_CreateTextureFromSurface(renderer, homepageSurface);
    images[0] = SDL_CreateTextureFromSurface(renderer, gameTileSurface);
    images[1] = SDL_CreateTextureFromSurface(renderer, chanceTileSurface);
    images[2] = SDL_CreateTextureFromSurface(renderer, normalTileSurface);
    images[3] = SDL_CreateTextureFromSurface(renderer, sodaTileSurface);
    images[4] = SDL_CreateTextureFromSurface(renderer, startTileSurface);
    images[5] = SDL_CreateTextureFromSurface(renderer, shopTileSurface);
    SDL_FreeSurface(startButtonSurface);
    SDL_FreeSurface(quitButtonSurface);
    SDL_FreeSurface(titleSurface);
    SDL_FreeSurface(bagpackSurface);
    SDL_FreeSurface(gameTileSurface);
    SDL_FreeSurface(chanceTileSurface);
    SDL_FreeSurface(normalTileSurface);
    SDL_FreeSurface(sodaTileSurface);
    SDL_FreeSurface(startTileSurface);
    SDL_FreeSurface(shopTileSurface);
    return 1;
}

int initBag(int mouseX, int mouseY) {
    // 獲取點擊位置相對於視窗的坐標
    int winX, winY;
    SDL_GetWindowPosition(window, &winX, &winY);
    // 計算新視窗的位置
    int newX = winX + mouseX;
    int newY = winY + mouseY;
    bagWindow = SDL_CreateWindow("Bag", newX, newY, 400, 300, SDL_WINDOW_SHOWN);
    bagRenderer = SDL_CreateRenderer(bagWindow, -1, SDL_RENDERER_ACCELERATED);
    return 1;               
}

void shuffleImages(int* imageOrder, int numImages) {
    // 隨機種子
    srand(time(NULL));

    int imageCounts[] = {6, 12, 9, 1}; // images[0]～images[3]各自的數量
    int index = 0;
    // 將每個images的index依照各自的數量放到imageOrder
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < imageCounts[i]; ++j) {
            imageOrder[index++] = i;
        }
    }

    // 隨機交換imageOrder中的元素
    for (int i = numImages - 1; i > 0; --i) {
        int j = rand() % (i + 1);
        int temp = imageOrder[i];
        imageOrder[i] = imageOrder[j];
        imageOrder[j] = temp;
    }
}

void renderGameScreen(int* imageOrder) {
    
    // 地圖位置（chance、game、normal、soda）
    int positions[28][2] = {
        {640, 0}, {720, 0}, {800, 0}, {880, 0}, {960, 0}, {1040, 0}, {1120, 0},
        {640, 640}, {720, 640}, {800, 640}, {880, 640}, {960, 640}, {1040, 640}, {1120, 640},
        {560, 80}, {560, 160}, {560, 240}, {560, 320}, {560, 400}, {560, 480}, {560, 560},
        {1200, 80}, {1200, 160}, {1200, 240}, {1200, 320}, {1200, 400}, {1200, 480}, {1200, 560}
    };
    // 地圖位置（shop、start）
    int fixedPositions[4][2] = {
        {560,0}, {1200,0}, {560,640}, {1200,640}
    };

    for (int i = 0; i < 4; i++) {
        SDL_Rect rect = {fixedPositions[i][0], fixedPositions[i][1], 80, 80};
        SDL_RenderCopy(renderer, images[5], NULL, &rect);
        if (i == 3) {
            SDL_Rect rect = {fixedPositions[i][0], fixedPositions[i][1], 80, 80};
            SDL_RenderCopy(renderer, images[4], NULL, &rect);
            break;
        }
    }
    for (int i = 0; i < 28; i++) {
        SDL_Rect rect = {positions[i][0], positions[i][1], 80, 80};
        int randomIndex = imageOrder[i];
        SDL_RenderCopy(renderer, images[randomIndex], NULL, &rect);
    }
}


int close() {
    SDL_DestroyTexture(startButtonTexture);
    SDL_DestroyTexture(quitButtonTexture);
    SDL_DestroyTexture(titleTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    for (int i = 0; i < 6; ++i) {
        SDL_DestroyTexture(images[i]);
    } 
    SDL_Quit();
    return 1;
}

void processEvents() {

    const int numImages = 28;
    int imageOrder[numImages];
    shuffleImages(imageOrder, numImages);

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
                                initBag(mouseX, mouseY);
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
            renderGameScreen(imageOrder);
            SDL_RenderCopy(renderer, homepageTexture, NULL, &homepageRect);
            SDL_RenderCopy(renderer, bagpackTexture, NULL, &bagpackRect);
        }
        SDL_RenderPresent(renderer);
    }
}

int main() {
    init();
    processEvents();
    close();
    return 0;
}

