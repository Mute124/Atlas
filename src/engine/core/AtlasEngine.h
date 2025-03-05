/*********************************************************************
 * @file   AtlasEngine.h
 * 
 * @brief  This file contains the main class of the engine. If you take
 * a look at @ref AtlasEngine, you will see that it contains multiple
 * different modules that can be used to extend the engine. If you want
 * to extend the engine, you can simply inherit the respective module
 * and utilize the concepts of polymorphism to your advantage. Should
 * you want to use a default implementation, this file is the one to 
 * use.
 * 
 * @date   February 2025
 * 
 * @since v0.0.8
 * 
 * @sa @ref IAtlasEngine for the basic interface of Atlas.
 * 
 * @sa @ref AtlasEngine for the default implementation of Atlas.
 *********************************************************************/
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

#include "ThreadSafeVariable.h"

/**
 * @brief This is what is being set as the default value for a module
 * within the AtlasEngine class.
 * 
 * @since v0.0.9
 */
#define ATLAS_DEFAULT_MODULE_VALUE nullptr

namespace Atlas {
	/**
	 * @brief This is the core and heart of Atlas. It is responsible for just
	 * about everything that is required to run the engine. It contains multiple
	 * different modules that can be used to extend the engine. See the remarks
	 * for a much more in-depth explanation of how to extend the engine with
	 * this.
	 * 
	 * @remarks If you want to extend the engine, you can simply inherit the
	 * respective module and utilize the concepts of polymorphism to your advantage.
	 * Keep in mind that the modules are stored as a ThreadSafeVariable and can 
	 * be used concurrently. That means the user can multi-thread these modules
	 * if they so choose. This is just a default implementation of the IAtlasEngine
	 * and you do not have to use this class. However, if you would like to use a 
	 * different implementation, you can simply inherit from IAtlasEngine and override
	 * the virtual functions. But one thing must remain: the class must inherit from
	 * IAtlasEngine or be a child of IAtlasEngine
	 * 
	 * @warning There should @b NOT be more than one instance of this object. 
	 * 
	 * @since v0.0.8
	 * 
	 * @sa @ref IAtlasEngine for the parent interface of this class.
	 * 
	 * @sa @ref BProject for where to start extending the engine (if you want to extend
	 * off of the basic implementation).
	 */
	class AtlasEngine : public IAtlasEngine {
	private:

		/**
		 * @brief Constructs a thread safe variable with the default value.
		 * 
		 * @remarks The reason for this function's existence is to reduce
		 * the amount of times the same code has to be written. Prior to
		 * the introduction of this function, the code would look like, as
		 * an example, this:
		 * @code
		 * ThreadSafeVariable<ConfigFileRegistry*> configFileRegistry = ThreadSafeVariable<ConfigFileRegistry*>(nullptr);
		 * 
		 * ThreadSafeVariable<ScriptingAPI*> scriptingAPI = ThreadSafeVariable<ScriptingAPI*>(nullptr);
		 * 
		 * ThreadSafeVariable<IWindow*> window = ThreadSafeVariable<IWindow*>(nullptr);
		 * 
		 * ThreadSafeVariable<Renderer*> renderer = ThreadSafeVariable<Renderer*>(nullptr);
		 * 
		 * ThreadSafeVariable<GameSettings*> gameSettings = ThreadSafeVariable<GameSettings*>(nullptr);
		 * 
		 * ThreadSafeVariable<FileSystemRegistry*> fileSystemRegistry = ThreadSafeVariable<FileSystemRegistry*>(nullptr);
		 * 
		 * ThreadSafeVariable<PhysicsEngine*> physicsEngine = ThreadSafeVariable<PhysicsEngine*>(nullptr);
		 * 
		 * ThreadSafeVariable<InputRegistry*> inputRegistry = ThreadSafeVariable<InputRegistry*>(nullptr);
		 * 
		 * ThreadSafeVariable<Logger*> logger = ThreadSafeVariable<Logger*>(nullptr);
		 * @endcode
		 * As you can see, the same constructor is being called multiple times.
		 * 
		 * @return A newly constructed thread safe variable instance.
		 * 
		 * @since v0.0.9
		 */
		template<class T_MODULE>
		static inline ThreadSafeVariable<T_MODULE*> ConstructModule() {
			return ThreadSafeVariable<T_MODULE*>(ATLAS_DEFAULT_MODULE_VALUE);
		}

		/**
		 * @brief A thread safe pointer to the config file registry. This is used to allow
		 * the user to register their own config files if they so choose.
		 * 
		 * @note This starts as a nullptr
		 * 
		 * @since v0.0.8
		 */
		ThreadSafeVariable<ConfigFileRegistry*> configFileRegistry = ConstructModule<ConfigFileRegistry>();

