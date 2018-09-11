#pragma once

#include <glew.h>
#include <gl\GL.h>
#include <glm\glm.hpp>

#include <string>
using namespace std;


class OGLProgram
{
public:
   OGLProgram();
   ~OGLProgram();

   bool Initialize(const string& vShaderFile, const string& fShaderFile,
                   const string& tcsShaderFile, const string& tesShaderFile,
                   const string& geomShaderFile,
                   string& msg);
   void Use(bool value);
   GLint GetUniformLocation(const char* uniformName);
   GLuint m_program;

private:
   bool CreateAndCompileShader(GLenum shader, const string& file, string& msg);
   bool ReadFile(const string& fileName, string& contents);
   bool CheckStatus(string& error);

private:
   GLuint m_vShader, m_fShader, m_tcsShader, m_tesShader, m_geomShader;
};


