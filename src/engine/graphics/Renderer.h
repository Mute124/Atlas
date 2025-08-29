/**************************************************************************************************
 * @file Renderer.h
 * 
 * @brief .
 * 
 * @date April 2025
 *
 * @since v
 ***************************************************************************************************/
#pragma once

#include <vector>
#include <memory>
#include <mutex>
#include <chrono>

#include "../core/Core.h"
#include "../core/Module.h"

#include "window/Window.h"
#include "backend/RenderingBackend.h"


namespace Atlas {
	/**
	 * @brief A base class for all renderers. This class is responsible for @b frontend rendering of your game. The rendering backend is responsible for the
	 * actual rendering. When using this class, you should inherit from it and implement the `init` and `update` methods. If you want this class to be
	 * multi-threaded by Atlas, you should make sure that the `init` and `update` methods are thread-safe. Additionally, make sure you set
	 * @ref canBeMultiThreaded to @a true if you want this to take effect. If you do not, Atlas will only run the `init` and `update` methods on the rendering
	 * thread instead of accounting for your renderer's need to be multi-threaded.
	 * 
	 * @note Terms used in this class's documentation:
	 * - **renderer thread**: The thread that Atlas uses to draw your game to the screen. 
	 * - **rendering backend**: API wrapper that Atlas uses to draw your game to the screen. For example, Vulkan, Raylib, and OpenGL are all rendering APIs.
	 * - **the renderer**: Either your implementation of this interface class, a wrapper around one, or this class.
	 * 
	 * @since v
	 * 
	 * @sa @ref EngineModule to read the documentation of this class's parent.
	 */
	class ARenderer : public EngineModule {
	protected:
		/**
		 * @brief Whether or not the renderer has been initialized.
		 *
		 * @remarks This does not neccesarily mean that the renderer is ready to be used.
		 *
		 * @note This should only be modified by the renderer's `init` method.
		 *
		 * @since v
		 */
		bool mIsInitialized = false;

		/**
		 * @brief Signals to Atlas whether or not the renderer can be multi-threaded. By default it is set to @a false.
		 *
		 * @note This should be set before the renderer's `init` method is called.
		 *
		 * @since v
		 */
		const bool mCanBeMultiThreaded = false;
		
		ARenderer(AGameWindow* gameWindow, RenderingBackend* backend, bool canBeMultiThreaded);

	public:
		/**
		 * @brief A polymorphic pointer to the main window that the renderer is rendering to. 
		 
		 * @note This is something that you should set before the renderer's `init` method is called. 
		 * 
		 * @note You should not change this value after the renderer's `init` method is called. Furthermore, the window's initialization should be done in the renderer's `init`
		 * method. Typically, this is simply calling `mainGameWindow->init()`. It is also recommended that you do not open the window in the renderer's `init` method yet in order
		 * ensure that any necessary initialization is done before the window is opened (even if that initialization is not done by the renderer).
		 * 
		 * @since v
		 */
		AGameWindow* mainGameWindow = nullptr;

		/**
		 * @brief A polymorphic pointer to the rendering backend that the renderer is using. 
		 * 
		 * @details This is the API wrapper that Atlas uses to draw your game to the screen. All triangles should be drawn using this backend.
		 * 
		 * @note This is something that you should set before the renderer's `init` method is called.
		 * 
		 * @since v
		 */
		RenderingBackend* renderingBackend = nullptr;

		/**
		 * @brief Sets the main game window and rendering backend to the specified parameters.
		 * 
		 * @param gameWindow 

		 * @param backend 
		 * 
		 * @since v
		 */
		ARenderer(AGameWindow* gameWindow, RenderingBackend* backend);

		/**
		 * @brief This is one of the most important methods in this class. It is responsible for initializing the renderer, ensuring that all setup is done correctly. It should initialize
		 * the main game window using whatever methods are needed to do so. It should also initialize the rendering backend.
		 *
		 * @since v
		 */
		virtual void init() = 0;
		
		virtual void update() = 0;

		virtual void cleanup() = 0;

		virtual bool shouldExit() = 0;

		bool canBeMultiThreaded() const;

		bool isInitialized() const;
	
		template<typename T_BACKEND_TYPE>
		T_BACKEND_TYPE* castRenderingBackendAs() {
			return this->renderingBackend->castAs<T_BACKEND_TYPE*>();
		}
	};
#ifdef ATLAS_RENDERER_3D
	 
#elif ATLAS_RENDERER_2D

#else
	#ifndef ATLAS_SUPPRESS_UNKOWN_RENDERER_ERROR
		#error "Unsupported renderer"
	#endif
#endif
}

