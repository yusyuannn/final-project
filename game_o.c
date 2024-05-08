#include <stdio.h>
#include <SDL2/SDL.h>
// #include "game.h"
#define player_num 2

typedef struct {
    char* name;  // 玩家名字，他可以自己打
    int position;   // 玩家位置，0~31格
    int money;    // 玩家的前，他說一定要用到float，看要不要用這個之類的
    int ginger_soda;
    int effect; // 被道具影響的效果，看有沒有需要
} Player;

typedef enum {
    SQUARE_Normal,
    SQUARE_chance,
    SQUARE_game,
    SQUARE_Store,
    SQUARE_ginger_soda,
} Square_type;

typedef enum {
    GAME_Minesweeper,
    GAME_Dice_number,
    GAME_Timer_Challenge,
    GAME_Flip_Card,
    GAME_Coin_catcher
} game_square_type;


int main(int argc, char** argv){

    /*
    應該會有一堆SDL的window, renderer, texture之類的 吧
    */
    Player player[player_num];
    int MAP[32];    // 存地圖格子用的陣列，每個element是Square_type的一種
    int game_round = 20; // 行動回數，像是設定總共走幾次遊戲結束這樣

    if(!GAME_init()){
        printf("game initialization failed");
    }

    GAME_start_menu();

    while(game_round > 0){
        printf("%d rounds left\n", game_round);
        for(int i = 0; i < player_num; i++){
            roll_dice();    // 玩家骰骰子，
            square_event(); // 根據玩家走到的格子決定事件並發生
        }
        game_round --;
    }

    GAME_end();

}

int GAME_init(){
/*
初始化遊戲，成功回傳1、失敗回傳0
初始化包含: 地圖生成、玩家資料初始化、render地圖跟玩家、背包資料初始化(如果需要的話)
*/
    initialize_map();
    initialize_player();
    
    render_map_and_player();
}
void GAME_start_menu(){

}
void GAME_end(){

}

void initialize_map(){
    // 地圖map[32]裡用rand隨機生成格子0~4(看上次ppt裡寫的數量分配)
}
void initialize_player(){
    // 玩家資料清乾淨，然後給玩家輸入名字
    char* buffer = (char*)malloc(21*sizeof(char));
    scanf("%20s", buffer);
    // 然後給玩家這個名字這樣
}
void render_map_and_player(){
    // 用SDL render東西這樣
}

int roll_dice(){
/*
骰骰子，玩家前進骰到的步數
*/
}

void square_event(){
    int player_position = 0;    // 暫時拿來看的整數，到時候從struct(player)裡抓，應該要是0~31

    switch (player_position)
    {
    case SQUARE_Normal:
        // 玩家拿錢
        break;

    case SQUARE_chance:
        // 進機會格的event
        break;

    case SQUARE_game:
        // 用rand跑出0~5看要進什麼遊戲
        int the_game = 0;
        switch (the_game)
        {
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
        break;

    case SQUARE_Store:
        // 進商店
        store();
        break;

    case SQUARE_ginger_soda:
        // 進薑汁汽水販賣機
        ginger_soda();
    
    default:
        break;
    }
    
}

void minigame_minesweeper(){

}
void minigame_Dice_number(){

}
void minigame_Timer_Challenge(){

}
void minigame_Flip_Card(){

}
void minigame_Coin_catcher(){

}
void store(){

}
void ginger_soda(){

}