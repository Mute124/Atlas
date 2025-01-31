#pragma once
#include <raylib.h>
#include <raymath.h>

namespace Atlas {
	/// <summary>
	/// 
	/// </summary>
	class Transform {
	private:
		Vector3 mPosition = Vector3Zero();
		Vector3 mRotationAxis = Vector3Zero();
		Vector3 mScale = Vector3Zero();
		float mRotationAngle = 0.0f;

	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="Transform"/> class.
		/// </summary>
		/// <param name="position">The position.</param>
		/// <param name="rotationAxis">The rotation axis.</param>
		/// <param name="scale">The scale.</param>
		/// <param name="rotationAngle">The rotation angle.</param>
		Transform(const Vector3& position, const Vector3& rotationAxis, const Vector3& scale, float rotationAngle);
		
		/// <summary>
		/// Initializes a new instance of the <see cref="Transform"/> class.
		/// </summary>
		Transform() = default;

		/// <summary>
		/// Gets the position.
		/// </summary>
		/// <returns></returns>
		Vector3 getPosition() const;
		
		/// <summary>
		/// Gets the rotation axis.
		/// </summary>
		/// <returns></returns>
		Vector3 getRotationAxis() const;
		
		/// <summary>
		/// Gets the scale.
		/// </summary>
		/// <returns></returns>
		Vector3 getScale() const;

		/// <summary>
		/// Gets the rotation angle.
		/// </summary>
		/// <returns></returns>
		float getRotationAngle() const;

		/// <summary>
		/// Sets the position.
		/// </summary>
		/// <param name="position">The position.</param>
		void setPosition(const Vector3& position);

		/// <summary>
		/// Sets the rotation axis.
		/// </summary>
		/// <param name="rotationAxis">The rotation axis.</param>
		void setRotationAxis(const Vector3& rotationAxis);

		/// <summary>
		/// Sets the scale.
		/// </summary>
		/// <param name="scale">The scale.</param>
		void setScale(const Vector3& scale);

		/// <summary>
		/// Sets the rotation angle.
		/// </summary>
		/// <param name="rotationAngle">The rotation angle.</param>
		void setRotationAngle(float rotationAngle);

		/// <summary>
		/// Translates the specified translation.
		/// </summary>
		/// <param name="translation">The translation.</param>
		/// <returns></returns>
		/// <summary>
		/// Translates the specified translation.
		/// </summary>
		/// <param name="translation">The translation.</param>
		/// <returns></returns>
		Transform& translate(const Vector3& translation) { mPosition += translation; return *this; }

		/// <summary>
		/// Rotates the specified axis.
		/// </summary>
		/// <param name="axis">The axis.</param>
		/// <param name="angle">The angle.</param>
		/// <returns></returns>
		/// <summary>
		/// Rotates the specified axis.
		/// </summary>
		/// <param name="axis">The axis.</param>
		/// <param name="angle">The angle.</param>
		/// <returns></returns>
		Transform& rotate(const Vector3& axis, float angle) { mRotationAxis += axis; mRotationAngle += angle; return *this; }

		/// <summary>
		/// Scales the specified scale.
		/// </summary>
		/// <param name="scale">The scale.</param>
		/// <returns></returns>
		/// <summary>
		/// Scales the specified scale.
		/// </summary>
		/// <param name="scale">The scale.</param>
		/// <returns></returns>
		Transform& scale(const Vector3& scale) { mScale += scale; return *this; }

		/// <summary>
		/// Operator=s the specified other.
		/// </summary>
		/// <param name="other">The other.</param>
		/// <returns></returns>
		Transform& operator=(const Transform& other) = default;

		/// <summary>
		/// Operator==s the specified other.
		/// </summary>
		/// <param name="other">The other.</param>
		/// <returns></returns>
		bool operator==(const Transform& other) const;
	};
}