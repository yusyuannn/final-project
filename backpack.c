#include "backpack.h"
#include "game.h"
// gcc -Isrc/Include -Lsrc/lib -o game game.c -lmingw32 -lSDL2main -lSDL2
#define larger_than_0(x) ((x) > 0 ? x :0)
SDL_Window* backpackWindow = NULL;
SDL_Renderer* backpackRenderer = NULL;
SDL_Texture* backpackWindowTexture = NULL;
///
SDL_Texture* useButtonTexture = NULL;
SDL_Texture* useButtonTextTexture = NULL;
///

char toolDescriptions[4][1000] = {
    "This sad can of soda that makes you go slower... ",
    "This is a can of soda that makes you go faster!",
    "A random cube for a random item~",
    "what happens when you drink it? Who knows :D ",
};

SDL_Rect playerTitleRect = {490, 40, 400, 60};
SDL_Rect toolIllustrationRect = {210, 370, 540, 200};
SDL_Rect returnButtonRect = {1130, 70, 50, 50};
SDL_Rect toolRect = {210, 160, 540, 100};
SDL_Rect toolTitleRect = {210, 160, 100, 100};
SDL_Rect tool_quanityRect = {210, 270, 100, 50};
SDL_Rect toolIconRects[4] = {
    {320, 160, 100, 100}, // decrease soda
    {430, 160, 100, 100}, // increase soda
    {540, 160, 100, 100}, // gamble Roulette
    {650, 160, 100, 100}  // unknown soda
};
SDL_Rect Text_numToolRect[4] = {
    {320, 270, 100, 50}, // decrease soda
    {430, 270, 100, 50}, // increase soda
    {540, 270, 100, 50}, // gamble Roulette
    {650, 270, 100, 50} // unknown soda
};
SDL_Rect moneyPrintRect = {980, 235, 100, 50};
SDL_Rect gingerSodaPrintRect = {980, 435, 100, 50};
SDL_Rect backpackWindowRect = {0, 0, 1280, 720};
SDL_Rect useButtonRect = {672, 530, 80, 40};

int backpack_main(int steps){ 
    
    // if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    //     printf("SDL_Init Error: %s\n", SDL_GetError());
    //     return 1;
    // }
    
    // if (TTF_Init() == -1) {
    //     printf("TTF_Init: %s\n", TTF_GetError());
    //     SDL_Quit();
    //     return 1;
    // }

    initialize_backpack();
    backpack_mouseEvent(steps);

    return 0;
}

