/*****************************************************************//**
 * @file   OS.h
 * 
 * @brief  A bunch of utilities that can interact with the OS in 
 * a mostly cross-platform way. Alot of this simply wraps around
 * the Windows API and the Linux API. The utilities try to remain
 * as close to how they originaly were written as possible, but
 * some changes have been made to make them more cross-platform.
 * 
 * @warning This file shall @b NEVER include raylib because raylib
 * will have name conflicts with the Windows API.
 * 
 * @date   February 2025
 * 
 * @since v0.0.9
 *********************************************************************/
#pragma once
#include "../renderer/window/IWindow.h"
#include "../core/ThreadSafeVariable.h"

#include "../core/CompileTimeCommons.h"

namespace Atlas {

#ifdef ATLAS_PLATFORM_WINDOWS
	/**
	 * @brief Equivalent to the DWORD type within the Windows API.
	 * The DWORD type is an unsigned 32-bit integer.
	 *
	 * @since v0.0.9
	 */
	using Win32DWORD = unsigned long;

	/**
	 * @brief The different types of window attention requests that can
	 * be used in RequestWindowAttention(). The @b enums are based on the
	 * Windows API attention request types, but the values are @b NOT
	 * always the same because of the different platforms and for formatting
	 * purposes.
	 * 
	 * @warning The values are not the same as the ones in the Windows API
	 * because the values are different for each platform. The enum is
	 * translated to the correct value for each respective platform.
	 * 
	 * @since v0.0.9
	 */
	enum class EWindowAttentionRequestType : int {
		/**
		 * @brief An unknown type of an attention request for a window. This
		 * either means:
		 * - The attention request type is unknown
		 * - The attention request type is not supported
		 * - Some other error occurred
		 * 
		 * @since v0.0.9
		 */
		UNKNOWN = -1,

		/**
		 * @brief Signals that whatever attention request is currently running
		 * should be stopped. Equivalent to the FLASHW_STOP macro in WinUser.h
		 * for Windows.
		 * 
		 * @since v0.0.9
		 */
		STOP,

		/**
		 * @brief The attention request being created should show up on the taskbar
		 * and in the tray. Equivalent to the FLASHW_ALL macro in WinUser.h for
		 * Windows.
		 *
		 * @since v0.0.9
		 */
		ALL,

		/**
		 * @brief The attention request being created should show up on the
		 * taskbar. Equivalent to the FLASHW_CAPTION macro in WinUser.h for
		 * Windows.
		 * 
		 * @since v0.0.9
		 */
		TASKBAR,

		/**
		 * @brief The attention request being created should show up on the tray.
		 * Equivalent to the FLASHW_TRAY macro in WinUser.h for Windows.
		 * 
		 * @since v0.0.9
		 */
		TRAY,

		/**
		 * @brief The attention request being created should continue to flash
		 * until the STOP flag is used. Equivalent to the
		 * FLASHW_TIMER macro in WinUser.h for Windows.
		 * 
		 * @since v0.0.9
		 */
		LOOP,

		/**
		 * @brief The attention request being created should continue to flash
		 * until the window is focused. Equivalent to the FLASHW_TIMERNOFG
		 * macro in WinUser.h for Windows.
		 * 
		 * @since v0.0.9
		 */
		UNTIL_FOCUSED
	};

	/**
	 * @brief The different models of Xbox devices. This is based on the
	 * GAMING_DEVICE_DEVICE_ID enum in the GamingDeviceInformation.h
	 * header file. Additionally, the values of this are the @b same as
	 * the ones in the GAMING_DEVICE_DEVICE_ID enum.
	 * 
	 * @since v0.0.9
	 * 
	 * @sa https://learn.microsoft.com/en-us/windows/win32/api/gamingdeviceinformation/ne-gamingdeviceinformation-gaming_device_device_id
	 * For Microsoft's documentation on the GAMING_DEVICE_DEVICE_ID enum
	 * that this
	 * mimics.
	 */
	enum class EXboxDeviceModel : int {
		/**
		 * @brief An unknown Xbox model or the device is not an Xbox.
		 * 
		 * @since v0.0.9
		 */
		UNKNOWN_MODEL = -1,

		/**
		 * @brief The device has been shown to not  be an Xbox. This is
		 * equivalent to GAMING_DEVICE_DEVICE_ID_NONE value in the
		 * GamingDeviceInformation.h header file.
		 * 
		 * @since v0.0.9
		 * 
		 * @sa https://learn.microsoft.com/en-us/windows/win32/api/gamingdeviceinformation/ne-gamingdeviceinformation-gaming_device_device_id
		 * For Microsoft's documentation on the GAMING_DEVICE_DEVICE_ID enum
		 * that this mimics.
		 */
		NOT_A_XBOX = 0,

