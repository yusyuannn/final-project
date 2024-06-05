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
    font = TTF_OpenFont("fonts/jf-openhuninn-2.0.ttf", 40);  // 可替換字體
    font_player = TTF_OpenFont("fonts/jf-openhuninn-2.0.ttf", 16); 
    font_tool= TTF_OpenFont("fonts/jf-openhuninn-2.0.ttf", 20); 
    
    initialize_menu();
    initialize_texture();
    initialize_map(MAP);
    initialize_player();

    return 1;
}

// 文字置中
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

// 文字自動換行
void renderTextWrapped(SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_Color color, SDL_Rect *rect) {
    char *textCopy = strdup(text);
    char *word;
    int lineWidth = 0;
    int lineHeight = TTF_FontHeight(font);
    int x = rect->x;
    int y = rect->y;
    char line[1024] = {0};
    word = strtok(textCopy, " ");
    while (word != NULL) {
        int wordWidth, wordHeight;
        TTF_SizeText(font, word, &wordWidth, &wordHeight);
        if (lineWidth + wordWidth > rect->w) {
            // 渲染目前文字
            SDL_Surface *surface = TTF_RenderText_Blended(font, line, color);
            SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_Rect dst = {x, y, surface->w, surface->h};
            SDL_RenderCopy(renderer, texture, NULL, &dst);
            SDL_FreeSurface(surface);
            SDL_DestroyTexture(texture);
            // 移至另一行
            y += lineHeight;
            line[0] = '\0';
            lineWidth = 0;
        }
        strcat(line, word);
        strcat(line, " ");
        lineWidth += wordWidth + TTF_FontLineSkip(font);
        word = strtok(NULL, " ");
    }
    // 渲染最後一行
    if (line[0] != '\0') {
        SDL_Surface *surface = TTF_RenderText_Blended(font, line, color);
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect dst = {x, y, surface->w, surface->h};
        SDL_RenderCopy(renderer, texture, NULL, &dst);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }
    free(textCopy);
}

