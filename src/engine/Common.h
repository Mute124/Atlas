/// \file Common.h
/// 
/// \brief Common includes and definitions that are used throughout the engine. There are going to be very limited amounts of actual engine includes, so this is not a replacement for that. 
/// Instead, this is a way to centralize the includes and definitions that are used throughout the engine.
/// 
/// \todo Add more haptic features
/// 
/// \remarks This file has a few configurations that are based on defines. TS_ENABLE_HAPTICS is used to enable/disable haptic features (ie. controller rumble), whereas TS_ENABLE_EXTENSIONS is used
/// to enable/disable extensions.
#pragma once

#include <iostream>
#include <cstdarg>
#include <cstdint>
#include <string>
#include <thread>

#include "utils/Singleton.h"
#include "input/Input.h"

/// <summary>
/// Since the location of the shared libraries and executables can vary, this is a simple solution to this. During Distribution builds, the path is more exact, however the rest are not because these
/// will be in the build directory rather than the final distribution directory.
/// </summary>
const std::string TS_ASSET_DIR = std::string(TS_TOP_LAYER) + "/game/assets/";
const std::string TS_DATA_DIR = std::string(TS_TOP_LAYER) + "/game/data/";
const std::string TS_GAME_DIR = std::string(TS_TOP_LAYER) + "/game/";
const std::string TS_TEMP_DIR = std::string(TS_TOP_LAYER) + "/temp/";

namespace Techstorm {

	using std::string;
	using std::cout;
	using std::endl;
	using std::thread;
	using std::this_thread::sleep_for;
	using std::chrono::milliseconds;
	using std::chrono::seconds;
	using std::unordered_map;

