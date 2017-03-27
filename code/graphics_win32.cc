
// #include "vulkan.cc"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <windows.h>
#ifdef RENDER_OPENGL
#include <GL/gl.h> // windows.h must be included before gl.h
#include <GL/glu.h>
#include "w:/lib/glext.h"
#endif
#undef near // FUCK YOU WINDOWS!!!
#undef far

#include "w:/lib/gj/gj_lib.h"
#include "w:/lib/gj/gj_win32.cc"

HDC hdc;

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
		Window = CreateWindowExA(0, WindowClass.lpszClassName, "Graphics Demo", WS_OVERLAPPEDWINDOW|WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, defaultViewport.x, defaultViewport.y, 0, 0, hInstance, 0);
		hdc = GetDC(Window);

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
				globalScroll = 0;

				MSG msg;
				while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
				{
					switch (msg.message) {
						case WM_MOUSEWHEEL: {
							short scrollDelta = GET_WHEEL_DELTA_WPARAM(msg.wParam);
							globalScroll = scrollDelta / WHEEL_DELTA;
						} break;
						case WM_SYSKEYUP:
						case WM_KEYUP: {
							switch (msg.wParam) {
								case 'N': {
									if (globalNormalVisualization) {
										globalNormalVisualization = false;
									} else {
										globalNormalVisualization = true;
									}
								} break;
								/*case VK_LEFT: {
									--selectedModel;
									if (selectedModel < 0) {
										selectedModel = 0;
									}
								} break;
								case VK_RIGHT: {
									++selectedModel;
									if (selectedModel >= arraySize(models)) {
										selectedModel = arraySize(models) - 1;
									}
								} break;*/
							}
						} break;
						case WM_SYSKEYDOWN:
						case WM_KEYDOWN: {
							switch (msg.wParam) {
								case VK_LEFT: {
									playerFov -= 1.0f;
								} break;
								case VK_RIGHT: {
									playerFov += 1.0f;
								} break;
							}
						}
						default: {
							TranslateMessage(&msg);
							DispatchMessage(&msg);
						} break;
					}
				}

#ifdef RENDER_DIRECTX
				DX11Draw(&dx);
#endif
#ifdef RENDER_OPENGL
				drawOpengl(Window);
				// stuff();
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