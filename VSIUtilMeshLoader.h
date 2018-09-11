#pragma once

enum MeshType
{
    UNKNOWN,
    ONLY_VERTEX,
    VERTEX_AND_NORMAL,
    VERTEX_AND_TEXTURE,
    VERTEX_NORMAL_AND_TEXTURE
};

void VSIUtilLoadMesh(char*, MeshType, 
    std::vector<glm::vec3>&,
    std::vector<glm::vec2>&,
    std::vector<glm::vec3>&);