#include <chunk.h>

#include <math.h>

long long mod(long long k, long long n);

// Constructors and Destructor
Chunk::Chunk() {
    _position = glm::vec3(0.0);
    _voxels = new Voxel::Type[CHUNK_DIM * CHUNK_DIM * CHUNK_DIM];
    generateBuffers();
}

Chunk::Chunk(const glm::vec3 &position) {
    _position = position;
    _voxels = new Voxel::Type[CHUNK_DIM * CHUNK_DIM * CHUNK_DIM];
    generateBuffers();
}

Chunk::~Chunk() {
    destroy();
}


bool Chunk::isFaceVisible(int x, int y, int z, FaceDirection direction) {
    if (isVoxelTransparent(x, y, z))
        return false;
    switch (direction)
    {
        case FRONT:
            return isVoxelTransparent(x, y, z - 1);
            break;

        case BACK:
            return isVoxelTransparent(x, y, z + 1);
            break;

        case LEFT:
            return isVoxelTransparent(x - 1, y, z);
            break;

        case RIGHT:
            return isVoxelTransparent(x + 1, y, z);
            break;

        case TOP:
            return isVoxelTransparent(x, y + 1, z);
            break;

        case BOTTOM:
            return isVoxelTransparent(x, y - 1, z);
            break;
        
        default:
            break;
    }
    return true;
}


bool Chunk::isFaceVisible(int x, int y, int z, FaceDirection direction, const std::vector<Chunk*> &neighbors) {
    if (isVoxelTransparent(x, y, z))
        return false;
    switch (direction)
    {
        case FRONT:
            return isVoxelTransparent(x, y, z - 1, neighbors);
            break;

        case BACK:
            return isVoxelTransparent(x, y, z + 1, neighbors);
            break;

        case LEFT:
            return isVoxelTransparent(x - 1, y, z, neighbors);
            break;

        case RIGHT:
            return isVoxelTransparent(x + 1, y, z, neighbors);
            break;

        case TOP:
            return isVoxelTransparent(x, y + 1, z, neighbors);
            break;

        case BOTTOM:
            return isVoxelTransparent(x, y - 1, z, neighbors);
            break;
        
        default:
            break;
    }
    return true;
}


// Configuration, Initialization and Destruction Functions
void Chunk::generateBuffers() {
    glGenVertexArrays(1, &_VAO);
    glGenBuffers(1, &_VBO);
    glGenBuffers(1, &_EBO);
}

void Chunk::deleteBuffers() {
    glDeleteBuffers(1, &_VBO);
    glDeleteBuffers(1, &_EBO);
    glDeleteVertexArrays(1, &_VAO);
}

void Chunk::defineVoxelData(TerrainGenerator &terrainGenerator) {
    for (int x = 0; x < CHUNK_DIM; ++x) {
        for (int y = 0; y < CHUNK_DIM; ++y) {
            for (int z = 0; z < CHUNK_DIM; z++) {
                float xPos = _position.x * CHUNK_DIM + x;
                float yPos = _position.y * CHUNK_DIM + y;
                float zPos = _position.z * CHUNK_DIM + z;

                int height = terrainGenerator.getHeight((int)xPos, (int)zPos);
                Voxel::Type voxelType = yPos > height ? Voxel::AIR : Voxel::STONE;
                _voxels[x * CHUNK_DIM * CHUNK_DIM + y * CHUNK_DIM + z] = voxelType;
                
                if (voxelType != Voxel::AIR)
                    _empty = false;
            }
        }
    }
}

void Chunk::defineCave(TerrainGenerator &terrainGenerator) {
    if (_empty)
        return;
    for (int x = 0; x < CHUNK_DIM; ++x) {
        for (int y = 0; y < CHUNK_DIM; ++y) {
            for (int z = 0; z < CHUNK_DIM; z++) {
                float xPos = _position.x * CHUNK_DIM + x;
                float yPos = _position.y * CHUNK_DIM + y;
                float zPos = _position.z * CHUNK_DIM + z;

                double voidness = terrainGenerator.getNoise((int)xPos, (int)yPos, (int)zPos);
                if (voidness > 0.9)
                    _voxels[x * CHUNK_DIM * CHUNK_DIM + y * CHUNK_DIM + z] = Voxel::AIR;
            }
        }
    }
}

void Chunk::destroy() {
    deleteBuffers();
    delete[] _voxels;
}


