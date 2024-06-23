#include "shop.h"
#include "game.h"

// gcc -Isrc/Include -Lsrc/lib -o shop shop.c -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

// 定義一個全局變量來保存對話框文本和金錢
char dialogText[10000] = "Welcome to the shop, what do you want?";
int *money; // 初始金額



void market(Product products[], SDL_Renderer *renderer, TTF_Font *font) {
    int poker[4] = {0, 1, 2, 3}; 
    int prices[4] = {10, 20, 30, 40};
    int i, j, tmp;
    srand(time(0));
    
    for (i = 0; i < 4; i++) {
        j = rand() % 4;
        tmp = poker[i];
        poker[i] = poker[j];
        poker[j] = tmp;
    }
    
    for (i = 0; i < 4; i++){
        products[i].imageIndex = poker[i];
    }
    
    const char *filenames[4] = {"main_game_image/commodity1.bmp", "main_game_image/commodity2.bmp", "main_game_image/commodity3.bmp", "main_game_image/commodity4.bmp"};
    
    for (i = 0; i < 3; i++) {
        products[i].price = prices[poker[i]];
        products[i].rect = (SDL_Rect){100 + i * 200, 310, 100, 100}; // 改變這裡的 x 座標為 50 + i * 200
        SDL_Surface *imageSurface = SDL_LoadBMP(filenames[products[i].imageIndex]); // 使用商品圖片索引獲取對應的文件名
        if (imageSurface == NULL) {
            printf("Unable to load image %s! SDL_Error: %s\n", filenames[products[i].imageIndex], SDL_GetError());
            exit(1);
        }
        products[i].image = SDL_CreateTextureFromSurface(renderer, imageSurface);
        SDL_FreeSurface(imageSurface);
        
        char priceText[50];
        sprintf(priceText, "$%d", products[i].price);
        
        SDL_Surface *surface = TTF_RenderText_Solid(font, priceText, (SDL_Color){0, 0, 0, 255});
        products[i].texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }
    
}

void renderTextWrapped(SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_Color color, SDL_Rect *rect, int wrapLength) {
    char *textCopy = strdup(text);
    int x = rect->x;
    int y = rect->y;

    SDL_Surface *surface = TTF_RenderText_Blended_Wrapped(font, textCopy, color, wrapLength);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect dst = {x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &dst);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
    free(textCopy);
}

void displayDialog(SDL_Renderer *renderer, TTF_Font *font, const char *format, ...) {
    // 清除對話框區域的內容
    SDL_Rect dialogRect = {830, 130, 380, 420};
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0); // 設置顏色為透明

    char message[1024]; // 假設文本不會超過1024個字符
    va_list args;
    va_start(args, format);
    vsnprintf(message, sizeof(message), format, args);
    va_end(args);

    SDL_Color textColor = {0, 0, 0}; // 黑色

    // 確定文本大小和位置
    int textWidth = 0, textHeight = 0;
    TTF_SizeText(font, message, &textWidth, &textHeight);
    SDL_Rect textRect = {830, 130, textWidth, textHeight}; // 文本的位置和大小

    // 渲染文本（自動換行）
    renderTextWrapped(renderer, font, message, textColor, &textRect, 380);
}

void itemDepiction(int num){
    switch(num){
        case 0:
            sprintf(dialogText, "It's Decrease Soda, go backkkkk k");
            break;

        case 1:
            sprintf(dialogText, "It's Increase Soda, go go gogogo");
            break;

        case 2:
            sprintf(dialogText, "It's Gamble Roulette, pay to winnnnnnnn");
            break;

        case 3:
            sprintf(dialogText, "It's Unknown Soda, idk :P");
            break;
    }
}

void displayMoney(SDL_Renderer *renderer, TTF_Font *font) {
    char moneyText[50];
    sprintf(moneyText, "$%d", *money);

    SDL_Color textColor = {0, 0, 0}; // 黑色
    SDL_Surface *surface = TTF_RenderText_Solid(font, moneyText, textColor);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    SDL_Rect moneyRect = {400, 215, 100, 50};
    SDL_RenderCopy(renderer, texture, NULL, &moneyRect);
    SDL_DestroyTexture(texture);
}

