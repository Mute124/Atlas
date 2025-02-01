#pragma once
#include <string>
#include <optional>
#include "EInputType.h"
#include "EInputTrigger.h"

namespace Atlas {

	/**
	 * @brief Serves as a search key/input category for the input system. 
	 * @remarks InputAction contains a name (std::string), an key (int), a device type (<see cref="EInputType"/>) and a trigger (<see cref="EInputTrigger"/>)
	 * that will be used to distinguish between different InputActions. Not only will it act as a key, but it will also act as a category. 
	 * @warning Please pay attention when using this because any slight change in InputAction keys will give you entirely different input results.
	 * @since v0.0.6
	 */
	class InputAction {
	public:
		std::string name;
		int key;
		std::optional<int> controllerID;
		EInputType type;
		EInputTrigger trigger;

		InputAction() = default;

		InputAction(const std::string& name, int key, const EInputType& type, const EInputTrigger& trigger);
	};
}