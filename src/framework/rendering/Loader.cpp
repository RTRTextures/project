#include <Windows.h>
#include "Loader.h"
#include <SOIL.h>
#include <glm.hpp>
using glm::vec4;

using namespace Utility;

// COURTSEY: ERIC LENGYEL
/*----------------------------------------------------------------------------------------------------------*/
void Loader::CalculateTangentArray(long vertexCount, const vector<vec3>& vertices, const vector<vec3>& normals,
                                   const vector<vec3>& texcoords, const vector<Triangle>& triangle, vector<vec4>& tangent)
{
   vec3 *tan1 = new vec3[vertexCount * 2];
   vec3 *tan2 = tan1 + vertexCount;
   ZeroMemory(tan1, vertexCount * sizeof(vec3) * 2);

   for(long a = 0; a < triangle.size(); a++) {
      long i1 = triangle[a].index[0];
      long i2 = triangle[a].index[1];
      long i3 = triangle[a].index[2];

      const vec3& v1 = vertices[i1];
      const vec3& v2 = vertices[i2];
      const vec3& v3 = vertices[i3];

      const vec2& w1 = vec2(texcoords[i1]);
      const vec2& w2 = vec2(texcoords[i2]);
      const vec2& w3 = vec2(texcoords[i3]);

      float x1 = v2.x - v1.x;
      float x2 = v3.x - v1.x;
      float y1 = v2.y - v1.y;
      float y2 = v3.y - v1.y;
      float z1 = v2.z - v1.z;
      float z2 = v3.z - v1.z;

      float s1 = w2.x - w1.x;
      float s2 = w3.x - w1.x;
      float t1 = w2.y - w1.y;
      float t2 = w3.y - w1.y;

      float r = 1.0F / (s1 * t2 - s2 * t1);
      vec3 sdir((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r, (t2 * z1 - t1 * z2) * r);
      vec3 tdir((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r, (s1 * z2 - s2 * z1) * r);

      tan1[i1] += sdir;
      tan1[i2] += sdir;
      tan1[i3] += sdir;

      tan2[i1] += tdir;
      tan2[i2] += tdir;
      tan2[i3] += tdir;
   }

   for(long a = 0; a < vertexCount; a++) {
      const vec3& n = normals[a];
      const vec3& t = tan1[a];

      // Gram-Schmidt orthogonalize
      tangent.push_back(vec4(glm::normalize(t - n * glm::dot(n, t)), 1.0f));

      // Calculate handedness
      tangent[a].w = (glm::dot(glm::cross(n, t), tan2[a]) < 0.0F) ? -1.0F : 1.0F;
   }

   delete[] tan1;
}

//---------------------------------------------------------------------------------------------------
bool Loader::LoadMesh(const char* filePath, MeshData& meshData, bool calculateTangents)
{
   meshData.Clear();

   int iRetVal;
   vector<vec3> tmpVertices, tmpNormals, tmpTexCoords;
   vector<int> vertexIndices, normalIndices, textureIndices;

   FILE* fp;
   fopen_s(&fp, filePath, "r");
   if(fp == NULL) {
      return false;
   }

   vec3 v, n, tc;

   char c;
   while(1) {
      c = fgetc(fp);
      if(c == EOF) {
         break;
      }
      else if(c == '\n')
         continue;

      if(c == 'v') {
         c = fgetc(fp);
         if(c == 'n') {
            iRetVal = fscanf_s(fp, " %f %f %f\n", &n.x, &n.y, &n.z);
            tmpNormals.push_back(n);
         }
         else if(c == 't') {
            iRetVal = fscanf_s(fp, " %f %f %f\n", &tc.x, &tc.y, &tc.z);
            tmpTexCoords.push_back(tc);
         }
         else {
            iRetVal = fscanf_s(fp, " %f %f %f\n", &v.x, &v.y, &v.z);
            tmpVertices.push_back(v);
         }
      }
      else if(c == 'f') {
         unsigned int vIndex[4], nIndex[4], tIndex[4];
         int matches = fscanf_s(fp, " %d/%d/%d %d/%d/%d %d/%d/%d\n",
                                &vIndex[0], &tIndex[0], &nIndex[0],
                                &vIndex[1], &tIndex[1], &nIndex[1],
                                &vIndex[2], &tIndex[2], &nIndex[2]);
         vertexIndices.push_back(vIndex[0]);
         vertexIndices.push_back(vIndex[1]);
         vertexIndices.push_back(vIndex[2]);

         textureIndices.push_back(tIndex[0]);
         textureIndices.push_back(tIndex[1]);
         textureIndices.push_back(tIndex[2]);

         normalIndices.push_back(nIndex[0]);
         normalIndices.push_back(nIndex[1]);
         normalIndices.push_back(nIndex[2]);
      }
      else {
         static char str[1024];
         char* temp = fgets(str, 512, fp);
         if(temp == NULL) {
            fclose(fp);

            return false;
         }
      }
   }


   unsigned int index;
   for(long i = 0; i < vertexIndices.size(); i++) {
      index = vertexIndices[i];
      meshData.vertices.push_back(tmpVertices[index - 1]);

      index = normalIndices[i];
      meshData.normals.push_back(tmpNormals[index - 1]);

      index = textureIndices[i];
      meshData.texcoords.push_back(tmpTexCoords[index - 1]);
   }

   fclose(fp);

   // calculate the tangents for the TBN matrix if so specified
   if(calculateTangents) {
      int tangentCount = (int) meshData.vertices.size();
      vector<Triangle> triangles;
      triangles.reserve(tangentCount / 3);

      Triangle triangle;
      for(int i = 0; i < meshData.vertices.size(); i += 3) {
         triangle.index[0] = i;
         triangle.index[1] = i + 1;
         triangle.index[2] = i + 2;

         triangles.push_back(triangle);
      }

      meshData.tangents.reserve(tangentCount);
      CalculateTangentArray(tangentCount, meshData.vertices, meshData.normals, meshData.texcoords, triangles, meshData.tangents);
   }

   return true;
}

//------------------------------------------------------------------------------------------------------
bool Loader::LoadTexture(const char* fileName, GLuint& textureId, GLenum activeTexture, bool enableMipMaps)
{
   int w, h;

   GLvoid* imageData = SOIL_load_image(fileName, &w, &h, 0, SOIL_LOAD_RGB);
   if(!imageData)
      return false;

   glEnable(GL_TEXTURE_2D);

   glActiveTexture(activeTexture);

   glGenTextures(1, &textureId);
   glBindTexture(GL_TEXTURE_2D, textureId);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                w, h, 0,
                GL_RGB,
                GL_UNSIGNED_BYTE, imageData);

   if(enableMipMaps) {
      glGenerateMipmap(GL_TEXTURE_2D);
      glTextureParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -1.0f);
   }

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, enableMipMaps ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, enableMipMaps ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

   SOIL_free_image_data((unsigned char*) imageData);

   return true;
}

//---------------------------------------------------------------------------------
bool Loader::LoadBlankTexture(GLuint &textureId, GLenum activeTexture, int w, int h)
{
   glEnable(GL_TEXTURE_2D);

   glActiveTexture(activeTexture);

   glGenTextures(1, &textureId);
   glBindTexture(GL_TEXTURE_2D, textureId);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                w, h, 0,
                GL_RGB,
                GL_UNSIGNED_BYTE, NULL);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

   return true;
}