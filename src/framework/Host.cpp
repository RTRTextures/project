#include "Host.h"
#include "RendererRegistrar.h"
#include <glew.h>
#include <gl/GL.h>

namespace Framework
{
    unsigned long long SystemMicrosecsTimestamp();

    Host::Host() :
        m_sceneStartTimestamp(0ll),
        m_sceneFrameCounter(0ll),
        m_epicStartTimestamp(0ll),
        m_epicFrameCounter(0ll),
        m_renderers(Features::GetRenderers()),
        m_window(NULL),
        m_exitLoop(false),
        m_isActive(true),
        m_currentSceneType(m_renderers.end()),
        m_winWidth(WIN_WIDTH),
        m_winHeight(WIN_HEIGHT)
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
                    // execute renderers
                    if (IsCurrentRendererValid() && RENDERER_RESULT_SUCCESS == InitializeScene())
                    {
                        while (!m_exitLoop)
                        {
                            if (!PumpMessage() && m_isActive)
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

        // initialize generic extension-wranglers (GLEW)
        if (GLEW_OK == glewInit())
        {
            // do global initializations
            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glClearDepth(1.0f);                 // set depth buffer
            glEnable(GL_DEPTH_TEST);            // enable depth testing
            glDepthFunc(GL_LEQUAL);             // type of depth testing

            for (auto sceneTypeEntry = m_renderers.begin();
                sceneTypeEntry != m_renderers.end() && result != RENDERER_RESULT_ERROR;
                ++sceneTypeEntry)
            {
                for (auto rendererEntry = sceneTypeEntry->second.begin();
                    rendererEntry != sceneTypeEntry->second.end() && result != RENDERER_RESULT_ERROR;
                    ++rendererEntry)
                {
                    result = rendererEntry->get()->Initialize(window) == RENDERER_RESULT_SUCCESS ?
                        RENDERER_RESULT_SUCCESS : RENDERER_RESULT_ERROR;
                }
            }

            if (result == RENDERER_RESULT_SUCCESS)
            {
                // initialize the renderers
                m_currentSceneType = m_renderers.begin();
                if (m_currentSceneType != m_renderers.end())
                {
                    m_currentRenderer = m_currentSceneType->second.begin();
                }
            }
        }
        else
        {
            result = RENDERER_RESULT_ERROR;
        }

        return result;
    }

    void Host::OnMessage(const Message &message)
    {
        // check for valid pointer before use
        if (IsCurrentRendererValid())
        {
            m_currentRenderer->get()->OnMessage(message);
        }
    }

    void Host::Resize(unsigned long width, unsigned long height)
    {
        //code
        if (height == 0)
        {
            height = 1;
        }

        // store these values
        m_winWidth = width;
        m_winHeight = height;

        glViewport(0, 0, (GLsizei)width, (GLsizei)height);
        m_currentRenderer->get()->OnResize(width, height);
    }

    RendererResult Host::InitializeScene()
    {
        RendererResult result = m_currentRenderer->get()->InitializeScene(m_currentSceneType->first) == RENDERER_RESULT_SUCCESS ?
            RENDERER_RESULT_SUCCESS : RENDERER_RESULT_ERROR;

        if (result == RENDERER_RESULT_SUCCESS)
        {
            // init scene specific variables
            m_sceneStartTimestamp = SystemMicrosecsTimestamp();
            m_sceneFrameCounter = 0;

            if (m_currentRenderer == m_currentSceneType->second.begin())
            {
                m_epicStartTimestamp = SystemMicrosecsTimestamp();
                m_epicFrameCounter = 0;
            }

            m_currentRenderer->get()->OnResize(m_winWidth, m_winHeight);
        }
        return result;
    }

    RendererResult Host::Render()
    {
        RendererResult result = RENDERER_RESULT_FINISHED;

        // basic operations
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        unsigned long long currentTs = SystemMicrosecsTimestamp();
        RenderParams params = {
            m_currentSceneType->first,
            currentTs - m_epicStartTimestamp,
            m_epicFrameCounter,
            currentTs - m_sceneStartTimestamp,
            m_sceneFrameCounter,
        };

        result = m_currentRenderer->get()->Render(params);

        if (result != RENDERER_RESULT_ERROR)
        {
            m_sceneFrameCounter++;
            m_epicFrameCounter++;
            SwapBuffers();
        }

        return result;
    }

    void Host::UninitializeScene()
    {
        m_currentRenderer->get()->UninitializeScene(m_currentSceneType->first);
    }

    void Host::Uninitialize()
    {
        // uninitialize in the reverse order of initialize()
        for (auto sceneTypeEntry = m_renderers.rbegin();
            sceneTypeEntry != m_renderers.rend();
            ++sceneTypeEntry)
        {
            for (auto rendererEntry = sceneTypeEntry->second.rbegin();
                rendererEntry != sceneTypeEntry->second.rend();
                ++rendererEntry)
            {
                rendererEntry->get()->Uninitialize();
            }
        }
    }

    RendererResult Host::SwitchScene()
    {
        RendererResult result = RENDERER_RESULT_FINISHED;

        if (IsCurrentRendererValid())
        {
            // Uninitialize current scene
            UninitializeScene();

            // switch to next renderer
            ++m_currentRenderer;

            // check if we need to switch the scene type
            while (m_currentRenderer == m_currentSceneType->second.end())
            {
                ++m_currentSceneType;
                if (m_currentSceneType == m_renderers.end())
                    break;
                m_currentRenderer = m_currentSceneType->second.begin();
            }

            // initialize the scene only if it is valid
            if (IsCurrentRendererValid())
            {
                result = InitializeScene();
            }
        }

        return result;
    }

    bool Host::IsCurrentRendererValid() const
    {
        return m_currentSceneType != m_renderers.end() &&
            m_currentRenderer != m_currentSceneType->second.end();
    }
}