
#include "Host.h"
#include <Windows.h>

namespace Framework
{
    class WindowsHost : public Host
    {
        wchar_t m_className[128];
        HDC     m_hdc;
        HGLRC   m_hrc;
        bool    m_isFullscreen;
        DWORD   m_windowStyle;
        WINDOWPLACEMENT m_wpPrev;

    public:
        WindowsHost() :
            m_hdc(NULL),
            m_hrc(NULL),
            m_isFullscreen(false),
            m_windowStyle(0)
        {
            ZeroMemory(&m_wpPrev, sizeof(m_wpPrev));
        }

        ~WindowsHost()
        {
            if (m_className[0] != L'\0')
            {
                UnregisterClass(m_className, (HINSTANCE)GetModuleHandle(NULL));
                memset(m_className, 0, sizeof(m_className));
            }
        }

        static LRESULT CALLBACK WndProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
        {
            WindowsHost *pThis = reinterpret_cast<WindowsHost*>(GetWindowLongPtr(window, GWLP_USERDATA));
            if (pThis != nullptr)
            {
                pThis->MessageHandler(message, wParam, lParam);
            }
            return DefWindowProc(window, message, wParam, lParam);
        }

        void MessageHandler(UINT message, WPARAM wParam, LPARAM lParam)
        {
            Message messageStruct = { message, wParam, lParam };
            OnMessage(messageStruct);

            switch (message)
            {
            case WM_ACTIVATE:
                if (HIWORD(wParam) == 0)
                    m_isActive = true;
                else
                    m_isActive = false;
                break;
            case WM_SIZE:
                Resize(LOWORD(lParam), HIWORD(lParam));
                break;
            case WM_KEYDOWN:
                switch (wParam)
                {
                case VK_ESCAPE:
                    m_exitLoop = true;
                    break;
                }
                break;
            case WM_CHAR:
                switch (wParam)
                {
                case 'F':
                case 'f':
                    ToggleFullscreen();
                    break;
                }
                break;
            case WM_DESTROY:
                m_exitLoop = true;
                break;
            }
        }

    protected:

        void CreateDrawable() override
        {
            // initialize variables required for creating window
            HINSTANCE instance = (HINSTANCE)GetModuleHandle(NULL);
            swprintf(m_className, _countof(m_className), L"RTRTexturesWindow%08X", GetCurrentProcessId());

            WNDCLASSEX wndclass = { 0 };
            wndclass.cbSize = sizeof(WNDCLASSEX);
            wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
            wndclass.cbClsExtra = 0;
            wndclass.cbWndExtra = 0;
            wndclass.hInstance = instance;
            wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
            wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
            wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
            wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
            wndclass.lpfnWndProc = WndProc;
            wndclass.lpszClassName = m_className;
            wndclass.lpszMenuName = NULL;

            RegisterClassEx(&wndclass);

            m_window = CreateWindowEx(WS_EX_APPWINDOW,
                m_className,
                L"RTR-Textures Project Host",
                WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE,
                0,
                0,
                WIN_WIDTH,
                WIN_HEIGHT,
                NULL,
                NULL,
                instance,
                NULL);

            if (m_window != NULL)
            {
                // set context to be used in windowproc
                (void)SetWindowLongPtr(m_window, GWLP_USERDATA, (LONG_PTR)this);

                ShowWindow(m_window, SW_SHOW);
                SetForegroundWindow(m_window);
                SetFocus(m_window);
            }
        }

        void DestroyDrawable() override
        {
            DestroyWindow(m_window);
        }

        bool InitGL() override
        {
            bool ret = false;
            PIXELFORMATDESCRIPTOR pfd = { 0 };
            int iPixelFormatIndex;

            //Initialization of structure 'PIXELFORMATDESCRIPTOR'
            pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
            pfd.nVersion = 1;
            pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
            pfd.iPixelType = PFD_TYPE_RGBA;
            pfd.cColorBits = 32;
            pfd.cRedBits = 8;
            pfd.cGreenBits = 8;
            pfd.cBlueBits = 8;
            pfd.cAlphaBits = 8;
            pfd.cDepthBits = 32;

            m_hdc = GetDC(m_window);

            iPixelFormatIndex = ChoosePixelFormat(m_hdc, &pfd);
            if (iPixelFormatIndex != 0)
            {
                if (SetPixelFormat(m_hdc, iPixelFormatIndex, &pfd) == TRUE)
                {
                    m_hrc = wglCreateContext(m_hdc);
                    if (m_hrc != NULL)
                    {
                        ret = wglMakeCurrent(m_hdc, m_hrc) == TRUE;
                    }
                }
            }
            return ret;
        }

		
        void UninitGL() override
        {
            if (m_hrc != NULL)
            {
                wglMakeCurrent(NULL, NULL);
                wglDeleteContext(m_hrc);
                m_hrc = NULL;
            }

            if (m_hdc != NULL)
            {
                ReleaseDC(m_window, m_hdc);
                m_hdc = NULL;
            }
        }

        void ToggleFullscreen()
        {
            MONITORINFO mi;
            if (!m_isFullscreen)
            {
                m_windowStyle = GetWindowLong(m_window, GWL_STYLE);
                if (m_windowStyle & WS_OVERLAPPEDWINDOW)
                {
                    mi = { sizeof(MONITORINFO) };
                    if (GetWindowPlacement(m_window, &m_wpPrev) && GetMonitorInfo(MonitorFromWindow(m_window, MONITORINFOF_PRIMARY), &mi))
                    {
                        SetWindowLong(m_window, GWL_STYLE, m_windowStyle & ~WS_OVERLAPPEDWINDOW);
                        SetWindowPos(m_window, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top, mi.rcMonitor.right - mi.rcMonitor.left, mi.rcMonitor.bottom - mi.rcMonitor.top, SWP_NOZORDER | SWP_FRAMECHANGED);
                    }
                }
                ShowCursor(FALSE);
            }
            else
            {
                //code
                SetWindowLong(m_window, GWL_STYLE, m_windowStyle | WS_OVERLAPPEDWINDOW);
                SetWindowPlacement(m_window, &m_wpPrev);
                SetWindowPos(m_window, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED);
                ShowCursor(TRUE);
            }

            m_isFullscreen = !m_isFullscreen;
        }

        void SwapBuffers() override
        {
            ::SwapBuffers(m_hdc);
        }

        bool PumpMessage() override
        {
            bool messagefetched = false;
            MSG msg;
            if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
            {
                if (msg.message == WM_QUIT)
                {
                    m_exitLoop = false;
                }
                else
                {
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                }
                messagefetched = true;
            }
            return messagefetched;
        }
    };
}

using namespace Framework;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
    WindowsHost host;
    return host.Run() ? 0 : -1;
}