#include "OS.h"


#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
//#define GLFW_EXPOSE_NATIVE_WGL
//#define GLFW_NATIVE_INCLUDE_NONE
#include <GLFW/glfw3native.h>

#ifdef ATLAS_PLATFORM_WINDOWS
	#include <Windows.h>
	#include <gamingdeviceinformation.h>
#endif

#ifdef ATLAS_PLATFORM_WINDOWS

Atlas::Win32DWORD Atlas::TranslateWindowAttentionRequestTypeWin32( EWindowAttentionRequestType const& type)
{
	constexpr Win32DWORD defaultValue = FLASHW_ALL;

	switch (type)
	{
	using enum Atlas::EWindowAttentionRequestType;
	case UNKNOWN:
		return defaultValue;
		break;
	case STOP:
		return FLASHW_STOP;
		break;
	case ALL:
		return FLASHW_ALL;
		break;
	case TASKBAR:
		return FLASHW_CAPTION;
		break;
	case TRAY:
		return FLASHW_TRAY;
		break;
	case LOOP:
		return FLASHW_TIMER;
		break;
	case UNTIL_FOCUSED:
		return FLASHW_TIMER;
		break;
	default:
		return defaultValue;
		break;
	}
}

void Atlas::RequestWindowAttentionWin32(ThreadSafeVariable<IWindow*> window, WindowAttentionRequestArgs* args)
{
	HWND win32WindowHandle = NULL;

	try {		
		win32WindowHandle = (HWND)std::any_cast<void*>(window->getNativeHandle());
	} catch (std::bad_any_cast& e) {
		std::cout << e.what() << std::endl;
	}

	if(win32WindowHandle == nullptr) {
		return;
	}

	//if (win32WindowHandle != NULL) {
		//::MessageBox(win32WindowHandle, "Atlas Engine", "Alert", MB_OK);
	//}	
	
	FLASHWINFO flashInfo;
	flashInfo.cbSize = sizeof(FLASHWINFO);
	flashInfo.hwnd = win32WindowHandle;
	flashInfo.dwFlags = TranslateWindowAttentionRequestTypeWin32(args->cFlags);
	flashInfo.uCount = args->cFlashCount; // Number of times to flash
	flashInfo.dwTimeout = args->cTimeout;

	FlashWindowEx(&flashInfo);
}

#endif

#ifdef ATLAS_PLATFORM_LINUX

void Atlas::RequestWindowAttentionLinux(ThreadSafeVariable<IWindow*> window, WindowAttentionRequestArgs* args)
{
}

#endif

void Atlas::RequestWindowAttention(ThreadSafeVariable<IWindow*> window, WindowAttentionRequestArgs* args)
{
#ifdef ATLAS_PLATFORM_WINDOWS
	Atlas::RequestWindowAttention(window, args);
#endif

#ifdef ATLAS_PLATFORM_LINUX
	Atlas::RequestWindowAttentionLinux(window, args);
#endif
	
}

bool Atlas::IsdeviceXbox()
{
#ifdef ATLAS_PLATFORM_LINUX
	return false;
#else
	//GAMING_DEVICE_MODEL_INFORMATION deviceModelInformation;
	//HRESULT result = GetGamingDeviceModelInformation(&deviceModelInformation);

	//if (FAILED(result)) {
	//	return false;
	//}//

	// See https://learn.microsoft.com/en-us/windows/win32/api/gamingdeviceinformation/ne-gamingdeviceinformation-gaming_device_device_id
	return false; //deviceModelInformation.deviceId != GAMING_DEVICE_DEVICE_ID_NONE;

#endif


}

std::string Atlas::GetXboxDeviceName()
{
#ifdef ATLAS_PLATFORM_WINDOWS
	return "This function is not yet supported on Windows";
#else
	return "Unknown";
#endif
}
