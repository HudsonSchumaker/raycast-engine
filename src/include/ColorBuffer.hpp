/*
    Windows 10/11
    SchumakerTeam
    Hudson Schumaker
*/

#pragma once
#include "Common.hpp"

class ColorBuffer final {
private:
    SDL_Renderer* renderer;
    uint32_t* colorBuffer = nullptr;
    SDL_Texture* colorBufferTexture;

public:
    ColorBuffer(SDL_Renderer* renderer);
    ~ColorBuffer();

    void clear();
    void clear(uint32_t color);
    void render();

};

