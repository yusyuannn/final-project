#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "ttt.h"
#include "dice_game.h"
#include "all_minigame.h"
#include "timer.h"
#include "game.h"

/*
INCLUDES = -I/usr/include/SDL2

LDFLAGS = -lSDL2 -lSDL2_ttf -lSDL2_image

g++ minigame.c ttt.cpp timer.cpp -o minigame -I/usr/include/SDL2 -lSDL2 -lSDL2_ttf -lSDL2_image
*/

void call_minigame(){
    srand(time(NULL));
    int num = rand() % 3;
    int game_result;
    switch(num){
        case 0:
            printf("Run dice game\n");
            run_dice_game();
            break;
        case 1:
            printf("Run ttt game\n");
            game_result = RunTTT();
            if(game_result == 1){
                if(currentPlayer == 0) player1.money += 300;
                else if(currentPlayer == 1) player2.money += 300;
            }else if(game_result == 2){
                if(currentPlayer == 0) player1.money -= 150;
                else if(currentPlayer == 1) player2.money -= 150;
            }
            break;

        case 2:
            printf("Run timer game\n");
            game_result = runTC();

                if(currentPlayer == 0) player1.money += game_result;
                else if(currentPlayer == 1) player2.money += game_result;

            break;
    }

}