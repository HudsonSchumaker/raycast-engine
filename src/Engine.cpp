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
    
    player.move(deltaTime);
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

    map.render(renderer);
    player.render(renderer);

    SDL_RenderPresent(renderer);
}

void Engine::load() {
    player.x = Common::H_WIDTH;
    player.y = Common::H_HEIGHT;
    player.w = 2;
    player.h = 2;
    player.turnDirection = 0;
    player.walkDirection = 0;
    player.rotationAngle = Common::PI / 2;
    player.walkSpeed = 100;
    player.turnSpeed = Common::degrees2Radians(45);

}