// center text
void renderTextCentered(SDL_Renderer* renderer, TTF_Font* font, const char* text, SDL_Color textColor, SDL_Rect rect) {
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

// text newline
void renderTextWrapped(SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_Color color, SDL_Rect *rect) {
    char *textCopy = strdup(text);
    int x = rect->x;
    int y = rect->y;

    SDL_Surface *surface = TTF_RenderText_Blended_Wrapped(font, textCopy, color, 540);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect dst = {x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &dst);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
    free(textCopy);
}


// initialize backpack
void initialize_backpack(){ 
    backpackWindow = SDL_CreateWindow("Backpack", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    backpackRenderer = SDL_CreateRenderer(backpackWindow, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(backpackRenderer, 255, 255, 255, 255);

    SDL_Surface* backpackWindowSurface = SDL_LoadBMP("main_game_image/backpack.bmp");
    if (!backpackWindowSurface) {
        printf("Error loading image: %s\n", SDL_GetError());
    } else {
        backpackWindowTexture = SDL_CreateTextureFromSurface(backpackRenderer, backpackWindowSurface);
        SDL_FreeSurface(backpackWindowSurface);
    }

    // SDL_RenderClear(backpackRenderer);
    if (backpackWindowTexture != NULL) {
        SDL_RenderCopy(backpackRenderer, backpackWindowTexture, NULL, &backpackWindowRect);
    }
    SDL_RenderPresent(backpackRenderer);
}

// initialize texture
void backpack_initialize_texture(){  
    SDL_Surface* useButtonSurface = SDL_LoadBMP("main_game_image/useButton.bmp");
    useButtonTexture = SDL_CreateTextureFromSurface(backpackRenderer, useButtonSurface);
    SDL_FreeSurface(useButtonSurface);
}


// click window
void backpack_mouseEvent(int steps){
    SDL_Event event;
    int running_inbackpack = 1;

    int descriptionAppear = 0;  // show item description
    int describeItem = 0; // item numbers

    SDL_StartTextInput();
    
    while (currentScreen == BACKPACK_SCREEN) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {                    // close window by the "X"
                running_inbackpack = 0;
                printf("pressed X, now what???\n");
                close_backpack();
                close();
                return;

            } else if (event.type == SDL_MOUSEBUTTONDOWN) {  // pressed mouse
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);

                if (mouse_is_above(mouseX, mouseY, returnButtonRect)) {   // click return
                    close_backpack();
                    // currentScreen = GAME_SCREEN;
                    descriptionAppear = 0;
                    running_inbackpack = 0;
                    return;

                } else if (mouse_is_above(mouseX, mouseY, toolRect)) {      // click tool
                    descriptionAppear = 1;
                    if (mouse_is_above(mouseX, mouseY, toolIconRects[0])) // click decreasing soda = 1         
                        describeItem = decreaseEffect;
                    else if (mouse_is_above(mouseX, mouseY, toolIconRects[1]))  // click increasing soda = 2             
                        describeItem = increaseEffect;    

                    else if (mouse_is_above(mouseX, mouseY, toolIconRects[2])) // click gamble Roulette = 3          
                        describeItem = gambleEffect;

                    else if (mouse_is_above(mouseX, mouseY, toolIconRects[3])) // click unknown soda = 4, 5, 6
                        describeItem = rand() % 2 + 4;
                } else if (mouse_is_above(mouseX, mouseY, useButtonRect)){
                    toolEffect(describeItem, currentPlayer);
                    close_backpack();
                }
            } 
        }  
        SDL_RenderClear(backpackRenderer);
        renderBackpackScreen();
        if (descriptionAppear == 1)
            renderItemDescription(describeItem);
        
        SDL_RenderPresent(backpackRenderer);
    }
}

// show backpack
void renderBackpackScreen() {
    if (backpackWindow == NULL) {
        initialize_backpack(); 
    }

    SDL_SetRenderDrawColor(backpackRenderer, 255, 255, 255, 255);
    SDL_RenderClear(backpackRenderer);

    if (backpackWindowTexture != NULL) {
        SDL_RenderCopy(backpackRenderer, backpackWindowTexture, NULL, &backpackWindowRect);
    } else {
        printf("backpackWindowTexture is NULL\n");
    }

    // show Tool text
    const char* tooltitleText = "Tool";
    renderTextCentered(backpackRenderer, font_tool, tooltitleText, textColor, toolTitleRect);

    // show Quanity text
    const char* tool_quanityText = "Quanity";   
    renderTextCentered(backpackRenderer, font_tool, tool_quanityText, textColor, tool_quanityRect);

    // show player's name
    const char* playerName = (currentPlayer == 0) ? player1.name : player2.name;
    char playerText[300];
    strcpy(playerText, playerName);
    strcat(playerText, "'s backpack");
    renderTextCentered(backpackRenderer, font_player, playerText, textColor, playerTitleRect);

    // show the number of tools
    char tool_textBuffer[4][10];
    char money_textBuffer[10];
    char gingerSoda_textBuffer[10];

    Player* currentPlayerPtr = (currentPlayer == 0) ? &player1 : &player2;

    snprintf(tool_textBuffer[0], sizeof(tool_textBuffer[0]), "%d", larger_than_0(currentPlayerPtr->numDecreaseSoda));
    snprintf(tool_textBuffer[1], sizeof(tool_textBuffer[1]), "%d", larger_than_0(currentPlayerPtr->numIncreaseSoda));
    snprintf(tool_textBuffer[2], sizeof(tool_textBuffer[2]), "%d", larger_than_0(currentPlayerPtr->numGambleRoulette));
    snprintf(tool_textBuffer[3], sizeof(tool_textBuffer[3]), "%d", larger_than_0(currentPlayerPtr->numUnknownSoda));

    snprintf(money_textBuffer, sizeof(money_textBuffer), "%d", currentPlayerPtr->money);
    snprintf(gingerSoda_textBuffer, sizeof(gingerSoda_textBuffer), "%d", currentPlayerPtr->ginger_soda);

    for (int i = 0; i < 4; i++) {
        renderTextCentered(backpackRenderer, font_player, tool_textBuffer[i], textColor, Text_numToolRect[i]);
    }

    // show the amount of money & ginger soda
    renderTextCentered(backpackRenderer, font_player, money_textBuffer, textColor, moneyPrintRect);
    renderTextCentered(backpackRenderer, font_player, gingerSoda_textBuffer, textColor, gingerSodaPrintRect);
}

