/**
* \file Project.h
* 
* \brief This file contains the BProject interface that is required for Atlas to run the user's project. The reason for the boilerplate is because it allows the engine to take a Laissez-faire approach and allow the user to do whatever they want.
* with all their code being able to be called from the engine.
* 
*/
#pragma once
#include "renderer/Renderer.h"
#include "renderer/window/IWindow.h"
#include "renderer/window/WindowDecorations.h"
#include "utils/Singleton.h"
#include "modding/ScriptingAPI.h"
#include "GameSettings.h"

#include "conf/Config.h"
#include "physics/Physics.h"
#include "input/Input.h"
#include "fs/FileSystemRegistry.h"
#include <memory>

namespace Atlas {
	class IAtlas {
	public:

		virtual ConfigFileRegistry* getConfigFileRegistry() = 0;
		virtual ScriptingAPI* getScriptingAPI() = 0;
		virtual IWindow* getWindow() = 0;
		virtual Renderer* getRenderer() = 0;
		virtual GameSettings* getGameSettings() = 0;
		virtual FileSystemRegistry* getFileSystemRegistry() = 0;
		virtual PhysicsEngine* getPhysicsEngine() = 0;
		virtual InputRegistry* getInputRegistry() = 0;
		virtual Logger* getLogger() = 0;

		virtual void setConfigFileRegistry(ConfigFileRegistry* configFileRegistry) = 0;
		virtual void setScriptingAPI(ScriptingAPI* scriptingAPI) = 0;
		virtual void setWindow(IWindow* window) = 0;
		virtual void setRenderer(Renderer* renderer) = 0;
		virtual void setGameSettings(GameSettings* gameSettings) = 0;
		virtual void setFileSystemRegistry(FileSystemRegistry* fileSystemRegistry) = 0;
		virtual void setPhysicsEngine(PhysicsEngine* physicsEngine) = 0;
		virtual void setInputRegistry(InputRegistry* inputRegistry) = 0;
		virtual void setLogger(Logger* logger) = 0;
	};

	class AtlasEngine : public IAtlas {
	private:		
		/**
		 * @brief A pointer to the config file registry. This is used to allow the user to register their own config files if they so choose.
		 * @note This starts as a nullptr
		 * @version v0.0.8
		 */
		ConfigFileRegistry* configFileRegistry = nullptr;
		/**
		 * @brief A pointer to the scripting API. This is used to allow the user to register their own API if they so choose.
		 * @note This starts as a nullptr
		 * @version v0.0.8
		 */
		ScriptingAPI* scriptingAPI = nullptr;
		
		/**
		 * @brief A pointer to the window. This is used to allow the user to register their own window if they so choose.
		 * @note This starts as a nullptr
		 * @version v0.0.8
		 */
		IWindow* window = nullptr;
		
		/**
		 * @brief A pointer to the renderer. This is used to allow the user to register their own renderer if they so choose.
		 * @note This starts as a nullptr
		 * @version v0.0.8
		 */
		Renderer* renderer = nullptr;
		
		/**
		 * @brief A pointer to the game settings. This is used to allow the user to register their own game settings if they so choose.
		 * @note This starts as a nullptr
		 * @warning This is not currently implemented nor used at the moment.
		 * @version v0.0.8
		 */
		GameSettings* gameSettings = nullptr;
		
		/**
		 * @brief A pointer to the file system registry. This is used to allow the user to register their own file system if they so choose.
		 * @note This starts as a nullptr
		 * @warning This is not currently implemented nor used at the moment.
		 * @version v0.0.8
		 */
		FileSystemRegistry* fileSystemRegistry = nullptr;
		
		/**
		 * @brief A pointer to the physics engine. This is used to allow the user to register their own physics engine if they so choose.
		 * @note This starts as a nullptr
		 * @version v0.0.8
		 */
		PhysicsEngine* physicsEngine = nullptr;

		/**
		 * @brief A pointer to the input registry. This is used to allow the user to register their own input registry if they so choose.
		 * @note This starts as a nullptr
		 * @version v0.0.8
		 */
		InputRegistry* inputRegistry = nullptr;

		/**
		 * @brief A pointer to the logger. This is used to allow the user to register their own logger if they so choose.
		 * @note This starts as a nullptr
		 * @version v0.0.8
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
		 * @version v0.0.8
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
		 * @version v0.0.8
		 */
		virtual ConfigFileRegistry* getConfigFileRegistry() override;

		/**
		* @brief Gets the scripting API.
		* @remarks This is a virtual function.
		* @note This will return a nullptr if the scripting API is not set. This is done through the @ref ATLAS_GENERATED_NULL_CHECK macro
		* @return A pointer to the scripting API
		* @version v0.0.8
		*/
		virtual ScriptingAPI* getScriptingAPI() override;
		
