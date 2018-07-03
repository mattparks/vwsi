#ifdef VK_USE_PLATFORM_XCB_KHR

#include "VWSI/vulkan_wsi.h"

#include <xcb/xcb.h>
#include <dlfcn.h>
#include <stdio.h>
#include <assert.h>

typedef struct WsiMonitor_T
{

} WsiMonitor_T;

typedef struct WsiShell_T
{
	WsiShellCallbacks			callbacks_;

	xcb_connection_t*			connection_;
	xcb_screen_t* 				screen_;
	xcb_window_t 				window_;
	xcb_atom_t 					wm_protocols_;
	xcb_atom_t 					wm_delete_window_;

	void*						lib_handle_;
} WsiShell_T;

xcb_intern_atom_cookie_t intern_atom_cookie(xcb_connection_t *c, const char* s)
{
	return xcb_intern_atom(c, 0, strlen(s), s);
}

xcb_atom_t intern_atom(xcb_connection_t *c, xcb_intern_atom_cookie_t cookie)
{
	xcb_atom_t atom = XCB_ATOM_NONE;
	xcb_intern_atom_reply_t *reply = xcb_intern_atom_reply(c, cookie, NULL);

	if (reply)
	{
		atom = reply->atom;
		free(reply);
	}

	return atom;
}

void handle_event(WsiShell shell, const xcb_generic_event_t *ev)
{
	switch (ev->response_type & 0x7f)
	{
	case XCB_CONFIGURE_NOTIFY:
	{
		const xcb_configure_notify_event_t *notify = (xcb_configure_notify_event_t *)ev;

		if (shell->callbacks_.pfnSize != NULL)
		{
			shell->callbacks_.pfnSize(shell, notify->width, notify->height, 0);
		}
	}
		break;
	case XCB_KEY_PRESS:
	{
		const xcb_key_press_event_t *press = (xcb_key_press_event_t *)ev;
	}
		break;
	case XCB_CLIENT_MESSAGE:
	{
		const xcb_client_message_event_t *msg = (xcb_client_message_event_t *)ev;

		if (msg->type == shell->wm_protocols_ && msg->data.data32[0] == shell->wm_delete_window_)
		{
			if (shell->callbacks_.pfnClose != NULL)
			{
				shell->callbacks_.pfnClose(shell);
			}
		}
	} break;
	default:
		break;
	}
}

VkResult wsiEnumerateMonitors(uint32_t *pMonitorCount, WsiMonitor *pMonitors)
{
	return VK_SUCCESS;
}

void wsiGetMonitorProperties(WsiMonitor monitor, WsiMonitorProperties *pMonitorProperties)
{
}

void initConnection(WsiShell shell)
{
	int scr;

	shell->connection_ = xcb_connect(NULL, &scr);

	if (!shell->connection_ || xcb_connection_has_error(shell->connection_))
	{
		xcb_disconnect(shell->connection_);
		assert("Failed to connect to the display server.");
	}

	const xcb_setup_t *setup = xcb_get_setup(shell->connection_);
	xcb_screen_iterator_t iter = xcb_setup_roots_iterator(setup);

	while (scr-- > 0)
	{
		xcb_screen_next(&iter);
	}

	shell->screen_ = iter.data;
}

void loadVk(WsiShell shell)
{
	const char filename[] = "libvulkan.so.1";
	void *handle, *symbol;

#ifdef UNINSTALLED_LOADER
	handle = dlopen(UNINSTALLED_LOADER, RTLD_LAZY);

    if (!handle)
    {
    	handle = dlopen(filename, RTLD_LAZY);
    }
#else
	handle = dlopen(filename, RTLD_LAZY);
#endif

	if (handle) symbol = dlsym(handle, "vkGetInstanceProcAddr");

	if (!handle || !symbol)
	{
		printf("%s\n", dlerror());

		if (handle)
		{
			dlclose(handle);
		}

		assert("Failed to load Vulkan!");
	}

	shell->lib_handle_ = handle;
}