// 初始化menu
void initialize_menu(){ 
    // 視窗、renderer
    window = SDL_CreateWindow("Ginger Soda", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(renderer, menuBackgrounColor.r, menuBackgrounColor.g, menuBackgrounColor.b, menuBackgrounColor.a);
    // 輸入框
    input_box1.rect = (SDL_Rect){760, 280, INPUT_BOX_WIDTH, INPUT_BOX_HEIGHT};
    input_box2.rect = (SDL_Rect){760, 330+INPUT_BOX_HEIGHT+10, INPUT_BOX_WIDTH, INPUT_BOX_HEIGHT};
    input_box3.rect = (SDL_Rect){760, 380+INPUT_BOX_HEIGHT*2+10, INPUT_BOX_WIDTH, INPUT_BOX_HEIGHT};
    input_box1.has_focus = false;
    input_box2.has_focus = false;
    input_box3.has_focus = false;
    input_box1.text[0] = '\0';
    input_box2.text[0] = '\0';
    input_box3.text[0] = '\0';
    SDL_Surface *input_box_surface = SDL_LoadBMP("images/inputBar.bmp");
    InputBoxTexture = SDL_CreateTextureFromSurface(renderer, input_box_surface);
    SDL_FreeSurface(input_box_surface);
}

// 載入圖片為texture
void initialize_texture(){  
    // MAIN_MENU    
    SDL_Surface* startButtonSurface = SDL_LoadBMP("images/start_button.bmp");
    SDL_Surface* quitButtonSurface = SDL_LoadBMP("images/quit_button.bmp");
    SDL_Surface* titleSurface = SDL_LoadBMP("images/title.bmp");
    // GAME_SCREEN   
    SDL_Surface* bagpack_iconSurface = SDL_LoadBMP("images/bagpack_icon.bmp");
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
    SDL_Surface* bagpackWindowSurface = SDL_LoadBMP("images/bagpack.bmp");
    // GAME_END_SCREEN
    SDL_Surface* bgSurface = SDL_LoadBMP("images/gameOverBG.bmp");

    // MAIN_MENU
    startButtonTexture = SDL_CreateTextureFromSurface(renderer, startButtonSurface);
    quitButtonTexture = SDL_CreateTextureFromSurface(renderer, quitButtonSurface);
    titleTexture = SDL_CreateTextureFromSurface(renderer, titleSurface);
    // GAME_SCREEN 
    bagpack_iconTexture = SDL_CreateTextureFromSurface(renderer, bagpack_iconSurface);
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
    bagpackWindowTexture = SDL_CreateTextureFromSurface(renderer, bagpackWindowSurface);
    // GAME_END_SCREEN
    bgTexture = SDL_CreateTextureFromSurface(renderer, bgSurface);

    // MAIN_MENU  
    SDL_FreeSurface(startButtonSurface);
    SDL_FreeSurface(quitButtonSurface);
    SDL_FreeSurface(titleSurface);
    // GAME_SCREEN 
    SDL_FreeSurface(bagpack_iconSurface);
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
    SDL_FreeSurface(bagpackWindowSurface);
    // GAME_END_SCREEN
    SDL_FreeSurface(bgSurface);
    
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

    int descriptionAppear = 0;  // 判斷道具說明字樣是否出現
    int currentTexture = 0; // 道具說明字樣

    SDL_StartTextInput();

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {                    // close window by the "X"
                running = 0;
            } else if (event.type == SDL_MOUSEMOTION) {      // move mouse
                int x, y;
                SDL_GetMouseState(&x, &y);
                input_box1.has_focus = SDL_PointInRect(&(SDL_Point){x, y}, &input_box1.rect);
                input_box2.has_focus = SDL_PointInRect(&(SDL_Point){x, y}, &input_box2.rect);
                input_box3.has_focus = SDL_PointInRect(&(SDL_Point){x, y}, &input_box3.rect);
                if (input_box1.has_focus || input_box2.has_focus || input_box3.has_focus) {
                    SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_IBEAM));
                } else {
                    SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW));
                }
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
                     if (mouse_is_above(mouseX, mouseY, returnButtonRect)) {   // press return
                        currentScreen = GAME_SCREEN;
                        descriptionAppear = 0; // 按下return時，道具解釋消失
                    } else if (mouse_is_above(mouseX, mouseY, toolRect)) {      // press tool
                        descriptionAppear = 1;
                        if (mouse_is_above(mouseX, mouseY, decreasingSodaRect)) // press decreasing soda = 1         
                            currentTexture = 1;     
                        else if (mouse_is_above(mouseX, mouseY, increasingSodaRect))  // press increasing soda = 2             
                            currentTexture = 2;                 
                        else if (mouse_is_above(mouseX, mouseY, gambleRouletteRect)) // press gamble Roulette = 3          
                            currentTexture = 3;
                        else if (mouse_is_above(mouseX, mouseY, unknownSodaRect)) // press unknown soda = 4
                            currentTexture = 4;       
                    }
                } else if (currentScreen == GAME_OVER_SCREEN) {                 
                    if (mouse_is_above(mouseX, mouseY, restartRect)) {          // press restart
                        game_round = 3;
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
            } else if (event.type == SDL_TEXTINPUT) {       // 處理字符輸入
                if (input_box1.has_focus) {         // 滑鼠游標位置在第一個輸入框，輸入的文字存為player2的名字
                    if (strlen(input_box1.text) + strlen(event.text.text) < sizeof(input_box1.text) - 1) {
                        size_t name_length = strlen(input_box1.text) + strlen(event.text.text) + 1;
                        char* new_name = malloc(name_length);
                        strcat(input_box1.text, event.text.text);
                        strcpy(new_name, input_box1.text);
                        player1.name = new_name;
                    }
                } else if (input_box2.has_focus) {  // 滑鼠游標位置在第二個輸入框，輸入的文字存為player1的名字
                    if (strlen(input_box2.text) + strlen(event.text.text) < sizeof(input_box2.text) - 1) {
                        size_t name_length = strlen(input_box2.text) + strlen(event.text.text) + 1;
                        char* new_name = malloc(name_length);
                        strcat(input_box2.text, event.text.text);
                        strcpy(new_name, input_box2.text);
                        player2.name = new_name;
                    }
                } else if (input_box3.has_focus) {  // 滑鼠游標位置在第三個輸入框，輸入的文字存為game round
                    if (strlen(input_box3.text) + strlen(event.text.text) < sizeof(input_box3.text) - 1) {
                        strcat(input_box3.text, event.text.text);
                        game_round = atoi(input_box3.text); 
                    }   
                }
            } else if (event.type == SDL_KEYDOWN) {     // 處理鍵盤按鍵
                if (input_box1.has_focus) {
                    if (event.key.keysym.sym == SDLK_BACKSPACE && strlen(input_box1.text) > 0) {
                        input_box1.text[strlen(input_box1.text) - 1] = '\0';
                    }
                } else if (input_box2.has_focus) {
                    if (event.key.keysym.sym == SDLK_BACKSPACE && strlen(input_box2.text) > 0) {
                        input_box2.text[strlen(input_box2.text) - 1] = '\0';
                    }
                } else if (input_box3.has_focus) {
                    if (event.key.keysym.sym == SDLK_BACKSPACE && strlen(input_box3.text) > 0) {
                        input_box3.text[strlen(input_box3.text) - 1] = '\0';
                    }
                }
            }
        }

        // 鍵盤游標閃爍
        if (SDL_GetTicks() - cursor_last_time >= CURSOR_BLINK_INTERVAL) {
            show_cursor = !show_cursor;
            cursor_last_time = SDL_GetTicks();
        }

        SDL_RenderClear(renderer);
        if (currentScreen == MAIN_MENU) {
            renderMenu();
        } else if (currentScreen == GAME_SCREEN) {
            renderGameScreen(MAP, steps);
        } else if (currentScreen == BAGPACK_SCREEN) {
            renderBagpackScreen();
            if (descriptionAppear == 1)
                renderItemDescription(currentTexture);
        } else if (currentScreen == GAME_OVER_SCREEN) {
            renderGameScreen(MAP, steps);
            renderGameOverScreen();
        }
        SDL_RenderPresent(renderer);
    }
    SDL_StopTextInput();
}

