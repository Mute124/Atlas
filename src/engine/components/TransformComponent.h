#pragma once
#include "../Component.h"
#include <raylib.h>
#include <raymath.h>

namespace Atlas {	
	/// <summary>
	/// A transform component represents an object's transform. This includes rotation, position, etc
	/// </summary>
	/// <seealso cref="Component" />
	class TransformComponent final : public Component {
	private:
		Matrix mMatrix; 
		Transform mTransform;
	public:
	
		/// <summary>
		/// Initializes a new instance of the <see cref="TransformComponent"/> class.
		/// </summary>
		TransformComponent();
		
		/// <summary>
		/// Finalizes an instance of the <see cref="TransformComponent"/> class.
		/// </summary>
		~TransformComponent();
		
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


	};
}