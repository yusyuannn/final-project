CC = gcc

CFLAGS = -Wall -Wextra -pedantic -std=c99 -Wno-strict-prototypes

LDFLAGS = -lSDL2

SOURCES = game.c

TARGET = game

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES) $(LDFLAGS)


clean:
	rm -f $(TARGET)