		/**
		* @brief Gets the window.
		* @remarks This is a virtual function.
		* @note This will return a nullptr if the window is not set. This is done through the @ref ATLAS_GENERATED_NULL_CHECK macro
		* @return A pointer to the window
		* @version v0.0.8
		* */
		virtual IWindow* getWindow() override;

		/**
		 * @brief Gets the renderer.
		 * @remarks This is a virtual function.
		 * @note This will return a nullptr if the renderer is not set. This is done through the @ref ATLAS_GENERATED_NULL_CHECK macro
		 * @return A pointer to the renderer
		 * @version v0.0.8
		 */
		virtual Renderer* getRenderer() override;
		

		virtual GameSettings* getGameSettings() override;
		

		virtual FileSystemRegistry* getFileSystemRegistry() override;
		

		virtual PhysicsEngine* getPhysicsEngine() override;
		

		virtual InputRegistry* getInputRegistry() override;

		virtual Logger* getLogger() override;

		virtual void setConfigFileRegistry(ConfigFileRegistry* configFileRegistry) override;


		virtual void setScriptingAPI(ScriptingAPI* scriptingAPI) override;


		virtual void setWindow(IWindow* window) override;


		virtual void setRenderer(Renderer* renderer) override;


		virtual void setGameSettings(GameSettings* gameSettings) override;


		virtual void setFileSystemRegistry(FileSystemRegistry* fileSystemRegistry) override;

		virtual void setPhysicsEngine(PhysicsEngine* physicsEngine) override;
				

		virtual void setInputRegistry(InputRegistry* inputRegistry) override;

		virtual void setLogger(Logger* logger) override;
	};

	class IProject {
	protected:
		virtual std::shared_ptr<AtlasEngine> setupAtlas() = 0;
	public:


		/// <summary>
		/// This is the first function that will be called on startup. 
		/// </summary>
		virtual void preInit() = 0;

		/// <summary>
		/// Initializes your project. This will be called after preInit and window initialization, meaning that this is the earliest point that you can use raylib. <b>THIS MUST BE OVERRIDEN BY YOUR PROJECT CLASS!</b>
		/// </summary>
		/// <param name="argc">Command line argument count int</param>
		/// <param name="argv">Command line arguments array</param>
		virtual void init(int argc, char* argv[]) = 0;

		/// <summary>
		/// Does a post initialization of the project. This is empty because it is not neccesary to override.
		/// </summary>
		virtual void postInit() = 0;

		/// <summary>
		/// Initializes the renderer and should be called after initWindow.
		/// </summary>
		virtual void initRenderer() = 0;

		/// <summary>
		/// Runs the actual game and is called after the init functions
		/// </summary>
		/// <param name="argc">Command line argument count int</param>
		/// <param name="argv">Command line arguments array</param>
		/// <returns>The game loop's exit code</returns>
		virtual int run(int argc, char* argv[]) = 0;

		/// <summary>
		/// Updates this instance in the updating thread.
		/// </summary>
		/// <returns></returns>
		virtual int update() = 0;

		virtual int workingUpdate() = 0;

		/// <summary>
		/// Pre-update step that is called in the physics or update thread (depending on thread count)
		/// </summary>
		/// <returns>The exit code of the step</returns>
		virtual int prePhysicsUpdate() = 0;

		/// <summary>
		/// This is where the actual physics update happens. <b>Please note that this will run on the physics thread!</b>
		/// </summary>
		/// <returns>The exit code of the step</returns>
		virtual int physicsUpdate() = 0;

		/// <summary>
		/// Post physics update that is called after physicsUpdate() in the physics or update thread (depending on thread count).
		/// </summary>
		/// <returns>The exit code of the step</returns>
		virtual int postPhysicsUpdate() = 0;

		/// <summary>
		/// Any preparation code for updating the scene's game objects. Use this if you need to do something after the physics update, but before the
		/// GameObject update. <b>Please note that this will run on the update thread!</b>
		/// </summary>
		/// <returns>The exit code of the step</returns>
		virtual int preObjectUpdate() = 0;

		/// <summary>
		/// This is where the actual GameObject update happens, and is called after preObjectUpdate(). <b>Please note that this will run on the update thread!</b>
		/// </summary>
		/// <returns>The exit code of the step</returns>
		virtual int objectUpdate() = 0;

		/// <summary>
		/// Called right after objectUpdate(). Use this to do anything after the GameObject update. <b>Please note that this will run on the update thread!</b>
		/// </summary>
		/// <returns></returns>
		virtual int postObjectUpdate() = 0;

