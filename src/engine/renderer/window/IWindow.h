/*****************************************************************//**
 * @file   IWindow.h
 * 
 * @brief  
 * 
 * @date   November 2024
 * 
 * @since v0.0.1
 *********************************************************************/
#pragma once
#include <any>

#include "WindowDecorations.h"

#include <GLFW/glfw3.h>

namespace Atlas {
	/**
	 * @brief This is an int.
	 * 
	 * @since 
	 */
	class IWindow {
	public:


		virtual void init(WindowDecorations* windowDecorations) = 0;

		virtual void update() = 0;

		virtual bool shouldClose() = 0;

		virtual void close() = 0;

		virtual std::any getNativePlatformHandle() = 0;
		
		virtual std::any getNativeHandle() = 0;

		virtual void requestAttention() = 0;
	};
}