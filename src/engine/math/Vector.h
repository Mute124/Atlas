#pragma once
#include <type_traits>

struct Vector3;

namespace Atlas {
	// These are exactly the same as Raylib's Vector3 and Vector2, due to library incompatibility, it has to be seperated. Make sure to 
	// use memcpy() to copy the data from one to the other! There will be little to no performance impact if you do this (500 conversions takes .6% of a ms)
	struct Vec3 {
		float x, y, z;

	};

	struct Vec2 {
		float x, y;
	};

	struct Vec4
	{
		float x, y, z, w;
	};
}