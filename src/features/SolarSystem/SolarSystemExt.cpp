#include <Renderer.h>
#include "SolarSystemExt.h"

#include <vector>
using namespace std;

#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtx\transform.hpp>
using glm::mat4;
using glm::vec2;

#include <Camera.h>
#include <OGLProgram.h>
using namespace Utility;

#include "Entities\SolarBody.h"
#include "Entities\SolarSystem.h"
#include "Entities\Space.h"

#include "..\VSIUtilMeshLoader.h"
#include "VSIUtilTexture.h"
#include "..\VSIUtilShader.h"
#include "VSIUtil.h"
#include <random>
#include "Noise.h"
#include "Perlin.h"
#include "SOIL.h"

using namespace Features;

extern int g_CameraIteratorIndex;
/*----------------------------------------------------------------------------------------------------------*/
//Global variable declarations
OGLProgram programPlanet, programEarth, programSun, programSpace;

/************** Rohit Changes for SUN ****************/
std::vector<glm::vec3> mVSIVertices;
std::vector<glm::vec2> mVSITextureCoords;
std::vector<glm::vec3> mVSINormals;
GLuint mProjMatLocation;
GLuint mMVLocation;
GLuint mOffsetLocation;
GLuint mVaoSun;
GLuint buffer[3];
int  mNoise3DTexSize = 128;
GLuint mNoise3DTexName;
GLubyte *mNoise3DTexPtr;
GLuint mProgramObjSun;
GLuint textureSun;

/*** Sun Bloom Effect ***/
GLuint renderFboSun;
GLuint texBrightpassSun;
GLuint filterFboSun[2];
GLuint texFilterSun[2];
GLuint texLutSun;
GLuint g_progObjBloomScene;
GLuint g_progObjBloomFilter;
GLuint g_progObjBloomResolve;
GLuint g_Resolve_Exposure;

enum
{
    MAX_SCENE_WIDTH = 2048,
    MAX_SCENE_HEIGHT = 2048
};

GLuint g_noiseFbo;
GLuint g_noiseFboTexture;
/************** Rohit Changes for SUN ****************/

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
Space space(100000000.0f);
//Space space(20000.0f);

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

  // data.scale = 4000.7493f;
  // data.rotation_speed = 0.0f;
  // data.revolution_speed = 0.0f;
  // data.revolution_radius = 0.0f;
  // data.texture_file = "res/SolarSystem/textures/planets/sun.jpg";
  // data.textureUnit = GL_TEXTURE0 + (count++);
  // data.mipmap = true;
  // planets[sun] = new SolarBody(data);

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
   data.revolution_speed = 0.09f;
   data.revolution_radius = distMercurySun * 1.992f;
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
   data.revolution_speed = 0.108f;
   data.revolution_radius = distMercurySun * 3.717f;
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
void Make3DNoiseTexture(void)
{
    int f, i, j, k, inc;
    int startFrequency = 25;
    int numOctaves = 4;
    double ni[3];
    double inci, incj, inck;
    int frequency = startFrequency;
    GLubyte *ptr;
    double amplitude = 0.6;


    if ((mNoise3DTexPtr = (GLubyte*)malloc(mNoise3DTexSize *
        mNoise3DTexSize *
        mNoise3DTexSize * 4)) == NULL)
    {
        return;
    }

    for (f = 0, inc = 0; f < numOctaves;
        ++f, frequency *= 2, ++inc, amplitude *= 0.5)
    {
        ptr = mNoise3DTexPtr;
        ni[0] = ni[1] = ni[2] = 0;

        inci = 1.0 / (mNoise3DTexSize / frequency);

        for (i = 0; i < mNoise3DTexSize; ++i, ni[0] += inci)
        {
            incj = 1.0 / (mNoise3DTexSize / frequency);
            for (j = 0; j < mNoise3DTexSize; ++j, ni[1] += incj)
            {
                inck = 1.0 / (mNoise3DTexSize / frequency);
                for (k = 0; k < mNoise3DTexSize; ++k, ni[2] += inck, ptr += 4)
                {
                    *(ptr + inc) = (GLubyte)(((noise3(ni) + 1.0) * amplitude) * 128.0);
                }
            }
        }
    }
}

void LoadTexture()
{
    glGenTextures(1, &mNoise3DTexName);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_3D, mNoise3DTexName);
    glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, mNoise3DTexSize,
        mNoise3DTexSize, mNoise3DTexSize, 0, GL_RGBA,
        GL_UNSIGNED_BYTE, mNoise3DTexPtr);
}

int VSIUtilLoadTexture(char* imageFile, GLuint* texture)
{
    GLvoid* image;
    int iRetVal = 0;
    int width, height;
    /* load an image file directly as a new OpenGL texture */

    image = SOIL_load_image
    (
        imageFile,
        &width,
        &height,
        0,
        SOIL_LOAD_RGBA
    );
    if (image == NULL)
    {
        const char* ch = SOIL_last_result();
        MessageBoxA(NULL, ch, "ERROR", MB_OK);
        return -1;
    }
    glGenTextures(1, texture);
    glBindTexture(GL_TEXTURE_2D, *texture);
    glTexStorage2D(GL_TEXTURE_2D, 8, GL_RGB8, width, height);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);

    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    return 0;
}

