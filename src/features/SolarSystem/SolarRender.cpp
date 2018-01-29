#pragma once

#include <Renderer.h>
#include <gl\GL.h>
#include <gl\GLU.h>
/* file for SOIL library to load the image*/
<<<<<<< HEAD
#include "..\tools\soil\src\SOIL.h"
=======
#include <SOIL.h>
>>>>>>> 262f5631734c516bd6e17471498adc4118646d03
#include"features\SolarSystem\SolarRender.h"
using namespace Features;

void SolarSystemRenderer::update()
{
<<<<<<< HEAD
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
int SolarSystemRenderer::LoadTexture(GLuint *texture, char *szPath)
{
	int 	iTextureWidth, iTextureHeight;
	unsigned char 	*pImageData = NULL;
	int iStatus = FALSE;
	pImageData = SOIL_load_image((const char*)szPath, &iTextureWidth, &iTextureHeight, 0, SOIL_LOAD_RGB);
	if (pImageData!=NULL)
	{

		iStatus = TRUE;
		glGenTextures(1, texture);
		glBindTexture(GL_TEXTURE_2D, *texture);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, iTextureWidth, iTextureHeight, GL_RGB, GL_UNSIGNED_BYTE, (void *)pImageData);
		SOIL_free_image_data(pImageData); // free the imageData
	}
	return(iStatus);
=======
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
int SolarSystemRenderer::LoadTexture(GLuint *texture, char *szPath)
{
    int iTextureWidth, iTextureHeight;
    unsigned char *pImageData = NULL;
    int iStatus = FALSE;
    pImageData = SOIL_load_image((const char*)szPath, &iTextureWidth, &iTextureHeight, 0, SOIL_LOAD_RGB);
    if (pImageData != NULL)
    {
        iStatus = TRUE;
        glGenTextures(1, texture);
        glBindTexture(GL_TEXTURE_2D, *texture);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        gluBuild2DMipmaps(GL_TEXTURE_2D, 3, iTextureWidth, iTextureHeight, GL_RGB, GL_UNSIGNED_BYTE, (void *)pImageData);
        SOIL_free_image_data(pImageData); // free the imageData
    }
    return(iStatus);
>>>>>>> 262f5631734c516bd6e17471498adc4118646d03
}

const char* SolarSystemRenderer::GetName()
{
<<<<<<< HEAD
	static const char name[] = "SolarSystemRenderer";
	return name;
=======
    static const char name[] = "SolarSystemRenderer";
    return name;
>>>>>>> 262f5631734c516bd6e17471498adc4118646d03
}

RendererResult SolarSystemRenderer::Initialize(Window window)
{
<<<<<<< HEAD
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
	LoadTexture(&gbTexture[0], "\\git\\project\\src\\features\\SolarSystem\\earth.bmp");
	LoadTexture(&gbTexture[1], "\\git\\project\\src\\features\\SolarSystem\\mercury.bmp");
	LoadTexture(&gbTexture[2], "\\git\\project\\src\\features\\SolarSystem\\venus.bmp");
	LoadTexture(&gbTexture[3], "\\git\\project\\src\\features\\SolarSystem\\mars.bmp");
	LoadTexture(&gbTexture[4], "\\git\\project\\src\\features\\SolarSystem\\jupiter.bmp");
	LoadTexture(&gbTexture[5], "\\git\\project\\src\\features\\SolarSystem\\saturn.bmp");
	LoadTexture(&gbTexture[6], "\\git\\project\\src\\features\\SolarSystem\\uranus.bmp");
	LoadTexture(&gbTexture[7], "\\git\\project\\src\\features\\SolarSystem\\neptune.bmp");
	LoadTexture(&gbTexture[8], "\\git\\project\\src\\features\\SolarSystem\\sun.bmp");
	LoadTexture(&gbTexture[9], "\\git\\project\\src\\features\\SolarSystem\\stars.bmp");
	for (int i = 0; i < 9; i++)
	{
		pQuadric[i] = gluNewQuadric();
		if (pQuadric[i] == NULL)
			return RENDERER_RESULT_ERROR;
	}
	return RENDERER_RESULT_SUCCESS;
=======
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
    memset(gbTexture, 0, sizeof(gbTexture));

    LoadTexture(&gbTexture[0], "res\\SolarSystem\\earth.bmp");
    LoadTexture(&gbTexture[1], "res\\SolarSystem\\mercury.bmp");
    LoadTexture(&gbTexture[2], "res\\SolarSystem\\venus.bmp");
    LoadTexture(&gbTexture[3], "res\\SolarSystem\\mars.bmp");
    LoadTexture(&gbTexture[4], "res\\SolarSystem\\jupiter.bmp");
    LoadTexture(&gbTexture[5], "res\\SolarSystem\\saturn.bmp");
    LoadTexture(&gbTexture[6], "res\\SolarSystem\\uranus.bmp");
    LoadTexture(&gbTexture[7], "res\\SolarSystem\\neptune.bmp");
    LoadTexture(&gbTexture[8], "res\\SolarSystem\\sun.bmp");
    LoadTexture(&gbTexture[9], "res\\SolarSystem\\stars.bmp");
    for (int i = 0; i < 9; i++)
    {
        pQuadric[i] = gluNewQuadric();
        if (pQuadric[i] == NULL)
            return RENDERER_RESULT_ERROR;
    }
    return RENDERER_RESULT_SUCCESS;
>>>>>>> 262f5631734c516bd6e17471498adc4118646d03
}

void SolarSystemRenderer::Uninitialize(void)
{
<<<<<<< HEAD
=======
    glDeleteTextures(_countof(gbTexture), gbTexture);
    memset(gbTexture, 0, sizeof(gbTexture));
>>>>>>> 262f5631734c516bd6e17471498adc4118646d03
}

RendererResult SolarSystemRenderer::InitializeScene(SceneType scene)
{
<<<<<<< HEAD
	return RENDERER_RESULT_SUCCESS;
=======
    return RENDERER_RESULT_SUCCESS;
>>>>>>> 262f5631734c516bd6e17471498adc4118646d03
}

void SolarSystemRenderer::UninitializeScene(SceneType scene)
{
}

void SolarSystemRenderer::mercury()
{
<<<<<<< HEAD
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
=======
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
>>>>>>> 262f5631734c516bd6e17471498adc4118646d03
}

void SolarSystemRenderer::venus()
{
<<<<<<< HEAD
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
=======
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
>>>>>>> 262f5631734c516bd6e17471498adc4118646d03
}


void SolarSystemRenderer::earth()
{
<<<<<<< HEAD
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
=======
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
>>>>>>> 262f5631734c516bd6e17471498adc4118646d03
}


void SolarSystemRenderer::mars()
{
<<<<<<< HEAD
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
=======
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
>>>>>>> 262f5631734c516bd6e17471498adc4118646d03
}

void SolarSystemRenderer::jupiter()
{
<<<<<<< HEAD
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
=======
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
>>>>>>> 262f5631734c516bd6e17471498adc4118646d03
}

void SolarSystemRenderer::saturn()
{
<<<<<<< HEAD
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
=======
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
>>>>>>> 262f5631734c516bd6e17471498adc4118646d03
}

void SolarSystemRenderer::uranus()
{
<<<<<<< HEAD
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
=======
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
>>>>>>> 262f5631734c516bd6e17471498adc4118646d03

}

void SolarSystemRenderer::neptune()
{
<<<<<<< HEAD
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
void SolarSystemRenderer::sun()
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
=======
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
void SolarSystemRenderer::sun()
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
>>>>>>> 262f5631734c516bd6e17471498adc4118646d03

}
void SolarSystemRenderer::stars()
{
<<<<<<< HEAD
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
RendererResult SolarSystemRenderer::Render(const RenderParams &params)
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
=======
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
RendererResult SolarSystemRenderer::Render(const RenderParams &params)
{
    RendererResult result = RENDERER_RESULT_FINISHED;
    update();
    // update for 30 secs
    if (params.scene == SCENE_SOLAR_SYSTEM && params.elapsedFromEpicStart < SECS_TO_MICROSECS(30))
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
>>>>>>> 262f5631734c516bd6e17471498adc4118646d03
}

void SolarSystemRenderer::OnMessage(const Message &message)
{
<<<<<<< HEAD
			return;
=======
    return;
>>>>>>> 262f5631734c516bd6e17471498adc4118646d03
}

void SolarSystemRenderer::OnResize(unsigned int width, unsigned int height)
{
<<<<<<< HEAD
	// select projection matrix
	glMatrixMode(GL_PROJECTION);
	// reset projection matrix
	glLoadIdentity();
	// calculate the aspect ratio of the view
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
=======
    // select projection matrix
    glMatrixMode(GL_PROJECTION);
    // reset projection matrix
    glLoadIdentity();
    // calculate the aspect ratio of the view
    gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
>>>>>>> 262f5631734c516bd6e17471498adc4118646d03
}
#pragma once
