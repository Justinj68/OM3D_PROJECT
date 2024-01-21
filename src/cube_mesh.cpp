#include <cube_mesh.h>

// Indices management
void addIndices(std::vector<PackedVertex>& vertices, std::vector<GLuint>& indices) {
    GLuint baseIndex = vertices.size() - 4;

    indices.push_back(baseIndex);
    indices.push_back(baseIndex + 1);
    indices.push_back(baseIndex + 2);

    indices.push_back(baseIndex);
    indices.push_back(baseIndex + 2);
    indices.push_back(baseIndex + 3);
}

void addIndices(std::vector<GLfloat> &vertices, std::vector<GLuint> &indices) {
    GLuint baseIndex = vertices.size() / 8 - 4;

    indices.push_back(baseIndex);
    indices.push_back(baseIndex + 1);
    indices.push_back(baseIndex + 2);

    indices.push_back(baseIndex);
    indices.push_back(baseIndex + 2);
    indices.push_back(baseIndex + 3);
}


// Greedy meshing
void addFrontFace(GLfloat z, GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, std::vector<PackedVertex>& vertices, std::vector<GLuint> &indices) {
    PackedVertex v1(x1, y1, z, NormalIndex::NORMAL_FRONT, 0);
    PackedVertex v2(x1, y2, z, NormalIndex::NORMAL_FRONT, 1);
    PackedVertex v3(x2, y2, z, NormalIndex::NORMAL_FRONT, 2);
    PackedVertex v4(x2, y1, z, NormalIndex::NORMAL_FRONT, 3);

    vertices.push_back(v1);
    vertices.push_back(v4);
    vertices.push_back(v3);
    vertices.push_back(v2);

    addIndices(vertices, indices);
}

void addBackFace(GLfloat z, GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, std::vector<PackedVertex>& vertices, std::vector<GLuint> &indices) {
    PackedVertex v1(x1, y1, z + 1, NormalIndex::NORMAL_BACK, 3);
    PackedVertex v2(x1, y2, z + 1, NormalIndex::NORMAL_BACK, 2);
    PackedVertex v3(x2, y2, z + 1, NormalIndex::NORMAL_BACK, 1);
    PackedVertex v4(x2, y1, z + 1, NormalIndex::NORMAL_BACK, 0);

    vertices.push_back(v1);
    vertices.push_back(v2);
    vertices.push_back(v3);
    vertices.push_back(v4);

    addIndices(vertices, indices);
}

void addRightFace(GLfloat x, GLfloat y1, GLfloat z1, GLfloat y2, GLfloat z2, std::vector<PackedVertex>& vertices, std::vector<GLuint> &indices) {
    PackedVertex v1(x + 1, y1, z1, NormalIndex::NORMAL_RIGHT, 3);
    PackedVertex v2(x + 1, y2, z1, NormalIndex::NORMAL_RIGHT, 2);
    PackedVertex v3(x + 1, y2, z2, NormalIndex::NORMAL_RIGHT, 1);
    PackedVertex v4(x + 1, y1, z2, NormalIndex::NORMAL_RIGHT, 0);

    vertices.push_back(v1);
    vertices.push_back(v4);
    vertices.push_back(v3);
    vertices.push_back(v2);

    addIndices(vertices, indices);
}

void addLeftFace(GLfloat x, GLfloat y1, GLfloat z1, GLfloat y2, GLfloat z2, std::vector<PackedVertex>& vertices, std::vector<GLuint> &indices) {
    PackedVertex v1(x, y1, z1, NormalIndex::NORMAL_LEFT, 3);
    PackedVertex v2(x, y2, z1, NormalIndex::NORMAL_LEFT, 2);
    PackedVertex v3(x, y2, z2, NormalIndex::NORMAL_LEFT, 1);
    PackedVertex v4(x, y1, z2, NormalIndex::NORMAL_LEFT, 0);

    vertices.push_back(v1);
    vertices.push_back(v2);
    vertices.push_back(v3);
    vertices.push_back(v4);

    addIndices(vertices, indices);
}

void addTopFace(GLfloat y, GLfloat x1, GLfloat z1, GLfloat x2, GLfloat z2, std::vector<PackedVertex>& vertices, std::vector<GLuint> &indices) {
    PackedVertex v1(x1, y + 1.0, z2, NormalIndex::NORMAL_TOP, 0);
    PackedVertex v2(x1, y + 1.0, z1, NormalIndex::NORMAL_TOP, 1);
    PackedVertex v3(x2, y + 1.0, z1, NormalIndex::NORMAL_TOP, 2);
    PackedVertex v4(x2, y + 1.0, z2, NormalIndex::NORMAL_TOP, 3);

    vertices.push_back(v1);
    vertices.push_back(v2);
    vertices.push_back(v3);
    vertices.push_back(v4);

    addIndices(vertices, indices);
}

void addBottomFace(GLfloat y, GLfloat x1, GLfloat z1, GLfloat x2, GLfloat z2, std::vector<PackedVertex>& vertices, std::vector<GLuint> &indices) {
    PackedVertex v1(x1, y, z2, NormalIndex::NORMAL_BOTTOM, 0);
    PackedVertex v2(x1, y, z1, NormalIndex::NORMAL_BOTTOM, 1);
    PackedVertex v3(x2, y, z1, NormalIndex::NORMAL_BOTTOM, 2);
    PackedVertex v4(x2, y, z2, NormalIndex::NORMAL_BOTTOM, 3);

    vertices.push_back(v1);
    vertices.push_back(v4);
    vertices.push_back(v3);
    vertices.push_back(v2);

    addIndices(vertices, indices);
}


