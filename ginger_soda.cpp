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

namespace event9 {
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
            SDL_Rect textRect = {x, y + offsetY, textSurface->w, textSurface->h};
            SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);
            SDL_FreeSurface(textSurface);
            SDL_DestroyTexture(textTexture);
        }
    }

    bool isMouseOverOption(int mouseX, int mouseY, int optionX, int optionY, int optionWidth, int optionHeight) {
        return mouseX >= optionX && mouseY >= optionY && mouseX <= optionX + optionWidth && mouseY <= optionY + optionHeight;
    }

    void typeOutText(SDL_Renderer* renderer, const std::string& text, TTF_Font* font, SDL_Color color, int x, int y, int delay, SDL_Texture* background) {
        std::string currentText = "";
        for (char c : text) {
            currentText += c;
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, background, NULL, NULL);
            renderText(renderer, currentText.c_str(), font, color, x, y);
            SDL_RenderPresent(renderer);
            std::this_thread::sleep_for(std::chrono::milliseconds(delay));
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

    int runEvent9(int price, int currentMoney) {
    int returnValue = -1;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    if (TTF_Init() == -1) {
        printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        return -1;
    }

    if (IMG_Init(IMG_INIT_PNG) == 0) {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        return -1;
    }

    const char* defaultFont = "fonts/NotoSansTC-Bold.ttf";
    TTF_Font* font = TTF_OpenFont(defaultFont, FONT_SIZE);
    if (font == NULL) {
        printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
        return -1;
    }

    SDL_Color textColor = {0, 0, 0};
    SDL_Color highlightColor = {255, 0, 0};
    char question[256];
    sprintf(question, "Ginger soda is on sale!! It costs %d now!!\nYou have %d dollars, do you want to buy one?", price, currentMoney);
    std::string optionYes = "YES";
    std::string optionNo = "NO";

    SDL_Window* window = SDL_CreateWindow("Select Option", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    SDL_Texture* background = loadImage(renderer, "main_game_image/9q.jpg");
    if (background == nullptr) {
        printf("Failed to load background image!\n");
        return -1;
    }

    int optionStartY = (4 * SCREEN_HEIGHT) / 5;
    int optionWidth = 100;  
    int optionHeight = TTF_FontHeight(font);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, background, NULL, NULL);
    typeOutText(renderer, question, font, textColor, 350, 20, 10, background);
    renderText(renderer, optionYes.c_str(), font, textColor, 500, optionStartY);
    renderText(renderer, optionNo.c_str(), font, textColor, 700, optionStartY);
    SDL_RenderPresent(renderer);

    SDL_Event e;
    bool quit = false;
    int mouseX, mouseY;
    int selectedOption = -1;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN) {
                SDL_GetMouseState(&mouseX, &mouseY);

                if (isMouseOverOption(mouseX, mouseY, 500, optionStartY, optionWidth, optionHeight)) {
                    selectedOption = 0;
                    if (e.type == SDL_MOUSEBUTTONDOWN) {
                        quit = true;
                    }
                } else if (isMouseOverOption(mouseX, mouseY, 700, optionStartY, optionWidth, optionHeight)) {
                    selectedOption = 1;
                    if (e.type == SDL_MOUSEBUTTONDOWN) {
                        quit = true;
                    }
                } else {
                    selectedOption = -1;
                }

                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderClear(renderer);
                SDL_RenderCopy(renderer, background, NULL, NULL);

                renderText(renderer, question, font, textColor, 350, 20);
                if (selectedOption == 0) {
                    renderText(renderer, optionYes.c_str(), font, highlightColor, 500, optionStartY);
                    renderText(renderer, optionNo.c_str(), font, textColor, 700, optionStartY);
                } else if (selectedOption == 1) {
                    renderText(renderer, optionYes.c_str(), font, textColor, 500, optionStartY);
                    renderText(renderer, optionNo.c_str(), font, highlightColor, 700, optionStartY);
                } else {
                    renderText(renderer, optionYes.c_str(), font, textColor, 500, optionStartY);
                    renderText(renderer, optionNo.c_str(), font, textColor, 700, optionStartY);
                }
                SDL_RenderPresent(renderer);
            }
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Window* newWindow = SDL_CreateWindow("Your Choice", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (newWindow == NULL) {
        printf("New Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    SDL_Renderer* newRenderer = SDL_CreateRenderer(newWindow, -1, SDL_RENDERER_ACCELERATED);
    if (newRenderer == NULL) {
        printf("New Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    SDL_Texture* newBackground = nullptr;
    if (selectedOption == 0) {
        newBackground = loadImage(newRenderer, "main_game_image/9a.png");
        returnValue = 1;
    } else if (selectedOption == 1) {
        newBackground = loadImage(newRenderer, "main_game_image/9b.png");
        returnValue = 0;
    } else {
        printf("Invalid option selected!\n");
        return -1;
    }

    if (newBackground != nullptr) {
        SDL_RenderClear(newRenderer);
        SDL_RenderCopy(newRenderer, newBackground, NULL, NULL);
        std::string answerText;
        if (selectedOption == 0) {
            answerText = "Congradulations!! You got a ginger soda!!";
        } else if (selectedOption == 1) {
            answerText = "Maybe next time...QQ";
        } else {
            answerText = "Invalid option selected!";
        }
        typeOutText(newRenderer, answerText, font, textColor, 350, (2 * SCREEN_HEIGHT) / 3, 10, newBackground);

        SDL_RenderPresent(newRenderer);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        SDL_DestroyTexture(newBackground);
    } else {
        printf("Failed to load new background image!\n");
    }

    SDL_DestroyRenderer(newRenderer);
    SDL_DestroyWindow(newWindow);

    // TTF_CloseFont(font);
    // TTF_Quit();
    // IMG_Quit();
    // SDL_Quit();

    return returnValue;
}

}

extern "C" {
    int call_gingersoda(int price, int currentMoney) {
        return event9::runEvent9(price, currentMoney);
    }
}