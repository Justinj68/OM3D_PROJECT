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
        void build(const std::vector<Chunk*> &neighbors, int* facesCount);
        void greedyBuild(const std::vector<Chunk*> &neighbors, int *facesCount);
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

        bool isVoxelTransparent(char x, char y, char z);
        bool isVoxelTransparent(char x, char y, char z, const std::vector<Chunk*> &neighbors);

        bool isFaceVisible(char x, char y, char z, Direction direction);
        bool isFaceVisible(char x, char y, char z, Direction direction, const std::vector<Chunk*> &neighbors);

        void processFaceY(char x, char y, char z, char processed[], const std::vector<Chunk*>& neighbors, std::vector<PackedVertex>& vertices, std::vector<GLuint>& indices, int* facesCount, DirectionMask mask, Direction direction);

        void processFaceXPositive(char x, char y, char z, char processed[], const std::vector<Chunk*> &neighbors, std::vector<PackedVertex> &vertices, std::vector<GLuint> &indices, int* facesCount);
        void processFaceXNegative(char x, char y, char z, char processed[], const std::vector<Chunk*> &neighbors, std::vector<PackedVertex> &vertices, std::vector<GLuint> &indices, int* facesCount);
        void processFaceYPositive(char x, char y, char z, char processed[], const std::vector<Chunk*> &neighbors, std::vector<PackedVertex> &vertices, std::vector<GLuint> &indices, int* facesCount);
        void processFaceYNegative(char x, char y, char z, char processed[], const std::vector<Chunk*> &neighbors, std::vector<PackedVertex> &vertices, std::vector<GLuint> &indices, int* facesCount);
        void processFaceZPositive(char x, char y, char z, char processed[], const std::vector<Chunk*> &neighbors, std::vector<PackedVertex> &vertices, std::vector<GLuint> &indices, int* facesCount);
        void processFaceZNegative(char x, char y, char z, char processed[], const std::vector<Chunk*> &neighbors, std::vector<PackedVertex> &vertices, std::vector<GLuint> &indices, int* facesCount);

        void uploadMeshData(std::vector<PackedVertex> &vertices, std::vector<GLuint> &indices);

        char getFaceMask(char x, char y, char z, const std::vector<Chunk*> &neighbors);

        void generateBuffers();
        void deleteBuffers();
};