#include <Windows.h>
#include "Loader.h"
#include <SOIL.h>
#include <glm.hpp>
using glm::vec4;

#include <iostream>
#include <fstream>
#include <string>

// COURTSEY: ERIC LENGYEL: Mathematics for game programming
//----------------------------------------------------------------------------------------------------------------------------------
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
void Loader::ProcessObject(MeshData& model,
                           vector<vec3>& tmpVertices, vector<vec3>& tmpNormals, vector<vec3>& tmpTexCoords,
                           vector<int>& vertexIndices, vector<int>& normalIndices, vector<int>& textureIndices,
                           bool calculateTangents)
{

   unsigned int index;
   model.vertices.reserve(vertexIndices.size());
   model.normals.reserve(vertexIndices.size());
   model.texcoords.reserve(vertexIndices.size());

   for(long i = 0; i < vertexIndices.size(); i++) {
      index = abs(vertexIndices[i]);
      if(index == 0)
         index = 1;
      model.vertices.push_back(tmpVertices[index - 1]);

      index = abs(normalIndices[i]);
      if(index == 0)
         index = 1;
      model.normals.push_back(tmpNormals[index - 1]);

      index = abs(textureIndices[i]);
      if(index == 0)
         index = 1;
      model.texcoords.push_back(tmpTexCoords[index - 1]);
   }

   // calculate the tangents for the TBN matrix if so specified
   if(calculateTangents) {
      int tangentCount = (int) model.vertices.size();
      vector<Triangle> triangles;
      triangles.reserve(tangentCount / 3);

      Triangle triangle;
      for(int i = 0; i < model.vertices.size(); i += 3) {
         triangle.index[0] = i;
         triangle.index[1] = i + 1;
         triangle.index[2] = i + 2;

         triangles.push_back(triangle);
      }

      model.tangents.reserve(tangentCount);
      CalculateTangentArray(tangentCount, model.vertices, model.normals, model.texcoords, triangles, model.tangents);
   }
}

//----------------------------------------------------------------------------------------------------------------------------------
void Loader::parseSingleValue(const string& line, float& f)
{
   static size_t index;
   static char* end;

   index = line.find_first_of(" ");

   f = strtof(line.substr(index + 1).c_str(), &end);
}

//----------------------------------------------------------------------------------------------------------------------------------
void Loader::parseSingleValue(const string& line, string& s)
{
   static size_t index;
   static char* end;

   index = line.find_first_of(" ");

   s = line.substr(index + 1).c_str();
}

//----------------------------------------------------------------------------------------------------------------------------------
vec3 Loader::parseTripleValue(const string& line)
{
   static vec3 value;
   static char* delim = " ", str[2048] = "", *end, *next_token;

   strcpy_s(str, 2048, line.c_str());
   char* token = strtok_s(str, delim, &next_token);

   int count = 0;
   while(token != NULL) {
      token = strtok_s(NULL, delim, &next_token);
      if(token)
         value[count++] = strtof(token, &end);
   }

   return value;
}

//---------------------------------------------------------------------------------------------------
int Loader::GetFaceCount(const string& line, string& format)
{
   int count = 0;
   size_t offset = line.find_first_of('/');

   while(offset != -1) {
      offset = line.find('/', offset + 1);

      //// sometimes obj files contain a space at the end just before the line ending
      //if(offset != -1 && offset != line.length())
      count++;
   }

   // we assume all 3: vertices, texcoords and normals are specified in the obj file
   count /= 2;

   format = "";
   for(int i = 0; i < count - 1; i++) {
      format += " %d/%d/%d";
   }

   if(count > 0)
      format += " %d/%d/%d\n";

   return count;
}

