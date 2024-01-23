#include <cube_mesh.h>

// Indices management
void addIndices(std::vector<PackedVertex>& vertices, std::vector<GLuint>& indices) {
    GLuint baseIndex = (GLuint)(vertices.size() - 4);

    indices.push_back(baseIndex);
    indices.push_back(baseIndex + 1);
    indices.push_back(baseIndex + 2);

    indices.push_back(baseIndex);
    indices.push_back(baseIndex + 2);
    indices.push_back(baseIndex + 3);
}

void addIndices(std::vector<GLfloat> &vertices, std::vector<GLuint> &indices) {
    GLuint baseIndex = (GLuint)(vertices.size() / 8 - 4);

    indices.push_back(baseIndex);
    indices.push_back(baseIndex + 1);
    indices.push_back(baseIndex + 2);

    indices.push_back(baseIndex);
    indices.push_back(baseIndex + 2);
    indices.push_back(baseIndex + 3);
}


// Greedy meshing
void addRightFace(char x, char y1, char z1, char y2, char z2, std::vector<PackedVertex>& vertices, std::vector<GLuint> &indices) {
    PackedVertex v1(x + 1, y1, z1, Direction::XPOS, UVIndex::BRC);
    PackedVertex v2(x + 1, y2, z1, Direction::XPOS, UVIndex::TRC);
    PackedVertex v3(x + 1, y2, z2, Direction::XPOS, UVIndex::TLC);
    PackedVertex v4(x + 1, y1, z2, Direction::XPOS, UVIndex::BLC);

    vertices.push_back(v1);
    vertices.push_back(v4);
    vertices.push_back(v3);
    vertices.push_back(v2);

    addIndices(vertices, indices);
}

void addLeftFace(char x, char y1, char z1, char y2, char z2, std::vector<PackedVertex>& vertices, std::vector<GLuint> &indices) {
    PackedVertex v1(x, y1, z1, Direction::XNEG, UVIndex::BRC);
    PackedVertex v2(x, y2, z1, Direction::XNEG, UVIndex::TRC);
    PackedVertex v3(x, y2, z2, Direction::XNEG, UVIndex::TLC);
    PackedVertex v4(x, y1, z2, Direction::XNEG, UVIndex::BLC);

    vertices.push_back(v1);
    vertices.push_back(v2);
    vertices.push_back(v3);
    vertices.push_back(v4);

    addIndices(vertices, indices);
}

void addTopFace(char y, char x1, char z1, char x2, char z2, std::vector<PackedVertex>& vertices, std::vector<GLuint> &indices) {
    PackedVertex v1(x1, y + 1, z2, Direction::YPOS, UVIndex::BLC);
    PackedVertex v2(x1, y + 1, z1, Direction::YPOS, UVIndex::TLC);
    PackedVertex v3(x2, y + 1, z1, Direction::YPOS, UVIndex::TRC);
    PackedVertex v4(x2, y + 1, z2, Direction::YPOS, UVIndex::BRC);

    vertices.push_back(v1);
    vertices.push_back(v2);
    vertices.push_back(v3);
    vertices.push_back(v4);

    addIndices(vertices, indices);
}

void addBottomFace(char y, char x1, char z1, char x2, char z2, std::vector<PackedVertex>& vertices, std::vector<GLuint> &indices) {
    PackedVertex v1(x1, y, z2, Direction::YNEG, UVIndex::BLC);
    PackedVertex v2(x1, y, z1, Direction::YNEG, UVIndex::TLC);
    PackedVertex v3(x2, y, z1, Direction::YNEG, UVIndex::TRC);
    PackedVertex v4(x2, y, z2, Direction::YNEG, UVIndex::BRC);

    vertices.push_back(v1);
    vertices.push_back(v4);
    vertices.push_back(v3);
    vertices.push_back(v2);

    addIndices(vertices, indices);
}

void addFrontFace(char z, char x1, char y1, char x2, char y2, std::vector<PackedVertex>& vertices, std::vector<GLuint> &indices) {
    PackedVertex v1(x1, y1, z, Direction::ZPOS, UVIndex::BLC);
    PackedVertex v2(x1, y2, z, Direction::ZPOS, UVIndex::TLC);
    PackedVertex v3(x2, y2, z, Direction::ZPOS, UVIndex::TRC);
    PackedVertex v4(x2, y1, z, Direction::ZPOS, UVIndex::BRC);

    vertices.push_back(v1);
    vertices.push_back(v4);
    vertices.push_back(v3);
    vertices.push_back(v2);

    addIndices(vertices, indices);
}

