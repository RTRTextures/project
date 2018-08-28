#include <Renderer.h>
#include "SolarSystemExt.h"

#include <vector>
using namespace std;

#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
using glm::mat4;
using glm::vec2;

#include <Camera.h>
#include <OGLProgram.h>
using namespace Utility;

#include "Entities\SolarBody.h"
#include "Entities\SolarSystem.h"

#include <random>
using namespace Features;

/*----------------------------------------------------------------------------------------------------------*/
//Global variable declarations
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

const char* SolarSystemExtRenderer::GetName()
{
   static const char name[] = "SolarSystemExtRenderer";
   return name;
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
bool SolarSystemExtRenderer::InitSolarSystem()
{
   if(!SolarBody::Initialize()) {
      MessageBoxA(NULL, "SOLARBODY_INIT_ERROR", "ERROR", MB_OK);
      Uninitialize();
      return false;
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
   data.texture_file = "res/SolarSystem/textures/planets/sun.jpg";
   data.textureUnit = GL_TEXTURE0 + (count++);
   data.mipmap = true;
   planets[sun] = new SolarBody(data);

   data.scale = 0.0342f;
   data.rotation_speed = 0.02f;
   data.revolution_speed = 0.01f;
   data.revolution_radius = distMercurySun;
   data.texture_file = "res/SolarSystem/textures/planets/mercury.jpg";
   data.textureUnit = GL_TEXTURE0 + (count++);
   data.mipmap = true;
   planets[mercury] = new SolarBody(data);

   data.scale = 0.0846f;
   data.rotation_speed = 0.2f;
   data.revolution_speed = 0.0065f;
   data.revolution_radius = distMercurySun * 1.0934f;
   data.texture_file = "res/SolarSystem/textures/planets/venus.jpg";
   data.textureUnit = GL_TEXTURE0 + (count++);
   data.mipmap = true;
   planets[venus] = new SolarBody(data);

   data.scale = 0.0892f;
   data.rotation_speed = 0.2f;
   data.revolution_speed = 0.004f;
   data.revolution_radius = distMercurySun * 2.292f;
   data.texture_file = "res/SolarSystem/textures/planets/earth.jpg";
   data.textureUnit = GL_TEXTURE0 + (count++);
   data.mipmap = true;
   planets[earth] = new SolarBody(data);

   data.scale = 0.008f;
   data.revolution_radius = 75.0f;
   data.rotation_speed = data.revolution_speed = 2.5f;
   data.texture_file = "res/SolarSystem/textures/moons/earth/moon.jpg";
   data.textureUnit = GL_TEXTURE0 + (count++);
   planets[earth]->AddSatellite(data);

   data.scale = 0.0475f;
   data.rotation_speed = 0.2f;
   data.revolution_speed = 0.003f;
   data.revolution_radius = distMercurySun * 2.968f;
   data.texture_file = "res/SolarSystem/textures/planets/mars.jpg";
   data.textureUnit = GL_TEXTURE0 + (count++);
   data.mipmap = true;
   planets[mars] = new SolarBody(data);

   data.scale = 1.0f;
   data.rotation_speed = 0.2f;
   data.revolution_speed = 0.019f;
   data.revolution_radius = distMercurySun * 7.717f;
   data.texture_file = "res/SolarSystem/textures/planets/jupiter.jpg";
   data.textureUnit = GL_TEXTURE0 + (count++);
   data.mipmap = true;
   planets[jupiter] = new SolarBody(data);

   data.scale = 0.025f;
   data.revolution_radius = 650.0f;
   data.revolution_speed = 2.3f;
   data.texture_file = "res/SolarSystem/textures/moons/jupiter/europa.png";
   data.textureUnit = GL_TEXTURE0 + (count++);


   data.scale = 0.002f;
   data.revolution_radius = 650.0f;
   data.revolution_speed = 0.3f;
   data.texture_file = "res/SolarSystem/textures/moons/jupiter/europa.png";
   data.textureUnit = GL_TEXTURE0 + (count++);
   planets[jupiter]->AddSatellite(data);

   data.scale = 0.007f;
   data.revolution_radius = 750.0f;
   data.texture_file = "res/SolarSystem/textures/moons/jupiter/Callisto.png";
   data.textureUnit = GL_TEXTURE0 + (count++);
   data.revolution_speed = 0.5f;
   planets[jupiter]->AddSatellite(data);

   data.scale = 0.029f;
   data.revolution_radius = 800.0f;
   data.texture_file = "res/SolarSystem/textures/moons/jupiter/ganymede.jpg";
   data.textureUnit = GL_TEXTURE0 + (count++);
   data.revolution_speed = 0.67f;
   planets[jupiter]->AddSatellite(data);

   data.scale = 0.009f;
   data.revolution_radius = 680.0f;
   data.texture_file = "res/SolarSystem/textures/moons/jupiter/io.png";
   data.textureUnit = GL_TEXTURE0 + (count++);
   data.revolution_speed = 0.85f;
   planets[jupiter]->AddSatellite(data);


   data.scale = 0.843f;
   data.rotation_speed = 0.2f;
   data.revolution_speed = 0.015f;
   data.revolution_radius = distMercurySun * 13.338f;
   data.texture_file = "res/SolarSystem/textures/planets/saturn.jpg";
   data.textureUnit = GL_TEXTURE0 + (count++);
   data.mipmap = true;
   planets[saturn] = new SolarBody(data);

   data.scale = 0.3575f;
   data.rotation_speed = 0.2f;
   data.revolution_speed = 0.032f;
   data.revolution_radius = distMercurySun * 25.788f;
   data.texture_file = "res/SolarSystem/textures/planets/uranus.jpg";
   data.textureUnit = GL_TEXTURE0 + (count++);
   data.mipmap = true;
   planets[uranus] = new SolarBody(data);

   data.scale = 0.3464f;
   data.rotation_speed = 0.2f;
   data.revolution_speed = 0.063f;
   data.revolution_radius = distMercurySun * 39.89f;
   data.texture_file = "res/SolarSystem/textures/planets/neptune.jpg";
   data.textureUnit = GL_TEXTURE0 + (count++);
   data.mipmap = true;
   planets[neptune] = new SolarBody(data);

   for(int i = mercury; i < lastPlanet; i++)
      solarSystem.AddSolarBody(planets[i]);

   return true;
}
/*----------------------------------------------------------------------------------------------------------*/

RendererResult SolarSystemExtRenderer::Initialize(Window window)
{
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

   if(!programPlanet.Initialize("res/SolarSystem/Shaders/vPlanet.glsl",
                                "res/SolarSystem/Shaders/fPlanet.glsl",
                                "",
                                "",
                                "",
                                msg)) {
      MessageBoxA(NULL, msg.c_str(), "ERROR", MB_OK);
      return RENDERER_RESULT_ERROR;
   }


   if(!programEarth.Initialize("res/SolarSystem/Shaders/vEarth.glsl",
                               "res/SolarSystem/Shaders/fEarth.glsl",
                               "",
                               "",
                               "",
                               msg)) {
      MessageBoxA(NULL, msg.c_str(), "ERROR", MB_OK);
      return RENDERER_RESULT_ERROR;
   }

   if(!programSun.Initialize("res/SolarSystem/Shaders/vSun.glsl",
                             "res/SolarSystem/Shaders/fSun.glsl",
                             "",
                             "",
                             "",
                             msg)) {
      MessageBoxA(NULL, msg.c_str(), "ERROR", MB_OK);
      return RENDERER_RESULT_ERROR;
   }

   //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   if(!InitSolarSystem())
      return RENDERER_RESULT_ERROR;

   camera.Set(vec3(-8967.87109f, 370.465637f, 14319.4648f), vec3(0.747302055f, -0.00399998995f, -0.664473951f));
   camera.SetSpeed(390.0f);

   return RENDERER_RESULT_SUCCESS;
}

void SolarSystemExtRenderer::Uninitialize(void)
{
   for(unsigned int i = sun; i < lastPlanet; i++)
      delete planets[i];

   SolarBody::Uninitialize();
}

RendererResult SolarSystemExtRenderer::InitializeScene(SceneType scene)
{
   return RENDERER_RESULT_SUCCESS;
}

void SolarSystemExtRenderer::UninitializeScene(SceneType scene)
{
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

RendererResult SolarSystemExtRenderer::Render(const RenderParams &params)
{
   RendererResult result = RENDERER_RESULT_SUCCESS;
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   RenderSun();

   for(int planet = mercury; planet < earth; planet++) {
      planets[planet]->Render(programPlanet, projectionMatrix, camera.GetViewMatrix());
   }

   for(int planet = mars; planet < lastPlanet; planet++) {
      planets[planet]->Render(programPlanet, projectionMatrix, camera.GetViewMatrix());
   }

   planets[earth]->Render(programEarth, projectionMatrix, camera.GetViewMatrix());

   return result;
}

void SolarSystemExtRenderer::OnMessage(const Message &message)
{
   switch(message.messageId) {
   case WM_MOUSEWHEEL:
      for(int i = 0; i < abs(GET_WHEEL_DELTA_WPARAM(message.wParam) / WHEEL_DELTA); i++) {
         if((short) GET_WHEEL_DELTA_WPARAM(message.wParam) > 0)
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
      mousePos.x = LOWORD(message.lParam);
      mousePos.y = HIWORD(message.lParam);
      if(handleMouseRotation) {
         camera.HandleMouse(mousePos);
      }
      break;

   case WM_KEYDOWN:
      switch(message.wParam) {
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
      switch(message.wParam) {
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

      default:
         break;
      }
      break;
   }
}

void SolarSystemExtRenderer::OnResize(unsigned int width, unsigned int height)
{
   if(height == 0)
      height = 1;

   projectionMatrix = glm::perspective(glm::radians(45.0f), float(width) / float(height), 0.1f, 1000000.0f);

   glViewport(0, 0, (GLsizei) width, (GLsizei) height);
}
