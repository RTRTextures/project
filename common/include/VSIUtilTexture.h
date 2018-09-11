#pragma once
#include <vector>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <glew.h>

void VSIUtilLoadCubeMapTextures(
    std::vector<std::string> textureFiles, 
    GLuint *texture);