/*
    Windows 10/11
    SchumakerTeam
    Hudson Schumaker
*/

#include "include/Player.hpp"

Player::Player() {
    x = y = w = h = 0.0f;
    turnDirection = 0; 
    walkDirection = 0; 
    rotationAngle = 0.0f;
    walkSpeed = 0.0f;
    turnSpeed = 0.0f;
}

void Player::move(float deltaTime, Map* map) {
    rotationAngle += turnDirection * turnSpeed * deltaTime;
    float moveStep = walkDirection * walkSpeed * deltaTime;

    float newPlayerX = x + cos(rotationAngle) * moveStep;
    float newPlayerY = y + sin(rotationAngle) * moveStep;

    if (!map->hasWallAt(newPlayerX, newPlayerY)) {
        x = newPlayerX;
        y = newPlayerY;
    }
}

void Player::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_Rect playerRect = {
        x * Common::MINIMAP_SCALE,
        y * Common::MINIMAP_SCALE,
        w * Common::MINIMAP_SCALE,
        h * Common::MINIMAP_SCALE
    };
    SDL_RenderFillRect(renderer, &playerRect);

    SDL_RenderDrawLine(
        renderer,
        Common::MINIMAP_SCALE * x,
        Common::MINIMAP_SCALE * y,
        Common::MINIMAP_SCALE * (x + cos(rotationAngle) * 40),
        Common::MINIMAP_SCALE * (y + sin(rotationAngle) * 40)
    );
}