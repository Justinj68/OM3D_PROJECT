#pragma once

#include <vector>

#include <camera.h>
#include <chunk.h>

#define WORLD_WIDTH    10
#define WORLD_HEIGHT   10

enum BuildMode : unsigned char {
    CLASSIC,
    GREEDY
};

class World {
    public:
        World();
        World(glm::vec3 *playerPosition, int world_width, int world_height);
        ~World();

        void build(BuildMode mode);
        void render(const Shader &shaderProgram);

        Chunk* getChunk(int x, int y, int z);

        int getHeight() const;
        
        int facesCount = 0;
    private:
        glm::vec3* _playerPosition;
        Chunk** _chunks;

        int _width;
        int _height;

        std::vector<Chunk*> getNeighboringChunks(int x, int y, int z);
};