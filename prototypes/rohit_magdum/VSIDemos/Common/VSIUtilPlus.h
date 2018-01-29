#pragma once

#include <Windows.h>
#include <glm\mat4x4.hpp>
#include <vector>
#include <gl\GL.h>
#include <gl\GLU.h>
#include <vector>

#ifdef VSIUTILPLUS_EXPORTS
#define VSIUTILPLUS_API __declspec(dllexport)
#else
#define VSIUTILPLUS_API __declspec(dllimport)
#endif

#define BUFFER_OFFSET(i) ((void*)(i))

#define MAX_NAME_LENGTH 2048

#define DEFAULT_WIDTH 1024
#define DEFAULT_HEIGHT 780
#define MAX_TEXTURES 100

namespace VSIUtil
{
	class VSIUtilPlus
	{
	public:
		HWND mhWnd;
		HINSTANCE mhInstance;
		HDC mhDC;
		HGLRC mhRC;
		bool mbDone;
		bool mbIsActiveWindow;
		bool mbIsEscapeKeyPressed;
		bool mbFullscreen;
		DWORD mdwStyle;
		WINDOWPLACEMENT mwpPrev;
		
		void Initialize();
		void Uninitialize();
		void ToggleFullscreen();
		GLubyte* loadShaderFromFile(char*);
		unsigned int getFileLength(std::ifstream&);
		int compileShader(GLubyte* source, GLenum shaderType, GLuint *shader);
		int linkShader(GLuint);
		
	public:
		GLuint mTexture[100];

		glm::mat4 mProjMatrix;

		enum BufferType
		{
			VERTEX = 0,
			NORMALS,
			TEXTURECOORDS
		};

		enum MeshType
		{
			UNKNOWN,
			ONLY_VERTEX,
			VERTEX_AND_NORMAL,
			VERTEX_AND_TEXTURE,
			VERTEX_NORMAL_AND_TEXTURE
		};

		enum SeparatorType
		{
			SINGLE,
			DOUBLE
		};
		static LRESULT CALLBACK mWndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

		wchar_t szAppName[512];

		void MessageHandler(UINT, WPARAM, LPARAM);

		VSIUTILPLUS_API VSIUtilPlus();
		VSIUTILPLUS_API ~VSIUtilPlus();
		VSIUTILPLUS_API virtual void WindowInit();
		VSIUTILPLUS_API virtual WPARAM VSIUtilRun(HINSTANCE hInstance,
			HINSTANCE hPrevInstance,
			LPSTR lpCmdLine,
			int nCmdShow);
		VSIUTILPLUS_API virtual void VSIUtilRender();
		VSIUTILPLUS_API virtual void VSIUtilUpdate();
		VSIUTILPLUS_API virtual void VSIUtilResize(unsigned long, unsigned long);
		VSIUTILPLUS_API virtual void VSIUtilSceneInit();
		VSIUTILPLUS_API virtual void VSIUtilSceneDeinit();
		VSIUTILPLUS_API virtual void VSIUtilLoadMesh(char*, MeshType, SeparatorType);
		VSIUTILPLUS_API virtual int VSIUtilLoadShaders(char*, char*);
		VSIUTILPLUS_API virtual int VSIUtilLoadTexture(char*);
		VSIUTILPLUS_API virtual void VSIUtilGenAndBindBuffer(GLuint, GLuint, BufferType);
		VSIUTILPLUS_API virtual std::vector<glm::vec3>* VSIUtilGetVertices() = 0;
		VSIUTILPLUS_API virtual std::vector<glm::vec3>* VSIUtilGetNormals() = 0;
		VSIUTILPLUS_API virtual std::vector<glm::vec2>* VSIUtilGetTexcoords() = 0;
	};
}

#if defined WIN32
#define VSI_MAIN(ClassName)				\
int WINAPI WinMain(HINSTANCE hInstance, \
	HINSTANCE hPrevInstance,            \
	LPSTR lpCmdLine,                    \
	int nCmdShow)                       \
{										\
	ClassName *util = new ClassName();	\
	util->WindowInit();					\
	WPARAM retVal = util->VSIUtilRun(	\
    hInstance,							\
	hPrevInstance,						\
	lpCmdLine,							\
	nCmdShow);							\
	delete(util);						\
	util = NULL;						\
	return (int)retVal;					\
}										
#endif