void LoadBloomShaders()
{
    g_progObjBloomScene = VSIUtilLoadShaders("VSIDemoSiliconAtomBloomScene.vs.glsl", "VSIDemoSiliconAtomBloomScene.fs.glsl");
    g_progObjBloomFilter = VSIUtilLoadShaders("VSIDemoSiliconAtomBloomFilter.vs.glsl", "VSIDemoSiliconAtomBloomFilter.fs.glsl");

    g_progObjBloomResolve = VSIUtilLoadShaders("VSIDemoSiliconAtomResolve.vs.glsl", "VSIDemoSiliconAtomResolve.fs.glsl");
    g_Resolve_Exposure = glGetUniformLocation(g_progObjBloomResolve, "exposure");
}

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

   if(!programSpace.Initialize("res/SolarSystem/Shaders/vSpace.glsl",
                               "res/SolarSystem/Shaders/fSpace.glsl",
                               "",
                               "",
                               "",
                               msg)) {
      MessageBoxA(NULL, msg.c_str(), "ERROR", MB_OK);
      return RENDERER_RESULT_ERROR;
   }

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

  //if(!programSun.Initialize("res/SolarSystem/Shaders/vSun.glsl",
  //                          "res/SolarSystem/Shaders/fSun.glsl",
  //                          "",
  //                          "",
  //                          "",
  //                          msg)) {
  //   MessageBoxA(NULL, msg.c_str(), "ERROR", MB_OK);
  //
  //   return RENDERER_RESULT_ERROR;
  //}
  /******************Rohit Changes for SUN **********************/
   VSIUtilLoadMesh("Sphere.obj", VERTEX_NORMAL_AND_TEXTURE, mVSIVertices, mVSITextureCoords, mVSINormals);
//   LoadBloomShaders();
   mProgramObjSun = VSIUtilLoadShaders("res/SolarSystem/Shaders/vSun.glsl", "res/SolarSystem/Shaders/fSun.glsl");
   mProjMatLocation = glGetUniformLocation(mProgramObjSun, "projectionViewMatrix");
   mMVLocation = glGetUniformLocation(mProgramObjSun, "modelMatrix");
   mOffsetLocation = glGetUniformLocation(mProgramObjSun, "offset");

   glGenVertexArrays(1, &mVaoSun);
   glBindVertexArray(mVaoSun);

   glGenBuffers(1, &buffer[0]);
   glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
   glBufferData(GL_ARRAY_BUFFER, (mVSIVertices.size() * sizeof(glm::vec3)), &(mVSIVertices[0].x), GL_STATIC_DRAW);
   glEnableVertexAttribArray(0);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

   Make3DNoiseTexture();
   LoadTexture();

  // glGenFramebuffers(1, &g_noiseFbo);
  // glBindFramebuffer(GL_FRAMEBUFFER, g_noiseFbo);
  // glGenTextures(1, &g_noiseFboTexture);
  // glBindTexture(GL_TEXTURE_2D, g_noiseFboTexture);
  // glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA16F, MAX_SCENE_WIDTH, MAX_SCENE_HEIGHT);
  // glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texBrightpassSun, 0);
  // static const GLenum noiseFboDrawBuffers[] = { GL_COLOR_ATTACHMENT0 };
  // glDrawBuffers(1, noiseFboDrawBuffers);
  //
  // static const GLenum bloombuffers[] = { GL_COLOR_ATTACHMENT0 };
  // static const GLfloat exposureLUT[20] = { 11.0f, 6.0f, 3.2f, 2.8f, 2.2f, 1.90f, 1.80f, 1.80f, 1.70f, 1.70f,  1.60f, 1.60f, 1.50f, 1.50f, 1.40f, 1.40f, 1.30f, 1.20f, 1.10f, 1.00f };
  // 
  // glGenFramebuffers(1, &renderFboSun);
  // glBindFramebuffer(GL_FRAMEBUFFER, renderFboSun);
  // glGenTextures(1, &texBrightpassSun);
  // glBindTexture(GL_TEXTURE_2D, texBrightpassSun);
  // glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA16F, MAX_SCENE_WIDTH, MAX_SCENE_HEIGHT);
  // glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texBrightpassSun, 0);
  // glDrawBuffers(1, bloombuffers);
  //
  // glGenFramebuffers(2, &filterFboSun[0]);
  // glGenTextures(2, &texFilterSun[0]);
  // for (int i = 0; i < 2; i++)
  // {
  //     glBindFramebuffer(GL_FRAMEBUFFER, filterFboSun[i]);
  //     glBindTexture(GL_TEXTURE_2D, texFilterSun[i]);
  //     glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA16F, i ? MAX_SCENE_WIDTH : MAX_SCENE_HEIGHT, i ? MAX_SCENE_HEIGHT : MAX_SCENE_WIDTH);
  //     glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texFilterSun[i], 0);
  //     glDrawBuffers(1, bloombuffers);
  // }
  //
  // glBindFramebuffer(GL_FRAMEBUFFER, 0);
  //
  // glGenTextures(1, &texLutSun);
  // glBindTexture(GL_TEXTURE_1D, texLutSun);
  // glTexStorage1D(GL_TEXTURE_1D, 1, GL_R32F, 20);
  // glTexSubImage1D(GL_TEXTURE_1D, 0, 0, 20, GL_RED, GL_FLOAT, exposureLUT);
  // glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  // glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

   glBindVertexArray(0);

   /******************Rohit Changes for SUN **********************/

  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  if(!InitSolarSystem())
     return RENDERER_RESULT_ERROR;

  if(!space.Initialize()) {
     return RENDERER_RESULT_ERROR;
  }

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
void RenderSpace()
{
   static mat4 projView;

   glDisable(GL_CULL_FACE);

   programSpace.Use(true);

   projView = projectionMatrix * camera.GetViewMatrix();

   static GLint uniformProjViewMatrix = programSpace.GetUniformLocation("projectionViewMatrix");
   static GLint uniformCubeSampler = programSpace.GetUniformLocation("cubeSampler");

   glUniformMatrix4fv(uniformProjViewMatrix, 1, GL_FALSE, &projView[0][0]);
   glUniform1i(uniformCubeSampler, 99);

   space.Render(GL_QUADS);

   programSpace.Use(false);
   glEnable(GL_CULL_FACE);
}

