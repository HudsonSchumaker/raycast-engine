/*
    Windows 10/11
    SchumakerTeam
    Hudson Schumaker
*/

#pragma once

#include <math.h>
#include <stdio.h>
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
    static const unsigned short TILE_SIZE = 64;
    static const unsigned short MAP_NUM_COLS = 20;
    static const unsigned short MAP_NUM_ROWS = 13;
    
    static const unsigned short WIDTH = (MAP_NUM_COLS * TILE_SIZE);
    static const unsigned short H_WIDTH = WIDTH / 2;
    static const unsigned short HEIGHT = (MAP_NUM_ROWS * TILE_SIZE);
    static const unsigned short H_HEIGHT = HEIGHT / 2;
    
    constexpr static double PI = 3.14159265;
    constexpr static double TWO_PI = 6.28318530;
    constexpr static float FPS = 144.0f;
    constexpr static float FRAME_TIME_LENGTH = 1000.0f / FPS;
    
    constexpr static double FOV_ANGLE = (60 * (PI / 180));
    constexpr static const float MINIMAP_SCALE_FACTOR = 1.0f;
    static const unsigned short NUM_RAYS = WIDTH;
    

    static float degrees2Radians(float degress) {
        return degress * (PI / 180);
    }

    static float normalizeAngle(float angle) {
        angle = remainder(angle, TWO_PI);
        if (angle < 0) {
            angle = TWO_PI + angle;
        }
        return angle;
    }

    static float distanceBetweenPoints(float x1, float y1, float x2, float y2) {
        return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
    }
};