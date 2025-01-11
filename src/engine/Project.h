/**
* \file Project.h
* 
* \brief This file contains the IProject interface that is required for Atlas to run the user's project. The reason for the boilerplate is because it allows the engine to take a Laissez-faire approach and allow the user to do whatever they want.
* with all their code being able to be called from the engine.
*/
#pragma once
#include "renderer/Renderer.h"
#include "renderer/Window.h"
#include "renderer/WindowDecorations.h"
#include "utils/Singleton.h"
#include "modding/ScriptingAPI.h"
#include "GameSettings.h"
#include "fs/FileSystem.h"
#include "conf/Config.h"
#include <stdint.h>
#include "dbg/Debugger.h"
#include <bitset>

namespace Atlas {
	/// <summary>
	/// This is an <b>abstract interface singleton</b> that all projects must implement and set the <i>ProjectReference</i> singleton to
	/// their project. In terms of functionality, this interface just holds the functions and variables that Atlas will call (besides some initialization). 
	/// \note If this boilerplate code is not done, Atlas will not know what to run.
	/// \warning <b>Do not directly call window code in here or your project will crash!</b>
	/// </summary>
	class IProject abstract {
	protected:
		Renderer mRenderer;
		WindowDecorations mWindowDecorations;
		ScriptingLibraryRegistry mLuaLibraries;
		ScriptingFunctionRegistry mLuaFunctions;
		
		/// <summary>
		/// Represents a polymorphic singleton reference to the user's project instance through a pointer of Atlas::IProject. This is
		/// mandatory for Atlas to run the user's engine.
		/// </summary>
		class ProjectReference : public Singleton<ProjectReference> {
		public:
			IProject* project = nullptr;

			/// <summary>
			/// Sets the project reference.
			/// </summary>
			/// <param name="project">The project.</param>
			void setProjectReference(IProject* project);
		};

	public:
		GameSettings settings;

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

		/// <summary>
		/// Gets the renderer instance.
		/// </summary>
		/// <returns>This project's mRenderer variable</returns>
		Renderer& getRenderer() { return mRenderer; }

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
		virtual void preInit() {
			Logger::Instance().init(LoggerConfig());
			//std::cout << "IProject::preInit" << std::endl;
			Log("Test");
			AddFileRegistryLoadFunction("png", [](std::shared_ptr<FileMeta> loadFunc) {
				return std::any_cast<Image>(LoadImage(loadFunc->path.c_str()));
			});
			
			AddFileRegistryLoadFunction("jpg", [](std::shared_ptr<FileMeta> loadFunc) {
				return std::any_cast<Image>(LoadImage(loadFunc->path.c_str()));
			});

			AddFileRegistryLoadFunction("jpeg", [](std::shared_ptr<FileMeta> loadFunc) {
				return std::any_cast<Image>(LoadImage(loadFunc->path.c_str()));
			});

			std::string gameDir = ATLAS_GAME_DIR;
			InitializeFileRegistry(gameDir.c_str());

			this->mLuaLibraries.push_back(sol::lib::base);

		}

		/// <summary>
		/// Initializes your project. This will be called after preInit and window initialization, meaning that this is the earliest point that you can use raylib. <b>THIS MUST BE OVERRIDEN BY YOUR PROJECT CLASS!</b>
		/// </summary>
		/// <param name="argc">Command line argument count int</param>
		/// <param name="argv">Command line arguments array</param>
		/// <inheritdoc />
		virtual void init(int argc, char* argv[]);

		/// <summary>
		/// Does a post initialization of the project. This is empty because it is not neccesary to override.
		/// </summary>
		virtual void postInit();

		/// <summary>
		/// Initializes the renderer and should be called after initWindow.
		/// </summary>
		/// <inheritdoc />
		virtual void initRenderer();

		/// <summary>
		/// Runs the actual game and is called after the init functions
		/// </summary>
		/// <param name="argc">Command line argument count int</param>
		/// <param name="argv">Command line arguments array</param>
		/// <returns>The game loop's exit code</returns>
		/// <inheritdoc />
		virtual int run(int argc, char* argv[]);

		virtual int update();

		/// <summary>
		/// Pre-update step that is called in the physics or update thread (depending on thread count)
		/// </summary>
		/// <returns>The exit code of the step</returns>
		/// <inheritdoc />
		virtual int prePhysicsUpdate();

		/// <summary>
		/// This is where the actual physics update happens. <b>Please note that this will run on the physics thread!</b>
		/// </summary>
		/// <returns>The exit code of the step</returns>
		/// <inheritdoc />
		virtual int physicsUpdate();

		/// <summary>
		/// Post physics update that is called after physicsUpdate() in the physics or update thread (depending on thread count).
		/// </summary>
		/// <returns>The exit code of the step</returns>
		/// <inheritdoc />
		virtual int postPhysicsUpdate();

		/// <summary>
		/// Any preparation code for updating the scene's game objects. Use this if you need to do something after the physics update, but before the
		/// GameObject update. <b>Please note that this will run on the update thread!</b>
		/// </summary>
		/// <returns>The exit code of the step</returns>
		/// <inheritdoc />
		virtual int preObjectUpdate();

		/// <summary>
		/// This is where the actual GameObject update happens, and is called after preObjectUpdate(). <b>Please note that this will run on the update thread!</b>
		/// </summary>
		/// <returns>The exit code of the step</returns>
		/// <inheritdoc />
		virtual int objectUpdate();

		/// <summary>
		/// Called right after objectUpdate(). Use this to do anything after the GameObject update. <b>Please note that this will run on the update thread!</b>
		/// </summary>
		/// <returns></returns>
		/// <inheritdoc />
		virtual int postObjectUpdate();

		/// <summary>
		/// Textures all GameObjects before render() is called and after update functions are called. This is essentially where the scene is
		/// rendered. <b>Please note that this will run on the render thread!</b>
		/// </summary>
		/// <returns> An integer that is the exit code for this texturing frame.</returns>
		/// <inheritdoc />
		virtual int texture();

		/// <summary>
		/// Assuming that the FBO is enabled, this will render the FBO and apply any post-processing effects to the FBO.
		/// <b>Please note that this will run on the update thread!</b>
		/// </summary>
		/// <returns> An integer that is the exit code for this rendering frame.</returns>
		/// <inheritdoc />
		virtual int render();

		/// <summary>
		/// Cleans up anything that may need to be cleaned up.
		/// </summary>
		/// <param name="exitCode">The code that the engine has exited with</param>
		/// <returns>The cleanup exit code.</returns>
		/// <inheritdoc />
		virtual int cleanup(int exitCode);

		/// <summary>
		/// Gets the current project instance.
		/// </summary>
		/// <returns>An IProject pointer of the current project reference instance</returns>
		static IProject* GetProject();
	};

	/// <summary>
	/// This function is used to set the <i>ProjectReference</i> singleton to the project.
	/// </summary>
	/// <typeparam name="T">The typename for what the user's project is</typeparam>
	/// <param name="project">The pointer to the user's project</param>

	template<typename T>
	inline void IProject::setProject(T* project) {
		ProjectReference::Instance().setProjectReference(project);
	}
}
