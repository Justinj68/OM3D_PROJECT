#include <world.h>

#include <chrono>
#include <ctime>

#include <terrain_generator.h>

// Constructors and Destructor
World::World() {
    _playerPosition = nullptr;
    _chunks = (Chunk**)malloc(WORLD_WIDTH * WORLD_HEIGHT * WORLD_WIDTH * sizeof(Chunk*));
    for (int x = 0; x < WORLD_WIDTH; ++x) {
        for (int y = 0; y < WORLD_HEIGHT; ++y) {
            for (int z = 0; z < WORLD_WIDTH; z++) {
                _chunks[x * WORLD_WIDTH * WORLD_HEIGHT + y * WORLD_WIDTH + z] = new Chunk();
            }
        }
    }
}

World::World(glm::vec3* playerPosition) {
    _playerPosition = playerPosition;
    _chunks = (Chunk**)malloc(WORLD_WIDTH * WORLD_HEIGHT * WORLD_WIDTH * sizeof(Chunk*));
    for (int x = 0; x < WORLD_WIDTH; ++x) {
        for (int y = 0; y < WORLD_HEIGHT; ++y) {
            for (int z = 0; z < WORLD_WIDTH; z++) {
                _chunks[x * WORLD_WIDTH * WORLD_HEIGHT + y * WORLD_WIDTH + z] = new Chunk();
            }
        }
    }
}

World::~World() {
    delete[] _chunks;
}


// Chunk Management Functions
Chunk* World::getChunk(int x, int y, int z) {
    if (x < 0 || x >= WORLD_WIDTH || y < 0 || y >= WORLD_HEIGHT || z < 0 || z >= WORLD_WIDTH)
        return nullptr;
    return _chunks[x * WORLD_WIDTH * WORLD_HEIGHT + y * WORLD_WIDTH + z];
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

void printTimeStats(double seconds_count, int nbChunks, const std::string& stage) {
    double secondsPerChunk = seconds_count / nbChunks;
    int avgMinutes = (int)(secondsPerChunk / 60.0);
    double avgSeconds = secondsPerChunk - (avgMinutes * 60);
    int minutes = (int)(seconds_count / 60.0);
    double seconds = seconds_count - (minutes * 60);

    std::cout << stage << " completed: " << std::endl
              << "\tTotal time: " << minutes << "m " << seconds << "s" << std::endl
              << "\tAvg. time/chunk: " << avgMinutes << "m " << avgSeconds << "s" << std::endl;
}

// World Configuration and Initialization Functions
void World::build(BuildMode mode) {
    int nbChunks = WORLD_WIDTH * WORLD_HEIGHT * WORLD_WIDTH;
    std::cout << "Defining chunks data..." << std::endl;
    auto startTime = std::chrono::system_clock::now();

    TerrainGenerator terrainGenerator(123456789, 0.01f, 60, 300);
    for (int x = 0; x < WORLD_WIDTH; ++x) {
        for (int y = 0; y < WORLD_HEIGHT; ++y) {
            for (int z = 0; z < WORLD_WIDTH; ++z) {
                Chunk* chunk = _chunks[x * WORLD_WIDTH * WORLD_HEIGHT + y * WORLD_WIDTH + z];
                chunk->setPosition(x - WORLD_WIDTH / 2, y, z - WORLD_WIDTH / 2);
                chunk->defineVoxelData(terrainGenerator);
                chunk->defineCave(terrainGenerator);
            }
        }
    }

    auto endTime = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = endTime - startTime;
    double seconds_count = elapsed_seconds.count();

    printTimeStats(seconds_count, nbChunks, "Voxel data definition");

    std::cout << "Building meshes..." << std::endl;
    startTime = std::chrono::system_clock::now();
    for (int x = 0; x < WORLD_WIDTH; ++x) {
        for (int y = 0; y < WORLD_HEIGHT; ++y) {
            for (int z = 0; z < WORLD_WIDTH; ++z) {
                Chunk* chunk = _chunks[x * WORLD_WIDTH * WORLD_HEIGHT + y * WORLD_WIDTH + z];
                if (mode == CLASSIC)
                    chunk->build(getNeighboringChunks(x, y, z));
                else
                    chunk->greedyBuild(getNeighboringChunks(x, y, z), &facesCount);
            }
        }
    }
    endTime = std::chrono::system_clock::now();
    elapsed_seconds = endTime - startTime;
    seconds_count = elapsed_seconds.count();

    printTimeStats(seconds_count, nbChunks, "Mesh building");
}


// Rendering Function
void World::render(const Shader &shaderProgram) {
    for (int i = 0; i < WORLD_WIDTH * WORLD_HEIGHT * WORLD_WIDTH; ++i) {
        Chunk *chunk = _chunks[i];
        if (chunk == nullptr)
            continue;
        glm::vec3 &centerPosition = chunk->getCenterPosition();
        centerPosition.y = (*_playerPosition).y;
        float distance = glm::distance(*_playerPosition, centerPosition);
        glm::vec3 &center = chunk->getCenterPosition();
        if (distance <= 256.0)
            chunk->render(shaderProgram);
    }
}
