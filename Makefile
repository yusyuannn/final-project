CC = gcc

CFLAGS = -Wall -Wextra -pedantic -std=c99 -Wno-strict-prototypes

LDFLAGS = -lSDL2 -lSDL2_ttf

SOURCES = game.c

TARGET = game

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES) $(LDFLAGS)

clean:
	rm -f $(TARGET)
