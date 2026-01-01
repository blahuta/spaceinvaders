CC = gcc

CFLAGS = -std=c17 -Wall -Wextra -Wunused-parameter

SRC = main.c player.c enemy.c bunker.c projectile.c text.c ui.c

SDL_FLAGS = $(shell pkg-config --cflags --libs sdl3 sdl3-ttf)


TARGET = main

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(SDL_FLAGS)

clean:
	rm -f $(TARGET)