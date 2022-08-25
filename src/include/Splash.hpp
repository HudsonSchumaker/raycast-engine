/*
    Windows 10/11
    SchumakerTeam
    Hudson Schumaker
*/

#pragma once
#include "Common.hpp"

class Splash final {

private:
    SDL_Renderer* renderer;
    SDL_Texture* logoTexture;

    void load();
    void loop();
    void render();
    void input();
    void unload();

public:
    Splash(SDL_Renderer* renderer);
    ~Splash();
};