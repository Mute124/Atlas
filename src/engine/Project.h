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

#include "fs/FileSystem.h"
#include "conf/Config.h"
#include <stdint.h>
#include "dbg/Debugger.h"
#include <bitset>
#include "renderer/GameModel.h"
#include "physics/Physics.h"

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

		virtual void setConfigFileRegistry(ConfigFileRegistry* configFileRegistry) = 0;
		virtual void setScriptingAPI(ScriptingAPI* scriptingAPI) = 0;
		virtual void setWindow(IWindow* window) = 0;
		virtual void setRenderer(Renderer* renderer) = 0;
		virtual void setGameSettings(GameSettings* gameSettings) = 0;
		virtual void setFileSystemRegistry(FileSystemRegistry* fileSystemRegistry) = 0;
		virtual void setPhysicsEngine(PhysicsEngine* physicsEngine) = 0;
	};

	class AtlasEngine : public IAtlas {
	private:		
		/// <summary>
		/// The configuration file registry
		/// </summary>
		ConfigFileRegistry* configFileRegistry = nullptr;
		
		/// <summary>
		/// The scripting API
		/// </summary>
		ScriptingAPI* scriptingAPI = nullptr;
		
		/// <summary>
		/// The window
		/// </summary>
		IWindow* window = nullptr;
		
		/// <summary>
		/// The renderer
		/// </summary>
		Renderer* renderer = nullptr;
		
		/// <summary>
		/// The game settings
		/// </summary>
		GameSettings* gameSettings = nullptr;
		
		/// <summary>
		/// The file system registry
		/// </summary>
		FileSystemRegistry* fileSystemRegistry = nullptr;
		
		/// <summary>
		/// The physics engine
		/// </summary>
		PhysicsEngine* physicsEngine = nullptr;
	public:
		
		/// <summary>
		/// Initializes a new instance of the <see cref="AtlasEngine"/> class.
		/// </summary>
		/// <param name="configFileRegistry">The configuration file registry.</param>
		/// <param name="scriptingAPI">The scripting API.</param>
		/// <param name="window">The window.</param>
		/// <param name="renderer">The renderer.</param>
		/// <param name="gameSettings">The game settings.</param>
		/// <param name="fileSystemRegistry">The file system registry.</param>
		/// <param name="physicsEngine">The physics engine.</param>
		AtlasEngine(ConfigFileRegistry* configFileRegistry, ScriptingAPI* scriptingAPI, IWindow* window, Renderer* renderer,
					GameSettings* gameSettings, FileSystemRegistry* fileSystemRegistry, PhysicsEngine* physicsEngine);
				
		/// <summary>
		/// Initializes a new instance of the <see cref="AtlasEngine"/> class.
		/// </summary>
		AtlasEngine() = default;
		
		/// <summary>
		/// Gets the configuration file registry.
		/// </summary>
		/// <returns></returns>
		virtual ConfigFileRegistry* getConfigFileRegistry() override;
		
		/// <summary>
		/// Gets the scripting API.
		/// </summary>
		/// <returns></returns>
		virtual ScriptingAPI* getScriptingAPI() override;
		
		/// <summary>
		/// Gets the window.
		/// </summary>
		/// <returns></returns>
		virtual IWindow* getWindow() override;
		
		/// <summary>
		/// Gets the renderer.
		/// </summary>
		/// <returns></returns>
		virtual Renderer* getRenderer() override;
		
		/// <summary>
		/// Gets the game settings.
		/// </summary>
		/// <returns></returns>
		virtual GameSettings* getGameSettings() override;
		
		/// <summary>
		/// Gets the file system registry.
		/// </summary>
		/// <returns></returns>
		virtual FileSystemRegistry* getFileSystemRegistry() override;
		
		/// <summary>
		/// Gets the physics engine.
		/// </summary>
		/// <returns></returns>
		virtual PhysicsEngine* getPhysicsEngine() override;

		/// <summary>
		/// Sets the configuration file registry.
		/// </summary>
		/// <param name="configFileRegistry">The configuration file registry.</param>
		virtual void setConfigFileRegistry(ConfigFileRegistry* configFileRegistry) override;

		/// <summary>
		/// Sets the scripting API.
		/// </summary>
		/// <param name="scriptingAPI">The scripting API.</param>
		virtual void setScriptingAPI(ScriptingAPI* scriptingAPI) override;

		/// <summary>
		/// Sets the window.
		/// </summary>
		/// <param name="window">The window.</param>
		virtual void setWindow(IWindow* window) override;

		/// <summary>
		/// Sets the renderer.
		/// </summary>
		/// <param name="renderer">The renderer.</param>
		virtual void setRenderer(Renderer* renderer) override;

		/// <summary>
		/// Sets the game settings.
		/// </summary>
		/// <param name="gameSettings">The game settings.</param>
		virtual void setGameSettings(GameSettings* gameSettings) override;


		/// <summary>
		/// Sets the file system registry.
		/// </summary>
		/// <param name="fileSystemRegistry">The file system registry.</param>
		virtual void setFileSystemRegistry(FileSystemRegistry* fileSystemRegistry) override;


		/// <summary>
		/// Sets the physics engine.
		/// </summary>
		/// <param name="physicsEngine">The physics engine.</param>
		virtual void setPhysicsEngine(PhysicsEngine* physicsEngine) override;
		
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
	/// This is the interface for the user's project. This is the only interface that Atlas will call.
	/// </summary>
	/// <remarks>
	/// This is an <b>abstract interface singleton</b> that all projects must implement and set the <i>ProjectReference</i> singleton to
	/// their project. In terms of functionality, this interface just holds the functions and variables that Atlas will call (besides some initialization). 
	/// \note If this boilerplate code is not done, Atlas will not know what to run. 
	/// \warning <b>Do not directly call window code in here or your project will crash!</b>
	/// </remarks>
	class BProject : public IProject {
	protected:
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
		static inline std::shared_ptr<AtlasEngine> mAtlas;

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

		/// <summary>
		/// Gets the current project instance.
		/// </summary>
		/// <returns>An BProject pointer of the current project reference instance</returns>
		static inline std::shared_ptr<BProject> GetProject();

		std::shared_ptr<AtlasEngine> getAtlasEngine() override;

	};

	/// <summary>
	/// This function is used to set the <i>ProjectReference</i> singleton to the project.
	/// </summary>
	/// <typeparam name="T">The typename for what the user's project is</typeparam>
	/// <param name="project">The pointer to the user's project</param>



	std::shared_ptr<AtlasEngine> GetAtlasEngine();
}
