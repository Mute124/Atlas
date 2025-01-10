#pragma once
#include "IGameObject.h"

namespace Atlas {
	/// <summary>
	/// A game object that can be rendered to the screen. Inherits from <see cref="IGameObject" />, and already provides the basic rendering functionality.
	/// </summary>
	class GameObject : public IGameObject {
	public:
		// This is being used to avoid a redundant constructor
		using IGameObject::IGameObject;

		/// <summary>
		/// This is a pure virtual function that is the drawing function for when objects are being textured to the render texture. If you do not have anything different to do here, then you may
		/// just call <see cref="render()" /> instead, but this must be implemented in order for the object to be rendered.
		/// </summary>
		virtual void texture() {}

		/// <summary>
		/// Renders this instance. This is not a pure virtual and is empty because it is up to the user to use this as they see fit without forcing them to implement it.
		/// </summary>
		virtual void render() override {}

		/// <summary>
		/// Destroys this instance, and is called right before the object is removed from memory. This must be implemented by the user.
		/// </summary>
		/// <inheritdoc />
		virtual void destroy() override {
			UnloadModel(this->model);
		}

		/// <summary>
		/// Cleanups this instance.
		/// \note This must be implemented by the user.
		/// </summary>
		/// <inheritdoc />
		virtual void cleanup() {};
	};
}