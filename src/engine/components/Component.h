#pragma once

#include <memory>

#include "PreUpdateEventArgs.h"
#include "UpdateEventArgs.h"
#include "PostUpdateEventArgs.h"

#include "RenderEventArgs.h"
#include "TextureEventArgs.h"

#include "CleanupEventArgs.h"
#include "DestroyEventArgs.h"

#include <type_traits>

namespace Atlas {
	class IGameObject;

	
	/// <summary>
	/// A component is used to extend a system, but is mainly used to extend a game object with additional functionality (<see cref="IGameObject"/>).
	/// </summary>
	class Component {
	private:
		std::shared_ptr<IGameObject> mOwner;

		friend IGameObject;
	protected:
		
		/// <summary>
		/// Sets the owner of this component.
		/// </summary>
		/// <param name="owner">The new owner of this component.</param>
		/// <returns>Whether or not the owner was set.</returns>
		virtual bool setOwner(std::shared_ptr<IGameObject> owner) {
			mOwner = owner;

			return mOwner != nullptr && owner != nullptr;
		}

	public:
				
		/// <summary>
		/// Initializes a new instance of the <see cref="Component"/> class.
		/// </summary>
		/// <param name="mOwner">The m owner.</param>
		explicit Component(const std::shared_ptr<IGameObject>& mOwner);

		/// <summary>
		/// Initializes a new instance of the <see cref="Component"/> class.
		/// </summary>
		Component() {}

		/// <summary>
		/// Finalizes an instance of the <see cref="Component"/> class.
		/// </summary>
		virtual ~Component();

		/// <summary>
		/// Called before the first update
		/// </summary>
		virtual void preUpdate(PreUpdateEventArgs* args);
				
		/// <summary>
		/// The main update frame
		/// </summary>
		virtual void update(UpdateEventArgs* args);
		
		/// <summary>
		/// Called right after the last update.
		/// </summary>
		virtual void postUpdate(PostUpdateEventArgs* args);
			
		/// <summary>
		/// Called after the render texture is drawn. This is the last function to be called in a frame
		/// </summary>
		virtual void render(RenderEventArgs* args);
				
		/// <summary>
		/// Called after updates, but before rendering. 
		/// \note This will be textured onto the render texture.
		/// </summary>
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
		
/*		/// <summary>
		/// Gets a component from the owner <b>AS A UNIQUE POINTER</b>.
		/// </summary>
		/// <returns></returns>
		template<T_COMPONENT T>
		std::unique_ptr<T> getComponent() {
			return getOwner()->getComponent<T>();
		}
		*/

		/// <summary>
		/// Gets the current owner <b>AS A SHARED POINTER</b>.
		/// </summary>
		/// <returns>A shared pointer to the current owner of this component, or nullptr if no owner is set.</returns>
		std::shared_ptr<IGameObject> getOwner();
	};
}