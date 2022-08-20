/*
    Windows 10/11
    SchumakerTeam
    Hudson Schumaker
*/

#include "include/Common.hpp"
#include "include/Splash.hpp"
#include "include/Main.h"

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

int main(int argc, char* argv[]) {
    setUp();
    splash();
   
    quit();
    return 0;
}

void setUp() {
    HWND windowHandle = GetConsoleWindow();
    ShowWindow(windowHandle, SW_HIDE);

    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();

    window = SDL_CreateWindow(
        Common::TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        Common::WIDTH,
        Common::HEIGHT,
        SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL
    );

    SDL_Surface* iconSurface = IMG_Load("data/joypad.png");
    SDL_SetWindowIcon(window, iconSurface);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 256);
}

void splash() {
    Splash* splash = new Splash(renderer);
    delete splash;
}

void quit() {
    SDL_Quit();
    IMG_Quit();
    TTF_Quit();
    Mix_Quit();
}
