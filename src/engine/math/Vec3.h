#pragma once
#include <raymath.h>
#include <Jolt/Jolt.h>
#include <Jolt/Math/Vec3.h>
#include <cmath>
#include <Jolt/Math/Real.h>

#include <string>

namespace Atlas {

	/// <summary>
	/// Represents a 3D vector. It will wrap raylib's Vector3 and Jolt's RVec3 to allow seamless integration. This also wraps raylib's vector math functions. 
	/// This class contains many functions because it wraps around Raymath's functions and Jolt's functions. Furthermore, there is alot of math that can be done with vectors, so 
	/// this class tries to wrap as much as possible and make it easier to work with. The reason that this class exists despite raylib's Vector3 and Jolt's RVec3 is because there may
	/// be future plans to add different renderers.
	/// </summary>
	class Vec3 {
	private:
		float mX;
		float mY;
		float mZ;
	public:

		/// <summary>
		/// Initializes a new instance of the <see cref="Vec3"/> class.
		/// </summary>
		Vec3();
		
		/// <summary>
		/// Initializes a new instance of the <see cref="Vec3"/> class.
		/// </summary>
		/// <param name="x">The x.</param>
		/// <param name="y">The y.</param>
		/// <param name="z">The z.</param>
		Vec3(float x, float y, float z);

		/// <summary>
		/// Initializes a new instance of the <see cref="Vec3"/> class.
		/// </summary>
		/// <param name="v">The v.</param>
		explicit Vec3(Vector3 const& v);
		
		/// <summary>
		/// Initializes a new instance of the <see cref="Vec3"/> class.
		/// </summary>
		/// <param name="v">The v.</param>
		explicit Vec3(JPH::RVec3 const& v);
		
		/// <summary>
		/// Implements the operator Vector3.
		/// </summary>
		/// <returns>
		/// The result of the operator.
		/// </returns>
		explicit operator Vector3() const;
		
		/// <summary>
		/// Implements the operator JPH::RVec3.
		/// </summary>
		/// <returns>
		/// The result of the operator.
		/// </returns>
		explicit operator JPH::RVec3() const;
		
		/// <summary>
		/// Implements the operator std::string. It converts this current vector to a string.
		/// </summary>
		/// <returns>
		/// This vector as a string formatted as <c>"x, y, z</c>
		/// </returns>
		explicit operator std::string() const;

		// ------------------------------------------------------
		// Logical operators
		// ------------------------------------------------------

		/// <summary>
		/// Operator[]s the specified index. This corrosponds to x, y, and z. x = 0, y = 1, and z = 2.
		/// </summary>
		/// <param name="index">The index.</param>
		/// <returns>The float value at the index</returns>
		float operator[](int index) const;

		/// <summary>
		/// Operator==s the specified other.
		/// </summary>
		/// <param name="other">The other.</param>
		/// <returns>Whether the vectors are equal</returns>
		bool operator==(const Vec3& other) const;

		// ------------------------------------------------------
		// Math operators
		// ------------------------------------------------------

		/// <summary>
		/// Operator+=s the specified other.
		/// </summary>
		/// <param name="other">The other.</param>
		/// <returns>The resulting vector</returns>
		Vec3& operator+=(const Vec3& other);

		/// <summary>
		/// Operator-=s the specified other.
		/// </summary>
		/// <param name="other">The other.</param>
		/// <returns>The resulting vector</returns>
		Vec3& operator-=(const Vec3& other);

		/// <summary>
		/// Operator*=s the specified scalar.
		/// </summary>
		/// <param name="scalar">The scalar.</param>
		/// <returns>The resulting vector</returns>
		Vec3& operator*=(const float scalar);

		/// <summary>
		/// Operator/=s the specified scalar.
		/// </summary>
		/// <param name="scalar">The scalar.</param>
		/// <returns>The resulting vector</returns>
		Vec3& operator/=(const float scalar);

		/// <summary>
		/// Operator+s the specified scalar.
		/// </summary>
		/// <param name="scalar">The scalar.</param>
		/// <returns>The resulting vector</returns>
		Vec3 operator+(const Vec3& scalar) const;

		/// <summary>
		/// Operator-s the specified scalar.
		/// </summary>
		/// <param name="scalar">The scalar.</param>
		/// <returns>The resulting vector</returns>
		Vec3 operator-(const Vec3& scalar) const;