// Mesh and Buffer Management Functions
void Chunk::build(const std::vector<Chunk*> &neighbors) {
    if (_empty) {
        _indicesCount = 0;
        return;
    }
    std::vector<PackedVertex> vertices;
    std::vector<GLuint> indices;
    for (int x = 0; x < CHUNK_DIM; ++x) {
        for (int y = 0; y < CHUNK_DIM; ++y) {
            for (int z = 0; z < CHUNK_DIM; z++) {
                if (isVoxelTransparent(x, y, z))
                    continue;
                if (isVoxelTransparent(x, y - 1, z, neighbors))
                    addBottomFace(x, y, z, vertices, indices);
                if (isVoxelTransparent(x, y + 1, z, neighbors))
                    addTopFace(x, y, z, vertices, indices);
                if (isVoxelTransparent(x - 1, y, z, neighbors))
                    addLeftFace(x, y, z, vertices, indices);
                if (isVoxelTransparent(x + 1, y, z, neighbors))
                    addRightFace(x, y, z, vertices, indices);
                if (isVoxelTransparent(x, y, z + 1, neighbors))
                    addFrontFace(x, y, z, vertices, indices);
                if (isVoxelTransparent(x, y, z - 1, neighbors))
                    addBackFace(x, y, z, vertices, indices);
            }
        }
    }
    _indicesCount = indices.size();
    uploadMeshData(vertices, indices);
}

// WORKS
void Chunk::processFaceXPositive(int x, int y, int z, char mask[], const std::vector<Chunk*> &neighbors, std::vector<PackedVertex> &vertices, std::vector<GLuint> &indices) {
    char filter = 0b00001000;
    if ((mask[x * CHUNK_DIM * CHUNK_DIM + y * CHUNK_DIM + z] & filter) || !isFaceVisible(x, y, z, RIGHT, neighbors))
        return;
    
    int depth = 1;
    int height = 1;

    while (z + depth < CHUNK_DIM && isFaceVisible(x, y, z + depth, RIGHT, neighbors) && !mask[x * CHUNK_DIM * CHUNK_DIM + y * CHUNK_DIM + z + depth])
        depth++;

    while (y + height < CHUNK_DIM && isFaceVisible(x, y + height, z, RIGHT, neighbors)) {
        bool addRow = true;
        for (int k = 0; k < depth; k++) {
            if (!isFaceVisible(x, y + height, z + k, RIGHT, neighbors)) {
                addRow = false;
                break;
            }
        }
        if (!addRow)
            break;
        height++;
    }

    addRightFace(x, y, z, y + height, z + depth, vertices, indices);

    for (int dz = 0; dz < depth; dz++) {
        for (int dy = 0; dy < height; dy++) {
            mask[x * CHUNK_DIM * CHUNK_DIM + (y + dy) * CHUNK_DIM + z + dz] |= filter;
        }
    }
}

// WORKS
void Chunk::processFaceXNegative(int x, int y, int z, char mask[], const std::vector<Chunk*> &neighbors, std::vector<PackedVertex> &vertices, std::vector<GLuint> &indices) {
    char filter = 0b00000100;
    if ((mask[x * CHUNK_DIM * CHUNK_DIM + y * CHUNK_DIM + z] & filter) || !isFaceVisible(x, y, z, LEFT, neighbors))
        return;
    
    int depth = 1;
    int height = 1;

    while (z + depth < CHUNK_DIM && isFaceVisible(x, y, z + depth, LEFT, neighbors) && !mask[x * CHUNK_DIM * CHUNK_DIM + y * CHUNK_DIM + z + depth])
        depth++;

    while (y + height < CHUNK_DIM && isFaceVisible(x, y + height, z, LEFT, neighbors)) {
        bool addRow = true;
        for (int k = 0; k < depth; k++) {
            if (!isFaceVisible(x, y + height, z + k, LEFT, neighbors)) {
                addRow = false;
                break;
            }
        }
        if (!addRow)
            break;
        height++;
    }

    addLeftFace(x, y, z, y + height, z + depth, vertices, indices);

    for (int dz = 0; dz < depth; dz++) {
        for (int dy = 0; dy < height; dy++) {
            mask[x * CHUNK_DIM * CHUNK_DIM + (y + dy) * CHUNK_DIM + z + dz] |= filter;
        }
    }
}
void Chunk::processFaceYPositive(int x, int y, int z, char mask[], const std::vector<Chunk*> &neighbors, std::vector<PackedVertex> &vertices, std::vector<GLuint> &indices) {
    char filter = 0b00100000;
    if ((mask[x * CHUNK_DIM * CHUNK_DIM + y * CHUNK_DIM + z] & filter) || !isFaceVisible(x, y, z, TOP, neighbors))
        return;
    
    int width = 1;
    int depth = 1;

    while (x + width < CHUNK_DIM && isFaceVisible(x + width, y, z, TOP, neighbors) && !mask[(x + width) * CHUNK_DIM * CHUNK_DIM + y * CHUNK_DIM + z])
        width++;

    while (z + depth < CHUNK_DIM && isFaceVisible(x, y, z + depth, TOP, neighbors) && !mask[x * CHUNK_DIM * CHUNK_DIM + y * CHUNK_DIM + z + depth]) {
        bool addRow = true;
        for (int k = 0; k < width; k++) {
            if (!isFaceVisible(x + k, y, z + depth, BOTTOM, neighbors)) {
                addRow = false;
                break;
            }
        }
        if (!addRow)
            break;
        depth++;
    }

    addTopFace(y, x, z, x + width, z + depth, vertices, indices);

    for (int dx = 0; dx < width; dx++) {
        for (int dz = 0; dz < depth; dz++) {
            mask[(x + dx) * CHUNK_DIM * CHUNK_DIM + y * CHUNK_DIM + z + dz] |= filter;
        }
    }
}
void Chunk::processFaceYNegative(int x, int y, int z, char mask[], const std::vector<Chunk*> &neighbors, std::vector<PackedVertex> &vertices, std::vector<GLuint> &indices) {
    char filter = 0b00010000;
    if ((mask[x * CHUNK_DIM * CHUNK_DIM + y * CHUNK_DIM + z] & filter) || !isFaceVisible(x, y, z, BOTTOM, neighbors))
        return;
    
    int width = 1;
    int depth = 1;

    while (x + width < CHUNK_DIM && isFaceVisible(x + width, y, z, BOTTOM, neighbors) && !mask[(x + width) * CHUNK_DIM * CHUNK_DIM + y * CHUNK_DIM + z])
        width++;

    while (z + depth < CHUNK_DIM && isFaceVisible(x, y, z + depth, BOTTOM, neighbors) && !mask[x * CHUNK_DIM * CHUNK_DIM + y * CHUNK_DIM + z + depth]) {
        bool addRow = true;
        for (int k = 0; k < width; k++) {
            if (!isFaceVisible(x + k, y, z + depth, BOTTOM, neighbors)) {
                addRow = false;
                break;
            }
        }
        if (!addRow)
            break;
        depth++;
    }

    addBottomFace(y, x, z, x + width, z + depth, vertices, indices);

    for (int dx = 0; dx < width; dx++) {
        for (int dz = 0; dz < depth; dz++) {
            mask[(x + dx) * CHUNK_DIM * CHUNK_DIM + y * CHUNK_DIM + z + dz] |= filter;
        }
    }
}

