#include <cube_mesh.h>

void addIndices(std::vector<GLfloat> &vertices, std::vector<GLuint> &indices) {
    GLuint baseIndex = vertices.size() / 3 - 4;
    // Triangle 1
    indices.push_back(baseIndex);
    indices.push_back(baseIndex + 1);
    indices.push_back(baseIndex + 2);

    // Triangle 2
    indices.push_back(baseIndex);
    indices.push_back(baseIndex + 2);
    indices.push_back(baseIndex + 3);
}

void addFrontFace(GLfloat x, GLfloat y, GLfloat z, std::vector<GLfloat> &vertices, std::vector<GLuint> &indices) {
    GLfloat v1[] = {x, y, z + 1.0};
    GLfloat v2[] = {x, y + 1.0, z + 1.0};
    GLfloat v3[] = {x + 1.0, y + 1.0, z + 1.0};
    GLfloat v4[] = {x + 1.0, y, z + 1.0};

    vertices.insert(vertices.end(), std::begin(v1), std::end(v1));
    vertices.insert(vertices.end(), std::begin(v2), std::end(v2));
    vertices.insert(vertices.end(), std::begin(v3), std::end(v3));
    vertices.insert(vertices.end(), std::begin(v4), std::end(v4));

    addIndices(vertices, indices);
}


void addBackFace(GLfloat x, GLfloat y, GLfloat z, std::vector<GLfloat> &vertices, std::vector<GLuint> &indices) {
    GLfloat v1[] = {x, y, z};
    GLfloat v2[] = {x, y + 1.0, z};
    GLfloat v3[] = {x + 1.0, y + 1.0, z};
    GLfloat v4[] = {x + 1.0, y, z};

    vertices.insert(vertices.end(), std::begin(v1), std::end(v1));
    vertices.insert(vertices.end(), std::begin(v4), std::end(v4));
    vertices.insert(vertices.end(), std::begin(v3), std::end(v3));
    vertices.insert(vertices.end(), std::begin(v2), std::end(v2));

    addIndices(vertices, indices);
}


void addLeftFace(GLfloat x, GLfloat y, GLfloat z, std::vector<GLfloat> &vertices, std::vector<GLuint> &indices) {
    GLfloat v1[] = {x, y, z + 1.0};
    GLfloat v2[] = {x, y + 1.0, z + 1.0};
    GLfloat v3[] = {x, y + 1.0, z};
    GLfloat v4[] = {x, y, z};

    vertices.insert(vertices.end(), std::begin(v1), std::end(v1));
    vertices.insert(vertices.end(), std::begin(v4), std::end(v4));
    vertices.insert(vertices.end(), std::begin(v3), std::end(v3));
    vertices.insert(vertices.end(), std::begin(v2), std::end(v2));

    addIndices(vertices, indices);
}


void addRightFace(GLfloat x, GLfloat y, GLfloat z, std::vector<GLfloat> &vertices, std::vector<GLuint> &indices) {
    GLfloat v1[] = {x + 1.0, y, z + 1.0};
    GLfloat v2[] = {x + 1.0, y + 1.0, z + 1.0};
    GLfloat v3[] = {x + 1.0, y + 1.0, z};
    GLfloat v4[] = {x + 1.0, y, z};

    vertices.insert(vertices.end(), std::begin(v1), std::end(v1));
    vertices.insert(vertices.end(), std::begin(v2), std::end(v2));
    vertices.insert(vertices.end(), std::begin(v3), std::end(v3));
    vertices.insert(vertices.end(), std::begin(v4), std::end(v4));

    addIndices(vertices, indices);
}


void addTopFace(GLfloat x, GLfloat y, GLfloat z, std::vector<GLfloat> &vertices, std::vector<GLuint> &indices) {
    GLfloat v1[] = {x, y + 1.0, z + 1.0};
    GLfloat v2[] = {x, y + 1.0, z};
    GLfloat v3[] = {x + 1.0, y + 1.0, z};
    GLfloat v4[] = {x + 1.0, y + 1.0, z + 1.0};

    vertices.insert(vertices.end(), std::begin(v1), std::end(v1));
    vertices.insert(vertices.end(), std::begin(v2), std::end(v2));
    vertices.insert(vertices.end(), std::begin(v3), std::end(v3));
    vertices.insert(vertices.end(), std::begin(v4), std::end(v4));

    addIndices(vertices, indices);
}


void addBottomFace(GLfloat x, GLfloat y, GLfloat z, std::vector<GLfloat> &vertices, std::vector<GLuint> &indices) {
    GLfloat v1[] = {x, y, z + 1.0};
    GLfloat v2[] = {x, y, z};
    GLfloat v3[] = {x + 1.0, y, z};
    GLfloat v4[] = {x + 1.0, y, z + 1.0};

    vertices.insert(vertices.end(), std::begin(v1), std::end(v1));
    vertices.insert(vertices.end(), std::begin(v4), std::end(v4));
    vertices.insert(vertices.end(), std::begin(v3), std::end(v3));
    vertices.insert(vertices.end(), std::begin(v2), std::end(v2));

    addIndices(vertices, indices);
}


void buildCube(float x, float y, float z, std::vector<GLfloat> &vertices, std::vector<GLuint> &indices) {
    addTopFace   (x, y, z, vertices, indices);
    addBackFace  (x, y, z, vertices, indices);
    addLeftFace  (x, y, z, vertices, indices);
    addRightFace (x, y, z, vertices, indices);
    addFrontFace (x, y, z, vertices, indices);
    addBottomFace(x, y, z, vertices, indices);
}