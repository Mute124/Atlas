#pragma once
#include "../utils/Singleton.h"
#include "SelfDeterminingInputAction.h"
#include <unordered_map>
#include "InputAction.h"
#include <functional>

#include "../core/ThreadSafeVariable.h"
#include <raylib.h>
#include "EInputTrigger.h"
#include "EInputType.h"
#include <stdexcept>
#include <string>

namespace Atlas {

	using InputActionCallbackID = int;

	/**
	 * @brief The input registry is used to register InputActions (<see cref="InputAction"/>) and their callbacks by using a Laissez-faire system.
	 * @note Remember that this is a singleton, so you may use this statically.
	 * @bug When using functions that require rendering, the engine will crash because the input registry runs on a different thread. This can be fixed through a sync system.
	 * @since v0.0.6
	 */
	class InputRegistry {
	public:
		class InputActionRegistry {
		public:
			std::unordered_map<InputAction*, std::unordered_map<InputActionCallbackID, std::function<void(InputAction*)>>> mActions;

			void registerAction(InputAction* action, std::function<void(InputAction*)> callback) {
				//if (!mActions.contains(action)) {
				//	mActions[action].try_emplace(mActions[action].size() + 1, callback);
				//}

				if (action == nullptr) {
					throw std::invalid_argument("InputAction cannot be null");
				}
				// Since callbacks are stored in an unordered_map, the next available id/slot is the map's size + 1.
				int id = mActions[action].size() + 1;

				mActions[action].try_emplace(id, callback);
			}

			void unregisterAction(InputAction* action) {
				mActions.erase(action);
			}

			void checkAll() {
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
			}

			bool checkPressedAction(InputAction* action)
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
			
			bool checkReleasedAction(InputAction* action)
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
			
			bool checkDownAction(InputAction* action)
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
			
			bool checkUpAction(InputAction* action)
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

			bool checkRepeatAction(InputAction* action)
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
		};
		
	private:
		//
#ifdef ATLAS_ENABLE_CONTROLLER_SUPPORT

#endif

#ifdef ATLAS_ENABLE_CONTROLLER_SUPPORT

		/// <summary>
		/// The left stick x-axis deadzone 
		/// </summary>
		const float cLeftStickDeadzoneX = 0.1f;

		/// <summary>
		/// The left stick y-axis deadzone
		/// </summary>
		const float cLeftStickDeadzoneY = 0.1f;

		/// <summary>
		/// The right stick x-axis deadzone
		/// </summary>
		const float cRightStickDeadzoneX = 0.1f;

		/// <summary>
		/// The right stick y-axis deadzone
		/// </summary>
		const float cRightStickDeadzoneY = 0.1f;

		/// <summary>
		/// The left trigger deadzone
		/// </summary>
		const float cLeftTriggerDeadzone = -0.9f;

		/// <summary>
		/// The right trigger deadzone
		/// </summary>
		const float cRightTriggerDeadzone = -0.9f;
#endif

		std::unordered_map<InputAction*, std::unordered_map<InputActionCallbackID, std::function<void(InputAction*)>>> mActions;
		
		std::unordered_map<std::string, ThreadSafeVariable<std::shared_ptr<InputActionRegistry>>> mActionRegistries;

		std::unordered_map<
			InputAction*,
			std::unordered_map<
			InputActionCallbackID,
			SelfDeterminingInputAction*
			>
		> mSelfDeterminingActions;

		bool checkPressedAction(InputAction* action)
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
		bool checkReleasedAction(InputAction* action)
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
		bool checkDownAction(InputAction* action)
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
		bool checkUpAction(InputAction* action)
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
		bool checkRepeatAction(InputAction* action)
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
	public:

		/// <summary>
		/// Registers a pointer of InputAction to the callback given.
		/// </summary>
		/// <remarks>
		/// This works as a key-value pair, where the key is the action and the 
		/// value is the callback and is the reason why the return value is important. 
		/// @warning Please do not lose it or you will not be able to unregister the callback or you may unregister the wrong one!
		/// </remarks>
		/// <param name="action">The InputAction to be used as a key.</param>
		/// <param name="callback">The callback that will be used to handle the InputAction.</param>
		/// <returns>The ID <b>of the callback</b>.</returns>
		/// 
		
		void addGroup(std::string const& name, std::shared_ptr<InputActionRegistry> group);

		/**
		 * @brief Registers a pointer of InputAction to the callback given.
		 * @remarks This works as a key-value pair, where the key is the action and the 
		 * value is the callback. This is is the reason why the return value is important. 
		 * @warning Please do not lose it or you will not be able to unregister the callback or you may unregister the wrong one!
		 * @param action 
		 * @param callback 
		 * @return 
		 */
		InputActionCallbackID registerActionCallback(InputAction* action, std::function<void(InputAction*)> const& callback);

		InputActionCallbackID registerSelfDeterminingActionCallback(InputAction* category, SelfDeterminingInputAction* action,
			std::function<bool(SelfDeterminingInputAction*)> const& checker, std::function<void(SelfDeterminingInputAction*)> const& callback);

		InputActionCallbackID registerSelfDeterminingActionCallback(InputAction* category, SelfDeterminingInputAction* action);

		/// <summary>
		/// Unregisters <b>ALL</b> callbacks for a given InputAction.
		/// </summary>
		/// <remarks>
		/// This is useful if a keybind is no longer needed and you want to remove all callbacks in that category from the input system. 
		/// For example, this is useful when the player is joining a game. See <see cref="InputAction"/> for more information on what an InputAction is.
		/// </remarks>
		/// <param name="action">The input action to unregister.</param>
		void unregisterAction(InputAction* action);

		/// <summary>
		/// Unregisters a specific callback for a given InputAction. 
		/// </summary>
		/// <param name="action">The action that owns the callback.</param>
		/// <param name="id">The callback's (the one being unregistered) identifier.</param>
		void unregisterCallback(InputAction* action, InputActionCallbackID id);

		/// <summary>
		/// Checks all registered InputActions for callbacks.
		/// </summary>
		/// <remarks>
		/// @note Without this, the callbacks won't be called and the input system is as useful as gloves are to an armless person.
		/// </remarks>
		void checkAll(std::string const& groupName);

		std::unordered_map<InputAction*, std::unordered_map<InputActionCallbackID, std::function<void(InputAction*)>>>& getActions();
	};
}