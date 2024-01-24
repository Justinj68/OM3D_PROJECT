#include <world.h>

#include <chrono>
#include <ctime>
#include <debug_color.h>

#include <terrain_generator.h>

// Constructors and Destructor
World::World() {
    _width = WORLD_WIDTH;
    _height = WORLD_HEIGHT;
    _playerPosition = nullptr;
    _chunks = (Chunk**)malloc(_width * _height * _width * sizeof(Chunk*));
    for (int x = 0; x < _width; ++x) {
        for (int y = 0; y < _height; ++y) {
            for (int z = 0; z < _width; z++) {
                _chunks[x * _width * _height + y * _width + z] = new Chunk();
            }
        }
    }
}

World::World(glm::vec3* playerPosition, int world_width, int world_height) {
    _width = world_width;
    _height = world_height;
    _playerPosition = playerPosition;
    _chunks = (Chunk**)malloc(_width * _height * _width * sizeof(Chunk*));
    for (int x = 0; x < _width; ++x) {
        for (int y = 0; y < _height; ++y) {
            for (int z = 0; z < _width; z++) {
                _chunks[x * _width * _height + y * _width + z] = new Chunk();
            }
        }
    }
}

World::~World() {
    delete[] _chunks;
}


// Chunk Management Functions
Chunk* World::getChunk(int x, int y, int z) {
    if (x < 0 || x >= _width || y < 0 || y >= _height || z < 0 || z >= _width)
        return nullptr;
    return _chunks[x * _width * _height + y * _width + z];
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

    std::cout << Color::YELLOW << stage << " completed: " << std::endl
              << "\tTotal time: " << Color::BLUE << minutes << Color::YELLOW << "m " << Color::BLUE << seconds << Color::YELLOW << "s" << std::endl
              << "\tAvg. time/chunk: " << Color::BLUE << avgMinutes << Color::YELLOW << "m " << Color::BLUE << avgSeconds << Color::YELLOW << "s"  << Color::RESET << std::endl;
}

// World Configuration and Initialization Functions
void World::build(BuildMode mode) {
    int nbChunks = _width * _height * _width;
    std::cout << Color::YELLOW << "Defining chunk data..." << Color::RESET << std::endl;
    auto startTime = std::chrono::system_clock::now();

    TerrainGenerator terrainGenerator(123456789, 0.01f, (int)(0.2 * (float)_height) * CHUNK_DIM + 1, (_height - 1) * CHUNK_DIM);
    for (int x = 0; x < _width; ++x) {
        for (int y = 0; y < _height; ++y) {
            for (int z = 0; z < _width; ++z) {
                Chunk* chunk = _chunks[x * _width * _height + y * _width + z];
                chunk->setPosition(x - _width / 2, y, z - _width / 2);
                chunk->defineVoxelData(terrainGenerator);
                chunk->defineCave(terrainGenerator);
            }
        }
    }

    auto endTime = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = endTime - startTime;
    double seconds_count = elapsed_seconds.count();

    printTimeStats(seconds_count, nbChunks, "Chunk data definition");

    std::cout << Color::YELLOW << "Building meshes..." << Color::RESET << std::endl;
    startTime = std::chrono::system_clock::now();
    for (int x = 0; x < _width; ++x) {
        for (int y = 0; y < _height; ++y) {
            for (int z = 0; z < _width; ++z) {
                Chunk* chunk = _chunks[x * _width * _height + y * _width + z];
                if (mode == CLASSIC)
                    chunk->build(getNeighboringChunks(x, y, z), &facesCount);
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
    for (int i = 0; i < _width * _height * _width; ++i) {
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


int World::getHeight() const {
    return _height;
}