// render Description of the item clicked
void renderItemDescription(int describeItem) { 
    // char TextDescription[1000];
    char* TextDescription = (char*)malloc(1000);
    // describeItem >= cannotMoveEffect ? cannotMoveEffect : describeItem;
    if(describeItem > increaseOwnMoneyEffect) describeItem = increaseOwnMoneyEffect;

    printf("describeItem = %d\n",describeItem);
    strcpy(TextDescription, toolDescriptions[describeItem - 1]);
    renderTextWrapped(backpackRenderer, font_player, TextDescription, textColor, &toolIllustrationRect); //�D�㻡��

    // Show Use Button
    backpack_initialize_texture();
    SDL_RenderCopy(backpackRenderer, useButtonTexture, NULL, &useButtonRect);
    renderTextCentered(backpackRenderer, font_player, "Use", textColor, useButtonRect);    
    free(TextDescription);
}

// use tool, and show effect
void toolEffect (int currentEffect, int currentPlayer){
    printf("toolEffect\n");
    if (currentPlayer == 0){
        switch (currentEffect){
            case decreaseEffect:
                player2.effect_steps = (player1.numDecreaseSoda > 0) ? -2 : 0;
                player1.numDecreaseSoda--;
                break;
            case increaseEffect:
                player1.effect_steps = (player1.numIncreaseSoda > 0) ? +2 : 0;
                player1.numIncreaseSoda--;
                break;
            case gambleEffect:
                if (player1.numGambleRoulette > 0 && !(rand() % 50))
                    player2.money = 0;
                    player1.numGambleRoulette--;
                break;
            case increaseOwnMoneyEffect:
                player1.money = (player1.numUnknownSoda > 0) ? (player1.money *= 1.5) : player1.money;
                player1.numUnknownSoda--;
                break;
            case increaseOppoMoneyEffect:
                player2.money = (player1.numUnknownSoda > 0) ? (player2.money *= 1.5) : player2.money;
                player1.numUnknownSoda--;
                break;
            default:
                break;
        }

    } else if (currentPlayer == 1){
        switch (currentEffect){
            case decreaseEffect:
                player1.effect_steps = (player2.numDecreaseSoda > 0) ? -2 : 0;
                player2.numDecreaseSoda--;
                break;
            case increaseEffect:
                player2.effect_steps = (player2.numIncreaseSoda > 0) ? +2 : 0;
                player2.numIncreaseSoda--;
                break;
            case gambleEffect:
                if (player2.numGambleRoulette > 0 && !(rand() % 50))
                    player2.money = 0;
                    player2.numGambleRoulette--;
                break;
            case increaseOwnMoneyEffect:
                player2.money = (player2.numUnknownSoda > 0) ? (player2.money *= 1.5) : player2.money;
                player2.numUnknownSoda--;
                break;
            case increaseOppoMoneyEffect:
                player1.money = (player2.numUnknownSoda > 0) ? (player1.money *= 1.5) : player1.money;
                player2.numUnknownSoda--;
                break;
            default:
                break;
        }

    }
}

// close backpack
int close_backpack() {
    currentScreen = GAME_SCREEN;
    printf("closing backpack\n");
    SDL_RenderClear(backpackRenderer);
    if (backpackRenderer) SDL_DestroyRenderer(backpackRenderer);
    if (backpackWindow) SDL_DestroyWindow(backpackWindow);
    // if (font_player) TTF_CloseFont(font_player);
    // if (font_tool) TTF_CloseFont(font_tool);

    printf("backpack closed\n");
 
    return 1;
}
