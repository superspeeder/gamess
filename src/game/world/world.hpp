//
// Created by andy on 6/25/2025.
//

#pragma once

#include <array>
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>
#include <memory>

#include "game/utils/lru_cache.hpp"
#include "game/world/tile.hpp"
#include "game/world/world_generator.hpp"

#include "game/render/buffer.hpp"
#include "game/render/vertex_array.hpp"

namespace game {
    class World;

    struct TileInstanceData {
        unsigned int tileId;
    };

    class Chunk {
    public:
        static constexpr uint32_t CHUNK_SIZE = 64;

        Chunk(const glm::ivec2& chunk, World* world);
        ~Chunk();

        void save();

        Tile& operator[](uint32_t x, uint32_t y);
        const Tile& operator[](uint32_t x, uint32_t y) const;

        Tile& operator[](const glm::uvec2& local);
        const Tile& operator[](const glm::uvec2& local) const;

        void markDirty();
        void rebuildMesh();

    private:
        std::array<Tile, CHUNK_SIZE * CHUNK_SIZE> m_Tiles;
        bool m_Dirty = false;
        World* m_World;

        std::shared_ptr<VertexArray> m_ChunkVao;
        std::shared_ptr<Buffer> m_InstanceBuffer;
    };

    class World {
    public:
        static constexpr std::size_t MAX_CHUNKS = 32;
        static constexpr uint32_t    TILE_SIZE  = 16;

        World();
        ~World();

        std::vector<Chunk*> chunksInView(const glm::vec2& minCorner, const glm::vec2& maxCorner);

        [[nodiscard]] std::shared_ptr<WorldGenerator> generator() const { return m_Generator; }

        inline static glm::ivec2 worldToChunk(const glm::vec2& world) noexcept {
            return glm::floor(world / static_cast<float>(TILE_SIZE) / static_cast<float>(Chunk::CHUNK_SIZE));
        }

        inline static glm::ivec2 worldToTile(const glm::vec2& world) noexcept {
            return glm::floor(world / static_cast<float>(TILE_SIZE));
        }

    private:
        lru_cache<glm::ivec2, Chunk, World*> m_Chunks;
        std::shared_ptr<WorldGenerator>      m_Generator;

        std::shared_ptr<Buffer> m_TileVbo;
    };

} // namespace game
