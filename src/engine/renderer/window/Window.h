/*****************************************************************//**
 * @file   Window.h
 * 
 * @brief  
 * 
 * @date   September 2024
 * 
 * @since v0.0.1
 *********************************************************************/
#pragma once

// Is this include needed?
#include <optional>

#include "WindowDecorations.h"
#include "IWindow.h"

namespace Atlas {

	/**
	 * @brief This is an implementation of the IWindow interface
	 * that is provided by default.
	 * 
	 * @since v0.0.1
	 */
	class Window : public IWindow {
	private:
		
		/**
		 * @brief .
		 * 
		 * @since v0.0.1
		 */
		WindowDecorations mDecorations;	
	public:

		/**
		 * @brief .
		 * 
		 * @since v0.0.1
		 */
		
		~Window();


		/**
		 * @brief .
		 * 
		 * @since v0.0.1
		 */
		WindowDecorations const& GetDecorations() const;

		/**
		 * @brief .
		 * 
		 * @param windowDecorations
		 * 
		 * @since 
		 */
		void init(WindowDecorations* windowDecorations) override;

		/**
		 * @brief .
		 * 
		 * @since v0.0.1
		 */
		void update() override;

		/**
		 * @brief .
		 * 
		 * @return 
		 * 
		 * @since v0.0.1
		 */
		bool shouldClose() override;

		/**
		 * @brief Closes the window.
		 * 
		 * @since v0.0.9
		 */
		void close() override;

		/**
		 * @brief Returns the native window handle as an std::any
		 * for the underlying platform. For example, it will return
		 * a HWND for win32.
		 * 
		 * @remarks As an example of how this function can be used,
		 * see the below example code (on a win32 platform):
		 * @code
		 * try{
		 *		HWND nativeHandle = std::any_cast<HWND>(getNativeHandle());
		 * } catch (const std::bad_any_cast& e) {
		 * 	// handle error
		 * }
		 * @endcode
		 * Make sure that you include the @a any and the @a windows
		 * header files wherever you use the above example code.
		 * 
		 * @return The platform-specific window handle as an std::any
		 * based on the underlying platform.
		 * 
		 * @since v0.0.9
		 * 
		 * @sa @ref getNativeHandle() For this function's sister
		 */
		std::any getNativePlatformHandle() override;

		/**
		 * @brief Returns a void* pointer of the native window handle.
		 * In the case of Windows, this will be a HWND.
		 * 
		 * @pre The window must have been initialized before this
		 * function can be called without issues.
		 * 
		 * @note Should you be looking for the window handle for
		 * what ever window management API (ie. GLFW, SDL, etc),
		 * use the @ref getNativePlatformHandle() function instead.
		 * 
		 * @return The native window handle as an std::any. This
		 * should be a glfwWindow* for GLFW and SDL_Window* for
		 * SDL.
		 * 
		 * @since v0.0.9
		 * 
		 * @sa @ref getNativePlatformHandle() For this function's
		 * sister function that returns the platform-specific
		 * window handle
		 */
		std::any getNativeHandle() override;

		void requestAttention() override;
	};
}