/*
    Windows 10/11
    SchumakerTeam
    Hudson Schumaker
*/

#include "include/Map.hpp"

Map::Map() {}

void Map::render(SDL_Renderer* renderer) {
    for (int i = 0; i < Common::MAP_NUM_ROWS; i++) {
        for (int j = 0; j < Common::MAP_NUM_COLS; j++) {
            int tileX = j * Common::TILE_SIZE;
            int tileY = i * Common::TILE_SIZE;
            int tileColor = map[i][j] != 0 ? 255 : 0;

            SDL_SetRenderDrawColor(renderer, tileColor, tileColor, tileColor, 255);
            SDL_Rect mapTileRect = {
                tileX * Common::MINIMAP_SCALE_FACTOR,
                tileY * Common::MINIMAP_SCALE_FACTOR,
                Common::TILE_SIZE * Common::MINIMAP_SCALE_FACTOR,
                Common::TILE_SIZE * Common::MINIMAP_SCALE_FACTOR
            };
            SDL_RenderFillRect(renderer, &mapTileRect);
        }
    }
}