		/**
		 * @brief A pointer to the scripting API. This is used to allow the user
		 * to register their own API if they so choose.
		 * 
		 * @note This starts as a nullptr
		 * 
		 * @since v0.0.8
		 */
		ThreadSafeVariable<ScriptingAPI*> scriptingAPI = ConstructModule<ScriptingAPI>();

		/**
		 * @brief A pointer to the window. This is used to allow the user to register
		 * their own window if they so choose.
		 * 
		 * @note This starts as a nullptr
		 * 
		 * @since v0.0.8
		 */
		ThreadSafeVariable<IWindow*> window = ConstructModule<IWindow>();

		/**
		 * @brief A pointer to the renderer. This is used to allow the user to register
		 * their own renderer if they so choose.
		 * 
		 * @note This starts as a nullptr
		 * 
		 * @since v0.0.8
		 */
		ThreadSafeVariable<Renderer*> renderer = ConstructModule<Renderer>();

		/**
		 * @brief A pointer to the game settings. This is used to allow the user to register
		 * their own game settings if they so choose.
		 * 
		 * @note This starts as a nullptr
		 * 
		 * @warning This is not currently implemented nor used at the moment.
		 * 
		 * @since v0.0.8
		 */
		ThreadSafeVariable<GameSettings*> gameSettings = ConstructModule<GameSettings>();

		/**
		 * @brief A pointer to the file system registry. This is used to allow the user to register
		 * their own file system if they so choose.
		 * 
		 * @note This starts as a nullptr
		 * 
		 * @warning This is not currently implemented nor used at the moment.
		 * 
		 * @since v0.0.8
		 */
		ThreadSafeVariable<FileSystemRegistry*> fileSystemRegistry = ConstructModule<FileSystemRegistry>();

		/**
		 * @brief A pointer to the physics engine. This is used to allow the user to register
		 * their own physics engine if they so choose.
		 * 
		 * @note This starts as a nullptr
		 * 
		 * @since v0.0.8
		 */
		ThreadSafeVariable<PhysicsEngine*> physicsEngine = ConstructModule<PhysicsEngine>();

		/**
		 * @brief A pointer to the input registry. This is used to allow the user to register
		 * their own input registry if they so choose.
		 * 
		 * @note This starts as a nullptr
		 * 
		 * @since v0.0.8
		 */
		ThreadSafeVariable<InputRegistry*> inputRegistry = ConstructModule<InputRegistry>();

		/**
		 * @brief A pointer to the logger. This is used to allow the user to register their own
		 * logger if they so choose.
		 * 
		 * @note This starts as a nullptr
		 * 
		 * @since v0.0.8
		 */
		ThreadSafeVariable<Logger*> logger = ConstructModule<Logger>();
	public:

		/**
		 * @brief Constructs a new AtlasEngine object by manually setting the modules with
		 * their respective set functions. As is mentioned within the definition of this 
		 * function, the code here is boilerplate code. If this boilerplate code is not done,
		 * Atlas will not set the modules correctly.
		 * 
		 * @todo Add nullptr checks for the modules. This should also throw exceptions if the modules
		 * are not set.
		 * 
		 * @param configFileRegistry A pointer to the config file registry object that Atlas will use
		 * 
		 * @param scriptingAPI A pointer to the scripting API object that Atlas will use
		 * 
		 * @param window A pointer to the window object that Atlas will use
		 * 
		 * @param renderer A pointer to the renderer object that Atlas will use
		 * 
		 * @param gameSettings A pointer to the game settings object that Atlas will use
		 * 
		 * @param fileSystemRegistry A pointer to the file system registry object that Atlas will use
		 * 
		 * @param physicsEngine A pointer to the physics engine object that Atlas will use
		 * 
		 * @param inputRegistry A pointer to the input registry object that Atlas will use
		 * 
		 * @param logger A pointer to the logger object that Atlas will use
		 * 
		 * @since v0.0.8
		 */
		AtlasEngine(ThreadSafeVariable<ConfigFileRegistry*> configFileRegistry, ThreadSafeVariable<ScriptingAPI*> scriptingAPI,
			ThreadSafeVariable<IWindow*> window, ThreadSafeVariable<Renderer*> renderer, ThreadSafeVariable<GameSettings*> gameSettings,
			ThreadSafeVariable<FileSystemRegistry*> fileSystemRegistry, ThreadSafeVariable<PhysicsEngine*> physicsEngine,
			ThreadSafeVariable<InputRegistry*> inputRegistry, ThreadSafeVariable<Logger*> logger);

