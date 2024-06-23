#include "dice_game.h"

void dicerenderTextCentered(SDL_Renderer* renderer, TTF_Font* font, const char* text, SDL_Color textColor, SDL_Rect rect) {
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text, textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    int textWidth = textSurface->w;
    int textHeight = textSurface->h;
    SDL_FreeSurface(textSurface);

    SDL_Rect textRect;
    textRect.w = textWidth;
    textRect.h = textHeight;
    textRect.x = rect.x + (rect.w - textWidth) / 2;
    textRect.y = rect.y + (rect.h - textHeight) / 2;

    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    SDL_DestroyTexture(textTexture);
}

void renderText(SDL_Renderer* renderer, TTF_Font* font, const char* text, int x, int y, SDL_Color color) {
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text, color);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_Rect textRect = {x, y, textSurface->w, textSurface->h};
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(textSurface);
}

SDL_Texture* loadBackground(SDL_Renderer* renderer, int num) {
    if(num == 1){
        SDL_Surface* surface = SDL_LoadBMP("minigame_image/dice_game1.bmp");
        if (!surface) {
            printf("Unable to load image! SDL_image Error: %s\n", IMG_GetError());
            return NULL;
        }
    
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (!texture) {
            printf("Unable to create texture from! SDL Error: %s\n", SDL_GetError());
        }
    
        SDL_FreeSurface(surface);
        return texture;
    }else{
        SDL_Surface* surface = SDL_LoadBMP("minigame_image/dice_game2.bmp");
        if (!surface) {
            printf("Unable to load image! SDL_image Error: %s\n", IMG_GetError());
            return NULL;
        }
    
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (!texture) {
            printf("Unable to create texture from! SDL Error: %s\n", SDL_GetError());
        }
    
        SDL_FreeSurface(surface);
        return texture;
    }
}

SDL_Texture* loadDiceTexture(SDL_Renderer* renderer, int value) {
    char filename[100];
    snprintf(filename, sizeof(filename), "main_game_image/dice_%d.bmp", value);
    SDL_Surface* diceSurface = SDL_LoadBMP(filename);
    if (diceSurface == NULL) {
        printf("Failed to load image! SDL_Error: %s\n", SDL_GetError());
        return NULL;
    }
    SDL_Texture* diceTexture = SDL_CreateTextureFromSurface(renderer, diceSurface);
    SDL_FreeSurface(diceSurface);
    return diceTexture;
}

void renderDiceAnimation(SDL_Renderer* renderer, SDL_Texture* diceTextures[], SDL_Rect dice1Rect, SDL_Rect dice2Rect, int* currentFrame1, int* currentFrame2) {
    SDL_RenderCopy(renderer, diceTextures[*currentFrame1], NULL, &dice1Rect);
    SDL_RenderCopy(renderer, diceTextures[*currentFrame2], NULL, &dice2Rect);
    *currentFrame1 = (*currentFrame1 + 1) % 6;
    *currentFrame2 = (*currentFrame2 + 1) % 6;
}