		/**
		 * @brief The device is an Xbox One. This is equivalent to the 
		 * GAMING_DEVICE_DEVICE_ID_XBOX_ONE value in the GamingDeviceInformation.h.
		 * This value is set to 0x768BAE26 so that it matches the value of the
		 * GAMING_DEVICE_DEVICE_ID_XBOX_ONE enum.
		 * 
		 * @since v0.0.9
		 * 
		 * @sa https://learn.microsoft.com/en-us/windows/win32/api/gamingdeviceinformation/ne-gamingdeviceinformation-gaming_device_device_id
		 * For Microsoft's documentation on the GAMING_DEVICE_DEVICE_ID enum
		 * that this mimics.
		 */
		XBOX_ONE = 0x768BAE26,

		/**
		 * @brief The device is an Xbox One S. This is equivalent to the
		 * GAMING_DEVICE_DEVICE_ID_XBOX_ONE_S value in the GamingDeviceInformation.h.
		 * This value is set to 0x2A7361D9 so that it matches the value of the
		 * GAMING_DEVICE_DEVICE_ID_XBOX_ONE_S enum. 
		 *  
		 * @since v0.0.9
		 * 
		 * @sa https://learn.microsoft.com/en-us/windows/win32/api/gamingdeviceinformation/ne-gamingdeviceinformation-gaming_device_device_id
		 * For Microsoft's documentation on the GAMING_DEVICE_DEVICE_ID enum
		 * that this mimics.
		 */
		XBOX_ONE_S = 0x2A7361D9,

		/**
		 * @brief The device is an Xbox One X. This is equivalent to the
		 * GAMING_DEVICE_DEVICE_ID_XBOX_ONE_X value in the GamingDeviceInformation.h.
		 * This value is set to 0x5AD617C7 so that it matches the value of the
		 * GAMING_DEVICE_DEVICE_ID_XBOX_ONE_X enum.
		 * 
		 * @since v0.0.9
		 * 
		 * @sa https://learn.microsoft.com/en-us/windows/win32/api/gamingdeviceinformation/ne-gamingdeviceinformation-gaming_device_device_id
		 * For Microsoft's documentation on the GAMING_DEVICE_DEVICE_ID enum
		 * that this mimics.
		 */
		XBOX_ONE_X = 0x5AD617C7,

		/**
		 * @brief The device is an Xbox One X Devkit. This is equivalent to the
		 * GAMING_DEVICE_DEVICE_ID_XBOX_ONE_X_DEVKIT value in the
		 * GamingDeviceInformation.h. This value is set to 0x10F7CDE3 so that
		 * it matches the value of the GAMING_DEVICE_DEVICE_ID_XBOX_ONE_X_DEVKIT
		 * enum.
		 * 
		 * @since v0.0.9
		 * 
		 * @sa https://learn.microsoft.com/en-us/windows/win32/api/gamingdeviceinformation/ne-gamingdeviceinformation-gaming_device_device_id
		 * For Microsoft's documentation on the GAMING_DEVICE_DEVICE_ID enum
		 * that this mimics.
		 */
		XBOX_ONE_X_DEVKIT = 0x10F7CDE3,

		/**
		 * @brief The device is an Xbox Series. This is equivalent to the
		 * GAMING_DEVICE_DEVICE_ID_XBOX_SERIES value in the
		 * GamingDeviceInformation.h.
		 * 
		 * @since v0.0.9
		 * 
		 * @sa https://learn.microsoft.com/en-us/windows/win32/api/gamingdeviceinformation/ne-gamingdeviceinformation-gaming_device_device_id
		 * For Microsoft's documentation on the GAMING_DEVICE_DEVICE_ID enum
		 * that this mimics.
		 */
		XBOX_SERIES_S,

		/**
		 * @brief The device is an Xbox Series X. This is equivalent to the
		 * GAMING_DEVICE_DEVICE_ID_XBOX_SERIES_X value in the
		 * GamingDeviceInformation.h.
		 * 
		 * @since v0.0.9
		 * 
		 * @sa https://learn.microsoft.com/en-us/windows/win32/api/gamingdeviceinformation/ne-gamingdeviceinformation-gaming_device_device_id
		 * For Microsoft's documentation on the GAMING_DEVICE_DEVICE_ID enum
		 * that this mimics.
		 */
		XBOX_SERIES_X,

