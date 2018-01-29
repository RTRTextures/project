#include <glew.h>

#include <glm.hpp>
#include <gtx/transform.hpp>
using glm::vec3;
using glm::mat4;

#include "Model3D.h"
using namespace Utility;

//---------------------------------------------------------------------------------------------------
Model3D::Model3D()
{
   m_vaoId = 0;
   for(int i = 0; i < vertexAttributeLast; i++)
      m_BufferIds[i] = 0;
   
   m_vertexCount = 0;
}

//---------------------------------------------------------------------------------------------------
Model3D::~Model3D()
{
   UnBindVAO();
   glDeleteVertexArrays(1, &m_vaoId);

   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glDeleteBuffers(vertexAttributeLast, m_BufferIds);
}

//---------------------------------------------------------------------------------------------------
void Model3D::BindVAO()
{
   glBindVertexArray(m_vaoId);

   if(m_BufferIds[vertexAttributePosition] != 0)
      glEnableVertexAttribArray(vertexAttributePosition);
   
   if(m_BufferIds[vertexAttributeColor] != 0)
      glEnableVertexAttribArray(vertexAttributeColor);

   if(m_BufferIds[vertexAttributeNormal] != 0)
      glEnableVertexAttribArray(vertexAttributeNormal);

   if(m_BufferIds[vertexAttributeTexture] != 0)
      glEnableVertexAttribArray(vertexAttributeTexture);

   if(m_BufferIds[vertexAttributeTangent] != 0)
      glEnableVertexAttribArray(vertexAttributeTangent);
}

//---------------------------------------------------------------------------------------------------
void Model3D::UnBindVAO()
{
   if(m_BufferIds[vertexAttributePosition] != 0)
      glDisableVertexAttribArray(vertexAttributePosition);

   if(m_BufferIds[vertexAttributeColor] != 0)
      glDisableVertexAttribArray(vertexAttributeColor);

   if(m_BufferIds[vertexAttributeNormal] != 0)
      glDisableVertexAttribArray(vertexAttributeNormal);

   if(m_BufferIds[vertexAttributeTexture] != 0)
      glDisableVertexAttribArray(vertexAttributeTexture);

   if(m_BufferIds[vertexAttributeTangent] != 0)
      glDisableVertexAttribArray(vertexAttributeTangent);

   glBindVertexArray(0);
   glBindBuffer(GL_ARRAY_BUFFER, 0);
}


//---------------------------------------------------------------------------------------------------
void Model3D::Translate(vec3& translations)
{
   modelMatrix *= glm::translate(translations);
}

//---------------------------------------------------------------------------------------------------
void Model3D::Rotate(vec3& rotations)
{
   modelMatrix *= glm::rotate(rotations.x, vec3(1.0f, 0.0f, 0.0f));
   modelMatrix *= glm::rotate(rotations.y, vec3(0.0f, 1.0f, 0.0f));
   modelMatrix *= glm::rotate(rotations.z, vec3(0.0f, 0.0f, 1.0f));
}

//---------------------------------------------------------------------------------------------------
void Model3D::ResetPosition()
{
   modelMatrix = mat4();
}

//---------------------------------------------------------------------------------------------------
mat4 Model3D::GetModelMatrix()
{
   return modelMatrix;
}

//---------------------------------------------------------------------------------------------------
bool Model3D::CreateVertexBuffer(vector<vec3>& data)
{
   if(m_BufferIds[vertexAttributePosition] != 0) {
      glBindBuffer(GL_ARRAY_BUFFER, 0);
      glDeleteBuffers(1, &m_BufferIds[vertexAttributePosition]);
   }

   if(data.size() <= 0)
      return false;

   m_vertexCount = (GLsizei) data.size();
   
   glGenBuffers(1, &m_BufferIds[vertexAttributePosition]);
   glBindBuffer(GL_ARRAY_BUFFER, m_BufferIds[vertexAttributePosition]);
   glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(vec3), data.data(), GL_STATIC_DRAW);
   glEnableVertexAttribArray(vertexAttributePosition);
   glVertexAttribPointer(vertexAttributePosition, 3, GL_FLOAT, GL_FALSE, 0, 0);

   glBindBuffer(GL_ARRAY_BUFFER, 0);

   return true;
}

//---------------------------------------------------------------------------------------------------
bool Model3D::CreateColorBuffer(vector<vec3>& data)
{
   if(m_BufferIds[vertexAttributeColor] != 0) {
      glBindBuffer(GL_ARRAY_BUFFER, 0);
      glDeleteBuffers(1, &m_BufferIds[vertexAttributeColor]);
   }

   if(data.size() <= 0)
      return false;

   glGenBuffers(1, &m_BufferIds[vertexAttributeColor]);
   glBindBuffer(GL_ARRAY_BUFFER, m_BufferIds[vertexAttributeColor]);
   glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(vec3), data.data(), GL_STATIC_DRAW);
   glEnableVertexAttribArray(vertexAttributeColor);
   glVertexAttribPointer(vertexAttributeColor, 3, GL_FLOAT, GL_FALSE, 0, 0);

   glBindBuffer(GL_ARRAY_BUFFER, 0);

   return true;
}

