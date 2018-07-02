#include <stdio.h>
#include <string>
#include <vector>
#include <stdexcept>
#include <VWSI/vulkan_wsi.h>
//#include <VWSI/vulkan_wsi.hpp>

std::string StringifyResultVk(const VkResult &result)
{
	switch (result)
	{
	case VK_SUCCESS:
		return "Success";
	case VK_NOT_READY:
		return "A fence or query has not yet completed";
	case VK_TIMEOUT:
		return "A wait operation has not completed in the specified time";
	case VK_EVENT_SET:
		return "An event is signaled";
	case VK_EVENT_RESET:
		return "An event is unsignaled";
	case VK_INCOMPLETE:
		return "A return array was too small for the result";
	case VK_ERROR_OUT_OF_HOST_MEMORY:
		return "A host memory allocation has failed";
	case VK_ERROR_OUT_OF_DEVICE_MEMORY:
		return "A device memory allocation has failed";
	case VK_ERROR_INITIALIZATION_FAILED:
		return "Initialization of an object could not be completed for implementation-specific reasons";
	case VK_ERROR_DEVICE_LOST:
		return "The logical or physical device has been lost";
	case VK_ERROR_MEMORY_MAP_FAILED:
		return "Mapping of a memory object has failed";
	case VK_ERROR_LAYER_NOT_PRESENT:
		return "A requested layer is not present or could not be loaded";
	case VK_ERROR_EXTENSION_NOT_PRESENT:
		return "A requested extension is not supported";
	case VK_ERROR_FEATURE_NOT_PRESENT:
		return "A requested feature is not supported";
	case VK_ERROR_INCOMPATIBLE_DRIVER:
		return "The requested version of Vulkan is not supported by the driver or is otherwise incompatible";
	case VK_ERROR_TOO_MANY_OBJECTS:
		return "Too many objects of the type have already been created";
	case VK_ERROR_FORMAT_NOT_SUPPORTED:
		return "A requested format is not supported on this device";
	case VK_ERROR_SURFACE_LOST_KHR:
		return "A surface is no longer available";
	case VK_SUBOPTIMAL_KHR:
		return "A swapchain no longer matches the surface properties exactly, but can still be used";
	case VK_ERROR_OUT_OF_DATE_KHR:
		return "A surface has changed in such a way that it is no longer compatible with the swapchain";
	case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:
		return "The display used by a swapchain does not use the same presentable image layout";
	case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
		return "The requested window is already connected to a VkSurfaceKHR, or to some other non-Vulkan API";
	case VK_ERROR_VALIDATION_FAILED_EXT:
		return "A validation layer found an error";
	default:
		return "ERROR: UNKNOWN VULKAN ERROR";
	}
}

void ErrorVk(const VkResult &result)
{
	if (result >= 0)
	{
		return;
	}

	std::string failure = StringifyResultVk(result);

	fprintf(stderr, "Vulkan error: %s, %i\n", failure.c_str(), result);
#ifdef FL_BUILD_WINDOWS
	MessageBox(nullptr, failure.c_str(), "Vulkan Error", 0);
#endif
	throw std::runtime_error("Vulkan runtime error.");
}

bool m_closed = false;

void CallbackPosition(WsiShell shell, uint32_t x, uint32_t y)
{
}

void CallbackSize(WsiShell shell, uint32_t width, uint32_t height, VkBool32 fullscreen)
{
	printf("%ix%i\n", width, height);
}

void CallbackFocus(WsiShell shell, VkBool32 focused)
{
}

void CallbackIconify(WsiShell shell, VkBool32 iconified)
{
}

void CallbackClose(WsiShell shell)
{
	printf("Closing display!\n");
	m_closed = true;
}

void CallbackCursorPosition(WsiShell shell, float x, float y)
{
}

void CallbackCursorEnter(WsiShell shell, VkBool32 entered)
{
}

void CallbackCursorScroll(WsiShell shell, float x, float y)
{
}

void CallbackKey(WsiShell shell, WsiKey key, WsiAction action, int mods)
{
	printf("%i, %i\n", key, action);
}

void CallbackMouseButton(WsiShell shell, WsiMouseButton mouseButton, WsiAction action)
{
}

void CallbackTouch(WsiShell shell, float x, float y, WsiAction action)
{
}

void CallbackJoystickConnect(WsiShell shell, WsiJoystick port, const char *name, uint32_t buttonCount, uint32_t axesCount, VkBool32 connected)
{
}

void CallbackJoystickButton(WsiShell shell, WsiJoystick port, uint32_t button, WsiAction action)
{
}

void CallbackJoystickAxis(WsiShell shell, WsiJoystick port, uint32_t axis, float amount)
{
	printf("Hello World!\n");
}

