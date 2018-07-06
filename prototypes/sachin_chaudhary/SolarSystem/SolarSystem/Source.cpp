#include <windows.h>
#include <glew.h>

#include <vector>
using namespace std;

#include <glm.hpp>
#include <gtx\transform.hpp>
using glm::mat4;
using glm::vec2;

#include "Framework\Camera.h"
using namespace Utility;
#include "Framework\OGLProgram.h"

#include "Entities\SolarBody.h"
#include "Entities\SolarSystem.h"


#define WIN_WIDTH 800
#define WIN_HEIGHT 600

#pragma comment(lib, "opengl32.lib")

/*----------------------------------------------------------------------------------------------------------*/
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void display(void);
void initialize(void);
void uninitialize(void);

/*----------------------------------------------------------------------------------------------------------*/
//Global variable declarations
HWND ghwnd = NULL;
HDC ghdc = NULL;
HGLRC ghrc = NULL;

DWORD dwStyle;
WINDOWPLACEMENT wpPrev = {sizeof(WINDOWPLACEMENT)};

bool gbActiveWindow = false;
bool gbEscapeKeyIsPressed = false;
bool gbFullscreen = false;

OGLProgram programEarth;

vec2 mousePos;
Camera camera;
bool handleMouseRotation = false;

typedef enum
{
   sun = 0,
   mercury,
   venus,
   earth,
   mars,
   jupiter,
   saturn,
   uranus,
   neptune,
   
   lastPlanet
} Planets;

SolarBody planets[lastPlanet];
SolarSystem solarSystem;

mat4 modelMatrix, projectionMatrix;
float distMercurySun = 6000;

/*----------------------------------------------------------------------------------------------------------*/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
   //variable declaration
   WNDCLASSEX wndclass;
   HWND hwnd;
   MSG msg;
   TCHAR szClassName[] = TEXT("RTROGL");
   bool bDone = false;

   //code
   //initializing members of struct WNDCLASSEX
   wndclass.cbSize = sizeof(WNDCLASSEX);
   wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
   wndclass.cbClsExtra = 0;
   wndclass.cbWndExtra = 0;
   wndclass.hInstance = hInstance;
   wndclass.hbrBackground = (HBRUSH) GetStockObject(BLACK_BRUSH);
   wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
   wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
   wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
   wndclass.lpfnWndProc = WndProc;
   wndclass.lpszClassName = szClassName;
   wndclass.lpszMenuName = NULL;


   //Registering Class
   RegisterClassEx(&wndclass);

   //Create Window
   hwnd = CreateWindowEx(WS_EX_APPWINDOW,
                         szClassName,
                         TEXT("Solar System"),
                         WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE,
                         0,
                         0,
                         WIN_WIDTH,
                         WIN_HEIGHT,
                         NULL,
                         NULL,
                         hInstance,
                         NULL);

   ghwnd = hwnd;

   //initialize
   initialize();

   ShowWindow(hwnd, SW_SHOW);
   SetForegroundWindow(hwnd);
   SetFocus(hwnd);

   //Message Loop
   while(bDone == false) {
      if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
         if(msg.message == WM_QUIT)
            bDone = true;
         else {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
         }
      }
      else {
         // GAME LOOP
         if(gbActiveWindow == true) {
            if(gbEscapeKeyIsPressed == true)
               bDone = true;

            // render the scene
            display();
         }
      }
   }

   uninitialize();

   return((int) msg.wParam);
}

