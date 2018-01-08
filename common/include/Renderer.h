/// @file Renderer.h
/// @brief Defines the interface between OpenGL generic host process and its pluggable 'Renderer' clients
///        Host: Framework OpenGL host process which will load multiple renderers for rendering a scene
///        Renderer: Implementer of the scene container in its class

#ifdef _WIN32
#include <windows.h>
#endif

#pragma once

namespace Interfaces
{
    /// Possible return values of methods implemented by renderers
    enum RendererResult
    {
        /// Indicates error in perfoming an operation
        RENDERER_RESULT_ERROR = 0,
        /// Indicates success in performing an operation
        RENDERER_RESULT_SUCCESS = 1,
        /// Indicates completion in processing of an operation
        RENDERER_RESULT_FINISHED = 2
    };

    /// Possible scene types supported by the host application. Renderer is per scene.
    /// SceneType can be also termed as an epic of scenes.
    /// Scenes are transitioned according to their order of registrations and can be transitioned
    /// in below cases -
    /// 1. When all components have finished rendering present scene
    /// 2. There is request to the renderer to switch the scene.
    enum SceneType
    {
        /// General initialization for all the scenes implemented by the component
        SCENE_TYPE_TEST0,
        SCENE_TYPE_TEST1,
		SCENE_SOLAR_SYSTEM,
    };

#ifdef _WIN32
    struct Message
    {
        UINT messageId;
        WPARAM wParam;
        LPARAM lParam;
    };
    #define Window HWND
#endif

    /// Parameters passed to a scene
    struct RenderParams
    {
        /// Type of the Scene
        SceneType scene;
        /// Microsecs time elapsed from start of an EPIC
        unsigned long long elapsedFromEpicStart;
        /// FrameId from start of an EPIC
        unsigned long long frameIdFromEpicStart;
        /// Microsecs time elapsed from start of a scene
        unsigned long long elapsedFromSceneStart;
        /// FrameId from start of a scene
        unsigned long long frameIdFromSceneStart;
    };

    /// @brief interface to render
    class IRenderer
    {
    public:
        /// Method for retrieving name of the renderer
        /// @return NULL on failure otherwise pointer to null-terminated string mentioning name of
        //               this renderer.
        virtual const char* GetName() = 0;

        /// Method for performing one-time renderer initialization.
        /// Renderer can initialize global/static instances as part of this method
        /// @param window identifier of the window where drawing is directed
        /// @return RENDERER_RESULT_SUCCESS if succeded
        ///         RENDERER_RESULT_ERROR if failed.
        virtual RendererResult Initialize(Window window) = 0;

        /// Method for performing one-time renderer un-initialization before it is unloaded
        /// Renderer can perform global cleanup as part of this method
        virtual void Uninitialize(void) = 0;

        /// Method for performing scene-specific initialization
        /// This method will be called by the host before rendering a scene to the active renderer.
        /// Renderer should do initialization of scene specific things as part of this method
        /// @param scene Identifier of a scene to be initialized
        /// @return RENDERER_RESULT_SUCCESS if succeded
        ///         RENDERER_RESULT_ERROR if failed.
        virtual RendererResult InitializeScene(SceneType scene) = 0;

        /// Method for performing scene-specific initialization
        /// This method will be called by the host after rendering a scene to the active renderer
        /// Renderer should do cleanup of scene specific things done as part of scene initialize.
        /// @param scene Identifier of a scene to be cleaned-up
        virtual void UninitializeScene(SceneType scene) = 0;

        /// Method for rendering a frame in a scene
        /// This method will be called by the host per frame of a scene only to the active renderer
        /// @param params describes the parameters curresponding to this render
        /// @return RENDERER_RESULT_SUCCESS if succeded in building the frame
        ///         RENDERER_RESULT_ERROR if failed in building the frame
        ///         RENDERER_RESULT_FINISHED if renderer has finished building its last frame of the scene.
        ///                                   in such cases no further frame calls would be made for this scene
        ///                                   to the renderer.
        virtual RendererResult Render(const RenderParams &params) = 0;

        /// Generic method to notify active renderer about a message posted to host window. The message can be
        /// from system or initiated by the host itself.
        /// @param message OS dependent structure that describes the system message being processed.
        virtual void OnMessage(const Message &message) = 0;

        /// Generic method to notify active renderer about the change in the dimensions of the host window
        /// @param width New width of the window
        /// @param height New height of the window
        virtual void OnResize(unsigned int width, unsigned int height) = 0;
    };

} // Interfaces
