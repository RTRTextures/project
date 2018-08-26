#pragma once

#include <Renderer.h>

namespace Features
{
    using namespace Interfaces;

    class SolarSystemExtRenderer : public IRenderer
    {
    public:
        const char* GetName() override;
        RendererResult Initialize(Window window) override;
        void Uninitialize(void) override;
        RendererResult InitializeScene(SceneType scene) override;
        void UninitializeScene(SceneType scene) override;
        virtual RendererResult Render(const RenderParams &params) override;
        void OnMessage(const Message &message) override;
        void OnResize(unsigned int width, unsigned int height) override;

    private:
        bool InitSolarSystem();
    };
}

