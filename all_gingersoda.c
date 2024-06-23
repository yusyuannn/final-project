#include <stdio.h>
#include "ginger_soda.h"
#include "all_gingersoda.h"

/*
g++ -o all_gingersoda all_gingersoda.c ginger_soda.cpp -lSDL2 -lSDL2_ttf -lSDL2_image -pthread
*/
int all_gingersoda(int currentMoney, int price) {

    int result = call_gingersoda(price, currentMoney);

    if (result == 1) {
        printf("Yes!\n");
    } else if (result == 0) {
        printf("No!\n");
    } else {
        printf("error\n");
    }

    return result;
}