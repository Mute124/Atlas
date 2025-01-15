#pragma once
#include "../GameObject.h"

namespace Atlas {
	class PlaneObject : public GameObject {
	public:

		PlaneObject() : GameObject()
		{

		}

		void render() override {
			DrawCube(Vector3{0.0f, -5.0f, 0.0f}, 1.0f, 1.0f, 1.0f, WHITE);
			//DrawPlane(Vector3{0.0f, -10.0f, 0.0f}, Vector2{100.0f, 100.0f}, WHITE);
		}
	};
}