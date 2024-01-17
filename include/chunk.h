#pragma once

#include <vector>

#include <glm/glm.hpp>

#include <cube_mesh.h>
#include <graphics/shader.h>
#include <voxel.h>

#define CHUNK_DIM 32

class Chunk {
    public:
        Chunk();
        Chunk(const glm::vec3 &position);

        void build();
        void defineVoxelData();
        void render(const Shader &shaderProgram);
        void build(const std::vector<Chunk*> &neighbors);

        GLuint getVAO() const;

        void setPosition(int x, int y, int z);
        void setPosition(const glm::vec3 &position);

        ~Chunk();
    private:
        GLuint _VAO;
        size_t _indicesCount;
        Voxel::Type* _voxels;
        glm::vec3 _position;

        bool isVoxelTransparent(int x, int y, int z);
        void manageBuffers(std::vector<GLfloat> &vertices, std::vector<GLuint> &indices);
        bool isVoxelTransparent(int x, int y, int z, const std::vector<Chunk*> &neighbors);
        void manageBuffers(std::vector<PackedVertex> &vertices, std::vector<GLuint> &indices);
};