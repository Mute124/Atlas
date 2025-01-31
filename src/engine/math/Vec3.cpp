#include "Vec3.h"
#include <format>

Atlas::Vec3::Vec3(float x, float y, float z) : mX(x), mY(y), mZ(z) {}

Atlas::Vec3::Vec3(Vector3 const& v) : mX(v.x), mY(v.y), mZ(v.z) {}

Atlas::Vec3::Vec3(JPH::RVec3 const& v) : mX(v.GetX()), mY(v.GetY()), mZ(v.GetZ()) {}

Atlas::Vec3::operator Vector3() const { 
	return { mX, mY, mZ }; 
}

Atlas::Vec3::operator JPH::RVec3() const {
	return JPH::RVec3(mX, mY, mZ);
}

Atlas::Vec3::operator std::string() const {
	return std::format("({}, {}, {})", mX, mY, mZ);
}

float Atlas::Vec3::operator[](int index) const { 
	return (&mX)[index]; 
}

bool Atlas::Vec3::operator==(const Vec3& other) const
{
	return mX == other.mX && mY == other.mY && mZ == other.mZ;
}

Atlas::Vec3& Atlas::Vec3::operator+=(const Vec3& other) {
	mX += other.mX;
	mY += other.mY;
	mZ += other.mZ;
	return *this;
}

Atlas::Vec3& Atlas::Vec3::operator-=(const Vec3& other) {
	mX -= other.mX;
	mY -= other.mY;
	mZ -= other.mZ;
	return *this;
}

Atlas::Vec3& Atlas::Vec3::operator*=(const float scalar) {
	mX *= scalar;
	mY *= scalar;
	mZ *= scalar;
	return *this;
}

Atlas::Vec3& Atlas::Vec3::operator/=(const float scalar) {
	mX /= scalar;
	mY /= scalar;
	mZ /= scalar;
	return *this;
}

Atlas::Vec3 Atlas::Vec3::operator+(const Vec3& scalar) const {
	return Vec3(mX + scalar.mX, mY + scalar.mY, mZ + scalar.mZ);
}

Atlas::Vec3 Atlas::Vec3::operator-(const Vec3& scalar) const {
	return Vec3(mX - scalar.mX, mY - scalar.mY, mZ - scalar.mZ);
}

Atlas::Vec3 Atlas::Vec3::operator*(const float scalar) const {
	return Vec3(mX * scalar, mY * scalar, mZ * scalar);
}

Atlas::Vec3 Atlas::Vec3::operator/(const float scalar) const {
	return Vec3(mX / scalar, mY / scalar, mZ / scalar);
}

Atlas::Vec3& Atlas::Vec3::operator=(Vec3 const& other) {
	mX = other.mX;
	mY = other.mY;
	mZ = other.mZ;
	return *this;
}

Atlas::Vec3& Atlas::Vec3::operator=(JPH::RVec3 const& other) {
	mX = other.GetX();
	mY = other.GetY();
	mZ = other.GetZ();
	return *this;
}

Atlas::Vec3& Atlas::Vec3::operator=(Vector3 const& other) {
	mX = other.x;
	mY = other.y;
	mZ = other.z;
	return *this;
}

Atlas::Vec3 Atlas::Vec3::Zero() {
	return Vec3(0, 0, 0);
}

Atlas::Vec3 Atlas::Vec3::One() {
	return Vec3(1, 1, 1);
}

Atlas::Vec3 Atlas::Vec3::perpendicular() {
	return static_cast<Vec3>(Vector3Perpendicular(operator Vector3()));
}

Atlas::Vec3 Atlas::Vec3::perpendicular(const Vec3& other) {
	return static_cast<Vec3>(Vector3Perpendicular(other.operator Vector3()));
}

Atlas::Vec3 Atlas::Vec3::max(const Vec3& other) {
	/*
	* This is being left here for future reference
	* return Vec3(mX > other.mX ? mX : other.mX, mY > other.mY ? mY : other.mY, mZ > other.mZ ? mZ : other.mZ);
	*
	*/

	float maxX = (mX > other.mX) ? mX : other.mX; // if mX is greater than other.mX, return mX, else return other.mX
	float maxY = (mY > other.mY) ? mY : other.mY; // if mY is greater than other.mY, return mY, else return other.mY
	float maxZ = (mZ > other.mZ) ? mZ : other.mZ; // if mZ is greater than other.mZ, return mZ, else return other.mZ

	return Vec3(maxX, maxY, maxZ);
}

Atlas::Vec3 Atlas::Vec3::min(const Vec3& other) {
	/*
	* This is being left here for future reference
	* return Vec3(mX < other.mX ? mX : other.mX, mY < other.mY ? mY : other.mY, mZ < other.mZ ? mZ : other.mZ);
	*/

	float minX = (mX < other.mX) ? mX : other.mX; // if mX is less than other.mX, return mX, else return other.mX
	float minY = (mY < other.mY) ? mY : other.mY; // if mY is less than other.mY, return mY, else return other.mY
	float minZ = (mZ < other.mZ) ? mZ : other.mZ; // if mZ is less than other.mZ, return mZ, else return other.mZ

	return Vec3(minX, minY, minZ);
}