int main(int argc, char **argv)
{
	uint32_t instanceMonitorCount;
	wsiEnumerateMonitors(&instanceMonitorCount, nullptr);
	std::vector<WsiMonitor> monitors(instanceMonitorCount);
	wsiEnumerateMonitors(&instanceMonitorCount, monitors.data());

	WsiMonitorProperties monitorProperties;
	wsiGetMonitorProperties(monitors[0], &monitorProperties);

	WsiShellCallbacks shellCallbacks = {};
	shellCallbacks.pfnPosition = CallbackPosition;
	shellCallbacks.pfnSize = CallbackSize;
	shellCallbacks.pfnFocus = CallbackFocus;
	shellCallbacks.pfnIconify = CallbackIconify;
	shellCallbacks.pfnClose = CallbackClose;
	shellCallbacks.pfnCursorPosition = CallbackCursorPosition;
	shellCallbacks.pfnCursorEnter = CallbackCursorEnter;
	shellCallbacks.pfnCursorScroll = CallbackCursorScroll;
	shellCallbacks.pfnKey = CallbackKey;
	shellCallbacks.pfnMouseButton = CallbackMouseButton;
	shellCallbacks.pfnTouch = CallbackTouch;
	//shellCallbacks.pfnJoystickConnect = CallbackJoystickConnect;
	//shellCallbacks.pfnJoystickButton = CallbackJoystickButton;
	//shellCallbacks.pfnJoystickAxis = CallbackJoystickAxis;

	WsiImage icon = {};
	icon.width = 32;
	icon.height = 32;
	icon.pixels = nullptr;

	WsiShellCreateInfo shellCreateInfo = {};
	shellCreateInfo.sType = WSI_STRUCTURE_TYPE_SHELL_CREATE_INFO;
	shellCreateInfo.pCallbacks = &shellCallbacks;
	shellCreateInfo.pIcon = &icon;
	shellCreateInfo.pCursor = nullptr;
	shellCreateInfo.width = 1080;
	shellCreateInfo.height = 720;
	shellCreateInfo.x = (monitorProperties.width - shellCreateInfo.width) / 2;
	shellCreateInfo.y = (monitorProperties.height - shellCreateInfo.height) / 2;
	shellCreateInfo.resizable = true;
	shellCreateInfo.pName = "Hello World";

	WsiShell shell;
	ErrorVk(wsiCreateShell(&shellCreateInfo, nullptr, &shell));

	WsiShellCallbacks *callbacks;
	wsiGetShellCallbacks(shell, &callbacks);
	callbacks->pfnJoystickConnect = CallbackJoystickConnect;
	callbacks->pfnJoystickButton = CallbackJoystickButton;
	callbacks->pfnJoystickAxis = CallbackJoystickAxis;

//	wsiCmdSetSize(shell, 720, 480);
//	wsiCmdSetPosition(shell, 300, 200);
//	wsiCmdSetName(shell, "New Name");
//	wsiCmdSetIcon(shell, &icon);
//	wsiCmdSetFullscreen(shell, monitors[0], true);

	VkApplicationInfo applicationInfo = {};
	applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	applicationInfo.pApplicationName = "Hello World";
	applicationInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	applicationInfo.pEngineName = "No Engine";
	applicationInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	applicationInfo.apiVersion = VK_API_VERSION_1_0;

	uint32_t instanceExtensionCount;
	wsiEnumerateShellExtensions(shell, &instanceExtensionCount, nullptr);
	std::vector<const char *> instanceExtensions(instanceExtensionCount);
	wsiEnumerateShellExtensions(shell, &instanceExtensionCount, instanceExtensions.data());

	VkInstanceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &applicationInfo;
	createInfo.enabledExtensionCount = instanceExtensionCount;
	createInfo.ppEnabledExtensionNames = instanceExtensions.data();
	createInfo.enabledLayerCount = 0;
	createInfo.ppEnabledLayerNames = nullptr;

	VkInstance instance;
	ErrorVk(vkCreateInstance(&createInfo, nullptr, &instance));

	/*uint32_t physicalDeviceCount;
	vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr);
	std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
	vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, physicalDevices.data());
	VkPhysicalDevice physicalDevice = physicalDevices[0];

	uint32_t graphicsFamilyIndex = 0;

	float queuePriorities[] = {1.0f};
	VkDeviceQueueCreateInfo deviceQueueCreateInfo = {};
	deviceQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	deviceQueueCreateInfo.queueFamilyIndex = graphicsFamilyIndex;
	deviceQueueCreateInfo.queueCount = 1;
	deviceQueueCreateInfo.pQueuePriorities = queuePriorities;

	VkPhysicalDeviceFeatures physicalDeviceFeatures = {};
	physicalDeviceFeatures.samplerAnisotropy = VK_TRUE;
	physicalDeviceFeatures.shaderClipDistance = VK_TRUE;
	physicalDeviceFeatures.shaderCullDistance = VK_TRUE;
	physicalDeviceFeatures.fillModeNonSolid = VK_TRUE;
	physicalDeviceFeatures.fragmentStoresAndAtomics = VK_TRUE;

	std::vector<const char *> deviceExtensionList = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

	VkDeviceCreateInfo deviceCreateInfo = {};
	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceCreateInfo.pEnabledFeatures = &physicalDeviceFeatures;
	deviceCreateInfo.queueCreateInfoCount = 1;
	deviceCreateInfo.pQueueCreateInfos = &deviceQueueCreateInfo;
	deviceCreateInfo.enabledLayerCount = 0;
	deviceCreateInfo.ppEnabledLayerNames = NULL;
	deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensionList.size());
	deviceCreateInfo.ppEnabledExtensionNames = deviceExtensionList.data();

	VkDevice logicalDevice;
	ErrorVk(vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, &logicalDevice));

	VkQueue queue;
	vkGetDeviceQueue(logicalDevice, graphicsFamilyIndex, 0, &queue);*/

	VkSurfaceKHR surface;
	ErrorVk(wsiCreateSurface(shell, instance, nullptr, &surface));

	//VkSurfaceCapabilitiesKHR surfaceCapabilities;
	//ErrorVk(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &surfaceCapabilities));

	while (!m_closed)
	{
		ErrorVk(wsiCmdPollEvents(shell));
	}

	wsiDestroyShell(shell, nullptr);
	vkDestroySurfaceKHR(instance, surface, nullptr);
	vkDestroyInstance(instance, nullptr);

//	std::cin.get();
	return 0;
}