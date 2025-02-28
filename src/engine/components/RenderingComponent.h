#pragma once
#include "../objects/IGameObject.h"

#include "Component.h"
#include <raylib.h>
#include <string>

namespace Atlas {
	class RenderingComponent : public Component {
	private:
	public:
		using Component::Component;

		RenderingComponent() = default;
	};
}