// WORKS
void Chunk::processFaceZPositive(int x, int y, int z, char mask[], const std::vector<Chunk*> &neighbors, std::vector<PackedVertex> &vertices, std::vector<GLuint> &indices) {
    char filter = 0b00000001;
    if ((mask[x * CHUNK_DIM * CHUNK_DIM + y * CHUNK_DIM + z] & filter) || !isFaceVisible(x, y, z, BACK, neighbors))
        return;
    
    int width = 1;
    int height = 1;

    while (x + width < CHUNK_DIM && isFaceVisible(x + width, y, z, BACK, neighbors))
        width++;

    while (y + height < CHUNK_DIM && isFaceVisible(x, y + height, z, BACK, neighbors) && !mask[x * CHUNK_DIM * CHUNK_DIM + (y + height) * CHUNK_DIM + z]) {
        bool addRow = true;
        for (int k = 0; k < width; k++) {
            if (!isFaceVisible(x + k, y + height, z, BACK, neighbors)) {
                addRow = false;
                break;
            }
        }
        if (!addRow)
            break;
        height++;
    }

    addBackFace(z, x, y, x + width, y + height, vertices, indices);

    for (int dx = 0; dx < width; dx++) {
        for (int dy = 0; dy < height; dy++) {
            mask[(x + dx) * CHUNK_DIM * CHUNK_DIM + (y + dy) * CHUNK_DIM + z] |= filter;
        }
    }
}


// WORKS
void Chunk::processFaceZNegative(int x, int y, int z, char mask[], const std::vector<Chunk*> &neighbors, std::vector<PackedVertex> &vertices, std::vector<GLuint> &indices) {
    char filter = 0b00000010;
    if ((mask[x * CHUNK_DIM * CHUNK_DIM + y * CHUNK_DIM + z] & filter) || !isFaceVisible(x, y, z, FRONT, neighbors))
        return;
    
    int width = 1;
    int height = 1;

    while (x + width < CHUNK_DIM && isFaceVisible(x + width, y, z, FRONT, neighbors))
        width++;

    while (y + height < CHUNK_DIM && isFaceVisible(x, y + height, z, FRONT, neighbors) && !mask[x * CHUNK_DIM * CHUNK_DIM + (y + height) * CHUNK_DIM + z]) {
        bool addRow = true;
        for (int k = 0; k < width; k++) {
            if (!isFaceVisible(x + k, y + height, z, FRONT, neighbors)) {
                addRow = false;
                break;
            }
        }
        if (!addRow)
            break;
        height++;
    }

    addFrontFace(z, x, y, x + width, y + height, vertices, indices);

    for (int dx = 0; dx < width; dx++) {
        for (int dy = 0; dy < height; dy++) {
            mask[(x + dx) * CHUNK_DIM * CHUNK_DIM + (y + dy) * CHUNK_DIM + z] |= filter;
        }
    }
}



