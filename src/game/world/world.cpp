//
// Created by andy on 6/25/2025.
//

#include "world.hpp"

#include <iostream>
#include <glm/gtx/string_cast.hpp>

namespace game {
    Chunk::Chunk(const glm::ivec2& chunk, const std::shared_ptr<WorldGenerator>& generator) {
        std::cout << "Load " << glm::to_string(chunk) << std::endl;
        if (chunk.x % 2 != chunk.y % 2) {
            for (uint32_t y = 0; y < CHUNK_SIZE; ++y) {
                for (uint32_t x = 0; x < CHUNK_SIZE; ++x) {
                    (*this)[x, y].tile_id = 1;
                }
            }
        }
    }

    Chunk::~Chunk() { save(); };

    void Chunk::save() {
        // TODO
    }

    Tile&       Chunk::operator[](const uint32_t x, const uint32_t y) { return m_Tiles[y * CHUNK_SIZE + x]; }
    const Tile& Chunk::operator[](const uint32_t x, const uint32_t y) const { return m_Tiles[y * CHUNK_SIZE + x]; }
    Tile&       Chunk::operator[](const glm::uvec2& local) { return m_Tiles[local.y * CHUNK_SIZE + local.x]; }
    const Tile& Chunk::operator[](const glm::uvec2& local) const { return m_Tiles[local.y * CHUNK_SIZE + local.x]; }

    void Chunk::markDirty() { m_Dirty = true; }

    void Chunk::rebuildMesh() {
        // TODO
    }

    static Chunk* chunk_load(const glm::ivec2& chunk, World* world) { return new Chunk(chunk, world->generator()); }

    static void chunk_unload(Chunk* chunk, World* world) { delete chunk; }

    World::World() : m_Chunks(MAX_CHUNKS, chunk_load, chunk_unload, this) {}

    World::~World() {}

    std::vector<Chunk*> World::chunksInView(const glm::vec2& minCornerPixel, const glm::vec2& maxCornerPixel) {
        const glm::ivec2 minCorner = worldToChunk(minCornerPixel);
        const glm::ivec2 maxCorner = worldToChunk(maxCornerPixel);

        std::vector<Chunk*> chunks;
        chunks.reserve((maxCorner.x - minCorner.x + 1) * (maxCorner.y - minCorner.y + 1));
        for (int x = minCorner.x; x <= maxCorner.x; x++) {
            for (int y = minCorner.y; y <= maxCorner.y; y++) {
                chunks.push_back(m_Chunks.get({x, y}));
            }
        }

        return chunks;
    }
} // namespace game
