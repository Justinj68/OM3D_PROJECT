#include <vector>

#include <glad/glad.h>

void addIndices(std::vector<GLfloat> &vertices, std::vector<GLuint> &indices);


void addBackFace(GLfloat x, GLfloat y, GLfloat z, std::vector<GLfloat> &vertices, std::vector<GLuint> &indices);
void addFrontFace(GLfloat x, GLfloat y, GLfloat z, std::vector<GLfloat> &vertices, std::vector<GLuint> &indices);

void addLeftFace(GLfloat x, GLfloat y, GLfloat z, std::vector<GLfloat> &vertices, std::vector<GLuint> &indices);
void addRightFace(GLfloat x, GLfloat y, GLfloat z, std::vector<GLfloat> &vertices, std::vector<GLuint> &indices);

void addTopFace(GLfloat x, GLfloat y, GLfloat z, std::vector<GLfloat> &vertices, std::vector<GLuint> &indices);
void addBottomFace(GLfloat x, GLfloat y, GLfloat z, std::vector<GLfloat> &vertices, std::vector<GLuint> &indices);


void buildCube(float x, float y, float z, std::vector<GLfloat> &vertices, std::vector<GLuint> &indices);

