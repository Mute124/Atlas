#pragma once

#include <memory>

#include "IAtlasEngine.h"
#include "../conf/Config.h"
#include "../dbg/Logging.h"
#include "../fs/FileSystemRegistry.h"
#include "../GameSettings.h"
#include "../input/InputRegistry.h"
#include "../modding/ScriptingAPI.h"
#include "../physics/Physics.h"
#include "../renderer/Renderer.h"
#include "../renderer/window/IWindow.h"

namespace Atlas {

	/**
	 * @brief 
	 */
	class AtlasEngine : public IAtlas {
	private:
		/**
		 * @brief A pointer to the config file registry. This is used to allow the user to register their own config files if they so choose.
		 * @note This starts as a nullptr
		 * @since v0.0.8
		 */
		ThreadSafeVariable<ConfigFileRegistry*> configFileRegistry = nullptr;
		/**
		 * @brief A pointer to the scripting API. This is used to allow the user to register their own API if they so choose.
		 * @note This starts as a nullptr
		 * @since v0.0.8
		 */
		ThreadSafeVariable<ScriptingAPI*> scriptingAPI = nullptr;

		/**
		 * @brief A pointer to the window. This is used to allow the user to register their own window if they so choose.
		 * @note This starts as a nullptr
		 * @since v0.0.8
		 */
		ThreadSafeVariable<IWindow*> window = nullptr;

		/**
		 * @brief A pointer to the renderer. This is used to allow the user to register their own renderer if they so choose.
		 * @note This starts as a nullptr
		 * @since v0.0.8
		 */
		ThreadSafeVariable<Renderer*> renderer = nullptr;

		/**
		 * @brief A pointer to the game settings. This is used to allow the user to register their own game settings if they so choose.
		 * @note This starts as a nullptr
		 * @warning This is not currently implemented nor used at the moment.
		 * @since v0.0.8
		 */
		ThreadSafeVariable<GameSettings*> gameSettings = nullptr;

		/**
		 * @brief A pointer to the file system registry. This is used to allow the user to register their own file system if they so choose.
		 * @note This starts as a nullptr
		 * @warning This is not currently implemented nor used at the moment.
		 * @since v0.0.8
		 */
		ThreadSafeVariable<FileSystemRegistry*> fileSystemRegistry = nullptr;

		/**
		 * @brief A pointer to the physics engine. This is used to allow the user to register their own physics engine if they so choose.
		 * @note This starts as a nullptr
		 * @since v0.0.8
		 */
		ThreadSafeVariable<PhysicsEngine*> physicsEngine = nullptr;

		/**
		 * @brief A pointer to the input registry. This is used to allow the user to register their own input registry if they so choose.
		 * @note This starts as a nullptr
		 * @since v0.0.8
		 */
		ThreadSafeVariable<InputRegistry*> inputRegistry = nullptr;

		/**
		 * @brief A pointer to the logger. This is used to allow the user to register their own logger if they so choose.
		 * @note This starts as a nullptr
		 * @since v0.0.8
		 */
		ThreadSafeVariable<Logger*> logger = nullptr;
	public:

		/**
		 * @brief Constructs a new AtlasEngine object.
		 * @param configFileRegistry A pointer to the config file registry object that Atlas will use
		 * @param scriptingAPI A pointer to the scripting API object that Atlas will use
		 * @param window A pointer to the window object that Atlas will use
		 * @param renderer A pointer to the renderer object that Atlas will use
		 * @param gameSettings A pointer to the game settings object that Atlas will use
		 * @param fileSystemRegistry A pointer to the file system registry object that Atlas will use
		 * @param physicsEngine A pointer to the physics engine object that Atlas will use
		 * @param inputRegistry A pointer to the input registry object that Atlas will use
		 * @param logger A pointer to the logger object that Atlas will use
		 * @since v0.0.8
		 */
		AtlasEngine(ThreadSafeVariable<ConfigFileRegistry*> configFileRegistry, ThreadSafeVariable<ScriptingAPI*> scriptingAPI,
			ThreadSafeVariable<IWindow*> window, ThreadSafeVariable<Renderer*> renderer, ThreadSafeVariable<GameSettings*> gameSettings,
			ThreadSafeVariable<FileSystemRegistry*> fileSystemRegistry, ThreadSafeVariable<PhysicsEngine*> physicsEngine,
			ThreadSafeVariable<InputRegistry*> inputRegistry, ThreadSafeVariable<Logger*> logger);

