#include "Space.h"
#include "..\Framework\Loader.h"

#include <string>
using namespace std;

//----------------------------------------------------------------------------------------
Space::Space(float size)
{
   m_size = size;
   m_vbuffer = m_tbuffer = 0;
   m_vao = 0;

   m_texture = 0;

   m_count = 0;
}

//----------------------------------------------------------------------------------------
Space::~Space()
{
   glDeleteVertexArrays(1, &m_vao);

   glDeleteBuffers(1, &m_vbuffer);
   glDeleteBuffers(1, &m_tbuffer);

   glDeleteTextures(1, &m_texture);
}

//----------------------------------------------------------------------------------------
bool Space::Initialize()
{
   vector<vec3> points, normals;
   vector<vec2> texcoords;

   LoadData(points, texcoords);

   m_count = (GLsizei) points.size();

   glGenVertexArrays(1, &m_vao);
   glGenBuffers(1, &m_vbuffer);
   glGenBuffers(1, &m_tbuffer);

   glBindVertexArray(m_vao);
      // vertices
      glBindBuffer(GL_ARRAY_BUFFER, m_vbuffer);
      glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * points.size(), points.data(), GL_STATIC_DRAW);
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

      // texcoords
      glBindBuffer(GL_ARRAY_BUFFER, m_tbuffer);
      glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * texcoords.size(), texcoords.data(), GL_STATIC_DRAW);
      glEnableVertexAttribArray(3);
      glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, 0);
   glBindVertexArray(0);

   bool result = true;
   vector<string> textures;

   string filename = "res/textures/space/2k_stars.jpg";
   for(int i=0; i<6; i++)
      textures.push_back(filename);

   result &= Loader::LoadCubeMapTexture(textures, m_texture, GL_TEXTURE0 + 99, true);

   return result;
}

//----------------------------------------------------------------------------------------
void Space::Render(GLenum mode)
{
   glBindVertexArray(m_vao);
      glDrawArrays(mode, 0, m_count);
   glBindVertexArray(0);
}

//----------------------------------------------------------------------------------------
void Space::LoadData(vector<vec3>& vertices, vector<vec2>& texcoords)
{
   vec3 vertex;
   vec2 texcoord;
   float halfSize = 0.5f * m_size;

   // front face
   vertex = {halfSize, halfSize, halfSize};
   texcoord = {1.0f, 0.0f};
   vertices.push_back(vertex);
   texcoords.push_back(texcoord);

   vertex = {-halfSize, halfSize, halfSize};
   texcoord = {0.0f, 0.0f};
   vertices.push_back(vertex);
   texcoords.push_back(texcoord);

   vertex = {-halfSize, -halfSize, halfSize};
   texcoord = {0.0f, 1.0f};
   vertices.push_back(vertex);
   texcoords.push_back(texcoord);

   vertex = {halfSize, -halfSize, halfSize};
   texcoord = {1.0f, 1.0f};
   vertices.push_back(vertex);
   texcoords.push_back(texcoord);

   // right face
   vertex = {halfSize, halfSize, -halfSize};
   texcoord = {1.0f, 0.0f};
   vertices.push_back(vertex);
   texcoords.push_back(texcoord);

   vertex = {halfSize, halfSize, halfSize};
   texcoord = {0.0f, 0.0f};
   vertices.push_back(vertex);
   texcoords.push_back(texcoord);

   vertex = {halfSize, -halfSize, halfSize};
   texcoord = {0.0f, 1.0f};
   vertices.push_back(vertex);
   texcoords.push_back(texcoord);

   vertex = {halfSize, -halfSize, -halfSize};
   texcoord = {1.0f, 1.0f};
   vertices.push_back(vertex);
   texcoords.push_back(texcoord);

   // back face
   vertex = {-halfSize, halfSize, -halfSize};
   texcoord = {1.0f, 0.0f};
   vertices.push_back(vertex);
   texcoords.push_back(texcoord);

   vertex = {halfSize, halfSize, -halfSize};
   texcoord = {0.0f, 0.0f};
   vertices.push_back(vertex);
   texcoords.push_back(texcoord);

   vertex = {halfSize, -halfSize, -halfSize};
   texcoord = {0.0f, 1.0f};
   vertices.push_back(vertex);
   texcoords.push_back(texcoord);

   vertex = {-halfSize, -halfSize, -halfSize};
   texcoord = {1.0f, 1.0f};
   vertices.push_back(vertex);
   texcoords.push_back(texcoord);

   // left face
   vertex = {-halfSize, halfSize, halfSize};
   texcoord = {1.0f, 0.0f};
   vertices.push_back(vertex);
   texcoords.push_back(texcoord);

   vertex = {-halfSize, halfSize, -halfSize};
   texcoord = {0.0f, 0.0f};
   vertices.push_back(vertex);
   texcoords.push_back(texcoord);

   vertex = {-halfSize, -halfSize, -halfSize};
   texcoord = {0.0f, 1.0f};
   vertices.push_back(vertex);
   texcoords.push_back(texcoord);

   vertex = {-halfSize, -halfSize, halfSize};
   texcoord = {1.0f, 1.0f};
   vertices.push_back(vertex);
   texcoords.push_back(texcoord);

   // top face
   vertex = {halfSize, halfSize, -halfSize};
   texcoord = {1.0f, 0.0f};
   vertices.push_back(vertex);
   texcoords.push_back(texcoord);

   vertex = {-halfSize, halfSize, -halfSize};
   texcoord = {0.0f, 0.0f};
   vertices.push_back(vertex);
   texcoords.push_back(texcoord);

   vertex = {-halfSize, halfSize, halfSize};
   texcoord = {0.0f, 1.0f};
   vertices.push_back(vertex);
   texcoords.push_back(texcoord);

   vertex = {halfSize, halfSize, halfSize};
   texcoord = {1.0f, 1.0f};
   vertices.push_back(vertex);
   texcoords.push_back(texcoord);

   // bottom face
   vertex = {halfSize, -halfSize, halfSize};
   texcoord = {1.0f, 0.0f};
   vertices.push_back(vertex);
   texcoords.push_back(texcoord);

   vertex = {-halfSize, -halfSize, halfSize};
   texcoord = {0.0f, 0.0f};
   vertices.push_back(vertex);
   texcoords.push_back(texcoord);

   vertex = {-halfSize, -halfSize, -halfSize};
   texcoord = {0.0f, 1.0f};
   vertices.push_back(vertex);
   texcoords.push_back(texcoord);

   vertex = {halfSize, -halfSize, -halfSize};
   texcoord = {1.0f, 1.0f};
   vertices.push_back(vertex);
   texcoords.push_back(texcoord);
}
