#pragma once

#include <vector>

#include <chunk.h>

#define WORLD_WIDTH     10
#define WORLD_HEIGHT     3

class World {
    public:
        World();

        void build();
        void render(const Shader &shaderProgram);

        Chunk* getChunk(int x, int y, int z);

        ~World();

    private:
        Chunk* _chunks;

        std::vector<Chunk*> getNeighboringChunks(int x, int y, int z);
};