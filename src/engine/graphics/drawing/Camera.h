#pragma once

#include <glm/glm.hpp>

namespace Atlas {
	class Camera {
	private:
		float mFOVY{ 70.0f };
		float mAspectRatio{ 16.0f / 9.0f };
		float mNearPlane{ 1.1f };
		float mFarPlane{ 10000.0f };

		glm::vec3 mPosition{ 0.0f, 0.0f, 0.0f };

		glm::mat4 mViewMatrix{ glm::mat4(1.0f) };
		glm::mat4 mProjectionMatrix{ glm::mat4(1.0f) };
	public:

		void update() {
			mViewmatrix = glm::translate
		}
	};
}