void createWindow(WsiShell shell, WsiShellCreateInfo createInfo)
{
	shell->window_ = xcb_generate_id(shell->connection_);

	uint32_t value_mask, value_list[32];
	value_mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
	value_list[0] = shell->screen_->black_pixel;
	value_list[1] = XCB_EVENT_MASK_KEY_PRESS | XCB_EVENT_MASK_STRUCTURE_NOTIFY;

	xcb_create_window(shell->connection_, XCB_COPY_FROM_PARENT, shell->window_, shell->screen_->root, 0, 0, createInfo.width, createInfo.height, 0,
		XCB_WINDOW_CLASS_INPUT_OUTPUT, shell->screen_->root_visual, value_mask, value_list);

	xcb_intern_atom_cookie_t utf8_string_cookie = intern_atom_cookie(shell->connection_, "UTF8_STRING");
	xcb_intern_atom_cookie_t _net_wm_name_cookie = intern_atom_cookie(shell->connection_, "_NET_WM_NAME");
	xcb_intern_atom_cookie_t wm_protocols_cookie = intern_atom_cookie(shell->connection_, "WM_PROTOCOLS");
	xcb_intern_atom_cookie_t wm_delete_window_cookie = intern_atom_cookie(shell->connection_, "WM_DELETE_WINDOW");

	// set title
	xcb_atom_t utf8_string = intern_atom(shell->connection_, utf8_string_cookie);
	xcb_atom_t _net_wm_name = intern_atom(shell->connection_, _net_wm_name_cookie);
	xcb_change_property(shell->connection_, XCB_PROP_MODE_REPLACE, shell->window_, _net_wm_name, utf8_string, 8, strlen(createInfo.pName),
		createInfo.pName);

	// advertise WM_DELETE_WINDOW
	shell->wm_protocols_ = intern_atom(shell->connection_, wm_protocols_cookie);
	shell->wm_delete_window_ = intern_atom(shell->connection_, wm_delete_window_cookie);
	xcb_change_property(shell->connection_, XCB_PROP_MODE_REPLACE, shell->window_, shell->wm_protocols_, XCB_ATOM_ATOM, 32, 1, &shell->wm_delete_window_);
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

	initConnection(shell);
	loadVk(shell);
	createWindow(shell, *pCreateInfo);

	*pShell = shell;
	return VK_SUCCESS;
}

void wsiDestroyShell(WsiShell shell, const VkAllocationCallbacks* pAllocator)
{
	xcb_destroy_window(shell->connection_, shell->window_);
	xcb_flush(shell->connection_);
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
	pExtensions[1] = VK_KHR_XCB_SURFACE_EXTENSION_NAME;
	return VK_SUCCESS;
}

VkResult wsiCreateSurface(WsiShell shell, VkInstance instance, const VkAllocationCallbacks *pAllocator, VkSurfaceKHR *pSurface)
{
	if (shell == NULL)
	{
		return VK_ERROR_MEMORY_MAP_FAILED;
	}

	*pSurface = VK_NULL_HANDLE;

	VkXcbSurfaceCreateInfoKHR surfaceCreateInfo;
	surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
	surfaceCreateInfo.connection = shell->connection_;
	surfaceCreateInfo.window = shell->window_;

	return vkCreateXcbSurfaceKHR(instance, &surfaceCreateInfo, pAllocator, pSurface);
}

VkResult wsiCmdPollEvents(WsiShell shell)
{
	xcb_generic_event_t *ev = xcb_poll_for_event(shell->connection_);

	if (ev != NULL)
	{
		handle_event(shell, ev);
		free(ev);
	}

	return VK_SUCCESS;
}

VkResult wsiCmdSetResizable(WsiShell shell, VkBool32 resizable)
{
	return VK_SUCCESS;
}

VkResult wsiCmdSetSize(WsiShell shell, uint32_t width, uint32_t height)
{
	return VK_SUCCESS;
}

VkResult wsiCmdSetPosition(WsiShell shell, uint32_t x, uint32_t y)
{
	return VK_SUCCESS;
}

VkResult wsiCmdSetName(WsiShell shell, const char *pName)
{
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

VkResult wsiCmdSetCursorPos(WsiShell shell, uint32_t x, uint32_t y)
{
	return VK_SUCCESS;
}

#endif // VK_USE_PLATFORM_XCB_KHR