void addBackFace(char z, char x1, char y1, char x2, char y2, std::vector<PackedVertex>& vertices, std::vector<GLuint> &indices) {
    PackedVertex v1(x1, y1, z + 1, Direction::ZNEG, UVIndex::BRC);
    PackedVertex v2(x1, y2, z + 1, Direction::ZNEG, UVIndex::TRC);
    PackedVertex v3(x2, y2, z + 1, Direction::ZNEG, UVIndex::TLC);
    PackedVertex v4(x2, y1, z + 1, Direction::ZNEG, UVIndex::BLC);

    vertices.push_back(v1);
    vertices.push_back(v2);
    vertices.push_back(v3);
    vertices.push_back(v4);

    addIndices(vertices, indices);
}


// Classic meshing
void addFrontFace(char x, char y, char z, std::vector<PackedVertex>& vertices, std::vector<GLuint>& indices) {
    PackedVertex v1(    x,     y, z + 1, Direction::ZPOS, UVIndex::BLC);
    PackedVertex v2(    x, y + 1, z + 1, Direction::ZPOS, UVIndex::TLC);
    PackedVertex v3(x + 1, y + 1, z + 1, Direction::ZPOS, UVIndex::TRC);
    PackedVertex v4(x + 1,     y, z + 1, Direction::ZPOS, UVIndex::BRC);

    vertices.push_back(v1);
    vertices.push_back(v2);
    vertices.push_back(v3);
    vertices.push_back(v4);

    addIndices(vertices, indices);
}

void addBackFace(char x, char y, char z, std::vector<PackedVertex> &vertices, std::vector<GLuint> &indices) {
    PackedVertex v1(    x,     y, z, Direction::ZNEG, UVIndex::BRC);
    PackedVertex v2(    x, y + 1, z, Direction::ZNEG, UVIndex::TRC);
    PackedVertex v3(x + 1, y + 1, z, Direction::ZNEG, UVIndex::TLC);
    PackedVertex v4(x + 1,     y, z, Direction::ZNEG, UVIndex::BLC);

    vertices.push_back(v1);
    vertices.push_back(v4);
    vertices.push_back(v3);
    vertices.push_back(v2);

    addIndices(vertices, indices);
}

void addLeftFace(char x, char y, char z, std::vector<PackedVertex> &vertices, std::vector<GLuint> &indices) {
    PackedVertex v1(x,     y, z + 1, Direction::XNEG, UVIndex::BLC);
    PackedVertex v2(x, y + 1, z + 1, Direction::XNEG, UVIndex::TLC);
    PackedVertex v3(x, y + 1,     z, Direction::XNEG, UVIndex::TRC);
    PackedVertex v4(x,     y,     z, Direction::XNEG, UVIndex::BRC);

    vertices.push_back(v1);
    vertices.push_back(v4);
    vertices.push_back(v3);
    vertices.push_back(v2);

    addIndices(vertices, indices);
}

void addRightFace(char x, char y, char z, std::vector<PackedVertex> &vertices, std::vector<GLuint> &indices) {
    PackedVertex v1(x + 1,     y, z + 1, Direction::XPOS, UVIndex::BLC);
    PackedVertex v2(x + 1, y + 1, z + 1, Direction::XPOS, UVIndex::TLC);
    PackedVertex v3(x + 1, y + 1,     z, Direction::XPOS, UVIndex::TRC);
    PackedVertex v4(x + 1,     y,     z, Direction::XPOS, UVIndex::BRC);

    vertices.push_back(v1);
    vertices.push_back(v2);
    vertices.push_back(v3);
    vertices.push_back(v4);

    addIndices(vertices, indices);
}

void addTopFace(char x, char y, char z, std::vector<PackedVertex> &vertices, std::vector<GLuint> &indices) {
    PackedVertex v1(    x, y + 1, z + 1, Direction::YPOS, UVIndex::BLC);
    PackedVertex v2(    x, y + 1,     z, Direction::YPOS, UVIndex::TLC);
    PackedVertex v3(x + 1, y + 1,     z, Direction::YPOS, UVIndex::TRC);
    PackedVertex v4(x + 1, y + 1, z + 1, Direction::YPOS, UVIndex::BRC);

    vertices.push_back(v1);
    vertices.push_back(v2);
    vertices.push_back(v3);
    vertices.push_back(v4);

    addIndices(vertices, indices);
}

void addBottomFace(char x, char y, char z, std::vector<PackedVertex> &vertices, std::vector<GLuint> &indices) {
    PackedVertex v1(    x, y, z + 1, Direction::YNEG, UVIndex::BRC);
    PackedVertex v2(    x, y,     z, Direction::YNEG, UVIndex::TRC);
    PackedVertex v3(x + 1, y,     z, Direction::YNEG, UVIndex::TLC);
    PackedVertex v4(x + 1, y, z + 1, Direction::YNEG, UVIndex::BLC);

    vertices.push_back(v1);
    vertices.push_back(v4);
    vertices.push_back(v3);
    vertices.push_back(v2);

    addIndices(vertices, indices);
}
