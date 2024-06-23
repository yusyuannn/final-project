#include "timer.h"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
TTF_Font* gFont = NULL;

LTexture gTimeTextTexture;
LTexture gPausePromptTexture;
LTexture gStartPromptTexture;
LTexture gMoneyTexture;
LTexture gBackgroundTexture;

LTexture::LTexture() {
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
}

LTexture::~LTexture() {
    free();
}

bool LTexture::loadFromFile(std::string path) {
    free();
    SDL_Texture* newTexture = NULL;
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL) {
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
    } else {
        SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if (newTexture == NULL) {
            printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        } else {
            mWidth = loadedSurface->w;
            mHeight = loadedSurface->h;
        }
        SDL_FreeSurface(loadedSurface);
    }
    mTexture = newTexture;
    return mTexture != NULL;
}

bool LTexture::loadFromRenderedText(std::string textureText, SDL_Color textColor) {
    free();
    SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, textureText.c_str(), textColor);
    if (textSurface != NULL) {
        mTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
        if (mTexture == NULL) {
            printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
        } else {
            mWidth = textSurface->w;
            mHeight = textSurface->h;
        }
        SDL_FreeSurface(textSurface);
    } else {
        printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
    }
    return mTexture != NULL;
}

void LTexture::free() {
    if (mTexture != NULL) {
        SDL_DestroyTexture(mTexture);
        mTexture = NULL;
        mWidth = 0;
        mHeight = 0;
    }
}

void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue) {
    SDL_SetTextureColorMod(mTexture, red, green, blue);
}

void LTexture::setBlendMode(SDL_BlendMode blending) {
    SDL_SetTextureBlendMode(mTexture, blending);
}

void LTexture::setAlpha(Uint8 alpha) {
    SDL_SetTextureAlphaMod(mTexture, alpha);
}

void LTexture::render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip) {
    SDL_Rect renderQuad = { x, y, mWidth, mHeight };
    if (clip != NULL) {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }
    SDL_RenderCopyEx(gRenderer, mTexture, clip, &renderQuad, angle, center, flip);
}

int LTexture::getWidth() {
    return mWidth;
}

int LTexture::getHeight() {
    return mHeight;
}

LTimer::LTimer() {
    mStartTicks = 0;
    mPausedTicks = 0;
    mPaused = false;
    mStarted = false;
}

void LTimer::start() {
    mStarted = true;
    mPaused = false;
    mStartTicks = SDL_GetTicks();
    mPausedTicks = 0;
}

void LTimer::stop() {
    mStarted = false;
    mPaused = false;
    mStartTicks = 0;
    mPausedTicks = 0;
}

void LTimer::pause() {
    if (mStarted && !mPaused) {
        mPaused = true;
        mPausedTicks = SDL_GetTicks() - mStartTicks;
        mStartTicks = 0;
    }
}

void LTimer::unpause() {
    if (mStarted && mPaused) {
        mPaused = false;
        mStartTicks = SDL_GetTicks() - mPausedTicks;
        mPausedTicks = 0;
    }
}

Uint32 LTimer::getTicks() {
    Uint32 time = 0;
    if (mStarted) {
        if (mPaused) {
            time = mPausedTicks;
        } else {
            time = SDL_GetTicks() - mStartTicks;
        }
    }
    return time;
}

bool LTimer::isStarted() {
    return mStarted;
}

bool LTimer::isPaused() {
    return mPaused && mStarted;
}

bool init() {
    bool success = true;
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        success = false;
    } else {
        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
            printf("Warning: Linear texture filtering not enabled!");
        }
        gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (gWindow == NULL) {
            printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
            success = false;
        } else {
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            if (gRenderer == NULL) {
                printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
                success = false;
            } else {
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags)) {
                    printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
                    success = false;
                }
                if (TTF_Init() == -1) {
                    printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
                    success = false;
                }
            }
        }
    }
    return success;
}

bool loadMedia() {
    bool success = true;
    gFont = TTF_OpenFont("fonts/NotoSansTC-Bold.ttf", 28);
    if (gFont == NULL) {
        printf("Failed to load SDL_ttf Error: %s\n", TTF_GetError());
        success = false;
    } else {
        SDL_Color textColor = {0, 0, 0, 255};
        if (!gStartPromptTexture.loadFromRenderedText("Press S to Start the Timer", textColor)) {
            printf("Unable to render start/stop prompt texture!\n");
            success = false;
        }

        LTexture tryPressTextTexture;
        if (!tryPressTextTexture.loadFromRenderedText("try to press at 5sec", textColor)) {
            printf("Unable to render 'try to press at 5sec' texture!\n");
            success = false;
        }

        if (!gPausePromptTexture.loadFromRenderedText("Press SPACE to Pause the Timer", textColor)) {
            printf("Unable to render pause/unpause prompt texture!\n");
            success = false;
        }

        if (!gMoneyTexture.loadFromRenderedText("Money: 0", textColor)) {
            printf("Unable to render money texture!\n");
            success = false;
        }
        if (!gBackgroundTexture.loadFromFile("minigame_image/timerBG.png")) {
            printf("Failed to load background texture!\n");
            success = false;
        }
    }
    return success;
}

