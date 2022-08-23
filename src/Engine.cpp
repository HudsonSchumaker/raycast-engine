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
    Ray::castAllRays(rays, &player, &map);
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

    generate3DProjection();
    colorBuffer->render();
    colorBuffer->clear();

    map.render(renderer);
    Ray::renderAllRays(renderer, rays, &player);
    player.render(renderer);

    SDL_RenderPresent(renderer);
}

void Engine::generate3DProjection() {
    for (int i = 0; i < Common::NUM_RAYS; i++) {
        float perpDistance = rays[i].distance * cos(rays[i].rayAngle - player.rotationAngle);
        float distanceProjPlane = (Common::WIDTH / 2) / tan(Common::FOV_ANGLE / 2);
        float projectedWallHeight = (Common::TILE_SIZE / perpDistance) * distanceProjPlane;

        int wallStripHeight = (int)projectedWallHeight;

        int wallTopPixel = (Common::HEIGHT / 2) - (wallStripHeight / 2);
        wallTopPixel = wallTopPixel < 0 ? 0 : wallTopPixel;

        int wallBottomPixel = (Common::HEIGHT / 2) + (wallStripHeight / 2);
        wallBottomPixel = wallBottomPixel > Common::HEIGHT ? Common::HEIGHT : wallBottomPixel;

        // render the wall from wallTopPixel to wallBottomPixel
        for (int y = wallTopPixel; y < wallBottomPixel; y++) {
            colorBuffer->setColorBuffer((Common::WIDTH * y) + i, rays[i].wasHitVertical ? 0xFFFFFFFF : 0xFFCCCCCC);
        }
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