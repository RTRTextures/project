

#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<memory.h>
#include<X11/Xlib.h>
#include<X11/Xutil.h>
#include<X11/XKBlib.h>
#include<X11/keysym.h>
#include<GL/glew.h>
#include<GL/gl.h>
#include<GL/glx.h>
#include "Host.h"

namespace Framework
{
	
	typedef 	GLXContext(*glXCreateContextAttribsARBProc)(Display *, GLXFBConfig, GLXContext, Bool, const int *);


	class XWindowsHost : public Host
	{
		char m_className[128];
		Display *m_pDisplay;
		XVisualInfo *m_pXVisualInfo;
		Colormap m_hColormap;
		GLXFBConfig m_hGLXFBConfig;
		glXCreateContextAttribsARBProc	m_glXCreateContextAttribsARB;
		GLXContext 						m_hGLXContext;
		bool    m_isFullscreen;
		long   m_windowStyle;

	

	public:
		XWindowsHost() :
			m_pDisplay(NULL),
			m_pXVisualInfo(NULL),
			m_glXCreateContextAttribsARB(NULL),
			m_isFullscreen(false),
			m_windowStyle(0)
		{
			
		}

		~XWindowsHost()
		{
			if (m_className[0] != '\0')
			{
				//UnregisterClass(m_className, (HINSTANCE)GetModuleHandle(NULL));
				memset(m_className, 0, sizeof(m_className));
			}
		}

		void ToggleFullscreen()
		{
			Atom  wm_state;
			Atom  fullscreen;
			XEvent  xev = { 0 };

			wm_state = XInternAtom(m_pDisplay, "_NET_WM_STATE", False);
			memset(&xev, 0, sizeof(xev));
			xev.type = ClientMessage;
			xev.xclient.window = m_window;
			xev.xclient.message_type = wm_state;
			xev.xclient.format = 32;
			xev.xclient.data.l[0] = m_isFullscreen ? 0 : 1;
			fullscreen = XInternAtom(m_pDisplay, "_NET_WM_STATE_FULLSCREEN", False);
			xev.xclient.data.l[1] = fullscreen;
			XSendEvent(m_pDisplay, RootWindow(m_pDisplay, m_pXVisualInfo->screen), False, StructureNotifyMask, &xev);
			m_isFullscreen = !m_isFullscreen;
			
		}


	protected:

		void CreateDrawable() override
		{
			XSetWindowAttributes 	winAttribs;
			GLXFBConfig 			*pGLXFBConfigs = NULL;
			GLXFBConfig 			bestGLXFBConfig;
			XVisualInfo 			*pTempXViualInfo = NULL;
			int 					iNumFBConfig = 0;
			int 					styleMask;
			int 					i;

			static int frameBufferAttributes[] =
			{
				GLX_X_RENDERABLE, True,
				GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
				GLX_RENDER_TYPE, GLX_RGBA_BIT,
				GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR,
				GLX_RED_SIZE, 8,
				GLX_GREEN_SIZE, 8,
				GLX_BLUE_SIZE, 8,
				GLX_ALPHA_SIZE, 8,
				GLX_DEPTH_SIZE, 24,
				GLX_STENCIL_SIZE, 8,
				GLX_DOUBLEBUFFER, True,
				//GLX_SAMPLE_BUFFERS, 1
				//GLX_SAMPLES
				None
			};

			m_pDisplay = XOpenDisplay(NULL);

			if (!m_pDisplay)
			{
				printf("ERROR : Unable To Obtain X Display.\n");
				exit(1);
			}

			pGLXFBConfigs = glXChooseFBConfig(m_pDisplay, DefaultScreen(m_pDisplay), frameBufferAttributes, &iNumFBConfig);

			if (!pGLXFBConfigs)
			{
				printf("Failed To Get Valid Framebuffer Config. Exitting Now ...\n");
				exit(1);
			}
			int 	bestFramebufferconfig = -1;
			int 	worstFramebufferconfig = -1;
			int 	bestNumberOfSamples = -1;
			int 	worstNumberOfSamples = 999;

			for (i = 0; i < iNumFBConfig; i++)
			{
				pTempXViualInfo = glXGetVisualFromFBConfig(m_pDisplay, pGLXFBConfigs[i]);
				if (pTempXViualInfo)
				{
					int sampleBuffer, samples;
					glXGetFBConfigAttrib(m_pDisplay, pGLXFBConfigs[i], GLX_SAMPLE_BUFFERS, &sampleBuffer);
					glXGetFBConfigAttrib(m_pDisplay, pGLXFBConfigs[i], GLX_SAMPLES, &samples);
					printf("Matching Framebuffer Config=%d : Visual ID=0x%lu : SAMPLE BUFFERS=%d : SAMPLES=%d\n", i, pTempXViualInfo->visualid, sampleBuffer, samples);
					if (bestFramebufferconfig < 0 || sampleBuffer && samples > bestNumberOfSamples)
					{
						bestFramebufferconfig = i;
						bestNumberOfSamples = samples;
					}
					if (worstFramebufferconfig < 0 || !sampleBuffer || samples < worstNumberOfSamples)
					{
						worstFramebufferconfig = i;
						worstNumberOfSamples = samples;
					}
				}
				XFree(pTempXViualInfo);
			}

			bestGLXFBConfig = pGLXFBConfigs[bestFramebufferconfig];

			m_hGLXFBConfig	= bestGLXFBConfig;

			XFree(pGLXFBConfigs);

			m_pXVisualInfo = glXGetVisualFromFBConfig(m_pDisplay, bestGLXFBConfig);
			// setting window's attributes
			winAttribs.border_pixel = 0;
			winAttribs.background_pixmap = 0;
			winAttribs.colormap = XCreateColormap(
													m_pDisplay,
													RootWindow(m_pDisplay, m_pXVisualInfo->screen), // you can give defaultScreen as well
													m_pXVisualInfo->visual,
													AllocNone
												); // for 'movable' memory allocation

			winAttribs.event_mask = StructureNotifyMask | KeyPressMask | ButtonPressMask | ExposureMask | VisibilityChangeMask | PointerMotionMask;

			styleMask = CWBorderPixel | CWEventMask | CWColormap;

			m_hColormap = winAttribs.colormap;
			m_window = XCreateWindow(
										m_pDisplay,
										RootWindow(m_pDisplay, m_pXVisualInfo->screen),
										0,
										0,
										WIN_WIDTH,
										WIN_HEIGHT,
										0, // border width
										m_pXVisualInfo->depth, // depth of visual (depth for Colormap)
										InputOutput, // class(type) of your window
										m_pXVisualInfo->visual,
										styleMask,
										&winAttribs
									);

			XStoreName(m_pDisplay, m_window, "1 PP Blue Window");

			Atom windowManagerDelete = XInternAtom(m_pDisplay, "WM_WINDOW_DELETE", True);
			XSetWMProtocols(m_pDisplay, m_window, &windowManagerDelete, 1);

			XMapWindow(m_pDisplay, m_window);
						
		}