void renderMenu() {
    // start
    SDL_RenderCopy(renderer, startButtonTexture, NULL, &startButtonRect);
    // exit
    SDL_RenderCopy(renderer, quitButtonTexture, NULL, &quitButtonRect);
    // title
    SDL_RenderCopy(renderer, titleTexture, NULL, &titleRect);
    // 輸入玩家名字
    SDL_Texture* hintTexture = renderText("Enter player1's name", textColor);
    SDL_Rect hintRect = {760, 230, 300, 40};
    SDL_RenderCopy(renderer, hintTexture, NULL, &hintRect);
    draw_input_box(renderer, font, &input_box1, show_cursor);
    hintTexture = renderText("Enter player2's name ", textColor);
    hintRect.y = 340;
    SDL_RenderCopy(renderer, hintTexture, NULL, &hintRect);
    draw_input_box(renderer, font, &input_box2, show_cursor);
    hintTexture = renderText("Enter game rounds   ", textColor);
    hintRect.y = 440;
    SDL_RenderCopy(renderer, hintTexture, NULL, &hintRect);
    draw_input_box(renderer, font, &input_box3, show_cursor);

}

void draw_input_box(SDL_Renderer *renderer, TTF_Font *font, InputBox *input_box, bool show_cursor) {
    // 輸入框
    SDL_RenderCopy(renderer, InputBoxTexture, NULL, &input_box->rect);
    // 顯示文字
    SDL_Color textColor = {0, 0, 0, 255};  // 文字顏色
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, input_box->text, textColor);
    if (textSurface) {
        SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        if (textTexture) {
            SDL_Rect textRect = {input_box->rect.x + 5, input_box->rect.y + (INPUT_BOX_HEIGHT - textSurface->h) / 2, textSurface->w, textSurface->h};
            SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
            SDL_DestroyTexture(textTexture);
        }
        SDL_FreeSurface(textSurface);
    }
    // 鍵盤游標
    if (show_cursor && input_box->has_focus) {
        int text_width;
        TTF_SizeText(font, input_box->text, &text_width, NULL); // 獲取文字大小
        int cursor_x = input_box->rect.x + 5 + text_width + 2;  // 計算鍵盤游標位置
        Uint8 r, g, b, a;
        SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);       // 獲取背景顏色
        SDL_SetRenderDrawColor(renderer, 255 - r, 255 - g, 255 - b, a);  // 設置鍵盤游標顏色為背景的相反色
        SDL_RenderDrawLine(renderer, cursor_x, input_box->rect.y + 5, cursor_x, input_box->rect.y + INPUT_BOX_HEIGHT - 5);
        SDL_SetRenderDrawColor(renderer, r, g, b, a);  // 恢復原来的背景顏色
    }
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
    SDL_RenderCopy(renderer, bagpack_iconTexture, NULL, &bagpackRect);
    
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
    // 一
    SDL_Surface* player1Surface = TTF_RenderText_Solid(font, player1.name, textColor);
    SDL_Texture* player1Texture = SDL_CreateTextureFromSurface(renderer, player1Surface);
    SDL_Rect player1nameRect = {220, 290, player1Surface->w, player1Surface->h};
    SDL_RenderCopy(renderer, player1Texture, NULL, &player1nameRect);
    SDL_Rect player1Rect = {160, 290, 50, 50};
    SDL_RenderCopy(renderer, player[0], NULL, &player1Rect);
    // 二
    SDL_Surface* player2Surface = TTF_RenderText_Solid(font, player2.name, textColor);
    SDL_Texture* player2Texture = SDL_CreateTextureFromSurface(renderer, player2Surface);
    SDL_Rect player2nameRect = {220, 360, player2Surface->w, player2Surface->h};
    SDL_RenderCopy(renderer, player2Texture, NULL, &player2nameRect);
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