//---------------------------------------------------------------------------------------------------
bool Loader::LoadMesh(const char* fileName, MeshData& meshData, bool calculateTangents)
{
   meshData.Clear();

   static char str[2048];
   static string format;

   int iRetVal;
   vector<vec3> tmpVertices, tmpNormals, tmpTexCoords;
   vector<int> vertexIndices, normalIndices, textureIndices;

   FILE* fp;
   fopen_s(&fp, fileName, "r");
   if(fp == nullptr) {
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
         static unsigned int vIndex[64], nIndex[64], tIndex[64];

         fgets(str, 1024, fp);
         static int index = 0, count, matches;

         count = GetFaceCount(str, format);
         matches = sscanf_s(str, format.c_str(),
                            &vIndex[0], &tIndex[0], &nIndex[0],
                            &vIndex[1], &tIndex[1], &nIndex[1],
                            &vIndex[2], &tIndex[2], &nIndex[2],
                            &vIndex[3], &tIndex[3], &nIndex[3],
                            &vIndex[4], &tIndex[4], &nIndex[4],
                            &vIndex[5], &tIndex[5], &nIndex[5],
                            &vIndex[6], &tIndex[6], &nIndex[6],
                            &vIndex[7], &tIndex[7], &nIndex[7],
                            &vIndex[8], &tIndex[8], &nIndex[8],
                            &vIndex[9], &tIndex[9], &nIndex[9],
                            &vIndex[10], &tIndex[10], &nIndex[10],
                            &vIndex[11], &tIndex[11], &nIndex[11],
                            &vIndex[12], &tIndex[12], &nIndex[12],
                            &vIndex[13], &tIndex[13], &nIndex[13],
                            &vIndex[14], &tIndex[14], &nIndex[14],
                            &vIndex[15], &tIndex[15], &nIndex[15],
                            &vIndex[16], &tIndex[16], &nIndex[16],
                            &vIndex[17], &tIndex[17], &nIndex[17],
                            &vIndex[18], &tIndex[18], &nIndex[18],
                            &vIndex[19], &tIndex[19], &nIndex[19],
                            &vIndex[20], &tIndex[20], &nIndex[20],
                            &vIndex[21], &tIndex[21], &nIndex[21],
                            &vIndex[22], &tIndex[22], &nIndex[22],
                            &vIndex[23], &tIndex[23], &nIndex[23],
                            &vIndex[24], &tIndex[24], &nIndex[24],
                            &vIndex[25], &tIndex[25], &nIndex[25],
                            &vIndex[26], &tIndex[26], &nIndex[26],
                            &vIndex[27], &tIndex[27], &nIndex[27],
                            &vIndex[28], &tIndex[28], &nIndex[28],
                            &vIndex[29], &tIndex[29], &nIndex[29],
                            &vIndex[30], &tIndex[30], &nIndex[30],
                            &vIndex[31], &tIndex[31], &nIndex[31],
                            &vIndex[32], &tIndex[32], &nIndex[32],
                            &vIndex[33], &tIndex[33], &nIndex[33],
                            &vIndex[34], &tIndex[34], &nIndex[34],
                            &vIndex[35], &tIndex[35], &nIndex[35],
                            &vIndex[36], &tIndex[36], &nIndex[36],
                            &vIndex[37], &tIndex[37], &nIndex[37],
                            &vIndex[38], &tIndex[38], &nIndex[38],
                            &vIndex[39], &tIndex[39], &nIndex[39],
                            &vIndex[40], &tIndex[40], &nIndex[40],
                            &vIndex[41], &tIndex[41], &nIndex[41],
                            &vIndex[42], &tIndex[42], &nIndex[42],
                            &vIndex[43], &tIndex[43], &nIndex[43],
                            &vIndex[44], &tIndex[44], &nIndex[44],
                            &vIndex[45], &tIndex[45], &nIndex[45],
                            &vIndex[46], &tIndex[46], &nIndex[46],
                            &vIndex[47], &tIndex[47], &nIndex[47],
                            &vIndex[48], &tIndex[48], &nIndex[48],
                            &vIndex[49], &tIndex[49], &nIndex[49],
                            &vIndex[50], &tIndex[50], &nIndex[50],
                            &vIndex[51], &tIndex[51], &nIndex[51],
                            &vIndex[52], &tIndex[52], &nIndex[52],
                            &vIndex[53], &tIndex[53], &nIndex[53],
                            &vIndex[54], &tIndex[54], &nIndex[54],
                            &vIndex[55], &tIndex[55], &nIndex[55],
                            &vIndex[56], &tIndex[56], &nIndex[56],
                            &vIndex[57], &tIndex[57], &nIndex[57],
                            &vIndex[58], &tIndex[58], &nIndex[58],
                            &vIndex[59], &tIndex[59], &nIndex[59],
                            &vIndex[60], &tIndex[60], &nIndex[60],
                            &vIndex[61], &tIndex[61], &nIndex[61],
                            &vIndex[62], &tIndex[62], &nIndex[62],
                            &vIndex[63], &tIndex[63], &nIndex[63]);

         //assert((count * 3) == matches);
         assert(count < 65);

         // for polygons we do simple triangulation by joining each
         // progressive vertex to the first one
         if(count > 3) {
            // this is a polygon
            for(int id = 2; id < count; id++) {
               vertexIndices.push_back(vIndex[0]);
               vertexIndices.push_back(vIndex[id - 1]);
               vertexIndices.push_back(vIndex[id]);

               textureIndices.push_back(tIndex[0]);
               textureIndices.push_back(tIndex[id - 1]);
               textureIndices.push_back(tIndex[id]);

               normalIndices.push_back(nIndex[0]);
               normalIndices.push_back(nIndex[id - 1]);
               normalIndices.push_back(nIndex[id]);
            }
         }
         else if(count == 3) {
            // this is a triangle
            for(index = 0; index < count; index++) {
               vertexIndices.push_back(vIndex[index]);
               textureIndices.push_back(tIndex[index]);
               normalIndices.push_back(nIndex[index]);
            }
         }
         else {
            // what do we do with these potential lines and points?
            //fclose(fp);

            //return false;
         }
      }
      else {
         char* temp = fgets(str, 1024, fp);
         if(temp == nullptr) {
            fclose(fp);

            return false;
         }
      }
   }


   unsigned int index;
   meshData.vertices.reserve(vertexIndices.size());
   meshData.normals.reserve(vertexIndices.size());
   meshData.texcoords.reserve(vertexIndices.size());

   for(long i = 0; i < vertexIndices.size(); i++) {
      index = abs(vertexIndices[i]);
      if(index == 0)
         index = 1;
      meshData.vertices.push_back(tmpVertices[index - 1]);

      index = abs(normalIndices[i]);
      if(index == 0)
         index = 1;
      meshData.normals.push_back(tmpNormals[index - 1]);

      index = abs(textureIndices[i]);
      if(index == 0)
         index = 1;
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

//----------------------------------------------------------------------------------------------------------------------------------
bool Loader::LoadMesh(const char* objFile, const char* matFile, vector<MeshData>& meshDatas, bool calculateTangents)
{
   bool hasMaterials = false;
   vector<Material> materials;
   int currentMaterialIndex = -1;

   if((matFile && strlen(matFile) > 0)) {
      // process the materials, if provided
      hasMaterials = Loader::LoadMaterials(matFile, materials) && materials.size() > 0;
   }

   if(!(objFile && strlen(objFile) > 0))
      return false;

   string line;
   ifstream file(objFile, ios::in);
   if(!file.is_open())
      return false;

   string format;
   vector<vec3> tmpVertices, tmpNormals, tmpTexCoords;
   vector<int> vertexIndices, normalIndices, textureIndices;
   vector<float> values;
   MeshData model;
   bool objectDefn = false;

   while(getline(file, line)) {
      if(line.find("usemtl ") != string::npos && hasMaterials) {
         static string material;
         static int index;
         parseSingleValue(line, material);

         for(unsigned int i = 0; i < materials.size(); i++) {
            if(materials[i].name == material) {
               currentMaterialIndex = i;
               break;
            }
         }
      }
      else if(line.find("v ") != string::npos) {
         if(objectDefn) {
            ProcessObject(model, tmpVertices, tmpNormals, tmpTexCoords, vertexIndices, normalIndices, textureIndices, calculateTangents);
            if(currentMaterialIndex != -1 && hasMaterials) {
               model.mat = materials[currentMaterialIndex];
               currentMaterialIndex = -1;
            }
            meshDatas.push_back(model);

            // do NOT clear vertices, texcoords and normals, for they need to be accrued
            model.Clear();
            vertexIndices.clear();
            normalIndices.clear();
            textureIndices.clear();

            objectDefn = false;
         }

         // have to parse, nevertheless, since we are at atV
         tmpVertices.push_back(parseTripleValue(line));
      }
      else if(line.find("vn ") != string::npos) {
         tmpNormals.push_back(parseTripleValue(line));
      }
      else if(line.find("vt ") != string::npos) {
         tmpTexCoords.push_back(parseTripleValue(line));
      }
      else if(line.find("f ") != string::npos) {
         static unsigned int vIndex[64], nIndex[81], tIndex[81];
         static int index = 0, count, matches;

         objectDefn = true;

         count = GetFaceCount(line, format);
         assert(count < 81);
         matches = sscanf_s(line.substr(1).c_str(), format.c_str(),
                            &vIndex[0], &tIndex[0], &nIndex[0],
                            &vIndex[1], &tIndex[1], &nIndex[1],
                            &vIndex[2], &tIndex[2], &nIndex[2],
                            &vIndex[3], &tIndex[3], &nIndex[3],
                            &vIndex[4], &tIndex[4], &nIndex[4],
                            &vIndex[5], &tIndex[5], &nIndex[5],
                            &vIndex[6], &tIndex[6], &nIndex[6],
                            &vIndex[7], &tIndex[7], &nIndex[7],
                            &vIndex[8], &tIndex[8], &nIndex[8],
                            &vIndex[9], &tIndex[9], &nIndex[9],
                            &vIndex[10], &tIndex[10], &nIndex[10],
                            &vIndex[11], &tIndex[11], &nIndex[11],
                            &vIndex[12], &tIndex[12], &nIndex[12],
                            &vIndex[13], &tIndex[13], &nIndex[13],
                            &vIndex[14], &tIndex[14], &nIndex[14],
                            &vIndex[15], &tIndex[15], &nIndex[15],
                            &vIndex[16], &tIndex[16], &nIndex[16],
                            &vIndex[17], &tIndex[17], &nIndex[17],
                            &vIndex[18], &tIndex[18], &nIndex[18],
                            &vIndex[19], &tIndex[19], &nIndex[19],
                            &vIndex[20], &tIndex[20], &nIndex[20],
                            &vIndex[21], &tIndex[21], &nIndex[21],
                            &vIndex[22], &tIndex[22], &nIndex[22],
                            &vIndex[23], &tIndex[23], &nIndex[23],
                            &vIndex[24], &tIndex[24], &nIndex[24],
                            &vIndex[25], &tIndex[25], &nIndex[25],
                            &vIndex[26], &tIndex[26], &nIndex[26],
                            &vIndex[27], &tIndex[27], &nIndex[27],
                            &vIndex[28], &tIndex[28], &nIndex[28],
                            &vIndex[29], &tIndex[29], &nIndex[29],
                            &vIndex[30], &tIndex[30], &nIndex[30],
                            &vIndex[31], &tIndex[31], &nIndex[31],
                            &vIndex[32], &tIndex[32], &nIndex[32],
                            &vIndex[33], &tIndex[33], &nIndex[33],
                            &vIndex[34], &tIndex[34], &nIndex[34],
                            &vIndex[35], &tIndex[35], &nIndex[35],
                            &vIndex[36], &tIndex[36], &nIndex[36],
                            &vIndex[37], &tIndex[37], &nIndex[37],
                            &vIndex[38], &tIndex[38], &nIndex[38],
                            &vIndex[39], &tIndex[39], &nIndex[39],
                            &vIndex[40], &tIndex[40], &nIndex[40],
                            &vIndex[41], &tIndex[41], &nIndex[41],
                            &vIndex[42], &tIndex[42], &nIndex[42],
                            &vIndex[43], &tIndex[43], &nIndex[43],
                            &vIndex[44], &tIndex[44], &nIndex[44],
                            &vIndex[45], &tIndex[45], &nIndex[45],
                            &vIndex[46], &tIndex[46], &nIndex[46],
                            &vIndex[47], &tIndex[47], &nIndex[47],
                            &vIndex[48], &tIndex[48], &nIndex[48],
                            &vIndex[49], &tIndex[49], &nIndex[49],
                            &vIndex[50], &tIndex[50], &nIndex[50],
                            &vIndex[51], &tIndex[51], &nIndex[51],
                            &vIndex[52], &tIndex[52], &nIndex[52],
                            &vIndex[53], &tIndex[53], &nIndex[53],
                            &vIndex[54], &tIndex[54], &nIndex[54],
                            &vIndex[55], &tIndex[55], &nIndex[55],
                            &vIndex[56], &tIndex[56], &nIndex[56],
                            &vIndex[57], &tIndex[57], &nIndex[57],
                            &vIndex[58], &tIndex[58], &nIndex[58],
                            &vIndex[59], &tIndex[59], &nIndex[59],
                            &vIndex[60], &tIndex[60], &nIndex[60],
                            &vIndex[61], &tIndex[61], &nIndex[61],
                            &vIndex[62], &tIndex[62], &nIndex[62],
                            &vIndex[63], &tIndex[63], &nIndex[63],
                            &vIndex[64], &tIndex[30], &nIndex[30],
                            &vIndex[65], &tIndex[31], &nIndex[31],
                            &vIndex[66], &tIndex[32], &nIndex[32],
                            &vIndex[67], &tIndex[33], &nIndex[33],
                            &vIndex[68], &tIndex[34], &nIndex[34],
                            &vIndex[69], &tIndex[35], &nIndex[35],
                            &vIndex[70], &tIndex[70], &nIndex[36],
                            &vIndex[71], &tIndex[71], &nIndex[71],
                            &vIndex[72], &tIndex[72], &nIndex[72],
                            &vIndex[73], &tIndex[73], &nIndex[73],
                            &vIndex[74], &tIndex[74], &nIndex[74],
                            &vIndex[75], &tIndex[75], &nIndex[75],
                            &vIndex[76], &tIndex[76], &nIndex[76],
                            &vIndex[77], &tIndex[77], &nIndex[77],
                            &vIndex[78], &tIndex[78], &nIndex[78],
                            &vIndex[79], &tIndex[79], &nIndex[79],
                            &vIndex[80], &tIndex[80], &nIndex[80]);

         //assert((count * 3) == matches);

         // for polygons we do simple triangulation by joining each
         // progressive vertex to the first one
         if(count > 3) {
            // this is a polygon
            for(int id = 2; id < count; id++) {
               vertexIndices.push_back(vIndex[0]);
               vertexIndices.push_back(vIndex[id - 1]);
               vertexIndices.push_back(vIndex[id]);

               textureIndices.push_back(tIndex[0]);
               textureIndices.push_back(tIndex[id - 1]);
               textureIndices.push_back(tIndex[id]);

               normalIndices.push_back(nIndex[0]);
               normalIndices.push_back(nIndex[id - 1]);
               normalIndices.push_back(nIndex[id]);
            }
         }
         else if(count == 3) {
            // this is a triangle
            for(index = 0; index < count; index++) {
               vertexIndices.push_back(vIndex[index]);
               textureIndices.push_back(tIndex[index]);
               normalIndices.push_back(nIndex[index]);
            }
         }
         else {
            // what do we do with these potential lines and points?
            //fclose(fp);

            //return false;
         }

      }
   }

   file.close();

   // the last remaining object to be processed
   if(vertexIndices.size() > 0) {
      ProcessObject(model, tmpVertices, tmpNormals, tmpTexCoords, vertexIndices, normalIndices, textureIndices, calculateTangents);
      if(currentMaterialIndex != -1 && hasMaterials) {
         model.mat = materials[currentMaterialIndex];
         currentMaterialIndex = -1;
      }
      meshDatas.push_back(model);
   }

   return true;
}


//----------------------------------------------------------------------------------------------------------------------------------
bool Loader::LoadMaterials(const char* fileName, vector<Material>& materials)
{
   if(!(fileName && strlen(fileName) > 0))
      return false;

   string line;
   ifstream file(fileName, ios::in);
   if(!file.is_open())
      return false;

   Material m;
   while(getline(file, line)) {
      // material name
      if(line.find("newmtl") != string::npos) {
         if(m.name != "") {
            materials.push_back(m);
            m.Reset();
         }

         parseSingleValue(line, m.name);
      }
      else if(line.find("map_Ka") != string::npos) {
         parseSingleValue(line, m.map_ka);
      }
      else if(line.find("map_Kd") != string::npos) {
         parseSingleValue(line, m.map_kd);
      }
      else if(line.find("map_bump") != string::npos) {
         parseSingleValue(line, m.map_normal);
      }
      else if(line.find("map_d") != string::npos) {
         parseSingleValue(line, m.map_mask);
      }
      else if(line.find("Ka") != string::npos) {
         m.ka = parseTripleValue(line);
      }
      else if(line.find("Kd") != string::npos) {
         m.kd = parseTripleValue(line);
      }
      else if(line.find("Ks") != string::npos) {
         m.ks = parseTripleValue(line);
      }
      else if(line.find("Ke") != string::npos) {
         m.ke = parseTripleValue(line);
      }
      else if(line.find("Ns") != string::npos) {
         parseSingleValue(line, m.Ns);
      }
   }

   file.close();

   // push the last remaining one, if any
   if(m.name != "") {
      materials.push_back(m);
      m.Reset();
   }

   return true;
}

//-----------------------------------------------------------------------------------------------------------
GLenum Loader::GetColorFormat(int nChannels)
{
   switch(nChannels) {
   case 1:
      return GL_ALPHA;

   case 2:
      return GL_RG;

   case 3:
      return GL_RGB;

   case 4:
      return GL_RGBA;

   default:
      return GL_RGB;
   }
}

//-----------------------------------------------------------------------------------------------------------
bool Loader::Load2DTexture(const char* fileName, GLuint& textureId, GLenum activeTexture, bool enableMipMaps)
{
   int w, h, nChannels;

   GLvoid* imageData = SOIL_load_image(fileName, &w, &h, &nChannels, SOIL_LOAD_AUTO);
   if(!imageData)
      return false;

   glEnable(GL_TEXTURE_2D);

   glActiveTexture(activeTexture);

   glGenTextures(1, &textureId);
   glBindTexture(GL_TEXTURE_2D, textureId);
   glTexImage2D(GL_TEXTURE_2D, 0, GetColorFormat(nChannels),
                w, h, 0,
                GetColorFormat(nChannels),
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

//-------------------------------------------------------------------------------------------------------------------
bool Loader::LoadCubeMapTexture(vector<string>& fileNames, GLuint& textureId, GLenum activeTexture, bool enableMipMaps)
{
   if(fileNames.size() != 6)
      return false;

   glEnable(GL_TEXTURE_CUBE_MAP);
   glActiveTexture(activeTexture);

   glGenTextures(1, &textureId);
   glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);

   //textureId = SOIL_load_OGL_cubemap
   //(
   //   fileNames[0].c_str(),
   //   fileNames[1].c_str(),
   //   fileNames[2].c_str(),
   //   fileNames[3].c_str(),
   //   fileNames[4].c_str(),
   //   fileNames[5].c_str(),
   //   SOIL_LOAD_RGB,
   //   SOIL_CREATE_NEW_ID,
   //   SOIL_FLAG_MIPMAPS
   //);

   int w, h, channels;
   for(int i = 0; i < 6; i++) {
      GLvoid* imageData = SOIL_load_image(fileNames[i].c_str(), &w, &h, &channels, SOIL_LOAD_RGB);
      if(!imageData)
         return false;
      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, (channels == 4) ? GL_RGBA : GL_RGB,
                   w, h, 0,
                   (channels == 4) ? GL_RGBA : GL_RGB,
                   GL_UNSIGNED_BYTE, imageData);

      if(enableMipMaps) {
         glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
         glTextureParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_LOD_BIAS, -1.0f);
      }

      SOIL_free_image_data((unsigned char*) imageData);
   }

   glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, enableMipMaps ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
   glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, enableMipMaps ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);

   glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

   return true;
}

