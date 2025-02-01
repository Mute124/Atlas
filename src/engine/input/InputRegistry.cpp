#include "InputRegistry.h"

#include <stdexcept>
#include <raylib.h>
#include "EInputTrigger.h"
#include "EInputType.h"
#include "InputAction.h"
#include "SelfDeterminingInputAction.h"
#include <functional>
#include <unordered_map>

bool Atlas::InputRegistry::checkPressedAction(InputAction* action)
{
	if (action == nullptr) {
		throw std::invalid_argument("action cannot be null");
	}

	int gamepad = 0;
	switch (action->type)
	{
		using enum Atlas::EInputType;
	case INPUT_KEYBOARD:
		return IsKeyPressed(action->key);
		break;
	case INPUT_MOUSE:
		return IsMouseButtonPressed(action->key);
		break;
	case INPUT_CONTROLLER:
		gamepad = action->controllerID.value_or(0);

		if (IsGamepadAvailable(gamepad))
		{
			return IsGamepadButtonPressed(gamepad, action->key);
		}
		else {
			//throw std::runtime_error("Gamepad " + std::to_string(gamepad) + " is not available or the id has not been set.");
			return false;
		}

		break;
	default:
		return false;
		break;
	}
}

bool Atlas::InputRegistry::checkReleasedAction(InputAction* action)
{
	if (action == nullptr) {
		throw std::invalid_argument("action cannot be null");
	}

	int gamepad = 0;
	switch (action->type)
	{
		using enum Atlas::EInputType;
	case INPUT_KEYBOARD:
		return IsKeyPressed(action->key);
		break;
	case INPUT_MOUSE:
		return IsMouseButtonPressed(action->key);
		break;
	case INPUT_CONTROLLER:
		gamepad = action->controllerID.value_or(0);

		if (IsGamepadAvailable(gamepad))
		{
			return IsGamepadButtonPressed(gamepad, action->key);
		}
		else {
			//throw std::runtime_error("Gamepad " + std::to_string(gamepad) + " is not available or the id has not been set.");
			return false;
		}

		break;
	default:
		return false;
		break;
	}
}

bool Atlas::InputRegistry::checkDownAction(InputAction* action)
{
	if (action == nullptr) {
		throw std::invalid_argument("action cannot be null");
	}

	int gamepad = 0;
	switch (action->type)
	{
		using enum Atlas::EInputType;
	case INPUT_KEYBOARD:
		return IsKeyDown(action->key);
		break;
	case INPUT_MOUSE:
		return IsMouseButtonDown(action->key);
		break;
	case INPUT_CONTROLLER:
		gamepad = action->controllerID.value_or(0);

		if (IsGamepadAvailable(gamepad))
		{
			return IsGamepadButtonDown(gamepad, action->key);
		}
		else {
			//throw std::runtime_error("Gamepad " + std::to_string(gamepad) + " is not available or the id has not been set.");
			return false;
		}

		break;
	default:
		return false;
		break;
	}
}

bool Atlas::InputRegistry::checkUpAction(InputAction* action)
{
	if (action == nullptr) {
		throw std::invalid_argument("action cannot be null");
	}

	int gamepad = 0;
	switch (action->type)
	{
		using enum Atlas::EInputType;
	case INPUT_KEYBOARD:
		return IsKeyUp(action->key);
		break;
	case INPUT_MOUSE:
		return IsMouseButtonUp(action->key);
		break;
	case INPUT_CONTROLLER:
		gamepad = action->controllerID.value_or(0);

		if (IsGamepadAvailable(gamepad))
		{
			return IsGamepadButtonUp(gamepad, action->key);
		}
		else {
			//throw std::runtime_error("Gamepad " + std::to_string(gamepad) + " is not available or the id has not been set.");
			return false;
		}

		break;
	default:
		return false;
		break;
	}
}