// Classic meshing
void addFrontFace(GLfloat x, GLfloat y, GLfloat z, std::vector<PackedVertex>& vertices, std::vector<GLuint>& indices) {
    PackedVertex v1(x, y, z + 1, NormalIndex::NORMAL_FRONT, 0);
    PackedVertex v2(x, y + 1, z + 1, NormalIndex::NORMAL_FRONT, 1);
    PackedVertex v3(x + 1, y + 1, z + 1, NormalIndex::NORMAL_FRONT, 2);
    PackedVertex v4(x + 1, y, z + 1, NormalIndex::NORMAL_FRONT, 3);

    vertices.push_back(v1);
    vertices.push_back(v2);
    vertices.push_back(v3);
    vertices.push_back(v4);

    addIndices(vertices, indices);
}

void addBackFace(GLfloat x, GLfloat y, GLfloat z, std::vector<PackedVertex> &vertices, std::vector<GLuint> &indices) {
    PackedVertex v1(x, y, z, NormalIndex::NORMAL_BACK, 3);
    PackedVertex v2(x, y + 1.0, z, NormalIndex::NORMAL_BACK, 2);
    PackedVertex v3(x + 1.0, y + 1.0, z, NormalIndex::NORMAL_BACK, 1);
    PackedVertex v4(x + 1.0, y, z, NormalIndex::NORMAL_BACK, 0);

    vertices.push_back(v1);
    vertices.push_back(v4);
    vertices.push_back(v3);
    vertices.push_back(v2);

    addIndices(vertices, indices);
}

void addLeftFace(GLfloat x, GLfloat y, GLfloat z, std::vector<PackedVertex> &vertices, std::vector<GLuint> &indices) {
    PackedVertex v1(x, y, z + 1.0, NormalIndex::NORMAL_LEFT, 0);
    PackedVertex v2(x, y + 1.0, z + 1.0, NormalIndex::NORMAL_LEFT, 1);
    PackedVertex v3(x, y + 1.0, z, NormalIndex::NORMAL_LEFT, 2);
    PackedVertex v4(x, y, z, NormalIndex::NORMAL_LEFT, 3);

    vertices.push_back(v1);
    vertices.push_back(v4);
    vertices.push_back(v3);
    vertices.push_back(v2);

    addIndices(vertices, indices);
}

void addRightFace(GLfloat x, GLfloat y, GLfloat z, std::vector<PackedVertex> &vertices, std::vector<GLuint> &indices) {
    PackedVertex v1(x + 1.0, y, z + 1.0, NormalIndex::NORMAL_RIGHT, 0);
    PackedVertex v2(x + 1.0, y + 1.0, z + 1.0, NormalIndex::NORMAL_RIGHT, 1);
    PackedVertex v3(x + 1.0, y + 1.0, z, NormalIndex::NORMAL_RIGHT, 2);
    PackedVertex v4(x + 1.0, y, z, NormalIndex::NORMAL_RIGHT, 3);

    vertices.push_back(v1);
    vertices.push_back(v2);
    vertices.push_back(v3);
    vertices.push_back(v4);

    addIndices(vertices, indices);
}

void addTopFace(GLfloat x, GLfloat y, GLfloat z, std::vector<PackedVertex> &vertices, std::vector<GLuint> &indices) {
    PackedVertex v1(x, y + 1.0, z + 1.0, NormalIndex::NORMAL_TOP, 0);
    PackedVertex v2(x, y + 1.0, z, NormalIndex::NORMAL_TOP, 1);
    PackedVertex v3(x + 1.0, y + 1.0, z, NormalIndex::NORMAL_TOP, 2);
    PackedVertex v4(x + 1.0, y + 1.0, z + 1.0, NormalIndex::NORMAL_TOP, 3);

    vertices.push_back(v1);
    vertices.push_back(v2);
    vertices.push_back(v3);
    vertices.push_back(v4);

    addIndices(vertices, indices);
}

void addBottomFace(GLfloat x, GLfloat y, GLfloat z, std::vector<PackedVertex> &vertices, std::vector<GLuint> &indices) {
    PackedVertex v1(x, y, z + 1.0, NormalIndex::NORMAL_BOTTOM, 3);
    PackedVertex v2(x, y, z, NormalIndex::NORMAL_BOTTOM, 2);
    PackedVertex v3(x + 1.0, y, z, NormalIndex::NORMAL_BOTTOM, 1);
    PackedVertex v4(x + 1.0, y, z + 1.0, NormalIndex::NORMAL_BOTTOM, 0);

    vertices.push_back(v1);
    vertices.push_back(v4);
    vertices.push_back(v3);
    vertices.push_back(v2);

    addIndices(vertices, indices);
}


// Cube build
void buildCube(float x, float y, float z, std::vector<PackedVertex> &vertices, std::vector<GLuint> &indices) {
    addTopFace   (x, y, z, vertices, indices);
    addBackFace  (x, y, z, vertices, indices);
    addLeftFace  (x, y, z, vertices, indices);
    addRightFace (x, y, z, vertices, indices);
    addFrontFace (x, y, z, vertices, indices);
    addBottomFace(x, y, z, vertices, indices);
}
