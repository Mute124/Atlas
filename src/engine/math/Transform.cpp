#include "Transform.h"

Atlas::Transform::Transform(const Vector3& position, const Vector3& rotationAxis, const Vector3& scale, float rotationAngle)
	: mPosition(position), mRotationAxis(rotationAxis), mScale(scale), mRotationAngle(rotationAngle)
{
}

Vector3 Atlas::Transform::getPosition() const { return mPosition; }

Vector3 Atlas::Transform::getRotationAxis() const { return mRotationAxis; }

Vector3 Atlas::Transform::getScale() const { return mScale; }

float Atlas::Transform::getRotationAngle() const { return mRotationAngle; }

/// <summary>
/// Sets the position.
/// </summary>
/// <param name="position">The position.</param>
void Atlas::Transform::setPosition(const Vector3& position) {}

/// <summary>
/// Sets the rotation axis.
/// </summary>
/// <param name="rotationAxis">The rotation axis.</param>
void Atlas::Transform::setRotationAxis(const Vector3& rotationAxis) {}

/// <summary>
/// Sets the scale.
/// </summary>
/// <param name="scale">The scale.</param>
void Atlas::Transform::setScale(const Vector3& scale) {}

/// <summary>
/// Sets the rotation angle.
/// </summary>
/// <param name="rotationAngle">The rotation angle.</param>
void Atlas::Transform::setRotationAngle(float rotationAngle) {}

/// <summary>
/// Operator==s the specified other.
/// </summary>
/// <param name="other">The other.</param>
/// <returns></returns>
bool Atlas::Transform::operator==(const Transform& other) const
{
	return mPosition == other.mPosition && mRotationAxis == other.mRotationAxis && mScale == other.mScale && mRotationAngle == other.mRotationAngle;
}


