/*
    Windows 10/11
    SchumakerTeam
    Hudson Schumaker
*/

#pragma once
#include "Common.hpp"
#include "Player.hpp" 
#include "Map.hpp" 
#include "Ray.hpp" 
#include "ColorBuffer.hpp" 

class Engine final {
private:
    SDL_Renderer* renderer;
    Player player;
    Map map;
    Ray rays[Common::NUM_RAYS];
    ColorBuffer* colorBuffer = nullptr;

    bool isRunning = false;
    int ticksLastFrame = 0;

    void load();
    void input();
    void update();
    void render();
    void loop();
    void generate3DProjection();
    
public:
    Engine(SDL_Renderer* renderer);
    ~Engine() = default;
    void start();
};