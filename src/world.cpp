#include <world.h>

World::World() {
    _chunks = new Chunk[WORLD_WIDTH * WORLD_HEIGHT * WORLD_WIDTH];
}


std::vector<Chunk*> World::getNeighboringChunks(int x, int y, int z) {
    // Back, Front, Left, Right, Bottom, Top
    std::vector<Chunk*> neighbors;
    neighbors.push_back(getChunk(x, y, z + 1));
    neighbors.push_back(getChunk(x, y, z - 1));
    neighbors.push_back(getChunk(x - 1, y, z));
    neighbors.push_back(getChunk(x + 1, y, z));
    neighbors.push_back(getChunk(x, y - 1, z));
    neighbors.push_back(getChunk(x, y + 1, z));
    return neighbors;
}


void World::build() {
    for (int x = 0; x < WORLD_WIDTH; ++x) {
        for (int y = 0; y < WORLD_HEIGHT; ++y) {
            for (int z = 0; z < WORLD_WIDTH; ++z) {
                Chunk& chunk = _chunks[x * WORLD_WIDTH * WORLD_HEIGHT + y * WORLD_WIDTH + z];
                chunk.setPosition(x, y, z);
                chunk.defineVoxelData();
            }
        }
    }
    std::cout << "Voxel data defined" << std::endl << "Building meshes";
    for (int x = 0; x < WORLD_WIDTH; ++x) {
        std::cout << ".";
        for (int y = 0; y < WORLD_HEIGHT; ++y) {
            for (int z = 0; z < WORLD_WIDTH; ++z) {
                Chunk& chunk = _chunks[x * WORLD_WIDTH * WORLD_HEIGHT + y * WORLD_WIDTH + z];
                chunk.build(getNeighboringChunks(x, y, z));
            }
        }
    }
    std::cout << std::endl << "Meshes built." << std::endl;
}


Chunk* World::getChunk(int x, int y, int z) {
    if (x < 0 || x >= WORLD_WIDTH || y < 0 || y >= WORLD_HEIGHT || z < 0 || z >= WORLD_WIDTH)
        return nullptr;
    return &(_chunks[x * WORLD_WIDTH * WORLD_HEIGHT + y * WORLD_WIDTH + z]);
}

void World::render(const Shader &shaderProgram) {
    for (int i = 0; i < WORLD_WIDTH * WORLD_HEIGHT * WORLD_WIDTH; ++i) {
        _chunks[i].render(shaderProgram);
    }
}


World::~World() {
    delete[] _chunks;
}