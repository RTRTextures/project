/// @file Renderer.h
/// @brief Defines the interface between OpenGL generic host process and its pluggable 'Renderer' clients
///        Host: Framework OpenGL host process which will load multiple renderers for rendering a scene
///        Renderer: Implementer of the scene container in dynamic linked library.

#ifdef _WIN32
#include <windows.h>
#endif

#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

    /// Major version should be updated when there is change in the existing interface
    #define HOST_MAJOR_VER  1
    /// Minor version should be incremented when there are changes in the host or addition of
    /// enumeration values
    #define HOST_MINOR_VER  0
    /// Host version string to be used by the host resource file. This should match to major-minor
    /// version pair
    #define HOST_VERSION_STRING "1.0"

    /// Possible return values of methods implemented by renderers
    typedef enum RendererResult_t
    {
        /// Indicates error in perfoming an operation
        RENDERER_RESULT_ERROR = 0,
        /// Indicates success in performing an operation
        RENDERER_RESULT_SUCCESS = 1,
        /// Indicates completion in processing of an operation
        RENDERER_RESULT_FINISHED = 2
    } RendererResult;

    /// Possible scenes supported by the host application. The scenes
    /// will be transitioned in below cases -
    /// 1. When all components have finished rendering present scene
    /// 2. There is request to the renderer to switch the scene.
    typedef enum SceneType_t
    {
        /// General initialization for all the scenes implemented by the component
        SCENE_TYPE_FIRST = 0,
        SCENE_TYPE_TEST0 = 0,
        SCENE_TYPE_TEST1 = 1,
        /// Add more scenes here in right order
        SCENE_TYPE_LAST = 2
    } SceneType;

    /// Types describing various methods implemented as part of the interface

    /// Method for performing one-time renderer initialization.
    /// Renderer can initialize global/static instances as part of this method
    /// @param window identifier of the window where drawing is directed
    /// @return RENDERER_RESULT_SUCCESS if succeded
    ///         RENDERER_RESULT_ERROR if failed.
#ifdef _WIN32
    #define Window HWND
#endif
    typedef RendererResult(*INITIALIZE_PROC)(Window window);

    /// Method for performing one-time renderer un-initialization before it is unloaded
    /// Renderer can perform global cleanup as part of this method
    typedef void(*UNINITIALIZE_PROC)(void);

    /// Method for performing scene-specific initialization
    /// This method will be called by the host before rendering a scene.
    /// Renderer should do initialization of scene specific things as part of this method
    /// @param scene Identifier of a scene to be initialized
    /// @return RENDERER_RESULT_SUCCESS if succeded
    ///         RENDERER_RESULT_ERROR if failed.
    typedef RendererResult (*SCENE_INITIALIZE_PROC)(SceneType scene);

    /// Method for performing scene-specific initialization
    /// This method will be called by the host after rendering a scene.
    /// Renderer should do cleanup of scene specific things done as part of scene initialize.
    /// @param scene Identifier of a scene to be cleaned-up
    typedef void (*SCENE_UNINITIALIZE_PROC)(SceneType scene);

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
    typedef RendererResult (*RENDER_SCENE_PROC)(SceneType scene, unsigned long long elapsed, unsigned long long frameId);

    /// Generic method to notify renderers about a message posted to renderer window. The message can be
    /// from system or initiated by the host itself.
    /// @param message OS dependent structure that describes the system message being processed.
#ifdef _WIN32
    typedef struct Message_t
    {
        UINT messageId;
        WPARAM wParam;
        LPARAM lParam;
    } Message;
#endif
    typedef void (*MESSAGE_PROC)(const Message* message);

    /// Current version of Host to Renderer interface
    #define INTERFACE_CURRENT_VER HOST_MAJOR_VER

    /// Structure defining interface between Host and Renderer components
    typedef struct RendererInfo_t
    {
        /// Version of the specification. It must be set to INTERFACE_CURRENT_VER
        unsigned version;
        /// Name of the renderer
        const char *name;
        /// Optional method to perform global initialization (can be NULL)
        /// It is called only once in the lifetime of the renderer.
        INITIALIZE_PROC initializeProc;
        /// Optional method to perform global uninitialization (can be NULL)
        /// It is called only once in the lifetime of the renderer
        UNINITIALIZE_PROC uninitializeProc;
        /// Optional scene specific initialize method (can be NULL)
        /// Called for each scene before it is rendered.
        SCENE_INITIALIZE_PROC initialzeSceneProc;
        /// Optional scene specific uninitialize method (can be NULL)
        /// Called for each scene after it is finished
        SCENE_UNINITIALIZE_PROC uninitializeSceneProc;
        /// Mandatory method for rendering a scene (cannot be NULL)
        /// Called for each frame of the scene for rendering.
        RENDER_SCENE_PROC renderSceneProc;
    } RendererInfo;

    /// The main entry point that renderers must implement.
    /// Host will load curresponding renderer dynamic library and
    /// retrieve information about the renderer using the entry point
    /// @return statically allocated renderer information structure.
#ifdef _WIN32
#define RENDERER_API __declspec(dllexport)
#else
#define RENDERER_API
#endif
    RENDERER_API const RendererInfo* GetRendererInfo();

    typedef RENDERER_API const RendererInfo* (*GET_RENDERER_INFO_PROC)();

    #define RENDERER_EXPORT_NAME "GetRendererInfo"


#ifdef __cplusplus
}
#endif

