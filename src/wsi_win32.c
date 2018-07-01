#include "VWSI/vulkan_wsi.h"

#include <windows.h>
#include <stdio.h>
#include <assert.h>

typedef struct WsiMonitor_T
{

} WsiMonitor_T;

typedef struct WsiShell_T
{
	WsiShellCallbacks			callbacks_;

	HINSTANCE 					hinstance_;
	HWND 						hwnd_;
	HMODULE 					hmodule_;
	PFN_vkGetInstanceProcAddr 	vkproc_;
} WsiShell_T;

LRESULT CALLBACK window_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	WsiShell shell = (WsiShell)GetWindowLongPtr(hwnd, GWLP_USERDATA);

	if (shell == NULL)
	{
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	switch (uMsg)
	{
	case WM_SIZE:
	{
		UINT w = LOWORD(lParam);
		UINT h = HIWORD(lParam);

		if (shell->callbacks_.pfnSize != NULL)
		{
			shell->callbacks_.pfnSize(shell, w, h, FALSE);
		}
	}
	break;
	case WM_KEYDOWN:
	{
	}
	break;
	case WM_CLOSE:
	{
		if (shell->callbacks_.pfnClose != NULL)
		{
			shell->callbacks_.pfnClose(shell);
		}
	}
	break;
	case WM_DESTROY:
	{

	}
	break;
	default:
	{
		return DefWindowProc(shell->hwnd_, uMsg, wParam, lParam);
	}
	break;
	}

	return 0;
}

VkResult wsiEnumerateMonitors(uint32_t *pMonitorCount, WsiMonitor *pMonitors)
{
	return VK_SUCCESS;
}

void wsiGetMonitorProperties(WsiMonitor monitor, WsiMonitorProperties *pMonitorProperties)
{
}

VkResult wsiCreateShell(const WsiShellCreateInfo *pCreateInfo, WsiShell *pShell)
{
	if (pCreateInfo == NULL)
	{
		return VK_INCOMPLETE;
	}

	*pShell = VK_NULL_HANDLE;

	WsiShell_T *shell = calloc(1, sizeof(WsiShell_T));
	memset(shell, 0, sizeof(shell));
	shell->callbacks_ = *pCreateInfo->pCallbacks;

	const char filename[] = "vulkan-1.dll";
	shell->hmodule_ = LoadLibrary(filename);
	shell->vkproc_ = NULL;

	if (shell->hmodule_)
	{
		shell->vkproc_ = (PFN_vkGetInstanceProcAddr)GetProcAddress(shell->hmodule_, "vkGetInstanceProcAddr");
	}

	if (!shell->hmodule_ || !shell->vkproc_)
	{
		assert("Failed to load Vulkan!");
	}

	shell->hinstance_ = GetModuleHandle(NULL);

	WNDCLASSEX win_class = {};
	win_class.cbSize = sizeof(WNDCLASSEX);
	win_class.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	win_class.lpfnWndProc = window_proc;
	win_class.hInstance = shell->hinstance_;
	win_class.hCursor = LoadCursor(NULL, IDC_ARROW);
	win_class.lpszClassName = "Shell";

	//if (pCreateInfo->pIcon != NULL)
	//{
	//	win_class.hIcon = Load;
	//}

	RegisterClassEx(&win_class);

	const DWORD win_style = WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE | WS_OVERLAPPEDWINDOW;

	RECT win_rect = {0, 0, pCreateInfo->width, pCreateInfo->height};
	AdjustWindowRect(&win_rect, win_style, 0);

	shell->hwnd_ = CreateWindowEx(WS_EX_APPWINDOW, "Shell", pCreateInfo->pName, win_style, pCreateInfo->x, pCreateInfo->y,
		win_rect.right - win_rect.left, win_rect.bottom - win_rect.top, NULL, NULL, shell->hinstance_, NULL);

	SetForegroundWindow(shell->hwnd_);
	SetWindowLongPtr(shell->hwnd_, GWLP_USERDATA, (LONG_PTR)shell);

	*pShell = shell;
	return VK_SUCCESS;
}

void wsiDestroyShell(WsiShell shell)
{
	CloseWindow(shell->hwnd_);
	FreeLibrary(shell->hmodule_);
}

VkResult wsiEnumerateShellExtensions(WsiShell shell, uint32_t *pExtensionCount, const char **pExtensions)
{
	*pExtensionCount = 2;

	if (pExtensions == NULL)
	{
		return VK_SUCCESS;
	}

	pExtensions[0] = VK_KHR_SURFACE_EXTENSION_NAME;
	pExtensions[1] = VK_KHR_WIN32_SURFACE_EXTENSION_NAME;
	return VK_SUCCESS;
}

VkResult wsiCreateSurface(WsiShell shell, VkInstance instance, const VkAllocationCallbacks *pAllocator, VkSurfaceKHR *pSurface)
{
	if (shell == NULL)
	{
		return VK_ERROR_MEMORY_MAP_FAILED;
	}

	*pSurface = VK_NULL_HANDLE;

	VkWin32SurfaceCreateInfoKHR surfaceCreateInfo;
	surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	surfaceCreateInfo.hinstance = shell->hinstance_;
	surfaceCreateInfo.hwnd = shell->hwnd_;

	return vkCreateWin32SurfaceKHR(instance, &surfaceCreateInfo, pAllocator, pSurface);
}

VkResult wsiPollEvents(WsiShell shell)
{
	MSG msg;

	while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			if (shell->callbacks_.pfnClose != NULL)
			{
				shell->callbacks_.pfnClose(shell);
			}

			return VK_SUCCESS;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return VK_SUCCESS;
}

void wsiGetShellCallbacks(WsiShell shell, WsiShellCallbacks **pCallbacks)
{
	*pCallbacks = &shell->callbacks_;
}

VkResult wsiCmdSetResizable(WsiShell shell, VkBool32 resizable)
{
	return VK_SUCCESS;
}

VkResult wsiCmdSetSize(WsiShell shell, uint32_t width, uint32_t height)
{
	// TODO: Don't reset window X, Y.
	RECT win_rect = {0, 0, width, height};
	MoveWindow(shell->hwnd_, 0, 0, win_rect.right - win_rect.left, win_rect.bottom - win_rect.top, FALSE);
	return VK_SUCCESS;
}

VkResult wsiCmdSetPosition(WsiShell shell, uint32_t x, uint32_t y)
{
	RECT curr_rect;
	GetClientRect(shell->hwnd_, &curr_rect);
	MoveWindow(shell->hwnd_, x, y, curr_rect.right - curr_rect.left, curr_rect.bottom - curr_rect.top, FALSE);
	return VK_SUCCESS;
}

VkResult wsiCmdSetName(WsiShell shell, const char *pName)
{
	SetWindowText(shell->hwnd_, pName);
	return VK_SUCCESS;
}

VkResult wsiCmdSetFullscreen(WsiShell shell, WsiMonitor monitor, VkBool32 fullscreen)
{
	return VK_SUCCESS;
}

VkResult wsiCmdSetIcon(WsiShell shell, WsiImage icon)
{
	return VK_SUCCESS;
}

VkResult wsiCmdSetCursor(WsiShell shell, WsiImage cursor)
{
	return VK_SUCCESS;
}

VkResult wsiCmdSetCursorMode(WsiShell shell, WsiCursorMode mode)
{
	return VK_SUCCESS;
}

VkResult wsiCmdSetCursorPos(WsiShell shell, float x, float y)
{
	return VK_SUCCESS;
}