		/**
		 * @brief The device is an Xbox Series X Devkit. This is equivalent to the
		 * GAMING_DEVICE_DEVICE_ID_XBOX_SERIES_X_DEVKIT value in the
		 * GamingDeviceInformation.h.
		 * 
		 * @since v0.0.9
		 * 
		 * @sa https://learn.microsoft.com/en-us/windows/win32/api/gamingdeviceinformation/ne-gamingdeviceinformation-gaming_device_device_id
		 * For Microsoft's documentation on the GAMING_DEVICE_DEVICE_ID enum
		 * that this mimics.
		 */
		XBOX_SERIES_X_DEVKIT
	};

	/**
	 * @brief The arguments for the RequestWindowAttention function.
	 * 
	 * @since v0.0.9
	 */
	using WindowAttentionRequestArgs = struct {
		/**
		 * @brief Denotes the type of attention request.
		 * 
		 * @since v0.0.9
		 */
		const EWindowAttentionRequestType cFlags;

		/**
		 * @brief The length of time that each flash should last. In other words,
		 * the interval between each flash. If set to 0, the default interval will
		 * be used.
		 * 
		 * @since v0.0.9
		 */
		const Win32DWORD cTimeout;

		/**
		 * @brief The amount of times that the attention request should flash.
		 * 
		 * @since v0.0.9
		 */
		const Win32DWORD cFlashCount;
	};
#endif

#ifdef ATLAS_PLATFORM_WINDOWS

	/**
	 * @brief Converts the EWindowAttentionRequestType enum to a Win32DWORD. The
	 * values it returns are the same as the FLASHW_* macros in WinUser.h. If the 
	 * value of the enum is unknown, it will return the value of the FLASHW_ALL
	 * because that is a generic value. This only works for Windows platforms.
	 * 
	 * @since v0.0.9
	 */
	Win32DWORD TranslateWindowAttentionRequestTypeWin32(EWindowAttentionRequestType const& type);

	/**
	 * @brief Equivalent to the FlashWindowEx function within the
	 * Windows API. This function will cause the window on the 
	 * taskbar to flash, aka "alert". This will only work on Windows
	 * platforms.
	 * 
	 * @pre The window must have been initialized before this is called.
	 * 
	 * @param window The window module within the engine
	 * 
	 * @since v0.0.9
	 */
	void RequestWindowAttentionWin32(ThreadSafeVariable<IWindow*> window, WindowAttentionRequestArgs* args);
#endif

#ifdef ATLAS_PLATFORM_LINUX

	/**
	 * @brief The linux version of the RequestWindowAttention function.
	 * 
	 * @pre The window must have been initialized before this is called.
	 * 
	 * @param window The window module within the engine
	 * 
	 * @since v0.0.9
	 * 
	 * @sa @ref RequestWindowAttentionWin32() For the Window's equivalent
	 * 
	 * @todo Implement this
	 */
	void RequestWindowAttentionLinux(ThreadSafeVariable<IWindow*> window, WindowAttentionRequestArgs* args);

#endif

	/**
	 * @brief Alerts the window in a cross-platform way by calling
	 * RequestWindowAttention() and RequestWindowAttentionLinux() depending on the OS
	 * being used.
	 * 
	 * @pre The window must have been initialized before this is called.
	 * 
	 * @param window The window module within the engine
	 * 
	 * @since v0.0.9
	 */	
	void RequestWindowAttention(ThreadSafeVariable<IWindow*> window, WindowAttentionRequestArgs* args);

	/**
	 * @brief Determines if the current device is an Xbox. This is accomplished by
	 * using the gamingdeviceinformation.h header file and the 
	 * GetGamingDeviceModelInformation() function.
	 * 
	 * @since v0.0.9
	 * 
	 * @sa https://learn.microsoft.com/en-us/windows/win32/api/gamingdeviceinformation/ne-gamingdeviceinformation-gaming_device_device_id 
	 * For Microsoft's documentation on the GAMING_DEVICE_DEVICE_ID enum that is used to 
	 * determine if the device is an Xbox or not.
	 */
	bool IsdeviceXbox();

	/**
	 * @brief Returns the name of the current Xbox model, if the device is an Xbox.
	 * If the device is not an Xbox, it will return an empty string.
	 * 
	 * @warning This is not implemented and is not used yet.
	 * 
	 * @since v0.0.9
	 * 
	 * @sa @ref IsdeviceXbox() for how this checks if the current device is an Xbox.
	 */
	std::string GetXboxDeviceName();

	/*********************************************************************
	* Non-Cross-Platform Utilities
	**********************************************************************/


}