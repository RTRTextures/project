#include "Shader.h"
using namespace Utility;

#include <fstream>
#include <sstream>
using namespace std;

//---------------------------------------------------------------------------------------------------
Shader::Shader(GLenum shaderType)
{
   switch(shaderType) {

   case GL_VERTEX_SHADER:
      m_ShaderId = glCreateShader(GL_VERTEX_SHADER);
      break;

   case GL_GEOMETRY_SHADER:
      m_ShaderId = glCreateShader(GL_GEOMETRY_SHADER);
      break;

   case GL_FRAGMENT_SHADER:
      m_ShaderId = glCreateShader(GL_FRAGMENT_SHADER);
      break;

   default:
      // either unsupported or invalid shader option
      m_ShaderId = -1;
      break;
   }

   m_isCompiled = false;
}

//---------------------------------------------------------------------------------------------------
Shader::~Shader()
{
   glDeleteShader(m_ShaderId);
}

//---------------------------------------------------------------------------------------------------
bool Shader::ReadFile(const char* fileName, std::string & contents)
{
   if(!(fileName && strlen(fileName) > 0))
      return false;

   bool status;

   ifstream file(fileName, ios::in);
   if(file.is_open() && file.good()) {
      stringstream ss;

      ss << file.rdbuf();
      contents = ss.str();
      file.close();

      status = true;
   }
   else
      status = false;

   file.close();

   return status;
}

//---------------------------------------------------------------------------------------------------
bool Shader::CheckCompilationErrors(string& errorMsg)
{
   GLint status;

   errorMsg = "";
   glGetShaderiv(m_ShaderId, GL_COMPILE_STATUS, &status);
   if(status != GL_TRUE) {
      GLint logLength, actualLogLength;

      glGetShaderiv(m_ShaderId, GL_INFO_LOG_LENGTH, &logLength);

      char* msg = new char[logLength + 1];
      glGetShaderInfoLog(m_ShaderId, logLength, &actualLogLength, msg);
      errorMsg = msg;
      delete[] msg;
   }

   return (status == GL_TRUE);
}

//---------------------------------------------------------------------------------------------------
bool Shader::SetSourceAndCompile(char* sourceFile, string& errorMsg)
{
   if(m_ShaderId == -1)
      return false;

   string contents;

   // read the shader source file
   if(!ReadFile(sourceFile, contents)) {
      errorMsg = "Unable to read the specified shader source file";
      return false;
   }

   // set the shader source
   const char* source = contents.data();
   glShaderSource(m_ShaderId, 1, &source, 0);

   // compile the shader
   glCompileShader(m_ShaderId);

   // perform error checking for the compiled shader
   m_isCompiled = CheckCompilationErrors(errorMsg);

   return m_isCompiled;
}