/*----------------------------------------------------------------------------------------------------------*/
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
   //function prototype
   void resize(int, int);
   void ToggleFullscreen(void);
   void uninitialize(void);

   //code
   switch(iMsg) {
   case WM_ACTIVATE:
      if(HIWORD(wParam) == 0)
         gbActiveWindow = true;
      else
         gbActiveWindow = false;
      break;

      //case WM_ERASEBKGND:
      //	return(0);

   case WM_SIZE:
      resize(LOWORD(lParam), HIWORD(lParam));
      break;
	  
   case WM_MOUSEWHEEL:
      for(int i = 0; i < abs(GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA); i++) {
         if((short) GET_WHEEL_DELTA_WPARAM(wParam) > 0)
            camera.MoveForward();
         else
            camera.MoveBackward();
      }
      break;
	  
   case WM_LBUTTONDOWN:
      handleMouseRotation = true;
      camera.HandleMouse(mousePos, true);
      break;

   case WM_LBUTTONUP:
      handleMouseRotation = false;
      camera.HandleMouse(mousePos, true);
      break;

   case WM_MOUSEMOVE:
      mousePos.x = LOWORD(lParam);
      mousePos.y = HIWORD(lParam);
      if(handleMouseRotation) {
         camera.HandleMouse(mousePos);
      }
      break;

   case WM_KEYDOWN:
      switch(wParam) {
      case VK_UP:
         camera.RotateUp();
         break;

      case VK_DOWN:
         camera.RotateDown();
         break;

      case VK_LEFT:
         camera.RotateLeft();
         break;

      case VK_RIGHT:
         camera.RotateRight();
         break;
      }
      break;

   case WM_CHAR:
      switch(wParam) {
      case VK_ESCAPE:
         gbEscapeKeyIsPressed = true;
         break;

      case 'w':
         camera.MoveForward();
         break;

      case 's':
         camera.MoveBackward();
         break;

      case 'a':
         camera.MoveLeft();
         break;

      case 'd':
         camera.MoveRight();
         break;

      case 'q':
         camera.MoveUp();
         break;

      case 'e':
         camera.MoveDown();
         break;

      case 'f':
      case 'F':
         if(gbFullscreen == false) {
            ToggleFullscreen();
            gbFullscreen = true;
         }
         else {
            ToggleFullscreen();
            gbFullscreen = false;
         }
         break;
      
      default:
         break;
      }
      break;

   case WM_DESTROY:
      PostQuitMessage(0);
      break;

   default:
      break;
   }

   return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}

