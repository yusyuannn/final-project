#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <thread>
#include <chrono>

#define SCREEN_WIDTH  1280
#define SCREEN_HEIGHT 720
#define FONT_SIZE 24

namespace event4 {
    void renderText(SDL_Renderer* renderer, const char* text, TTF_Font* font, SDL_Color color, int x, int y) {
        std::string renderText = text;
        std::size_t pos = renderText.find("\n");
        int offsetY = 0;
        while (pos != std::string::npos) {
            std::string line = renderText.substr(0, pos);
            renderText.erase(0, pos + 1); 
            SDL_Surface* textSurface = TTF_RenderUTF8_Blended(font, line.c_str(), color);
            if (textSurface != nullptr) {
                SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
                SDL_Rect textRect = {x, y + offsetY, textSurface->w, textSurface->h};
                SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);
                offsetY += textSurface->h; 
                SDL_FreeSurface(textSurface);
                SDL_DestroyTexture(textTexture);
            }
            pos = renderText.find("\n");
        }
        SDL_Surface* textSurface = TTF_RenderUTF8_Blended(font, renderText.c_str(), color);
        if (textSurface != nullptr) {
            SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            int textWidth, textHeight;
            SDL_QueryTexture(textTexture, NULL, NULL, &textWidth, &textHeight);
            SDL_Rect textRect = {(SCREEN_WIDTH - textWidth) / 2, (SCREEN_HEIGHT - textHeight) / 2, textWidth, textHeight};
            SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);
            SDL_FreeSurface(textSurface);
            SDL_DestroyTexture(textTexture);
        }
    }

    void typeOutText(SDL_Renderer* renderer, const std::string& text, TTF_Font* font, SDL_Color color) {
        int x = (SCREEN_WIDTH - 600) / 2;
        int y = (SCREEN_HEIGHT - FONT_SIZE) / 2;
        std::string currentText = "";
        for (char c : text) {
            currentText += c;
            SDL_RenderClear(renderer);
            renderText(renderer, currentText.c_str(), font, color, x, y);
            SDL_RenderPresent(renderer);
            std::this_thread::sleep_for(std::chrono::milliseconds(50)); 
        }
    }

    SDL_Texture* loadImage(SDL_Renderer* renderer, const std::string& path) {
        SDL_Surface* loadedSurface = IMG_Load(path.c_str());
        if (loadedSurface == NULL) {
            std::cerr << "Unable to load image " << path << "! SDL_image Error: " << IMG_GetError() << std::endl;
            return NULL;
        }
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        SDL_FreeSurface(loadedSurface);
        return texture;
    }

    void runEvent4() {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
            return;
        }

        if (TTF_Init() == -1) {
            std::cerr << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << std::endl;
            return;
        }

        if (IMG_Init(IMG_INIT_PNG) == 0) {
            std::cerr << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
            return;
        }

        const char* defaultFont = "fonts/NotoSansTC-Bold.ttf";
        TTF_Font* font = TTF_OpenFont(defaultFont, FONT_SIZE);
        if (font == NULL) {
            std::cerr << "Failed to load font! SDL_ttf Error: " << TTF_GetError() << std::endl;
            return;
        }

        SDL_Color textColor = {0, 0, 0};
        
        // std::string text = "The author forgot to write the event, but she decided to deduct 300 bucks from you.\n 作者忘記寫這題了，但她決定扣你300塊";
        std::string text = "The author forgot to write the event, but she decided to deduct 300 bucks from you.";

        SDL_Window* window = SDL_CreateWindow("Event 4", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (window == NULL) {
            std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
            return;
        }

        SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (renderer == NULL) {
            std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
            return;
        }

        SDL_Texture* background = loadImage(renderer, "event_image/4.png");
        if (background == nullptr) {
            std::cerr << "Failed to load background image! SDL_image Error: " << IMG_GetError() << std::endl;
            return;
        }

        SDL_SetRenderDrawColor(renderer, 255, 242, 203, 255);
        SDL_RenderClear(renderer);
        // SDL_RenderCopy(renderer, background, NULL, NULL);
        // SDL_RenderPresent(renderer);

        typeOutText(renderer, text, font, textColor);

        SDL_Delay(2000);

        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        // TTF_CloseFont(font);
        // TTF_Quit();
        // IMG_Quit();
        // SDL_Quit();
    }
}

extern "C" {
    void runEvent4() {
        event4::runEvent4();
    }
}
