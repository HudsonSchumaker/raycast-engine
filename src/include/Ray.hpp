/*
    Windows 10/11
    SchumakerTeam
    Hudson Schumaker
*/

#pragma once
#include "Common.hpp"
#include "Player.hpp"
#include "Map.hpp"

class Ray final {
public:
    float rayAngle;
    float wallHitX;
    float wallHitY;
    float distance;
    int wasHitVertical;
    int isRayFacingUp;
    int isRayFacingDown;
    int isRayFacingLeft;
    int isRayFacingRight;
    int wallHitContent;

    Ray();
    ~Ray() = default;

    void static castAllRays(Ray* rays, const Player* player, Map* map);
    void static renderAllRays(SDL_Renderer* renderer, Ray* rays, const Player* player);

private:
    void static castRay(float rayAngle, Ray* ray, const Player* player, Map* map);
};
