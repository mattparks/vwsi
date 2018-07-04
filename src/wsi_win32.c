#ifdef VK_USE_PLATFORM_WIN32_KHR

#include "VWSI/vulkan_wsi.h"

#include <windows.h>
#include <windowsx.h>
#include <shellapi.h>
#include <stdio.h>
#include <assert.h>

#ifndef WM_MOUSEHWHEEL
#define WM_MOUSEHWHEEL 0x020E
#endif
#ifndef HID_USAGE_PAGE_GENERIC
#define HID_USAGE_PAGE_GENERIC ((USHORT) 0x01)
#endif
#ifndef HID_USAGE_GENERIC_MOUSE
#define HID_USAGE_GENERIC_MOUSE ((USHORT) 0x02)
#endif

typedef struct WsiMonitor_T
{
	uint32_t id_;
} WsiMonitor_T;

typedef struct WsiShell_T
{
	// Callbacks.
	WsiShellCallbacks callbacks_;

	// Window.
	HINSTANCE hinstance_;
	HWND hwnd_;
	HMODULE hmodule_;
	PFN_vkGetInstanceProcAddr vkproc_;

	// Cursor movement.
	int cursorTracked_;
	int cursorDisabled_;
	int cursorPosX, cursorPosY;
	WsiCursorMode cursorMode_;
	HCURSOR cursor_;

	// Mouse input.
	RAWINPUT *rawInput_;
	int rawInputSize_;
} WsiShell_T;

