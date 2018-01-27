#ifndef PROGRAM_CLASS_H
#define PROGRAM_CLASS_H

#include "Shader.h"
#include <map>
using namespace std;

namespace Utility
{
   typedef struct
   {
      int Id;
      string uniformName;
   } Uniform;

   class Program
   {
   public:
      // public member functions
      Program();
      ~Program();

      bool AttachShader(Shader& shader);
      bool DetachShader(Shader& shader);
      bool Link(bool initDefaultUniforms, string& errorMsg);

      void Use();
      void DoNotUse();

      // uniforms
      bool InitializeUniforms(vector<Uniform>& uniforms);

      bool SetUniform1i(unsigned int Id, int value);
      bool SetUniform1f(unsigned int Id, float value);
      bool SetUniform3fv(unsigned int Id, GLfloat* value);
      bool SetUniform4fv(unsigned int Id, GLfloat* value);
      bool SetUniformMatrix4fv(unsigned int Id, GLfloat* value);

      void SetLightLocation(GLfloat* value);
      void SetCameraLocation(GLfloat* value);

      void SetProjectionMatrix(GLfloat* value);
      void SetModelMatrix(GLfloat* value);
      void SetViewMatrix(GLfloat* value);

   private:
      // private member functions here
      void InitDefaultUniforms();
      GLint GetUniformLocation(unsigned int Id);

   private:
      // private member variables
      bool m_isLinked;
      GLuint m_ProgramId;
      vector<GLuint> m_ShaderIds;
      map<int, GLint> m_mapLocations;
      bool m_defaultUniforms;

      enum
      {
         _uniformModelMatrix_ = 1000,
         _uniformViewMatrix_,
         _uniformProjectionMatrix_,
         _uniformLightLocationWorld_,
         _uniformCameraLocationWorld_
      };

   };
}

#endif
