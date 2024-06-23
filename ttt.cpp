// ttt.cpp

#include "ttt.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

// (Include the rest of your game code here)

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;
const int CELL_SIZE = 190;

enum Player { NONE, PLAYER_X, PLAYER_O };

Player board[3][3] = { {NONE, NONE, NONE}, {NONE, NONE, NONE}, {NONE, NONE, NONE} };
Player ttt_currentPlayer = PLAYER_X;
bool gameWon = false;
bool gameDraw = false;
bool playerLost = false; 
std::string winnerText = "";
int amount = 0;

SDL_Texture* LoadTexture(SDL_Renderer* renderer, const std::string& file) {
    SDL_Surface* surface = IMG_Load(file.c_str());
    if (!surface) {
        std::cerr << "Failed to load image: " << file << " SDL_Error: " << SDL_GetError() << std::endl;
        return nullptr;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

void DrawTexture(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y) {
    SDL_Rect rect = {x, y, CELL_SIZE, CELL_SIZE}; 
    SDL_RenderCopy(renderer, texture, NULL, &rect);
}

void DrawMarks(SDL_Renderer* renderer) {
    SDL_Texture* xTexture = LoadTexture(renderer, "minigame_image/TX.png");
    if (!xTexture) {
        std::cerr << "Failed to load X texture!" << std::endl;
        return;
    }
    SDL_Texture* oTexture = LoadTexture(renderer, "minigame_image/TO.png");
    if (!oTexture) {
        std::cerr << "Failed to load O texture!" << std::endl;
        SDL_DestroyTexture(xTexture);
        return;
    }

    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            int centerX = col * (WINDOW_WIDTH / 3) + (WINDOW_WIDTH / 6) - (CELL_SIZE / 2); 
            int centerY = row * (WINDOW_HEIGHT / 3) + (WINDOW_HEIGHT / 6) - (CELL_SIZE / 2); 
            if (board[row][col] == PLAYER_X) {
                DrawTexture(renderer, xTexture, centerX, centerY);
            } else if (board[row][col] == PLAYER_O) {
                DrawTexture(renderer, oTexture, centerX, centerY);
            }
        }
    }

    SDL_DestroyTexture(xTexture);
    SDL_DestroyTexture(oTexture);
}

void DrawText(SDL_Renderer* renderer, const std::string& text, int x, int y, SDL_Color color) {
    TTF_Font* Sans = TTF_OpenFont("fonts/NotoSansTC-Bold.ttf", 24);
    if (!Sans) {
        std::cerr << "TTF_OpenFont: " << TTF_GetError() << std::endl;
        return;
    }

    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(Sans, text.c_str(), color); 
    SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

    SDL_Rect Message_rect;
    Message_rect.x = x;
    Message_rect.y = y;
    Message_rect.w = surfaceMessage->w;
    Message_rect.h = surfaceMessage->h;

    SDL_RenderCopy(renderer, Message, NULL, &Message_rect);

    SDL_FreeSurface(surfaceMessage);
    SDL_DestroyTexture(Message);
    TTF_CloseFont(Sans);
}

bool CheckWin(Player player) {
    for (int i = 0; i < 3; ++i) {
        if (board[i][0] == player && board[i][1] == player && board[i][2] == player) return true;
        if (board[0][i] == player && board[1][i] == player && board[2][i] == player) return true;
    }
    if (board[0][0] == player && board[1][1] == player && board[2][2] == player) return true;
    if (board[0][2] == player && board[1][1] == player && board[2][0] == player) return true;
    return false;
}

bool CheckDraw() {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (board[i][j] == NONE) return false;
        }
    }
    return true;
}

