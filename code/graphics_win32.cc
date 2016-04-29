
#define assert(expression) {if (!(expression)) { *((int*)0) = 0; }}
#define null 0
#define PI 3.14159265359f

// #include "vulkan.cc"

#include <math.h>
#include <windows.h>
#ifdef RENDER_OPENGL
#include <GL/gl.h> // windows.h must be included before gl.h
#include <GL/glu.h>
#endif
#undef near // FUCK YOU WINDOWS!!!
#undef far

#include "gj_lib.h"

#ifdef RENDER_OPENGL
#include "opengl.cc"
#endif
#ifdef RENDER_DIRECTX
#include "directx11.cc"
#endif

bool running = true;

LRESULT CALLBACK WindowProc (HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
		case WM_CLOSE: {
			running = false;
		} break;
		case WM_DESTROY: {
			running = false;
		} break;
		default: {
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		} break;
	}

	return 0;
}

int CALLBACK WinMain (HINSTANCE hInstance,
					  HINSTANCE hPrevInstance,
					  LPSTR lpCmdLine,
					  int nCmdShow)
{
	WNDCLASS WindowClass = {};
	WindowClass.style = CS_OWNDC|CS_HREDRAW|CS_VREDRAW;
	WindowClass.lpfnWndProc = WindowProc;
	WindowClass.hInstance = hInstance;
	WindowClass.lpszClassName = "WindowClass";

	HWND Window;

	if (RegisterClassA(&WindowClass))
	{
		Window = CreateWindowExA(0, WindowClass.lpszClassName, "Graphics Demo", WS_OVERLAPPEDWINDOW|WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, 0, 0, hInstance, 0);

		// DWORD style = GetWindowLong(Window, GWL_STYLE);
		// SetWindowLong(Window, GWL_STYLE, style & ~WS_OVERLAPPEDWINDOW);

		if (Window)
		{
			UpdateWindow(Window);

			// VulkanInit(hInstance, Window);
#ifdef RENDER_DIRECTX
			dx_state dx = {};
			dx.hwnd = Window;
			DX11Init(&dx);
#endif
#ifdef RENDER_OPENGL
			initOpengl(Window);
#endif

			while (running)
			{
				MSG msg;
				while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}

#ifdef RENDER_DIRECTX
				DX11Draw(&dx);
#endif
#ifdef RENDER_OPENGL
				drawOpengl(Window);
#endif
			}
		}
	}
	else
	{
		OutputDebugString("Failed to register window class \n");
	}

	return 0;
}