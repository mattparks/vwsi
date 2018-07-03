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
#define HID_USAGE_PAGE_GENERIC         ((USHORT) 0x01)
#endif
#ifndef HID_USAGE_GENERIC_MOUSE
#define HID_USAGE_GENERIC_MOUSE        ((USHORT) 0x02)
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

	// Keyboard input.
	short int keycodes_[512];
	short int scancodes_[WSI_KEY_MAX_ENUM];
} WsiShell_T;

void reportError(const char *message)
{
	printf("VWSI Error: %s\n", message);
}

void createKeyTables(WsiShell shell)
{
	int scancode;

    memset(shell->keycodes_, -1, sizeof(shell->keycodes_));
    memset(shell->scancodes_, -1, sizeof(shell->scancodes_));

    shell->keycodes_[0x00B] = WSI_KEY_0;
    shell->keycodes_[0x002] = WSI_KEY_1;
    shell->keycodes_[0x003] = WSI_KEY_2;
    shell->keycodes_[0x004] = WSI_KEY_3;
    shell->keycodes_[0x005] = WSI_KEY_4;
    shell->keycodes_[0x006] = WSI_KEY_5;
    shell->keycodes_[0x007] = WSI_KEY_6;
    shell->keycodes_[0x008] = WSI_KEY_7;
    shell->keycodes_[0x009] = WSI_KEY_8;
    shell->keycodes_[0x00A] = WSI_KEY_9;
    shell->keycodes_[0x01E] = WSI_KEY_A;
    shell->keycodes_[0x030] = WSI_KEY_B;
    shell->keycodes_[0x02E] = WSI_KEY_C;
    shell->keycodes_[0x020] = WSI_KEY_D;
    shell->keycodes_[0x012] = WSI_KEY_E;
    shell->keycodes_[0x021] = WSI_KEY_F;
    shell->keycodes_[0x022] = WSI_KEY_G;
    shell->keycodes_[0x023] = WSI_KEY_H;
    shell->keycodes_[0x017] = WSI_KEY_I;
    shell->keycodes_[0x024] = WSI_KEY_J;
    shell->keycodes_[0x025] = WSI_KEY_K;
    shell->keycodes_[0x026] = WSI_KEY_L;
    shell->keycodes_[0x032] = WSI_KEY_M;
    shell->keycodes_[0x031] = WSI_KEY_N;
    shell->keycodes_[0x018] = WSI_KEY_O;
    shell->keycodes_[0x019] = WSI_KEY_P;
    shell->keycodes_[0x010] = WSI_KEY_Q;
    shell->keycodes_[0x013] = WSI_KEY_R;
    shell->keycodes_[0x01F] = WSI_KEY_S;
    shell->keycodes_[0x014] = WSI_KEY_T;
    shell->keycodes_[0x016] = WSI_KEY_U;
    shell->keycodes_[0x02F] = WSI_KEY_V;
    shell->keycodes_[0x011] = WSI_KEY_W;
    shell->keycodes_[0x02D] = WSI_KEY_X;
    shell->keycodes_[0x015] = WSI_KEY_Y;
    shell->keycodes_[0x02C] = WSI_KEY_Z;

    shell->keycodes_[0x028] = WSI_KEY_APOSTROPHE;
    shell->keycodes_[0x02B] = WSI_KEY_BACKSLASH;
    shell->keycodes_[0x033] = WSI_KEY_COMMA;
    shell->keycodes_[0x00D] = WSI_KEY_EQUAL;
    shell->keycodes_[0x029] = WSI_KEY_GRAVE_ACCENT;
    shell->keycodes_[0x01A] = WSI_KEY_LEFT_BRACKET;
    shell->keycodes_[0x00C] = WSI_KEY_MINUS;
    shell->keycodes_[0x034] = WSI_KEY_PERIOD;
    shell->keycodes_[0x01B] = WSI_KEY_RIGHT_BRACKET;
    shell->keycodes_[0x027] = WSI_KEY_SEMICOLON;
    shell->keycodes_[0x035] = WSI_KEY_SLASH;
    shell->keycodes_[0x056] = WSI_KEY_WORLD_2;

    shell->keycodes_[0x00E] = WSI_KEY_BACKSPACE;
    shell->keycodes_[0x153] = WSI_KEY_DELETE;
    shell->keycodes_[0x14F] = WSI_KEY_END;
    shell->keycodes_[0x01C] = WSI_KEY_ENTER;
    shell->keycodes_[0x001] = WSI_KEY_ESCAPE;
    shell->keycodes_[0x147] = WSI_KEY_HOME;
    shell->keycodes_[0x152] = WSI_KEY_INSERT;
    shell->keycodes_[0x15D] = WSI_KEY_MENU;
    shell->keycodes_[0x151] = WSI_KEY_PAGE_DOWN;
    shell->keycodes_[0x149] = WSI_KEY_PAGE_UP;
    shell->keycodes_[0x045] = WSI_KEY_PAUSE;
    shell->keycodes_[0x146] = WSI_KEY_PAUSE;
    shell->keycodes_[0x039] = WSI_KEY_SPACE;
    shell->keycodes_[0x00F] = WSI_KEY_TAB;
    shell->keycodes_[0x03A] = WSI_KEY_CAPS_LOCK;
    shell->keycodes_[0x145] = WSI_KEY_NUM_LOCK;
    shell->keycodes_[0x046] = WSI_KEY_SCROLL_LOCK;
    shell->keycodes_[0x03B] = WSI_KEY_F1;
    shell->keycodes_[0x03C] = WSI_KEY_F2;
    shell->keycodes_[0x03D] = WSI_KEY_F3;
    shell->keycodes_[0x03E] = WSI_KEY_F4;
    shell->keycodes_[0x03F] = WSI_KEY_F5;
    shell->keycodes_[0x040] = WSI_KEY_F6;
    shell->keycodes_[0x041] = WSI_KEY_F7;
    shell->keycodes_[0x042] = WSI_KEY_F8;
    shell->keycodes_[0x043] = WSI_KEY_F9;
    shell->keycodes_[0x044] = WSI_KEY_F10;
    shell->keycodes_[0x057] = WSI_KEY_F11;
    shell->keycodes_[0x058] = WSI_KEY_F12;
    shell->keycodes_[0x064] = WSI_KEY_F13;
    shell->keycodes_[0x065] = WSI_KEY_F14;
    shell->keycodes_[0x066] = WSI_KEY_F15;
    shell->keycodes_[0x067] = WSI_KEY_F16;
    shell->keycodes_[0x068] = WSI_KEY_F17;
    shell->keycodes_[0x069] = WSI_KEY_F18;
    shell->keycodes_[0x06A] = WSI_KEY_F19;
    shell->keycodes_[0x06B] = WSI_KEY_F20;
    shell->keycodes_[0x06C] = WSI_KEY_F21;
    shell->keycodes_[0x06D] = WSI_KEY_F22;
    shell->keycodes_[0x06E] = WSI_KEY_F23;
    shell->keycodes_[0x076] = WSI_KEY_F24;
    shell->keycodes_[0x038] = WSI_KEY_LEFT_ALT;
    shell->keycodes_[0x01D] = WSI_KEY_LEFT_CONTROL;
    shell->keycodes_[0x02A] = WSI_KEY_LEFT_SHIFT;
    shell->keycodes_[0x15B] = WSI_KEY_LEFT_SUPER;
    shell->keycodes_[0x137] = WSI_KEY_PRINT_SCREEN;
    shell->keycodes_[0x138] = WSI_KEY_RIGHT_ALT;
    shell->keycodes_[0x11D] = WSI_KEY_RIGHT_CONTROL;
    shell->keycodes_[0x036] = WSI_KEY_RIGHT_SHIFT;
    shell->keycodes_[0x15C] = WSI_KEY_RIGHT_SUPER;
    shell->keycodes_[0x150] = WSI_KEY_DOWN;
    shell->keycodes_[0x14B] = WSI_KEY_LEFT;
    shell->keycodes_[0x14D] = WSI_KEY_RIGHT;
    shell->keycodes_[0x148] = WSI_KEY_UP;

    shell->keycodes_[0x052] = WSI_KEY_PAD_0;
    shell->keycodes_[0x04F] = WSI_KEY_PAD_1;
    shell->keycodes_[0x050] = WSI_KEY_PAD_2;
    shell->keycodes_[0x051] = WSI_KEY_PAD_3;
    shell->keycodes_[0x04B] = WSI_KEY_PAD_4;
    shell->keycodes_[0x04C] = WSI_KEY_PAD_5;
    shell->keycodes_[0x04D] = WSI_KEY_PAD_6;
    shell->keycodes_[0x047] = WSI_KEY_PAD_7;
    shell->keycodes_[0x048] = WSI_KEY_PAD_8;
    shell->keycodes_[0x049] = WSI_KEY_PAD_9;
    shell->keycodes_[0x04E] = WSI_KEY_PAD_ADD;
    shell->keycodes_[0x053] = WSI_KEY_PAD_DECIMAL;
    shell->keycodes_[0x135] = WSI_KEY_PAD_DIVIDE;
    shell->keycodes_[0x11C] = WSI_KEY_PAD_ENTER;
    shell->keycodes_[0x037] = WSI_KEY_PAD_MULTIPLY;
    shell->keycodes_[0x04A] = WSI_KEY_PAD_SUBTRACT;

    for (scancode = 0; scancode < 512; scancode++)
    {
        if (shell->keycodes_[scancode] > 0)
        {
			shell->scancodes_[shell->keycodes_[scancode]] = scancode;
        }
    }
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

    return shell->keycodes_[HIWORD(lParam) & 0x1FF];
}

