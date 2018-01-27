#ifndef MODEL3D_HEADER_H
#define MODEL3D_HEADER_H

#include <glew.h>

#include <vector>
using namespace std;

#include <glm.hpp>
#include <gtx/transform.hpp>
using glm::vec3;
using glm::vec4;
using glm::mat4;

namespace Utility
{
   class Model3D
   {
   public:
      // public member functions

      Model3D();
      virtual ~Model3D();

      void Translate(vec3& translations);
      void Rotate(vec3& rotations);
      void ResetPosition();
      mat4 GetModelMatrix();

      virtual void Render(GLenum mode);

   private:
      // private member functions
      void BindVAO();
      void UnBindVAO();

   private:
      typedef enum
      {
         vertexAttributePosition = 0,
         vertexAttributeColor,
         vertexAttributeNormal,
         vertexAttributeTexture,
         vertexAttributeTangent,

         vertexAttributeLast // any future vertex attributes MUST be above this one
      } VertexAttributes;

      // private member variables
      GLuint m_BufferIds[vertexAttributeLast];
      GLuint m_vaoId;
      GLsizei m_vertexCount;
      mat4 modelMatrix;

   protected:
      bool CreateVertexBuffer(vector<vec3>& data);
      bool CreateColorBuffer(vector<vec3>& data);
      bool CreateNormalBuffer(vector<vec3>& data);
      bool CreateTextureBuffer(vector<vec3>& data);
      bool CreateTangentBuffer(vector<vec4>& data);

      bool CreateVAO();

      // protected member functions
      virtual bool LoadData() = 0;
   };
}

#endif
