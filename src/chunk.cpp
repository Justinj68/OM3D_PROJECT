#include <chunk.h>

#include <math.h>

char mod(char k, char n);

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


bool Chunk::isFaceVisible(char x, char y, char z, Direction direction) {
    if (isVoxelTransparent(x, y, z))
        return false;
    switch (direction) {
        case Direction::XNEG:
            return isVoxelTransparent(x - 1, y, z);
        case Direction::XPOS:
            return isVoxelTransparent(x + 1, y, z);
        case Direction::YNEG:
            return isVoxelTransparent(x, y - 1, z);
        case Direction::YPOS:
            return isVoxelTransparent(x, y + 1, z);
        case Direction::ZNEG:
            return isVoxelTransparent(x, y, z + 1);
        case Direction::ZPOS:
            return isVoxelTransparent(x, y, z - 1);
    }
    return true;
}


bool Chunk::isFaceVisible(char x, char y, char z, Direction direction, const std::vector<Chunk*> &neighbors) {
    if (isVoxelTransparent(x, y, z))
        return false;
    switch (direction) {
        case Direction::XNEG:
            return isVoxelTransparent(x - 1, y, z, neighbors);
        case Direction::XPOS:
            return isVoxelTransparent(x + 1, y, z, neighbors);
        case Direction::YNEG:
            return isVoxelTransparent(x, y - 1, z, neighbors);
        case Direction::YPOS:
            return isVoxelTransparent(x, y + 1, z, neighbors);
        case Direction::ZNEG:
            return isVoxelTransparent(x, y, z + 1, neighbors);
        case Direction::ZPOS:
            return isVoxelTransparent(x, y, z - 1, neighbors);
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
    for (char x = 0; x < CHUNK_DIM; ++x) {
        for (char y = 0; y < CHUNK_DIM; ++y) {
            for (char z = 0; z < CHUNK_DIM; z++) {
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
    for (char x = 0; x < CHUNK_DIM; ++x) {
        for (char y = 0; y < CHUNK_DIM; ++y) {
            for (char z = 0; z < CHUNK_DIM; z++) {
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
    for (char x = 0; x < CHUNK_DIM; ++x) {
        for (char y = 0; y < CHUNK_DIM; ++y) {
            for (char z = 0; z < CHUNK_DIM; z++) {
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
void Chunk::processFaceXPositive(char x, char y, char z, char processed[], const std::vector<Chunk*> &neighbors, std::vector<PackedVertex> &vertices, std::vector<GLuint> &indices) {
    char filter = DirectionMask::MASK_XPOS;
    if ((processed[x * CHUNK_DIM * CHUNK_DIM + y * CHUNK_DIM + z] & filter) || !isFaceVisible(x, y, z, Direction::XPOS, neighbors))
        return;
    
    char depth = 1;
    char height = 1;

    while (z + depth < CHUNK_DIM && isFaceVisible(x, y, z + depth, Direction::XPOS, neighbors) && !processed[x * CHUNK_DIM * CHUNK_DIM + y * CHUNK_DIM + z + depth])
        depth++;

    while (y + height < CHUNK_DIM && isFaceVisible(x, y + height, z, Direction::XPOS, neighbors)) {
        bool addRow = true;
        for (char k = 0; k < depth; k++) {
            if (!isFaceVisible(x, y + height, z + k, Direction::XPOS, neighbors)) {
                addRow = false;
                break;
            }
        }
        if (!addRow)
            break;
        height++;
    }

    addRightFace(x, y, z, y + height, z + depth, vertices, indices);

    for (char dz = 0; dz < depth; dz++) {
        for (char dy = 0; dy < height; dy++) {
            processed[x * CHUNK_DIM * CHUNK_DIM + (y + dy) * CHUNK_DIM + z + dz] |= filter;
        }
    }
}

// WORKS
void Chunk::processFaceXNegative(char x, char y, char z, char processed[], const std::vector<Chunk*> &neighbors, std::vector<PackedVertex> &vertices, std::vector<GLuint> &indices) {
    char filter = DirectionMask::MASK_XNEG;
    if ((processed[x * CHUNK_DIM * CHUNK_DIM + y * CHUNK_DIM + z] & filter) || !isFaceVisible(x, y, z, Direction::XNEG, neighbors))
        return;
    
    char depth = 1;
    char height = 1;

    while (z + depth < CHUNK_DIM && isFaceVisible(x, y, z + depth, Direction::XNEG, neighbors) && !processed[x * CHUNK_DIM * CHUNK_DIM + y * CHUNK_DIM + z + depth])
        depth++;

    while (y + height < CHUNK_DIM && isFaceVisible(x, y + height, z, Direction::XNEG, neighbors)) {
        bool addRow = true;
        for (char k = 0; k < depth; k++) {
            if (!isFaceVisible(x, y + height, z + k, Direction::XNEG, neighbors)) {
                addRow = false;
                break;
            }
        }
        if (!addRow)
            break;
        height++;
    }

    addLeftFace(x, y, z, y + height, z + depth, vertices, indices);

    for (char dz = 0; dz < depth; dz++) {
        for (char dy = 0; dy < height; dy++) {
            processed[x * CHUNK_DIM * CHUNK_DIM + (y + dy) * CHUNK_DIM + z + dz] |= filter;
        }
    }
}


void Chunk::processFaceYPositive(char x, char y, char z, char processed[], const std::vector<Chunk*> &neighbors, std::vector<PackedVertex> &vertices, std::vector<GLuint> &indices) {
    DirectionMask mask = DirectionMask::MASK_YPOS;
    Direction direction = Direction::YPOS;
    if ((processed[x * CHUNK_DIM * CHUNK_DIM + y * CHUNK_DIM + z] & mask) || !isFaceVisible(x, y, z, direction, neighbors))
        return;
    
    char width1 = 1, width2 = 1, width = 1;
    char depth1 = 1, depth2 = 1, depth = 1;

    while (x + width1 < CHUNK_DIM && isFaceVisible(x + width1, y, z, direction, neighbors) && !processed[(x + width1) * CHUNK_DIM * CHUNK_DIM + y * CHUNK_DIM + z])
        width1++;

    while (z + depth1 < CHUNK_DIM && isFaceVisible(x, y, z + depth1, direction, neighbors) && !processed[x * CHUNK_DIM * CHUNK_DIM + y * CHUNK_DIM + z + depth1]) {
        bool addRow = true;
        for (int k = 0; k < width1; k++) {
            if (!isFaceVisible(x + k, y, z + depth1, direction, neighbors)) {
                addRow = false;
                break;
            }
        }
        if (!addRow)
            break;
        depth1++;
    }

    // while (z + depth2 < CHUNK_DIM && isFaceVisible(x, y, z + depth2, direction, neighbors) && !processed[x * CHUNK_DIM * CHUNK_DIM + y * CHUNK_DIM + z + depth2])
    //     depth2++;

    // while (x + width2 < CHUNK_DIM && isFaceVisible(x + width2, y, z, direction, neighbors) && !processed[(x + width2) * CHUNK_DIM * CHUNK_DIM + y * CHUNK_DIM + z]) {
    //     bool addRow = true;
    //     for (int k = 0; k < depth2; k++) {
    //         if (!isFaceVisible(x + width2, y, z + k, direction, neighbors)) {
    //             addRow = false;
    //             break;
    //         }
    //     }
    //     if (!addRow)
    //         break;
    //     width2++;
    // }

    // if (width1 * depth1 >= width2 * depth2)
    //     width = width1, depth = depth1;
    // else
    //     width = width2, depth = depth2;

    width = width1;
    depth = depth1;
    addTopFace(y, x, z, x + width, z + depth, vertices, indices);

    for (char dx = 0; dx < width; dx++) {
        for (char dz = 0; dz < depth; dz++) {
            processed[(x + dx) * CHUNK_DIM * CHUNK_DIM + y * CHUNK_DIM + z + dz] |= mask;
        }
    }
}
void Chunk::processFaceYNegative(char x, char y, char z, char processed[], const std::vector<Chunk*> &neighbors, std::vector<PackedVertex> &vertices, std::vector<GLuint> &indices) {
    DirectionMask mask = DirectionMask::MASK_YNEG;
    Direction direction = Direction::YNEG;
    if ((processed[x * CHUNK_DIM * CHUNK_DIM + y * CHUNK_DIM + z] & mask) || !isFaceVisible(x, y, z, direction, neighbors))
        return;
    
    char width1 = 1, width2 = 1, width = 1;
    char depth1 = 1, depth2 = 1, depth = 1;

    while (x + width1 < CHUNK_DIM && isFaceVisible(x + width1, y, z, direction, neighbors) && !processed[(x + width1) * CHUNK_DIM * CHUNK_DIM + y * CHUNK_DIM + z])
        width1++;

    while (z + depth1 < CHUNK_DIM && isFaceVisible(x, y, z + depth1, direction, neighbors) && !processed[x * CHUNK_DIM * CHUNK_DIM + y * CHUNK_DIM + z + depth1]) {
        bool addRow = true;
        for (int k = 0; k < width1; k++) {
            if (!isFaceVisible(x + k, y, z + depth1, direction, neighbors)) {
                addRow = false;
                break;
            }
        }
        if (!addRow)
            break;
        depth1++;
    }

    // while (z + depth2 < CHUNK_DIM && isFaceVisible(x, y, z + depth2, direction, neighbors) && !processed[x * CHUNK_DIM * CHUNK_DIM + y * CHUNK_DIM + z + depth2])
    //     depth2++;

    // while (x + width2 < CHUNK_DIM && isFaceVisible(x + width2, y, z, direction, neighbors) && !processed[(x + width2) * CHUNK_DIM * CHUNK_DIM + y * CHUNK_DIM + z]) {
    //     bool addRow = true;
    //     for (int k = 0; k < depth2; k++) {
    //         if (!isFaceVisible(x + width2, y, z + k, direction, neighbors)) {
    //             addRow = false;
    //             break;
    //         }
    //     }
    //     if (!addRow)
    //         break;
    //     width2++;
    // }

    // if (width1 * depth1 >= width2 * depth2)
    //     width = width1, depth = depth1;
    // else
    //     width = width2, depth = depth2;
    width = width1;
    depth = depth1;

    addBottomFace(y, x, z, x + width, z + depth, vertices, indices);

    for (char dx = 0; dx < width; dx++) {
        for (char dz = 0; dz < depth; dz++) {
            processed[(x + dx) * CHUNK_DIM * CHUNK_DIM + y * CHUNK_DIM + z + dz] |= mask;
        }
    }
}

// WORKS
void Chunk::processFaceZPositive(char x, char y, char z, char processed[], const std::vector<Chunk*> &neighbors, std::vector<PackedVertex> &vertices, std::vector<GLuint> &indices, int* facesCount) {
    DirectionMask mask = DirectionMask::MASK_ZPOS;
    Direction direction = Direction::ZPOS;
    if ((processed[x * CHUNK_DIM * CHUNK_DIM + y * CHUNK_DIM + z] & mask) || !isFaceVisible(x, y, z, direction, neighbors))
        return;
    
    char width1 = 1, width2 = 1, width = 1;
    char height1 = 1, height2 = 1, height = 1;

    while (x + width1 < CHUNK_DIM && isFaceVisible(x + width1, y, z, direction, neighbors))
        width1++;

    while (y + height1 < CHUNK_DIM && isFaceVisible(x, y + height1, z, direction, neighbors) && !processed[x * CHUNK_DIM * CHUNK_DIM + (y + height1) * CHUNK_DIM + z]) {
        bool addRow = true;
        for (char k = 0; k < width1; k++) {
            if (!isFaceVisible(x + k, y + height1, z, direction, neighbors)) {
                addRow = false;
                break;
            }
        }
        if (!addRow)
            break;
        height1++;
    }


    // while (y + height2 < CHUNK_DIM && isFaceVisible(x, y + height2, z, direction, neighbors))
    //     height2++;

    // while (x + width2 < CHUNK_DIM && isFaceVisible(x + width2, y, z, direction, neighbors) && !processed[(x + width2) * CHUNK_DIM * CHUNK_DIM + y * CHUNK_DIM + z]) {
    //     bool addRow = true;
    //     for (char k = 0; k < height2; k++) {
    //         if (!isFaceVisible(x + width2, y + k, z, direction, neighbors)) {
    //             addRow = false;
    //             break;
    //         }
    //     }
    //     if (!addRow)
    //         break;
    //     width2++;
    // }

    // if (width1 * height1 >= width2 * height2)
    //     width = width1, height = height1;
    // else
    //     width = width2, height = height2;
    width = width1;
    height = height1;

    addFrontFace(z, x, y, x + width1, y + height1, vertices, indices);

    for (char dx = 0; dx < width; dx++) {
        for (char dy = 0; dy < height; dy++) {
            processed[(x + dx) * CHUNK_DIM * CHUNK_DIM + (y + dy) * CHUNK_DIM + z] |= mask;
        }
    }
}


// WORKS
void Chunk::processFaceZNegative(char x, char y, char z, char processed[], const std::vector<Chunk*> &neighbors, std::vector<PackedVertex> &vertices, std::vector<GLuint> &indices) {
    DirectionMask mask = DirectionMask::MASK_ZNEG;
    Direction direction = Direction::ZNEG;
    if ((processed[x * CHUNK_DIM * CHUNK_DIM + y * CHUNK_DIM + z] & mask) || !isFaceVisible(x, y, z, direction, neighbors))
        return;
    
    char width1 = 1, width2 = 1, width = 1;
    char height1 = 1, height2 = 1, height = 1;

    while (x + width1 < CHUNK_DIM && isFaceVisible(x + width1, y, z, direction, neighbors))
        width1++;

    while (y + height1 < CHUNK_DIM && isFaceVisible(x, y + height1, z, direction, neighbors) && !processed[x * CHUNK_DIM * CHUNK_DIM + (y + height1) * CHUNK_DIM + z]) {
        bool addRow = true;
        for (char k = 0; k < width1; k++) {
            if (!isFaceVisible(x + k, y + height1, z, direction, neighbors)) {
                addRow = false;
                break;
            }
        }
        if (!addRow)
            break;
        height1++;
    }


    // while (y + height2 < CHUNK_DIM && isFaceVisible(x, y + height2, z, direction, neighbors))
    //     height2++;

    // while (x + width2 < CHUNK_DIM && isFaceVisible(x + width2, y, z, direction, neighbors) && !processed[(x + width2) * CHUNK_DIM * CHUNK_DIM + y * CHUNK_DIM + z]) {
    //     bool addRow = true;
    //     for (char k = 0; k < height2; k++) {
    //         if (!isFaceVisible(x + width2, y + k, z, direction, neighbors)) {
    //             addRow = false;
    //             break;
    //         }
    //     }
    //     if (!addRow)
    //         break;
    //     width2++;
    // }

    // if (width1 * height1 >= width2 * height2)
    //     width = width1, height = height1;
    // else
    //     width = width2, height = height2;
    width = width1;
    height = height1;

    addBackFace(z, x, y, x + width, y + height, vertices, indices);

    for (char dx = 0; dx < width; dx++) {
        for (char dy = 0; dy < height; dy++) {
            processed[(x + dx) * CHUNK_DIM * CHUNK_DIM + (y + dy) * CHUNK_DIM + z] |= mask;
        }
    }
}



void Chunk::greedyBuild(const std::vector<Chunk*> &neighbors, int *facesCount) {
    if (_empty) {
        _indicesCount = 0;
        return;
    }
    char processed[CHUNK_DIM * CHUNK_DIM * CHUNK_DIM] = {0};
    std::vector<PackedVertex> vertices;
    std::vector<GLuint> indices;

    for (char x = 0; x < CHUNK_DIM; ++x) {
        for (char y = 0; y < CHUNK_DIM; ++y) {
            for (char z = 0; z < CHUNK_DIM; z++) {
                if (isVoxelTransparent(x, y, z))
                    continue;

                // Traitement des faces pour chaque direction
                processFaceXPositive(x, y, z, processed, neighbors, vertices, indices);
                processFaceXNegative(x, y, z, processed, neighbors, vertices, indices);

                processFaceYPositive(x, y, z, processed, neighbors, vertices, indices);
                processFaceYNegative(x, y, z, processed, neighbors, vertices, indices);
                
                processFaceZPositive(x, y, z, processed, neighbors, vertices, indices, facesCount);
                processFaceZNegative(x, y, z, processed, neighbors, vertices, indices);
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
    glDrawElements(GL_TRIANGLES, (GLsizei)(_indicesCount), GL_UNSIGNED_INT, (void*)0);
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
bool Chunk::isVoxelTransparent(char x, char y, char z) {
    if (x < 0 || x >= CHUNK_DIM || y < 0 || y >= CHUNK_DIM || z < 0 || z >= CHUNK_DIM)
        return true;
    return Voxel::isTransparent(_voxels[x * CHUNK_DIM * CHUNK_DIM + y * CHUNK_DIM + z]);
}

bool Chunk::isVoxelTransparent(char x, char y, char z, const std::vector<Chunk*> &neighbors) {
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

char mod(char k, char n) {
    return ((k %= n) < 0) ? k+n : k;
}