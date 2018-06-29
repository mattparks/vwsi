#include "wsi/vulkan_wsi.h"

#include <windows.h>
#include <stdio.h>
#include <assert.h>

HINSTANCE hinstance_;
HWND hwnd_;
HMODULE hmodule_;
WsiCallbacks callbacks_;
PFN_vkGetInstanceProcAddr vkproc_;

LRESULT CALLBACK window_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	WsiShell shell = GetWindowLongPtr(hwnd, GWLP_USERDATA);

	if (!shell) {
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	switch (uMsg) {
	case WM_SIZE: {
		UINT w = LOWORD(lParam);
		UINT h = HIWORD(lParam);
		callbacks_.size(shell, w, h, false);
	} break;
	case WM_KEYDOWN: {
	} break;
	case WM_CLOSE:
		callbacks_.close(shell);
		break;
	case WM_DESTROY:
		break;
	default:
		return DefWindowProc(hwnd_, uMsg, wParam, lParam);
		break;
	}

	return 0;
}

VkResult wsiEnumerateMonitors(uint32_t *pMonitorCount, WsiMonitor *pMonitors)
{
	return VK_SUCCESS;
}

void wsiGetMonitorProperties(WsiMonitor monitor, WsiMonitorProperties *monitorProperties)
{
}

VkResult wsiCreateShell(const WsiShellCreateInfo *pCreateInfo, WsiShell *pShell)
{
	callbacks_ = *pCreateInfo->pCallbacks;


	const char filename[] = "vulkan-1.dll";
	HMODULE mod;
	vkproc_ = NULL;

	mod = LoadLibrary(filename);

	if (mod)
	{
		vkproc_ = (PFN_vkGetInstanceProcAddr)GetProcAddress(mod, "vkGetInstanceProcAddr");
	}

	if (!mod || !vkproc_)
	{
		assert("Failed to load Vulkan!");
	}

	hmodule_ = mod;

	pShell = malloc(sizeof(WsiShell));

	hinstance_ = GetModuleHandle(NULL);

	WNDCLASSEX win_class = {};
	win_class.cbSize = sizeof(WNDCLASSEX);
	win_class.style = CS_HREDRAW | CS_VREDRAW;
	win_class.lpfnWndProc = window_proc;
	win_class.hInstance = hinstance_;
	win_class.hCursor = LoadCursor(NULL, IDC_ARROW);
	win_class.lpszClassName = "Shell";
	RegisterClassEx(&win_class);

	const DWORD win_style = WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE | WS_OVERLAPPEDWINDOW;

	RECT win_rect = {0, 0, pCreateInfo->width, pCreateInfo->height};
	AdjustWindowRect(&win_rect, win_style, false);

	hwnd_ = CreateWindowEx(WS_EX_APPWINDOW, "Shell", pCreateInfo->title, win_style, pCreateInfo->x, pCreateInfo->y,
		win_rect.right - win_rect.left, win_rect.bottom - win_rect.top, NULL, NULL, hinstance_, NULL);

	SetForegroundWindow(hwnd_);
	SetWindowLongPtr(hwnd_, GWLP_USERDATA, (LONG_PTR)pShell);

	return VK_SUCCESS;
}

void wsiDestroyShell(WsiShell shell)
{
	CloseWindow(hwnd_);
	FreeLibrary(hmodule_);
}

VkResult wsiEnumerateShellExtensions(WsiShell shell, uint32_t *pExtensionCount, const char **pExtensions)
{
	*pExtensionCount = 2;

	if (pExtensions == NULL)
	{
		return VK_SUCCESS;
	}

	pExtensions[0] = "VK_KHR_surface";
	pExtensions[1] = "VK_KHR_win32_surface";
	return VK_SUCCESS;
}

VkResult wsiCreateSurface(WsiShell shell, VkInstance instance, const VkAllocationCallbacks *allocator, VkSurfaceKHR *surface)
{
	*surface = VK_NULL_HANDLE;
	VkResult err;
	VkWin32SurfaceCreateInfoKHR sci;
	PFN_vkCreateWin32SurfaceKHR vkCreateWin32SurfaceKHR;

	vkCreateWin32SurfaceKHR = (PFN_vkCreateWin32SurfaceKHR)vkGetInstanceProcAddr(instance, "vkCreateWin32SurfaceKHR");

	if (!vkCreateWin32SurfaceKHR)
	{
		assert("Win32: Vulkan instance missing VK_KHR_win32_surface extension");
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}

	memset(&sci, 0, sizeof(sci));
	sci.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	sci.hinstance = GetModuleHandle(NULL);
	sci.hwnd = hwnd_;

	err = vkCreateWin32SurfaceKHR(instance, &sci, allocator, surface);

	if (err)
	{
		assert("Win32: Failed to create Vulkan surface: %s");
	}

	return err;
}

VkResult wsiPollEvents(WsiShell shell)
{
	MSG msg;

	while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			callbacks_.close(shell);
			break;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return VK_SUCCESS;
}

VkResult wsiCmdSetSize(WsiShell shell, uint32_t width, uint32_t height)
{
	// TODO: Fix x, y being reset.
	RECT win_rect = {0, 0, width, height};
	MoveWindow(hwnd_, 0, 0, win_rect.right - win_rect.left, win_rect.bottom - win_rect.top, FALSE);
	return VK_SUCCESS;
}

VkResult wsiCmdSetPosition(WsiShell shell, uint32_t x, uint32_t y)
{
	RECT curr_rect;
	GetClientRect(hwnd_, &curr_rect);
	MoveWindow(hwnd_, x, y, curr_rect.right - curr_rect.left, curr_rect.bottom - curr_rect.top, FALSE);
	return VK_SUCCESS;
}

VkResult wsiCmdSetTitle(WsiShell shell, const char *title)
{
	SetWindowText(hwnd_, title);
	return VK_SUCCESS;
}

VkResult wsiCmdSetIcon(WsiShell shell, const WsiIcon *icon)
{
	// TODO: Implement.
	return VK_SUCCESS;
}

VkResult wsiCmdSetFullscreen(WsiShell shell, WsiMonitor monitor, bool fullscreen)
{
	// TODO: Implement.
	return VK_SUCCESS;
}
