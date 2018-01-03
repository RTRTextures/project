#pragma once

#include <Renderer_Static.h>

namespace Features
{
    using namespace Interfaces;

    class TestRenderer : public IRenderer
    {
    public:
        /// Method for performing one-time renderer initialization.
        /// Renderer can initialize global/static instances as part of this method
        /// @param window identifier of the window where drawing is directed
        /// @return RENDERER_RESULT_SUCCESS if succeded
        ///         RENDERER_RESULT_ERROR if failed.
        virtual RendererResult Initialize(Window window) 
        {
            return RENDERER_RESULT_SUCCESS;
        }

        /// Method for performing one-time renderer un-initialization before it is unloaded
        /// Renderer can perform global cleanup as part of this method
        virtual void Uninitialize(void)
        {

        }

        /// Method for performing scene-specific initialization
        /// This method will be called by the host before rendering a scene.
        /// Renderer should do initialization of scene specific things as part of this method
        /// @param scene Identifier of a scene to be initialized
        /// @return RENDERER_RESULT_SUCCESS if succeded
        ///         RENDERER_RESULT_ERROR if failed.
        virtual RendererResult InitializeScene(SceneType scene)
        {
            return RENDERER_RESULT_SUCCESS;
        }

        /// Method for performing scene-specific initialization
        /// This method will be called by the host after rendering a scene.
        /// Renderer should do cleanup of scene specific things done as part of scene initialize.
        /// @param scene Identifier of a scene to be cleaned-up
        virtual void UninitializeScene(SceneType scene)
        {
        }

        /// Method for rendering a frame in a scene
        /// This method will be called by the host per frame of a scene. Renderer should not call methods to
        /// switch the frame.
        /// @param scene describes the scene identifier for which the method is called
        /// @param elapsed describes the time elapsed from starting of this scene
        /// @param frameId describes the frameId specific to scene
        /// @return RENDERER_RESULT_SUCCESS if succeded in building the frame
        ///         RENDERER_RESULT_ERROR if failed in building the frame
        ///         RENDERER_RESULT_FINISHED if renderer has finished building its last frame of the scene.
        ///                                   in such cases no further frame calls would be made for this scene
        ///                                   to the renderer.
        virtual RendererResult Render(SceneType scene, unsigned long long elapsed, unsigned long long frameId)
        {
            return RENDERER_RESULT_FINISHED;
        }

        /// Generic method to notify renderers about a message posted to renderer window. The message can be
        /// from system or initiated by the host itself.
        /// @param message OS dependent structure that describes the system message being processed.
        virtual void OnMessage(const Message* message)
        {
        }
    };
}