		/// <summary>
		/// Operator*s the specified scalar.
		/// </summary>
		/// <param name="scalar">The scalar.</param>
		/// <returns>The resulting vector</returns>
		Vec3 operator*(const float scalar) const;

		/// <summary>
		/// Operator/s the specified scalar.
		/// </summary>
		/// <param name="scalar">The scalar.</param>
		/// <returns>The resulting vector</returns>
		Vec3 operator/(const float scalar) const;

		/// <summary>
		/// Operator=s the specified other.
		/// </summary>
		/// <param name="other">The other vector.</param>
		/// <returns>The resulting vector</returns>
		Vec3& operator=(Vec3 const& other);

		/// <summary>
		/// Operator=s the specified other.
		/// </summary>
		/// <param name="other">The other vector.</param>
		/// <returns>The resulting vector</returns>
		Vec3& operator=(JPH::RVec3 const& other);

		/// <summary>
		/// Operator=s the specified other.
		/// </summary>
		/// <param name="other">The other vector.</param>
		/// <returns>The resulting vector</returns>
		Vec3& operator=(Vector3 const& other);

		// math functions

		// Non chainable
		//---------------

		/// <summary>
		/// Returns a vector with all values set to zero.
		/// </summary>
		/// <returns>The resulting vector</returns>
		static inline Vec3 Zero();

		/// <summary>
		/// Returns a vector with all values set to one.
		/// </summary>
		/// <returns>The resulting vector</returns>
		static inline Vec3 One();

		/// <summary>
		/// Returns a vector perpendicular to this one.
		/// </summary>
		/// <returns>The resulting vector</returns>
		Vec3 perpendicular();

		/// <summary>
		/// Returns a vector perpendicular to the specified one.
		/// </summary>
		/// <param name="other">The other.</param>
		/// <returns>The resulting vector</returns>
		Vec3 perpendicular(const Vec3& other);

		/// <summary>
		/// Returns a vector with the largest value for each axis.
		/// </summary>
		/// <param name="other">The other.</param>
		/// <returns>The largest vector</returns>
		Vec3 max(const Vec3& other);

		/// <summary>
		/// Returns a vector with the smallest value for each axis.
		/// </summary>
		/// <param name="other">The other.</param>
		/// <returns>The smallest vector</returns>
		Vec3 min(const Vec3& other);

		/// <summary>
		/// Projects this vector onto the specified one.
		/// </summary>
		/// <param name="other">The other.</param>
		/// <returns>The projected vector</returns>
		Vec3 project(const Vec3& other);

		/// <summary>
		/// Unprojects this vector onto the specified one.
		/// </summary>
		/// <param name="projection">The projection matrix.</param>
		/// <param name="view">The view matrix.</param>
		/// <returns>The unprojected vector</returns>
		Vec3 unproject(const Matrix& projection, const Matrix& view);

		/// <summary>
		/// Rejects this vector onto the specified one.
		/// </summary>
		/// <param name="other">The other.</param>
		/// <returns>The rejected vector</returns>
		Vec3 reject(const Vec3& other);

		/// <summary>
		/// Reflects this vector onto the specified one.
		/// </summary>
		/// <param name="normal">The normal.</param>
		/// <returns></returns>
		Vec3 reflect(const Vec3& normal);

		/// <summary>
		/// Refracts this vector onto the specified one.
		/// </summary>
		/// <param name="normal">The normal.</param>
		/// <param name="ratio">The ratio.</param>
		/// <returns></returns>
		Vec3 refract(const Vec3& normal, float ratio);

		/// <summary>
		/// Linearly interpolates between this vector and the specified one.
		/// </summary>
		/// <param name="other">The other.</param>
		/// <param name="t">The t.</param>
		/// <returns></returns>
		Vec3 lerp(const Vec3& other, float t);

		/// <summary>
		/// Cubically interpolates between this vector and the specified one.
		/// </summary>
		/// <param name="v1">The v1.</param>
		/// <param name="tangent1">The tangent1.</param>
		/// <param name="v2">The v2.</param>
		/// <param name="tangent2">The tangent2.</param>
		/// <param name="amount">The amount.</param>
		/// <returns></returns>
		Vec3 cubicHermite(const Vec3& v1, const Vec3& tangent1, const Vec3& v2, const Vec3& tangent2, float amount);

