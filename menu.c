#include <SDL2/SDL.h>
#include "menu.h"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

SDL_Window* window;
SDL_Window* bagWindow;
SDL_Renderer* renderer;
SDL_Renderer* bagRenderer;
SDL_Texture* startButtonTexture;
SDL_Texture* quitButtonTexture;
SDL_Texture* backButtonTexture;
SDL_Texture* titleTexture;
SDL_Texture* mapTexture;
SDL_Texture* bagpackTexture;
SDL_Texture* homepageTexture;

// 各個物件的位置
SDL_Rect startButtonRect = { 260, 500, 335, 135 };
SDL_Rect quitButtonRect = { 280, 650, 284, 120 };
SDL_Rect backButtonRect = { 10, 5, 40, 40 };
SDL_Rect titleRect = { 120, 20, 551,  444};
SDL_Rect mapRect = { 0, 0, 800, 800 };
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
    return 1;
}

int initButton() {
    SDL_Surface* startButtonSurface = SDL_LoadBMP("start_button.bmp");
    SDL_Surface* quitButtonSurface = SDL_LoadBMP("quit_button.bmp");
    SDL_Surface* backButtonSurface = SDL_LoadBMP("return_button.bmp");
    SDL_Surface* titleSurface = SDL_LoadBMP("title.bmp");
    SDL_Surface* mapSurface = SDL_LoadBMP("white_map.bmp");
    SDL_Surface* bagpackSurface = SDL_LoadBMP("bagpack.bmp");
    SDL_Surface* homepageSurface = SDL_LoadBMP("homepage.bmp");
    startButtonTexture = SDL_CreateTextureFromSurface(renderer, startButtonSurface);
    quitButtonTexture = SDL_CreateTextureFromSurface(renderer, quitButtonSurface);
    backButtonTexture = SDL_CreateTextureFromSurface(renderer, backButtonSurface);
    titleTexture = SDL_CreateTextureFromSurface(renderer, titleSurface);
    mapTexture = SDL_CreateTextureFromSurface(renderer, mapSurface);
    bagpackTexture = SDL_CreateTextureFromSurface(renderer, bagpackSurface);
    homepageTexture = SDL_CreateTextureFromSurface(renderer, homepageSurface);
    SDL_FreeSurface(startButtonSurface);
    SDL_FreeSurface(quitButtonSurface);
    SDL_FreeSurface(backButtonSurface);
    SDL_FreeSurface(titleSurface);
    SDL_FreeSurface(mapSurface);
    SDL_FreeSurface(bagpackSurface);
    SDL_FreeSurface(homepageSurface);
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

int close() {
    SDL_DestroyTexture(startButtonTexture);
    SDL_DestroyTexture(quitButtonTexture);
    SDL_DestroyTexture(backButtonTexture);
    SDL_DestroyTexture(titleTexture);
    SDL_DestroyTexture(mapTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 1;
}

void processEvents() {
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
                // 處理新視窗的事件
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
            SDL_RenderCopy(renderer, mapTexture, NULL, &mapRect);
            SDL_RenderCopy(renderer, homepageTexture, NULL, &homepageRect);
            SDL_RenderCopy(renderer, bagpackTexture, NULL, &bagpackRect);
        }
        SDL_RenderPresent(renderer);
    }
}

int main() {
    init();
    initButton();
    processEvents();
    close();
    return 0;
}

