#pragma once

#include "Renderer.h"
#include "RendererRegistrar.h"

namespace Framework
{
    using namespace Interfaces;
    using namespace Features;

    const unsigned WIN_WIDTH = 800;
    const unsigned WIN_HEIGHT = 600;

    class Host
    {
    private:
        unsigned long long          m_sceneStartTimestamp;
        unsigned long long          m_sceneFrameCounter;
        unsigned long long          m_epicStartTimestamp;
        unsigned long long          m_epicFrameCounter;
        SceneToRenderersMap         &m_renderers;
        SceneToRenderersMapIter     m_currentSceneType;
        RendererStateIter           m_currentRenderer;

    protected:
        Window                      m_window;
        bool                        m_exitLoop;
        bool                        m_isActive;
                        Host();
        virtual         ~Host();

    public:
        bool            Run();

    private:
                        Host(const Host&);
        Host& operator=(const Host&) { return *this; }
        RendererResult  Initialize(Window window);
        RendererResult  InitializeScene();
        void            UninitializeScene();
        RendererResult  Render();
        void            Uninitialize();
        RendererResult  SwitchScene();
        bool            IsCurrentRendererValid() const;

    protected:
        void            OnMessage(const Message &message);
        void            Resize(unsigned long width, unsigned long height);

        // methods need to be implemented by the child class
        virtual void    CreateDrawable() = 0;
        virtual void    DestroyDrawable() = 0;
        virtual bool    InitGL() = 0;
        virtual void    UninitGL() = 0;
        virtual void    SwapBuffers() = 0;
        virtual bool    PumpMessage() = 0;
    };
}