const WsiKey WIN32_TO_WSI_KEY[512] = {
	0, WSI_KEY_ESCAPE, WSI_KEY_1, WSI_KEY_2, WSI_KEY_3, WSI_KEY_4, WSI_KEY_5, WSI_KEY_6, WSI_KEY_7,
	WSI_KEY_8, WSI_KEY_9, WSI_KEY_0, WSI_KEY_MINUS, WSI_KEY_EQUAL, WSI_KEY_BACKSPACE, WSI_KEY_TAB,
	WSI_KEY_Q, WSI_KEY_W, WSI_KEY_E, WSI_KEY_R, WSI_KEY_T, WSI_KEY_Y, WSI_KEY_U, WSI_KEY_I, WSI_KEY_O,
	WSI_KEY_P, WSI_KEY_LEFT_BRACKET, WSI_KEY_RIGHT_BRACKET, WSI_KEY_ENTER, WSI_KEY_LEFT_CONTROL,
	WSI_KEY_A, WSI_KEY_S, WSI_KEY_D, WSI_KEY_F, WSI_KEY_G, WSI_KEY_H, WSI_KEY_J, WSI_KEY_K, WSI_KEY_L,
	WSI_KEY_SEMICOLON, WSI_KEY_APOSTROPHE, WSI_KEY_GRAVE_ACCENT, WSI_KEY_LEFT_SHIFT, WSI_KEY_BACKSLASH,
	WSI_KEY_Z, WSI_KEY_X, WSI_KEY_C, WSI_KEY_V, WSI_KEY_B, WSI_KEY_N, WSI_KEY_M, WSI_KEY_COMMA,
	WSI_KEY_PERIOD, WSI_KEY_SLASH, WSI_KEY_RIGHT_SHIFT, WSI_KEY_PAD_MULTIPLY, WSI_KEY_LEFT_ALT,
	WSI_KEY_SPACE, WSI_KEY_CAPS_LOCK, WSI_KEY_F1, WSI_KEY_F2, WSI_KEY_F3, WSI_KEY_F4, WSI_KEY_F5,
	WSI_KEY_F6, WSI_KEY_F7, WSI_KEY_F8, WSI_KEY_F9, WSI_KEY_F10, WSI_KEY_PAUSE, WSI_KEY_SCROLL_LOCK,
	WSI_KEY_PAD_7, WSI_KEY_PAD_8, WSI_KEY_PAD_9, WSI_KEY_PAD_SUBTRACT, WSI_KEY_PAD_4, WSI_KEY_PAD_5,
	WSI_KEY_PAD_6, WSI_KEY_PAD_ADD, WSI_KEY_PAD_1, WSI_KEY_PAD_2, WSI_KEY_PAD_3, WSI_KEY_PAD_0,
	WSI_KEY_PAD_DECIMAL, 0, 0, WSI_KEY_WORLD_2, WSI_KEY_F11, WSI_KEY_F12, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, WSI_KEY_F13, WSI_KEY_F14, WSI_KEY_F15, WSI_KEY_F16, WSI_KEY_F17, WSI_KEY_F18, WSI_KEY_F19,
	WSI_KEY_F20, WSI_KEY_F21, WSI_KEY_F22, WSI_KEY_F23, 0, 0, 0, 0, 0, 0, 0, WSI_KEY_F24, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, WSI_KEY_PAD_ENTER,
	WSI_KEY_RIGHT_CONTROL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	WSI_KEY_PAD_DIVIDE, 0, WSI_KEY_PRINT_SCREEN, WSI_KEY_RIGHT_ALT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	WSI_KEY_NUM_LOCK, WSI_KEY_PAUSE, WSI_KEY_HOME, WSI_KEY_UP, WSI_KEY_PAGE_UP, 0, WSI_KEY_LEFT, 0,
	WSI_KEY_RIGHT, 0, WSI_KEY_END, WSI_KEY_DOWN, WSI_KEY_PAGE_DOWN, WSI_KEY_INSERT, WSI_KEY_DELETE, 0,
	0, 0, 0, 0, 0, 0, WSI_KEY_LEFT_SUPER, WSI_KEY_RIGHT_SUPER, WSI_KEY_MENU, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

void reportError(const char *message)
{
	printf("VWSI Error: %s\n", message);
}

WsiKey translateKey(WsiShell shell, WPARAM wParam, LPARAM lParam)
{
    // The Ctrl keys require special handling
    if (wParam == VK_CONTROL)
    {
        MSG next;
        DWORD time;

        // Right side keys have the extended key bit set
        if (lParam & 0x01000000)
        {
            return WSI_KEY_RIGHT_CONTROL;
        }

        // If Right Alt, ignore this (synthetic) Left Ctrl message
        time = GetMessageTime();

        if (PeekMessageW(&next, NULL, 0, 0, PM_NOREMOVE))
        {
            if (next.message == WM_KEYDOWN ||
                next.message == WM_SYSKEYDOWN ||
                next.message == WM_KEYUP ||
                next.message == WM_SYSKEYUP)
            {
                if (next.wParam == VK_MENU &&
                    (next.lParam & 0x01000000) &&
                    next.time == time)
                {
                    // Next message is Right Alt down so discard this
                    return WSI_KEY_UNKNOWN;
                }
            }
        }

        return WSI_KEY_LEFT_CONTROL;
    }

    if (wParam == VK_PROCESSKEY)
    {
        // IME notifies that keys have been filtered by setting the virtual key-code to VK_PROCESSKEY
        return WSI_KEY_UNKNOWN;
    }

    return WIN32_TO_WSI_KEY[HIWORD(lParam) & 0x1FF];
}

WsiModifierFlags getKeyMods(void)
{
	WsiModifierFlags mods = 0;

	if (GetKeyState(VK_SHIFT) & 0x8000)
		mods |= WSI_MODIFIER_SHIFT_BIT;
	if (GetKeyState(VK_CONTROL) & 0x8000)
		mods |= WSI_MODIFIER_CONTROL_BIT;
	if (GetKeyState(VK_MENU) & 0x8000)
		mods |= WSI_MODIFIER_ALT_BIT;
	if ((GetKeyState(VK_LWIN) | GetKeyState(VK_RWIN)) & 0x8000)
		mods |= WSI_MODIFIER_SUPER_BIT;
	if (GetKeyState(VK_CAPITAL) & 1)
		mods |= WSI_MODIFIER_CAPS_LOCK_BIT;
	if (GetKeyState(VK_NUMLOCK) & 1)
		mods |= WSI_MODIFIER_NUM_LOCK_BIT;

	return mods;
}

void updateCursorImage(WsiShell shell)
{
	if (shell->cursorMode_ == WSI_CURSOR_MODE_NORMAL)
    {
    	if (shell->cursor_ != NULL)
    	{
    		SetCursor(shell->cursor_);
		}
		else
		{
			SetCursor(LoadCursorW(NULL, IDC_ARROW));
		}
    }
    else
    {
    	SetCursor(NULL);
    }
}

void updateCursorClip(WsiShell shell)
{
	if (shell != NULL)
	{
		RECT clipRect;
		GetClientRect(shell->hwnd_, &clipRect);
		ClientToScreen(shell->hwnd_, (POINT*) &clipRect.left);
		ClientToScreen(shell->hwnd_, (POINT*) &clipRect.right);
		ClipCursor(&clipRect);
	}
	else
	{
		ClipCursor(NULL);
	}
}

LRESULT CALLBACK window_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	WsiShell shell = (WsiShell) GetWindowLongPtr(hwnd, GWLP_USERDATA);

	if (shell == NULL)
	{
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	switch (uMsg)
	{
	case WM_DISPLAYCHANGE:
	{
		// TODO: Update monitor list
		break;
	}
	case WM_MOVE:
	{
		UINT x = LOWORD(lParam);
		UINT y = HIWORD(lParam);

		if (shell->cursorDisabled_)
		{
			updateCursorClip(shell);
		}

		if (shell->callbacks_.pfnPosition != NULL)
		{
			shell->callbacks_.pfnPosition(shell, x, y);
		}

		break;
	}
	case WM_SIZE:
	{
        const int iconified = wParam == SIZE_MINIMIZED;
		UINT w = LOWORD(lParam);
		UINT h = HIWORD(lParam);

		if (shell->cursorDisabled_)
		{
			updateCursorClip(shell);
		}

		if (shell->callbacks_.pfnSize != NULL)
		{
			shell->callbacks_.pfnSize(shell, w, h, iconified, FALSE);
		}

		break;
	}
    case WM_SETFOCUS:
    {
		if (shell->callbacks_.pfnFocus != NULL)
		{
			shell->callbacks_.pfnFocus(shell, TRUE);
		}

		break;
    }
    case WM_KILLFOCUS:
    {
		if (shell->callbacks_.pfnFocus != NULL)
		{
			shell->callbacks_.pfnFocus(shell, FALSE);
		}

		break;
    }
    case WM_ERASEBKGND:
    {
        return TRUE;
    }
	case WM_PAINT:
	{
		ValidateRect(shell->hwnd_, NULL);
		break;
	}
	case WM_DESTROY:
	{
		break;
	}
	case WM_CLOSE:
	{
		if (shell->callbacks_.pfnClose != NULL)
		{
			shell->callbacks_.pfnClose(shell);
		}

		break;
	}
    case WM_SETCURSOR:
    {
        if (LOWORD(lParam) == HTCLIENT)
        {
            updateCursorImage(shell);

            return TRUE;
        }

        break;
    }
	case WM_INPUT:
	{
		HRAWINPUT ri = (HRAWINPUT) lParam;

		UINT size;
		GetRawInputData(ri, RID_INPUT, NULL, &size, sizeof(RAWINPUTHEADER));

		if (size > (UINT) shell->rawInputSize_)
		{
			free(shell->rawInput_);
			shell->rawInput_ = calloc(size, 1);
			shell->rawInputSize_ = size;
		}

		size = shell->rawInputSize_;

		if (GetRawInputData(ri, RID_INPUT, shell->rawInput_, &size,
			sizeof(RAWINPUTHEADER)) == (UINT) -1)
		{
			reportError("Win32: Failed to retrieve raw input data");
			break;
		}

		RAWINPUT* data = shell->rawInput_;
		uint32_t x, y;
		float dx, dy;

		if (data->data.mouse.usFlags & MOUSE_MOVE_ABSOLUTE)
		{
			x = data->data.mouse.lLastX;
			y = data->data.mouse.lLastX;
			dx = x - shell->cursorPosX;
			dy = y - shell->cursorPosY;
		}
		else
		{
			POINT ptCursor;
			GetCursorPos(&ptCursor);
			ScreenToClient(shell->hwnd_, &ptCursor);

			dx = data->data.mouse.lLastX;
			dy = data->data.mouse.lLastY;
			x = ptCursor.x;
			y = ptCursor.y;
		}

		shell->cursorPosX = x;
		shell->cursorPosY = y;

		if (shell->callbacks_.pfnCursorPosition != NULL)
		{
			shell->callbacks_.pfnCursorPosition(shell, x, y, dx, dy);
		}

		break;
	}
    case WM_MOUSEMOVE:
    {
		int x = GET_X_LPARAM(lParam);
		int y = GET_Y_LPARAM(lParam);
		shell->cursorPosX = x;
		shell->cursorPosY = y;

		if (!shell->cursorTracked_)
		{
			if (shell->callbacks_.pfnCursorEnter != NULL)
			{
				shell->callbacks_.pfnCursorEnter(shell, TRUE);
			}

            TRACKMOUSEEVENT tme;
            ZeroMemory(&tme, sizeof(tme));
            tme.cbSize = sizeof(tme);
            tme.dwFlags = TME_LEAVE;
            tme.hwndTrack = shell->hwnd_;
            TrackMouseEvent(&tme);

			shell->cursorTracked_ = 1;
		}

		break;
    }
    case WM_MOUSELEAVE:
    {
		if (shell->callbacks_.pfnCursorEnter != NULL)
		{
			shell->callbacks_.pfnCursorEnter(shell, FALSE);
		}

		shell->cursorTracked_ = 0;

		break;
    }
    case WM_MOUSEHWHEEL:
    {
		if (shell->callbacks_.pfnCursorScroll != NULL)
		{
			shell->callbacks_.pfnCursorScroll(shell, -(SHORT) HIWORD(wParam) / (float) WHEEL_DELTA, 0.0f);
		}

		break;
    }
    case WM_MOUSEWHEEL:
    {
		if (shell->callbacks_.pfnCursorScroll != NULL)
		{
			shell->callbacks_.pfnCursorScroll(shell, 0.0f, (SHORT) HIWORD(wParam) / (float) WHEEL_DELTA);
		}

		break;
    }
    case WM_LBUTTONDOWN:
    case WM_RBUTTONDOWN:
    case WM_MBUTTONDOWN:
    case WM_XBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_RBUTTONUP:
    case WM_MBUTTONUP:
    case WM_XBUTTONUP:
    {
    	WsiMouseButton button;
    	WsiAction action;

        if (uMsg == WM_LBUTTONDOWN || uMsg == WM_LBUTTONUP)
        {
            button = WSI_MOUSE_BUTTON_LEFT;
        }
        else if (uMsg == WM_RBUTTONDOWN || uMsg == WM_RBUTTONUP)
        {
            button = WSI_MOUSE_BUTTON_RIGHT;
        }
        else if (uMsg == WM_MBUTTONDOWN || uMsg == WM_MBUTTONUP)
        {
            button = WSI_MOUSE_BUTTON_MIDDLE;
        }
        else if (GET_XBUTTON_WPARAM(wParam) == XBUTTON1)
        {
            button = WSI_MOUSE_BUTTON_4;
        }
        else
        {
            button = WSI_MOUSE_BUTTON_5;
        }

        if (uMsg == WM_LBUTTONDOWN || uMsg == WM_RBUTTONDOWN ||
            uMsg == WM_MBUTTONDOWN || uMsg == WM_XBUTTONDOWN)
        {
            action = WSI_ACTION_PRESS;
        }
        else
        {
            action = WSI_ACTION_RELEASE;
        }

		if (shell->callbacks_.pfnMouseButton != NULL)
		{
			shell->callbacks_.pfnMouseButton(shell, button, action);
		}

		break;
	}
	case WM_CHAR:
	case WM_SYSCHAR:
	case WM_UNICHAR:
	{
		if (uMsg == WM_UNICHAR && wParam == UNICODE_NOCHAR)
		{
			return TRUE;
		}

		static char charBuf[4];
		strncpy(charBuf, (const char*)&wParam, 4);

		if (shell->callbacks_.pfnChar != NULL)
		{
			shell->callbacks_.pfnChar(shell, charBuf);
		}

		break;
	}
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
	{
		WsiKey key = translateKey(shell, wParam, lParam);
		WsiAction action = ((lParam >> 31) & 1) ? WSI_ACTION_RELEASE : WSI_ACTION_PRESS;
		WsiModifierFlags mods = getKeyMods();

		if (key == WSI_KEY_UNKNOWN)
		{
			break;
		}

		if (shell->callbacks_.pfnKey != NULL)
		{
			if (action == WSI_ACTION_RELEASE && wParam == VK_SHIFT)
			{
				// Release both Shift keys on Shift up event
				shell->callbacks_.pfnKey(shell, WSI_KEY_LEFT_SHIFT, action, mods);
				shell->callbacks_.pfnKey(shell, WSI_KEY_RIGHT_SHIFT, action, mods);
			}
			else if (wParam == VK_SNAPSHOT)
			{
				// Key down is not reported for the Print Screen key
				shell->callbacks_.pfnKey(shell, key, WSI_ACTION_PRESS, mods);
				shell->callbacks_.pfnKey(shell, key, WSI_ACTION_RELEASE, mods);
			}
			else
			{
				shell->callbacks_.pfnKey(shell, key, action, mods);
			}
		}

		break;
	}
#ifdef ENABLE_MULTITOUCH
	case WM_POINTERUPDATE:
	case WM_POINTERDOWN:
	case WM_POINTERUP:
	{
		uint32_t x = GET_X_LPARAM(lParam);
		uint32_t y = GET_Y_LPARAM(lParam);
    	WsiAction action;
		POINTER_INFO pointerInfo;

        if (GetPointerInfo(GET_POINTERID_WPARAM(wParam), &pointerInfo))
        {
            uint32_t id = pointerInfo.pointerId;
            POINT pt = pointerInfo.ptPixelLocation;
            ScreenToClient(shell->hwnd_, &pt);

            switch (message)
            {
                case WM_POINTERDOWN:
                {
                	action = WSI_ACTION_PRESS;
                	break;
                }
                case WM_POINTERUP:
                {
                	action = WSI_ACTION_RELEASE;
                	break;
                }
                case WM_POINTERUPDATE:
                {
                	action = WSI_ACTION_REPEAT;
                	break;
                }
            }

			if (shell->callbacks_.pfnTouch != NULL)
			{
				shell->callbacks_.pfnTouch(shell, id, x, y, action);
			}
        }
		break;
	}
#endif
	// TODO: Joystick connect, button, axis
	}

	return DefWindowProc(shell->hwnd_, uMsg, wParam, lParam);
}

VkResult wsiEnumerateMonitors(uint32_t *pMonitorCount, WsiMonitor *pMonitors)
{
	*pMonitorCount = 1; // TODO: Multiple monitors

	if (pMonitors == NULL)
	{
		return VK_SUCCESS;
	}

	WsiMonitor_T *monitor0 = calloc(1, sizeof(WsiMonitor_T));
	memset(monitor0, 0, sizeof(monitor0));
	pMonitors[0] = monitor0;
	return VK_SUCCESS;
}

void wsiGetMonitorProperties(WsiMonitor monitor, WsiMonitorProperties *pMonitorProperties)
{
	pMonitorProperties->width = GetSystemMetrics(SM_CXSCREEN);
	pMonitorProperties->height = GetSystemMetrics(SM_CYSCREEN);
}

HICON createIcon(WsiImage image, int xhot, int yhot, VkBool32 icon)
{
	if (image.pixels == NULL)
	{
		return NULL;
	}

	int i;
	HDC dc;
	HICON handle;
	HBITMAP color, mask;
	BITMAPV5HEADER bi;
	ICONINFO ii;
	unsigned char *target = NULL;
	unsigned char *source = image.pixels;

	ZeroMemory(&bi, sizeof(bi));
	bi.bV5Size = sizeof(bi);
	bi.bV5Width = image.width;
	bi.bV5Height = -image.height;
	bi.bV5Planes = 1;
	bi.bV5BitCount = 32;
	bi.bV5Compression = BI_BITFIELDS;
	bi.bV5RedMask = 0x00ff0000;
	bi.bV5GreenMask = 0x0000ff00;
	bi.bV5BlueMask = 0x000000ff;
	bi.bV5AlphaMask = 0xff000000;

	dc = GetDC(NULL);
	color = CreateDIBSection(dc, (BITMAPINFO *) &bi, DIB_RGB_COLORS,
		(void **) &target, NULL, (DWORD) 0);
	ReleaseDC(NULL, dc);

	if (!color)
	{
		reportError("Win32: Failed to create RGBA bitmap");
		return NULL;
	}

	mask = CreateBitmap(image.width, image.height, 1, 1, NULL);

	if (!mask)
	{
		reportError("Win32: Failed to create mask bitmap");
		DeleteObject(color);
		return NULL;
	}

	for (i = 0; i < image.width * image.height; i++)
	{
		target[0] = source[2];
		target[1] = source[1];
		target[2] = source[0];
		target[3] = source[3];
		target += 4;
		source += 4;
	}

	ZeroMemory(&ii, sizeof(ii));
	ii.fIcon = icon;
	ii.xHotspot = xhot;
	ii.yHotspot = yhot;
	ii.hbmMask = mask;
	ii.hbmColor = color;

	handle = CreateIconIndirect(&ii);

	DeleteObject(color);
	DeleteObject(mask);

	if (!handle)
	{
		if (icon)
		{
			reportError("Win32: Failed to create icon");
		}
		else
		{
			reportError("Win32: Failed to create cursor");
		}
	}

	return handle;
}

void loadVk(WsiShell shell)
{
	const char filename[] = "vulkan-1.dll";
	shell->hmodule_ = LoadLibrary(filename);
	shell->vkproc_ = NULL;

	if (shell->hmodule_)
	{
		shell->vkproc_ = (PFN_vkGetInstanceProcAddr) GetProcAddress(shell->hmodule_, "vkGetInstanceProcAddr");
	}

	if (!shell->hmodule_ || !shell->vkproc_)
	{
		assert("Failed to load Vulkan!");
	}
}

void createWindow(WsiShell shell, WsiShellCreateInfo createInfo)
{
	shell->hinstance_ = GetModuleHandle(NULL);

	WNDCLASSEXW win_class;
	ZeroMemory(&win_class, sizeof(win_class));
	win_class.cbSize = sizeof(WNDCLASSEX);
	win_class.style = CS_HREDRAW | CS_VREDRAW;
	win_class.lpfnWndProc = window_proc;
	win_class.cbClsExtra = 0;
	win_class.cbWndExtra = 0;
	win_class.hInstance = shell->hinstance_;
	win_class.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	win_class.hCursor = LoadCursor(NULL, IDC_ARROW);
	win_class.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	win_class.lpszMenuName = NULL;
	win_class.lpszClassName = "Shell";
	win_class.hIconSm = LoadIcon(NULL, IDI_WINLOGO);

	if (createInfo.pIcon != NULL)
	{
		win_class.hIcon = createIcon(*createInfo.pIcon, 0, 0, TRUE);
	}

	RegisterClassEx(&win_class);

	const DWORD win_style = WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE | WS_OVERLAPPEDWINDOW;

	RECT win_rect = {0, 0, createInfo.width, createInfo.height};
	AdjustWindowRect(&win_rect, win_style, 0);

	shell->hwnd_ = CreateWindowEx(WS_EX_APPWINDOW, "Shell", createInfo.pName, win_style, createInfo.x, createInfo.y,
		win_rect.right - win_rect.left, win_rect.bottom - win_rect.top, NULL, NULL, shell->hinstance_, NULL);

	SetForegroundWindow(shell->hwnd_);
	SetFocus(shell->hwnd_);
	SetWindowLongPtr(shell->hwnd_, GWLP_USERDATA, (LONG_PTR) shell);
}

void createInput(WsiShell shell)
{
	const RAWINPUTDEVICE rid = { HID_USAGE_PAGE_GENERIC, HID_USAGE_GENERIC_MOUSE, RIDEV_INPUTSINK, shell->hwnd_ };

	if (!RegisterRawInputDevices(&rid, 1, sizeof(rid)))
	{
		reportError("Win32: Failed to register raw input device");
	}
}

VkResult wsiCreateShell(const WsiShellCreateInfo *pCreateInfo, const VkAllocationCallbacks* pAllocator, WsiShell *pShell)
{
	if (pCreateInfo == NULL)
	{
		return VK_INCOMPLETE;
	}

	*pShell = VK_NULL_HANDLE;

	WsiShell_T *shell = calloc(1, sizeof(WsiShell_T));
	memset(shell, 0, sizeof(shell));
	shell->callbacks_ = *pCreateInfo->pCallbacks;

	loadVk(shell);
	createWindow(shell, *pCreateInfo);
	createInput(shell);

	*pShell = shell;
	return VK_SUCCESS;
}

void wsiDestroyShell(WsiShell shell, const VkAllocationCallbacks* pAllocator)
{
	CloseWindow(shell->hwnd_);
	FreeLibrary(shell->hmodule_);
}

void wsiGetShellCallbacks(WsiShell shell, WsiShellCallbacks **pCallbacks)
{
	*pCallbacks = &shell->callbacks_;
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

VkResult wsiCmdPollEvents(WsiShell shell)
{
	RECT area;
	GetClientRect(shell->hwnd_, &area);

	if (shell->cursorDisabled_)
	{
		SetCursorPos(area.right / 2, area.bottom / 2);
	}

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

VkResult wsiCmdSetResizable(WsiShell shell, VkBool32 resizable)
{
	return VK_SUCCESS; // TODO
}

VkResult wsiCmdSetSize(WsiShell shell, uint32_t width, uint32_t height)
{
	SetWindowPos(shell->hwnd_, HWND_TOP, 0, 0, width, height, SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOMOVE | SWP_NOZORDER);
	return VK_SUCCESS;
}

VkResult wsiCmdSetPosition(WsiShell shell, uint32_t x, uint32_t y)
{
	MoveWindow(shell->hwnd_, x, y, 0, 0, SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOSIZE);
	return VK_SUCCESS;
}

VkResult wsiCmdSetName(WsiShell shell, const char *pName)
{
	SetWindowText(shell->hwnd_, pName);
	return VK_SUCCESS;
}

VkResult wsiCmdSetFullscreen(WsiShell shell, WsiMonitor monitor, VkBool32 fullscreen)
{
	return VK_SUCCESS; // TODO
}

VkResult wsiCmdSetIcon(WsiShell shell, WsiImage icon)
{
	HICON bigIcon = createIcon(icon, 0, 0, TRUE);

	if (bigIcon == NULL)
	{
		return VK_ERROR_INITIALIZATION_FAILED;
	}

    SendMessage(shell->hwnd_, WM_SETICON, ICON_BIG, (LPARAM) bigIcon);
	return VK_SUCCESS;
}

VkResult wsiCmdSetCursor(WsiShell shell, WsiImage cursor)
{
	HICON icon = createIcon(cursor, 0, 0, FALSE);

	if (icon == NULL)
	{
		return VK_ERROR_INITIALIZATION_FAILED;
	}

	shell->cursor_ = (HCURSOR) icon;
	return VK_SUCCESS;
}

VkResult wsiCmdSetCursorMode(WsiShell shell, WsiCursorMode mode)
{
	shell->cursorMode_ = mode;

	if (mode == WSI_CURSOR_MODE_DISABLED)
	{
    	const RAWINPUTDEVICE rid = { HID_USAGE_PAGE_GENERIC, HID_USAGE_GENERIC_MOUSE, 0, shell->hwnd_ };

		shell->cursorDisabled_ = TRUE;
		updateCursorImage(shell);
		updateCursorClip(shell);

		if (!RegisterRawInputDevices(&rid, 1, sizeof(rid)))
    	{
    	    reportError("Win32: Failed to register raw input device");
    	}
	}
	else
	{
    	const RAWINPUTDEVICE rid = { HID_USAGE_PAGE_GENERIC, HID_USAGE_GENERIC_MOUSE, RIDEV_REMOVE, NULL };

		shell->cursorDisabled_ = FALSE;
		updateCursorClip(NULL);
		updateCursorImage(shell);

    	if (!RegisterRawInputDevices(&rid, 1, sizeof(rid)))
    	{
    	    reportError("Win32: Failed to remove raw input device");
    	}
	}

	return VK_SUCCESS;
}

VkResult wsiCmdSetCursorPos(WsiShell shell, uint32_t x, uint32_t y)
{
	SetCursorPos(x, y);
	return VK_SUCCESS;
}

#endif // VK_USE_PLATFORM_WIN32_KHR