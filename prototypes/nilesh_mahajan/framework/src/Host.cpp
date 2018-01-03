#include "Host.h"
#include "RendererRegistrar.h"

namespace Framework
{
    unsigned long long SystemMicrosecsTimestamp();

    Host::Host() :
        m_current(SCENE_TYPE_FIRST),
        m_sceneStartTimestamp(0ll),
        m_sceneFrameCounter(0ll),
        m_renderers(Features::GetRenderers()),
        m_window(NULL),
        m_exitLoop(false)
    {
    }

    bool Host::Run()
    {
        bool result = false;
        CreateDrawable();

        if (m_window != NULL)
        {
            if (InitGL())
            {
                if (Initialize(m_window) == RENDERER_RESULT_SUCCESS)
                {
                    while (!m_exitLoop)
                    {
                        if (!PumpMessage())
                        {
                            RendererResult rendererResult = Render();
                            if (RENDERER_RESULT_FINISHED == rendererResult)
                            {
                                rendererResult = SwitchScene();
                            }

                            if (rendererResult != RENDERER_RESULT_SUCCESS)
                            {
                                m_exitLoop = true;
                                result = (rendererResult == RENDERER_RESULT_FINISHED) ? true : false;
                            }
                        }
                    }
                }
            }

            // cleanup
            Uninitialize();
            UninitGL();
            DestroyDrawable();
        }
        return result;
    }

    Host::~Host()
    {
    }

    RendererResult Host::Initialize(Window window)
    {
        RendererResult result = RENDERER_RESULT_SUCCESS;
        for (auto rendererEntry = m_renderers.begin();
            rendererEntry != m_renderers.end() && result != RENDERER_RESULT_ERROR;
            ++rendererEntry)
        {
            if (rendererEntry->GetLastResult() != RENDERER_RESULT_ERROR)
            {
                rendererEntry->SetLastResult(rendererEntry->GetRenderer()->Initialize(window));
                result = (rendererEntry->GetLastResult() == RENDERER_RESULT_SUCCESS) ?
                    RENDERER_RESULT_SUCCESS : RENDERER_RESULT_ERROR;
            }
        }
        return result;
    }

    void Host::OnMessage(const Message * message)
    {
        for (auto rendererEntry = m_renderers.begin();
            rendererEntry != m_renderers.end();
            ++rendererEntry)
        {
            rendererEntry->GetRenderer()->OnMessage(message);
        }
    }

    RendererResult Host::InitializeScene()
    {
        RendererResult result = RENDERER_RESULT_SUCCESS;
        for (auto rendererEntry = m_renderers.begin();
            rendererEntry != m_renderers.end() && result != RENDERER_RESULT_ERROR;
            ++rendererEntry)
        {
            if (rendererEntry->GetLastResult() != RENDERER_RESULT_ERROR)
            {
                rendererEntry->SetActiveScene(m_current);
                rendererEntry->SetLastResult(rendererEntry->GetRenderer()->InitializeScene(m_current));
                result = (rendererEntry->GetLastResult() == RENDERER_RESULT_SUCCESS) ?
                    RENDERER_RESULT_SUCCESS : RENDERER_RESULT_ERROR;
            }
        }
        return result;
    }

    RendererResult Host::Render()
    {
        RendererResult result = RENDERER_RESULT_FINISHED;
        for (auto rendererEntry = m_renderers.begin();
            rendererEntry != m_renderers.end() && result != RENDERER_RESULT_ERROR;
            ++rendererEntry)
        {
            if (rendererEntry->GetLastResult() == RENDERER_RESULT_SUCCESS)
            {
                rendererEntry->SetLastResult(
                    rendererEntry->GetRenderer()->Render(
                        m_current,
                        SystemMicrosecsTimestamp() - m_sceneStartTimestamp,
                        m_sceneFrameCounter)
                    );
                RendererResult lastResult = rendererEntry->GetLastResult();
                if (lastResult != RENDERER_RESULT_FINISHED)
                {
                    result = lastResult;
                }
            }
        }

        if (result != RENDERER_RESULT_ERROR)
        {
            m_sceneFrameCounter++;
            SwapBuffers();
        }

        return result;
    }

    void Host::UninitializeScene()
    {
        for (auto rendererEntry = m_renderers.begin();
            rendererEntry != m_renderers.end();
            ++rendererEntry)
        {
            rendererEntry->GetRenderer()->UninitializeScene(m_current);
        }
    }

    void Host::Uninitialize()
    {
        for (auto rendererEntry = m_renderers.begin();
            rendererEntry != m_renderers.end();
            ++rendererEntry)
        {
            rendererEntry->GetRenderer()->Uninitialize();
        }
    }

    RendererResult Host::SwitchScene()
    {
        RendererResult result = RENDERER_RESULT_FINISHED;

        if (m_current < SCENE_TYPE_LAST)
        {
            // Uninitialize current scene
            UninitializeScene();

            unsigned current = static_cast<unsigned>(m_current);
            m_current = static_cast<SceneType>(current + 1);

            if (m_current < SCENE_TYPE_LAST)
            {
                // initialze the new scene
                result = InitializeScene();
                m_sceneStartTimestamp = SystemMicrosecsTimestamp();
                m_sceneFrameCounter = 0;
            }
        }

        return result;
    }
}