#include <stdio.h>
#include "ttt.h"
#include "timer.h"

/*

g++ minigame.c ttt.cpp timer.cpp -o minigame -I/usr/include/SDL2 -lSDL2 -lSDL2_ttf -lSDL2_image
*/

int main(){

    RunTTT();
    runTC();

}