	/*/// <summary>
	/// Represents a 3D vector. It will wrap raylib's Vector3 and Jolt's RVec3 to allow seamless integration. This also wraps raylib's vector math functions.
	/// </summary>
	struct Vec3 {
		float x, y, z;

		Vec3(float x, float y, float z) : x(x), y(y), z(z) {}
		explicit Vec3(Vector3 const& v) : x(v.x), y(v.y), z(v.z) {}
		explicit Vec3(JPH::RVec3 const& v) : x(v.GetX()), y(v.GetY()), z(v.GetZ()) {}

		operator Vector3() const { return { x, y, z }; }
		operator JPH::RVec3() const { return { x, y, z }; }
		
		/// <summary>
		/// Operator[]s the specified index. This corrosponds to x, y, and z.
		/// </summary>
		/// <param name="index">The index.</param>
		/// <returns>The float value at the index</returns>
		float operator[](int index) const { return (&x)[index]; }
		
		/// <summary>
		/// Operator==s the specified other.
		/// </summary>
		/// <param name="other">The other.</param>
		/// <returns>Whether the vectors are equal</returns>
		bool operator==(const Vec3& other) const
		{
			return x == other.x && y == other.y && z == other.z;
		}

		// math operators
		
		/// <summary>
		/// Operator+=s the specified other.
		/// </summary>
		/// <param name="other">The other.</param>
		/// <returns>The resulting vector</returns>
		Vec3& operator+=(const Vec3& other) {
			x += other.x;
			y += other.y;
			z += other.z;
			return *this; 
		}
		
		/// <summary>
		/// Operator-=s the specified other.
		/// </summary>
		/// <param name="other">The other.</param>
		/// <returns>The resulting vector</returns>
		Vec3& operator-=(const Vec3& other) {
			x -= other.x;
			y -= other.y;
			z -= other.z;
			return *this;
		}

		/// <summary>
		/// Operator*=s the specified scalar.
		/// </summary>
		/// <param name="scalar">The scalar.</param>
		/// <returns>The resulting vector</returns>
		Vec3& operator*=(const float scalar) {
			x *= scalar;
			y *= scalar;
			z *= scalar;
			return *this;
		}

		/// <summary>
		/// Operator/=s the specified scalar.
		/// </summary>
		/// <param name="scalar">The scalar.</param>
		/// <returns>The resulting vector</returns>
		Vec3& operator/=(const float scalar) {
			x /= scalar;
			y /= scalar;
			z /= scalar;
			return *this;
		}
		
		/// <summary>
		/// Operator+s the specified scalar.
		/// </summary>
		/// <param name="scalar">The scalar.</param>
		/// <returns>The resulting vector</returns>
		Vec3 operator+(const Vec3& scalar) const {
			return Vec3(x + scalar.x, y + scalar.y, z + scalar.z);
		}

		/// <summary>
		/// Operator-s the specified scalar.
		/// </summary>
		/// <param name="scalar">The scalar.</param>
		/// <returns>The resulting vector</returns>
		Vec3 operator-(const Vec3& scalar) const {
			return Vec3(x - scalar.x, y - scalar.y, z - scalar.z);
		}

		/// <summary>
		/// Operator*s the specified scalar.
		/// </summary>
		/// <param name="scalar">The scalar.</param>
		/// <returns>The resulting vector</returns>
		Vec3 operator*(const float scalar) const {
			return Vec3(x * scalar, y * scalar, z * scalar);
		}

		/// <summary>
		/// Operator/s the specified scalar.
		/// </summary>
		/// <param name="scalar">The scalar.</param>
		/// <returns>The resulting vector</returns>
		Vec3 operator/(const float scalar) const {
			return Vec3(x / scalar, y / scalar, z / scalar);
		}
				
		/// <summary>
		/// Operator=s the specified other.
		/// </summary>
		/// <param name="other">The other vector.</param>
		/// <returns>The resulting vector</returns>
		Vec3& operator=(Vec3 const& other) {
			x = other.x;
			y = other.y;
			z = other.z;
			return *this; 
		} 

		/// <summary>
		/// Operator=s the specified other.
		/// </summary>
		/// <param name="other">The other vector.</param>
		/// <returns>The resulting vector</returns>
		Vec3& operator=(JPH::RVec3 const& other) {
			x = other.x;
			y = other.y;
			z = other.z;
			return *this;
		}

		/// <summary>
		/// Operator=s the specified other.
		/// </summary>
		/// <param name="other">The other vector.</param>
		/// <returns>The resulting vector</returns>
		Vec3& operator=(Vector3 const& other) {
			x = other.x;
			y = other.y;
			z = other.z;
			return *this;
		}

		// math functions

		// Non chainable
		//---------------
		
		/// <summary>
		/// Returns a vector with all values set to zero.
		/// </summary>
		/// <returns>The resulting vector</returns>
		Vec3 zero() {
			return Vec3(0, 0, 0); 
		}
		
		/// <summary>
		/// Returns a vector with all values set to one.
		/// </summary>
		/// <returns>The resulting vector</returns>
		Vec3 one() {
			return Vec3(1, 1, 1); 
		}

		/// <summary>
		/// Returns a vector perpendicular to this one.
		/// </summary>
		/// <returns>The resulting vector</returns>
		Vec3 perpendicular() {
			return (Vec3)Vector3Perpendicular(operator Vector3());
		}
		
		/// <summary>
		/// Returns a vector perpendicular to the specified one.
		/// </summary>
		/// <param name="other">The other.</param>
		/// <returns>The resulting vector</returns>
		Vec3 perpendicular(const Vec3& other) {
			return (Vec3)Vector3Perpendicular(other.operator Vector3());
		}
		
		/// <summary>
		/// Returns a vector with the largest value for each axis.
		/// </summary>
		/// <param name="other">The other.</param>
		/// <returns>The largest vector</returns>
		Vec3 max(const Vec3& other) {
			return Vec3(x > other.x ? x : other.x, y > other.y ? y : other.y, z > other.z ? z : other.z);
		}
		
		/// <summary>
		/// Returns a vector with the smallest value for each axis.
		/// </summary>
		/// <param name="other">The other.</param>
		/// <returns>The smallest vector</returns>
		Vec3 min(const Vec3& other) {
			return Vec3(x < other.x ? x : other.x, y < other.y ? y : other.y, z < other.z ? z : other.z);
		}
		
		/// <summary>
		/// Projects this vector onto the specified one.
		/// </summary>
		/// <param name="other">The other.</param>
		/// <returns>The projected vector</returns>
		Vec3 project(const Vec3& other) {
			return (Vec3)Vector3Project(operator Vector3(), other.operator Vector3());
		}

		/// <summary>
		/// Unprojects this vector onto the specified one.
		/// </summary>
		/// <param name="projection">The projection matrix.</param>
		/// <param name="view">The view matrix.</param>
		/// <returns>The unprojected vector</returns>
		Vec3 unproject(const Matrix& projection, const Matrix& view) {
			return (Vec3)Vector3Unproject(operator Vector3(), projection, view);
		}
		
		/// <summary>
		/// Rejects this vector onto the specified one.
		/// </summary>
		/// <param name="other">The other.</param>
		/// <returns>The rejected vector</returns>
		Vec3 reject(const Vec3& other) {
			return (Vec3)Vector3Reject(operator Vector3(), other.operator Vector3());
		}
		
		/// <summary>
		/// Reflects this vector onto the specified one.
		/// </summary>
		/// <param name="normal">The normal.</param>
		/// <returns></returns>
		Vec3 reflect(const Vec3& normal) {
			return (Vec3)Vector3Reflect(operator Vector3(), normal.operator Vector3());
		}

		/// <summary>
		/// Refracts this vector onto the specified one.
		/// </summary>
		/// <param name="normal">The normal.</param>
		/// <param name="ratio">The ratio.</param>
		/// <returns></returns>
		Vec3 refract(const Vec3& normal, float ratio) {
			return (Vec3)Vector3Refract(operator Vector3(), normal.operator Vector3(), ratio);
		}

		/// <summary>
		/// Linearly interpolates between this vector and the specified one.
		/// </summary>
		/// <param name="other">The other.</param>
		/// <param name="t">The t.</param>
		/// <returns></returns>
		Vec3 lerp(const Vec3& other, float t) {
			return (Vec3)Vector3Lerp(operator Vector3(), other.operator Vector3(), t);
		}

		/// <summary>
		/// Cubically interpolates between this vector and the specified one.
		/// </summary>
		/// <param name="v1">The v1.</param>
		/// <param name="tangent1">The tangent1.</param>
		/// <param name="v2">The v2.</param>
		/// <param name="tangent2">The tangent2.</param>
		/// <param name="amount">The amount.</param>
		/// <returns></returns>
		Vec3 cubicHermite(const Vec3& v1, const Vec3& tangent1, const Vec3& v2, const Vec3& tangent2, float amount) {
			return (Vec3)Vector3CubicHermite(v1.operator Vector3(), tangent1.operator Vector3(), v2.operator Vector3(), tangent2.operator Vector3(), amount);
		}

		/// <summary>
		/// Gets the barycenter between the specified points.
		/// </summary>
		/// <param name="p">The p.</param>
		/// <param name="a">The a.</param>
		/// <param name="b">The b.</param>
		/// <param name="c">The c.</param>
		/// <returns>The barycentric point between the specified points</returns>
		Vec3 baryCenter(const Vec3& p, const Vec3& a, const Vec3& b, const Vec3& c) {
			return (Vec3)Vector3Barycenter(p.operator Vector3(), a.operator Vector3(), b.operator Vector3(), c.operator Vector3());
		}
				
		/// <summary>
		/// Dots the specified other vector.
		/// </summary>
		/// <param name="other">The other.</param>
		/// <returns></returns>
		float dot(const Vec3& other) {
			return x * other.x + y * other.y + z * other.z;
		}
		
		/// <summary>
		/// Gets the distance from this vector to the specified one.
		/// </summary>
		/// <param name="other">The other.</param>
		/// <returns></returns>
		float distance(const Vec3& other) {
			return sqrtf((x - other.x) * (x - other.x) + (y - other.y) * (y - other.y) + (z - other.z) * (z - other.z));
		}

		/// <summary>
		/// Gets this vector's length.
		/// </summary>
		/// <returns></returns>
		float length() {
			return Vector3Length(operator Vector3());
		}

		/// <summary>
		/// Gets this vector's length squared.
		/// </summary>
		/// <returns></returns>
		float lengthSquared() {
			return Vector3LengthSqr(operator Vector3());
		}

		/// <summary>
		/// Gets the angle between this vector and the specified one.
		/// </summary>
		/// <param name="other">The other.</param>
		/// <returns></returns>
		float angle(const Vec3& other) {
			return Vector3Angle(operator Vector3(), other.operator Vector3());
		}

		// Chainable
		//---------------
		
		/// <summary>
		/// Cross multiplies this vector with the specified one.
		/// </summary>
		/// <param name="other">The other.</param>
		/// <returns></returns>
		Vec3& cross(const Vec3& other) {
			x = y * other.z - z * other.y;
			y = z * other.x - x * other.z;
			z = x * other.y - y * other.x;
			return *this;
		}

		/// <summary>
		/// Negates this vector.
		/// </summary>
		/// <returns></returns>
		Vec3& negate() {
			x = -x;
			y = -y;
			z = -z;
			return *this;
		}

		/// <summary>
		/// Normalizes this vector.
		/// </summary>
		/// <returns></returns>
		Vec3& normalize() {
			float length = Vector3Length(operator Vector3());
			x = x / length;
			y = y / length;
			z = z / length;
			return *this;
		}

		/// <summary>
		/// Transforms this vector by the specified matrix.
		/// </summary>
		/// <param name="matrix">The matrix.</param>
		/// <returns></returns>
		Vec3& transform(Matrix const& matrix) {
			*this = Vector3Transform(operator Vector3(), matrix);
			return *this;
		}

		/// <summary>
		/// Rotates this vector by the specified quaternion.
		/// </summary>
		/// <param name="quaternion">The quaternion.</param>
		/// <returns></returns>
		Vec3& rotateByQuaternion(Quaternion const& quaternion) {
			*this = Vector3RotateByQuaternion(operator Vector3(), quaternion);
			return *this;
		}

		/// <summary>
		/// Rotates this vector by the specified axis and angle.
		/// </summary>
		/// <param name="axis">The axis.</param>
		/// <param name="angle">The angle.</param>
		/// <returns></returns>
		Vec3& rotateByAxisAngle(const Vec3& axis, float angle) {
			*this = Vector3RotateByAxisAngle(operator Vector3(), axis.operator Vector3(), angle);
			return *this;
		}

		/// <summary>
		/// Clamps this vector between the specified min and max values.
		/// </summary>
		/// <param name="min">The min.</param>
		/// <param name="max">The max.</param>
		/// <returns></returns>
		Vec3& clamp(const Vec3& min, const Vec3& max) {
			*this = Vector3Clamp(operator Vector3(), min, max);
			return *this;
		}

		/// <summary>
		/// Clamps this vector between the specified min and max values.
		/// </summary>
		/// <param name="min">The min.</param>
		/// <param name="max">The max.</param>
		/// <returns></returns>
		Vec3& clamp(float min, float max) {
			*this = Vector3ClampValue(operator Vector3(), min, max);
		}

		/// <summary>
		/// Moves this vector towards the specified target by normalizing the direction.
		/// </summary>
		/// <param name="target">The target.</param>
		/// <param name="distance">The distance.</param>
		/// <returns></returns>
		Vec3& normalizedMoveTowards(const Vec3& target, float distance) {
			Vec3 direction = target - *this;
			direction.normalize();
			*this += direction * distance;
			return *this;
		}

		/// <summary>
		/// Moves this vector towards the specified target.
		/// </summary>
		/// <param name="target">The target.</param>
		/// <param name="distance">The distance.</param>
		/// <returns></returns>
		Vec3& moveTowards(const Vec3& target, float distance) {
			*this = Vector3MoveTowards(operator Vector3(), target.operator Vector3(), distance);
		}
		
		/// <summary>
		/// Inverts this vector.
		/// </summary>
		/// <returns></returns>
		Vec3& invert() {
			*this = Vector3Invert(operator Vector3());
			return *this;
		}
	};*/


