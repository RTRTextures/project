#include "SiliconAtom/SiliconAtom.h"
#include "VSIUtilTexture.h"
#include "..\VSIUtilShader.h"
#include <Camera.h>
#include <glm\mat4x4.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

extern bool fastCamera;

extern vec2  mousePos;
extern Utility::Camera camera;
extern bool handleMouseRotation;

extern mat4 modelMatrix, projectionMatrix;

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
    //glEnable(GL_DEPTH_TEST);
    // depth test to do
    glDepthFunc(GL_LEQUAL);
    // set really nice percpective calculations ?
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    // We will always cull back faces for better performance
    //glEnable(GL_CULL_FACE);


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

    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    VSIUtilLoadCubeMapTextures(cubeMapTexture, &m_TexCubeMap);
    glActiveTexture(GL_TEXTURE0);

    return RENDERER_RESULT_SUCCESS;
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

RendererResult SiliconAtom::Render(
    const RenderParams &params
)
{
    RendererResult result = RENDERER_RESULT_SUCCESS;

    static const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    static const GLfloat one = 1.0f;

    glClearBufferfv(GL_COLOR, 0, black);
    glClearBufferfv(GL_DEPTH, 0, &one);

    static GLfloat f = 0.0f;
    f += 0.00001f;

    glBindVertexArray(m_vao);

    glm::mat4 viewMatrix = glm::lookAt(glm::vec3(15.0f * sinf(f), 0.0f, 15.0f* cosf(f)),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0, 1.0f, 0.0f));

    glm::mat4 mvMatrix = viewMatrix *
        glm::rotate(glm::mat4(1.0f), f * 50.0f, glm::vec3(1.0f, 0.0f, 0.0f))*
        glm::rotate(glm::mat4(1.0f), f * 130.0f, glm::vec3(0.0f, 1.0f, 0.0f)) *
        glm::translate(glm::mat4(1.0f), glm::vec3(f * 4.0f, -4.0f, 0.0f));

    glUseProgram(m_progCubeBoxObj);

    glUniformMatrix4fv(
        m_mvCubeBoxLocation,
        1,
        GL_FALSE,
        glm::value_ptr(mvMatrix));

    glDisable(GL_DEPTH_TEST);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glUseProgram(0);

    return result;
}