//
// Created by andy on 6/26/2025.
//

#include "world_generator.hpp"

#include <chrono>
#include <glm/common.hpp>

namespace game {
    WorldGenerator::WorldGenerator() {
        m_Seed = std::chrono::system_clock::now().time_since_epoch().count();
        m_Noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
        m_Noise2.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);

        m_Noise.SetSeed(m_Seed);
        m_Noise2.SetSeed(m_Seed + 1);

        m_Noise.SetDomainWarpType(FastNoiseLite::DomainWarpType_OpenSimplex2);
        m_Noise.SetDomainWarpAmp(8.0);

        m_Noise.SetFractalOctaves(3);
        m_Noise.SetFractalType(FastNoiseLite::FractalType_DomainWarpProgressive);

        m_Noise2.SetFractalOctaves(3);
        m_Noise2.SetFractalType(FastNoiseLite::FractalType_FBm);

        m_Noise3.SetSeed(m_Seed + 2);
        m_Noise3.SetDomainWarpAmp(16.0f);
        m_Noise3.SetDomainWarpType(FastNoiseLite::DomainWarpType_OpenSimplex2Reduced);
        m_Noise3.SetNoiseType(FastNoiseLite::NoiseType_Cellular);
        m_Noise3.SetFractalType(FastNoiseLite::FractalType_Ridged);
        m_Noise3.SetFractalGain(0.25);
        m_Noise3.SetFractalOctaves(4);
        m_Noise3.SetCellularDistanceFunction(FastNoiseLite::CellularDistanceFunction_Hybrid);
    }

    WorldGenerator::~WorldGenerator() = default;

    Tile WorldGenerator::generateTile(const glm::ivec2 &tileCoords) {
        glm::vec2 ftc = tileCoords;
        m_Noise.DomainWarp(ftc.x, ftc.y);
        auto h = 64.0f * m_Noise2.GetNoise(ftc.x * 0.5f, 0.0f);

        auto stoneHeight = 8.0f * m_Noise2.GetNoise(ftc.x * 0.5f, 0.5f) - 80.0f;

        if (tileCoords.y <= h) {
            if (h - tileCoords.y <= 1.0) {
                return Tile{2};
            }
            if (tileCoords.y <= stoneHeight) {
                auto factor = 0.75f + 0.25f * glm::smoothstep(stoneHeight - 480.0f, stoneHeight, static_cast<float>(tileCoords.y));
                auto depth = (1.0f + m_Noise3.GetNoise(ftc.x / 3.0f, ftc.y / 3.0f)) / 2.0f;
                if (depth > 1.0f * factor) {
                    return Tile { 0 };
                }
                if (depth > 0.9f * factor) {
                    return Tile { 4 };
                }
                return Tile { 6 };
            }
            return Tile{1};
        }
        if (tileCoords.y < 0) {
            return Tile{3};
        }
        return Tile{0};
    }
} // namespace game