void TCclose() {
    gTimeTextTexture.free();
    gStartPromptTexture.free();
    gPausePromptTexture.free();
    gMoneyTexture.free();
    gBackgroundTexture.free();
    TTF_CloseFont(gFont);
    gFont = NULL;
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;
    // TTF_Quit();
    // IMG_Quit();
    // SDL_Quit();
}

int runTC() {
    int money = 0;
    bool gameOver = false;
    float stopTime = 0.0f;

    if (!init()) {
        printf("Failed to initialize!\n");
        return 0; 
    }

    if (!loadMedia()) {
        printf("Failed to load media!\n");
        TCclose();
        return 0; 
    }

    bool quit = false;
    SDL_Event e;
    SDL_Color textColor = {0, 0, 0, 255};
    LTimer timer;
    std::stringstream timeText;
    LTexture tcTexture;
    if (!tcTexture.loadFromFile("minigame_image/tc.png")) {
        printf("Failed to load tc.png!\n");
        quit = true;
    }
    LTexture tryPressTexture;
    if (!tryPressTexture.loadFromRenderedText("Try to press at 5sec", textColor)) {
        printf("Unable to render try press texture!\n");
        quit = true;
    }

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_s) {
                    if (timer.isStarted()) {
                        timer.stop();
                    } else {
                        timer.start();
                    }
                } else if (e.key.keysym.sym == SDLK_SPACE && !gameOver) {
                    gameOver = true;
                    timer.pause();
                    stopTime = timer.getTicks() / 1000.0f;
                    int moneyIncrement = 100 * (1 / std::abs(5 - stopTime));
                    money += moneyIncrement;
                } else if (e.key.keysym.sym == SDLK_SPACE && gameOver) {
                    quit = true;
                }
            }
        }

        timeText.str("");
        timeText << "Seconds since start time " << (timer.getTicks() / 1000.f);
        if (!gTimeTextTexture.loadFromRenderedText(timeText.str().c_str(), textColor)) {
            printf("Unable to render time texture!\n");
        }

        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(gRenderer);

        gBackgroundTexture.render(0, 0, NULL, 0.0, NULL, SDL_FLIP_NONE);
        gStartPromptTexture.render((SCREEN_WIDTH - gStartPromptTexture.getWidth()) / 2, 0);
        gPausePromptTexture.render((SCREEN_WIDTH - gPausePromptTexture.getWidth()) / 2, gStartPromptTexture.getHeight());

        gTimeTextTexture.render((SCREEN_WIDTH - gTimeTextTexture.getWidth()) / 2, (SCREEN_HEIGHT - gTimeTextTexture.getHeight()) / 2);
        tryPressTexture.render((SCREEN_WIDTH - tryPressTexture.getWidth()) / 2, gPausePromptTexture.getHeight() + gStartPromptTexture.getHeight() + 10);

        if (gameOver) {
            tcTexture.render((SCREEN_WIDTH - tcTexture.getWidth()) / 2, (SCREEN_HEIGHT - tcTexture.getHeight()) / 2);
            LTexture stopTimeTexture;
            std::stringstream stopTimeText;
            stopTimeText << "You stopped at " << stopTime << " sec";
            if (!stopTimeTexture.loadFromRenderedText(stopTimeText.str().c_str(), textColor)) {
                printf("Unable to render stop time texture!\n");
            }
            stopTimeTexture.render((SCREEN_WIDTH - stopTimeTexture.getWidth()) / 2, (SCREEN_HEIGHT - stopTimeTexture.getHeight()) / 2 - 50);

            std::stringstream moneyText;
            moneyText << "Money: " << money;
            if (!gMoneyTexture.loadFromRenderedText(moneyText.str().c_str(), textColor)) {
                printf("Unable to render money texture!\n");
            }
            int moneyTextureWidth = gMoneyTexture.getWidth();
            int moneyTextureHeight = gMoneyTexture.getHeight();
            gMoneyTexture.render((SCREEN_WIDTH - moneyTextureWidth) / 2, (SCREEN_HEIGHT - moneyTextureHeight) / 2 + 50);
        }

        SDL_RenderPresent(gRenderer);
    }

    TCclose();
    return money; 
}
