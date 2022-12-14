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

void Ray::renderAllRays(SDL_Renderer* renderer, Ray* rays, const Player* player) {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    for (int i = 0; i < Common::NUM_RAYS; i++) {
        SDL_RenderDrawLine(
            renderer,
            Common::MINIMAP_SCALE * player->x,
            Common::MINIMAP_SCALE * player->y,
            Common::MINIMAP_SCALE * rays[i].wallHitX,
            Common::MINIMAP_SCALE * rays[i].wallHitY
        );
    }
}

void Ray::castAllRays(Ray* rays, const Player* player, Map* map) {
    float rayAngle = player->rotationAngle - (Common::FOV_ANGLE / 2);

    for (int stripId = 0; stripId < Common::NUM_RAYS; stripId++) {
        castRay(rayAngle, &rays[stripId], player, map);
        rayAngle += Common::FOV_ANGLE / Common::NUM_RAYS;
    }
}

void Ray::castRay(float rayAngle, Ray* ray, const Player* player, Map* map) {
    rayAngle = Common::normalizeAngle(rayAngle);

    bool isRayFacingDown = rayAngle > 0 && rayAngle < Common::PI;
    bool isRayFacingUp = !isRayFacingDown;

    bool isRayFacingRight = rayAngle < 0.5 * Common::PI || rayAngle > 1.5 * Common::PI;
    bool isRayFacingLeft = !isRayFacingRight;

    float xintercept, yintercept;
    float xstep, ystep;

    ///////////////////////////////////////////
    // HORIZONTAL RAY-GRID INTERSECTION CODE
    ///////////////////////////////////////////
    bool foundHorzWallHit = false;
    float horzWallHitX = 0;
    float horzWallHitY = 0;
    int horzWallContent = 0;

    // Find the y-coordinate of the closest horizontal grid intersection
    yintercept = floor(player->y / Common::TILE_SIZE) * Common::TILE_SIZE;
    yintercept += isRayFacingDown ? Common::TILE_SIZE : 0;

    // Find the x-coordinate of the closest horizontal grid intersection
    xintercept = player->x + (yintercept - player->y) / tan(rayAngle);

    // Calculate the increment xstep and ystep
    ystep = Common::TILE_SIZE;
    ystep *= isRayFacingUp ? -1 : 1;

    xstep = Common::TILE_SIZE / tan(rayAngle);
    xstep *= (isRayFacingLeft && xstep > 0) ? -1 : 1;
    xstep *= (isRayFacingRight && xstep < 0) ? -1 : 1;

    float nextHorzTouchX = xintercept;
    float nextHorzTouchY = yintercept;

    // Increment xstep and ystep until we find a wall VERTICAL
    while (nextHorzTouchX >= 0 && nextHorzTouchX <= Common::WIDTH && nextHorzTouchY >= 0 && nextHorzTouchY <= Common::HEIGHT) {
        float xToCheck = nextHorzTouchX;
        float yToCheck = nextHorzTouchY + (isRayFacingUp ? -1 : 0);

        if (map->hasWallAt(xToCheck, yToCheck)) {
            // found a wall hit
            horzWallHitX = nextHorzTouchX;
            horzWallHitY = nextHorzTouchY;
            horzWallContent = map->getCellValue((int)floor(yToCheck / Common::TILE_SIZE), (int)floor(xToCheck / Common::TILE_SIZE));
            foundHorzWallHit = true;
            break;
        }
        else {
            nextHorzTouchX += xstep;
            nextHorzTouchY += ystep;
        }
    }

    ///////////////////////////////////////////
    // VERTICAL RAY-GRID INTERSECTION CODE
    ///////////////////////////////////////////
    bool foundVertWallHit = false;
    float vertWallHitX = 0;
    float vertWallHitY = 0;
    int vertWallContent = 0;

    // Find the x-coordinate of the closest horizontal grid intersection
    xintercept = floor(player->x / Common::TILE_SIZE) * Common::TILE_SIZE;
    xintercept += isRayFacingRight ? Common::TILE_SIZE : 0;

    // Find the y-coordinate of the closest horizontal grid intersection
    yintercept = player->y + (xintercept - player->x) * tan(rayAngle);

    // Calculate the increment xstep and ystep
    xstep = Common::TILE_SIZE;
    xstep *= isRayFacingLeft ? -1 : 1;

    ystep = Common::TILE_SIZE * tan(rayAngle);
    ystep *= (isRayFacingUp && ystep > 0) ? -1 : 1;
    ystep *= (isRayFacingDown && ystep < 0) ? -1 : 1;

    float nextVertTouchX = xintercept;
    float nextVertTouchY = yintercept;

    // Increment xstep and ystep until we find a wall HORIZONTAL
    while (nextVertTouchX >= 0 && nextVertTouchX <= Common::WIDTH && nextVertTouchY >= 0 && nextVertTouchY <= Common::HEIGHT) {
        float xToCheck = nextVertTouchX + (isRayFacingLeft ? -1 : 0);
        float yToCheck = nextVertTouchY;

        if (map->hasWallAt(xToCheck, yToCheck)) {
            // found a wall hit
            vertWallHitX = nextVertTouchX;
            vertWallHitY = nextVertTouchY;
            vertWallContent = map->getCellValue((int)floor(yToCheck / Common::TILE_SIZE), (int)floor(xToCheck / Common::TILE_SIZE));
            foundVertWallHit = true;
            break;
        }
        else {
            nextVertTouchX += xstep;
            nextVertTouchY += ystep;
        }
    }

    // Calculate both horizontal and vertical hit distances and choose the smallest one
    float horzHitDistance = foundHorzWallHit
        ? Common::distanceBetweenPoints(player->x, player->y, horzWallHitX, horzWallHitY)
        : FLT_MAX;
    float vertHitDistance = foundVertWallHit
        ? Common::distanceBetweenPoints(player->x, player->y, vertWallHitX, vertWallHitY)
        : FLT_MAX;

    if (vertHitDistance < horzHitDistance) {
        ray->distance = vertHitDistance;
        ray->wallHitX = vertWallHitX;
        ray->wallHitY = vertWallHitY;
        ray->wallHitContent = vertWallContent;
        ray->wasHitVertical = true;
    }
    else {
        ray->distance = horzHitDistance;
        ray->wallHitX = horzWallHitX;
        ray->wallHitY = horzWallHitY;
        ray->wallHitContent = horzWallContent;
        ray->wasHitVertical = false;
    }

    ray->rayAngle = rayAngle;
    ray->isRayFacingDown = isRayFacingDown;
    ray->isRayFacingUp = isRayFacingUp;
    ray->isRayFacingLeft = isRayFacingLeft;
    ray->isRayFacingRight = isRayFacingRight;
}