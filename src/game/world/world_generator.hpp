//
// Created by andy on 6/26/2025.
//

#pragma once
#include "FastNoiseLite.h"
#include "game/world/tile.hpp"

#include <glm/vec2.hpp>

namespace game {

    class WorldGenerator {
      public:
        WorldGenerator();
        ~WorldGenerator();

        Tile generateTile(const glm::ivec2& tileCoords);

      private:
        int m_Seed;
        FastNoiseLite m_Noise;
        FastNoiseLite m_Noise2;
        FastNoiseLite m_Noise3;
    };

} // namespace game
