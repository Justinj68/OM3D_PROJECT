#pragma once

#include <vector>

#include <glad/glad.h>

/**
 *  Representation of the orthonormal coordinate system in 3D
 *
 *             y
 *             |
 *             |
 *             |_____ x
 *              \
 *               \
 *                z
 *  
 *  Axes:
 *  - X-axis (x) : from left to right.
 *  - Y-axis (y) : from bottom to top.
 *  - Z-axis (z) : from back to front.
 */


enum Direction : unsigned char {
    ZPOS = 0, // Front
    YPOS = 1, // Top
    XPOS = 2, // Right
    ZNEG = 3, // Back
    YNEG = 4, // Bottom
    XNEG = 5, // Left
};

enum DirectionMask : unsigned char {
    MASK_ZPOS = 0b00000001, //  1
    MASK_YPOS = 0b00000010, //  2
    MASK_XPOS = 0b00000100, //  4
    MASK_ZNEG = 0b00001000, //  8
    MASK_YNEG = 0b00010000, // 16
    MASK_XNEG = 0b00100000, // 32
};

enum UVIndex : unsigned char {
    BLC = 0, // Bottom   Left    corner
    TLC = 1, // Top      Left    corner
    TRC = 2, // Top      Right   corner
    BRC = 3, // Bottom   Right   corner
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


// Greedy meshing
void addLeftFace(char x, char y1, char z1, char y2, char z2, std::vector<PackedVertex>& vertices, std::vector<GLuint> &indices);
void addRightFace(char x, char y1, char z1, char y2, char z2, std::vector<PackedVertex>& vertices, std::vector<GLuint> &indices);

void addBottomFace(char y, char x1, char z1, char x2, char z2, std::vector<PackedVertex>& vertices, std::vector<GLuint> &indices);
void addTopFace(char y, char x1, char z1, char x2, char z2, std::vector<PackedVertex>& vertices, std::vector<GLuint> &indices);

void addBackFace(char z, char x1, char y1, char x2, char y2, std::vector<PackedVertex>& vertices, std::vector<GLuint> &indices);
void addFrontFace(char z, char x1, char y1, char x2, char y2, std::vector<PackedVertex>& vertices, std::vector<GLuint> &indices);


// Classic meshing
void addBackFace(char x, char y, char z, std::vector<PackedVertex>& vertices, std::vector<GLuint>& indices);
void addFrontFace(char x, char y, char z, std::vector<PackedVertex>& vertices, std::vector<GLuint>& indices);

void addLeftFace(char x, char y, char z, std::vector<PackedVertex>& vertices, std::vector<GLuint>& indices);
void addRightFace(char x, char y, char z, std::vector<PackedVertex>& vertices, std::vector<GLuint>& indices);

void addTopFace(char x, char y, char z, std::vector<PackedVertex>& vertices, std::vector<GLuint>& indices);
void addBottomFace(char x, char y, char z, std::vector<PackedVertex>& vertices, std::vector<GLuint>& indices);
