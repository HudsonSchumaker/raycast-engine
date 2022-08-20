/*
    Windows 10/11
    SchumakerTeam
    Hudson Schumaker
*/

#pragma once
#include "Common.hpp"

class Player final {
public:
    float x, y, w, h;
    int turnDirection; // -1 for left, +1 for right
    int walkDirection; // -1 for back, +1 for front
    float rotationAngle;
    float walkSpeed;
    float turnSpeed;

    Player();
    ~Player() = default;
    void render(SDL_Renderer* renderer);
    void move(float deltaTime);
};