Atlas::Vec3 Atlas::Vec3::project(const Vec3& other) {

	return static_cast<Vec3>(
		Vector3Project(
			operator Vector3(),
			other.operator Vector3()
		)
	);

}

 Atlas::Vec3 Atlas::Vec3::unproject(const Matrix& projection, const Matrix& view) {
	return static_cast<Vec3>(
		Vector3Unproject(
			operator Vector3(),
			projection,
			view
		)
	);
}

 Atlas::Vec3 Atlas::Vec3::reject(const Vec3& other) {
	return static_cast<Vec3>(
		Vector3Reject(
			operator Vector3(),
			other.operator Vector3()
		)
	);
}

 Atlas::Vec3 Atlas::Vec3::reflect(const Vec3& normal) {
	/*	 
		Remove at some point:

		Vector3 v1 = operator Vector3();
		Vector3 normalVector = normal.operator Vector3();

		Vector3 result = Vector3Reflect(v1, normalVector);

		return static_cast<Vec3>(result);
	 */

	 return static_cast<Vec3>(Vector3Reflect(operator Vector3(), normal.operator Vector3()));
}

Atlas::Vec3 Atlas::Vec3::refract(const Vec3& normal, float ratio) {
	return static_cast<Vec3>(
		Vector3Refract(
			operator Vector3(),
			normal.operator Vector3(),
			ratio
		)
	);
}

Atlas::Vec3 Atlas::Vec3::lerp(const Vec3& other, float t) {
	return static_cast<Vec3>(
		Vector3Lerp(
			operator Vector3(),
			other.operator Vector3(),
			t
		
		)
	);
}

Atlas::Vec3 Atlas::Vec3::cubicHermite(const Vec3& v1, const Vec3& tangent1, const Vec3& v2, const Vec3& tangent2, float amount) {
	return static_cast<Vec3>(
		Vector3CubicHermite(
			v1.operator Vector3(),
			tangent1.operator Vector3(),
			v2.operator Vector3(),
			tangent2.operator Vector3(),
			amount
		
		)
	);
}

Atlas::Vec3 Atlas::Vec3::baryCenter(const Vec3& p, const Vec3& a, const Vec3& b, const Vec3& c) {
	return static_cast<Vec3>(
		Vector3Barycenter(
			p.operator Vector3(),
			a.operator Vector3(),
			b.operator Vector3(),
			c.operator Vector3()
		)
	);
}

float Atlas::Vec3::dot(const Vec3& other) {
	return Vector3DotProduct(operator Vector3(), other.operator Vector3());
}

float Atlas::Vec3::distance(const Vec3& other) {
	
	return Vector3Distance(operator Vector3(), other.operator Vector3());
}

float Atlas::Vec3::length() {
	return Vector3Length(operator Vector3());
}

float Atlas::Vec3::lengthSquared() {
	return Vector3LengthSqr(operator Vector3());
}

float Atlas::Vec3::angle(const Vec3& other) {
	return Vector3Angle(operator Vector3(), other.operator Vector3());
}

Atlas::Vec3& Atlas::Vec3::cross(const Vec3& other) {
	*this = static_cast<Vec3>(Vector3CrossProduct(operator Vector3(), other.operator Vector3()));

	return *this;
}

Atlas::Vec3& Atlas::Vec3::negate() {
	mX = -mX;
	mY = -mY;
	mZ = -mZ;
	return *this;
}

Atlas::Vec3& Atlas::Vec3::normalize() {
	float length = Vector3Length(operator Vector3());
	mX = mX / length;
	mY = mY / length;
	mZ = mZ / length;
	return *this;
}

Atlas::Vec3& Atlas::Vec3::transform(Matrix const& matrix) {
	*this = Vector3Transform(operator Vector3(), matrix);
	return *this;
}

Atlas::Vec3& Atlas::Vec3::rotateByQuaternion(Quaternion const& quaternion) {
	*this = Vector3RotateByQuaternion(operator Vector3(), quaternion);
	return *this;
}

 Atlas::Vec3& Atlas::Vec3::rotateByAxisAngle(const Vec3& axis, float angle) {
	*this = Vector3RotateByAxisAngle(operator Vector3(), axis.operator Vector3(), angle);
	return *this;
}

Atlas::Vec3& Atlas::Vec3::clamp(const Vec3& min, const Vec3& max) {
	*this = Vector3Clamp(operator Vector3(), min.operator Vector3(), max.operator Vector3());
	return *this;
}

Atlas::Vec3& Atlas::Vec3::clamp(float min, float max) {
	*this = Vector3ClampValue(operator Vector3(), min, max);
	return *this;
}

Atlas::Vec3& Atlas::Vec3::normalizedMoveTowards(const Vec3& target, float distance) {
	Vec3 direction = target - *this;
	direction.normalize();
	*this += direction * distance;
	return *this;
}

Atlas::Vec3& Atlas::Vec3::moveTowards(const Vec3& target, float distance) {
	*this = Vector3MoveTowards(operator Vector3(), target.operator Vector3(), distance);
	return *this;
}

Atlas::Vec3& Atlas::Vec3::invert() {
	*this = Vector3Invert(operator Vector3());
	return *this;
}

float Atlas::Vec3::getX() const { 
	return mX; 
}

float Atlas::Vec3::getY() const { 
	return mY; 
}

float Atlas::Vec3::getZ() const { 
	return mZ; 
}

void Atlas::Vec3::setX(float x) { 
	mX = x;
}

void Atlas::Vec3::setY(float y) { 
	mY = y; 
}

void Atlas::Vec3::setZ(float z) {
	mZ = z;
}