		/// <summary>
		/// Gets the barycenter between the specified points.
		/// </summary>
		/// <param name="p">The p.</param>
		/// <param name="a">The a.</param>
		/// <param name="b">The b.</param>
		/// <param name="c">The c.</param>
		/// <returns>The barycentric point between the specified points</returns>
		Vec3 baryCenter(const Vec3& p, const Vec3& a, const Vec3& b, const Vec3& c);

		/// <summary>
		/// Dots the specified other vector.
		/// </summary>
		/// <param name="other">The other.</param>
		/// <returns></returns>
		float dot(const Vec3& other);

		/// <summary>
		/// Gets the distance from this vector to the specified one.
		/// </summary>
		/// <param name="other">The other.</param>
		/// <returns></returns>
		float distance(const Vec3& other);

		/// <summary>
		/// Gets this vector's length.
		/// </summary>
		/// <returns></returns>
		float length();

		/// <summary>
		/// Gets this vector's length squared.
		/// </summary>
		/// <returns></returns>
		float lengthSquared();

		/// <summary>
		/// Gets the angle between this vector and the specified one.
		/// </summary>
		/// <param name="other">The other.</param>
		/// <returns></returns>
		float angle(const Vec3& other);

		// Chainable
		//---------------

		/// <summary>
		/// Cross multiplies this vector with the specified one.
		/// </summary>
		/// <param name="other">The other.</param>
		/// <returns></returns>
		Vec3& cross(const Vec3& other);

		/// <summary>
		/// Negates this vector.
		/// </summary>
		/// <returns></returns>
		Vec3& negate();

		/// <summary>
		/// Normalizes this vector.
		/// </summary>
		/// <returns></returns>
		Vec3& normalize();

		/// <summary>
		/// Transforms this vector by the specified matrix.
		/// </summary>
		/// <param name="matrix">The matrix.</param>
		/// <returns></returns>
		Vec3& transform(Matrix const& matrix);

		/// <summary>
		/// Rotates this vector by the specified quaternion.
		/// </summary>
		/// <param name="quaternion">The quaternion.</param>
		/// <returns></returns>
		Vec3& rotateByQuaternion(Quaternion const& quaternion);

		/// <summary>
		/// Rotates this vector by the specified axis and angle.
		/// </summary>
		/// <param name="axis">The axis.</param>
		/// <param name="angle">The angle.</param>
		/// <returns></returns>
		Vec3& rotateByAxisAngle(const Vec3& axis, float angle);

		/// <summary>
		/// Clamps this vector between the specified min and max values.
		/// </summary>
		/// <param name="min">The min.</param>
		/// <param name="max">The max.</param>
		/// <returns></returns>
		Vec3& clamp(const Vec3& min, const Vec3& max);

		/// <summary>
		/// Clamps this vector between the specified min and max values.
		/// </summary>
		/// <param name="min">The min.</param>
		/// <param name="max">The max.</param>
		/// <returns></returns>
		Vec3& clamp(float min, float max);

		/// <summary>
		/// Moves this vector towards the specified target by normalizing the direction.
		/// </summary>
		/// <param name="target">The target.</param>
		/// <param name="distance">The distance.</param>
		/// <returns></returns>
		Vec3& normalizedMoveTowards(const Vec3& target, float distance);

		/// <summary>
		/// Moves this vector towards the specified target.
		/// </summary>
		/// <param name="target">The target.</param>
		/// <param name="distance">The distance.</param>
		/// <returns></returns>
		Vec3& moveTowards(const Vec3& target, float distance);

		/// <summary>
		/// Inverts this vector.
		/// </summary>
		/// <returns>An inverted vector</returns>
		Vec3& invert();
		
		/// <summary>
		/// Gets the x value.
		/// </summary>
		/// <returns>The current x value</returns>
		float getX() const;
		
		/// <summary>
		/// Gets the y value.
		/// </summary>
		/// <returns>The current y value</returns>
		float getY() const;

		/// <summary>
		/// Gets the z value.
		/// </summary>
		/// <returns>The current z value</returns>
		float getZ() const;
		
		/// <summary>
		/// Sets the x.
		/// </summary>
		/// <param name="x">The x.</param>
		void setX(float x);
		
		/// <summary>
		/// Sets the y.
		/// </summary>
		/// <param name="y">The y.</param>
		void setY(float y);
				
		/// <summary>
		/// Sets the z.
		/// </summary>
		/// <param name="z">The z.</param>
		void setZ(float z);
	};

}