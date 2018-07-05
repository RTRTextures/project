#include "SolarBody.h"
#include "..\Framework\Loader.h"

//GLuint SolarBody::m_vertexBuffer = 0;
//GLuint SolarBody::m_normalBuffer = 0;
//GLuint SolarBody::m_textureBuffer = 0;
//GLuint SolarBody::m_tangentBuffer = 0;
//GLuint SolarBody::m_vao = 0;
//
//bool SolarBody::m_isInitialized;

//-------------------------------------------------------------------------------------------------------
SolarBody::SolarBody()
{
   m_scale = 0.0f;
   m_rotateSpeed = m_revolveSpeed = m_revolveRadius = 0.0f;
   m_rotateAngle = m_revolveAngle = 360.0f;
}

//-------------------------------------------------------------------------------------------------------
SolarBody::~SolarBody()
{
   glBindVertexArray(0);
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   glDeleteVertexArrays(1, &m_vao);

   glDeleteBuffers(1, &m_vertexBuffer);
   glDeleteBuffers(1, &m_normalBuffer);
   glDeleteBuffers(1, &m_textureBuffer);
   glDeleteBuffers(1, &m_tangentBuffer);

   for(unsigned int i = 0; i < m_textures.size(); i++)
      glDeleteTextures(1, &m_textures[i].textureId);
}

//-------------------------------------------------------------------------------------------------------
bool SolarBody::Initialize()
{
   if(m_isInitialized)
      return true;

   MeshData mdata;

   if(!Loader::LoadMesh("res/meshes/solarbody.obj", mdata, true))
      return false;

   m_count = mdata.vertices.size();

   glGenVertexArrays(1, &m_vao);

   glGenBuffers(1, &m_vertexBuffer);
   glGenBuffers(1, &m_normalBuffer);
   glGenBuffers(1, &m_textureBuffer);
   glGenBuffers(1, &m_tangentBuffer);

   glBindVertexArray(m_vao);
   // vertices
   glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
   glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * mdata.vertices.size(), mdata.vertices.data(), GL_STATIC_DRAW);
   glEnableVertexAttribArray(0);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
   glBindBuffer(GL_ARRAY_BUFFER, 0);


   // normals
   glBindBuffer(GL_ARRAY_BUFFER, m_normalBuffer);
   glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * mdata.normals.size(), mdata.normals.data(), GL_STATIC_DRAW);
   glEnableVertexAttribArray(2);
   glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   // texcoords
   glBindBuffer(GL_ARRAY_BUFFER, m_textureBuffer);
   glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * mdata.texcoords.size(), mdata.texcoords.data(), GL_STATIC_DRAW);
   glEnableVertexAttribArray(3);
   glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   // tangents
   glBindBuffer(GL_ARRAY_BUFFER, m_tangentBuffer);
   glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * mdata.tangents.size(), mdata.tangents.data(), GL_STATIC_DRAW);
   glEnableVertexAttribArray(4);
   glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, 0);
   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindVertexArray(0);


   m_isInitialized=  m_vertexBuffer != 0 &&
      m_normalBuffer != 0 &&
      m_textureBuffer != 0 &&
      m_tangentBuffer != 0;

   return m_isInitialized;
}

//-------------------------------------------------------------------------------------------------------
void SolarBody::Render()
{
   glBindVertexArray(m_vao);
   glDrawArrays(GL_TRIANGLES, 0, (GLsizei) m_count);
   glBindVertexArray(0);

   // scale
   m_modelMatrix = glm::scale(vec3(m_scale));

   // do revolutions using translations; revolutions are in the x-z plane
   if(m_revolveSpeed > 0 && m_revolveRadius > 0) {

      m_modelMatrix = glm::translate(m_modelMatrix, vec3(m_revolveRadius * glm::cos(glm::radians(m_revolveAngle)),
                                                         0.0f,
                                                         m_revolveRadius * glm::sin(glm::radians(m_revolveAngle))));
      m_revolveAngle -= m_revolveSpeed;

      if(m_revolveAngle <= 0.0f)
         m_revolveAngle = 360.0f;
   }

   // add the rotations to the translated position
   if(m_rotateSpeed > 0) {
      m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(m_rotateAngle), vec3(0.0f, 1.0f, 0.0f));

      m_rotateAngle -= m_rotateSpeed;
      if(m_rotateAngle <= 0.0f)
         m_rotateAngle = 360.0f;
   }
}

//-------------------------------------------------------------------------------------------------------
void SolarBody::Scale(float scale)
{
   m_scale = scale;
}

//-------------------------------------------------------------------------------------------------------
void SolarBody::Rotate(float speed)
{
   m_rotateSpeed = speed;
}

//-------------------------------------------------------------------------------------------------------
void SolarBody::Revolve(float radius, float speed)
{
   m_revolveRadius = radius;
   m_revolveSpeed = speed;
}

//-------------------------------------------------------------------------------------------------------
const mat4& SolarBody::GetModelMatrix()
{
   return m_modelMatrix;
}

//-------------------------------------------------------------------------------------------------------
void SolarBody::SetTexture(const string& textureFile,
                                   GLenum activeTextureUnit,
                                   bool mipmaps)
{
   m_diffuseTextureId = -1;
   if(textureFile == "")
      return;

   Texture texture;

   bool result = Loader::Load2DTexture(textureFile.c_str(), texture.textureId,
                                       activeTextureUnit, mipmaps);
   texture.textureUnit = activeTextureUnit;
   m_textures.push_back(texture);

   m_diffuseTextureId = result ? texture.textureUnit - GL_TEXTURE0 : -1;
}

//-------------------------------------------------------------------------------------------------------
unsigned int SolarBody::GetDiffuseTextureId()
{
   return m_diffuseTextureId;
}
