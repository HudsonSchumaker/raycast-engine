/*
    Windows 10/11
    SchumakerTeam
    Hudson Schumaker
*/

#pragma once

#include <math.h>
#include <vector>
#include <Windows.h>
#include <string>
#include <iostream>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <SDL_thread.h>
#include <Windows.h>

class Common final {

public:
    inline static const char TITLE[] = "- Raycast -";
    static const unsigned short WIDTH = 800;
    static const unsigned short H_WIDTH = WIDTH / 2;
    static const unsigned short HEIGHT = 600;
    static const unsigned short H_HEIGHT = HEIGHT / 2;

    static const short TILE_SIZE = 64;
    static const short MAP_NUM_ROWS = 13;
    static const short MAP_NUM_COLS = 20;

    constexpr static double PI = 3.14159265;
    constexpr static double TWO_PI = 6.28318530;
    
    constexpr static float FOV_ANGLE = (60 * (PI / 180));
    constexpr static float MINIMAP_SCALE_FACTOR = 1.0f;
    constexpr static float FPS = 60.0f;
    constexpr static float FRAME_TIME_LENGTH = 1000.0f / FPS;
};