		/// <summary>
		/// Textures all GameObjects before render() is called and after update functions are called. This is essentially where the scene is
		/// rendered. <b>Please note that this will run on the render thread!</b>
		/// </summary>
		/// <returns> An integer that is the exit code for this texturing frame.</returns>
		virtual int texture() = 0;

		/// <summary>
		/// Assuming that the FBO is enabled, this will render the FBO and apply any post-processing effects to the FBO.
		/// <b>Please note that this will run on the update thread!</b>
		/// </summary>
		/// <returns> An integer that is the exit code for this rendering frame.</returns>
		virtual int render() = 0;

		virtual int draw() = 0;

		/// <summary>
		/// Cleans up anything that may need to be cleaned up.
		/// </summary>
		/// <param name="exitCode">The code that the engine has exited with</param>
		/// <returns>The cleanup exit code.</returns>
		virtual int cleanup(int exitCode) = 0;

		virtual std::shared_ptr<AtlasEngine> getAtlasEngine() = 0;
	};

	/// <summary>
	/// 
	/// </summary>
	/// <remarks>
	/// This is an <b>abstract interface singleton</b> that all projects must implement and set the <i>ProjectReference</i> singleton to
	/// their project. In terms of functionality, this interface just holds the functions and variables that Atlas will call (besides some initialization). 
	/// \note If this boilerplate code is not done, Atlas will not know what to run. 
	/// \warning <b>Do not directly call window code in here or your project will crash!</b>
	/// </remarks>
	
	/**
	 * @brief This is the interface for the user's project. This is the only interface that Atlas will call.
	 * @remarks This is an <b>abstract interface singleton</b> that all projects must implement and set the <i>ProjectReference</i> singleton to
	 * their project. In terms of functionality, this interface just holds the functions and variables that Atlas will call (besides some initialization). 
	 * @note If this boilerplate code is not done, Atlas will not know what to run. 
	 * @warning <b>Do not directly call window code in here or your project will crash!</b>
	 * @version 0.0.1
	 */
	class BProject : public IProject {
	protected:
		bool mHasCleanedUp = false;
		Renderer mRenderer;
		WindowDecorations mWindowDecorations;
		ScriptingLibraryRegistry mLuaLibraries;
		ScriptingFunctionRegistry mLuaFunctions;
		
		/// <summary>
		/// Represents a polymorphic singleton reference to the user's project instance through a pointer of Atlas::BProject. This is
		/// mandatory for Atlas to run the user's engine.
		/// </summary>
		class ProjectReference : public Singleton<ProjectReference> {
		public:
			std::shared_ptr<BProject> mProject = nullptr;

			
			
			//ProjectReference(BProject* project) : mProject(std::make_shared<BProject>(project)) {}

			ProjectReference() {}
			/// <summary>
			/// Sets the project reference.
			/// </summary>
			/// <param name="project">The project.</param>
			void setProjectReference(BProject* project);

			std::shared_ptr<BProject> getProject() { return mProject; }
		};
		
		static inline std::shared_ptr<BProject> mProject = nullptr;

		virtual std::shared_ptr<AtlasEngine> setupAtlas() override;

		//explicit BProject(std::shared_ptr<AtlasEngine> mAtlas);
	public:
		GameSettings settings;
		static inline std::shared_ptr<AtlasEngine> mAtlas = nullptr;

		BProject();


		/// <summary>
		/// Returns the currently active libraries for lua.
		/// </summary>
		/// <returns>A reference to mLuaLibraries</returns>
		ScriptingLibraryRegistry& getLuaLibraries() { return mLuaLibraries; }

		/// <summary>
		/// Gets the lua functions.
		/// </summary>
		/// <returns>A reference to mLuaFunctions</returns>
		ScriptingFunctionRegistry& getLuaFunctions() { return mLuaFunctions; }

		/// <summary>
		/// Gets the window decorations. This includes the window title, icon, size, etc.
		/// </summary>
		/// <returns>A reference to mWindowDecorations</returns>
		WindowDecorations& getWindowDecorations();

		/// <summary>
		/// Sets the window decorations.
		/// </summary>
		/// <seealso cref="Atlas::WindowDecorations" />
		/// <param name="windowDecorations">The window decorations.</param>
		void setWindowDecorations(WindowDecorations& windowDecorations);

/*		/// <summary>
		/// Gets the renderer instance.
		/// </summary>
		/// <returns>This project's mRenderer variable</returns>
		Renderer& getRenderer() { return mRenderer; }*/

