#pragma once

#include <Renderer_Static.h>
#include <gl\GL.h>

namespace Features
{
    using namespace Interfaces;

    // Sample renderer to render multi-color pyramid as part of SCENE_TYPE_TEST0 scene
    class TestRenderer : public IRenderer
    {
    public:
        /// Method for retrieving name of the renderer
        /// @param rendererName buffer to be filled with the renderer name
        /// @return RENDERER_RESULT_SUCCESS if succeded, rendererName to contain null-terminated name
        ///         RENDERER_RESULT_ERROR if failed, renderName is ignored
        const char* GetName() override
        {
            static const char name[] = "TestRenderer";
            return name;
        }

        /// Method for performing one-time renderer initialization.
        /// Renderer can initialize global/static instances as part of this method
        /// @param window identifier of the window where drawing is directed
        /// @return RENDERER_RESULT_SUCCESS if succeded
        ///         RENDERER_RESULT_ERROR if failed.
        RendererResult Initialize(Window window) override
        {
            return RENDERER_RESULT_SUCCESS;
        }

        /// Method for performing one-time renderer un-initialization before it is unloaded
        /// Renderer can perform global cleanup as part of this method
        void Uninitialize(void) override
        {
        }

        /// Method for performing scene-specific initialization
        /// This method will be called by the host before rendering a scene.
        /// Renderer should do initialization of scene specific things as part of this method
        /// @param scene Identifier of a scene to be initialized
        /// @return RENDERER_RESULT_SUCCESS if succeded
        ///         RENDERER_RESULT_ERROR if failed.
        RendererResult InitializeScene(SceneType scene) override
        {
            return RENDERER_RESULT_SUCCESS;
        }

        /// Method for performing scene-specific initialization
        /// This method will be called by the host after rendering a scene.
        /// Renderer should do cleanup of scene specific things done as part of scene initialize.
        /// @param scene Identifier of a scene to be cleaned-up
        void UninitializeScene(SceneType scene) override
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
        RendererResult Render(SceneType scene, unsigned long long elapsed, unsigned long long frameId) override
        {
            static unsigned renderFrameCount = 100;
            RendererResult result = RENDERER_RESULT_FINISHED;

            // render 100 frames before transitioning to next scene
            if (scene == SCENE_TYPE_TEST0 && --renderFrameCount > 0)
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

                glTranslatef(0.0f, 0.0f, -6.0f);
                glRotatef(1.0f * frameId, 0.0f, 1.0f, 0.0f);

                GLfloat pyramidVertices[][3] = {
                    // front
                    { 0.0f, 1.0f, 0.0f },
                    { -1.0f, -1.0f, 1.0f },
                    { 1.0f, -1.0f, 1.0f },

                    // left
                    { 0.0f, 1.0f, 0.0f },
                    { -1.0f, -1.0f, -1.0f },
                    { -1.0f, -1.0f, 1.0f },

                    //back
                    { 0.0f, 1.0f, 0.0f },
                    { -1.0f, -1.0f, -1.0f },
                    { 1.0f, -1.0f, -1.0f },

                    // right
                    { 0.0f, 1.0f, 0.0f },
                    { 1.0f, -1.0f, 1.0f },
                    { 1.0f, -1.0f, -1.0f },
                };

                // Draw triangles for the pyramid
                glBegin(GL_TRIANGLES);
                    glColor3f(1.0f, 0.0f, 0.0f);
                    glNormal3f(0.0f, 0.447214f, 0.894427f);
                    glVertex3fv((GLfloat*)&pyramidVertices[0]);
                    glVertex3fv((GLfloat*)&pyramidVertices[1]);
                    glVertex3fv((GLfloat*)&pyramidVertices[2]);

                    glColor3f(0.0f, 1.0f, 0.0f);
                    glNormal3f(-0.894427f, 0.447214f, 0.0f);
                    glVertex3fv((GLfloat*)&pyramidVertices[3]);
                    glVertex3fv((GLfloat*)&pyramidVertices[4]);
                    glVertex3fv((GLfloat*)&pyramidVertices[5]);

                    glColor3f(0.0f, 0.0f, 1.0f);
                    glNormal3f(0.0f, 0.447214f, -0.894427f);
                    glVertex3fv((GLfloat*)&pyramidVertices[6]);
                    glVertex3fv((GLfloat*)&pyramidVertices[7]);
                    glVertex3fv((GLfloat*)&pyramidVertices[8]);

                    glColor3f(0.0f, 1.0f, 1.0f);
                    glNormal3f(0.894427f, 0.447214f, 0.0f);
                    glVertex3fv((GLfloat*)&pyramidVertices[9]);
                    glVertex3fv((GLfloat*)&pyramidVertices[10]);
                    glVertex3fv((GLfloat*)&pyramidVertices[11]);
                glEnd();

                result = RENDERER_RESULT_SUCCESS;
            }
            return result;
        }

