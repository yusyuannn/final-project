CC = gcc
PP = g++
INCLUDES = -I src/include
LDFLAGS = -L src/lib
LIBS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_mixer -lSDL2_image -lSDL2_image# -lSDL2_image

EVENT_SOURCE = all_event.c event1.cpp event2.cpp event3.cpp event4.cpp event5.cpp event6.cpp event7.cpp event8.cpp

MINIGAME_SOURCE = all_minigame.c dice_game.c ttt.cpp timer.cpp

GS_SOURCE = all_gingersoda.c ginger_soda.cpp

GAME_SOURCE = $(EVENT_SOURCE) $(MINIGAME_SOURCE) $(GS_SOURCE) backpack.c shop.c 
# GAME_SOURCE = backpack.c shop.c dice_game.c

game: game.c $(GAME_SOURCE)
	$(PP) $^ -o $@ $(INCLUDES) $(LIBS) $(LDFLAGS)

all_events: $(EVENT_SOURCE)
	$(PP) $(EVENT_SOURCE) -o all_events $(INCLUDES) $(LDFLAGS)

backpack: backpack.c
	$(PP) $< -o $@ $(INCLUDES) $(LDFLAGS) $(LIBS)

dice_game: dice_game.c
	$(PP) $< -o $@ $(INCLUDES) $(LDFLAGS) $(LIBS)

shop: shop.c
	$(PP) $< -o $@ $(INCLUDES) $(LDFLAGS) $(LIBS)

tictactoe: tictactoe.cpp
	$(PP) tictactoe.cpp -o tictactoe $(INCLUDES) $(LDFLAGS)

runTC: runTC.cpp
	$(PP) runTC.cpp -o runTC $(INCLUDES) $(LDFLAGS)


event1: event1.cpp
	$(PP) event1.cpp -o event1 $(INCLUDES) $(LDFLAGS)

event2: event2.cpp
	$(PP) event2.cpp -o event2 $(INCLUDES) $(LDFLAGS)

event3: event3.cpp
	$(PP) event3.cpp -o event3 $(INCLUDES) $(LDFLAGS)

event4: event4.cpp
	$(PP) event4.cpp -o event4 $(INCLUDES) $(LDFLAGS)

event5: event5.cpp
	$(PP) event5.cpp -o event5 $(INCLUDES) $(LDFLAGS)

event6: event6.cpp
	$(PP) event6.cpp -o event6 $(INCLUDES) $(LDFLAGS)

event7: event7.cpp
	$(PP) event7.cpp -o event7 $(INCLUDES) $(LDFLAGS)

event8: event8.cpp
	$(PP) event8.cpp -o event8 $(INCLUDES) $(LDFLAGS)
# # # make:
# # # CC = gcc
# # # PP = g++

# # # LDFLAGS = -lSDL2 -lSDL2_ttf -lSDL2_image

# # # INCLUDES = -I/usr/include/SDL2 -L/usr/lib/x86_64-linux-gnu

# # CC = gcc
# # PP = g++

# # CFLAG = -Wall

# # INCLUDES = -I src/include

# # LDFLAGS = -lSDL2 -lSDL2_ttf -lmingw32 -lSDL2main # -lSDL2_image

# # EVENT_SOURCE = all_event.c event1.cpp event2.cpp event3.cpp event4.cpp event5.cpp event6.cpp event7.cpp event8.cpp

# # MINIGAME_SOURCE = all_minigame.c dice_game.c ttt.cpp timer.cpp

# # GAME_SOURCE = $(MINIGAME_SOURCE) backpack.c shop.c

# # game: game.c $(GAME_SOURCE)
# # 	$(PP) game.c $(GAME_SOURCE) -o game $(LDFLAGS) $(INCLUDES)

# # all_events: $(EVENT_SOURCE)
# # 	$(PP) $(EVENT_SOURCE) -o all_events $(INCLUDES) $(LDFLAGS)
# # # g++ all_events.cpp event1.cpp event2.cpp event3.cpp event4.cpp event5.cpp event6.cpp event7.cpp event8.cpp -o all_events -I/usr/include/SDL2 -L/usr/lib/x86_64-linux-gnu -lSDL2 -lSDL2_ttf -lSDL2_image

# # dice_game: dice_game.c
# # 	$(PP) dice_game.c -o dice_game -Wall $(CFLAG) $(LDFLAGS) $(INCLUDES)

# # event1: event1.cpp
# # 	$(PP) event1.cpp -o event1 $(INCLUDES) $(LDFLAGS)

# # event2: event2.cpp
# # 	$(PP) event2.cpp -o event2 $(INCLUDES) $(LDFLAGS)

# # event3: event3.cpp
# # 	$(PP) event3.cpp -o event3 $(INCLUDES) $(LDFLAGS)

# # event4: event4.cpp
# # 	$(PP) event4.cpp -o event4 $(INCLUDES) $(LDFLAGS)

# # event5: event5.cpp
# # 	$(PP) event5.cpp -o event5 $(INCLUDES) $(LDFLAGS)

# # event6: event6.cpp
# # 	$(PP) event6.cpp -o event6 $(INCLUDES) $(LDFLAGS)

# # event7: event7.cpp
# # 	$(PP) event7.cpp -o event7 $(INCLUDES) $(LDFLAGS)

# # event8: event8.cpp
# # 	$(PP) event8.cpp -o event8 $(INCLUDES) $(LDFLAGS)

# # toegame: toegame.cpp
# # 	$(PP) toegame.cpp -o toegame $(INCLUDES) $(LDFLAGS)

# # minigame_Timer_Challenge: minigame_Timer_Challenge.cpp
# # 	$(PP) minigame_Timer_Challenge.cpp -o minigame_Timer_Challenge $(INCLUDES) $(LDFLAGS)

# # tictactoe: tictactoe.cpp
# # 	$(PP) tictactoe.cpp -o tictactoe $(INCLUDES) $(LDFLAGS)

# # runTC: runTC.cpp
# # 	$(PP) runTC.cpp -o runTC $(INCLUDES) $(LDFLAGS)

# # backpack: backpack.c
# # 	$(PP) backpack.c -o backpack $(INCLUDES) $(LDFLAGS)


# # # 	g++ game2.cpp -o game2 -I/usr/include/SDL2 -I/usr/include/SDL2 -L/usr/lib/x86_64-linux-gnu -lSDL2 -lSDL2_ttf -lSDL2_image
# # #   g++ toegame.cpp -o toegame -I/usr/include/SDL2 -I/usr/include/SDL2 -L/usr/lib/x86_64-linux-gnu -lSDL2 -lSDL2_ttf -lSDL2_image
# # # 	g++ main.cpp event1.cpp event2.cpp -o main -I/usr/include/SDL2 -L/usr/lib/x86_64-linux-gnu -lSDL2 -lSDL2_ttf -lSDL2_image