//-----------------------------------------------------------------------------------
bool Loader::LoadBlank2DColorTexture(GLuint &textureId, GLenum activeTexture, int w, int h)
{
   textureId = 0;

   glEnable(GL_TEXTURE_2D);
   glActiveTexture(activeTexture);

   glGenTextures(1, &textureId);
   glBindTexture(GL_TEXTURE_2D, textureId);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                w, h, 0,
                GL_RGB,
                GL_UNSIGNED_BYTE, nullptr);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

   return (textureId != 0);
}

//-------------------------------------------------------------------------------------------
bool Loader::LoadBlank2DDepthTexture(GLuint& textureId, GLenum activeTexture, int w, int h)
{
   textureId = 0;

   glEnable(GL_TEXTURE_2D);
   glActiveTexture(activeTexture);

   glGenTextures(1, &textureId);
   glBindTexture(GL_TEXTURE_2D, textureId);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
                w, h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

   return (textureId != 0);
}

//--------------------------------------------------------------------------------------------
bool Loader::LoadBlankCubeMapTexture(GLuint& textureId, GLenum activeTexture, int size)
{
   textureId = 0;

   glEnable(GL_TEXTURE_CUBE_MAP);
   glActiveTexture(activeTexture);

   glGenTextures(1, &textureId);
   glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);

   // width and height must be identical for FBO cubemap rendering
   // https://www.khronos.org/opengl/wiki/Cubemap_Texture#Samplers
   for(int i = 0; i < 6; i++) {
      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB,
                   size, size, 0,
                   GL_RGB,
                   GL_UNSIGNED_BYTE, NULL);
   }

   glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

   glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

   return (textureId != 0);
}