void renderBagpackScreen() {// 顯示背包    
    SDL_RenderCopy(renderer, bagpackWindowTexture, NULL, &bagpackWindowRect);
    // 顯示文字
    // 顯示Tool字樣
    const char* tooltitleText = "Tool";
    renderTextCentered(renderer, font_tool, tooltitleText, textColor, toolTitleRect);
    // 顯示Quanity字樣
    const char* tool_quanityText = "Quanity";   
    renderTextCentered(renderer, font_tool, tool_quanityText, textColor, tool_quanityRect);

    // 顯示目前玩家
    const char* playerName = (currentPlayer == 0) ? player1.name : player2.name;
    char playerText[300];
    strcpy(playerText, playerName);
    strcat(playerText, "'s Bagpack");

    renderTextCentered(renderer, font_player, playerText, textColor, playerTitleRect);
    
    // 道具 & money & ginger soda數量
    char tool_textBuffer[4][10]; //字符儲存的數量與大小
    char money_textBuffer[10];
    char gingerSoda_textBuffer[10];
    if (currentPlayer == 0) { // player 1
        snprintf(tool_textBuffer[0], sizeof(tool_textBuffer[0]), "%d", player1.numDecreaseSoda);
        snprintf(tool_textBuffer[1], sizeof(tool_textBuffer[1]), "%d", player1.numIncreaseSoda);
        snprintf(tool_textBuffer[2], sizeof(tool_textBuffer[2]), "%d", player1.numGambleRoulette);
        snprintf(tool_textBuffer[3], sizeof(tool_textBuffer[3]), "%d", player1.numUnknownSoda);

        snprintf(money_textBuffer, sizeof(money_textBuffer), "%d", player1.money);
        snprintf(gingerSoda_textBuffer, sizeof(gingerSoda_textBuffer), "%d", player1.ginger_soda);
    } else { // player 2
        snprintf(tool_textBuffer[0], sizeof(tool_textBuffer[0]), "%d", player2.numDecreaseSoda);
        snprintf(tool_textBuffer[1], sizeof(tool_textBuffer[1]), "%d", player2.numIncreaseSoda);
        snprintf(tool_textBuffer[2], sizeof(tool_textBuffer[2]), "%d", player2.numGambleRoulette);
        snprintf(tool_textBuffer[3], sizeof(tool_textBuffer[3]), "%d", player2.numUnknownSoda);

        snprintf(money_textBuffer, sizeof(money_textBuffer), "%d", player2.money);
        snprintf(gingerSoda_textBuffer, sizeof(gingerSoda_textBuffer), "%d", player2.ginger_soda);
    }

    // 顯示道具數量
    for (int i = 0; i < 4; i++) {
        renderTextCentered(renderer, font, tool_textBuffer[i], textColor, Text_numToolRect[i]);
    }
    // 顯示money & ginger soda數量
    renderTextCentered(renderer, font, money_textBuffer, textColor, moneyPrintRect);
    renderTextCentered(renderer, font, gingerSoda_textBuffer, textColor, gingerSodaPrintRect);
}

