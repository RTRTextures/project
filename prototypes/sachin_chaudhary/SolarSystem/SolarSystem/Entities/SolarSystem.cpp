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

   for(unsigned int i = 0; i < m_SolarBodies.size(); i++) {
      m_SolarBodies[i]->Render(program, projectionMatrix, viewMatrix);
   }
}