		/**
		 * @brief The default constructor of this class. 
		 * 
		 * @remarks Since all the variables are set to nullptr by default, 
		 * this constructor should only be used if you will manually set
		 * the modules. This @b MUST happen before the engine is initialized,
		 * ortherwise Atlas will not work, at all. This is because the
		 * engine relies on a module system to function properly. 
		 * 
		 * @since v0.0.8
		 */
		AtlasEngine() = default;

		/**
		 * @brief Gets the config file registry.
		 * 
		 * @remarks This is a virtual function.
		 * 
		 * @note This will return a nullptr if the config file registry is not set.
		 * This is done through the @ref ATLAS_GENERATED_NULL_CHECK macro
		 * 
		 * @return A pointer to the config file registry
		 * 
		 * @since v0.0.8
		 */
		ThreadSafeVariable<ConfigFileRegistry*> getConfigFileRegistry() override;

		/**
		* @brief Gets the scripting API.
		* 
		* @remarks This is a virtual function.
		* 
		* @note This will return a nullptr if the scripting API is not set.
		* This is done through the @ref ATLAS_GENERATED_NULL_CHECK macro
		* 
		* @return A pointer to the scripting API
		* 
		* @since v0.0.8
		*/
		ThreadSafeVariable<ScriptingAPI*> getScriptingAPI() override;

		/**
		* @brief Gets the window.
		* 
		* @remarks This is a virtual function.
		* 
		* @note This will return a nullptr if the window is not set.
		* This is done through the @ref ATLAS_GENERATED_NULL_CHECK macro
		* 
		* @return A pointer to the window
		* 
		* @since v0.0.8
		* */
		ThreadSafeVariable<IWindow*> getWindow() override;

		/**
		 * @brief Gets the renderer module.
		 * 
		 * @remarks This is a virtual function.
		 * 
		 * @note This will return a nullptr if the renderer is not set.
		 * This is done through the @ref ATLAS_GENERATED_NULL_CHECK macro
		 * 
		 * @return A pointer to the renderer
		 * 
		 * @since v0.0.8
		 */
		ThreadSafeVariable<Renderer*> getRenderer() override;

		/**
		 * @brief 
		 * 
		 * @return 
		 */
		ThreadSafeVariable<GameSettings*> getGameSettings() override;

		/**
		 * @brief 
		 * 
		 * @return 
		 */
		ThreadSafeVariable<FileSystemRegistry*> getFileSystemRegistry() override;

		/**
		 * @brief 
		 * 
		 * @return 
		 */
		ThreadSafeVariable<PhysicsEngine*> getPhysicsEngine() override;

		/**
		 * @brief 
		 * 
		 * @return 
		 */
		ThreadSafeVariable<InputRegistry*> getInputRegistry() override;

		/**
		 * @brief 
		 * 
		 * @return 
		 */
		ThreadSafeVariable<Logger*> getLogger() override;

		/**
		 * @brief 
		 * 
		 * @param configFileRegistry 
		 */
		void setConfigFileRegistry(ThreadSafeVariable<ConfigFileRegistry*> configFileRegistry) override;

		/**
		 * @brief 
		 * 
		 * @param scriptingAPI 
		 */
		void setScriptingAPI(ThreadSafeVariable<ScriptingAPI*> scriptingAPI) override;

		/**
		 * @brief 
		 * 
		 * @param window 
		 */
		void setWindow(ThreadSafeVariable<IWindow*> window) override;

		/**
		 * @brief 
		 * 
		 * @param renderer 
		 */
		void setRenderer(ThreadSafeVariable<Renderer*> renderer) override;

		/**
		 * @brief 
		 * 
		 * @param gameSettings 
		 */
		void setGameSettings(ThreadSafeVariable<GameSettings*> gameSettings) override;

		/**
		 * @brief 
		 * 
		 * @param fileSystemRegistry 
		 */
		void setFileSystemRegistry(ThreadSafeVariable<FileSystemRegistry*> fileSystemRegistry) override;

		/**
		 * @brief 
		 * 
		 * @param physicsEngine 
		 */
		void setPhysicsEngine(ThreadSafeVariable<PhysicsEngine*> physicsEngine) override;

		/**
		 * @brief 
		 * 
		 * @param inputRegistry 
		 */
		void setInputRegistry(ThreadSafeVariable<InputRegistry*> inputRegistry) override;

		/**
		 * @brief 
		 * 
		 * @param logger 
		 */
		void setLogger(ThreadSafeVariable<Logger*> logger) override;
	};
}