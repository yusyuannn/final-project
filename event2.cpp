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

namespace event2 {
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

    void runEvent2() {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
            return;
        }

        if (TTF_Init() == -1) {
            printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
            return;
        }

        if (IMG_Init(IMG_INIT_PNG) == 0) {
            printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
            return;
        }

        const char* defaultFont = "fonts/NotoSansTC-Bold.ttf";
        TTF_Font* font = TTF_OpenFont(defaultFont, FONT_SIZE);
        if (font == NULL) {
            printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
            return;
        }

        SDL_Color textColor = {0, 0, 0};
        SDL_Color highlightColor = {255, 0, 0};
        /*"你的朋友在宴會中吵了起來，他們的聲量逐漸引起了更多人關注，這時的你會";
        "[A] 不要打了，要打就去練武室打";
        "[B] 趁機吃掉自助餐的熱門餐點";*/
        std::string question = "Your friends started a fight at the party, and the sound of them fighting attracted the crowds.\n What would you do in this situation??\n你的朋友在宴會中吵了起來，他們的聲量逐漸引起了更多人關注，這時的你會";
        std::string optionA = "[A] Tell them to stop this or do this in the daning room. 要打就去練武室打";
        std::string optionB = "[B] Sieze the moment and eat all the good food. 趁機吃掉自助餐的熱門餐點";

        SDL_Window* window = SDL_CreateWindow("Select Option", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (window == NULL) {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
            return;
        }

        SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (renderer == NULL) {
            printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
            return;
        }

        SDL_Texture* background = loadImage(renderer, "event_image/2q.png");
        if (background == nullptr) {
            printf("Failed to load background image!\n");
            return;
        }

        SDL_Texture* img2 = loadImage(renderer, "event_image/2a.png");
        SDL_Texture* img3 = loadImage(renderer, "event_image/2b.png");

        if (img2 == NULL || img3 == NULL) {
            printf("Failed to load images!\n");
            return;
        }

        int optionStartY = (4 * SCREEN_HEIGHT) / 5;
        int optionHeight = TTF_FontHeight(font);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, background, NULL, NULL);
        typeOutText(renderer, question, font, textColor, 10, 10, 10, background);
        renderText(renderer, optionA.c_str(), font, textColor, 10, optionStartY);
        renderText(renderer, optionB.c_str(), font, textColor, 10, optionStartY + optionHeight);
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

                    if (isMouseOverOption(mouseX, mouseY, 10, optionStartY, SCREEN_WIDTH, optionHeight)) {
                        selectedOption = 0;
                        if (e.type == SDL_MOUSEBUTTONDOWN) {
                            quit = true;
                        }
                    } else if (isMouseOverOption(mouseX, mouseY, 10, optionStartY + optionHeight, SCREEN_WIDTH, optionHeight)) {
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

                    renderText(renderer, question.c_str(), font, textColor, 10, 10);
                    if (selectedOption == 0) {
                        renderText(renderer, optionA.c_str(), font, highlightColor, 10, optionStartY);
                        renderText(renderer, optionB.c_str(), font, textColor, 10, optionStartY + optionHeight);
                    } else if (selectedOption == 1) {
                        renderText(renderer, optionA.c_str(), font, textColor, 10, optionStartY);
                        renderText(renderer, optionB.c_str(), font, highlightColor, 10, optionStartY + optionHeight);
                    } else {
                        renderText(renderer, optionA.c_str(), font, textColor, 10, optionStartY);
                        renderText(renderer, optionB.c_str(), font, textColor, 10, optionStartY + optionHeight);
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
            return;
        }

        SDL_Renderer* newRenderer = SDL_CreateRenderer(newWindow, -1, SDL_RENDERER_ACCELERATED);
        if (newRenderer == NULL) {
            printf("New Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
            return;
        }


        SDL_Texture* newBackground = nullptr;
        if (selectedOption == 0) {
            newBackground = loadImage(newRenderer, "event_image/2a.png");
        } else if (selectedOption == 1) {
            newBackground = loadImage(newRenderer, "event_image/2b.png");
        } else {
            printf("Invalid selected option!\n");
            return;
        }

 
        SDL_SetRenderDrawColor(newRenderer, 255, 255, 255, 255);
        SDL_RenderClear(newRenderer);
        SDL_RenderCopy(newRenderer, newBackground, NULL, NULL);
        /*"被爆打一波抬出去了(後退一步)";
        "好吃好吃";*/

        std::string answerText;
        if (selectedOption == 0) {
            answerText = "You were destroyed. (and stepped back with an empty stomach)\n被爆打一波抬出去了(後退一步)";
        } else if (selectedOption == 1) {
            answerText = "\nYummy Yummy ^^\n好吃好吃";
        } else {
            answerText = "Invalid option selected!";
        }

        typeOutText(newRenderer, answerText, font, textColor, 50, 50, 10, newBackground);

        SDL_RenderPresent(newRenderer);

        SDL_Delay(5000);

        SDL_DestroyTexture(newBackground);
        SDL_DestroyRenderer(newRenderer);
        SDL_DestroyWindow(newWindow);
        // TTF_CloseFont(font);
        // TTF_Quit();
        // IMG_Quit();
        // SDL_Quit();
    }
}

extern "C" {
    void runEvent2() {
        event2::runEvent2();
    }
}
