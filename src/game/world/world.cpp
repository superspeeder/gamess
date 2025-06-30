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

    TileInstanceData TileInstanceData::of(unsigned int x, unsigned int y, const Tile tile, unsigned char borderInfo) {
        return TileInstanceData{.localTilePos = {x, y}, .tileId = tile.tile_id, .tileBorderInfo = borderInfo};
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
        unsigned int binding             = m_ChunkVao->nextBinding();
        unsigned int tilePosAttribute    = m_ChunkVao->nextAttribute();
        unsigned int tileIdAttribute     = m_ChunkVao->nextAttribute();
        unsigned int tileBorderAttribute = m_ChunkVao->nextAttribute();

        glBindVertexBuffer(binding, m_InstanceBuffer->getHandle(), 0, sizeof(TileInstanceData));
        glVertexArrayBindingDivisor(m_ChunkVao->getHandle(), binding, 1);

        glVertexAttribBinding(tilePosAttribute, binding);
        glVertexAttribIFormat(tilePosAttribute, 2, GL_UNSIGNED_INT, offsetof(TileInstanceData, localTilePos));
        glEnableVertexAttribArray(tilePosAttribute);

        glVertexAttribBinding(tileIdAttribute, binding);
        glVertexAttribIFormat(tileIdAttribute, 1, GL_UNSIGNED_INT, offsetof(TileInstanceData, tileId));
        glEnableVertexAttribArray(tileIdAttribute);

        glVertexAttribBinding(tileBorderAttribute, binding);
        glVertexAttribIFormat(tileBorderAttribute, 1, GL_UNSIGNED_BYTE, offsetof(TileInstanceData, tileBorderInfo));
        glEnableVertexAttribArray(tileBorderAttribute);
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

    void Chunk::setTile(const uint32_t x, const uint32_t y, const Tile &tile) {
        markDirty();
        m_Tiles[y * CHUNK_SIZE + x]            = tile;
        m_TileInstanceData[y * CHUNK_SIZE + x] = TileInstanceData::of(x, y, tile, borderInfoFor(x, y));

        if (tile.tile_id == 0) {
            if (x > 0) {
                m_TileInstanceData[y * CHUNK_SIZE + x - 1].tileBorderInfo |= 0b100;
            }

            if (y > 0) {
                m_TileInstanceData[(y - 1) * CHUNK_SIZE + x].tileBorderInfo |= 0b1000;
            }

            if (x < CHUNK_SIZE - 1) {
                m_TileInstanceData[y * CHUNK_SIZE + x + 1].tileBorderInfo |= 0b1;
            }

            if (y < CHUNK_SIZE - 1) {
                m_TileInstanceData[(y + 1) * CHUNK_SIZE + x].tileBorderInfo |= 0b10;
            }
        } else {
            if (x > 0) {
                m_TileInstanceData[y * CHUNK_SIZE + x - 1].tileBorderInfo &= 0b1011;
            }

            if (y > 0) {
                m_TileInstanceData[(y - 1) * CHUNK_SIZE + x].tileBorderInfo &= 0b0111;
            }

            if (x < CHUNK_SIZE - 1) {
                m_TileInstanceData[y * CHUNK_SIZE + x + 1].tileBorderInfo &= 0b1110;
            }

            if (y < CHUNK_SIZE - 1) {
                m_TileInstanceData[(y + 1) * CHUNK_SIZE + x].tileBorderInfo &= 0b1101;
            }
        }
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

    bool Chunk::isAir(uint32_t x, uint32_t y) const {
        return m_Tiles[y * CHUNK_SIZE + x].tile_id == 0;
    }

    void Chunk::onNeighborLoaded(Chunk *chunk) {
        for (uint32_t i = 0; i < CHUNK_SIZE; i++) {
            // borderings
            if (chunk->m_ChunkPosition.x > m_ChunkPosition.x && chunk->isAir(0, i)) {
                m_TileInstanceData[i * CHUNK_SIZE + CHUNK_SIZE - 1].tileBorderInfo |= 0b100;
            } else if (chunk->m_ChunkPosition.y > m_ChunkPosition.y && chunk->isAir(i, 0)) {
                m_TileInstanceData[CHUNK_SIZE * (CHUNK_SIZE - 1) + i].tileBorderInfo |= 0b1000;
            } else if (chunk->m_ChunkPosition.x < m_ChunkPosition.x && chunk->isAir(CHUNK_SIZE - 1, i)) {
                m_TileInstanceData[CHUNK_SIZE * i].tileBorderInfo |= 0b1;
            } else if (chunk->m_ChunkPosition.y < m_ChunkPosition.y && chunk->isAir(i, CHUNK_SIZE - 1)) {
                m_TileInstanceData[i].tileBorderInfo |= 0b10;
            }
        }
    }

    static Chunk *chunk_load(const glm::ivec2 &chunkPos, World *world) {
        auto chunk = new Chunk(chunkPos, world);
        world->onLoadedChunk(chunk);
        return chunk;
    }

    static void chunk_unload(Chunk *chunk, World *world) {
        delete chunk;
    }

    unsigned char Chunk::borderInfoFor(const uint32_t x, const uint32_t y) const {
        unsigned char borderInfo = 0;
        if (x > 0) {
            borderInfo |= isAir(x - 1, y) ? 0b1 : 0b0;
        }

        if (y > 0) {
            borderInfo |= isAir(x, y - 1) ? 0b10 : 0b0;
        }

        if (x < CHUNK_SIZE - 1) {
            borderInfo |= isAir(x + 1, y) ? 0b100 : 0b0;
        }

        if (y < CHUNK_SIZE - 1) {
            borderInfo |= isAir(x, y + 1) ? 0b1000 : 0b0;
        }

        return borderInfo;
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

    void World::onLoadedChunk(Chunk *chunk) {
        Chunk* left = m_Chunks.get_if_present(chunk->position() - glm::ivec2(1, 0));
        Chunk* right = m_Chunks.get_if_present(chunk->position() + glm::ivec2(1, 0));
        Chunk* down = m_Chunks.get_if_present(chunk->position() - glm::ivec2(0, 1));
        Chunk* up = m_Chunks.get_if_present(chunk->position() + glm::ivec2(0, 1));

        if (left) {
            left->onNeighborLoaded(chunk);
            chunk->onNeighborLoaded(left);
        }

        if (right) {
            right->onNeighborLoaded(chunk);
            chunk->onNeighborLoaded(right);
        }

        if (down) {
            down->onNeighborLoaded(chunk);
            chunk->onNeighborLoaded(down);
        }

        if (up) {
            up->onNeighborLoaded(chunk);
            chunk->onNeighborLoaded(up);
        }
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
