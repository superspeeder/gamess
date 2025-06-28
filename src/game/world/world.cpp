//
// Created by andy on 6/25/2025.
//

#include "world.hpp"


#include <glm/gtx/string_cast.hpp>
#include <iostream>

#include <glm/gtc/noise.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>

#include <cmath>

namespace game {
    const std::vector<unsigned int> PRIMARY_TILE_ATTRIBUTES = {2};
    constexpr static glm::uvec2     ATLAS_SIZE              = glm::uvec2(4, 4);

    TileInstanceData TileInstanceData::of(unsigned int x, unsigned int y, const Tile tile) {
        return TileInstanceData{.localTilePos = {x, y}, .tileId = tile.tile_id};
    }

    Chunk::Chunk(const glm::ivec2 &chunk, World *world) : m_ChunkPosition(chunk), m_World(world) {

        std::cout << "Load " << glm::to_string(chunk) << std::endl;
        for (uint32_t y = 0; y < CHUNK_SIZE; ++y) {
            int wy = y + chunk.y * CHUNK_SIZE;
            for (uint32_t x = 0; x < CHUNK_SIZE; ++x) {
                int wx = x + chunk.x * CHUNK_SIZE;
                setTile(x, y, world->generator()->generateTile({wx, wy}));
            }
        }

        m_InstanceBuffer = std::make_shared<Buffer>(
            sizeof(TileInstanceData) * CHUNK_SIZE * CHUNK_SIZE, m_TileInstanceData.data(), BufferUsage::DynamicDraw
        );
        m_ChunkVao = std::make_shared<VertexArray>();
        m_ChunkVao->bindVertexBuffer(world->primaryTileVbo(), PRIMARY_TILE_ATTRIBUTES);

        m_ChunkVao->bind();
        m_InstanceBuffer->bind(BufferTarget::Array);
        unsigned int binding          = m_ChunkVao->nextBinding();
        unsigned int tilePosAttribute = m_ChunkVao->nextAttribute();
        unsigned int tileIdAttribute  = m_ChunkVao->nextAttribute();

        glBindVertexBuffer(binding, m_InstanceBuffer->getHandle(), 0, sizeof(TileInstanceData));
        glVertexArrayBindingDivisor(m_ChunkVao->getHandle(), binding, 1);

        glVertexAttribBinding(tilePosAttribute, binding);
        glVertexAttribIFormat(tilePosAttribute, 2, GL_UNSIGNED_INT, offsetof(TileInstanceData, localTilePos));
        glEnableVertexAttribArray(tilePosAttribute);

        glVertexAttribBinding(tileIdAttribute, binding);
        glVertexAttribIFormat(tileIdAttribute, 1, GL_UNSIGNED_INT, offsetof(TileInstanceData, tileId));
        glEnableVertexAttribArray(tileIdAttribute);
    }

    Chunk::~Chunk() {
        save();
    };

    void Chunk::save() {
        // TODO
    }

    const Tile &Chunk::operator[](const uint32_t x, const uint32_t y) const {
        return m_Tiles[y * CHUNK_SIZE + x];
    }

    const Tile &Chunk::operator[](const glm::uvec2 &local) const {
        return m_Tiles[local.y * CHUNK_SIZE + local.x];
    }

    void Chunk::setTile(uint32_t x, uint32_t y, const Tile &tile) {
        markDirty();
        m_Tiles[y * CHUNK_SIZE + x]            = tile;
        m_TileInstanceData[y * CHUNK_SIZE + x] = TileInstanceData::of(x, y, tile);
    }

    void Chunk::setTile(const glm::uvec2 &local, const Tile &tile) {
        setTile(local.x, local.y, tile);
    }

    void Chunk::markDirty() {
        m_Dirty = true;
    }

    void Chunk::rebuildMesh() {
        m_InstanceBuffer->subdata(m_TileInstanceData.data(), m_TileInstanceData.size() * sizeof(TileInstanceData), 0);
        m_Dirty = false;
    }

    bool Chunk::needsRebuildMesh() const {
        return m_Dirty;
    }

    void Chunk::drawCall() const {
        m_ChunkVao->bind();

        glUniform2f(
            m_World->tileShader()->getUniformLocation("uChunkOffset"),
            static_cast<float>(m_ChunkPosition.x * static_cast<int>(CHUNK_SIZE)),
            static_cast<float>(m_ChunkPosition.y * static_cast<int>(CHUNK_SIZE))
        );
        glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, 4, CHUNK_SIZE * CHUNK_SIZE);
    }

    static Chunk *chunk_load(const glm::ivec2 &chunk, World *world) {
        return new Chunk(chunk, world);
    }

    static void chunk_unload(Chunk *chunk, World *world) {
        delete chunk;
    }

    World::World() : m_Chunks(MAX_CHUNKS, chunk_load, chunk_unload, this) {
        std::vector<float> tileVertices = {0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f};
        m_TileVbo                       = std::make_shared<Buffer>(tileVertices, BufferUsage::StaticDraw);
        m_TileShader                    = Shader::loadSimpleShader("res/shaders/tile.vert", "res/shaders/tile.frag");
        m_AtlasTexture                  = Texture::load("res/textures/tiles.png");
        m_Generator                     = std::make_shared<WorldGenerator>();
    }

    World::~World() {}

    std::vector<Chunk *> World::chunksInView(const glm::vec2 &minCornerTile, const glm::vec2 &maxCornerTile) {
        const glm::ivec2 minCorner = worldToChunk(minCornerTile);
        const glm::ivec2 maxCorner = worldToChunk(maxCornerTile);

        std::vector<Chunk *> chunks;
        chunks.reserve((maxCorner.x - minCorner.x + 1) * (maxCorner.y - minCorner.y + 1));
        for (int x = minCorner.x; x <= maxCorner.x; x++) {
            for (int y = minCorner.y; y <= maxCorner.y; y++) {
                chunks.push_back(m_Chunks.get({x, y}));
            }
        }

        return chunks;
    }

    void World::render(const Camera &camera) {

        m_TileShader->use();
        glUniformMatrix4fv(
            m_TileShader->getUniformLocation("uTransformMatrix"), 1, GL_FALSE, glm::value_ptr(camera.combinedMatrix())
        );
        glUniform2ui(m_TileShader->getUniformLocation("uAtlasSize"), ATLAS_SIZE.x, ATLAS_SIZE.y);

        m_AtlasTexture->bind(0);
        glUniform1i(m_TileShader->getUniformLocation("uTexture"), 0);

        for (const auto chunks = chunksInView(camera.minCorner(), camera.maxCorner()); const auto &chunk : chunks) {
            if (chunk->needsRebuildMesh()) {
                chunk->rebuildMesh();
            }
            chunk->drawCall();
        }
    }
} // namespace game
