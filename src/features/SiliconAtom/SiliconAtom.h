#pragma once
#include <Renderer.h>
#include <gl/glew.h>
#include <vector>
#include <glm\gtc\matrix_transform.hpp>

using namespace Interfaces;

class SiliconAtom :public IRenderer
{
private:
    GLuint m_vao;
    GLuint m_progCubeBoxObj;
    GLuint m_progSiliconAtom;
    std::vector<glm::vec3> m_vertices;
    std::vector<glm::vec3> m_normals;
    std::vector<glm::vec2> m_textures;
    std::vector<std::string> cubeMapTexture;

    GLuint m_TexCubeMap;
    GLuint m_mvCubeBoxLocation;

public:
    SiliconAtom();
    ~SiliconAtom();

    const char* GetName() override;
    RendererResult Initialize(Window window) override;
    void Uninitialize(void) override;
    RendererResult InitializeScene(SceneType scene) override;
    void UninitializeScene (SceneType scene) override;
    RendererResult Render(const RenderParams &params) override;
    void OnMessage(const Message &message) override;
    void OnResize(unsigned int width, unsigned int height) override;
};