/*----------------------------------------------------------------------------------------------------------*/
void RenderSun()
{
    static GLfloat offset = 0.1f;
    //glBindFramebuffer(GL_FRAMEBUFFER, g_noiseFbo);
    glUseProgram(mProgramObjSun);

    mat4 modelMatrix = mat4(1.0);

    modelMatrix = glm::scale(modelMatrix, vec3(4000.7493f));

    static mat4 projectionViewMatrix;
    projectionViewMatrix = projectionMatrix * camera.GetViewMatrix();

    glUniformMatrix4fv(mMVLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glUniformMatrix4fv(mProjMatLocation, 1, GL_FALSE, glm::value_ptr(projectionViewMatrix));
    glUniform1f(mOffsetLocation, offset);
    offset += 0.00005f;
    glBindVertexArray(mVaoSun);
    glActiveTexture(GL_TEXTURE0);
    glDrawArrays(GL_TRIANGLES, 0, mVSIVertices.size());
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    /*******Bloom Code begins *******/
    //glUseProgram(g_progObjBloomScene);
    //glBindFramebuffer(GL_FRAMEBUFFER, renderFboSun);
    //glDrawBuffer(GL_BACK);
    //glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_2D, g_noiseFboTexture);
    //
    //glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    //glDisable(GL_DEPTH_TEST);
    //glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //glUseProgram(g_progObjBloomFilter);
    //
    //glBindFramebuffer(GL_FRAMEBUFFER, filterFboSun[0]);
    //glBindTexture(GL_TEXTURE_2D, texBrightpassSun);
    //
    //glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    //glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //glBindFramebuffer(GL_FRAMEBUFFER, filterFboSun[1]);
    //glBindTexture(GL_TEXTURE_2D, texFilterSun[0]);
    //
    //glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    //glUseProgram(g_progObjBloomResolve);
    //
    //static GLfloat e = 0.0;
    //glUniform1f(g_Resolve_Exposure, e);
    //e +=0.001;
    //glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //glActiveTexture(GL_TEXTURE0);
    //
    //glBindTexture(GL_TEXTURE_2D, texFilterSun[1]);
    //glActiveTexture(GL_TEXTURE1);
    //glBindTexture(GL_TEXTURE_2D, g_noiseFboTexture);
    //glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

}

RendererResult SolarSystemExtRenderer::Render(const RenderParams &params)
{
   RendererResult result = RENDERER_RESULT_SUCCESS;
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   RenderSpace();
   RenderSun();

   for(int planet = mercury; planet < earth; planet++) {
      planets[planet]->Render(programPlanet, projectionMatrix, camera.GetViewMatrix());
   }
   
   for(int planet = mars; planet < lastPlanet; planet++) {
      planets[planet]->Render(programPlanet, projectionMatrix, camera.GetViewMatrix());
   }
   
   planets[earth]->Render(programEarth, projectionMatrix, camera.GetViewMatrix());
   
   if (g_CameraIteratorIndex >= 1350)
   {
       return RENDERER_RESULT_FINISHED;
   }
 
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

   projectionMatrix = glm::perspective(glm::radians(45.0f), float(width) / float(height), 0.1f, 1000000000.0f);

   glViewport(0, 0, (GLsizei) width, (GLsizei) height);
}