void HandleClick(int x, int y) {
    if (gameWon || gameDraw) return;

    int row = y / (WINDOW_HEIGHT / 3);
    int col = x / (WINDOW_WIDTH / 3);
    if (board[row][col] == NONE) {
        board[row][col] = ttt_currentPlayer;
        if (CheckWin(ttt_currentPlayer)) {
            gameWon = true;
            winnerText = "Player " + std::string(ttt_currentPlayer == PLAYER_X ? "X" : "O") + " wins!";
            if (ttt_currentPlayer == PLAYER_X) {
                amount += 100; 
            } else {
                amount -= 100; 
            }
            return;
        }
        if (CheckDraw()) {
            gameDraw = true;
            winnerText = "It's a draw!";
            amount += 50; 
            return;
        }
        ttt_currentPlayer = (ttt_currentPlayer == PLAYER_X) ? PLAYER_O : PLAYER_X;

        if (ttt_currentPlayer == PLAYER_O) {
            std::vector<std::pair<int, int>> emptySpaces;
            for (int i = 0; i < 3; ++i) {
                for (int j = 0; j < 3; ++j) {
                    if (board[i][j] == NONE) {
                        emptySpaces.push_back({i, j});
                    }
                }
            }
            if (!emptySpaces.empty()) {
                std::srand(std::time(nullptr));
                int randomIndex = std::rand() % emptySpaces.size();
                board[emptySpaces[randomIndex].first][emptySpaces[randomIndex].second] = ttt_currentPlayer;
                if (CheckWin(ttt_currentPlayer)) {
                    gameWon = true;
                    playerLost = true; 
                    winnerText = "Player O wins!";
                    amount -= 100;
                } else if (CheckDraw()) {
                    gameDraw = true;
                    winnerText = "It's a draw!";
                    amount += 50; 
                }
                ttt_currentPlayer = PLAYER_X;
            }
        }
    }
}

void ShowResult(SDL_Renderer* renderer) {
    SDL_Surface* resultImage = IMG_Load("minigame_image/result.png");
    if (resultImage == nullptr) {
        std::cerr << "Failed to load result image! SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }
    SDL_Texture* resultTexture = SDL_CreateTextureFromSurface(renderer, resultImage);
    SDL_FreeSurface(resultImage);
    SDL_Rect destRect;
    destRect.x = (WINDOW_WIDTH - resultImage->w) / 2;
    destRect.y = (WINDOW_HEIGHT - resultImage->h) / 2;
    destRect.w = resultImage->w;
    destRect.h = resultImage->h;
    SDL_RenderCopy(renderer, resultTexture, NULL, &destRect);
    std::string resultMessage;
    SDL_Color textColor;
    if (gameWon) {
        if (playerLost) {
            resultMessage = "You lost!(money-150)"; 
        } else {
            resultMessage = "You won!(money+300)";
        }
        textColor = {0, 0, 0};  
    } else if (gameDraw) {
        resultMessage = "It's a draw!";
        textColor = {0, 0, 0}; 
    }
    TTF_Font* Sans = TTF_OpenFont("fonts/NotoSansTC-Bold.ttf", 24);
    if (!Sans) {
        std::cerr << "TTF_OpenFont: " << TTF_GetError() << std::endl;
        SDL_DestroyTexture(resultTexture);
        return;
    }
    SDL_Surface* textSurface = TTF_RenderText_Solid(Sans, resultMessage.c_str(), textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_Rect textRect;
    textRect.w = textSurface->w;
    textRect.h = textSurface->h;
    textRect.x = (WINDOW_WIDTH - textRect.w) / 2;
    textRect.y = (WINDOW_HEIGHT - textRect.h) / 2;
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    SDL_RenderPresent(renderer);
    SDL_Delay(3000);
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
    TTF_CloseFont(Sans);
    SDL_DestroyTexture(resultTexture);
}

int RunTTT() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 2;
    }
    if (TTF_Init() == -1) {
        std::cerr << "TTF_Init: " << TTF_GetError() << std::endl;
        SDL_Quit();
        return 2;
    }

    SDL_Window* window = SDL_CreateWindow("Tic Tac Toe",
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          WINDOW_WIDTH, WINDOW_HEIGHT,
                                          SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        TTF_Quit();
        SDL_Quit();
        return 2;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 2;
    }

    SDL_Surface* backgroundImage = IMG_Load("minigame_image/TB.png");
    if (backgroundImage == nullptr) {
        std::cerr << "Failed to load background image! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 2;
    }

    SDL_Texture* backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundImage);
    SDL_FreeSurface(backgroundImage);



    ttt_currentPlayer = PLAYER_X;
    gameWon = false;
    gameDraw = false;
    playerLost = false; 
    int amount = 0;



    bool quit = false;
    SDL_Event e;

    SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
    SDL_RenderPresent(renderer);

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                HandleClick(x, y);
                if (gameWon || gameDraw) {
                    quit = true;
                }
            }
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
        DrawMarks(renderer);
        SDL_RenderPresent(renderer);
    }

    ShowResult(renderer);

    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    // TTF_Quit();
    // SDL_Quit();

    // lose:-150, win:+300

    if (gameWon) {
        return playerLost ? 2 : 1; // 輸或贏
    } else if (gameDraw) {
        return 3; // 平局
    } else {
        return 2; // 輸
    }
}
