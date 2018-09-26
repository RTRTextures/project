#pragma once 

#include <glew.h>
#include <vector>
#include "glm/mat4x4.hpp"

enum BufferType
{
    VERTEX = 0,
    NORMALS,
    TEXTURECOORDS,
    TANGENTS
};

void VSIUtilGenAndBindBuffer(GLuint buffer,
    GLuint attribLocation,
    BufferType bufferType,
    std::vector<glm::vec3>&,
    std::vector<glm::vec2>&,
    std::vector<glm::vec3>&);