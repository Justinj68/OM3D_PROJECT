#include <world.h>

World::World() {
    _chunks = new Chunk[WORLD_WIDTH * WORLD_HEIGHT * WORLD_WIDTH];
}


void World::build() {
    for (int x = 0; x < WORLD_WIDTH; ++x) {
        for (int y = 0; y < WORLD_HEIGHT; ++y) {
            for (int z = 0; z < WORLD_WIDTH; ++z) {
                Chunk &chunk = _chunks[x * WORLD_WIDTH * WORLD_HEIGHT + y * WORLD_HEIGHT + z];
                chunk.setPosition(x, y, z);
                chunk.defineVoxelData();
                chunk.build();
            }
        }
    }
}


void World::render(const Shader &shaderProgram) {
    for (int i = 0; i < WORLD_WIDTH * WORLD_HEIGHT * WORLD_WIDTH; ++i) {
        _chunks[i].render(shaderProgram);
    }
}


World::~World() {
    delete[] _chunks;
}