	template<typename T>
	class ReferenceHolder {
	public:

		explicit ReferenceHolder(T& value) : mValue(value) {}

		void set(T& value) {
			mValue = value;
		}

		T& get() {
			return mValue;
		}


	private:
		T& mValue;
	};


	/// This is a module for haptic experiences (ie. controller rumble) and helps make more immersive experiences. 
	/// \todo Add more haptic features, like monitor flashing and such.
#ifdef TS_ENABLE_HAPTICS

	/// <summary>
	/// Initializes the haptics module. Keep in mind this only works if TS_ENABLE_HAPTICS is defined.
	/// </summary>
	void InitHaptics();
	
	/// <summary>
	/// Sets the controller rumble. Keep in mind this only works if TS_ENABLE_HAPTICS is defined.
	/// </summary>
	void SetControllerRumble(int controllerIndex, int leftMotorSpeed, int rightMotorSpeed);
	
	/// <summary>
	/// Starts the controller rumble. Keep in mind this only works if TS_ENABLE_HAPTICS is defined.
	/// </summary>
	void StartRumblingController(int controllerIndex, int leftMotorSpeed, int rightMotorSpeed);
	
	void StopRumblingController(int controllerIndex);
#endif

	/// \brief This #ifdef is to enable/disable extensions. Currently this is not implemented, but is planned for the future.
	/// \todo Implement extensions
#ifdef TS_ENABLE_EXTENSIONS
	// Not yet implemented
	#ifdef TS_ENABLE_DISCORD_INTEGRATION
		#ifndef TS_SILENCE_DISCORD_INTEGRATION_WARNING
		#error "Discord integration is not yet implemented. To silence this warning, define TS_SILENCE_DISCORD_INTEGRATION_WARNING."
		#endif
	#endif
#endif
}
