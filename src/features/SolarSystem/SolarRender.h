#pragma once

#include <Renderer.h>
#include <gl\GL.h>
#include <gl\GLU.h>
namespace Features
{
<<<<<<< HEAD
	using namespace Interfaces;

	class SolarSystemRenderer : public IRenderer
	{
	private:
		GLfloat			g_Angle;
		GLfloat 		g_Mercury;
		GLfloat 		g_Venus;
		GLfloat 		g_Earth;
		GLfloat 		g_Mars;
		GLfloat 		g_Jupiter;
		GLfloat 		g_Saturn;
		GLfloat 		g_Uranus;
		GLfloat 		g_Neptune;
		GLuint			gbTexture[10];
		GLUquadric		*pQuadric[9];
	public:
		void update();
		int LoadTexture(GLuint *texture, char *szPath);
		const char* GetName() override;
		RendererResult Initialize(Window window) override;
		void Uninitialize(void) override;
		RendererResult InitializeScene(SceneType scene) override;
		void UninitializeScene(SceneType scene) override;
		void mercury();
		void venus();
		void earth();
		void mars();
		void jupiter();
		void saturn();
		void uranus();
		void neptune();
		void sun();
		void stars();
		virtual RendererResult Render(const RenderParams &params) override;
		void OnMessage(const Message &message) override;
		void OnResize(unsigned int width, unsigned int height) override;
};
}

	
=======
    using namespace Interfaces;

    class SolarSystemRenderer : public IRenderer
    {
    private:
        GLfloat         g_Angle;
        GLfloat         g_Mercury;
        GLfloat         g_Venus;
        GLfloat         g_Earth;
        GLfloat         g_Mars;
        GLfloat         g_Jupiter;
        GLfloat         g_Saturn;
        GLfloat         g_Uranus;
        GLfloat         g_Neptune;
        GLuint          gbTexture[10];
        GLUquadric      *pQuadric[9];
    public:
        void update();
        int LoadTexture(GLuint *texture, char *szPath);
        const char* GetName() override;
        RendererResult Initialize(Window window) override;
        void Uninitialize(void) override;
        RendererResult InitializeScene(SceneType scene) override;
        void UninitializeScene(SceneType scene) override;
        void mercury();
        void venus();
        void earth();
        void mars();
        void jupiter();
        void saturn();
        void uranus();
        void neptune();
        void sun();
        void stars();
        virtual RendererResult Render(const RenderParams &params) override;
        void OnMessage(const Message &message) override;
        void OnResize(unsigned int width, unsigned int height) override;
};
}

>>>>>>> 262f5631734c516bd6e17471498adc4118646d03
#pragma once