WsiModifierFlags getKeyMods(void)
{
	WsiModifierFlags mods = 0;

	if (GetKeyState(VK_SHIFT) & 0x8000)
	{
		mods |= WSI_MODIFIER_SHIFT_BIT;
	}

	if (GetKeyState(VK_CONTROL) & 0x8000)
	{
		mods |= WSI_MODIFIER_CONTROL_BIT;
	}

	if (GetKeyState(VK_MENU) & 0x8000)
	{
		mods |= WSI_MODIFIER_ALT_BIT;
	}

	if ((GetKeyState(VK_LWIN) | GetKeyState(VK_RWIN)) & 0x8000)
	{
		mods |= WSI_MODIFIER_SUPER_BIT;
	}

	if (GetKeyState(VK_CAPITAL) & 1)
	{
		mods |= WSI_MODIFIER_CAPS_LOCK_BIT;
	}

	if (GetKeyState(VK_NUMLOCK) & 1)
	{
		mods |= WSI_MODIFIER_NUM_LOCK_BIT;
	}

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
			shell->callbacks_.pfnSize(shell, w, h, FALSE);
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
	// TODO: Iconify
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

		if (data->data.mouse.usFlags & MOUSE_MOVE_ABSOLUTE)
		{
			shell->cursorPosX = data->data.mouse.lLastX;
			shell->cursorPosY = data->data.mouse.lLastY;
		}
		else
		{
			shell->cursorPosX += data->data.mouse.lLastX;
			shell->cursorPosY += data->data.mouse.lLastY;
		}

		//	wsiCmdSetCursorPos(shell, shell->cursorPosX, shell->cursorPosY);

		if (shell->callbacks_.pfnCursorPosition != NULL)
		{
			shell->callbacks_.pfnCursorPosition(shell, shell->cursorPosX, shell->cursorPosY);
		}

		break;
	}
    case WM_MOUSEMOVE:
    {
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
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
	{
		WsiKey key = translateKey(shell, wParam, lParam);
		int scancode = (lParam >> 16) & 0x1ff;
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
				shell->callbacks_.pfnKey(shell, WSI_KEY_LEFT_SHIFT, scancode, action, mods);
				shell->callbacks_.pfnKey(shell, WSI_KEY_RIGHT_SHIFT, scancode, action, mods);
			}
			else if (wParam == VK_SNAPSHOT)
			{
				// Key down is not reported for the Print Screen key
				shell->callbacks_.pfnKey(shell, key, scancode, WSI_ACTION_PRESS, mods);
				shell->callbacks_.pfnKey(shell, key, scancode, WSI_ACTION_RELEASE, mods);
			}
			else
			{
				shell->callbacks_.pfnKey(shell, key, scancode, action, mods);
			}
		}

		break;
	}
    /*case WM_DEVICECHANGE:
    {
    	if (wParam == DBT_DEVICEARRIVAL)
    	{
            DEV_BROADCAST_HDR *dbh = (DEV_BROADCAST_HDR*) lParam;

            if (dbh && dbh->dbch_devicetype == DBT_DEVTYP_DEVICEINTERFACE)
            {
				if (shell->callbacks_.pfnJoystickConnect != NULL)
				{
					shell->callbacks_.pfnJoystickConnect(shell, WSI_JOYSTICK_1, "test", 10, 10, TRUE);
				}
			}
    	}
    	else if (wParam == DBT_DEVICEREMOVECOMPLETE)
    	{
        	DEV_BROADCAST_HDR *dbh = (DEV_BROADCAST_HDR*) lParam;

        	if (dbh && dbh->dbch_devicetype == DBT_DEVTYP_DEVICEINTERFACE)
        	{
				if (shell->callbacks_.pfnJoystickConnect != NULL)
				{
					shell->callbacks_.pfnJoystickConnect(shell, WSI_JOYSTICK_1, "NULL", 0, 0, FALSE);
				}
        	}
    	}
		break;
	}*/
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
    int i;
    HDC dc;
    HICON handle;
    HBITMAP color, mask;
    BITMAPV5HEADER bi;
    ICONINFO ii;
    unsigned char* target = NULL;
    unsigned char* source = image.pixels;

    ZeroMemory(&bi, sizeof(bi));
    bi.bV5Size        = sizeof(bi);
    bi.bV5Width       = image.width;
    bi.bV5Height      = -image.height;
    bi.bV5Planes      = 1;
    bi.bV5BitCount    = 32;
    bi.bV5Compression = BI_BITFIELDS;
    bi.bV5RedMask     = 0x00ff0000;
    bi.bV5GreenMask   = 0x0000ff00;
    bi.bV5BlueMask    = 0x000000ff;
    bi.bV5AlphaMask   = 0xff000000;

    dc = GetDC(NULL);
    color = CreateDIBSection(dc,
                             (BITMAPINFO*) &bi,
                             DIB_RGB_COLORS,
                             (void**) &target,
                             NULL,
                             (DWORD) 0);
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

    for (i = 0;  i < image.width * image.height;  i++)
    {
        target[0] = source[2];
        target[1] = source[1];
        target[2] = source[0];
        target[3] = source[3];
        target += 4;
        source += 4;
    }

    ZeroMemory(&ii, sizeof(ii));
    ii.fIcon    = icon;
    ii.xHotspot = xhot;
    ii.yHotspot = yhot;
    ii.hbmMask  = mask;
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

