#ifndef MESH_LOADER_H
#define MESH_LOADER_H
#include <glew.h>
#include <vector>
using namespace std;

#include <glm.hpp>
#include <gtx/transform.hpp>
using glm::vec2;
using glm::vec3;
using glm::vec4;

namespace Utility
{
   typedef struct
   {
      unsigned int index[3];
   } Triangle;

   typedef struct
   {
      vector<vec3> vertices;
      vector<vec3> texcoords;
      vector<vec3> normals;
      vector<vec4> tangents;

      void Clear()
      {
         vertices.clear();
         texcoords.clear();
         normals.clear();
         tangents.clear();
      }
   } MeshData;

   class Loader
   {
   public:
      static bool LoadMesh(const char* fileName, MeshData& meshData, bool calculateTangents = false);
      static bool LoadTexture(const char* fileName, GLuint& textureId, GLenum activeTexture = GL_TEXTURE0, bool enableMipMaps = false);
      static bool LoadBlankTexture(GLuint &textureId, GLenum activeTexture, int w, int h);

   private:
      Loader() {}
      static void CalculateTangentArray(long vertexCount, const vector<vec3>& vertices, const vector<vec3>& normals,
                                        const vector<vec3>& texcoords, const vector<Triangle>& triangles, vector<vec4>& tangent);
   };
}

#endif
