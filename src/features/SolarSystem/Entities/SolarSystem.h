#pragma once

#include "SolarBody.h"
#include <OGLProgram.h>

class SolarSystem
{
public:
   SolarSystem();
   ~SolarSystem();

   void AddSolarBody(SolarBody* solarbody);
   void Render(OGLProgram& program, mat4& projectionMatrix, mat4 viewMatrix);

private:
   vector<SolarBody*> m_SolarBodies;
};

