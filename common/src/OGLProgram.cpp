#include "OGLProgram.h"
#include <fstream>
#include <sstream>

using namespace std;

//--------------------------------------------------------------------------------------
OGLProgram::OGLProgram()
{
   m_program = m_vShader = m_fShader = m_tcsShader = m_tesShader = m_geomShader = 0;
}

//--------------------------------------------------------------------------------------
OGLProgram::~OGLProgram()
{
   glUseProgram(0);

   glDetachShader(m_program, m_vShader);
   glDetachShader(m_program, m_fShader);
   glDetachShader(m_program, m_tcsShader);
   glDetachShader(m_program, m_tesShader);
   glDetachShader(m_program, m_geomShader);

   glDeleteShader(m_vShader);
   glDeleteShader(m_fShader);
   glDeleteShader(m_tcsShader);
   glDeleteShader(m_tesShader);
   glDeleteShader(m_geomShader);

   glDeleteProgram(m_program);
}

//--------------------------------------------------------------------------------------
bool OGLProgram::Initialize(const string& vShaderFile, const string& fShaderFile,
                               const string& tcsShaderFile, const string& tesShaderFile,
                               const string& geomShaderFile,
                               string& msg)
{
   msg = "";

   CreateAndCompileShader(GL_VERTEX_SHADER, vShaderFile, msg);
   CreateAndCompileShader(GL_FRAGMENT_SHADER, fShaderFile, msg);

   // program
   m_program = glCreateProgram();
   glAttachShader(m_program, m_vShader);
   glAttachShader(m_program, m_fShader);

   if(tcsShaderFile != "") {
      CreateAndCompileShader(GL_TESS_CONTROL_SHADER, tcsShaderFile, msg);
      glAttachShader(m_program, m_tcsShader);
   }

   if(tesShaderFile != "") {
      CreateAndCompileShader(GL_TESS_EVALUATION_SHADER, tesShaderFile, msg);
      glAttachShader(m_program, m_tesShader);
   }

   if(geomShaderFile != "") {
      CreateAndCompileShader(GL_GEOMETRY_SHADER, geomShaderFile, msg);
      glAttachShader(m_program, m_geomShader);
   }

   glLinkProgram(m_program);

   CheckStatus(msg);

   return (msg == "");
}


//--------------------------------------------------------------------------------------
void OGLProgram::Use(bool value)
{
   value ? glUseProgram(m_program) : glUseProgram(0);
}

//--------------------------------------------------------------------------------------
GLint OGLProgram::GetUniformLocation(const char* uniformName)
{
   Use(true);

   if(uniformName && strlen(uniformName) > 0)
      return glGetUniformLocation(m_program, uniformName);
   else
      return -1;
}

//--------------------------------------------------------------------------------------
bool OGLProgram::CreateAndCompileShader(GLenum shader, const string& file, string& msg)
{
   string contents;
   const char* str;

   // read the file
   if(!ReadFile(file, contents)) {
      msg += "Unable to read the shader file\n";
      return false;
   }

   // create the shader object and set source
   switch(shader) {
   case GL_VERTEX_SHADER:
      str = contents.data();
      m_vShader = glCreateShader(GL_VERTEX_SHADER);
      glShaderSource(m_vShader, 1, &str, 0);
      glCompileShader(m_vShader);
      break;

   case GL_FRAGMENT_SHADER:
      str = contents.data();
      m_fShader = glCreateShader(GL_FRAGMENT_SHADER);
      glShaderSource(m_fShader, 1, &str, 0);
      glCompileShader(m_fShader);
      break;

   case GL_TESS_CONTROL_SHADER:
      str = contents.data();
      m_tcsShader = glCreateShader(GL_TESS_CONTROL_SHADER);
      glShaderSource(m_tcsShader, 1, &str, 0);
      glCompileShader(m_tcsShader);
      break;

   case GL_TESS_EVALUATION_SHADER:
      str = contents.data();
      m_tesShader = glCreateShader(GL_TESS_EVALUATION_SHADER);
      glShaderSource(m_tesShader, 1, &str, 0);
      glCompileShader(m_tesShader);
      break;

   case GL_GEOMETRY_SHADER:
      str = contents.data();
      m_geomShader = glCreateShader(GL_GEOMETRY_SHADER);
      glShaderSource(m_geomShader, 1, &str, 0);
      glCompileShader(m_geomShader);
      break;

   default:
      return false;
   }

   return true;
}

//--------------------------------------------------------------------------------------
bool OGLProgram::ReadFile(const string& fileName, string& contents)
{
   ifstream file;

   contents = "";
   file.open(fileName.c_str(), ios::in);
   if(file.is_open()) {
      stringstream ss;

      ss.clear();
      ss << file.rdbuf();

      contents = ss.str();
   }
   else
      return false;

   file.close();

   return true;
}

//--------------------------------------------------------------------------------------
bool OGLProgram::CheckStatus(string& msg)
{
   // vertex shader status
   GLint status, length;
   char err[4096];
   bool result = true;

   glGetShaderiv(m_vShader, GL_COMPILE_STATUS, &status);
   if(status != GL_TRUE) {
      msg += "VERTEX SHADER LOG\n";
      msg += "------------------------\n";
      glGetShaderInfoLog(m_vShader, 4096, &length, err);
      if(length > 0)
         msg += err;

      result = false;
   }

   glGetShaderiv(m_fShader, GL_COMPILE_STATUS, &status);
   if(status != GL_TRUE) {
      msg += "\nFRAGMENT SHADER LOG\n";
      msg += "----------------------------\n";
      glGetShaderInfoLog(m_fShader, 4096, &length, err);
      if(length > 0)
         msg += err;

      result = false;
   }

   if(m_tcsShader != 0) {
      glGetShaderiv(m_tcsShader, GL_COMPILE_STATUS, &status);
      if(status != GL_TRUE) {
         msg += "\nTESS CONTROL SHADER LOG\n";
         msg += "--------------------------------\n";
         glGetShaderInfoLog(m_tcsShader, 4096, &length, err);
         if(length > 0)
            msg += err;

         result = false;
      }
   }

   if(m_tesShader != 0) {
      glGetShaderiv(m_tesShader, GL_COMPILE_STATUS, &status);
      if(status != GL_TRUE) {
         msg += "\nTESS EVAL SHADER LOG\n";
         msg += "--------------------------\n";
         glGetShaderInfoLog(m_tesShader, 4096, &length, err);
         if(length > 0)
            msg += err;

         result = false;
      }
   }

   if(m_geomShader != 0) {
      glGetShaderiv(m_geomShader, GL_COMPILE_STATUS, &status);
      if(status != GL_TRUE) {
         msg += "\nGEOMETRY SHADER LOG\n";
         msg += "---------------------------\n";
         glGetShaderInfoLog(m_geomShader, 4096, &length, err);
         if(length > 0)
            msg += err;

         result = false;
      }
   }

   glGetProgramiv(m_program, GL_LINK_STATUS, &status);
   if(status != GL_TRUE) {
      msg += "\nPROGRAM LOG\n";
      msg += "------------------\n";
      glGetProgramInfoLog(m_program, 4096, &length, err);
      msg += err;

      result = false;
   }


   return result;
}