        /// Generic method to notify renderers about a message posted to renderer window. The message can be
        /// from system or initiated by the host itself.
        /// @param message OS dependent structure that describes the system message being processed.
        void OnMessage(const Message* message) override
        {
        }
    };

    // Sample renderer to render yellow colored pyramid as part of SCENE_TYPE_TEST1 scene
    class TestRenderer2 : public IRenderer
    {
    public:
        /// Method for retrieving name of the renderer
        /// @param rendererName buffer to be filled with the renderer name
        /// @return RENDERER_RESULT_SUCCESS if succeded, rendererName to contain null-terminated name
        ///         RENDERER_RESULT_ERROR if failed, renderName is ignored
        const char* GetName() override
        {
            static const char name[] = "TestRenderer2";
            return name;
        }

        /// Method for performing one-time renderer initialization.
        /// Renderer can initialize global/static instances as part of this method
        /// @param window identifier of the window where drawing is directed
        /// @return RENDERER_RESULT_SUCCESS if succeded
        ///         RENDERER_RESULT_ERROR if failed.
        RendererResult Initialize(Window window) override
        {
            return RENDERER_RESULT_SUCCESS;
        }

        /// Method for performing one-time renderer un-initialization before it is unloaded
        /// Renderer can perform global cleanup as part of this method
        void Uninitialize(void) override
        {
        }

        /// Method for performing scene-specific initialization
        /// This method will be called by the host before rendering a scene.
        /// Renderer should do initialization of scene specific things as part of this method
        /// @param scene Identifier of a scene to be initialized
        /// @return RENDERER_RESULT_SUCCESS if succeded
        ///         RENDERER_RESULT_ERROR if failed.
        RendererResult InitializeScene(SceneType scene) override
        {
            return RENDERER_RESULT_SUCCESS;
        }

        /// Method for performing scene-specific initialization
        /// This method will be called by the host after rendering a scene.
        /// Renderer should do cleanup of scene specific things done as part of scene initialize.
        /// @param scene Identifier of a scene to be cleaned-up
        void UninitializeScene(SceneType scene) override
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
        RendererResult Render(SceneType scene, unsigned long long elapsed, unsigned long long frameId) override
        {
            RendererResult result = RENDERER_RESULT_FINISHED;

            // render for 5 secs before transitioning to next scene
            if (scene == SCENE_TYPE_TEST1 && elapsed < 5 * 1000 * 1000)
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

                glTranslatef(0.0f, 0.0f, -6.0f);
                glRotatef(1.0f * frameId, 0.0f, 1.0f, 0.0f);

                GLfloat pyramidVertices[][3] = {
                    // front
                    { 0.0f, 1.0f, 0.0f },
                    { -1.0f, -1.0f, 1.0f },
                    { 1.0f, -1.0f, 1.0f },

                    // left
                    { 0.0f, 1.0f, 0.0f },
                    { -1.0f, -1.0f, -1.0f },
                    { -1.0f, -1.0f, 1.0f },

                    //back
                    { 0.0f, 1.0f, 0.0f },
                    { -1.0f, -1.0f, -1.0f },
                    { 1.0f, -1.0f, -1.0f },

                    // right
                    { 0.0f, 1.0f, 0.0f },
                    { 1.0f, -1.0f, 1.0f },
                    { 1.0f, -1.0f, -1.0f },
                };

                // Draw yellow triangles for the pyramid
                glColor3f(1.0f, 1.0f, 0.0f);

                glBegin(GL_TRIANGLES);
                glNormal3f(0.0f, 0.447214f, 0.894427f);
                glVertex3fv((GLfloat*)&pyramidVertices[0]);
                glVertex3fv((GLfloat*)&pyramidVertices[1]);
                glVertex3fv((GLfloat*)&pyramidVertices[2]);

                glNormal3f(-0.894427f, 0.447214f, 0.0f);
                glVertex3fv((GLfloat*)&pyramidVertices[3]);
                glVertex3fv((GLfloat*)&pyramidVertices[4]);
                glVertex3fv((GLfloat*)&pyramidVertices[5]);

                glNormal3f(0.0f, 0.447214f, -0.894427f);
                glVertex3fv((GLfloat*)&pyramidVertices[6]);
                glVertex3fv((GLfloat*)&pyramidVertices[7]);
                glVertex3fv((GLfloat*)&pyramidVertices[8]);

                glNormal3f(0.894427f, 0.447214f, 0.0f);
                glVertex3fv((GLfloat*)&pyramidVertices[9]);
                glVertex3fv((GLfloat*)&pyramidVertices[10]);
                glVertex3fv((GLfloat*)&pyramidVertices[11]);
                glEnd();

                result = RENDERER_RESULT_SUCCESS;
            }
            return result;
        }

        /// Generic method to notify renderers about a message posted to renderer window. The message can be
        /// from system or initiated by the host itself.
        /// @param message OS dependent structure that describes the system message being processed.
        void OnMessage(const Message* message) override
        {
        }
    };

}
