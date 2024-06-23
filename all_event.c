#include "event1.h"
#include "event2.h"
#include "event3.h"
#include "event4.h"
#include "event5.h"
#include "event6.h"
#include "event7.h"
#include "event8.h"
#include "all_events.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

/*
g++ all_event.c event1.cpp event2.cpp event3.cpp event4.cpp event5.cpp event6.cpp event7.cpp event8.cpp -o all_events -I/usr/include/SDL2 -L/usr/lib/x86_64-linux-gnu -lSDL2 -lSDL2_ttf -lSDL2_image
*/

// int main() {
//     call_events();
//     return 0;
// }

void call_events(){
    srand(time(NULL));
    int num = 1 + rand() % event_num;

    printf("event %d\n", num);
    switch (num) {
    case 1:
        runEvent1();
        break;
    case 2:
        runEvent2();
        break;
    case 3:
        runEvent3();
        break;
    case 4:
        runEvent4();
        break;
    case 5:
        runEvent5();
        break;
    case 6:
        runEvent6();
        break;
    case 7:
        runEvent7();
        break;
    case 8:
        runEvent8();
        break;
    }

}