#pragma once
#include <string>
#include <optional>
#include "EInputType.h"
#include "EInputTrigger.h"

namespace Atlas {

	/**
	 * @brief Serves as a search key/input category for the input system. 
	 * @remarks InputAction contains a name (std::string), an key (int), a device type (@ref EInputType) and a trigger (@ref EInputTrigger)
	 * that will be used to distinguish between different InputActions. Not only will it act as a key, but it will also act as a category. 
	 * @warning Please pay attention when using this because any slight change in InputAction keys will give you entirely different input results.
	 * @since v0.0.6
	 * @sa InputRegistry
	 */
	class InputAction {
	public:
		/**
		 * @brief The name of the InputAction.
		 * @remarks This is used to search for InputActions.
		 * @since v0.0.6
		 */
		std::string name;

		/**
		 * @brief The key associated with the InputAction.
		 * @remarks This is the physical key or button that triggers the InputAction.
		 * @since v0.0.6
		 */
		int key;

		/**
		 * @brief The controller ID associated with the InputAction (if applicable).
		 * @remarks This is the ID of the controller that the InputAction is associated with.
		 * @since v0.0.6
		 */
		std::optional<int> controllerID;

		/**
		 * @brief The device type associated with the InputAction.
		 * @remarks This is the type of device that the InputAction is associated with (e.g. keyboard, mouse, controller).
		 * @since v0.0.6
		 */
		EInputType type;

		/**
		 * @brief The trigger type associated with the InputAction.
		 * @remarks This is the type of trigger that the InputAction is associated with (e.g. press, release, hold).
		 * @since v0.0.6
		 */
		EInputTrigger trigger;

		/**
		 * @brief Default constructor for InputAction.
		 * @remarks This constructor initializes all member variables to their default values.
		 * @since v0.0.6
		 */
		InputAction() = default;

		/**
		 * @brief Constructor for InputAction.
		 * @param name The name of the InputAction.
		 * @param key The key associated with the InputAction.
		 * @param type The device type associated with the InputAction.
		 * @param trigger The trigger type associated with the InputAction.
		 * @since v0.0.6
		 */
		InputAction(const std::string& name, int key, const EInputType& type, const EInputTrigger& trigger);
	};
}