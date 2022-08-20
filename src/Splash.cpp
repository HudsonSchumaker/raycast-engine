/*
    Windows 10/11
    SchumakerTeam
    Hudson Schumaker
*/

#include "include/Splash.hpp"

Splash::Splash(SDL_Renderer* renderer) {
    this->renderer = renderer;
    load();
    loop();
}

Splash::~Splash() {
    unload();
}

void Splash::load() {
    SDL_Surface* logoSurface = IMG_Load("data/logo.png");
    logoTexture = SDL_CreateTextureFromSurface(renderer, logoSurface);
    
    SDL_FreeSurface(logoSurface);
}

void Splash::loop() {
    for (char i = 0; i < 4; i++) {
        input();
        render();
        SDL_Delay(1000);
    }
}

void Splash::render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_Point size;
    SDL_QueryTexture(logoTexture, NULL, NULL, &size.x, &size.y);
    SDL_Rect rect = { Common::H_WIDTH - (size.x / 2) - 20, Common::H_HEIGHT - (size.y / 2) - 20, size.x, size.y };
    SDL_RenderCopy(renderer, logoTexture, NULL, &rect);

    SDL_RenderPresent(renderer);
}

void Splash::input() {
    SDL_Event sdlEvent;
    while (SDL_PollEvent(&sdlEvent)) {
        switch (sdlEvent.type) {
            case SDL_QUIT:
                exit(0);
                break;
        }
    }
}

void Splash::unload() {
    SDL_DestroyTexture(logoTexture);
}