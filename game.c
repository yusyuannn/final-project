#include "game.h"
#include "game_def.h"
// #include "dice_game.h"
#include "all_events.h"
#include "all_minigame.h"
#include "all_gingersoda.h"
#include "shop.h"
#include "backpack.h"
// #include <thread>

// #include "ginger_soda.h"


// gcc -Isrc/Include -Lsrc/lib -o game game.c -lmingw32 -lSDL2main -lSDL2
Mix_Music* gMusic;
Mix_Music* evMusic;


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


    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
    {
        printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
        return 1;
    }
    
    gMusic = NULL;
    gMusic = Mix_LoadMUS( "audio/maingameBGM.mp3" );
    evMusic = NULL;
    evMusic = Mix_LoadMUS( "audio/eventBGM.mp3" );

    Mix_PlayMusic( gMusic, -1 );


    initialize_menu();
    initialize_texture();
    initialize_map(MAP);
    initialize_player();

    return 1;
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
    SDL_Surface *input_box_surface = SDL_LoadBMP("main_game_image/inputBar.bmp");
    InputBoxTexture = SDL_CreateTextureFromSurface(renderer, input_box_surface);
    SDL_FreeSurface(input_box_surface);
}

// 載入圖片為texture
void initialize_texture(){  
    // MAIN_MENU    
    SDL_Surface* startButtonSurface = SDL_LoadBMP("main_game_image/start_button.bmp");
    SDL_Surface* quitButtonSurface = SDL_LoadBMP("main_game_image/quit_button.bmp");
    SDL_Surface* titleSurface = SDL_LoadBMP("main_game_image/title.bmp");
    // GAME_SCREEN   
    SDL_Surface* bagpack_iconSurface = SDL_LoadBMP("main_game_image/backpack_icon.bmp");
    SDL_Surface* homepageSurface = SDL_LoadBMP("main_game_image/homepage.bmp");
    SDL_Surface* gameTileSurface = SDL_LoadBMP("main_game_image/tile_game.bmp");
    SDL_Surface* chanceTileSurface = SDL_LoadBMP("main_game_image/tile_chance.bmp");
    SDL_Surface* normalTileSurface = SDL_LoadBMP("main_game_image/tile_normal.bmp");
    SDL_Surface* sodaTileSurface = SDL_LoadBMP("main_game_image/tile_soda.bmp");
    SDL_Surface* startTileSurface = SDL_LoadBMP("main_game_image/tile_start.bmp");
    SDL_Surface* storeTileSurface = SDL_LoadBMP("main_game_image/tile_store.bmp");
    SDL_Surface* player1Surface = SDL_LoadBMP("main_game_image/circle_black.bmp");
    SDL_Surface* player2Surface = SDL_LoadBMP("main_game_image/circle_white.bmp");
    // GAME_END_SCREEN
    SDL_Surface* bgSurface = SDL_LoadBMP("main_game_image/gameOverBG.bmp");

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
    // GAME_END_SCREEN
    SDL_FreeSurface(bgSurface);
    
    // DICE
    for (int i = 0; i < 6; ++i) {
        char filename[100];
        snprintf(filename, sizeof(filename), "main_game_image/dice_%d.bmp", i + 1);
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
    player1.numDecreaseSoda = 5;
    player2.numDecreaseSoda = 5;
    player1.numIncreaseSoda = 5;
    player2.numIncreaseSoda = 5;
    player1.numGambleRoulette = 5;
    player2.numGambleRoulette = 5;
    player1.numUnknownSoda = 5;
    player2.numUnknownSoda = 5;
    currentPlayer = 0;
    player1.dir = RIGHT;
    player2.dir = RIGHT;
    player1.effect_steps = 0;
    player2.effect_steps = 0;
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
    currentScreen = MAIN_MENU;
    int steps = 1;

    SDL_StartTextInput();

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {                    // close window by the "X"
                running = 0;
            } else if (event.type == SDL_MOUSEMOTION) {      // move mouse
                int x, y;
                SDL_GetMouseState(&x, &y);
                SDL_Point curr_pos = {x, y};
                input_box1.has_focus = SDL_PointInRect(&curr_pos, &input_box1.rect);
                input_box2.has_focus = SDL_PointInRect(&curr_pos, &input_box2.rect);
                input_box3.has_focus = SDL_PointInRect(&curr_pos, &input_box3.rect);
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
                        currentScreen = BACKPACK_SCREEN;
                        backpack_main(steps);
                        printf("left backpack\n");

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
                        char* new_name =(char*) malloc(name_length);
                        strcat(input_box1.text, event.text.text);
                        strcpy(new_name, input_box1.text);
                        player1.name = new_name;
                    }
                } else if (input_box2.has_focus) {  // 滑鼠游標位置在第二個輸入框，輸入的文字存為player1的名字
                    if (strlen(input_box2.text) + strlen(event.text.text) < sizeof(input_box2.text) - 1) {
                        size_t name_length = strlen(input_box2.text) + strlen(event.text.text) + 1;
                        char* new_name = (char*)malloc(name_length);
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
    int step_effect_inMap = 0;
    
    if (currentPlayer == 0) {
        position = player1.position;
        direction = &player1.dir;
        step_effect_inMap = player1.effect_steps;
        player1.effect_steps = 0;
    } else {
        position = player2.position;
        direction = &player2.dir;
        step_effect_inMap = player2.effect_steps;
        player2.effect_steps = 0;
        game_round--;
    }

    steps += step_effect_inMap;

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
                        // shop_main();

            break;
        case SQUARE_chance:
            Mix_PausedMusic();
            Mix_PlayMusic(evMusic, -1);

            printf("enter event\n");
            call_events();

            Mix_PausedMusic();
            Mix_PlayMusic(gMusic, -1);
            break;
        case SQUARE_game:
            Mix_PausedMusic();
            Mix_PlayMusic(evMusic, -1);

            printf("enter game\n");
            call_minigame();

            Mix_PausedMusic();
            Mix_PlayMusic(gMusic, -1);
            break;
        case SQUARE_store:
            printf("enter store\n");
            shop_main();
            printf("left store\n");
            break;
        case SQUARE_ginger_soda:
            printf("sell ginger soda\n");
            int gingersoda_price = 100;
            if(currentPlayer == 0){
                if(all_gingersoda(player1.money, gingersoda_price))
                    player1.ginger_soda++;
                    player1.money -= gingersoda_price;
            }else if(currentPlayer == 1){
                if(all_gingersoda(player2.money, gingersoda_price))
                    player2.ginger_soda++;
                    player2.money -= gingersoda_price;
            }

            int randomSquare = 4 + rand() % (32 - 4);
            int temp = MAP[randomSquare];
            MAP[randomSquare] = MAP[index];
            MAP[index] = temp;
            break;
    }
}

// 從格子位置得知格子類型
int getSquareTypeFromPosition(int x, int y) {
    for (int i = 0; i < 32; ++i) {
        int squareX = mapRect[i][0];
        int squareY = mapRect[i][1];
        if (x >= squareX && x < squareX + 80 && y >= squareY && y < squareY + 80) {
            return i;
        }
    }
    return -1;
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

void GAME_end(){
    if(!close()){
        printf("game close failed\n");
    }
}

// 釋放資源
int close() {
    // if (newRenderer) SDL_DestroyRenderer(newRenderer);
    // if (newWindow) SDL_DestroyWindow(newWindow);
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    if (font) TTF_CloseFont(font);
    if (font_player) TTF_CloseFont(font);
    if (font_tool) TTF_CloseFont(font);

    Mix_FreeMusic( gMusic );
    Mix_HaltMusic();

    TTF_Quit();
    SDL_Quit();
    Mix_Quit();

    return 1;
}