		/// <summary>
		/// This function is used to set the <i>ProjectReference</i> singleton to the project.
		/// </summary>
		/// <typeparam name="T">The typename for what the user's project is</typeparam>
		/// <param name="project">The pointer to the user's project</param>
		template<typename T>
		void setProject(T* project);

		/// <summary>
		/// This is the first function that will be called on startup. 
		/// </summary>
		/// <inheritdoc />
		virtual void preInit() override;

		/// <summary>
		/// Initializes your project. This will be called after preInit and window initialization, meaning that this is the earliest point that you can use raylib. <b>THIS MUST BE OVERRIDEN BY YOUR PROJECT CLASS!</b>
		/// </summary>
		/// <param name="argc">Command line argument count int</param>
		/// <param name="argv">Command line arguments array</param>
		/// <inheritdoc />
		virtual void init(int argc, char* argv[]) override;

		/// <summary>
		/// Does a post initialization of the project. This is empty because it is not neccesary to override.
		/// </summary>
		virtual void postInit() override;

		/// <summary>
		/// Initializes the renderer and should be called after initWindow.
		/// </summary>
		/// <inheritdoc />
		virtual void initRenderer() override;

		/// <summary>
		/// Runs the actual game and is called after the init functions
		/// </summary>
		/// <param name="argc">Command line argument count int</param>
		/// <param name="argv">Command line arguments array</param>
		/// <returns>The game loop's exit code</returns>
		/// <inheritdoc />
		virtual int run(int argc, char* argv[]) override;

		/// <summary>
		/// Updates this instance in the updating thread.
		/// </summary>
		/// <returns></returns>
		virtual int update() override;

		virtual int workingUpdate() override;

		/// <summary>
		/// Pre-update step that is called in the physics or update thread (depending on thread count)
		/// </summary>
		/// <returns>The exit code of the step</returns>
		/// <inheritdoc />
		virtual int prePhysicsUpdate() override;

		/// <summary>
		/// This is where the actual physics update happens. <b>Please note that this will run on the physics thread!</b>
		/// </summary>
		/// <returns>The exit code of the step</returns>
		/// <inheritdoc />
		virtual int physicsUpdate() override;

		/// <summary>
		/// Post physics update that is called after physicsUpdate() in the physics or update thread (depending on thread count).
		/// </summary>
		/// <returns>The exit code of the step</returns>
		/// <inheritdoc />
		virtual int postPhysicsUpdate() override;

		/// <summary>
		/// Any preparation code for updating the scene's game objects. Use this if you need to do something after the physics update, but before the
		/// GameObject update. <b>Please note that this will run on the update thread!</b>
		/// </summary>
		/// <returns>The exit code of the step</returns>
		/// <inheritdoc />
		virtual int preObjectUpdate() override;

		/// <summary>
		/// This is where the actual GameObject update happens, and is called after preObjectUpdate(). <b>Please note that this will run on the update thread!</b>
		/// </summary>
		/// <returns>The exit code of the step</returns>
		/// <inheritdoc />
		virtual int objectUpdate() override;

		/// <summary>
		/// Called right after objectUpdate(). Use this to do anything after the GameObject update. <b>Please note that this will run on the update thread!</b>
		/// </summary>
		/// <returns></returns>
		/// <inheritdoc />
		virtual int postObjectUpdate() override;

		/// <summary>
		/// Textures all GameObjects before render() is called and after update functions are called. This is essentially where the scene is
		/// rendered. <b>Please note that this will run on the render thread!</b>
		/// </summary>
		/// <returns> An integer that is the exit code for this texturing frame.</returns>
		/// <inheritdoc />
		virtual int texture() override;

		/// <summary>
		/// Assuming that the FBO is enabled, this will render the FBO and apply any post-processing effects to the FBO.
		/// <b>Please note that this will run on the update thread!</b>
		/// </summary>
		/// <returns> An integer that is the exit code for this rendering frame.</returns>
		/// <inheritdoc />
		virtual int render() override;

		virtual int draw() override;

		/// <summary>
		/// Cleans up anything that may need to be cleaned up.
		/// </summary>
		/// <param name="exitCode">The code that the engine has exited with</param>
		/// <returns>The cleanup exit code.</returns>
		/// <inheritdoc />
		virtual int cleanup(int exitCode) override;

		/**
		 * @brief Get the Project object
		 * 
		 * @return std::shared_ptr<BProject> 
		 */
		static inline std::shared_ptr<BProject> GetProject();

		/**
		 * @brief Get the Atlas Engine object
		 * 
		 * @return std::shared_ptr<AtlasEngine> 
		 */
		std::shared_ptr<AtlasEngine> getAtlasEngine() override;

	};


	std::shared_ptr<AtlasEngine> GetAtlasEngine();
}