void Chunk::greedyBuild(const std::vector<Chunk*> &neighbors) {
    if (_empty) {
        _indicesCount = 0;
        return;
    }
    char mask[CHUNK_DIM * CHUNK_DIM * CHUNK_DIM] = {0};
    std::vector<PackedVertex> vertices;
    std::vector<GLuint> indices;

    for (int x = 0; x < CHUNK_DIM; ++x) {
        for (int y = 0; y < CHUNK_DIM; ++y) {
            for (int z = 0; z < CHUNK_DIM; z++) {
                if (isVoxelTransparent(x, y, z))
                    continue;

                // Traitement des faces pour chaque direction
                processFaceXPositive(x, y, z, mask, neighbors, vertices, indices);
                processFaceXNegative(x, y, z, mask, neighbors, vertices, indices);

                processFaceYPositive(x, y, z, mask, neighbors, vertices, indices);
                processFaceYNegative(x, y, z, mask, neighbors, vertices, indices);
                
                processFaceZPositive(x, y, z, mask, neighbors, vertices, indices);
                processFaceZNegative(x, y, z, mask, neighbors, vertices, indices);
            }
        }
    }
    _indicesCount = indices.size();
    uploadMeshData(vertices, indices);
}


void Chunk::uploadMeshData(std::vector<PackedVertex> &vertices, std::vector<GLuint> &indices) {
    glBindVertexArray(_VAO);

    // Envoi des vertex data (emballés dans PackedVertex)
    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(PackedVertex), vertices.data(), GL_STATIC_DRAW);

    // Envoi des index data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    glVertexAttribIPointer(0, 1, GL_UNSIGNED_INT, sizeof(PackedVertex), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    // glBindVertexArray(0);
}


// Rendering Functions
void Chunk::render(const Shader &shaderProgram) {
    if (_indicesCount == 0)
        return;
    glm::mat4 model = glm::translate(glm::mat4(1.0f), _position * (float)CHUNK_DIM);
    shaderProgram.setMat4("model", model);
    glBindVertexArray(_VAO);
    glDrawElements(GL_TRIANGLES, _indicesCount, GL_UNSIGNED_INT, (void*)0);
}


// Getters and Setters
glm::vec3 Chunk::getCenterPosition() const {
    float halfChunkDim = CHUNK_DIM / 2.0;
    return glm::vec3(_position.x * CHUNK_DIM + halfChunkDim, _position.y * CHUNK_DIM + halfChunkDim, _position.z * CHUNK_DIM + halfChunkDim); 
}

GLuint Chunk::getVAO() const {
    return _VAO;
}

void Chunk::setPosition(int x, int y, int z) {
    _position = glm::vec3(x, y, z);
}

void Chunk::setPosition(const glm::vec3 &position) {
    _position = position;
}


// Utility Functions
bool Chunk::isVoxelTransparent(int x, int y, int z) {
    if (x < 0 || x >= CHUNK_DIM || y < 0 || y >= CHUNK_DIM || z < 0 || z >= CHUNK_DIM)
        return true;
    return Voxel::isTransparent(_voxels[x * CHUNK_DIM * CHUNK_DIM + y * CHUNK_DIM + z]);
}

bool Chunk::isVoxelTransparent(int x, int y, int z, const std::vector<Chunk*> &neighbors) {
    if (!(x < 0 || x >= CHUNK_DIM || y < 0 || y >= CHUNK_DIM || z < 0 || z >= CHUNK_DIM))
        return Voxel::isTransparent(_voxels[x * CHUNK_DIM * CHUNK_DIM + y * CHUNK_DIM + z]);

    // Back, Front, Left, Right, Bottom, Top
    char chunkID = 0;
    if (z < 0)
        chunkID = 1;
    else if (x < 0)
        chunkID = 2;
    else if (x >= CHUNK_DIM)
        chunkID = 3;
    else if (y < 0)
        chunkID = 4;
    else if (y >= CHUNK_DIM)
        chunkID = 5;
    
    Chunk* chunk = neighbors.at(chunkID);
    if (chunk == nullptr)
        return true;
    
    x = mod(x, CHUNK_DIM);
    y = mod(y, CHUNK_DIM);
    z = mod(z, CHUNK_DIM);

    return chunk->isVoxelTransparent(x, y, z);
}

long long mod(long long k, long long n) {
    return ((k %= n) < 0) ? k+n : k;
}