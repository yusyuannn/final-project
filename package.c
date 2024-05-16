#include <SDL.h>
#include <SDL_image.h> // For loading images

#include <stdio.h>

#include "item_boundary.h"

// window size
#define WIDTH 640  // 1280
#define HEIGHT 360 // 720

// button place in map
#define map_bag_button_LEFT 530
#define map_bag_button_RIGHT 630
#define map_bag_button_UP 15
#define map_bag_button_DOWN 60

int open_bag();

int main(int argc, char *argv[])
{

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return 1;
    }

    // Create a window_map
    SDL_Window *window_map = SDL_CreateWindow("Map", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);

    if (!window_map)
    {
        SDL_Log("Failed to create window_map: %s", SDL_GetError());
        return 1;
    }

    // Create a renderer
    SDL_Renderer *renderer_map = SDL_CreateRenderer(window_map, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer_map)
    {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        return 1;
    }

    // Load image
    SDL_Surface *imageSurface = IMG_Load("map.png"); // Change "map.png" to your image file
    if (!imageSurface)
    {
        SDL_Log("Failed to load image: %s", SDL_GetError());
        return 1;
    }

    // Convert surface to texture
    SDL_Texture *imageTexture = SDL_CreateTextureFromSurface(renderer_map, imageSurface);
    SDL_FreeSurface(imageSurface);
    if (!imageTexture)
    {
        SDL_Log("Failed to create texture: %s", SDL_GetError());
        return 1;
    }

    // Main loop
    SDL_Event event;
    int quit = 0;
    while (!quit)
    {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT)
                { // Check for left mouse button click
                    Sint32 x = event.button.x;
                    Sint32 y = event.button.y;
                    // printf("Mouse clicked at (%d, %d)\n", x, y);
                    if ((map_bag_button_LEFT <= x && x <= map_bag_button_RIGHT) && (map_bag_button_UP <= y && y <= map_bag_button_DOWN))
                    {
                        // Create a window_bag
                        open_bag();
                    }
                }
                break;
            }
        }

        // Draw the current image
        SDL_RenderClear(renderer_map);
        SDL_RenderCopy(renderer_map, imageTexture, NULL, NULL);
        SDL_RenderPresent(renderer_map);
    }

    // Clean up
    SDL_DestroyTexture(imageTexture);
    SDL_DestroyRenderer(renderer_map);
    SDL_DestroyWindow(window_map);
    SDL_Quit();

    return 0;
}

int open_bag()
{
    // Create a window_bag
    SDL_Window *window_bag = SDL_CreateWindow("player's bag", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
    if (!window_bag)
    {
        SDL_Log("Failed to create window_bag: %s", SDL_GetError());
        return 1;
    }
    // Create a renderer
    SDL_Renderer *renderer_bag = SDL_CreateRenderer(window_bag, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer_bag)
    {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        return 1;
    }

    // Load image
    SDL_Surface *imageSurface = IMG_Load("bag.png"); // Change "map.png" to your image file
    if (!imageSurface)
    {
        SDL_Log("Failed to load image: %s", SDL_GetError());
        return 1;
    }

    // Convert surface to texture
    SDL_Texture *imageTexture = SDL_CreateTextureFromSurface(renderer_bag, imageSurface);
    SDL_FreeSurface(imageSurface);
    if (!imageTexture)
    {
        SDL_Log("Failed to create texture: %s", SDL_GetError());
        return 1;
    }

    // Draw the current image
    SDL_RenderClear(renderer_bag);
    SDL_RenderCopy(renderer_bag, imageTexture, NULL, NULL);
    SDL_RenderPresent(renderer_bag);

    SDL_Event event;
    int quit = 0;
    while (quit == 0)
    {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT)
                { // Check for left mouse button click
                    Sint32 x = event.button.x;
                    Sint32 y = event.button.y;
                    printf("Mouse clicked at (%d, %d)\n", x, y);
                    if ((map_bag_button_LEFT <= x || x <= map_bag_button_RIGHT) && (map_bag_button_UP <= y || y <= map_bag_button_DOWN))
                    {
                        quit = 1; // close bag
                    }

                    // 下方的我還叫不出來，待處理
                    //  if (place_decrease_soda)
                    //      printf("Use decrease soda\n");

                    // if (place_increase_soda)
                    //     printf("Use increase soda\n");

                    // if (place_gamble)
                    //     printf("Use gamble\n");

                    // if (place_ginger_soda)
                    //     printf("Use ginger soda\n");
                }
                break;
            }
        }
    }

    if (quit == 1)
    {
        // Clean up
        SDL_DestroyTexture(imageTexture);
        SDL_DestroyRenderer(renderer_bag);
        SDL_DestroyWindow(window_bag);
    }
}

// Mouse clicked at (530, 18)
// Mouse clicked at (634, 18)
// Mouse clicked at (529, 63)
// Mouse clicked at (634, 62)
