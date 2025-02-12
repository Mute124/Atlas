#pragma once
#include <memory>

#include "PreUpdateEventArgs.h"
#include "UpdateEventArgs.h"
#include "PostUpdateEventArgs.h"

#include "RenderEventArgs.h"
#include "TextureEventArgs.h"

#include "CleanupEventArgs.h"
#include "DestroyEventArgs.h"

#include <string>

namespace Atlas {
	using ComponentName = std::string;

	class IGameObject;

	class ObjectHolder {
	public:
		IGameObject* obj;

		ObjectHolder(IGameObject* obj) : obj(obj) {}
	};

	/// <summary>
	/// A component is used to extend a system, but is mainly used to extend a game object with additional functionality (<see cref="IGameObject"/>).
	/// </summary>
	class Component {
	private:

		/**
		 * @brief The game object (as a pointer) that this component is attached to
		 * @since v0.0.7
		 */
		IGameObject* mOwner;

		friend IGameObject;
	protected:
		
		/**
		 * @brief Sets the owner of this component. 
		 * @remarks This is a virtual function. 
		 * @param owner The new owner of this component. This is an @ref IGameObject pointer. This works with polymorphism
		 * @return Whether or not the owner was set
		 * @todo Make the parameter a shared pointer
		 * @todo Add exceptions and add checks
		 * @since v0.0.7
		 */
		virtual bool setOwner(IGameObject* owner);

	public:


		/**
		 * @brief An explicit constructor that initializes a new instance of the @ref Component class
		 * @param mOwner What to set the owner to
		 * @since v0.0.7
		 */
		explicit Component(IGameObject* mOwner);

		/**
		 * @brief Initializes a new instance of the <see cref="Component"/> class.
		 * @since v0.0.7
		 */
		Component() {}

		/**
		 * @brief Finalizes an instance of the <see cref="Component"/> class.
		 * @since v0.0.7
		 */
		virtual ~Component();


		/**
		 * @brief Called before the first update
		 * @param args The arguments for the event
		 * @since v0.0.7
		 */
		virtual void preUpdate(PreUpdateEventArgs* args);
				
		/**
		 * @brief Updates this instance
		 * @param args The arguments for the event
		 * @since v0.0.7
		 */
		virtual void update(UpdateEventArgs* args);
		
		/**
		 * @brief Called right after the last update.
		 * @param args The arguments for the event
		 * @since v0.0.7
		 */
		virtual void postUpdate(PostUpdateEventArgs* args);
		
		/**
		 * @brief Called after the render texture is drawn. This is the last function to be called in a frame
		 * @param args The arguments for the event
		 * @since v0.0.7
		 */
		virtual void render(RenderEventArgs* args);
				
		/// <summary>
		/// Called after updates, but before rendering. 
		/// \note This will be textured onto the render texture.
		/// </summary>
		 
		
		/**
		 * @brief Called after updates, but before rendering.
		 * @note This will be textured onto the render texture.
		 * @param args 
		 */
		virtual void texture(TextureEventArgs* args);

		/// <summary>
		/// Destroys this instance.
		/// \note This is called when the owner is destroyed
		/// </summary>
		virtual void destroy(DestroyEventArgs* args);

		/// <summary>
		/// Cleanups this instance. 
		/// \note This is called when the owner is destroyed and after the destroy function has been called.
		/// </summary>
		virtual void cleanup(CleanupEventArgs* args);
		
		/// <summary>
		/// Gets the current owner <b>AS A SHARED POINTER</b>.
		/// </summary>
		/// <returns>A shared pointer to the current owner of this component, or nullptr if no owner is set.</returns>
		std::shared_ptr<ObjectHolder> getOwner();
	};
}