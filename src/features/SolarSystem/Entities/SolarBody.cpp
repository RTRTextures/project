#include "SolarBody.h"
#include <Loader.h>
using glm::mat3;

GLuint SolarBody::m_vertexBuffer = 0;
GLuint SolarBody::m_normalBuffer = 0;
GLuint SolarBody::m_textureBuffer = 0;
GLuint SolarBody::m_tangentBuffer = 0;
GLuint SolarBody::m_vao = 0;

size_t SolarBody::m_count = 0;
bool SolarBody::m_isInitialized;

const float speedScale = 1.0f;

//-------------------------------------------------------------------------------------------------------
const vec3& SolarBody::RotateAroundAxis(vec3 axis, float radius, float angle)
{
   axis = glm::normalize(axis);

   float C = radius * glm::cos(glm::radians(angle));
   float S = radius * glm::sin(glm::radians(angle));
   float t = 1 - C;

   static vec3 pt;

   pt[0] = (t * axis.x * axis.x) + C;
   pt[1] = (t * axis.x * axis.y) - (S * axis.z);
   pt[2] = (t * axis.x * axis.z) + (S * axis.y);

   return pt;
}

//-------------------------------------------------------------------------------------------------------
SolarBody::SolarBody(const SolarData& data)
{
   m_scale = data.scale;
   m_rotateSpeed = speedScale * data.rotation_speed;
   m_revolveSpeed = speedScale * data.revolution_speed;
   m_revolveRadius = data.revolution_radius;

   m_axis = data.axis;

   SetTexture(data.texture_file, data.textureUnit, data.mipmap);

   // assume anti-clockwise rotations (at least for now)
   m_rotateAngle = m_revolveAngle = 360.0f;

   m_lastProgram = nullptr;
}

//-------------------------------------------------------------------------------------------------------
SolarBody::~SolarBody()
{
   for(unsigned int i = 0; i < m_satellites.size(); i++)
      delete m_satellites[i];

   for(unsigned int i = 0; i < m_textures.size(); i++)
      glDeleteTextures(1, &m_textures[i].textureId);
}

//-------------------------------------------------------------------------------------------------------
bool SolarBody::Initialize()
{
   if(m_isInitialized)
      return true;

   MeshData mdata;
   if(!Loader::LoadMesh("res/SolarSystem/meshes/solarbody.obj", mdata, true))
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


   m_isInitialized = m_vertexBuffer != 0 &&
      m_normalBuffer != 0 &&
      m_textureBuffer != 0 &&
      m_tangentBuffer != 0;

   return m_isInitialized;
}

//-------------------------------------------------------------------------------------------------------
void SolarBody::Uninitialize()
{
   glBindVertexArray(0);
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   glDeleteVertexArrays(1, &m_vao);

   glDeleteBuffers(1, &m_vertexBuffer);
   glDeleteBuffers(1, &m_normalBuffer);
   glDeleteBuffers(1, &m_textureBuffer);
   glDeleteBuffers(1, &m_tangentBuffer);

   m_vertexBuffer = m_normalBuffer = m_textureBuffer = m_tangentBuffer = m_vao = 0;
   m_count = 0;
   m_isInitialized = false;
}

//-----------------------------------------------------------------------------------------------------------------
void SolarBody::Render(OGLProgram& program, mat4& projectionMatrix, mat4 viewMatrix, const mat4& offsetMatrix)
{
   static GLint uniformModelMatrix;
   static GLint uniformProjectionViewMatrix;
   static GLint uniformSampler;

   program.Use(true);
   if(m_lastProgram != &program) {
      uniformModelMatrix = program.GetUniformLocation("modelMatrix");
      uniformProjectionViewMatrix = program.GetUniformLocation("projectionViewMatrix");
      uniformSampler = program.GetUniformLocation("sampler");

      m_lastProgram = &program;
   }

   m_modelMatrix = offsetMatrix;

   static mat3 m;
   static vec3 v;

   v = RotateAroundAxis(m_axis, m_revolveRadius, m_revolveAngle);

   // do revolutions using translations; revolutions are around the specified axis through the origin
   if(m_revolveRadius > 0) {
      m_modelMatrix = glm::translate(m_modelMatrix, v);

      // render all the satellites: this will be recursive
      for(unsigned int i = 0; i < m_satellites.size(); i++)
         m_satellites[i]->Render(program, projectionMatrix, viewMatrix, m_modelMatrix);

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

   // perform scaling at the end
   m_modelMatrix = glm::scale(m_modelMatrix, vec3(m_scale));

   static mat4 projectionViewMatrix;
   projectionViewMatrix = projectionMatrix * viewMatrix;
   glUniformMatrix4fv(uniformModelMatrix, 1, GL_FALSE, &m_modelMatrix[0][0]);
   glUniformMatrix4fv(uniformProjectionViewMatrix, 1, GL_FALSE, &projectionViewMatrix[0][0]);

   glUniform1i(uniformSampler, m_diffuseTextureId);

   // render the solar body
   glBindVertexArray(m_vao);
   glDrawArrays(GL_TRIANGLES, 0, (GLsizei) m_count);
   glBindVertexArray(0);

   //program.Use(false);
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

//-------------------------------------------------------------------------------------------------------
bool SolarBody::AddSatellite(const SolarData& data)
{
   SolarBody* satellite = new SolarBody(data);
   m_satellites.push_back(satellite);

   return true;
}
