#include <VSIUtilPlus.h>
#include <stdio.h>

namespace VSIUtil
{
	void VSIUtilPlus::VSIUtilLoadMesh(char* filePath,
		MeshType meshType,
		SeparatorType separator)
	{
		std::vector<unsigned int> vertexIndices;
		std::vector<unsigned int> normalIndices;
		std::vector<unsigned int> textureIndices;
		std::vector<glm::vec3> tempVertices;
		std::vector<glm::vec3> tempNormals;
		std::vector<glm::vec2> tempTextures;

		int iRetVal;
		FILE* filePtr;
		iRetVal = fopen_s(&filePtr, filePath, "r");
		if (filePtr == NULL)
		{
			MessageBoxA(NULL, "Failed to open the object file", "FILE_OPEN_ERROR", MB_OK);
		}

		while (1)
		{
			char c;
			c = fgetc(filePtr);
			if (c == EOF)
			{
				break;
			}

			if (c == 'v')
			{
				c = fgetc(filePtr);
				if (c == 'n')
				{
					glm::vec3 tempNormal;
					iRetVal = fscanf_s(filePtr, " %f %f %f\n", &tempNormal.x, &tempNormal.y, &tempNormal.z);
					tempNormals.push_back(tempNormal);
				}
				else if (c == 't')
				{
					glm::vec2 tempTexture;
					iRetVal = fscanf_s(filePtr, " %f %f\n", &tempTexture.x, &tempTexture.y);
					tempTextures.push_back(tempTexture); 
				}
				else
				{
					glm::vec3 tempVertex;
					iRetVal = fscanf_s(filePtr, "%f %f %f\n", &tempVertex.x, &tempVertex.y, &tempVertex.z);
					tempVertices.push_back(tempVertex);
				}
			}
			else if (c == 'f')
			{
				unsigned int vIndex[3], tIndex[3], nIndex[3];
				int matches;
				switch (separator)
				{
				case SINGLE:
					switch (meshType)
					{
					case ONLY_VERTEX:
						matches = fscanf_s(filePtr, " %d %d %d\n",
							&vIndex[0],
							&vIndex[1],
							&vIndex[2]);
						if (matches != 3)
						{
							MessageBoxA(NULL, "Failed to read indices", "INDEX_READ_ERROR", MB_OK);
							return;
						}
						break;

					case VERTEX_AND_NORMAL:
						matches = fscanf_s(filePtr, " %d/%d %d/%d %d/%d\n",
							&vIndex[0], &nIndex[0],
							&vIndex[1], &nIndex[1],
							&vIndex[2], &nIndex[2]);
						if (matches != 6)
						{
							MessageBoxA(NULL, "Failed to read indices", "INDEX_READ_ERROR", MB_OK);
							return;
						}

						break;

					case VERTEX_AND_TEXTURE:
						matches = fscanf_s(filePtr, " %d/%d %d/%d %d/%d\n",
							&vIndex[0], &tIndex[0],
							&vIndex[1], &tIndex[1],
							&vIndex[2], &tIndex[2]);
						if (matches != 6)
						{
							MessageBoxA(NULL, "Failed to read indices", "INDEX_READ_ERROR", MB_OK);
							return;
						}
						break;

					case VERTEX_NORMAL_AND_TEXTURE:
						matches = fscanf_s(filePtr, " %d/%d/%d %d/%d/%d %d/%d/%d\n",
							&vIndex[0], &tIndex[0], &nIndex[0],
							&vIndex[1], &tIndex[1], &nIndex[1],
							&vIndex[2], &tIndex[2], &nIndex[2]) ;
						if (matches != 9)
						{
							MessageBoxA(NULL, "Failed to read indices", "INDEX_READ_ERROR", MB_OK);
							return;
						}
						break;
					}
					break;
				case DOUBLE:
					switch (meshType)
					{
					case ONLY_VERTEX:
						matches = fscanf_s(filePtr, " %d %d %d\n",
							&vIndex[0],
							&vIndex[1],
							&vIndex[2]);
						if (matches != 3)
						{
							MessageBoxA(NULL, "Failed to read indices", "INDEX_READ_ERROR", MB_OK);
							return;
						}
						break;

					case VERTEX_AND_NORMAL:
						matches = fscanf_s(filePtr, " %d//%d %d//%d %d//%d\n",
							&vIndex[0], &nIndex[0],
							&vIndex[1], &nIndex[1],
							&vIndex[2], &nIndex[2]);
						if (matches != 6)
						{
							MessageBoxA(NULL, "Failed to read indices", "INDEX_READ_ERROR", MB_OK);
							return;
						}

						break;

					case VERTEX_AND_TEXTURE:
						matches = fscanf_s(filePtr, " %d//%d %d//%d %d//%d\n",
							&vIndex[0], &tIndex[0],
							&vIndex[1], &tIndex[1],
							&vIndex[2], &tIndex[2]);
						if (matches != 6)
						{
							MessageBoxA(NULL, "Failed to read indices", "INDEX_READ_ERROR", MB_OK);
							return;
						}
						break;

					case VERTEX_NORMAL_AND_TEXTURE:
						matches = fscanf_s(filePtr, " %d//%d//%d %d//%d//%d %d//%d//%d\n",
							&vIndex[0], &tIndex[0], &nIndex[0],
							& vIndex[1], &tIndex[1], &nIndex[1],
							& vIndex[2], &tIndex[2], &nIndex[2]);
						if (matches != 9)
						{
							MessageBoxA(NULL, "Failed to read indices", "INDEX_READ_ERROR", MB_OK);
							return;
						}
					}
					break;
				}
				
				switch (meshType)
				{
				case ONLY_VERTEX:
					vertexIndices.push_back(vIndex[0]);
					vertexIndices.push_back(vIndex[1]);
					vertexIndices.push_back(vIndex[2]);
					break;

				case VERTEX_AND_NORMAL:

					vertexIndices.push_back(vIndex[0]);
					vertexIndices.push_back(vIndex[1]);
					vertexIndices.push_back(vIndex[2]);
					
					normalIndices.push_back(nIndex[0]);
					normalIndices.push_back(nIndex[1]);
					normalIndices.push_back(nIndex[2]);
					break;
				case VERTEX_AND_TEXTURE:
					vertexIndices.push_back(vIndex[0]);
					vertexIndices.push_back(vIndex[1]);
					vertexIndices.push_back(vIndex[2]);

					textureIndices.push_back(tIndex[0]);
					textureIndices.push_back(tIndex[1]);
					textureIndices.push_back(tIndex[2]);
					break;

				case VERTEX_NORMAL_AND_TEXTURE:

					vertexIndices.push_back(vIndex[0]);
					vertexIndices.push_back(vIndex[1]);
					vertexIndices.push_back(vIndex[2]);

					normalIndices.push_back(nIndex[0]);
					normalIndices.push_back(nIndex[1]);
					normalIndices.push_back(nIndex[2]);

					textureIndices.push_back(tIndex[0]);
					textureIndices.push_back(tIndex[1]);
					textureIndices.push_back(tIndex[2]);
					break;
				}
			}
			else
			{
				char str[512];
				char* temp = fgets(str, 512, filePtr);
				if (temp == NULL)
				{
					MessageBoxA(NULL, "fgets() failed.", "FGETS_ERROR", MB_OK);
				}
			}
		}

		std::vector<glm::vec3>* vertices = VSIUtilGetVertices();
		std::vector<glm::vec3>* normals = VSIUtilGetNormals();
		std::vector<glm::vec2>* textureCoords = VSIUtilGetTexcoords();
		
		for (int i = 0; i < vertexIndices.size(); i++)
		{
			unsigned int vertexIndex = vertexIndices[i];
			(*vertices).push_back(tempVertices[vertexIndex - 1]);
			if (normalIndices.size() > 0)
			{
				unsigned int normalIndex = normalIndices[i];
				(*normals).push_back(tempNormals[normalIndex - 1]);
			}
			if (textureIndices.size() > 0)
			{
				unsigned int textureIndex = textureIndices[i];
				(*textureCoords).push_back(tempTextures[textureIndex - 1]);
			}
		}

	}
}