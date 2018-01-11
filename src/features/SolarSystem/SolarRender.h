#pragma once

#include <Renderer.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include"features\SolarSystem\SolarResource.h"
namespace Features
{
	using namespace Interfaces;

	class SolarSystemRenderer : public IRenderer
	{
	private:
		float		g_Angle;
		float 		g_Mercury;
		float 		g_Venus;
		float 		g_Earth;
		float 		g_Mars;
		float 		g_Jupiter;
		float 		g_Saturn;
		float 		g_Uranus;
		float 		g_Neptune;
		GLuint		gbTexture[10];
		GLUquadric	*pQuadric[9];
	public:
		void update()
		{
			g_Angle = g_Angle + 0.1f;
			if (g_Angle >= 360)
				g_Angle = 0.1f;

			g_Mercury = g_Mercury + 0.7f;
			g_Venus = g_Venus + 0.3f;
			g_Earth = g_Earth + 0.5f;
			g_Mars = g_Mars + 0.3f;
			g_Jupiter = g_Jupiter + 0.5f;
			g_Saturn = g_Saturn + 0.6f;
			g_Uranus = g_Uranus + 0.7f;
			g_Neptune = g_Neptune + 0.5f;
		}
		int LoadTexture(GLuint *texture, TCHAR resource[])
		{
			HBITMAP hBitmap;
			BITMAP bmp;
			int iStatus = FALSE;

			glGenTextures(1, texture);
			hBitmap = (HBITMAP)LoadImage(GetModuleHandle(NULL), resource, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
			if (hBitmap)
			{
				iStatus = TRUE;
				GetObject(hBitmap, sizeof(bmp), &bmp);
				glBindTexture(GL_TEXTURE_2D, *texture);
				glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

				gluBuild2DMipmaps(GL_TEXTURE_2D, 3, bmp.bmWidth, bmp.bmHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, bmp.bmBits);

				DeleteObject(hBitmap);
			}

			return(iStatus);
		}


		const char* GetName() override
		{
			static const char name[] = "SolarSystemRenderer";
			return name;
		}

		RendererResult Initialize(Window window) override
		{
			glEnable(GL_TEXTURE_2D);
			g_Mercury = 0.0f;
			g_Venus = 0.0f;
			g_Earth = 0.0f;
			g_Mars = 0.0f;
			g_Jupiter = 0.0f;
			g_Saturn = 0.0f;
			g_Uranus = 0.0f;
			g_Neptune = 0.0f;
			g_Angle = 0.0f;

			LoadTexture(&gbTexture[0], MAKEINTRESOURCE(EARTH_BITMAP));
			LoadTexture(&gbTexture[1], MAKEINTRESOURCE(MERCURY_BITMAP));
			LoadTexture(&gbTexture[2], MAKEINTRESOURCE(VENUS_BITMAP));
			LoadTexture(&gbTexture[3], MAKEINTRESOURCE(MARS_BITMAP));
			LoadTexture(&gbTexture[4], MAKEINTRESOURCE(JUPITER_BITMAP));
			LoadTexture(&gbTexture[5], MAKEINTRESOURCE(SATURN_BITMAP));
			LoadTexture(&gbTexture[6], MAKEINTRESOURCE(URANUS_BITMAP));
			LoadTexture(&gbTexture[7], MAKEINTRESOURCE(NEPTUNE_BITMAP));
			LoadTexture(&gbTexture[8], MAKEINTRESOURCE(SUN_BITMAP));
			LoadTexture(&gbTexture[9], MAKEINTRESOURCE(STARS_BITMAP));


			for (int i = 0; i < 9; i++)
				pQuadric[i] = gluNewQuadric();

			return RENDERER_RESULT_SUCCESS;
		}

		void Uninitialize(void) override
		{
		}

		RendererResult InitializeScene(SceneType scene) override
		{
			return RENDERER_RESULT_SUCCESS;
		}

		void UninitializeScene(SceneType scene) override
		{
		}

		void mercury()
		{
			glPushMatrix();
				glRotatef(g_Mercury, 0.0f, 0.0f, 1.0f);
				glTranslatef(-0.15f, 0.0f, -1.0);
				glScalef(0.02f, 0.02f, 0.02f);

				glRotatef(95, 1.0, 0.0, 0.0);
				glRotatef(180, 0.0, 1.0, 0.0);
				glRotatef(g_Angle, 0.0, 0.0, 1.0);
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glBindTexture(GL_TEXTURE_2D, gbTexture[1]);
				gluQuadricTexture(pQuadric[1], 1);
				gluSphere(pQuadric[1], 0.75f, 100, 100);
			glPopMatrix();
		}

		void venus()
		{
			glPushMatrix();
				glRotatef(g_Venus, 0.0f, 0.0f, 1.0f);
				glTranslatef(-0.24f, 0.0f, -1.0);
				glScalef(0.05f, 0.05f, 0.05f);
				glRotatef(95, 1.0, 0.0, 0.0);
				glRotatef(180, 0.0, 1.0, 0.0);
				glRotatef(g_Angle, 0.0, 0.0, 1.0);
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glBindTexture(GL_TEXTURE_2D, gbTexture[2]);
				gluQuadricTexture(pQuadric[2], 1);
				gluSphere(pQuadric[2], 0.75f, 100, 100);
			glPopMatrix();
		}

		void earth()
		{
			glPushMatrix();
				glRotatef(g_Earth, 0.0f, 0.0f, 1.0f);
				glTranslatef(-0.33f, 0.0f, -1.0);
				glScalef(0.05f, 0.05f, 0.05f);
				glRotatef(95, 1.0, 0.0, 0.0);
				glRotatef(180, 0.0, 1.0, 0.0);

				glRotatef(g_Angle, 0.0, 0.0, 1.0f);
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glBindTexture(GL_TEXTURE_2D, gbTexture[0]);
				gluQuadricTexture(pQuadric[0], 1);
				gluSphere(pQuadric[0], 0.75f, 100, 100);
			glPopMatrix();

		}



		void mars()
		{
			glPushMatrix();
				glRotatef(g_Mars, 0.0f, 0.0f, 1.0f);
				glTranslatef(-0.43f, 0.0f, -1.0);
				glScalef(0.05f, 0.05f, 0.05f);
				glRotatef(95, 1.0, 0.0, 0.0);
				glRotatef(180, 0.0, 1.0, 0.0);
				glRotatef(g_Angle, 0.0, 0.0, 1.0);
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glBindTexture(GL_TEXTURE_2D, gbTexture[3]);
				gluQuadricTexture(pQuadric[3], 1);
				gluSphere(pQuadric[3], 0.75f, 100, 100);
			glPopMatrix();
		}

		void jupiter()
		{
			glPushMatrix();
				glRotatef(g_Jupiter, 0.0f, 0.0f, 1.0f);
				glTranslatef(-0.53f, 0.0f, -1.0);

				glScalef(0.05f, 0.05f, 0.05f);
				glRotatef(95, 1.0, 0.0, 0.0);
				glRotatef(180, 0.0, 1.0, 0.0);
				glRotatef(g_Angle, 0.0, 0.0, 1.0);
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glBindTexture(GL_TEXTURE_2D, gbTexture[4]);
				gluQuadricTexture(pQuadric[4], 1);
				gluSphere(pQuadric[4], 0.75f, 100, 100);
			glPopMatrix();

		}


		void saturn()
		{
			glPushMatrix();
				glRotatef(g_Saturn, 0.0f, 0.0f, 1.0f);
				glTranslatef(-0.65f, 0.0f, -1.0);
				glScalef(0.05f, 0.05f, 0.05f);
				glRotatef(95, 1.0, 0.0, 0.0);
				glRotatef(180, 0.0, 1.0, 0.0);
				glRotatef(g_Angle, 0.0, 0.0, 1.0);
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glBindTexture(GL_TEXTURE_2D, gbTexture[5]);
				gluQuadricTexture(pQuadric[5], 1);
				gluSphere(pQuadric[5], 0.75f, 100, 100);
			glPopMatrix();


		}

		void uranus()
		{
			glPushMatrix();
				glRotatef(g_Uranus, 0.0f, 0.0f, 1.0f);
				glTranslatef(-0.73f, 0.0f, -1.0);
				glScalef(0.05f, 0.05f, 0.05f);
				glRotatef(95, 1.0, 0.0, 0.0);
				glRotatef(180, 0.0, 1.0, 0.0);
				glRotatef(g_Angle, 0.0, 0.0, 1.0);
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glBindTexture(GL_TEXTURE_2D, gbTexture[6]);
				gluQuadricTexture(pQuadric[6], 1);
				gluSphere(pQuadric[6], 0.75f, 100, 100);
			glPopMatrix();

		}

		void neptune()
		{
			glPushMatrix();
				glRotatef(g_Neptune, 0.0f, 0.0f, 1.0f);
				glTranslatef(-0.83f, 0.0f, -1.0);
				glScalef(0.05f, 0.05f, 0.05f);
				glRotatef(95, 1.0, 0.0, 0.0);
				glRotatef(180, 0.0, 1.0, 0.0);
				glRotatef(g_Angle, 0.0, 0.0, 1.0);
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glBindTexture(GL_TEXTURE_2D, gbTexture[7]);
				gluQuadricTexture(pQuadric[7], 1);
				gluSphere(pQuadric[7], 0.75f, 100, 100);
			glPopMatrix();
		}



		void sun()
		{
			glPushMatrix();
				glTranslatef(-0.01f, 0.02f, -2.0);
				glScalef(0.15f, 0.15f, 0.15f);

				glRotatef(95, 1.0, 0.0, 0.0);
				glRotatef(180, 0.0, 1.0, 0.0);
				glRotatef(g_Angle, 0.0, 0.0, 1.0);
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glBindTexture(GL_TEXTURE_2D, gbTexture[8]);
				gluQuadricTexture(pQuadric[8], 1);
				gluSphere(pQuadric[8], 0.75f, 100, 100);
			glPopMatrix();

		}



		void stars()
		{
			glPushMatrix();
			glTranslatef(0.0f, 0.0f, -1.0f);
			glBindTexture(GL_TEXTURE_2D, gbTexture[9]);
			glBegin(GL_QUADS);
				glTexCoord2f(1.0f, 0.0f);
				glVertex3f(1.0f, 1.0f, -1.0f); 

				glTexCoord2f(1.0f, 1.0f);
				glVertex3f(-1.0f, 1.0f, -1.0f);

				glTexCoord2f(0.0f, 1.0f);
				glVertex3f(-1.0f, -1.0f, -1.0f);

				glTexCoord2f(0.0f, 0.0f);
				glVertex3f(1.0f, -1.0f, -1.0f);
			glEnd();
			glPopMatrix();

		}


		virtual RendererResult Render(const RenderParams &params) override
		{
			RendererResult result = RENDERER_RESULT_FINISHED;
			update();
			if (params.scene == SCENE_SOLAR_SYSTEM)
			{
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				glMatrixMode(GL_MODELVIEW);
				glLoadIdentity();
				gluLookAt(0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
				stars();
				sun();
				earth();
				mercury();
				venus();

				mars();
				jupiter();
				saturn();
				uranus();
				neptune();

				result = RENDERER_RESULT_SUCCESS;
			}
			return result;
		}
		
		void OnMessage(const Message &message) override
		{
			return;
		}

		void OnResize(unsigned int width, unsigned int height) override
		{
			// select projection matrix
			glMatrixMode(GL_PROJECTION);
			// reset projection matrix
			glLoadIdentity();
			// calculate the aspect ratio of the view
			gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
		}
	};
}

	
#pragma once
