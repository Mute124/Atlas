#pragma once
#include "IAtlasEngine.h"

namespace Atlas {
	class AtlasEngine : public IAtlas {
	private:
		/**
		 * @brief A pointer to the config file registry. This is used to allow the user to register their own config files if they so choose.
		 * @note This starts as a nullptr
		 * @since v0.0.8
		 */
		ConfigFileRegistry* configFileRegistry = nullptr;
		/**
		 * @brief A pointer to the scripting API. This is used to allow the user to register their own API if they so choose.
		 * @note This starts as a nullptr
		 * @since v0.0.8
		 */
		ScriptingAPI* scriptingAPI = nullptr;

		/**
		 * @brief A pointer to the window. This is used to allow the user to register their own window if they so choose.
		 * @note This starts as a nullptr
		 * @since v0.0.8
		 */
		IWindow* window = nullptr;

		/**
		 * @brief A pointer to the renderer. This is used to allow the user to register their own renderer if they so choose.
		 * @note This starts as a nullptr
		 * @since v0.0.8
		 */
		Renderer* renderer = nullptr;

		/**
		 * @brief A pointer to the game settings. This is used to allow the user to register their own game settings if they so choose.
		 * @note This starts as a nullptr
		 * @warning This is not currently implemented nor used at the moment.
		 * @since v0.0.8
		 */
		GameSettings* gameSettings = nullptr;

		/**
		 * @brief A pointer to the file system registry. This is used to allow the user to register their own file system if they so choose.
		 * @note This starts as a nullptr
		 * @warning This is not currently implemented nor used at the moment.
		 * @since v0.0.8
		 */
		FileSystemRegistry* fileSystemRegistry = nullptr;

		/**
		 * @brief A pointer to the physics engine. This is used to allow the user to register their own physics engine if they so choose.
		 * @note This starts as a nullptr
		 * @since v0.0.8
		 */
		PhysicsEngine* physicsEngine = nullptr;

		/**
		 * @brief A pointer to the input registry. This is used to allow the user to register their own input registry if they so choose.
		 * @note This starts as a nullptr
		 * @since v0.0.8
		 */
		InputRegistry* inputRegistry = nullptr;

		/**
		 * @brief A pointer to the logger. This is used to allow the user to register their own logger if they so choose.
		 * @note This starts as a nullptr
		 * @since v0.0.8
		 */
		Logger* logger = nullptr;
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
		AtlasEngine(ConfigFileRegistry* configFileRegistry, ScriptingAPI* scriptingAPI, IWindow* window, Renderer* renderer,
					GameSettings* gameSettings, FileSystemRegistry* fileSystemRegistry, PhysicsEngine* physicsEngine,
			InputRegistry* inputRegistry, Logger* logger);

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
		ConfigFileRegistry* getConfigFileRegistry() override;

		/**
		* @brief Gets the scripting API.
		* @remarks This is a virtual function.
		* @note This will return a nullptr if the scripting API is not set. This is done through the @ref ATLAS_GENERATED_NULL_CHECK macro
		* @return A pointer to the scripting API
		* @since v0.0.8
		*/
		ScriptingAPI* getScriptingAPI() override;

		/**
		* @brief Gets the window.
		* @remarks This is a virtual function.
		* @note This will return a nullptr if the window is not set. This is done through the @ref ATLAS_GENERATED_NULL_CHECK macro
		* @return A pointer to the window
		* @since v0.0.8
		* */
		IWindow* getWindow() override;

		/**
		 * @brief Gets the renderer.
		 * @remarks This is a virtual function.
		 * @note This will return a nullptr if the renderer is not set. This is done through the @ref ATLAS_GENERATED_NULL_CHECK macro
		 * @return A pointer to the renderer
		 * @since v0.0.8
		 */
		Renderer* getRenderer() override;


		GameSettings* getGameSettings() override;


		FileSystemRegistry* getFileSystemRegistry() override;


		PhysicsEngine* getPhysicsEngine() override;


		InputRegistry* getInputRegistry() override;

		Logger* getLogger() override;

		void setConfigFileRegistry(ConfigFileRegistry* configFileRegistry) override;


		void setScriptingAPI(ScriptingAPI* scriptingAPI) override;


		void setWindow(IWindow* window) override;


		void setRenderer(Renderer* renderer) override;


		void setGameSettings(GameSettings* gameSettings) override;


		void setFileSystemRegistry(FileSystemRegistry* fileSystemRegistry) override;

		void setPhysicsEngine(PhysicsEngine* physicsEngine) override;


		void setInputRegistry(InputRegistry* inputRegistry) override;

		void setLogger(Logger* logger) override;
	};
}