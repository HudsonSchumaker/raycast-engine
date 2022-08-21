/*
    Windows 10/11
    SchumakerTeam
    Hudson Schumaker
*/

#include "include/Ray.hpp"

Ray::Ray() {
    rayAngle = 0.0f;
    wallHitX = 0.0f;
    wallHitY = 0.0f;
    distance = 0.0f;
    wasHitVertical = 0;
    isRayFacingUp = 0;
    isRayFacingDown = 0;
    isRayFacingLeft = 0;
    isRayFacingRight = 0;
    wallHitContent = 0;
}