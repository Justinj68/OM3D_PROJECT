#pragma once

#include <vector>

#include <glad/glad.h>

enum NormalIndex {
    NORMAL_FRONT = 0,
    NORMAL_TOP = 1,
    NORMAL_RIGHT = 2,
    NORMAL_BACK = 3,
    NORMAL_BOTTOM = 4,
    NORMAL_LEFT = 5
};


struct PackedVertex {
    uint32_t data;

    PackedVertex(int x, int y, int z, int normalIndex, int uvIndex) {
        data = (x & 0x3F) | ((y & 0x3F) << 6) | ((z & 0x3F) << 12) | ((normalIndex & 0x07) << 18) | ((uvIndex & 0x03) << 21);
    }

    void decode(int& x, int& y, int& z, int& normalIndex, int& uvIndex) {
        x = data & 0x3F;
        y = (data >> 6) & 0x3F;
        z = (data >> 12) & 0x3F;
        normalIndex = (data >> 18) & 0x07;
        uvIndex = (data >> 21) & 0x03;
    }
};


// Packed data
void addBackFace(GLfloat x, GLfloat y, GLfloat z, std::vector<PackedVertex>& vertices, std::vector<GLuint>& indices);
void addFrontFace(GLfloat x, GLfloat y, GLfloat z, std::vector<PackedVertex>& vertices, std::vector<GLuint>& indices);

void addLeftFace(GLfloat x, GLfloat y, GLfloat z, std::vector<PackedVertex>& vertices, std::vector<GLuint>& indices);
void addRightFace(GLfloat x, GLfloat y, GLfloat z, std::vector<PackedVertex>& vertices, std::vector<GLuint>& indices);

void addTopFace(GLfloat x, GLfloat y, GLfloat z, std::vector<PackedVertex>& vertices, std::vector<GLuint>& indices);
void addBottomFace(GLfloat x, GLfloat y, GLfloat z, std::vector<PackedVertex>& vertices, std::vector<GLuint>& indices);

void buildCube(float x, float y, float z, std::vector<PackedVertex> &vertices, std::vector<GLuint> &indices);


// Non-packed data
void addBackFace(GLfloat x, GLfloat y, GLfloat z, std::vector<GLfloat> &vertices, std::vector<GLuint> &indices);
void addFrontFace(GLfloat x, GLfloat y, GLfloat z, std::vector<GLfloat> &vertices, std::vector<GLuint> &indices);

void addLeftFace(GLfloat x, GLfloat y, GLfloat z, std::vector<GLfloat> &vertices, std::vector<GLuint> &indices);
void addRightFace(GLfloat x, GLfloat y, GLfloat z, std::vector<GLfloat> &vertices, std::vector<GLuint> &indices);

void addTopFace(GLfloat x, GLfloat y, GLfloat z, std::vector<GLfloat> &vertices, std::vector<GLuint> &indices);
void addBottomFace(GLfloat x, GLfloat y, GLfloat z, std::vector<GLfloat> &vertices, std::vector<GLuint> &indices);

void buildCube(float x, float y, float z, std::vector<GLfloat> &vertices, std::vector<GLuint> &indices);