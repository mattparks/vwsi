#ifdef VK_USE_PLATFORM_XCB_KHR
#include "wsi_xcb.c"
#elif VK_USE_PLATFORM_WAYLAND_KHR
#include "wsi_wayland.c"
#elif VK_USE_PLATFORM_WIN32_KHR
#include "wsi_win32.c"
#elif VK_USE_PLATFORM_ANDROID_KHR
#include "wsi_android.c"
#endif