		void DestroyDrawable() override
		{
			
			XDestroyWindow(m_pDisplay, m_window);
		}

		bool InitGL() override
		{
			m_glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc)glXGetProcAddressARB((GLubyte *)"glXCreateContextAttribsARB");
			GLint attribs[] = {
								GLX_CONTEXT_MAJOR_VERSION_ARB,3,
								GLX_CONTEXT_MINOR_VERSION_ARB,1,
								GLX_CONTEXT_PROFILE_MASK_ARB, GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
								0 // array must be terminated by 0
							};
			m_hGLXContext = m_glXCreateContextAttribsARB(m_pDisplay, m_hGLXFBConfig, 0, True, attribs);

			if (!m_hGLXContext)
			{
				GLint attribs[] = {
					GLX_CONTEXT_MAJOR_VERSION_ARB, 1,
					GLX_CONTEXT_MINOR_VERSION_ARB, 0,
					0
				};
				printf("Failed To Create GLX 4.5 context. Hence Using Old-Style GLX Context\n");
				m_hGLXContext = m_glXCreateContextAttribsARB(m_pDisplay, m_hGLXFBConfig, 0, True, attribs);

			}
			else
			{
				printf("OpenGL Context 3.1 Is Created.\n");
			}


			if (!glXIsDirect(m_pDisplay, m_hGLXContext))
			{
				printf("Indirect GLX Rendering Context Obtained\n");
			}
			else
			{
				printf("Direct GLX Rendering Context Obtained\n");
			}

			glXMakeCurrent(m_pDisplay, m_window, m_hGLXContext);
			return true;
			
		}

		void UninitGL() override
		{
			GLXContext currentGLXContext;
			currentGLXContext = glXGetCurrentContext();
			if (currentGLXContext != NULL && currentGLXContext == m_hGLXContext)
				glXMakeCurrent(m_pDisplay, 0, 0);
			if (m_hGLXContext)
				glXDestroyContext(m_pDisplay, m_hGLXContext);
			
			if (m_window)
				XDestroyWindow(m_pDisplay, m_window);

			if (m_hColormap)
				XFreeColormap(m_pDisplay, m_hColormap);

			if (m_pXVisualInfo)
			{
				free(m_pXVisualInfo);
				m_pXVisualInfo = NULL;
			}

			if (m_pDisplay)
			{
				XCloseDisplay(m_pDisplay);
				m_pDisplay = NULL;
			}


		}

		void SwapBuffers() override
		{
			glXSwapBuffers(m_pDisplay, m_window);
		}

		bool PumpMessage() override
		{
			bool messagefetched = false;
			XEvent event;
			KeySym keysym;
			while (XPending(m_pDisplay))
			{
				XNextEvent(m_pDisplay, &event);
				switch (event.type)
				{
				case MapNotify:
					m_isActive = true;
					break;
				case KeyPress:
					keysym = XkbKeycodeToKeysym(m_pDisplay, event.xkey.keycode, 0, 0);
					switch (keysym)
					{
					case XK_Escape:
						m_exitLoop = true;
						break;
					case XK_F:
					case XK_f:
						ToggleFullscreen();
						break;
					default:
						break;
					}
					break;
				case ButtonPress:
					switch (event.xbutton.button)
					{
					case 1:break;
					case 2:break;
					case 3:break;
					}
					break;
				case MotionNotify:
					break;
				case ConfigureNotify:
					Resize((unsigned long)event.xconfigure.width, (unsigned long)event.xconfigure.height);
					break;
				case Expose:
					break;
				case DestroyNotify:
					break;
				case 33:
					m_exitLoop = true;
					break;
				default:
					break;
				}//event.type
				messagefetched = true;
			}//XPending
			 
			return messagefetched;
		}

	};
}

using namespace Framework;

int main(int argc, char* argv[])
{
	XWindowsHost host;
	bool bResult = host.Run();
	return 1;
}