/**
* @file Project.h
* @brief This file contains the BProject interface that is required for Atlas to run the user's project. The reason for the boilerplate is because it allows the engine to take a Laissez-faire approach and allow the user to do whatever they want.
* with all their code being able to be called from the engine.
* @since v0.0.1
*/
#pragma once

#include "Core/AtlasEngine.h"
#include "core/IProject.h"
#include <memory>
#include "GameSettings.h"
#include "modding/ScriptingAPI.h"
#include "renderer/Renderer.h"
#include "renderer/window/WindowDecorations.h"
#include "utils/Singleton.h"

namespace Atlas {
	/**
	 * @brief This is the interface for the user's project. This is the only interface that Atlas will call.
	 * @remarks This is an <b>abstract interface singleton</b> that all projects must implement and set the <i>ProjectReference</i> singleton to
	 * their project. In terms of functionality, this interface just holds the functions and variables that Atlas will call (besides some initialization). 
	 * @note If this boilerplate code is not done, Atlas will not know what to run. 
	 * @warning <b>Do not directly call window code in here or your project will crash!</b>
	 * @since Added in v0.0.1
	 */
	class BProject : public IProject {
	protected:
		bool mHasCleanedUp = false;
		Renderer mRenderer;
		WindowDecorations mWindowDecorations;
		ScriptingLibraryRegistry mLuaLibraries;
		ScriptingFunctionRegistry mLuaFunctions;
		
		/**
		 * @brief Represents a polymorphic singleton reference to the user's project instance 
		 * through a pointer of Atlas::BProject. This is mandatory for Atlas to run the user's engine.
		 * @deprecated Use Atlas::BProject::getProject()
		 * @since v0.0.1
		 */
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
