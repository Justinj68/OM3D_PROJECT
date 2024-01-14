#pragma once

#include <chunk.h>

#define WORLD_WIDTH     25
#define WORLD_HEIGHT     1

class World {
    public:
        World();

        void build();
        void render(const Shader &shaderProgram);

        ~World();

    private:
        Chunk* _chunks;
};