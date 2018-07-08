#pragma once

#include <glew.h>

#include <glm.hpp>
using glm::vec3;
using glm::mat4;

#include <vector>
#include <string>
using namespace std;

#include "..\Framework\OGLProgram.h"

class SolarBody
{
public:

   typedef struct
   {
      GLuint textureId;
      GLenum textureUnit;
   } Texture;

   SolarBody();
   ~SolarBody();

   static bool Initialize();
   static void Uninitialize();

   void Scale(float scale);
   void Rotate(float speed);
   void Revolve(float radius, float speed);

   const mat4& GetModelMatrix();
   void SetTexture(const string& textureFile, GLenum activeTextureUnit, bool mipmaps);
   unsigned int GetDiffuseTextureId();
   virtual void Render();

private:
   static GLuint m_vertexBuffer, m_normalBuffer, m_textureBuffer, m_tangentBuffer, m_vao;
   static bool m_isInitialized;
   static size_t m_count;

   unsigned int m_diffuseTextureId;
   
   float m_scale;
   float m_rotateSpeed, m_rotateAngle;
   float m_revolveSpeed, m_revolveAngle, m_revolveRadius;

  
   vector<Texture> m_textures;
   mat4 m_modelMatrix;
};