void renderItemDescription(int currentTexture) { // 顯示道具說明字樣
    char toolText[1000];
    strcpy(toolText, tool[currentTexture - 1]);
    renderTextWrapped(renderer, font, toolText, textColor, &toolIllustrationRect); //道具標題(名稱)
    char TextDescription[1000];
    strcpy(TextDescription, toolDescriptions[currentTexture - 1]);
    renderTextWrapped(renderer, font, TextDescription, textColor, &toolIllustrationRect); //道具說明
}

void renderGameOverScreen() {
    SDL_RenderCopy(renderer, bgTexture, NULL, &bgRect);
    // 玩家資訊
    char playerInfo[100];
    SDL_Surface* playerSurface;
    SDL_Texture* playerTexture;
    // 一
    playerSurface = TTF_RenderText_Solid(font, player1.name, textColor);    // name
    playerTexture = SDL_CreateTextureFromSurface(renderer, playerSurface);
    SDL_Rect player1Rect = {(SCREEN_WIDTH-playerSurface->w)/2, 200, playerSurface->w, playerSurface->h};
    SDL_RenderCopy(renderer, playerTexture, NULL, &player1Rect);
    snprintf(playerInfo, sizeof(playerInfo), "Money: %d, Ginger Soda: %d", player1.money, player1.ginger_soda);
    playerSurface = TTF_RenderText_Solid(font, playerInfo, textColor);    // info
    playerTexture = SDL_CreateTextureFromSurface(renderer, playerSurface);
    SDL_Rect player1infoRect = {(SCREEN_WIDTH-playerSurface->w)/2, 250, playerSurface->w, playerSurface->h};
    SDL_RenderCopy(renderer, playerTexture, NULL, &player1infoRect);
    // 二
    playerSurface = TTF_RenderText_Solid(font, player2.name, textColor);    // name
    playerTexture = SDL_CreateTextureFromSurface(renderer, playerSurface);
    SDL_Rect player2Rect = {(SCREEN_WIDTH-playerSurface->w)/2, 300, playerSurface->w, playerSurface->h};
    SDL_RenderCopy(renderer, playerTexture, NULL, &player2Rect);
    snprintf(playerInfo, sizeof(playerInfo), "Money: %d, Ginger Soda: %d", player2.money, player2.ginger_soda);
    playerSurface = TTF_RenderText_Solid(font, playerInfo, textColor);    // info
    playerTexture = SDL_CreateTextureFromSurface(renderer, playerSurface);
    SDL_Rect player2infoRect = {(SCREEN_WIDTH-playerSurface->w)/2, 350, playerSurface->w, playerSurface->h};
    SDL_RenderCopy(renderer, playerTexture, NULL, &player2infoRect);
    // button
    SDL_Surface* buttonSurface;
    SDL_Texture* buttonTexture;
    // restart
    buttonSurface = TTF_RenderText_Solid(font, "RESTART", textColor);
    buttonTexture = SDL_CreateTextureFromSurface(renderer, buttonSurface);
    SDL_RenderCopy(renderer, buttonTexture, NULL, &restartRect);
    // exit
    buttonSurface = TTF_RenderText_Solid(font, "EXIT", textColor);
    buttonTexture = SDL_CreateTextureFromSurface(renderer, buttonSurface);
    SDL_RenderCopy(renderer, buttonTexture, NULL, &exitRect);
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
        printf("%s\n", player1.name);
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
    if (font_player) TTF_CloseFont(font);
    if (font_tool) TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();
    return 1;
}
