#pragma once
#include <string>
#include <functional>
#include <optional>

#include "InputKey.h"

namespace Atlas {
	enum class EInputType {
		Button,
		Axis
	};

	enum class EAxisDirection {
		X,
		Y
	};

	struct InputEvent {
		EInputSource source;
		InputKey key;
		EInputType type;
	};

	using ButtonEvent = InputEvent;

	struct AxisEvent : public InputEvent {
		EAxisDirection direction;
		float value;
		float delta;
	};

	/*	struct InputBinding {
		std::string name;
		EInputSource source;
	};*/

	struct InputAction {
		std::string name;
		//EInputType triggerType;
		InputKey key;
		EInputSource source;

		std::function<void(const InputEvent&)> callback;
	};
};
