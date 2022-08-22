/*
    Windows 10/11
    SchumakerTeam
    Hudson Schumaker
*/

#include "include/Engine.hpp"

Engine::Engine(SDL_Renderer* renderer) {
    this->renderer = renderer;
}

void Engine::start() {
    load();
    loop();
}

void Engine::loop() {
    isRunning = true;
    while (isRunning) {
        input();
        update(); 
        render();
    }
}

void Engine::update() {
    int timeToWait = Common::FRAME_TIME_LENGTH - (SDL_GetTicks() - ticksLastFrame);

    if (timeToWait > 0 && timeToWait <= Common::FRAME_TIME_LENGTH) {
        SDL_Delay(timeToWait);
    }

    float deltaTime = (SDL_GetTicks() - ticksLastFrame) / 1000.0f;
    ticksLastFrame = SDL_GetTicks();
    
    player.move(deltaTime, &map);
    castAllRays();

}

void Engine::castAllRays() {
    float rayAngle = player.rotationAngle - (Common::FOV_ANGLE / 2);

    for (int stripId = 0; stripId < Common::NUM_RAYS; stripId++) {
        castRay(rayAngle, stripId);
        rayAngle += Common::FOV_ANGLE / Common::NUM_RAYS;
    }
}

void Engine::castRay(float rayAngle, int stripId) {
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
    yintercept = floor(player.y / Common::TILE_SIZE) * Common::TILE_SIZE;
    yintercept += isRayFacingDown ? Common::TILE_SIZE : 0;

    // Find the x-coordinate of the closest horizontal grid intersection
    xintercept = player.x + (yintercept - player.y) / tan(rayAngle);

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

        if (map.hasWallAt(xToCheck, yToCheck)) {
            // found a wall hit
            horzWallHitX = nextHorzTouchX;
            horzWallHitY = nextHorzTouchY;
            horzWallContent = map.getCellValue((int)floor(yToCheck / Common::TILE_SIZE), (int)floor(xToCheck / Common::TILE_SIZE));
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
    xintercept = floor(player.x / Common::TILE_SIZE) * Common::TILE_SIZE;
    xintercept += isRayFacingRight ? Common::TILE_SIZE : 0;

    // Find the y-coordinate of the closest horizontal grid intersection
    yintercept = player.y + (xintercept - player.x) * tan(rayAngle);

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

        if (map.hasWallAt(xToCheck, yToCheck)) {
            // found a wall hit
            vertWallHitX = nextVertTouchX;
            vertWallHitY = nextVertTouchY;
            vertWallContent = map.getCellValue((int)floor(yToCheck / Common::TILE_SIZE), (int)floor(xToCheck / Common::TILE_SIZE));
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
        ? Common::distanceBetweenPoints(player.x, player.y, horzWallHitX, horzWallHitY)
        : FLT_MAX;
    float vertHitDistance = foundVertWallHit
        ? Common::distanceBetweenPoints(player.x, player.y, vertWallHitX, vertWallHitY)
        : FLT_MAX;

    if (vertHitDistance < horzHitDistance) {
        rays[stripId].distance = vertHitDistance;
        rays[stripId].wallHitX = vertWallHitX;
        rays[stripId].wallHitY = vertWallHitY;
        rays[stripId].wallHitContent = vertWallContent;
        rays[stripId].wasHitVertical = true;
    } else {
        rays[stripId].distance = horzHitDistance;
        rays[stripId].wallHitX = horzWallHitX;
        rays[stripId].wallHitY = horzWallHitY;
        rays[stripId].wallHitContent = horzWallContent;
        rays[stripId].wasHitVertical = false;
    }
    rays[stripId].rayAngle = rayAngle;
    rays[stripId].isRayFacingDown = isRayFacingDown;
    rays[stripId].isRayFacingUp = isRayFacingUp;
    rays[stripId].isRayFacingLeft = isRayFacingLeft;
    rays[stripId].isRayFacingRight = isRayFacingRight;
}

void Engine::input() {
    SDL_Event event;
    SDL_PollEvent(&event);
    switch (event.type) {
        case SDL_QUIT: {
            isRunning = false;
            break;
        }
        case SDL_KEYDOWN: {
            if (event.key.keysym.sym == SDLK_ESCAPE)
                isRunning = false;
            if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w)
                player.walkDirection = +1;
            if (event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s)
                player.walkDirection = -1;
            if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d)
                player.turnDirection = +1;
            if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a)
                player.turnDirection = -1;
            break;
        }
        
        case SDL_KEYUP: {
            if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w)
                player.walkDirection = 0;
            if (event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s)
                player.walkDirection = 0;
            if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d)
                player.turnDirection = 0;
            if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a)
                player.turnDirection = 0;
            break;
        }
    }
}

void Engine::render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    colorBuffer->render();
    colorBuffer->clear();

    map.render(renderer);
    renderRays();
    player.render(renderer);

    SDL_RenderPresent(renderer);
}

void Engine::renderRays() {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    for (int i = 0; i < Common::NUM_RAYS; i++) {
        SDL_RenderDrawLine(
            renderer,
            Common::MINIMAP_SCALE * player.x,
            Common::MINIMAP_SCALE * player.y,
            Common::MINIMAP_SCALE * rays[i].wallHitX,
            Common::MINIMAP_SCALE * rays[i].wallHitY
        );
    }
}

void Engine::load() {
    player.x = Common::H_WIDTH;
    player.y = Common::H_HEIGHT;
    player.w = 2;
    player.h = 2;
    player.turnDirection = 0;
    player.walkDirection = 0;
    player.rotationAngle = 3 * Common::PI / 2;
    player.walkSpeed = 100;
    player.turnSpeed = Common::degrees2Radians(45);

    colorBuffer = new ColorBuffer(renderer);
}