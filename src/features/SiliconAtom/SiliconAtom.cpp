#include "SiliconAtom/SiliconAtom.h"
#include "VSIUtilTexture.h"
#include "VSIUtilShader.h"
#include "VSIUtilMeshLoader.h"
#include "VSIUtilBuffers.h"
#include <Camera.h>
#include <glm\mat4x4.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#define DELTA_FIRST_ORBIT 0.001f
#define DELTA_SECOND_ORBIT 0.003f
#define DELTA_THIRD_ORBIT 0.008f

extern bool fastCamera;

extern vec2  mousePos;
extern Utility::Camera camera;
extern bool handleMouseRotation;
std::vector<glm::vec3> gVerticesSphere;
std::vector<glm::vec3> gNormalsSphere;
std::vector<glm::vec2> gTexCoordsSphere;
extern mat4 modelMatrix, projectionMatrix;
GLuint gSiliconBuffer[3];

GLuint gAtomTexture[2];

GLuint gSiliconProgObj;
GLuint gProjLocation;
GLuint gModelLocation;
GLuint gViewLocation;

GLuint gDiffuseLocation;
GLuint gSpecularLocation;
GLuint gAmbientLocation;
GLuint gAlphaValueLocation;

static unsigned long count;

SiliconAtom::SiliconAtom()
{

}

SiliconAtom::~SiliconAtom()
{

}

const char* SiliconAtom::GetName()
{
    static const char name[] = "SiliconAtom";
    return name;
}

RendererResult SiliconAtom::Initialize(Window window)
{
    glewInit();

    return RENDERER_RESULT_SUCCESS;
}

void SiliconAtom::Uninitialize(void)
{

}

