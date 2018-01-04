#pragma once

#include "Renderer_Static.h"
#include <list>
#include <memory>

namespace Features
{
    typedef std::shared_ptr<Interfaces::IRenderer> IRendererPtr;
    using namespace Interfaces;

    class RendererState
    {
    private:
        IRendererPtr            m_renderer;
        SceneType               m_activeScene;
        RendererResult          m_lastResult;

    public:
        RendererState(IRendererPtr renderer) :
            m_renderer(renderer),
            m_lastResult(RENDERER_RESULT_SUCCESS),
            m_activeScene(SCENE_TYPE_FIRST)
        {
        }

        void            SetLastResult(RendererResult result) { m_lastResult = result; }
        RendererResult  GetLastResult() const { return m_lastResult; }
        void            SetActiveScene(SceneType scene) { m_activeScene = scene; }
        SceneType       GetActiveScene() const { return m_activeScene; }
        IRendererPtr    GetRenderer() { return m_renderer; }
    };

    // returns global instance of renderer states
    std::list<RendererState>& GetRenderers();
}
