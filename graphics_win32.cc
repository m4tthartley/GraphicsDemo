/*
	ISSUES
	- Doesn't mesion very clearly that you need to define VK_USE_PLATFORM_WIN32_KHR
	- VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR is spelled wrong in the docs
	- When creating the win32 surface you have to give it the hInstance and hwnd, the docs said you don't have to
*/

// #include <windows.h>
// TODO: Do we not need windows.h anymore
#define VK_USE_PLATFORM_WIN32_KHR
#include "w:/lib/vulkan/vulkan.h"

bool running = TRUE;

void VulkanInit (HINSTANCE hInstance, HWND hwnd)
{
	VkInstanceCreateInfo instanceCreateInfo = {};
	instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceCreateInfo.enabledExtensionCount;
	char *extensionNames[] = {"VK_KHR_surface", "VK_KHR_win32_surface"};
	instanceCreateInfo.ppEnabledExtensionNames = extensionNames;
	// VkApplicationInfo applicationInfo;
	VkInstance vulkanInstance;
	VkResult instanceResult = vkCreateInstance(&instanceCreateInfo, NULL, &vulkanInstance);
	if (instanceResult == VK_SUCCESS)
	{
		unsigned int physicalDeviceCount = 8;
		VkPhysicalDevice physicalDevices[8];
		VkResult physicalDevicesResult = vkEnumeratePhysicalDevices(vulkanInstance, &physicalDeviceCount, physicalDevices);

		if (physicalDevicesResult == VK_SUCCESS && physicalDeviceCount > 0)
		{
			VkPhysicalDeviceProperties physicalDeviceProperties;
			vkGetPhysicalDeviceProperties(physicalDevices[0], &physicalDeviceProperties);

			unsigned int queueFamilyPropertyCount = 8;
			VkQueueFamilyProperties queueFamilyProperties[8] = {};
			vkGetPhysicalDeviceQueueFamilyProperties(physicalDevices[0], &queueFamilyPropertyCount, queueFamilyProperties);

			if (queueFamilyPropertyCount > 0)
			{
				VkDeviceQueueCreateInfo deviceQueueCreateInfo = {};
				deviceQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
				deviceQueueCreateInfo.queueFamilyIndex = 0;
				deviceQueueCreateInfo.queueCount = 1/*queueFamilyProperties[0].queueCount*/;
				float queuePriorities = 0.0f;
				deviceQueueCreateInfo.pQueuePriorities = &queuePriorities;

				VkDeviceCreateInfo deviceCreateInfo = {};
				deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
				deviceCreateInfo.queueCreateInfoCount = 1;
				deviceCreateInfo.pQueueCreateInfos = &deviceQueueCreateInfo;

				VkDevice device;
				VkResult deviceResult = vkCreateDevice(physicalDevices[0], &deviceCreateInfo, NULL, &device);

				if (deviceResult == VK_SUCCESS) {
					VkWin32SurfaceCreateInfoKHR surfaceCreateInfo = {};
					surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
					surfaceCreateInfo.hinstance = hInstance;
					surfaceCreateInfo.hwnd = hwnd;
					VkSurfaceKHR surface;
					VkResult win32SurfaceResult = vkCreateWin32SurfaceKHR(vulkanInstance, &surfaceCreateInfo, NULL, &surface);

					if (win32SurfaceResult == VK_SUCCESS) {
						unsigned int surfaceFormatCount = 8;
						VkSurfaceFormatKHR surfaceFormats[8];
						VkResult surfaceFormatsResult = vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevices[0], surface, &surfaceFormatCount, surfaceFormats);

						VkSurfaceCapabilitiesKHR surfaceCapabilities = {};
						VkResult surfaceCapabilitiesResult = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevices[0], surface, &surfaceCapabilities);

						VkSwapchainCreateInfoKHR swapchainCreateInfo = {};
						swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
						swapchainCreateInfo.surface = surface;
						swapchainCreateInfo.minImageCount = surfaceCapabilities.minImageCount;
						swapchainCreateInfo.imageFormat = surfaceFormats[0].format;
						swapchainCreateInfo.imageColorSpace = surfaceFormats[0].colorSpace;
						swapchainCreateInfo.imageExtent = surfaceCapabilities.maxImageExtent;
						swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
						swapchainCreateInfo.preTransform = surfaceCapabilities.currentTransform;
						swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
						swapchainCreateInfo.imageArrayLayers = 1;
						swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
						swapchainCreateInfo.queueFamilyIndexCount = 0;
						swapchainCreateInfo.pQueueFamilyIndices = NULL;
						swapchainCreateInfo.presentMode = VK_PRESENT_MODE_FIFO_KHR;
						swapchainCreateInfo.clipped = TRUE;
						VkSwapchainKHR swapchain;
						VkResult swapchainResult = vkCreateSwapchainKHR(device, &swapchainCreateInfo, NULL, &swapchain);

						if (swapchainResult == VK_SUCCESS) {
							unsigned int swapchainImageCount = 8;
							VkImage swapchainImages[8];
							vkGetSwapchainImagesKHR(device, swapchain, &swapchainImageCount, swapchainImages);

							VkImageView backBufferView;
							VkImageViewCreateInfo backBufferCreateInfo = {};
							backBufferCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
							backBufferCreateInfo.image = swapchainImages[0];
							backBufferCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
							backBufferCreateInfo.format = surfaceFormats[0].format;

							backBufferCreateInfo.components = {VK_COMPONENT_SWIZZLE_R,
															   VK_COMPONENT_SWIZZLE_G,
															   VK_COMPONENT_SWIZZLE_B,
															   VK_COMPONENT_SWIZZLE_A};

							backBufferCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
							backBufferCreateInfo.subresourceRange.baseMipLevel = 0;
							backBufferCreateInfo.subresourceRange.levelCount = 1;
							backBufferCreateInfo.subresourceRange.baseArrayLayer = 0;
							backBufferCreateInfo.subresourceRange.layerCount = 1;
							VkResult createImageViewResult = vkCreateImageView(device, &backBufferCreateInfo, NULL, &backBufferView);

							if (createImageViewResult == VK_SUCCESS) {
								VkQueue deviceQueue;
								vkGetDeviceQueue(device, 0, 0, &deviceQueue);

								// here you will specify the total list of attachments
							    // (which in this case is just one, that's e.g. R8G8B8A8_UNORM)
							    // as well as describe a single subpass, using that attachment
							    // for color and with no depth-stencil attachment

								VkAttachmentDescription attachment = {};
								attachment.flags = 0;
								attachment.format = surfaceFormats[0].format;
								attachment.samples = VK_SAMPLE_COUNT_1_BIT;
								attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
								attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
								attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
								attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
								attachment.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
								attachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

								VkAttachmentReference attachmentReference = {};
								attachmentReference.attachment = 0;
								attachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

								VkSubpassDescription subPassDescription = {};
								subPassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
								subPassDescription.colorAttachmentCount = 1;
								subPassDescription.pColorAttachments = &attachmentReference;

								VkRenderPass renderPass;
								VkRenderPassCreateInfo renderPassCreateInfo = {};
								renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
								renderPassCreateInfo.attachmentCount = 1;
								renderPassCreateInfo.pAttachments = &attachment;
								renderPassCreateInfo.subpassCount = 1;
								renderPassCreateInfo.pSubpasses = &subPassDescription;
								VkResult createRenderPassResult = vkCreateRenderPass(device, &renderPassCreateInfo, NULL, &renderPass);


								VkFramebuffer frameBuffer;
								VkFramebufferCreateInfo frameBufferCreateInfo = {};
								frameBufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
								frameBufferCreateInfo.renderPass = renderPass;
								frameBufferCreateInfo.attachmentCount = 1;
								frameBufferCreateInfo.pAttachments = &backBufferView;
								frameBufferCreateInfo.width = surfaceCapabilities.currentExtent.width;
								frameBufferCreateInfo.height = surfaceCapabilities.currentExtent.height;
								frameBufferCreateInfo.layers = 1;
								VkResult CreateFrameBufferResult = vkCreateFramebuffer(device, &frameBufferCreateInfo, NULL, &frameBuffer);

								int x = 0;
							}
						}
					}
				}
			}
		}
	}
}

void VulkanDraw ()
{
}

#include "directx11.cc"

LRESULT CALLBACK WindowProc (HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
		case WM_CLOSE: {
			running = FALSE;
		} break;
		case WM_DESTROY: {
			running = FALSE;
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

	dx_state dx = {};

	if (RegisterClassA(&WindowClass))
	{
		Window = CreateWindowExA(0, WindowClass.lpszClassName, "Graphics Demo", WS_OVERLAPPEDWINDOW|WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, 0, 0, hInstance, 0);

		// DWORD style = GetWindowLong(Window, GWL_STYLE);
		// SetWindowLong(Window, GWL_STYLE, style & ~WS_OVERLAPPEDWINDOW);

		if (Window)
		{
			UpdateWindow(Window);

			// VulkanInit(hInstance, Window);
			dx.hwnd = Window;
			DX11Init(&dx);

			while (running)
			{
				MSG msg;
				while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}

				DX11Draw(&dx);
			}
		}
	}
	else
	{
		OutputDebugString("Failed to register window class \n");
	}

	return 0;
}