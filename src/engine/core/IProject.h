#pragma once
#include <memory>
#include "AtlasEngine.h"
#include "IAtlasEngine.h"

#include <type_traits>

namespace Atlas {
	
	/**
	 * @brief Represents an atlas project interface. This concept ensures that the type is a child class of @ref IAtlasEngine
	 * @note Not used yet
	 * @since v0.0.9
	 */
	template<class T_ATLAS_ENGINE_TYPE>
	concept AtlasProject = requires(T_ATLAS_ENGINE_TYPE t) {
		{ t } -> std::convertible_to<IAtlasEngine>;
	};

	class IProject {
	protected:
		virtual ThreadSafeVariable<AtlasEngine*> setupAtlas() = 0;
	public:
		/**
		 * @brief This is the first function that will be called on startup. 
		 * @since v0.0.1
		 */
		virtual void preInit() = 0;

		/**
		 * @brief Initializes your project.
		 * 
		 * @remarks This will be called after preInit and window initialization, meaning that this is the earliest point that you can use raylib.
		 * 
		 * @warning <b>THIS MUST BE OVERRIDEN BY YOUR PROJECT CLASS!</b>
		 * 
		 * @param argc 
		 * 
		 * @param argv 
		 */
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


		virtual ThreadSafeVariable<AtlasEngine*> getAtlasEngine() = 0;
	};
}