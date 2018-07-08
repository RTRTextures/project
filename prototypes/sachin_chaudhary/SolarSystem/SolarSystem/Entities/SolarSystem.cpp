#include "SolarSystem.h"


//-------------------------------------------------------------------------------------------------------
SolarSystem::SolarSystem()
{
}

//-------------------------------------------------------------------------------------------------------
SolarSystem::~SolarSystem()
{
}

//-------------------------------------------------------------------------------------------------------
void SolarSystem::AddSolarBody(SolarBody* solarbody)
{
   m_SolarBodies.push_back(solarbody);
}

//-------------------------------------------------------------------------------------------------------
void SolarSystem::Render(OGLProgram& program, mat4& projectionMatrix, mat4 viewMatrix)
{
   static mat4 mvp;

   program.Use(true);

   static GLint uniformMvp = program.GetUniformLocation("mvp");
   static GLint uniformSampler = program.GetUniformLocation("sampler");

   for(unsigned int i = 0; i < m_SolarBodies.size(); i++) {

      mvp = projectionMatrix * viewMatrix * m_SolarBodies[i]->GetModelMatrix();
      glUniformMatrix4fv(uniformMvp, 1, GL_FALSE, &mvp[0][0]);
      glUniform1i(uniformSampler, m_SolarBodies[i]->GetDiffuseTextureId());

      m_SolarBodies[i]->Render();
   }
}