RendererResult SiliconAtom::InitializeScene(SceneType scene)
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    glShadeModel(GL_SMOOTH);
    // set-up depth buffer
    glClearDepth(1.0f);
    // enable depth testing
    glEnable(GL_DEPTH_TEST);
    // depth test to do
    glDepthFunc(GL_LEQUAL);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // set really nice percpective calculations ?
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    // We will always cull back faces for better performance
    glEnable(GL_CULL_FACE);


    cubeMapTexture.push_back("res\\Silicon\\CubeMap\\px.jpg");
    cubeMapTexture.push_back("res\\Silicon\\CubeMap\\nx.jpg");
    cubeMapTexture.push_back("res\\Silicon\\CubeMap\\py.jpg");
    cubeMapTexture.push_back("res\\Silicon\\CubeMap\\ny.jpg");
    cubeMapTexture.push_back("res\\Silicon\\CubeMap\\pz.jpg");
    cubeMapTexture.push_back("res\\Silicon\\CubeMap\\nz.jpg");

    m_progCubeBoxObj = VSIUtilLoadShaders(
        "res\\Silicon\\Shaders\\VSICubeMapping.vs.glsl",
        "res\\Silicon\\Shaders\\VSICubeMapping.fs.glsl");

    m_mvCubeBoxLocation = glGetUniformLocation(
        m_progCubeBoxObj,
        "mvMatrix");
    
    VSIUtilLoadMesh(
        "res\\Silicon\\Model\\Sphere.obj", 
        VERTEX_NORMAL_AND_TEXTURE,
        gVerticesSphere, 
        gTexCoordsSphere, 
        gNormalsSphere);

    gSiliconProgObj = VSIUtilLoadShaders(
        "res\\Silicon\\Shaders\\VSISilicon.vs.glsl", 
        "res\\Silicon\\Shaders\\VSISilicon.fs.glsl");

    gProjLocation = glGetUniformLocation(gSiliconProgObj, "proj_matrix");
    gModelLocation = glGetUniformLocation(gSiliconProgObj, "model_matrix");
    gViewLocation = glGetUniformLocation(gSiliconProgObj, "view_matrix");
    gDiffuseLocation = glGetUniformLocation(gSiliconProgObj, "diffuse_albedo");
    gSpecularLocation = glGetUniformLocation(gSiliconProgObj, "specular_albedo");
    gAmbientLocation = glGetUniformLocation(gSiliconProgObj, "ambient");
    gAlphaValueLocation = glGetUniformLocation(gSiliconProgObj, "alpha_value");
    
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    VSIUtilGenAndBindBuffer(
        gSiliconBuffer[0], 
        0, 
        VERTEX,
        gVerticesSphere,
        gTexCoordsSphere,
        gNormalsSphere);

    VSIUtilGenAndBindBuffer(
        gSiliconBuffer[1], 
        1, 
        TEXTURECOORDS,
        gVerticesSphere,
        gTexCoordsSphere,
        gNormalsSphere);

    VSIUtilGenAndBindBuffer(
        gSiliconBuffer[2], 
        2, 
        NORMALS,
        gVerticesSphere,
        gTexCoordsSphere,
        gNormalsSphere
        );

    glActiveTexture(GL_TEXTURE0);
    VSIUtilLoadCubeMapTextures(cubeMapTexture, &m_TexCubeMap);

    //glActiveTexture(GL_TEXTURE0);
    //VSIUtilLoadTexture("res\\Silicon\\Textures\\bluesolid.jpg", &gAtomTexture[1]);
    
    glBindVertexArray(0);
    return RENDERER_RESULT_SUCCESS;
}
void RenderPhosphorusAtom(glm::mat4 modelMatrix, 
    glm::mat4 viewMatrix,
    BOOL ifMove)
{
    static GLfloat alphaFactor = 0.0f;
    /***************Silicon Proton 1******************/
    {
        glm::mat4 model_matrixProton1 = glm::scale(modelMatrix, vec3(0.2f));
        glUniformMatrix4fv(gModelLocation, 1, GL_FALSE, glm::value_ptr(model_matrixProton1));
        glUniformMatrix4fv(gViewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
        glUniformMatrix4fv(gProjLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
        glUniform3f(gDiffuseLocation, 0.1, 0.1, 0.1);
        glUniform3f(gSpecularLocation, 0.1, 0.1, 0.1);
        glUniform3f(gAmbientLocation, 0.5, 0.3, 0.3);
        glUniform1f(gAlphaValueLocation, 0.95f - alphaFactor);

        glDrawArrays(GL_TRIANGLES, 0, gVerticesSphere.size());
    }
    /***************Silicon Proton 1******************/
    /***************Silicon Proton 2******************/
    {
        glm::mat4 model_matrixProton2 = glm::scale(modelMatrix, vec3(0.2f));
        model_matrixProton2 = glm::translate(
            model_matrixProton2,
            glm::vec3(0.0, 2.0, 0.0));
        glUniformMatrix4fv(gModelLocation, 1, GL_FALSE, glm::value_ptr(model_matrixProton2));
        glUniformMatrix4fv(gViewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
        glUniformMatrix4fv(gProjLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
        glUniform3f(gDiffuseLocation, 0.1, 0.1, 0.1);
        glUniform3f(gSpecularLocation, 0.1, 0.1, 0.1);
        glUniform3f(gAmbientLocation, 0.5, 0.3, 0.3);
        glUniform1f(gAlphaValueLocation, 0.95f - alphaFactor);

        glDrawArrays(GL_TRIANGLES, 0, gVerticesSphere.size());
    }
    /***************Silicon Proton 2******************/
    /***************Silicon Proton 3******************/
    {
        glm::mat4 model_matrixProton3 = glm::scale(modelMatrix, vec3(0.2f));
        model_matrixProton3 = glm::translate(
            model_matrixProton3,
            glm::vec3(1.7, 1.0, 0.0));
        glUniformMatrix4fv(gModelLocation, 1, GL_FALSE, glm::value_ptr(model_matrixProton3));
        glUniformMatrix4fv(gViewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
        glUniformMatrix4fv(gProjLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
        glUniform3f(gDiffuseLocation, 0.1, 0.1, 0.1);
        glUniform3f(gSpecularLocation, 0.1, 0.1, 0.1);
        glUniform3f(gAmbientLocation, 0.3, 0.5, 0.3);
        glUniform1f(gAlphaValueLocation, 0.95f - alphaFactor);

        glDrawArrays(GL_TRIANGLES, 0, gVerticesSphere.size());
    }
    /***************Silicon Proton 3******************/
    /***************Silicon Proton 4******************/
    {
        glm::mat4 model_matrixProton4 = glm::scale(modelMatrix, vec3(0.2f));
        model_matrixProton4 = glm::translate(
            model_matrixProton4,
            glm::vec3(-1.7, 1.0, 0.0));
        glUniformMatrix4fv(gModelLocation, 1, GL_FALSE, glm::value_ptr(model_matrixProton4));
        glUniformMatrix4fv(gViewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
        glUniformMatrix4fv(gProjLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
        glUniform3f(gDiffuseLocation, 0.1, 0.1, 0.1);
        glUniform3f(gSpecularLocation, 0.1, 0.1, 0.1);
        glUniform3f(gAmbientLocation, 0.3, 0.5, 0.3);
        glUniform1f(gAlphaValueLocation, 0.95f - alphaFactor);

        glDrawArrays(GL_TRIANGLES, 0, gVerticesSphere.size());
    }
    /***************Silicon Proton 4******************/
    /***************Silicon Proton 5******************/
    {
        glm::mat4 model_matrixProton5 = glm::scale(modelMatrix, vec3(0.2f));
        model_matrixProton5 = glm::translate(
            model_matrixProton5,
            glm::vec3(-1.7, -1.0, 0.0));
        glUniformMatrix4fv(gModelLocation, 1, GL_FALSE, glm::value_ptr(model_matrixProton5));
        glUniformMatrix4fv(gViewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
        glUniformMatrix4fv(gProjLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
        glUniform3f(gDiffuseLocation, 0.1, 0.1, 0.1);
        glUniform3f(gSpecularLocation, 0.1, 0.1, 0.1);
        glUniform3f(gAmbientLocation, 0.5, 0.3, 0.3);
        glUniform1f(gAlphaValueLocation, 0.95f - alphaFactor);

        glDrawArrays(GL_TRIANGLES, 0, gVerticesSphere.size());
    }
    /***************Silicon Proton 5******************/
    /***************Silicon Proton 6******************/
    {
        glm::mat4 model_matrixProton6 = glm::scale(modelMatrix, vec3(0.2f));
        model_matrixProton6 = glm::translate(
            model_matrixProton6,
            glm::vec3(0.0, -2.0, 0.0));
        glUniformMatrix4fv(gModelLocation, 1, GL_FALSE, glm::value_ptr(model_matrixProton6));
        glUniformMatrix4fv(gViewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
        glUniformMatrix4fv(gProjLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
        glUniform3f(gDiffuseLocation, 0.1, 0.1, 0.1);
        glUniform3f(gSpecularLocation, 0.1, 0.1, 0.1);
        glUniform3f(gAmbientLocation, 0.3, 0.5, 0.3);
        glUniform1f(gAlphaValueLocation, 0.95f - alphaFactor);

        glDrawArrays(GL_TRIANGLES, 0, gVerticesSphere.size());
    }
    /***************Silicon Proton 6******************/
    /***************Silicon Proton 7******************/
    {
        glm::mat4 model_matrixProton7 = glm::scale(modelMatrix, vec3(0.2f));
        model_matrixProton7 = glm::translate(
            model_matrixProton7,
            glm::vec3(1.7, -1.0, 0.0));
        glUniformMatrix4fv(gModelLocation, 1, GL_FALSE, glm::value_ptr(model_matrixProton7));
        glUniformMatrix4fv(gViewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
        glUniformMatrix4fv(gProjLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
        glUniform3f(gDiffuseLocation, 0.1, 0.1, 0.1);
        glUniform3f(gSpecularLocation, 0.1, 0.1, 0.1);
        glUniform3f(gAmbientLocation, 0.5, 0.3, 0.3);
        glUniform1f(gAlphaValueLocation, 0.95f - alphaFactor);

        glDrawArrays(GL_TRIANGLES, 0, gVerticesSphere.size());
    }
    /***************Silicon Proton 7******************/
    /***************Silicon Proton 8******************/
    {
        glm::mat4 model_matrixProton8 = glm::scale(modelMatrix, vec3(0.2f));
        model_matrixProton8 = glm::translate(
            model_matrixProton8,
            glm::vec3(0.8, 1.0, 1.4));
        glUniformMatrix4fv(gModelLocation, 1, GL_FALSE, glm::value_ptr(model_matrixProton8));
        glUniformMatrix4fv(gViewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
        glUniformMatrix4fv(gProjLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
        glUniform3f(gDiffuseLocation, 0.1, 0.1, 0.1);
        glUniform3f(gSpecularLocation, 0.1, 0.1, 0.1);
        glUniform3f(gAmbientLocation, 0.3, 0.5, 0.3);
        glUniform1f(gAlphaValueLocation, 0.95f - alphaFactor);

        glDrawArrays(GL_TRIANGLES, 0, gVerticesSphere.size());
    }
    /***************Silicon Proton 8******************/
    /***************Silicon Proton 9******************/
    {
        glm::mat4 model_matrixProton9 = glm::scale(modelMatrix, vec3(0.2f));
        model_matrixProton9 = glm::translate(
            model_matrixProton9,
            glm::vec3(0.8, 1.0, -1.4));
        glUniformMatrix4fv(gModelLocation, 1, GL_FALSE, glm::value_ptr(model_matrixProton9));
        glUniformMatrix4fv(gViewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
        glUniformMatrix4fv(gProjLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
        glUniform3f(gDiffuseLocation, 0.1, 0.1, 0.1);
        glUniform3f(gSpecularLocation, 0.1, 0.1, 0.1);
        glUniform3f(gAmbientLocation, 0.5, 0.3, 0.3);
        glUniform1f(gAlphaValueLocation, 0.95f - alphaFactor);

        glDrawArrays(GL_TRIANGLES, 0, gVerticesSphere.size());
    }
    /***************Silicon Proton 9******************/
    /***************Silicon Proton 10******************/
    {
        glm::mat4 model_matrixProton10 = glm::scale(modelMatrix, vec3(0.2f));
        model_matrixProton10 = glm::translate(
            model_matrixProton10,
            glm::vec3(-0.8, 1.0, -1.4));
        glUniformMatrix4fv(gModelLocation, 1, GL_FALSE, glm::value_ptr(model_matrixProton10));
        glUniformMatrix4fv(gViewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
        glUniformMatrix4fv(gProjLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
        glUniform3f(gDiffuseLocation, 0.1, 0.1, 0.1);
        glUniform3f(gSpecularLocation, 0.1, 0.1, 0.1);
        glUniform3f(gAmbientLocation, 0.3, 0.5, 0.3);
        glUniform1f(gAlphaValueLocation, 0.95f - alphaFactor);

        glDrawArrays(GL_TRIANGLES, 0, gVerticesSphere.size());
    }
    /***************Silicon Proton 10******************/
    /***************Silicon Proton 11******************/
    {
        glm::mat4 model_matrixProton11 = glm::scale(modelMatrix, vec3(0.2f));
        model_matrixProton11 = glm::translate(
            model_matrixProton11,
            glm::vec3(-0.8, 1.0, 1.4));
        glUniformMatrix4fv(gModelLocation, 1, GL_FALSE, glm::value_ptr(model_matrixProton11));
        glUniformMatrix4fv(gViewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
        glUniformMatrix4fv(gProjLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
        glUniform3f(gDiffuseLocation, 0.1, 0.1, 0.1);
        glUniform3f(gSpecularLocation, 0.1, 0.1, 0.1);
        glUniform3f(gAmbientLocation, 0.5, 0.3, 0.3);
        glUniform1f(gAlphaValueLocation, 0.95f - alphaFactor);

        glDrawArrays(GL_TRIANGLES, 0, gVerticesSphere.size());
    }
    /***************Silicon Proton 11******************/
    /***************Silicon Proton 12******************/
    {
        glm::mat4 model_matrixProton12 = glm::scale(modelMatrix, vec3(0.2f));
        model_matrixProton12 = glm::translate(
            model_matrixProton12,
            glm::vec3(-0.8, -1.0, 1.4));
        glUniformMatrix4fv(gModelLocation, 1, GL_FALSE, glm::value_ptr(model_matrixProton12));
        glUniformMatrix4fv(gViewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
        glUniformMatrix4fv(gProjLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
        glUniform3f(gDiffuseLocation, 0.1, 0.1, 0.1);
        glUniform3f(gSpecularLocation, 0.1, 0.1, 0.1);
        glUniform3f(gAmbientLocation, 0.5, 0.3, 0.3);
        glUniform1f(gAlphaValueLocation, 0.95f - alphaFactor);

        glDrawArrays(GL_TRIANGLES, 0, gVerticesSphere.size());
    }
    /***************Silicon Proton 12******************/
    /***************Silicon Proton 13******************/
    {
        glm::mat4 model_matrixProton13 = glm::scale(modelMatrix, vec3(0.2f));
        model_matrixProton13 = glm::translate(
            model_matrixProton13,
            glm::vec3(0.8, -1.0, 1.4));
        glUniformMatrix4fv(gModelLocation, 1, GL_FALSE, glm::value_ptr(model_matrixProton13));
        glUniformMatrix4fv(gViewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
        glUniformMatrix4fv(gProjLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
        glUniform3f(gDiffuseLocation, 0.1, 0.1, 0.1);
        glUniform3f(gSpecularLocation, 0.1, 0.1, 0.1);
        glUniform3f(gAmbientLocation, 0.5, 0.3, 0.3);
        glUniform1f(gAlphaValueLocation, 0.95f - alphaFactor);

        glDrawArrays(GL_TRIANGLES, 0, gVerticesSphere.size());
    }
    /***************Silicon Proton 13******************/
    /***************Silicon Proton 14******************/
    {
        glm::mat4 model_matrixProton14 = glm::scale(modelMatrix, vec3(0.2f));
        model_matrixProton14 = glm::translate(
            model_matrixProton14,
            glm::vec3(0.8, -1.0, -1.4));
        glUniformMatrix4fv(gModelLocation, 1, GL_FALSE, glm::value_ptr(model_matrixProton14));
        glUniformMatrix4fv(gViewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
        glUniformMatrix4fv(gProjLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
        glUniform3f(gDiffuseLocation, 0.1, 0.1, 0.1);
        glUniform3f(gSpecularLocation, 0.1, 0.1, 0.1);
        glUniform3f(gAmbientLocation, 0.3, 0.5, 0.3);
        glUniform1f(gAlphaValueLocation, 0.95f - alphaFactor);

        glDrawArrays(GL_TRIANGLES, 0, gVerticesSphere.size());
    }
    /***************Silicon Proton 14******************/
    /***************Silicon Proton 15******************/
    {
        glm::mat4 model_matrixProton15 = glm::scale(modelMatrix, vec3(0.2f));
        model_matrixProton15 = glm::translate(
            model_matrixProton15,
            glm::vec3(-0.8, -1.0, -1.4));
        glUniformMatrix4fv(gModelLocation, 1, GL_FALSE, glm::value_ptr(model_matrixProton15));
        glUniformMatrix4fv(gViewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
        glUniformMatrix4fv(gProjLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
        glUniform3f(gDiffuseLocation, 0.1, 0.1, 0.1);
        glUniform3f(gSpecularLocation, 0.1, 0.1, 0.1);
        glUniform3f(gAmbientLocation, 0.3, 0.5, 0.3);
        glUniform1f(gAlphaValueLocation, 0.95f - alphaFactor);

        glDrawArrays(GL_TRIANGLES, 0, gVerticesSphere.size());
    }
    /***************Silicon Proton 15******************/
    /***************Silicon Nucleus******************/
    {
        glUniformMatrix4fv(gModelLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
        glUniformMatrix4fv(gViewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
        glUniformMatrix4fv(gProjLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
        glUniform3f(gDiffuseLocation, 0.1, 0.1, 0.1);
        glUniform3f(gSpecularLocation, 0.1, 0.1, 0.1);
        glUniform3f(gAmbientLocation, 0.1, 0.3, 0.1);
        glUniform1f(gAlphaValueLocation, 0.65f - alphaFactor);

        glDrawArrays(GL_TRIANGLES, 0, gVerticesSphere.size());
    }
    /***************Silicon Nucleus******************/

    /************** Electron 1 ********************/
    static float theta11 = 0.0f;
    {
        glm::mat4 model_matrixElectron11 = glm::mat4(1.0f);

        model_matrixElectron11 = glm::translate(
            modelMatrix,
            glm::vec3(1.5f * cos(theta11), (1.5f * sin(theta11)), 0.0));

        model_matrixElectron11 = glm::scale(model_matrixElectron11, vec3(0.2f));

        glUniformMatrix4fv(gModelLocation, 1, GL_FALSE, glm::value_ptr(model_matrixElectron11));
        glUniformMatrix4fv(gViewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
        glUniformMatrix4fv(gProjLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
        glUniform3f(gDiffuseLocation, 0.4, 0.3, 0.3);
        glUniform3f(gSpecularLocation, 0.4, 0.3, 0.3);
        glUniform3f(gAmbientLocation, 0.6, 0.6, 0.6);
        glUniform1f(gAlphaValueLocation, 1.0f -alphaFactor);

        glDrawArrays(GL_TRIANGLES, 0, gVerticesSphere.size());
    }
    /************** Electron 1 ********************/

    /************** Electron 2 ********************/
    static float beta12 = 90.0f;
    {
        glm::mat4 model_matrixElectron12 = glm::mat4(1.0f);

        model_matrixElectron12 = glm::translate(
            modelMatrix,
            glm::vec3(1.5f * cos(beta12), 0.0, (1.5f * sin(beta12))));

        model_matrixElectron12 = glm::scale(model_matrixElectron12, vec3(0.2f));

        glUniformMatrix4fv(gModelLocation, 1, GL_FALSE, glm::value_ptr(model_matrixElectron12));
        glUniformMatrix4fv(gViewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
        glUniformMatrix4fv(gProjLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
        glUniform3f(gDiffuseLocation, 0.4, 0.3, 0.3);
        glUniform3f(gSpecularLocation, 0.4, 0.3, 0.3);
        glUniform3f(gAmbientLocation, 0.6, 0.6, 0.6);
        glUniform1f(gAlphaValueLocation, 1.0f - alphaFactor);

        glDrawArrays(GL_TRIANGLES, 0, gVerticesSphere.size());
    }
    /************** Electron 2 ********************/

    /************** Electron 3 ********************/
    static float alpha21 = 100.0f;
    {
        glm::mat4 model_matrixElectron21 = glm::mat4(1.0f);

        model_matrixElectron21 = glm::translate(
            modelMatrix,
            glm::vec3(2.3f * cos(alpha21), 0.0, (2.3f * sin(alpha21))));

        model_matrixElectron21 = glm::scale(model_matrixElectron21, vec3(0.2f));

        glUniformMatrix4fv(gModelLocation, 1, GL_FALSE, glm::value_ptr(model_matrixElectron21));
        glUniformMatrix4fv(gViewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
        glUniformMatrix4fv(gProjLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
        glUniform3f(gDiffuseLocation, 0.42, 0.3, 0.3);
        glUniform3f(gSpecularLocation, 0.42, 0.8, 0.8);
        glUniform3f(gAmbientLocation, 0.7, 0.6, 0.6);
        glUniform1f(gAlphaValueLocation, 1.0f - alphaFactor);

        glDrawArrays(GL_TRIANGLES, 0, gVerticesSphere.size());
    }
    /************** Electron 3 ********************/

    /************** Electron 4 ********************/
    static float gamma22 = 10.0f;
    {
        glm::mat4 model_matrixElectron22 = glm::mat4(1.0f);

        model_matrixElectron22 = glm::rotate(
            modelMatrix,
            45.0f,
            glm::vec3(0.0, 0.0, 1.0));

        model_matrixElectron22 = glm::translate(
            model_matrixElectron22,
            glm::vec3((2.3f * sin(gamma22)), 0.0, (2.3f * cos(gamma22))));

        model_matrixElectron22 = glm::scale(model_matrixElectron22, vec3(0.2f));

        glUniformMatrix4fv(gModelLocation, 1, GL_FALSE, glm::value_ptr(model_matrixElectron22));
        glUniformMatrix4fv(gViewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
        glUniformMatrix4fv(gProjLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
        glUniform3f(gDiffuseLocation, 0.42, 0.3, 0.3);
        glUniform3f(gSpecularLocation, 0.42, 0.3, 0.3);
        glUniform3f(gAmbientLocation, 0.7, 0.6, 0.6);
        glUniform1f(gAlphaValueLocation, 1.0f - alphaFactor);

        glDrawArrays(GL_TRIANGLES, 0, gVerticesSphere.size());
    }
    /************** Electron 4 ********************/

    /************** Electron 5 ********************/
    static float omega23 = 50.0f;
    {
        glm::mat4 model_matrixElectron23 = glm::mat4(1.0f);

        model_matrixElectron23 = glm::rotate(
            modelMatrix,
            45.0f,
            glm::vec3(0.0, 0.0, -1.0));

        model_matrixElectron23 = glm::translate(
            model_matrixElectron23,
            glm::vec3((2.3f * sin(omega23)), 0.0, (2.3f * cos(omega23))));

        model_matrixElectron23 = glm::scale(model_matrixElectron23, vec3(0.2f));

        glUniformMatrix4fv(gModelLocation, 1, GL_FALSE, glm::value_ptr(model_matrixElectron23));
        glUniformMatrix4fv(gViewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
        glUniformMatrix4fv(gProjLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
        glUniform3f(gDiffuseLocation, 0.42, 0.3, 0.3);
        glUniform3f(gSpecularLocation, 0.42, 0.3, 0.3);
        glUniform3f(gAmbientLocation, 0.7, 0.6, 0.6);
        glUniform1f(gAlphaValueLocation, 1.0f - alphaFactor);

        glDrawArrays(GL_TRIANGLES, 0, gVerticesSphere.size());
    }
    /************** Electron 5 ********************/

    /************** Electron 6 ********************/
    static float delta24 = 90.0f;
    {
        glm::mat4 model_matrixElectron24 = glm::mat4(1.0f);

        model_matrixElectron24 = glm::translate(
            modelMatrix,
            glm::vec3((2.3f * sin(delta24)), (2.3f * cos(delta24)), 0.0));

        model_matrixElectron24 = glm::scale(model_matrixElectron24, vec3(0.2f));

        glUniformMatrix4fv(gModelLocation, 1, GL_FALSE, glm::value_ptr(model_matrixElectron24));
        glUniformMatrix4fv(gViewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
        glUniformMatrix4fv(gProjLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
        glUniform3f(gDiffuseLocation, 0.42, 0.3, 0.3);
        glUniform3f(gSpecularLocation, 0.42, 0.3, 0.3);
        glUniform3f(gAmbientLocation, 0.7, 0.6, 0.6);
        glUniform1f(gAlphaValueLocation, 1.0f - alphaFactor);

        glDrawArrays(GL_TRIANGLES, 0, gVerticesSphere.size());
    }
    /************** Electron 6 ********************/

    /************** Electron 7 ********************/
    static float epsilon25 = 120.0f;
    {
        glm::mat4 model_matrixElectron25 = glm::mat4(1.0f);

        model_matrixElectron25 = glm::translate(
            modelMatrix,
            glm::vec3(0.0, (2.3f * sin(epsilon25)), (2.3f * cos(epsilon25))));

        model_matrixElectron25 = glm::scale(model_matrixElectron25, vec3(0.2f));

        glUniformMatrix4fv(gModelLocation, 1, GL_FALSE, glm::value_ptr(model_matrixElectron25));
        glUniformMatrix4fv(gViewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
        glUniformMatrix4fv(gProjLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
        glUniform3f(gDiffuseLocation, 0.42, 0.3, 0.3);
        glUniform3f(gSpecularLocation, 0.42, 0.3, 0.3);
        glUniform3f(gAmbientLocation, 0.7, 0.6, 0.6);
        glUniform1f(gAlphaValueLocation, 1.0f - alphaFactor);

        glDrawArrays(GL_TRIANGLES, 0, gVerticesSphere.size());
    }
    /************** Electron 7 ********************/

    /************** Electron 8 ********************/
    static float zeta26 = 150.0f;
    {
        glm::mat4 model_matrixElectron26 = glm::mat4(1.0f);

        model_matrixElectron26 = glm::rotate(
            modelMatrix,
            67.0f,
            glm::vec3(0.0, 0.0, -1.0));

        model_matrixElectron26 = glm::translate(
            model_matrixElectron26,
            glm::vec3(0.0, (2.3f * sin(zeta26)), (2.3f * cos(zeta26))));

        model_matrixElectron26 = glm::scale(model_matrixElectron26, vec3(0.2f));


        glUniformMatrix4fv(gModelLocation, 1, GL_FALSE, glm::value_ptr(model_matrixElectron26));
        glUniformMatrix4fv(gViewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
        glUniformMatrix4fv(gProjLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
        glUniform3f(gDiffuseLocation, 0.42, 0.3, 0.3);
        glUniform3f(gSpecularLocation, 0.42, 0.3, 0.3);
        glUniform3f(gAmbientLocation, 0.7, 0.6, 0.6);
        glUniform1f(gAlphaValueLocation, 1.0f - alphaFactor);

        glDrawArrays(GL_TRIANGLES, 0, gVerticesSphere.size());
    }
    /************** Electron 8 ********************/

    /************** Electron 9 ********************/
    static float eta27 = 160.0f;
    {
        glm::mat4 model_matrixElectron27 = glm::mat4(1.0f);

        model_matrixElectron27 = glm::rotate(
            modelMatrix,
            -67.0f,
            glm::vec3(0.0, 0.0, -1.0));

        model_matrixElectron27 = glm::translate(
            model_matrixElectron27,
            glm::vec3(0.0, (2.3f * sin(eta27)), (2.3f * cos(eta27))));

        model_matrixElectron27 = glm::scale(model_matrixElectron27, vec3(0.2f));


        glUniformMatrix4fv(gModelLocation, 1, GL_FALSE, glm::value_ptr(model_matrixElectron27));
        glUniformMatrix4fv(gViewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
        glUniformMatrix4fv(gProjLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
        glUniform3f(gDiffuseLocation, 0.42, 0.3, 0.3);
        glUniform3f(gSpecularLocation, 0.42, 0.3, 0.3);
        glUniform3f(gAmbientLocation, 0.7, 0.6, 0.6);
        glUniform1f(gAlphaValueLocation, 1.0f - alphaFactor);

        glDrawArrays(GL_TRIANGLES, 0, gVerticesSphere.size());
    }
    /************** Electron 9 ********************/

    /************** Electron 10 ********************/
    static float kappa28 = 39.0f;
    {
        glm::mat4 model_matrixElectron28 = glm::mat4(1.0f);

        model_matrixElectron28 = glm::rotate(
            modelMatrix,
            25.0f,
            glm::vec3(0.0, 0.0, -1.0));

        model_matrixElectron28 = glm::translate(
            model_matrixElectron28,
            glm::vec3(0.0, (2.3f * sin(kappa28)), (2.3f * cos(kappa28))));

        model_matrixElectron28 = glm::scale(model_matrixElectron28, vec3(0.2f));


        glUniformMatrix4fv(gModelLocation, 1, GL_FALSE, glm::value_ptr(model_matrixElectron28));
        glUniformMatrix4fv(gViewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
        glUniformMatrix4fv(gProjLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
        glUniform3f(gDiffuseLocation, 0.42, 0.3, 0.3);
        glUniform3f(gSpecularLocation, 0.42, 0.3, 0.3);
        glUniform3f(gAmbientLocation, 0.7, 0.6, 0.6);
        glUniform1f(gAlphaValueLocation, 1.0f - alphaFactor);

        glDrawArrays(GL_TRIANGLES, 0, gVerticesSphere.size());
    }
    /************** Electron 10 ********************/

    /************** Electron 11 ********************/
    static float toggle = 0.01f;
    static float lambda31 = 59.0f;
    {
        glm::mat4 model_matrixElectron31 = glm::mat4(1.0f);

        if (!ifMove)
        {
            model_matrixElectron31 = glm::translate(
                modelMatrix,
                glm::vec3(0.0, (2.8f * sin(lambda31)), (2.8f * cos(lambda31))));
        }
        else
        {
            if (toggle > 0.0)
            {
                toggle = -0.04f;
            }
            else
            {
                toggle = 0.04f;
            }
            if (count > 2200)
            {
                toggle = toggle / 5;
                model_matrixElectron31 = glm::translate(
                    glm::mat4(1.0f),
                    glm::vec3(toggle, 0.3, 0.0));
            }
            else
            {
                model_matrixElectron31 = glm::translate(
                    modelMatrix,
                    glm::vec3((2.8f * sin(lambda31)), toggle, (2.8f * cos(lambda31))));
            }
        }

        model_matrixElectron31 = glm::scale(model_matrixElectron31, vec3(0.2f));
        
        glUniformMatrix4fv(gModelLocation, 1, GL_FALSE, glm::value_ptr(model_matrixElectron31));
        glUniformMatrix4fv(gViewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
        glUniformMatrix4fv(gProjLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
        glUniform3f(gDiffuseLocation, 1.0, 0.8, 0.8);
        glUniform3f(gSpecularLocation, 1.0, 0.8, 0.8);
        glUniform3f(gAmbientLocation, 0.9, 0.6, 0.6);
        glUniform1f(gAlphaValueLocation, 1.0f - alphaFactor);

        glDrawArrays(GL_TRIANGLES, 0, gVerticesSphere.size());
    }
    /************** Electron 11 ********************/

    /************** Electron 12 ********************/
    static float mu32 = 0.0f;
    {
        glm::mat4 model_matrixElectron32 = glm::mat4(1.0f);

        if (!ifMove)
        {
            model_matrixElectron32 = glm::translate(
                modelMatrix,
                glm::vec3((2.8f * sin(mu32)), 0.0, (2.8f * cos(mu32))));
        }
        else if (count > 2200)
        {
            model_matrixElectron32 = glm::translate(
                glm::mat4(1.0f),
                glm::vec3(toggle, -0.3, 0.0));
        }
        else
        {
           model_matrixElectron32 = glm::translate(
                modelMatrix,
                glm::vec3((2.8f * sin(mu32)), toggle, (2.8f * cos(mu32))));
        }
        model_matrixElectron32 = glm::scale(model_matrixElectron32, vec3(0.2f));


        glUniformMatrix4fv(gModelLocation, 1, GL_FALSE, glm::value_ptr(model_matrixElectron32));
        glUniformMatrix4fv(gViewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
        glUniformMatrix4fv(gProjLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
        glUniform3f(gDiffuseLocation, 1.0, 0.8, 0.8);
        glUniform3f(gSpecularLocation, 1.0, 0.8, 0.8);
        glUniform3f(gAmbientLocation, 0.9, 0.6, 0.6);
        glUniform1f(gAlphaValueLocation, 1.0f - alphaFactor);

        glDrawArrays(GL_TRIANGLES, 0, gVerticesSphere.size());
    }
    /************** Electron 12 ********************/

    /************** Electron 13 ********************/
    static float mu33 = 45.0f;
    {
        glm::mat4 model_matrixElectron33 = glm::mat4(1.0f);

        model_matrixElectron33 = glm::rotate(
            modelMatrix,
            45.0f,
            glm::vec3(0.0, 0.0, 1.0));

        if (!ifMove)
        {
            model_matrixElectron33 = glm::translate(
                model_matrixElectron33,
                glm::vec3((2.8f * sin(mu33)), 0.0, (2.8f * cos(mu33))));
        }
        else if (count > 2200)
        {
            model_matrixElectron33 = glm::translate(
                glm::mat4(1.0f),
                glm::vec3(0.0, toggle, 0.0));
        }
        else
        {
            model_matrixElectron33 = glm::translate(
                model_matrixElectron33,
                glm::vec3((2.8f * sin(mu33)), toggle, (2.8f * cos(mu33))));
        }

        model_matrixElectron33 = glm::scale(model_matrixElectron33, vec3(0.2f));

        glUniformMatrix4fv(gModelLocation, 1, GL_FALSE, glm::value_ptr(model_matrixElectron33));
        glUniformMatrix4fv(gViewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
        glUniformMatrix4fv(gProjLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
        glUniform3f(gDiffuseLocation, 1.0, 0.8, 0.8);
        glUniform3f(gSpecularLocation, 1.0, 0.8, 0.8);
        glUniform3f(gAmbientLocation, 0.9, 0.6, 0.6);
        glUniform1f(gAlphaValueLocation, 1.0f - alphaFactor);

        glDrawArrays(GL_TRIANGLES, 0, gVerticesSphere.size());
    }
    /************** Electron 13 ********************/

    /************** Electron 14 ********************/
    static float mu34 = -45.0f;
    {
        glm::mat4 model_matrixElectron34 = glm::mat4(1.0f);

        model_matrixElectron34 = glm::rotate(
            modelMatrix,
            45.0f,
            glm::vec3(0.0, 0.0, -1.0));

        if (!ifMove)
        {
            model_matrixElectron34 = glm::translate(
                model_matrixElectron34,
                glm::vec3((2.8f * sin(mu34)), 0.0, (2.8f * cos(mu34))));
        }
        else if (count > 2200)
        {
            model_matrixElectron34 = glm::translate(
                glm::mat4(1.0f),
                glm::vec3(0.0, toggle, 0.0));
        }
        else
        {
            model_matrixElectron34 = glm::translate(
                model_matrixElectron34,
                glm::vec3((2.8f * sin(mu34)), toggle, (2.8f * cos(mu34))));
        }
        model_matrixElectron34 = glm::scale(model_matrixElectron34, vec3(0.2f));


        glUniformMatrix4fv(gModelLocation, 1, GL_FALSE, glm::value_ptr(model_matrixElectron34));
        glUniformMatrix4fv(gViewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
        glUniformMatrix4fv(gProjLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
        glUniform3f(gDiffuseLocation, 1.0, 0.8, 0.8);
        glUniform3f(gSpecularLocation, 1.0, 0.8, 0.8);
        glUniform3f(gAmbientLocation, 0.9, 0.6, 0.6);
        glUniform1f(gAlphaValueLocation, 1.0f - alphaFactor);

        glDrawArrays(GL_TRIANGLES, 0, gVerticesSphere.size());
    }
    /************** Electron 14 ********************/
    /************** Electron 15 ********************/
    static float mu35 = -70.0f;
    {
        glm::mat4 model_matrixElectron35 = glm::mat4(1.0f);

        model_matrixElectron35 = glm::rotate(
            modelMatrix,
            70.0f,
            glm::vec3(-1.0, 0.0, 0.0));

        if (!ifMove)
        {
            model_matrixElectron35 = glm::translate(
                model_matrixElectron35,
                glm::vec3((2.8f * sin(mu35)), 0.0, (2.8f * cos(mu35))));
        }
        else if (alphaFactor >= 0.88f)
        {
            model_matrixElectron35 = glm::translate(
                glm::mat4(1.0f),
                glm::vec3(0.0, toggle * 5, -0.2));
        }
        else
        {
            model_matrixElectron35 = glm::translate(
                model_matrixElectron35,
                glm::vec3((2.8f * sin(mu35)), toggle * 5, (2.8f * cos(mu35))));
        }
        model_matrixElectron35 = glm::scale(model_matrixElectron35, vec3(0.2f));


        glUniformMatrix4fv(gModelLocation, 1, GL_FALSE, glm::value_ptr(model_matrixElectron35));
        glUniformMatrix4fv(gViewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
        glUniformMatrix4fv(gProjLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
        glUniform3f(gDiffuseLocation, 1.0, 0.8, 0.8);
        glUniform3f(gSpecularLocation, 1.0, 0.8, 0.8);
        glUniform3f(gAmbientLocation, 0.9, 0.6, 0.6);
        glUniform1f(gAlphaValueLocation, 1.0f);

        glDrawArrays(GL_TRIANGLES, 0, gVerticesSphere.size());
    }
    /************** Electron 15 ********************/
    if (count > 2300)
    {
        if(alphaFactor < 1.0f)
            alphaFactor += 0.001f;
    }
    theta11 += DELTA_FIRST_ORBIT;
    if (theta11 >= 360)
        theta11 = 0.0;

    beta12 += DELTA_FIRST_ORBIT;
    if (beta12 >= 360)
        beta12 = 0.0;

    alpha21 += DELTA_SECOND_ORBIT;
    if (alpha21 >= 360)
        alpha21 = 0.0;

    gamma22 += DELTA_SECOND_ORBIT;
    if (gamma22 >= 360)
        gamma22 = 0.0;

    omega23 += DELTA_SECOND_ORBIT;
    if (omega23 >= 360)
        omega23 = 0.0;

    delta24 += DELTA_SECOND_ORBIT;
    if (delta24 >= 360)
        delta24 = 0.0;

    epsilon25 += DELTA_SECOND_ORBIT;
    if (epsilon25 >= 360)
        epsilon25 = 0.0;

    zeta26 += DELTA_SECOND_ORBIT;
    if (zeta26 >= 360)
        zeta26 = 0.0;

    eta27 += DELTA_SECOND_ORBIT;
    if (eta27 >= 360)
        eta27 = 0.0;

    kappa28 += DELTA_SECOND_ORBIT;
    if (kappa28 >= 360)
        kappa28 = 0.0;

    lambda31 += DELTA_THIRD_ORBIT;
    if (lambda31 >= 360)
        lambda31 = 0.0;

    mu32 += DELTA_THIRD_ORBIT;
    if (mu32 >= 360)
        mu32 = 0.0;

    mu33 += DELTA_THIRD_ORBIT;
    if (mu32 >= 360)
        mu32 = 0.0;

    mu34 += DELTA_THIRD_ORBIT;
    if (mu34 >= 360)
        mu34 = 0.0;

    mu35 += DELTA_THIRD_ORBIT;
    if (mu35 >= 360)
        mu35 = 0.0;
}

void RenderSiliconAtom(glm::mat4 modelMatrix, 
    glm::mat4 viewMatrix,
    BOOL ifMove)
{
    static GLfloat alphaFactor = 0.0f;
    /***************Silicon Proton 1******************/
    glm::mat4 model_matrixProton1 = glm::scale(modelMatrix, vec3(0.2f));
    glUniformMatrix4fv(gModelLocation, 1, GL_FALSE, glm::value_ptr(model_matrixProton1));
    glUniformMatrix4fv(gViewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(gProjLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniform3f(gDiffuseLocation, 0.1, 0.1, 0.1);
    glUniform3f(gSpecularLocation, 0.1, 0.1, 0.1);
    glUniform3f(gAmbientLocation, 0.5, 0.3, 0.3);
    glUniform1f(gAlphaValueLocation, 0.95f - alphaFactor);

    glDrawArrays(GL_TRIANGLES, 0, gVerticesSphere.size());
    /***************Silicon Proton 1******************/
    /***************Silicon Proton 2******************/
    glm::mat4 model_matrixProton2 = glm::scale(modelMatrix, vec3(0.2f));
    model_matrixProton2 = glm::translate(
        model_matrixProton2,
        glm::vec3(0.0, 2.0, 0.0));
    glUniformMatrix4fv(gModelLocation, 1, GL_FALSE, glm::value_ptr(model_matrixProton2));
    glUniformMatrix4fv(gViewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(gProjLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniform3f(gDiffuseLocation, 0.1, 0.1, 0.1);
    glUniform3f(gSpecularLocation, 0.1, 0.1, 0.1);
    glUniform3f(gAmbientLocation, 0.5, 0.3, 0.3);
    glUniform1f(gAlphaValueLocation, 0.95f - alphaFactor);

    glDrawArrays(GL_TRIANGLES, 0, gVerticesSphere.size());
    /***************Silicon Proton 2******************/
    /***************Silicon Proton 3******************/
    glm::mat4 model_matrixProton3 = glm::scale(modelMatrix, vec3(0.2f));
    model_matrixProton3 = glm::translate(
        model_matrixProton3,
        glm::vec3(1.7, 1.0, 0.0));
    glUniformMatrix4fv(gModelLocation, 1, GL_FALSE, glm::value_ptr(model_matrixProton3));
    glUniformMatrix4fv(gViewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(gProjLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniform3f(gDiffuseLocation, 0.1, 0.1, 0.1);
    glUniform3f(gSpecularLocation, 0.1, 0.1, 0.1);
    glUniform3f(gAmbientLocation, 0.3, 0.5, 0.3);
    glUniform1f(gAlphaValueLocation, 0.95f - alphaFactor);

    glDrawArrays(GL_TRIANGLES, 0, gVerticesSphere.size());
    /***************Silicon Proton 3******************/
    /***************Silicon Proton 4******************/
    glm::mat4 model_matrixProton4 = glm::scale(modelMatrix, vec3(0.2f));
    model_matrixProton4 = glm::translate(
        model_matrixProton4,
        glm::vec3(-1.7, 1.0, 0.0));
    glUniformMatrix4fv(gModelLocation, 1, GL_FALSE, glm::value_ptr(model_matrixProton4));
    glUniformMatrix4fv(gViewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(gProjLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniform3f(gDiffuseLocation, 0.1, 0.1, 0.1);
    glUniform3f(gSpecularLocation, 0.1, 0.1, 0.1);
    glUniform3f(gAmbientLocation, 0.3, 0.5, 0.3);
    glUniform1f(gAlphaValueLocation, 0.95f - alphaFactor);

    glDrawArrays(GL_TRIANGLES, 0, gVerticesSphere.size());
    /***************Silicon Proton 4******************/
    /***************Silicon Proton 5******************/
    glm::mat4 model_matrixProton5 = glm::scale(modelMatrix, vec3(0.2f));
    model_matrixProton5 = glm::translate(
        model_matrixProton5,
        glm::vec3(-1.7, -1.0, 0.0));
    glUniformMatrix4fv(gModelLocation, 1, GL_FALSE, glm::value_ptr(model_matrixProton5));
    glUniformMatrix4fv(gViewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(gProjLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniform3f(gDiffuseLocation, 0.1, 0.1, 0.1);
    glUniform3f(gSpecularLocation, 0.1, 0.1, 0.1);
    glUniform3f(gAmbientLocation, 0.5, 0.3, 0.3);
    glUniform1f(gAlphaValueLocation, 0.95f - alphaFactor);

    glDrawArrays(GL_TRIANGLES, 0, gVerticesSphere.size());
    /***************Silicon Proton 5******************/
    /***************Silicon Proton 6******************/
    glm::mat4 model_matrixProton6 = glm::scale(modelMatrix, vec3(0.2f));
    model_matrixProton6 = glm::translate(
        model_matrixProton6,
        glm::vec3(0.0, -2.0, 0.0));
    glUniformMatrix4fv(gModelLocation, 1, GL_FALSE, glm::value_ptr(model_matrixProton6));
    glUniformMatrix4fv(gViewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(gProjLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniform3f(gDiffuseLocation, 0.1, 0.1, 0.1);
    glUniform3f(gSpecularLocation, 0.1, 0.1, 0.1);
    glUniform3f(gAmbientLocation, 0.3, 0.5, 0.3);
    glUniform1f(gAlphaValueLocation, 0.95f - alphaFactor);

    glDrawArrays(GL_TRIANGLES, 0, gVerticesSphere.size());
    /***************Silicon Proton 6******************/
    /***************Silicon Proton 7******************/
    glm::mat4 model_matrixProton7 = glm::scale(modelMatrix, vec3(0.2f));
    model_matrixProton7 = glm::translate(
        model_matrixProton7,
        glm::vec3(1.7, -1.0, 0.0));
    glUniformMatrix4fv(gModelLocation, 1, GL_FALSE, glm::value_ptr(model_matrixProton7));
    glUniformMatrix4fv(gViewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(gProjLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniform3f(gDiffuseLocation, 0.1, 0.1, 0.1);
    glUniform3f(gSpecularLocation, 0.1, 0.1, 0.1);
    glUniform3f(gAmbientLocation, 0.5, 0.3, 0.3);
    glUniform1f(gAlphaValueLocation, 0.95f - alphaFactor);

    glDrawArrays(GL_TRIANGLES, 0, gVerticesSphere.size());
    /***************Silicon Proton 7******************/
    /***************Silicon Proton 8******************/
    glm::mat4 model_matrixProton8 = glm::scale(modelMatrix, vec3(0.2f));
    model_matrixProton8 = glm::translate(
        model_matrixProton8,
        glm::vec3(0.8, 1.0, 1.4));
    glUniformMatrix4fv(gModelLocation, 1, GL_FALSE, glm::value_ptr(model_matrixProton8));
    glUniformMatrix4fv(gViewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(gProjLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniform3f(gDiffuseLocation, 0.1, 0.1, 0.1);
    glUniform3f(gSpecularLocation, 0.1, 0.1, 0.1);
    glUniform3f(gAmbientLocation, 0.3, 0.5, 0.3);
    glUniform1f(gAlphaValueLocation, 0.95f - alphaFactor);

    glDrawArrays(GL_TRIANGLES, 0, gVerticesSphere.size());
    /***************Silicon Proton 8******************/
    /***************Silicon Proton 9******************/
    glm::mat4 model_matrixProton9 = glm::scale(modelMatrix, vec3(0.2f));
    model_matrixProton9 = glm::translate(
        model_matrixProton9,
        glm::vec3(0.8, 1.0, -1.4));
    glUniformMatrix4fv(gModelLocation, 1, GL_FALSE, glm::value_ptr(model_matrixProton9));
    glUniformMatrix4fv(gViewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(gProjLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniform3f(gDiffuseLocation, 0.1, 0.1, 0.1);
    glUniform3f(gSpecularLocation, 0.1, 0.1, 0.1);
    glUniform3f(gAmbientLocation, 0.5, 0.3, 0.3);
    glUniform1f(gAlphaValueLocation, 0.95f - alphaFactor);

    glDrawArrays(GL_TRIANGLES, 0, gVerticesSphere.size());
    /***************Silicon Proton 9******************/
    /***************Silicon Proton 10******************/
    glm::mat4 model_matrixProton10 = glm::scale(modelMatrix, vec3(0.2f));
    model_matrixProton10 = glm::translate(
        model_matrixProton10,
        glm::vec3(-0.8, 1.0, -1.4));
    glUniformMatrix4fv(gModelLocation, 1, GL_FALSE, glm::value_ptr(model_matrixProton10));
    glUniformMatrix4fv(gViewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(gProjLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniform3f(gDiffuseLocation, 0.1, 0.1, 0.1);
    glUniform3f(gSpecularLocation, 0.1, 0.1, 0.1);
    glUniform3f(gAmbientLocation, 0.3, 0.5, 0.3);
    glUniform1f(gAlphaValueLocation, 0.95f - alphaFactor);

    glDrawArrays(GL_TRIANGLES, 0, gVerticesSphere.size());
    /***************Silicon Proton 10******************/
    /***************Silicon Proton 11******************/
    glm::mat4 model_matrixProton11 = glm::scale(modelMatrix, vec3(0.2f));
    model_matrixProton11 = glm::translate(
        model_matrixProton11,
        glm::vec3(-0.8, 1.0, 1.4));
    glUniformMatrix4fv(gModelLocation, 1, GL_FALSE, glm::value_ptr(model_matrixProton11));
    glUniformMatrix4fv(gViewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(gProjLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniform3f(gDiffuseLocation, 0.1, 0.1, 0.1);
    glUniform3f(gSpecularLocation, 0.1, 0.1, 0.1);
    glUniform3f(gAmbientLocation, 0.5, 0.3, 0.3);
    glUniform1f(gAlphaValueLocation, 0.95f - alphaFactor);

    glDrawArrays(GL_TRIANGLES, 0, gVerticesSphere.size());
    /***************Silicon Proton 11******************/
    /***************Silicon Proton 12******************/
    glm::mat4 model_matrixProton12 = glm::scale(modelMatrix, vec3(0.2f));
    model_matrixProton12 = glm::translate(
        model_matrixProton12,
        glm::vec3(-0.8, -1.0, 1.4));
    glUniformMatrix4fv(gModelLocation, 1, GL_FALSE, glm::value_ptr(model_matrixProton12));
    glUniformMatrix4fv(gViewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(gProjLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniform3f(gDiffuseLocation, 0.1, 0.1, 0.1);
    glUniform3f(gSpecularLocation, 0.1, 0.1, 0.1);
    glUniform3f(gAmbientLocation, 0.5, 0.3, 0.3);
    glUniform1f(gAlphaValueLocation, 0.95f - alphaFactor);

    glDrawArrays(GL_TRIANGLES, 0, gVerticesSphere.size());
    /***************Silicon Proton 12******************/
    /***************Silicon Proton 13******************/
    glm::mat4 model_matrixProton13 = glm::scale(modelMatrix, vec3(0.2f));
    model_matrixProton13 = glm::translate(
        model_matrixProton13,
        glm::vec3(0.8, -1.0, 1.4));
    glUniformMatrix4fv(gModelLocation, 1, GL_FALSE, glm::value_ptr(model_matrixProton13));
    glUniformMatrix4fv(gViewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(gProjLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniform3f(gDiffuseLocation, 0.1, 0.1, 0.1);
    glUniform3f(gSpecularLocation, 0.1, 0.1, 0.1);
    glUniform3f(gAmbientLocation, 0.5, 0.3, 0.3);
    glUniform1f(gAlphaValueLocation, 0.95f - alphaFactor);

    glDrawArrays(GL_TRIANGLES, 0, gVerticesSphere.size());
    /***************Silicon Proton 13******************/
    /***************Silicon Proton 14******************/
    glm::mat4 model_matrixProton14 = glm::scale(modelMatrix, vec3(0.2f));
    model_matrixProton14 = glm::translate(
        model_matrixProton14,
        glm::vec3(0.8, -1.0, -1.4));
    glUniformMatrix4fv(gModelLocation, 1, GL_FALSE, glm::value_ptr(model_matrixProton14));
    glUniformMatrix4fv(gViewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(gProjLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniform3f(gDiffuseLocation, 0.1, 0.1, 0.1);
    glUniform3f(gSpecularLocation, 0.1, 0.1, 0.1);
    glUniform3f(gAmbientLocation, 0.3, 0.5, 0.3);
    glUniform1f(gAlphaValueLocation, 0.95f - alphaFactor);

    glDrawArrays(GL_TRIANGLES, 0, gVerticesSphere.size());
    /***************Silicon Proton 14******************/
    /***************Silicon Proton 15******************/
    glm::mat4 model_matrixProton15 = glm::scale(modelMatrix, vec3(0.2f));
    model_matrixProton15 = glm::translate(
        model_matrixProton15,
        glm::vec3(-0.8, -1.0, -1.4));
    glUniformMatrix4fv(gModelLocation, 1, GL_FALSE, glm::value_ptr(model_matrixProton15));
    glUniformMatrix4fv(gViewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(gProjLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniform3f(gDiffuseLocation, 0.1, 0.1, 0.1);
    glUniform3f(gSpecularLocation, 0.1, 0.1, 0.1);
    glUniform3f(gAmbientLocation, 0.3, 0.5, 0.3);
    glUniform1f(gAlphaValueLocation, 0.95f - alphaFactor);

    glDrawArrays(GL_TRIANGLES, 0, gVerticesSphere.size());
    /***************Silicon Proton 15******************/
    /***************Silicon Nucleus******************/
    glUniformMatrix4fv(gModelLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glUniformMatrix4fv(gViewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(gProjLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniform3f(gDiffuseLocation, 0.1, 0.1, 0.1);
    glUniform3f(gSpecularLocation, 0.1, 0.1, 0.1);
    glUniform3f(gAmbientLocation, 0.1, 0.1, 0.3);
    glUniform1f(gAlphaValueLocation, 0.65f - alphaFactor);

    glDrawArrays(GL_TRIANGLES, 0, gVerticesSphere.size());
    /***************Silicon Nucleus******************/
    /************** Electron 1 ********************/
    static float theta11 = 0.0f;

    glm::mat4 model_matrixElectron11 = glm::mat4(1.0f);

    model_matrixElectron11 = glm::translate(
        modelMatrix,
        glm::vec3(1.5f * cos(theta11), (1.5f * sin(theta11)), 0.0));

    model_matrixElectron11 = glm::scale(model_matrixElectron11, vec3(0.2f));

    glUniformMatrix4fv(gModelLocation, 1, GL_FALSE, glm::value_ptr(model_matrixElectron11));
    glUniformMatrix4fv(gViewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(gProjLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniform3f(gDiffuseLocation, 0.4, 0.3, 0.3);
    glUniform3f(gSpecularLocation, 0.4, 0.3, 0.3);
    glUniform3f(gAmbientLocation, 0.6, 0.6, 0.6);
    glUniform1f(gAlphaValueLocation, 1.0f - alphaFactor);

    glDrawArrays(GL_TRIANGLES, 0, gVerticesSphere.size());
    /************** Electron 1 ********************/

    /************** Electron 2 ********************/
    static float beta12 = 90.0f;

    glm::mat4 model_matrixElectron12 = glm::mat4(1.0f);

    model_matrixElectron12 = glm::translate(
        modelMatrix,
        glm::vec3(1.5f * cos(beta12), 0.0, (1.5f * sin(beta12))));

    model_matrixElectron12 = glm::scale(model_matrixElectron12, vec3(0.2f));

    glUniformMatrix4fv(gModelLocation, 1, GL_FALSE, glm::value_ptr(model_matrixElectron12));
    glUniformMatrix4fv(gViewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(gProjLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniform3f(gDiffuseLocation, 0.4, 0.3, 0.3);
    glUniform3f(gSpecularLocation, 0.4, 0.3, 0.3);
    glUniform3f(gAmbientLocation, 0.6, 0.6, 0.6);
    glUniform1f(gAlphaValueLocation, 1.0f - alphaFactor);

    glDrawArrays(GL_TRIANGLES, 0, gVerticesSphere.size());
    /************** Electron 2 ********************/

    /************** Electron 3 ********************/
    static float alpha21 = 100.0f;

    glm::mat4 model_matrixElectron21 = glm::mat4(1.0f);

    model_matrixElectron21 = glm::translate(
        modelMatrix,
        glm::vec3(2.3f * cos(alpha21), 0.0, (2.3f * sin(alpha21))));

    model_matrixElectron21 = glm::scale(model_matrixElectron21, vec3(0.2f));



    glUniformMatrix4fv(gModelLocation, 1, GL_FALSE, glm::value_ptr(model_matrixElectron21));
    glUniformMatrix4fv(gViewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(gProjLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniform3f(gDiffuseLocation, 0.42, 0.3, 0.3);
    glUniform3f(gSpecularLocation, 0.42, 0.8, 0.8);
    glUniform3f(gAmbientLocation, 0.7, 0.6, 0.6);
    glUniform1f(gAlphaValueLocation, 1.0f - alphaFactor);

    glDrawArrays(GL_TRIANGLES, 0, gVerticesSphere.size());
    /************** Electron 3 ********************/

    /************** Electron 4 ********************/
    static float gamma22 = 10.0f;

    glm::mat4 model_matrixElectron22 = glm::mat4(1.0f);

    model_matrixElectron22 = glm::rotate(
        modelMatrix,
        45.0f,
        glm::vec3(0.0, 0.0, 1.0));

    model_matrixElectron22 = glm::translate(
        model_matrixElectron22,
        glm::vec3((2.3f * sin(gamma22)), 0.0, (2.3f * cos(gamma22))));

    model_matrixElectron22 = glm::scale(model_matrixElectron22, vec3(0.2f));



    glUniformMatrix4fv(gModelLocation, 1, GL_FALSE, glm::value_ptr(model_matrixElectron22));
    glUniformMatrix4fv(gViewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(gProjLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniform3f(gDiffuseLocation, 0.42, 0.3, 0.3);
    glUniform3f(gSpecularLocation, 0.42, 0.3, 0.3);
    glUniform3f(gAmbientLocation, 0.7, 0.6, 0.6);
    glUniform1f(gAlphaValueLocation, 1.0f - alphaFactor);

    glDrawArrays(GL_TRIANGLES, 0, gVerticesSphere.size());
    /************** Electron 4 ********************/

    /************** Electron 5 ********************/
    static float omega23 = 50.0f;

    glm::mat4 model_matrixElectron23 = glm::mat4(1.0f);

    model_matrixElectron23 = glm::rotate(
        modelMatrix,
        45.0f,
        glm::vec3(0.0, 0.0, -1.0));

    model_matrixElectron23 = glm::translate(
        model_matrixElectron23,
        glm::vec3((2.3f * sin(omega23)), 0.0, (2.3f * cos(omega23))));

    model_matrixElectron23 = glm::scale(model_matrixElectron23, vec3(0.2f));



    glUniformMatrix4fv(gModelLocation, 1, GL_FALSE, glm::value_ptr(model_matrixElectron23));
    glUniformMatrix4fv(gViewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(gProjLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniform3f(gDiffuseLocation, 0.42, 0.3, 0.3);
    glUniform3f(gSpecularLocation, 0.42, 0.3, 0.3);
    glUniform3f(gAmbientLocation, 0.7, 0.6, 0.6);
    glUniform1f(gAlphaValueLocation, 1.0f - alphaFactor);

    glDrawArrays(GL_TRIANGLES, 0, gVerticesSphere.size());
    /************** Electron 5 ********************/

    /************** Electron 6 ********************/
    static float delta24 = 90.0f;

    glm::mat4 model_matrixElectron24 = glm::mat4(1.0f);

    model_matrixElectron24 = glm::translate(
        modelMatrix,
        glm::vec3((2.3f * sin(delta24)), (2.3f * cos(delta24)), 0.0));

    model_matrixElectron24 = glm::scale(model_matrixElectron24, vec3(0.2f));



    glUniformMatrix4fv(gModelLocation, 1, GL_FALSE, glm::value_ptr(model_matrixElectron24));
    glUniformMatrix4fv(gViewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(gProjLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniform3f(gDiffuseLocation, 0.42, 0.3, 0.3);
    glUniform3f(gSpecularLocation, 0.42, 0.3, 0.3);
    glUniform3f(gAmbientLocation, 0.7, 0.6, 0.6);
    glUniform1f(gAlphaValueLocation, 1.0f - alphaFactor);

    glDrawArrays(GL_TRIANGLES, 0, gVerticesSphere.size());
    /************** Electron 6 ********************/

    /************** Electron 7 ********************/
    static float epsilon25 = 120.0f;

    glm::mat4 model_matrixElectron25 = glm::mat4(1.0f);

    model_matrixElectron25 = glm::translate(
        modelMatrix,
        glm::vec3(0.0, (2.3f * sin(epsilon25)), (2.3f * cos(epsilon25))));

    model_matrixElectron25 = glm::scale(model_matrixElectron25, vec3(0.2f));



    glUniformMatrix4fv(gModelLocation, 1, GL_FALSE, glm::value_ptr(model_matrixElectron25));
    glUniformMatrix4fv(gViewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(gProjLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniform3f(gDiffuseLocation, 0.42, 0.3, 0.3);
    glUniform3f(gSpecularLocation, 0.42, 0.3, 0.3);
    glUniform3f(gAmbientLocation, 0.7, 0.6, 0.6);
    glUniform1f(gAlphaValueLocation, 1.0f - alphaFactor);

    glDrawArrays(GL_TRIANGLES, 0, gVerticesSphere.size());
    /************** Electron 7 ********************/

    /************** Electron 8 ********************/
    static float zeta26 = 150.0f;

    glm::mat4 model_matrixElectron26 = glm::mat4(1.0f);

    model_matrixElectron26 = glm::rotate(
        modelMatrix,
        67.0f,
        glm::vec3(0.0, 0.0, -1.0));

    model_matrixElectron26 = glm::translate(
        model_matrixElectron26,
        glm::vec3(0.0, (2.3f * sin(zeta26)), (2.3f * cos(zeta26))));

    model_matrixElectron26 = glm::scale(model_matrixElectron26, vec3(0.2f));


    glUniformMatrix4fv(gModelLocation, 1, GL_FALSE, glm::value_ptr(model_matrixElectron26));
    glUniformMatrix4fv(gViewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(gProjLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniform3f(gDiffuseLocation, 0.42, 0.3, 0.3);
    glUniform3f(gSpecularLocation, 0.42, 0.3, 0.3);
    glUniform3f(gAmbientLocation, 0.7, 0.6, 0.6);
    glUniform1f(gAlphaValueLocation, 1.0f - alphaFactor);

    glDrawArrays(GL_TRIANGLES, 0, gVerticesSphere.size());
    /************** Electron 8 ********************/

    /************** Electron 9 ********************/
    static float eta27 = 160.0f;

    glm::mat4 model_matrixElectron27 = glm::mat4(1.0f);

    model_matrixElectron27 = glm::rotate(
        modelMatrix,
        -67.0f,
        glm::vec3(0.0, 0.0, -1.0));

    model_matrixElectron27 = glm::translate(
        model_matrixElectron27,
        glm::vec3(0.0, (2.3f * sin(eta27)), (2.3f * cos(eta27))));

    model_matrixElectron27 = glm::scale(model_matrixElectron27, vec3(0.2f));


    glUniformMatrix4fv(gModelLocation, 1, GL_FALSE, glm::value_ptr(model_matrixElectron27));
    glUniformMatrix4fv(gViewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(gProjLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniform3f(gDiffuseLocation, 0.42, 0.3, 0.3);
    glUniform3f(gSpecularLocation, 0.42, 0.3, 0.3);
    glUniform3f(gAmbientLocation, 0.7, 0.6, 0.6);
    glUniform1f(gAlphaValueLocation, 1.0f - alphaFactor);

    glDrawArrays(GL_TRIANGLES, 0, gVerticesSphere.size());
    /************** Electron 9 ********************/

    /************** Electron 10 ********************/
    static float kappa28 = 39.0f;

    glm::mat4 model_matrixElectron28 = glm::mat4(1.0f);

    model_matrixElectron28 = glm::rotate(
        modelMatrix,
        25.0f,
        glm::vec3(0.0, 0.0, -1.0));

    model_matrixElectron28 = glm::translate(
        model_matrixElectron28,
        glm::vec3(0.0, (2.3f * sin(kappa28)), (2.3f * cos(kappa28))));

    model_matrixElectron28 = glm::scale(model_matrixElectron28, vec3(0.2f));


    glUniformMatrix4fv(gModelLocation, 1, GL_FALSE, glm::value_ptr(model_matrixElectron28));
    glUniformMatrix4fv(gViewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(gProjLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniform3f(gDiffuseLocation, 0.42, 0.3, 0.3);
    glUniform3f(gSpecularLocation, 0.42, 0.3, 0.3);
    glUniform3f(gAmbientLocation, 0.7, 0.6, 0.6);
    glUniform1f(gAlphaValueLocation, 1.0f - alphaFactor);

    glDrawArrays(GL_TRIANGLES, 0, gVerticesSphere.size());
    /************** Electron 10 ********************/

    /************** Electron 11 ********************/
    static float lambda31 = 59.0f;
    static float toggle = 0.01f;

    glm::mat4 model_matrixElectron31 = glm::mat4(1.0f);

    if (!ifMove)
    {
        model_matrixElectron31 = glm::translate(
            modelMatrix,
            glm::vec3(0.0, (2.8f * sin(lambda31)), (2.8f * cos(lambda31))));
    }
    else
    {
        if (toggle > 0.0)
        {
            toggle = -0.04f;
        }
        else
        {
            toggle = 0.04f;
        }
        if (count > 2200)
        {
            toggle = toggle / 5;
            model_matrixElectron31 = glm::translate(
                glm::mat4(1.0f),
                glm::vec3(0.0, toggle, 0.0));
        }
        else
        {
            model_matrixElectron31 = glm::translate(
                modelMatrix,
                glm::vec3((2.8f * sin(lambda31)), toggle, (2.8f * cos(lambda31))));
        }
    }    

    model_matrixElectron31 = glm::scale(model_matrixElectron31, vec3(0.2f));

    glUniformMatrix4fv(gModelLocation, 1, GL_FALSE, glm::value_ptr(model_matrixElectron31));
    glUniformMatrix4fv(gViewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(gProjLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniform3f(gDiffuseLocation, 1.0, 0.8, 0.8);
    glUniform3f(gSpecularLocation, 1.0, 0.8, 0.8);
    glUniform3f(gAmbientLocation, 0.9, 0.6, 0.6);
    glUniform1f(gAlphaValueLocation, 1.0f - alphaFactor);

    glDrawArrays(GL_TRIANGLES, 0, gVerticesSphere.size());
    /************** Electron 11 ********************/

    /************** Electron 12 ********************/
    static float mu32 = 0.0f;

    glm::mat4 model_matrixElectron32 = glm::mat4(1.0f);

    if (!ifMove)
    {
        model_matrixElectron32 = glm::translate(
            modelMatrix,
            glm::vec3((2.8f * sin(mu32)), 0.0, (2.8f * cos(mu32))));
    }
    else if (count > 2200)
    {
        model_matrixElectron32 = glm::translate(
            glm::mat4(1.0f),
            glm::vec3(0.0, toggle, 0.0));
    }
    else
    {
        model_matrixElectron32 = glm::translate(
            modelMatrix,
            glm::vec3((2.8f * sin(mu32)), toggle, (2.8f * cos(mu32))));
    }
    model_matrixElectron32 = glm::scale(model_matrixElectron32, vec3(0.2f));


    glUniformMatrix4fv(gModelLocation, 1, GL_FALSE, glm::value_ptr(model_matrixElectron32));
    glUniformMatrix4fv(gViewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(gProjLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniform3f(gDiffuseLocation, 1.0, 0.8, 0.8);
    glUniform3f(gSpecularLocation, 1.0, 0.8, 0.8);
    glUniform3f(gAmbientLocation, 0.9, 0.6, 0.6);
    glUniform1f(gAlphaValueLocation, 1.0f - alphaFactor);

    glDrawArrays(GL_TRIANGLES, 0, gVerticesSphere.size());
    /************** Electron 12 ********************/

    /************** Electron 13 ********************/
    static float mu33 = 45.0f;

    glm::mat4 model_matrixElectron33 = glm::mat4(1.0f);

    model_matrixElectron33 = glm::rotate(
        modelMatrix,
        45.0f,
        glm::vec3(0.0, 0.0, 1.0));

    if (!ifMove)
    {
        model_matrixElectron33 = glm::translate(
            model_matrixElectron33,
            glm::vec3((2.8f * sin(mu33)), 0.0, (2.8f * cos(mu33))));
    }
    else if (count > 2200)
    {
        model_matrixElectron33 = glm::translate(
            glm::mat4(1.0f),
            glm::vec3(0.0, toggle, 0.0));
    }
    else
    {
        model_matrixElectron33 = glm::translate(
            model_matrixElectron33,
            glm::vec3((2.8f * sin(mu33)), toggle, (2.8f * cos(mu33))));
    }

    model_matrixElectron33 = glm::scale(model_matrixElectron33, vec3(0.2f));


    glUniformMatrix4fv(gModelLocation, 1, GL_FALSE, glm::value_ptr(model_matrixElectron33));
    glUniformMatrix4fv(gViewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(gProjLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniform3f(gDiffuseLocation, 1.0, 0.8, 0.8);
    glUniform3f(gSpecularLocation, 1.0, 0.8, 0.8);
    glUniform3f(gAmbientLocation, 0.9, 0.6, 0.6);
    glUniform1f(gAlphaValueLocation, 1.0f - alphaFactor);

    glDrawArrays(GL_TRIANGLES, 0, gVerticesSphere.size());
    /************** Electron 13 ********************/

    /************** Electron 14 ********************/
    static float mu34 = -45.0f;

    glm::mat4 model_matrixElectron34 = glm::mat4(1.0f);

    model_matrixElectron34 = glm::rotate(
        modelMatrix,
        45.0f,
        glm::vec3(0.0, 0.0, -1.0));

    if (!ifMove)
    {
        model_matrixElectron34 = glm::translate(
            model_matrixElectron34,
            glm::vec3((2.8f * sin(mu34)), 0.0, (2.8f * cos(mu34))));
    }
    else if (count > 2200)
    {
        model_matrixElectron34 = glm::translate(
            glm::mat4(1.0f),
            glm::vec3(0.0, toggle, 0.0));
    }
    else
    {
        count = count;
        model_matrixElectron34 = glm::translate(
             model_matrixElectron34,
            glm::vec3((2.8f * sin(mu34)), toggle, (2.8f * cos(mu34))));
    }
    model_matrixElectron34 = glm::scale(model_matrixElectron34, vec3(0.2f));


    glUniformMatrix4fv(gModelLocation, 1, GL_FALSE, glm::value_ptr(model_matrixElectron34));
    glUniformMatrix4fv(gViewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(gProjLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniform3f(gDiffuseLocation, 1.0, 0.8, 0.8);
    glUniform3f(gSpecularLocation, 1.0, 0.8, 0.8);
    glUniform3f(gAmbientLocation, 0.9, 0.6, 0.6);
    glUniform1f(gAlphaValueLocation, 1.0f - alphaFactor);

    glDrawArrays(GL_TRIANGLES, 0, gVerticesSphere.size());
    /************** Electron 14 ********************/

    if (count > 2300)
    {
        alphaFactor += 0.001f;
    }

    theta11 += DELTA_FIRST_ORBIT;
    if (theta11 >= 360)
        theta11 = 0.0;

    beta12 += DELTA_FIRST_ORBIT;
    if (beta12 >= 360)
        beta12 = 0.0;

    alpha21 += DELTA_SECOND_ORBIT;
    if (alpha21 >= 360)
        alpha21 = 0.0;

    gamma22 += DELTA_SECOND_ORBIT;
    if (gamma22 >= 360)
        gamma22 = 0.0;

    omega23 += DELTA_SECOND_ORBIT;
    if (omega23 >= 360)
        omega23 = 0.0;

    delta24 += DELTA_SECOND_ORBIT;
    if (delta24 >= 360)
        delta24 = 0.0;

    epsilon25 += DELTA_SECOND_ORBIT;
    if (epsilon25 >= 360)
        epsilon25 = 0.0;

    zeta26 += DELTA_SECOND_ORBIT;
    if (zeta26 >= 360)
        zeta26 = 0.0;

    eta27 += DELTA_SECOND_ORBIT;
    if (eta27 >= 360)
        eta27 = 0.0;

    kappa28 += DELTA_SECOND_ORBIT;
    if (kappa28 >= 360)
        kappa28 = 0.0;

    lambda31 += DELTA_THIRD_ORBIT;
    if (lambda31 >= 360)
        lambda31 = 0.0;

    mu32 += DELTA_THIRD_ORBIT;
    if (mu32 >= 360)
        mu32 = 0.0;

    mu33 += DELTA_THIRD_ORBIT;
    if (mu32 >= 360)
        mu32 = 0.0;

    mu34 += DELTA_THIRD_ORBIT;
    if (mu34 >= 360)
        mu34 = 0.0;
}

RendererResult SiliconAtom::Render(
    const RenderParams &params
)
{
    RendererResult result = RENDERER_RESULT_SUCCESS;
    count++;

    static const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    static const GLfloat one = 1.0f;

    glClearBufferfv(GL_COLOR, 0, black);
    glClearBufferfv(GL_DEPTH, 0, &one);

    static GLfloat range = 0.0f;
    static GLfloat sidewordsMoveSiliconAtom = 0.0f;
    static GLfloat sidewordsMovePhosphorusAtom = -10.0f;
    static BOOL ifMove = FALSE;
    if(range >= -10.0f)
        range -= 0.01f;
    else
    {
        if (sidewordsMoveSiliconAtom <= 3.0f)
        {
            sidewordsMoveSiliconAtom += 0.01f;
        }
    }

    static GLfloat angle = 0.0f;

    /***************Cube Map******************/
    glBindVertexArray(m_vao);

    glm::mat4 viewMatrix = glm::lookAt(glm::vec3(0.0, 0.0, -1000.0),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0, 1.0f, 0.0f));
   
    glm::mat4 mvMatrix = viewMatrix  * 
        glm::translate(glm::mat4(1.0f), glm::vec3(4.0f, -4.0f, 0.0f));
   
    glUseProgram(m_progCubeBoxObj);
   
    glUniformMatrix4fv(
        m_mvCubeBoxLocation,
        1,
        GL_FALSE,
        glm::value_ptr(mvMatrix));
   
    glActiveTexture(GL_TEXTURE0);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glUseProgram(0);
    /***************Cube Map******************/


    glUseProgram(gSiliconProgObj);

    mat4 modelMatrix = glm::mat4(1.0f);
    viewMatrix = glm::lookAt(
        glm::vec3(0.0f, 0.0f, range),
              glm::vec3(0.0f, 0.0f, 0.0f),
              glm::vec3(0.0, 1.0f, 0.0f));
   
    modelMatrix = glm::translate(glm::mat4(1.0), glm::vec3(sidewordsMoveSiliconAtom, 0.0f, 0.0f));
    if (!ifMove)
    {
        modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0.0f, 1.0f, 0.0f));
    }
    else
    {
        modelMatrix = glm::rotate(modelMatrix, 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    }

    RenderSiliconAtom(modelMatrix, viewMatrix, ifMove);

    if (sidewordsMoveSiliconAtom > 3.0f)
    {
        if (sidewordsMovePhosphorusAtom <= -3.0f)
        {
            sidewordsMovePhosphorusAtom += 0.01f;
        }
        else
        {
            ifMove = TRUE;
        }
        modelMatrix = glm::translate(glm::mat4(1.0), glm::vec3(sidewordsMovePhosphorusAtom, 0.0f, 0.0f));
        if (!ifMove)
        {
            modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0.0f, 1.0f, 0.0f));
        }
        else
        {
            modelMatrix = glm::rotate(modelMatrix, 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        }
        RenderPhosphorusAtom(modelMatrix, viewMatrix, ifMove);
    }
   //
   // modelMatrix = glm::translate(glm::mat4(1.0), glm::vec3(-6.0f, 0.0f, -10.0f));
   // RenderAtom(modelMatrix, viewMatrix);

    angle += 0.02f;
    if (angle >= 360.0f)
        angle = 0.0f;

    glBindVertexArray(0);
    /***************Silicon Nucleus******************/

    return result;
}

void SiliconAtom::UninitializeScene(SceneType scene)
{

}

void SiliconAtom::OnMessage(const Message &message)
{
    switch (message.messageId) {
    case WM_MOUSEWHEEL:
        for (int i = 0; i < abs(GET_WHEEL_DELTA_WPARAM(message.wParam) / WHEEL_DELTA); i++) {
            if ((short)GET_WHEEL_DELTA_WPARAM(message.wParam) > 0)
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
        if (handleMouseRotation) {
            camera.HandleMouse(mousePos);
        }
        break;

    case WM_KEYDOWN:
        switch (message.wParam) {
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
        switch (message.wParam) {
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
            if (fastCamera)
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

void SiliconAtom::OnResize(
    unsigned int width,
    unsigned int height)
{
    if (height == 0)
        height = 1;

    projectionMatrix = glm::perspective(
        45.0f,
        float(width) / float(height),
        0.1f,
        1000.0f);

    glViewport(
        0,
        0,
        (GLsizei)width,
        (GLsizei)height
    );
}