/*----------------------------------------------------------------------------------------------------------*/
void ToggleFullscreen(void)
{
   //variable declarations
   MONITORINFO mi;

   //code
   if(gbFullscreen == false) {
      dwStyle = GetWindowLong(ghwnd, GWL_STYLE);
      if(dwStyle & WS_OVERLAPPEDWINDOW) {
         mi = {sizeof(MONITORINFO)};
         if(GetWindowPlacement(ghwnd, &wpPrev) && GetMonitorInfo(MonitorFromWindow(ghwnd, MONITORINFOF_PRIMARY), &mi)) {
            SetWindowLong(ghwnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
            SetWindowPos(ghwnd, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top, mi.rcMonitor.right - mi.rcMonitor.left, mi.rcMonitor.bottom - mi.rcMonitor.top, SWP_NOZORDER | SWP_FRAMECHANGED);
         }
      }
      ShowCursor(FALSE);
   }

   else {
      //code
      SetWindowLong(ghwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
      SetWindowPlacement(ghwnd, &wpPrev);
      SetWindowPos(ghwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED);

      ShowCursor(TRUE);
   }
}

/*----------------------------------------------------------------------------------------------------------*/
void InitSolarSystem()
{
   if(!planets[sun].Initialize()) {
      MessageBoxA(ghwnd, "MECRCURY_INIT_ERROR", "ERROR", MB_OK);
      uninitialize();
      exit(-1);
   }
   else {
      planets[sun].Scale(9.7493f);
      planets[sun].Rotate(0.0f);
      planets[sun].Revolve(0.0f, 0.0f);
      planets[sun].SetTexture("res/textures/planets/sun.jpg", GL_TEXTURE0, true);
   }

   if(!planets[mercury].Initialize()) {
      MessageBoxA(ghwnd, "MECRCURY_INIT_ERROR", "ERROR", MB_OK);
      uninitialize();
      exit(-1);
   }
   else {
      planets[mercury].Scale(0.0342f);
      planets[mercury].Rotate(0.2f);
      planets[mercury].Revolve(distMercurySun/0.0342f, 0.1f);
      planets[mercury].SetTexture("res/textures/planets/mercury.jpg", GL_TEXTURE1, true);
   }


   if(!planets[venus].Initialize()) {
      MessageBoxA(ghwnd, "VENUS_INIT_ERROR", "ERROR", MB_OK);
      uninitialize();
      exit(-1);
   }
   else {
      planets[venus].Scale(0.0846f);
      planets[venus].Rotate(0.2f);
      planets[venus].Revolve(distMercurySun * 1.0934/0.0846f, 0.065f);
      planets[venus].SetTexture("res/textures/planets/venus.jpg", GL_TEXTURE2, true);
   }

   if(!planets[earth].Initialize()) {
      MessageBoxA(ghwnd, "EARTH_INIT_ERROR", "ERROR", MB_OK);
      uninitialize();
      exit(-1);
   }
   else {
      planets[earth].Scale(0.0892f);
      planets[earth].Rotate(0.2f);
      planets[earth].Revolve(distMercurySun * 2.292f/0.0892f, 0.09f);
      planets[earth].SetTexture("res/textures/planets/earth.jpg", GL_TEXTURE3, true);
   }

   if(!planets[mars].Initialize()) {
      MessageBoxA(ghwnd, "MARS_INIT_ERROR", "ERROR", MB_OK);
      uninitialize();
      exit(-1);
   }
   else {
      planets[mars].Scale(0.0475f);
      planets[mars].Rotate(0.2f);
      planets[mars].Revolve(distMercurySun * 2.968f/0.0475f, 0.03f);
      planets[mars].SetTexture("res/textures/planets/mars.jpg", GL_TEXTURE4, true);
   }

   if(!planets[jupiter].Initialize()) {
      MessageBoxA(ghwnd, "JUPITER_INIT_ERROR", "ERROR", MB_OK);
      uninitialize();
      exit(-1);
   }
   else {
      planets[jupiter].Scale(1.0f);
      planets[jupiter].Rotate(0.2f);
      planets[jupiter].Revolve(distMercurySun * 7.717f, 0.09f);
      planets[jupiter].SetTexture("res/textures/planets/jupiter.jpg", GL_TEXTURE5, true);
   }

   if(!planets[saturn].Initialize()) {
      MessageBoxA(ghwnd, "SATURN_INIT_ERROR", "ERROR", MB_OK);
      uninitialize();
      exit(-1);
   }
   else {
      planets[saturn].Scale(0.843f);
      planets[saturn].Rotate(0.2f);
      planets[saturn].Revolve(distMercurySun * 13.338/0.843f, 0.15f);
      planets[saturn].SetTexture("res/textures/planets/saturn.jpg", GL_TEXTURE6, true);
   }

   if(!planets[uranus].Initialize()) {
      MessageBoxA(ghwnd, "URANUS_INIT_ERROR", "ERROR", MB_OK);
      uninitialize();
      exit(-1);
   }
   else {
      planets[uranus].Scale(0.3575f);
      planets[uranus].Rotate(0.2f);
      planets[uranus].Revolve(distMercurySun * 25.788f/0.3575f, 0.32f);
      planets[uranus].SetTexture("res/textures/planets/uranus.jpg", GL_TEXTURE7, true);
   }

   if(!planets[neptune].Initialize()) {
      MessageBoxA(ghwnd, "NEPTUNE_INIT_ERROR", "ERROR", MB_OK);
      uninitialize();
      exit(-1);
   }
   else {
      planets[neptune].Scale(0.3464f);
      planets[neptune].Rotate(0.2f);
      planets[neptune].Revolve(distMercurySun * 39.89f/0.3464f, 0.63f);
      planets[neptune].SetTexture("res/textures/planets/neptune.jpg", GL_TEXTURE8, true);
   }

   for(int i = sun; i < lastPlanet; i++) {
      solarSystem.AddSolarBody(&planets[(Planets) i]);
   }
}

/*----------------------------------------------------------------------------------------------------------*/
void initialize(void)
{
   //function prototypes
   void resize(int, int);

   //variable declarations
   PIXELFORMATDESCRIPTOR pfd;
   int iPixelFormatIndex;

   //code
   ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));

   //Initialization of structure 'PIXELFORMATDESCRIPTOR'
   pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
   pfd.nVersion = 1;
   pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
   pfd.iPixelType = PFD_TYPE_RGBA;
   pfd.cColorBits = 32;
   pfd.cRedBits = 8;
   pfd.cGreenBits = 8;
   pfd.cBlueBits = 8;
   pfd.cAlphaBits = 8;
   pfd.cDepthBits = 32;

   ghdc = GetDC(ghwnd);

   iPixelFormatIndex = ChoosePixelFormat(ghdc, &pfd);
   if(iPixelFormatIndex == 0) {
      ReleaseDC(ghwnd, ghdc);
      ghdc = NULL;
   }

   if(SetPixelFormat(ghdc, iPixelFormatIndex, &pfd) == FALSE) {
      ReleaseDC(ghwnd, ghdc);
      ghdc = NULL;
   }

   ghrc = wglCreateContext(ghdc);
   if(ghrc == NULL) {
      ReleaseDC(ghwnd, ghdc);
      ghdc = NULL;
   }

   if(wglMakeCurrent(ghdc, ghrc) == FALSE) {
      wglDeleteContext(ghrc);
      ghrc = NULL;
      ReleaseDC(ghwnd, ghdc);
      ghdc = NULL;
   }

   glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

   glShadeModel(GL_SMOOTH);
   // set-up depth buffer
   glClearDepth(1.0f);
   // enable depth testing
   glEnable(GL_DEPTH_TEST);
   // depth test to do
   glDepthFunc(GL_LEQUAL);
   // set really nice percpective calculations ?
   glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
   // We will always cull back faces for better performance
   glEnable(GL_CULL_FACE);

   glewInit();

   string msg;
   if(!programEarth.Initialize("Shaders/vEarth.glsl",
                               "Shaders/fEarth.glsl",
                               "",
                               "",
                               "",
                               msg)) {
      MessageBoxA(ghwnd, msg.c_str(), "ERROR", MB_OK);
      uninitialize();
      exit(-1);
   }

   InitSolarSystem();

   camera.Set(vec3(0.0f, 0.0f, 18000.0f), vec3(0.0f, 0.0f, -1.0f));
   camera.SetSpeed(390.0f);
}

/*----------------------------------------------------------------------------------------------------------*/
void display(void)
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   solarSystem.Render(programEarth, projectionMatrix, camera.GetViewMatrix());

   SwapBuffers(ghdc);
}

/*----------------------------------------------------------------------------------------------------------*/
void resize(int width, int height)
{
   if(height == 0)
      height = 1;

   projectionMatrix = glm::perspective(glm::radians(45.0f), float(width) / float(height), 0.1f, 1000000.0f);

   glViewport(0, 0, (GLsizei) width, (GLsizei) height);
}

/*----------------------------------------------------------------------------------------------------------*/
void uninitialize(void)
{
   //UNINITIALIZATION CODE

   if(gbFullscreen == true) {
      dwStyle = GetWindowLong(ghwnd, GWL_STYLE);
      SetWindowLong(ghwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
      SetWindowPlacement(ghwnd, &wpPrev);
      SetWindowPos(ghwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED);

      ShowCursor(TRUE);
   }

   wglMakeCurrent(NULL, NULL);

   wglDeleteContext(ghrc);
   ghrc = NULL;

   ReleaseDC(ghwnd, ghdc);
   ghdc = NULL;

   DestroyWindow(ghwnd);
   ghwnd = NULL;
}
