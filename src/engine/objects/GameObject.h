#pragma once
#include "IGameObject.h"


namespace Atlas {
	/**
	 * @brief A game object that can be rendered to the screen. This already provides the basic rendering functionality.
	 * @since v0.0.1
	 * @sa @ref IGameObject
	 */
	class GameObject : public IGameObject {
	public:
		GameObject();

		/// <summary>
		/// Pres the update.
		/// </summary>
		virtual void preUpdate() override;

		/// <summary>
		/// Updates this instance.
		/// </summary>
		virtual void update() override;

		/// <summary>
		/// Posts the update.
		/// </summary>
		virtual void postUpdate() override;

		/// <summary>
		/// This is a pure virtual function that is the drawing function for when objects are being textured to the render texture. If you do not have anything different to do here, then you may
		/// just call <see cref="render()" /> instead, but this must be implemented in order for the object to be rendered.
		/// </summary>
		virtual void texture() override;

		/// <summary>
		/// Renders this instance. This is not a pure virtual and is empty because it is up to the user to use this as they see fit without forcing them to implement it.
		/// </summary>
		virtual void render() override;

		/// <summary>
		/// Destroys this instance, and is called right before the object is removed from memory. 
		/// \note This must be implemented by the user.
		/// </summary>
		/// <inheritdoc />
		virtual void destroy() override;

		virtual void cleanup() override;
	};
}