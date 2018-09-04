#pragma once

#include <glew.h>

#include <vector>
using namespace std;

#include <glm.hpp>
using glm::vec3;
using glm::vec2;

class Space
{
public:
   Space(float size);
   ~Space();

   bool Initialize();
   void Render(GLenum mode);

private:
   void LoadData(vector<vec3>& vertices, vector<vec2>& texcoords);

private:
   GLsizei m_count;
   GLuint m_vbuffer, m_tbuffer, m_vao;
   GLuint m_texture;

   float m_size;
};