int shop_main() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }
    
    if (TTF_Init() == -1) {
        printf("TTF_Init: %s\n", TTF_GetError());
        return 1;
    }
    
    SDL_Window* window = SDL_CreateWindow("Shop", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }
    
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    
    TTF_Font *font = TTF_OpenFont("fonts/Cubic_11_1.100_R.ttf", 28);
    if (font == NULL) {
        printf("Failed to load font: %s\n", TTF_GetError());
        return 1;
    }
    
    SDL_Surface *bgSurface = SDL_LoadBMP("main_game_image/shop.bmp");
    if (bgSurface == NULL) {
        printf("Unable to load image %s! SDL_Error: %s\n", "main_game_image/shop.bmp", SDL_GetError());
        return 1;
    }
    
    SDL_Texture *bgTexture = SDL_CreateTextureFromSurface(renderer, bgSurface);
    SDL_FreeSurface(bgSurface);
    
    Product products[3];
    market(products, renderer, font);
    
    SDL_Rect exitButtonRect = {200, 600, 300, 75};

    SDL_Rect buyButtonRects[3];
    for (int i = 0; i < 3; i++) {
        buyButtonRects[i] = (SDL_Rect){78 + i * 200, 520, 150, 50};
    }
    
    if(currentPlayer == 0) money = &(player1.money);
    if(currentPlayer == 1) money = &(player2.money);

    SDL_Event e;
    int quit = 0;
    
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            }
            
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                
                for (int i = 0; i < 3; i++) {
                    if (x > products[i].rect.x && x < products[i].rect.x + products[i].rect.w &&
                        y > products[i].rect.y && y < products[i].rect.y + products[i].rect.h) {
                        itemDepiction(products[i].imageIndex);   
                    }

                    // Check if buy button is clicked
                    if (x > buyButtonRects[i].x && x < buyButtonRects[i].x + buyButtonRects[i].w &&
                        y > buyButtonRects[i].y && y < buyButtonRects[i].y + buyButtonRects[i].h) {
                        if (*money >= products[i].price) {
                            *money -= products[i].price;
                            switch(products[i].imageIndex){
                                case 0:
                                    if(currentPlayer == 0) player1.numDecreaseSoda ++;
                                    if(currentPlayer == 1) player2.numDecreaseSoda ++;
                                    break;

                                case 1:
                                    if(currentPlayer == 0) player1.numIncreaseSoda ++;
                                    if(currentPlayer == 1) player2.numIncreaseSoda ++;
                                    break;

                                case 2:
                                    if(currentPlayer == 0) player1.numGambleRoulette ++;
                                    if(currentPlayer == 1) player2.numGambleRoulette ++;
                                    break;

                                case 3:
                                    if(currentPlayer == 0) player1.numUnknownSoda ++;
                                    if(currentPlayer == 1) player2.numUnknownSoda ++;
                                    break;
                            }
                            sprintf(dialogText, "You bought it for $%d! Thank you!", products[i].price);
                            itemDepiction(products[i].imageIndex);
                            // products[i].imageIndex = 4;
                        } else {
                            sprintf(dialogText, "OOPS! Not enough money! You need $%d more.", products[i].price - *money);
                        }



                    }
                }
                
                if (x > exitButtonRect.x && x < exitButtonRect.x + exitButtonRect.w &&
                    y > exitButtonRect.y && y < exitButtonRect.y + exitButtonRect.h) {
                    quit = 1;
                }
            }
        }
        
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, bgTexture, NULL, NULL);
        displayDialog(renderer, font, dialogText);
        displayMoney(renderer, font); // 顯示金錢
        
        for (int i = 0; i < 3; i++) {
            SDL_RenderCopy(renderer, products[i].image, NULL, &products[i].rect);
            SDL_Rect priceRect = {products[i].rect.x - 20, products[i].rect.y + 110, 150, 50};
            SDL_RenderCopy(renderer, products[i].texture, NULL, &priceRect);
        }
        
        SDL_RenderPresent(renderer);
    }
    
    for (int i = 0; i < 3; i++) {
        SDL_DestroyTexture(products[i].texture);
        SDL_DestroyTexture(products[i].image);
    }
    
    SDL_DestroyTexture(bgTexture);
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    
    return 0;
}