#pragma once
#include "Component.h"
#include <raylib.h>


namespace Atlas {	
	/// <summary>
	/// A transform component represents an object's transform. This includes rotation, position, etc
	/// </summary>
	/// <seealso cref="Component" />
	class TransformComponent final : public Component {
	private:

	public:
		Matrix mMatrix;
		Transform mTransform;

		Vector3 pos = { 0.0f, 0.0f, 0.0f };

		explicit TransformComponent(IGameObject* mOwner) : Component(mOwner) {}

		/// <summary>
		/// Initializes a new instance of the <see cref="TransformComponent"/> class.
		/// </summary>
		TransformComponent();
		
		/// <summary>
		/// Finalizes an instance of the <see cref="TransformComponent"/> class.
		/// </summary>
		~TransformComponent() final {}
		
		/// <summary>
		/// Translates the specified translation.
		/// </summary>
		/// <param name="translation">The translation.</param>
		void translate(Vector3 const& translation);
		
		/// <summary>
		/// Translates the specified x.
		/// </summary>
		/// <param name="x">The x.</param>
		/// <param name="y">The y.</param>
		/// <param name="z">The z.</param>
		void translate(float x, float y, float z);

		void rotate(Quaternion const& delta) { }

		TransformComponent& operator=(TransformComponent& comp) {
			this->mTransform = comp.mTransform;
			this->mMatrix = comp.mMatrix;
			this->pos = comp.pos;

			return *this;
		}
	};
}