#pragma once

#include <vector>

#include <glm/glm.hpp>

#include <cube_mesh.h>
#include <graphics/shader.h>
#include <terrain_generator.h>
#include <voxel.h>

#define CHUNK_DIM 32

class Chunk {
    public:
        Chunk();
        Chunk(const glm::vec3 &position);
        ~Chunk();

        void render(const Shader &shaderProgram);
        void build(const std::vector<Chunk*> &neighbors);
        void greedyBuild(const std::vector<Chunk*> &neighbors);
        void defineCave(TerrainGenerator &terrainGenerator);
        void defineVoxelData(TerrainGenerator &terrainGenerator);

        GLuint getVAO() const;
        glm::vec3 getCenterPosition() const;
        void setPosition(int x, int y, int z);
        void setPosition(const glm::vec3 &position);

        void destroy();
    private:
        GLuint _VAO, _VBO, _EBO;
        size_t _indicesCount;
        Voxel::Type* _voxels;
        glm::vec3 _position;
        bool _empty;

        bool isVoxelTransparent(int x, int y, int z);
        bool isVoxelTransparent(int x, int y, int z, const std::vector<Chunk*> &neighbors);

        bool isFaceVisible(int x, int y, int z, FaceDirection direction);
        bool isFaceVisible(int x, int y, int z, FaceDirection direction, const std::vector<Chunk*> &neighbors);

        void processFaceXPositive(int x, int y, int z, char mask[], const std::vector<Chunk*> &neighbors, std::vector<PackedVertex> &vertices, std::vector<GLuint> &indices);
        void processFaceXNegative(int x, int y, int z, char mask[], const std::vector<Chunk*> &neighbors, std::vector<PackedVertex> &vertices, std::vector<GLuint> &indices);
        void processFaceYPositive(int x, int y, int z, char mask[], const std::vector<Chunk*> &neighbors, std::vector<PackedVertex> &vertices, std::vector<GLuint> &indices);
        void processFaceYNegative(int x, int y, int z, char mask[], const std::vector<Chunk*> &neighbors, std::vector<PackedVertex> &vertices, std::vector<GLuint> &indices);
        void processFaceZPositive(int x, int y, int z, char mask[], const std::vector<Chunk*> &neighbors, std::vector<PackedVertex> &vertices, std::vector<GLuint> &indices);
        void processFaceZNegative(int x, int y, int z, char mask[], const std::vector<Chunk*> &neighbors, std::vector<PackedVertex> &vertices, std::vector<GLuint> &indices);

        void uploadMeshData(std::vector<PackedVertex> &vertices, std::vector<GLuint> &indices);

        char getFaceMask(int x, int y, int z, const std::vector<Chunk*> &neighbors);

        void generateBuffers();
        void deleteBuffers();
};