void run_dice_game() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return;
    }

    if (TTF_Init() == -1) {
        printf("TTF could not initialize! TTF_Error: %s\n", TTF_GetError());
        SDL_Quit();
        return;
    }

    SDL_Window* window = SDL_CreateWindow("擲骰子遊戲",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          DICE_SCREEN_WIDTH,
                                          DICE_SCREEN_HEIGHT,
                                          SDL_WINDOW_SHOWN);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    TTF_Font* font = TTF_OpenFont("fonts/Cubic_11_1.100_R.ttf", 28);
    SDL_Texture* backgroundTexture1 = loadBackground(renderer, 1);
    SDL_Texture* backgroundTexture2 = loadBackground(renderer, 2);
    SDL_Texture* currentBackground = backgroundTexture1; // 初始化为背景1
    SDL_Texture* diceTextures[6];
    for (int i = 0; i < 6; ++i) {
        diceTextures[i] = loadDiceTexture(renderer, i + 1);
    }

    SDL_Rect dice1Rect = {DICE_SCREEN_WIDTH / 3 - DICE_SIZE / 2, (DICE_SCREEN_HEIGHT - DICE_SIZE) / 2, DICE_SIZE, DICE_SIZE};
    SDL_Rect dice2Rect = {2 * DICE_SCREEN_WIDTH / 3 - DICE_SIZE / 2, (DICE_SCREEN_HEIGHT - DICE_SIZE) / 2, DICE_SIZE, DICE_SIZE};

    srand(time(NULL));

    int points1[SIZE];
    int points2[SIZE];
    int counter = 0;

    int quit = 0;
    SDL_Event e;
    const char* resultText = NULL;
    int showCheckButton = 0;
    SDL_Color Color = {0, 0, 0, 255}; // 黑色

    int isAnimating = 1;
    int showText = 1;
    int currentFrame1 = 0;
    int currentFrame2 = 0;
    Uint32 lastFrameTime = 0;
    Uint32 frameDuration = 50; 
    
    while (!quit) {
        Uint32 currentTime = SDL_GetTicks();
        if (currentTime - lastFrameTime >= frameDuration) {
            lastFrameTime = currentTime;
            SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, currentBackground, NULL, NULL);

            if (isAnimating) {
                renderDiceAnimation(renderer, diceTextures, dice1Rect, dice2Rect, &currentFrame1, &currentFrame2);
            }

            char infoText[100];
            if(showText){
                snprintf(infoText, sizeof(infoText), "Let's Play A Dice Roll Game");
                renderText(renderer, font, infoText, 450,  100, Color);
            }
            
            snprintf(infoText, sizeof(infoText), "YOUR");
            renderText(renderer, font, infoText, DICE_SCREEN_WIDTH / 3 - 30, (DICE_SCREEN_HEIGHT - DICE_SIZE) / 2 - 60, Color);
            snprintf(infoText, sizeof(infoText), "COM");
            renderText(renderer, font, infoText, 2 * DICE_SCREEN_WIDTH / 3 - 30, (DICE_SCREEN_HEIGHT - DICE_SIZE) / 2 - 60, Color);

            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT) {
                    quit = 1;
                } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                    int x, y;
                    SDL_GetMouseState(&x, &y);

                    if (showCheckButton) {
                        SDL_RenderCopy(renderer, backgroundTexture2, NULL, NULL);
                        if (x >= (DICE_SCREEN_WIDTH - 200) / 2 && x <= (DICE_SCREEN_WIDTH + 200) / 2 && y >= DICE_SCREEN_HEIGHT - 100 && y <= DICE_SCREEN_HEIGHT - 50) {
                            quit = 1;
                        }
                    } else {
                        SDL_RenderCopy(renderer, backgroundTexture1, NULL, NULL);
                        if (x >= (DICE_SCREEN_WIDTH / 2) - 200 - 10 && x <= (DICE_SCREEN_WIDTH / 2) - 10 && y >= DICE_SCREEN_HEIGHT - 100 - 10 && y <= DICE_SCREEN_HEIGHT - 10) {
                            points1[counter] = rand() % 6 + 1;
                            points2[counter] = rand() % 6 + 1;
                            if (points1[counter] == points2[counter]) {
                                resultText = "TRY AGAIN";
                            } else if (points1[counter] > points2[counter]) {
                                resultText = "YOU WIN 50 COIN";
                                showCheckButton = 1;
                            } else {
                                resultText = "YOU LOSE QQ";
                                showCheckButton = 1;
                            }
                            counter++;
                            isAnimating = 0;
                        } else if (x >= (DICE_SCREEN_WIDTH / 2) + 10 && x <= (DICE_SCREEN_WIDTH / 2) + 200 + 10 && y >= DICE_SCREEN_HEIGHT - 100 - 10 && y <= DICE_SCREEN_HEIGHT - 10) {
                            points1[counter] = rand() % 6 + 1;
                            points2[counter] = rand() % 6 + 1;
                            if (points1[counter] == points2[counter]) {
                                resultText = "TRY AGAIN";
                            } else if (points1[counter] < points2[counter]) {
                                resultText = "YOU WIN 50 COIN";
                                showCheckButton = 1;
                            } else {
                                resultText = "YOU LOSE QQ";
                                showCheckButton = 1;
                            }
                            counter++;
                            isAnimating = 0;
                        }
                        showText = 0;
                    }
                }
            }
            
            if (resultText != NULL) {
                // 根据情况切换背景
                if (strcmp(resultText, "YOU WIN 50 COIN") == 0 || strcmp(resultText, "YOU LOSE QQ") == 0) {
                    currentBackground = backgroundTexture2;
                }
            }

            if (!isAnimating && counter > 0) {
                if (resultText != NULL) {
                    renderText(renderer, font, resultText, 540,  100, Color);
                }

                SDL_Texture* dice1Texture = loadDiceTexture(renderer, points1[counter - 1]);
                SDL_Texture* dice2Texture = loadDiceTexture(renderer, points2[counter - 1]);
                SDL_Rect dice1RectFinal = {DICE_SCREEN_WIDTH / 3 - DICE_SIZE / 2, (DICE_SCREEN_HEIGHT - DICE_SIZE) / 2, DICE_SIZE, DICE_SIZE};
                SDL_Rect dice2RectFinal = {2 * DICE_SCREEN_WIDTH / 3 - DICE_SIZE / 2, (DICE_SCREEN_HEIGHT - DICE_SIZE) / 2, DICE_SIZE, DICE_SIZE};
                SDL_RenderCopy(renderer, dice1Texture, NULL, &dice1RectFinal);
                SDL_RenderCopy(renderer, dice2Texture, NULL, &dice2RectFinal);
            }
            SDL_RenderPresent(renderer);
        }
    }

    for (int i = 0; i < 6; ++i) {
        SDL_DestroyTexture(diceTextures[i]);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    // TTF_CloseFont(font);
    // TTF_Quit();
    // SDL_Quit();

}