		/**
		 * @brief The default constructor.
		 */
		AtlasEngine() = default;

		/**
		 * @brief Gets the config file registry.
		 * @remarks This is a virtual function.
		 * @note This will return a nullptr if the config file registry is not set. This is done through the @ref ATLAS_GENERATED_NULL_CHECK macro
		 * @return A pointer to the config file registry
		 * @since v0.0.8
		 */
		ThreadSafeVariable<ConfigFileRegistry*> getConfigFileRegistry() override;

		/**
		* @brief Gets the scripting API.
		* @remarks This is a virtual function.
		* @note This will return a nullptr if the scripting API is not set. This is done through the @ref ATLAS_GENERATED_NULL_CHECK macro
		* @return A pointer to the scripting API
		* @since v0.0.8
		*/
		ThreadSafeVariable<ScriptingAPI*> getScriptingAPI() override;

		/**
		* @brief Gets the window.
		* @remarks This is a virtual function.
		* @note This will return a nullptr if the window is not set. This is done through the @ref ATLAS_GENERATED_NULL_CHECK macro
		* @return A pointer to the window
		* @since v0.0.8
		* */
		ThreadSafeVariable<IWindow*> getWindow() override;

		/**
		 * @brief Gets the renderer.
		 * @remarks This is a virtual function.
		 * @note This will return a nullptr if the renderer is not set. This is done through the @ref ATLAS_GENERATED_NULL_CHECK macro
		 * @return A pointer to the renderer
		 * @since v0.0.8
		 */
		ThreadSafeVariable<Renderer*> getRenderer() override;

		/**
		 * @brief 
		 * @return 
		 */
		ThreadSafeVariable<GameSettings*> getGameSettings() override;

		/**
		 * @brief 
		 * @return 
		 */
		ThreadSafeVariable<FileSystemRegistry*> getFileSystemRegistry() override;

		/**
		 * @brief 
		 * @return 
		 */
		ThreadSafeVariable<PhysicsEngine*> getPhysicsEngine() override;

		/**
		 * @brief 
		 * @return 
		 */
		ThreadSafeVariable<InputRegistry*> getInputRegistry() override;

		/**
		 * @brief 
		 * @return 
		 */
		ThreadSafeVariable<Logger*> getLogger() override;

		/**
		 * @brief 
		 * @param configFileRegistry 
		 */
		void setConfigFileRegistry(ThreadSafeVariable<ConfigFileRegistry*> configFileRegistry) override;

		/**
		 * @brief 
		 * @param scriptingAPI 
		 */
		void setScriptingAPI(ThreadSafeVariable<ScriptingAPI*> scriptingAPI) override;

		/**
		 * @brief 
		 * @param window 
		 */
		void setWindow(ThreadSafeVariable<IWindow*> window) override;

		/**
		 * @brief 
		 * @param renderer 
		 */
		void setRenderer(ThreadSafeVariable<Renderer*> renderer) override;

		/**
		 * @brief 
		 * @param gameSettings 
		 */
		void setGameSettings(ThreadSafeVariable<GameSettings*> gameSettings) override;

		/**
		 * @brief 
		 * @param fileSystemRegistry 
		 */
		void setFileSystemRegistry(ThreadSafeVariable<FileSystemRegistry*> fileSystemRegistry) override;

		/**
		 * @brief 
		 * @param physicsEngine 
		 */
		void setPhysicsEngine(ThreadSafeVariable<PhysicsEngine*> physicsEngine) override;

		/**
		 * @brief 
		 * @param inputRegistry 
		 */
		void setInputRegistry(ThreadSafeVariable<InputRegistry*> inputRegistry) override;

		/**
		 * @brief 
		 * @param logger 
		 */
		void setLogger(ThreadSafeVariable<Logger*> logger) override;
	};
}