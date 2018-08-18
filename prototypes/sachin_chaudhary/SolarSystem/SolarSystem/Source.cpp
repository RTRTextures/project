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

#include <random>

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

OGLProgram programPlanet, programEarth, programSun;

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

SolarBody* planets[lastPlanet];
SolarSystem solarSystem;

mat4 modelMatrix, projectionMatrix;
const float distMercurySun = 6000.0f;

bool fastCamera = true;

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

      case 'p':
         fastCamera = !fastCamera;
         if(fastCamera)
            camera.SetSpeed(390.0f);
         else
            camera.SetSpeed(2.0f);
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
double Random(double min = 0.0f, double max = 1.0f)
{
   static std::random_device rd;  //Will be used to obtain a seed for the random number engine
   static std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()

   uniform_real_distribution<double> dis(min, max);

   return dis(gen);
}

/*----------------------------------------------------------------------------------------------------------*/
void InitSolarSystem()
{
   if(!SolarBody::Initialize()) {
      MessageBoxA(ghwnd, "SOLARBODY_INIT_ERROR", "ERROR", MB_OK);
      uninitialize();
      exit(-1);
   }

   unsigned int count = 0;
   SolarBody::SolarData data;

   // global Y: common axis of rotation for all planets and satellites
   // any abitrary rotation axis is supported though !
   data.axis = vec3(0.0f, 1.0f, 0.0f);

   data.scale = 9.7493f;
   data.rotation_speed = 0.0f;
   data.revolution_speed = 0.0f;
   data.revolution_radius = 0.0f;
   data.texture_file = "res/textures/planets/sun.jpg";
   data.textureUnit = GL_TEXTURE0 + (count++);
   data.mipmap = true;
   planets[sun] = new SolarBody(data);

   data.scale = 0.0342f;
   data.rotation_speed = 0.02f;
   data.revolution_speed = 0.01f;
   data.revolution_radius = distMercurySun;
   data.texture_file = "res/textures/planets/mercury.jpg";
   data.textureUnit = GL_TEXTURE0 + (count++);
   data.mipmap = true;
   planets[mercury] = new SolarBody(data);

   data.scale = 0.0846f;
   data.rotation_speed = 0.2f;
   data.revolution_speed = 0.0065f;
   data.revolution_radius = distMercurySun * 1.0934f;
   data.texture_file = "res/textures/planets/venus.jpg";
   data.textureUnit = GL_TEXTURE0 + (count++);
   data.mipmap = true;
   planets[venus] = new SolarBody(data);

   data.scale = 0.0892f;
   data.rotation_speed = 0.2f;
   data.revolution_speed = 0.004f;
   data.revolution_radius = distMercurySun * 2.292f;
   data.texture_file = "res/textures/planets/earth.jpg";
   data.textureUnit = GL_TEXTURE0 + (count++);
   data.mipmap = true;
   planets[earth] = new SolarBody(data);

   data.scale = 0.008f;
   data.revolution_radius = 75.0f;
   data.rotation_speed = data.revolution_speed = 2.5f;
   data.texture_file = "res/textures/moons/earth/moon.jpg";
   data.textureUnit = GL_TEXTURE0 + (count++);
   planets[earth]->AddSatellite(data);

   data.scale = 0.0475f;
   data.rotation_speed = 0.2f;
   data.revolution_speed = 0.003f;
   data.revolution_radius = distMercurySun * 2.968f;
   data.texture_file = "res/textures/planets/mars.jpg";
   data.textureUnit = GL_TEXTURE0 + (count++);
   data.mipmap = true;
   planets[mars] = new SolarBody(data);

   data.scale = 1.0f;
   data.rotation_speed = 0.2f;
   data.revolution_speed = 0.019f;
   data.revolution_radius = distMercurySun * 7.717f;
   data.texture_file = "res/textures/planets/jupiter.jpg";
   data.textureUnit = GL_TEXTURE0 + (count++);
   data.mipmap = true;
   planets[jupiter] = new SolarBody(data);

   data.scale = 0.025f;
   data.revolution_radius = 650.0f;
   data.revolution_speed = 2.3f;
   data.texture_file = "res/textures/moons/jupiter/europa.png";
   data.textureUnit = GL_TEXTURE0 + (count++);


   data.scale = 0.002f;
   data.revolution_radius = 650.0f;
   data.revolution_speed = 0.3f;
   data.texture_file = "res/textures/moons/jupiter/europa.png";
   data.textureUnit = GL_TEXTURE0 + (count++);
   planets[jupiter]->AddSatellite(data);

   data.scale = 0.007f;
   data.revolution_radius = 750.0f;
   data.texture_file = "res/textures/moons/jupiter/Callisto.png";
   data.textureUnit = GL_TEXTURE0 + (count++);
   data.revolution_speed = 0.5f;
   planets[jupiter]->AddSatellite(data);

   data.scale = 0.029f;
   data.revolution_radius = 800.0f;
   data.texture_file = "res/textures/moons/jupiter/ganymede.jpg";
   data.textureUnit = GL_TEXTURE0 + (count++);
   data.revolution_speed = 0.67f;
   planets[jupiter]->AddSatellite(data);

   data.scale = 0.009f;
   data.revolution_radius = 680.0f;
   data.texture_file = "res/textures/moons/jupiter/io.png";
   data.textureUnit = GL_TEXTURE0 + (count++);
   data.revolution_speed = 0.85f;
   planets[jupiter]->AddSatellite(data);


   data.scale = 0.843f;
   data.rotation_speed = 0.2f;
   data.revolution_speed = 0.015f;
   data.revolution_radius = distMercurySun * 13.338f;
   data.texture_file = "res/textures/planets/saturn.jpg";
   data.textureUnit = GL_TEXTURE0 + (count++);
   data.mipmap = true;
   planets[saturn] = new SolarBody(data);

   data.scale = 0.3575f;
   data.rotation_speed = 0.2f;
   data.revolution_speed = 0.032f;
   data.revolution_radius = distMercurySun * 25.788f;
   data.texture_file = "res/textures/planets/uranus.jpg";
   data.textureUnit = GL_TEXTURE0 + (count++);
   data.mipmap = true;
   planets[uranus] = new SolarBody(data);

   data.scale = 0.3464f;
   data.rotation_speed = 0.2f;
   data.revolution_speed = 0.063f;
   data.revolution_radius = distMercurySun * 39.89f;
   data.texture_file = "res/textures/planets/neptune.jpg";
   data.textureUnit = GL_TEXTURE0 + (count++);
   data.mipmap = true;
   planets[neptune] = new SolarBody(data);

   for(int i = mercury; i < lastPlanet; i++)
      solarSystem.AddSolarBody(planets[i]);
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

   if(!programPlanet.Initialize("Shaders/vPlanet.glsl",
                               "Shaders/fPlanet.glsl",
                               "",
                               "",
                               "",
                               msg)) {
      MessageBoxA(ghwnd, msg.c_str(), "ERROR", MB_OK);
      uninitialize();
      exit(-1);
   }


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

   if(!programSun.Initialize("Shaders/vSun.glsl",
                             "Shaders/fSun.glsl",
                             "",
                             "",
                             "",
                             msg)) {
      MessageBoxA(ghwnd, msg.c_str(), "ERROR", MB_OK);
      uninitialize();
      exit(-1);
   }

   //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   InitSolarSystem();

   camera.Set(vec3(-8967.87109f, 370.465637f, 14319.4648f), vec3(0.747302055f, -0.00399998995f, -0.664473951f));
   camera.SetSpeed(390.0f);
}

/*----------------------------------------------------------------------------------------------------------*/
void RenderSun()
{
   static float start = 0.0f;
   programSun.Use(true);
   static GLint uniformStart = programSun.GetUniformLocation("start");
   start += 0.00016f;
   glUniform1f(uniformStart, start);

   planets[sun]->Render(programSun, projectionMatrix, camera.GetViewMatrix());
}

/*----------------------------------------------------------------------------------------------------------*/
void display(void)
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   RenderSun();

   for(int planet = mercury; planet < earth; planet++) {
      planets[planet]->Render(programPlanet, projectionMatrix, camera.GetViewMatrix());
   }

   for(int planet = mars; planet < lastPlanet; planet++) {
      planets[planet]->Render(programPlanet, projectionMatrix, camera.GetViewMatrix());
   }

   planets[earth]->Render(programEarth, projectionMatrix, camera.GetViewMatrix());

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

   for(unsigned int i = sun; i < lastPlanet; i++)
      delete planets[i];

   SolarBody::Uninitialize();
}
