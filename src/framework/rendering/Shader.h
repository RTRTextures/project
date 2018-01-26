#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include <glew.h>
#include <vector>

namespace Utility
{
   class Shader
   {
   public:
      // public member functions
      Shader(GLenum shaderType);
      ~Shader();

      bool SetSourceAndCompile(char* sourceFile, std::string& errorMsg);
      
      GLuint GetId() { return m_ShaderId; }
      bool IsCompiled() { return m_isCompiled; }

   private:
      // private member functions
      bool ReadFile(const char* fileName, std::string& contents);
      bool CheckCompilationErrors(std::string& errorMsg);

   private:
      // private member variables
      GLuint m_ShaderId;
      bool m_isCompiled;
   };

}

#endif