bool Atlas::InputRegistry::checkRepeatAction(InputAction* action)
{
	if (action == nullptr) {
		throw std::invalid_argument("action cannot be null");
	}

	int gamepad = 0;
	switch (action->type)
	{
		using enum Atlas::EInputType;
	case INPUT_KEYBOARD:
		return IsKeyPressedRepeat(action->key);
		break;
	case INPUT_MOUSE:
		// TODO: Add mouse repeat support
		return false;
		break;
	case INPUT_CONTROLLER:
		// TODO: Add controller repeat support
		return false;
		break;
	default:
		return false;
		break;
	}
}

Atlas::InputActionCallbackID Atlas::InputRegistry::registerActionCallback(InputAction* action, std::function<void(InputAction*)> const& callback) {
	if (action == nullptr) {
		throw std::invalid_argument("InputAction cannot be null");
	}
	// Since callbacks are stored in an unordered_map, the next available id/slot is the map's size + 1.
	int id = mActions[action].size() + 1;

	mActions[action].try_emplace(id, callback);
	return id;
}
Atlas::InputActionCallbackID Atlas::InputRegistry::registerSelfDeterminingActionCallback(InputAction* category, SelfDeterminingInputAction* action, std::function<bool(SelfDeterminingInputAction*)> const& checker, std::function<void(SelfDeterminingInputAction*)> const& callback)
{
	/*	if (action == nullptr) {
	throw std::invalid_argument("SelfDeterminingInputAction cannot be null");
	}

	if (category == nullptr) {
	throw std::invalid_argument("InputAction cannot be null");
	}

	if (!mSelfDeterminingActions.contains(category)) {
	mSelfDeterminingActions.try_emplace(category);
	}


	// Since callbacks are stored in an unordered_map, the next available id/slot is the map's size + 1.
	int id = mSelfDeterminingActions[category].size() + 1;

	mSelfDeterminingActions[action].try_emplace(id, action);
	return id;*/
	if (!action || !category) {
		throw std::invalid_argument("SelfDeterminingInputAction or InputAction cannot be null");
	}

	if (!mSelfDeterminingActions.contains(category)) {
		mSelfDeterminingActions.try_emplace(category);
	}

	int id = mSelfDeterminingActions[category].size() + 1;
	mSelfDeterminingActions[action].try_emplace(id, action);
	return id;
}

Atlas::InputActionCallbackID Atlas::InputRegistry::registerSelfDeterminingActionCallback(InputAction* category, SelfDeterminingInputAction* action)
{
	return registerSelfDeterminingActionCallback(action, action);
}

void Atlas::InputRegistry::unregisterAction(InputAction* action) {
	mActions.erase(action);
}

void Atlas::InputRegistry::unregisterCallback(InputAction* action, Atlas::InputActionCallbackID id) {
	mActions[action].erase(id);
}

void Atlas::InputRegistry::checkAll() {
	// Iterate through all registered actions in mActions (and then through all of it's callbacks)
	for (auto const& [key, value] : mActions) {
		bool triggered = false;

		// Check all registered callbacks
		switch (key->trigger)
		{
			using enum Atlas::EInputTrigger;
		case INPUT_TRIGGER_PRESSED:
			triggered = checkPressedAction(key);
			break;
		case INPUT_TRIGGER_REPEAT:
			triggered = checkRepeatAction(key);
			break;
		case INPUT_TRIGGER_RELEASED:
			triggered = checkReleasedAction(key);
			break;
		case INPUT_TRIGGER_DOWN:
			triggered = checkDownAction(key);
			break;
		case INPUT_TRIGGER_UP:
			triggered = checkUpAction(key);
			break;
		default:
			throw std::runtime_error("Invalid trigger type");
			break;
		}

		for (auto const& [id, action] : value) {

			if (triggered) {
				action(key);
			}

			//checks.second(actions.first);
		}
	}

	// Iterate through all registered actions in mActions (and then through all of it's callbacks)
	for (auto const& [key, value] : mSelfDeterminingActions) {
		for (auto const& [id, action] : value) {
			if (action->check()) {
				action->callback();
			}

			//checks.second(actions.first);
		}
	}

}

std::unordered_map<Atlas::InputAction*, std::unordered_map<Atlas::InputActionCallbackID, std::function<void(Atlas::InputAction*)>>>& Atlas::InputRegistry::getActions() { return mActions; }
