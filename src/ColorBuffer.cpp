/*
    Windows 10/11
    SchumakerTeam
    Hudson Schumaker
*/

#include "include/ColorBuffer.hpp"

ColorBuffer::ColorBuffer(SDL_Renderer* renderer) {
    this->renderer = renderer;

    // allocate the total amount of bytes in memory to hold the colorbuffer
    colorBuffer = (uint32_t*)malloc(sizeof(uint32_t) * Common::WIDTH * Common::HEIGHT);

    // create an SDL_Texture to display the colorbuffer
    colorBufferTexture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        Common::WIDTH,
        Common::HEIGHT
    );
}

ColorBuffer::~ColorBuffer() {
    free(colorBuffer);
    SDL_DestroyTexture(colorBufferTexture);
}

void ColorBuffer::clear() {
    clear(0xFF888888); // gray
}

void ColorBuffer::clear(uint32_t color) {
    for (int x = 0; x < Common::WIDTH; x++)
        for (int y = 0; y < Common::HEIGHT; y++)
            colorBuffer[(Common::WIDTH * y) + x] = color;
}

void ColorBuffer::render() {
    SDL_UpdateTexture(
        colorBufferTexture,
        NULL,
        colorBuffer,
        (int)(Common::WIDTH * sizeof(uint32_t))
    );
    SDL_RenderCopy(renderer, colorBufferTexture, NULL, NULL);
}
void ColorBuffer::setColorBuffer(int index, uint32_t color) {
    colorBuffer[index] = color;
}
