#ifndef FRAMEWORK_MESH_LOADER_CLASS_H
#define FRAMEWORK_MESH_LOADER_CLASS_H

#include <glew.h>
#include <gl/GL.h>
#include <vector>
using namespace std;

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
using glm::vec2;
using glm::vec3;
using glm::vec4;


typedef struct
{
   unsigned int index[3];
} Triangle;

typedef struct
{
   string name;
   vec3 ka, kd, ks, ke;
   float Ns;
   string map_ka, map_kd, map_normal, map_mask;

   void Reset()
   {
      ka = kd = ks = ke = {0.0f, 0.0f, 0.0f};
      map_ka = map_kd = map_normal = map_mask = "";
   }
} Material;

typedef struct
{
   vector<vec3> vertices;
   vector<vec3> texcoords;
   vector<vec3> normals;
   vector<vec4> tangents;
   Material mat;

   void Clear()
   {
      mat.Reset();

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
   static bool LoadMesh(const char* objFile, const char* matFile, vector<MeshData>& meshDatas, bool calculateTangents = false);
   static bool LoadMaterials(const char* fileName, vector<Material>& materials);

   static bool Load2DTexture(const char* fileName, GLuint& textureId, GLenum activeTexture = GL_TEXTURE0, bool enableMipMaps = false);
   static bool LoadCubeMapTexture(vector<string>& fileNames, GLuint& textureId, GLenum activeTexture = GL_TEXTURE0, bool enableMipMaps = false);
   static bool LoadBlank2DColorTexture(GLuint &textureId, GLenum activeTexture, int w, int h);
   static bool LoadBlank2DDepthTexture(GLuint &textureId, GLenum activeTexture, int w, int h);
   static bool LoadBlankCubeMapTexture(GLuint &textureId, GLenum activeTexture, int size);
private:
   Loader() {}
   static void ProcessObject(MeshData& model,
                             vector<vec3>& tmpVertices, vector<vec3>&tmpNormals, vector<vec3>&tmpTexCoords,
                             vector<int>& vertexIndices, vector<int>&normalIndices, vector<int>& textureIndices,
                             bool calculateTangents = false);
   static void parseSingleValue(const string& line, float& f);
   static void parseSingleValue(const string& line, string& s);
   static vec3 parseTripleValue(const string& line);
   static GLenum GetColorFormat(int nChannels);
   static int GetFaceCount(const string& line, string& format);
   static void CalculateTangentArray(long vertexCount, const vector<vec3>& vertices, const vector<vec3>& normals,
                                     const vector<vec3>& texcoords, const vector<Triangle>& triangles, vector<vec4>& tangent);
};


#endif
