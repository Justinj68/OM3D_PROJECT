#include <chunk.h>

#include <cube_mesh.h>

Chunk::Chunk() {
    _position = glm::vec3(0.0);
    _voxels = new Voxel::Type[CHUNK_DIM * CHUNK_DIM * CHUNK_DIM];
}


Chunk::Chunk(const glm::vec3 &position) {
    _position = position;
    _voxels = new Voxel::Type[CHUNK_DIM * CHUNK_DIM * CHUNK_DIM];
}


void Chunk::defineVoxelData() {
    for (int x = 0; x < CHUNK_DIM; ++x) {
        for (int y = 0; y < CHUNK_DIM; ++y) {
            for (int z = 0; z < CHUNK_DIM; z++) {
                _voxels[x * CHUNK_DIM * CHUNK_DIM + y * CHUNK_DIM + z] = Voxel::STONE;
            }
        }
    }
}


GLuint Chunk::getVAO() const {
    return _VAO;
}


bool Chunk::isVoxelTransparent(int x, int y, int z) {
    if (x < 0 || x >= CHUNK_DIM || y < 0 || y >= CHUNK_DIM || z < 0 || z >= CHUNK_DIM)
        return true;
    return Voxel::isTransparent(_voxels[x * CHUNK_DIM * CHUNK_DIM + y * CHUNK_DIM + z]);
}


void Chunk::build() {
    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;

    for (int x = 0; x < CHUNK_DIM; ++x) {
        for (int y = 0; y < CHUNK_DIM; ++y) {
            for (int z = 0; z < CHUNK_DIM; z++) {
                if (isVoxelTransparent(x, y - 1, z))
                    addBottomFace(x, y, z, vertices, indices);
                if (isVoxelTransparent(x, y + 1, z))
                    addTopFace(x, y, z, vertices, indices);
                if (isVoxelTransparent(x - 1, y, z))
                    addLeftFace(x, y, z, vertices, indices);
                if (isVoxelTransparent(x + 1, y, z))
                    addRightFace(x, y, z, vertices, indices);
                if (isVoxelTransparent(x, y, z + 1))
                    addFrontFace(x, y, z, vertices, indices);
                if (isVoxelTransparent(x, y, z - 1))
                    addBackFace(x, y, z, vertices, indices);
            }
        }
    }
    _indicesCount = indices.size();

    GLuint VBO, EBO;
    glGenVertexArrays(1, &_VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0);
}


void Chunk::render(const Shader &shaderProgram) {
    if (_indicesCount == 0)
        return;
    glm::mat4 model = glm::translate(glm::mat4(1.0f), _position * (float)CHUNK_DIM);
    shaderProgram.setMat4("model", model);
    glBindVertexArray(_VAO);
    glDrawElements(GL_TRIANGLES, _indicesCount, GL_UNSIGNED_INT, (void*)0);
}


void Chunk::setPosition(int x, int y, int z) {
    _position = glm::vec3(x, y, z);
}


void Chunk::setPosition(const glm::vec3 &position) {
    _position = position;
}


Chunk::~Chunk() {
    delete[] _voxels;
}