//---------------------------------------------------------------------------------------------------
bool Model3D::CreateTextureBuffer(vector<vec3>& data)
{
   if(data.size() <= 0)
      return false;

   if(m_BufferIds[vertexAttributeTexture] != 0) {
      glBindBuffer(GL_ARRAY_BUFFER, 0);
      glDeleteBuffers(1, &m_BufferIds[vertexAttributeTexture]);
   }

   glGenBuffers(1, &m_BufferIds[vertexAttributeTexture]);
   glBindBuffer(GL_ARRAY_BUFFER, m_BufferIds[vertexAttributeTexture]);
   glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(vec3), data.data(), GL_STATIC_DRAW);
   glEnableVertexAttribArray(vertexAttributeTexture);
   glVertexAttribPointer(vertexAttributeTexture, 3, GL_FLOAT, GL_FALSE, 0, 0);

   glBindBuffer(GL_ARRAY_BUFFER, 0);

   return true;
}

//---------------------------------------------------------------------------------------------------
bool Model3D::CreateNormalBuffer(vector<vec3>& data)
{
   if(data.size() <= 0)
      return false;

   if(m_BufferIds[vertexAttributeNormal] != 0) {
      glBindBuffer(GL_ARRAY_BUFFER, 0);
      glDeleteBuffers(1, &m_BufferIds[vertexAttributeNormal]);
   }

   glGenBuffers(1, &m_BufferIds[vertexAttributeNormal]);
   glBindBuffer(GL_ARRAY_BUFFER, m_BufferIds[vertexAttributeNormal]);
   glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(vec3), data.data(), GL_STATIC_DRAW);
   glEnableVertexAttribArray(vertexAttributeNormal);
   glVertexAttribPointer(vertexAttributeNormal, 3, GL_FLOAT, GL_FALSE, 0, 0);

   glBindBuffer(GL_ARRAY_BUFFER, 0);

   return true;
}

//---------------------------------------------------------------------------------------------------
bool Model3D::CreateTangentBuffer(vector<vec4>& data)
{
   if(data.size() <= 0)
      return false;

   if(m_BufferIds[vertexAttributeTangent] != 0) {
      glBindBuffer(GL_ARRAY_BUFFER, 0);
      glDeleteBuffers(1, &m_BufferIds[vertexAttributeTangent]);
   }

   glGenBuffers(1, &m_BufferIds[vertexAttributeTangent]);
   glBindBuffer(GL_ARRAY_BUFFER, m_BufferIds[vertexAttributeTangent]);
   glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(vec4), data.data(), GL_STATIC_DRAW);
   glEnableVertexAttribArray(vertexAttributeTangent);
   glVertexAttribPointer(vertexAttributeTangent, 4, GL_FLOAT, GL_FALSE, 0, 0);

   glBindBuffer(GL_ARRAY_BUFFER, 0);

   return true;
}

//---------------------------------------------------------------------------------------------------
bool Model3D::CreateVAO()
{
   if(m_vaoId != 0) {
      glBindVertexArray(0);
      glDeleteVertexArrays(1, &m_vaoId);
   }

   glGenVertexArrays(1, &m_vaoId);
   glBindVertexArray(m_vaoId);

   if(m_BufferIds[vertexAttributePosition] != 0) {
      glBindBuffer(GL_ARRAY_BUFFER, m_BufferIds[vertexAttributePosition]);
      glEnableVertexAttribArray(vertexAttributePosition);
      glVertexAttribPointer(vertexAttributePosition, 3, GL_FLOAT, GL_FALSE, 0, 0);
   }

   if(m_BufferIds[vertexAttributeColor] != 0) {
      glBindBuffer(GL_ARRAY_BUFFER, m_BufferIds[vertexAttributeColor]);
      glEnableVertexAttribArray(vertexAttributeColor);
      glVertexAttribPointer(vertexAttributeColor, 3, GL_FLOAT, GL_FALSE, 0, 0);
   }

   if(m_BufferIds[vertexAttributeNormal] != 0) {
      glBindBuffer(GL_ARRAY_BUFFER, m_BufferIds[vertexAttributeNormal]);
      glEnableVertexAttribArray(vertexAttributeNormal);
      glVertexAttribPointer(vertexAttributeNormal, 3, GL_FLOAT, GL_FALSE, 0, 0);
   }

   if(m_BufferIds[vertexAttributeTexture] != 0) {
      glBindBuffer(GL_ARRAY_BUFFER, m_BufferIds[vertexAttributeTexture]);
      glEnableVertexAttribArray(vertexAttributeTexture);
      glVertexAttribPointer(vertexAttributeTexture, 3, GL_FLOAT, GL_FALSE, 0, 0);
   }

   if(m_BufferIds[vertexAttributeTangent] != 0) {
      glBindBuffer(GL_ARRAY_BUFFER, m_BufferIds[vertexAttributeTangent]);
      glEnableVertexAttribArray(vertexAttributeTangent);
      glVertexAttribPointer(vertexAttributeTangent, 4, GL_FLOAT, GL_FALSE, 0, 0);
   }

   glBindVertexArray(0);

   return (m_vaoId != 0);
}

//---------------------------------------------------------------------------------------------------
void Model3D::Render(GLenum mode)
{
   //if(m_vaoId != 0)
   //   CreateVAO();

   BindVAO();
   glDrawArrays(mode, 0, m_vertexCount);
   UnBindVAO();
}
