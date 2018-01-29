#include "Program.h"

using namespace std;
using namespace Utility;

//---------------------------------------------------------------------------------------------------
Program::Program()
{
   m_ProgramId = glCreateProgram();
   m_defaultUniforms = false;
}

//---------------------------------------------------------------------------------------------------
Program::~Program()
{
   for(unsigned int i = 0; i < m_ShaderIds.size(); i++)
      glDetachShader(m_ProgramId, m_ShaderIds[i]);

   glUseProgram(0);
}

//---------------------------------------------------------------------------------------------------
bool Program::AttachShader(Shader& shader)
{
   GLuint shaderId = shader.GetId();

   if(shaderId != -1 && shader.IsCompiled()) {
      glAttachShader(m_ProgramId, shaderId);
      m_ShaderIds.push_back(shaderId);

      return true;
   }

   return false;
}

//---------------------------------------------------------------------------------------------------
bool Program::DetachShader(Shader& shader)
{
   GLuint shaderId = shader.GetId();

   vector<GLuint>::iterator it = std::find(m_ShaderIds.begin(), m_ShaderIds.end(), shaderId);
   if(it != m_ShaderIds.end()) {
      glDetachShader(m_ProgramId, shaderId);
      m_ShaderIds.erase(it);

      return true;
   }

   return false;
}

//---------------------------------------------------------------------------------------------------
bool Program::Link(bool initDefaultUniforms, string& errorMsg)
{
   GLint status;

   errorMsg = "";
   glLinkProgram(m_ProgramId);

   glGetProgramiv(m_ProgramId, GL_LINK_STATUS, &status);
   if(status != GL_TRUE) {
      GLint logLength, actualLogLength;

      glGetProgramiv(m_ProgramId, GL_INFO_LOG_LENGTH, &logLength);

      char* msg = new char[logLength + 1];
      glGetProgramInfoLog(m_ProgramId, logLength, &actualLogLength, msg);
      errorMsg = msg;
      delete[] msg;
   }

   m_isLinked = (status == GL_TRUE);

   if(initDefaultUniforms) {
      m_defaultUniforms = true;

      Use();
      InitDefaultUniforms();
      DoNotUse();
   }

   return m_isLinked;
}

//---------------------------------------------------------------------------------------------------
void Program::Use()
{
   if(m_isLinked)
      glUseProgram(m_ProgramId);
}

//---------------------------------------------------------------------------------------------------
void Program::DoNotUse()
{
   glUseProgram(0);
}

//---------------------------------------------------------------------------------------------------
void Program::InitDefaultUniforms()
{
   Uniform u;
   vector<Uniform> uniforms;

   u.Id = _uniformModelMatrix_;
   u.uniformName = "modelMatrix";
   uniforms.push_back(u);

   u.Id = _uniformViewMatrix_;
   u.uniformName = "viewMatrix";
   uniforms.push_back(u);

   u.Id = _uniformProjectionMatrix_;
   u.uniformName = "projectionMatrix";
   uniforms.push_back(u);

   u.Id = _uniformLightLocationWorld_;
   u.uniformName = "lightLocationWorld";
   uniforms.push_back(u);

   u.Id = _uniformCameraLocationWorld_;
   u.uniformName = "cameraLocationWorld";
   uniforms.push_back(u);

   InitializeUniforms(uniforms);
}

//---------------------------------------------------------------------------------------------------
bool Program::InitializeUniforms(vector<Uniform>& uniforms)
{
   if(!m_isLinked)
      return false;

   // a necessary step before getting the uniform locations
   glUseProgram(m_ProgramId);

   for(int i = 0; i < uniforms.size(); i++)
      m_mapLocations.insert(pair<int, GLint>(uniforms[i].Id, glGetUniformLocation(m_ProgramId, uniforms[i].uniformName.c_str())));

   return true;
}

//---------------------------------------------------------------------------------------------------
GLint Program::GetUniformLocation(unsigned int Id)
{
   GLint location = -1;

   if(m_mapLocations.find(Id) != m_mapLocations.end())
      location = m_mapLocations[Id];

   return location;
}

//---------------------------------------------------------------------------------------------------wq
bool Program::SetUniform1i(unsigned int Id, int value)
{
   GLint location = GetUniformLocation(Id);

   if(location != -1)
      glUniform1i(location, value);

   return (location != -1);
}

//---------------------------------------------------------------------------------------------------
bool Program::SetUniform1f(unsigned int Id, float value)
{
   GLint location = GetUniformLocation(Id);

   if(location != -1)
      glUniform1f(location, value);

   return (location != -1);
}

//---------------------------------------------------------------------------------------------------
bool Program::SetUniformMatrix4fv(unsigned int Id, GLfloat* value)
{
   GLint location = GetUniformLocation(Id);

   if(location != -1)
      glUniformMatrix4fv(location, 1, GL_FALSE, value);

   return (location != -1);
}

//---------------------------------------------------------------------------------------------------
bool Program::SetUniform3fv(unsigned int Id, GLfloat* value)
{
   GLint location = GetUniformLocation(Id);

   if(location != -1)
      glUniform3fv(location, 1, value);

   return (location != -1);
}

//---------------------------------------------------------------------------------------------------
bool Program::SetUniform4fv(unsigned int Id, GLfloat * value)
{
   GLint location = GetUniformLocation(Id);

   if(location != -1)
      glUniform4fv(location, 1, value);

   return (location != -1);
}

//---------------------------------------------------------------------------------------------------
void Program::SetLightLocation(GLfloat* value)
{
   SetUniform3fv(_uniformLightLocationWorld_, value);
}

//---------------------------------------------------------------------------------------------------
void Program::SetCameraLocation(GLfloat * value)
{
   SetUniform3fv(_uniformCameraLocationWorld_, value);
}

//---------------------------------------------------------------------------------------------------
void Program::SetProjectionMatrix(GLfloat* value)
{
   SetUniformMatrix4fv(_uniformProjectionMatrix_, value);
}

//---------------------------------------------------------------------------------------------------
void Program::SetModelMatrix(GLfloat* value)
{
   SetUniformMatrix4fv(_uniformModelMatrix_, value);
}

//---------------------------------------------------------------------------------------------------
void Program::SetViewMatrix(GLfloat* value)
{
   SetUniformMatrix4fv(_uniformViewMatrix_, value);
}