void load_vk(WsiShell shell)
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

void create_window(WsiShell shell, WsiShellCreateInfo createInfo)
{
	shell->hinstance_ = GetModuleHandle(NULL);

	WNDCLASSEX win_class = {};
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

void create_input(WsiShell shell)
{
    RAWINPUTDEVICE Rid[1];
    Rid[0].usUsagePage = HID_USAGE_PAGE_GENERIC;
    Rid[0].usUsage = HID_USAGE_GENERIC_MOUSE;
    Rid[0].dwFlags = RIDEV_INPUTSINK;
    Rid[0].hwndTarget = shell->hwnd_;
    RegisterRawInputDevices(Rid, 1, sizeof(Rid[0]));
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

	createKeyTables(shell);
	load_vk(shell);
	create_window(shell, *pCreateInfo);
	create_input(shell);

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
	MSG msg;

	while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			if (shell->callbacks_.pfnClose != NULL)
			{
				shell->callbacks_.pfnClose(shell);
			}

		if (shell->callbacks_.pfnIconify != NULL)
		{
			printf("icn\n");
			shell->callbacks_.pfnIconify(shell, IsIconic(shell->hwnd_));
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
    	const RAWINPUTDEVICE rid = { 0x01, 0x02, 0, shell->hwnd_ };

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
    	const RAWINPUTDEVICE rid = { 0x01, 0x02, RIDEV_REMOVE, NULL };

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

VkResult wsiCmdSetCursorPos(WsiShell shell, float x, float y)
{
	SetCursorPos(x, y);
	return VK_SUCCESS;
}

#endif // VK_USE_PLATFORM_WIN32_KHR