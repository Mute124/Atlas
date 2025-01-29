/**
* @file Input.h
* 
* @brief The input system for Atlas.
* 
* @details The input system for Atlas works by a Laissez-faire system where the user can register InputActions (<see cref="InputAction"/>) by using functions as callbacks. The reason
* for this is because it is much easier to do it this way (and it is also more efficient & flexible). Plus, it also allows you to register multiple callbacks for the same InputAction, 
* which may be useful.
* 
* @todo Create examples of how to use @ref Atlas::InputRegistry.
* @todo Document @ref Atlas::EInputTrigger & @ref Atlas::EInputType
*/
#pragma once
#include <functional>
#include <string>
#include <unordered_map>
#include <optional>
#include "../utils/Singleton.h"

#define ATLAS_ENABLE_CONTROLLER_SUPPORT

namespace Atlas {
	
	/// <summary>
	/// The enumeration for the different input device categories. This is here because the input system is going to be expanded in the future and it will be useful to 
	/// maintaining developer sanity.
	/// </summary>
	enum class EInputType {
		INPUT_KEYBOARD,
		INPUT_MOUSE,
		INPUT_CONTROLLER
	};
	
	/// <summary>
	/// What sort of action should trigger the InputAction? 
	/// @warning Please pay attention when using this because any slight change in InputAction keys will give you entirely different input results.
	/// </summary>
	enum class EInputTrigger {
		INPUT_TRIGGER_PRESSED,
		INPUT_TRIGGER_RELEASED,
		INPUT_TRIGGER_DOWN,
		INPUT_TRIGGER_UP,
		INPUT_TRIGGER_REPEAT
	};
	
	/// <summary>
	/// Serves as a search key/input category for the input system. 
	/// @warning Please pay attention when using this because any slight change in InputAction keys will give you entirely different input results.
	/// </summary>
	/// <remarks>
	/// InputAction contains a name (std::string), an key (int), a device type (<see cref="EInputType"/>) and a trigger (<see cref="EInputTrigger"/>) that will be used to distinguish between
	/// different InputActions. Not only will it act as a key, but it will also act as a category. 
	/// </remarks>
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

	class SelfDeterminingInputAction : public InputAction {
	public:
		
		std::function<bool(const SelfDeterminingInputAction*)> mChecker;
		std::function<void(const SelfDeterminingInputAction*)> mCallback;

		SelfDeterminingInputAction(
			std::function<bool(const SelfDeterminingInputAction*)> const& checker, 
			std::function<void(const SelfDeterminingInputAction*)> const& callback
		) : mChecker(checker), mCallback(callback) {}

		SelfDeterminingInputAction() = default;

		bool check() const {
			return mChecker(this);
		}

		void callback() const {
			mCallback(this); 
		}
	};

	/// <summary>
	/// The input registry is used to register InputActions (<see cref="InputAction"/>) and their callbacks by using a Laissez-faire system. 
	/// @note Remember that this is a singleton, so you may use this statically.
	/// </summary>
	/// <seealso cref="Singleton&lt;InputRegistry&gt;" />
	class InputRegistry : public Singleton<InputRegistry> {
	protected:


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


		std::unordered_map<InputAction*, std::unordered_map<int, std::function<void(InputAction*)>>> mActions;

		std::unordered_map<
			InputAction*,
			std::unordered_map<
				int,
				SelfDeterminingInputAction*
			>
		> mSelfDeterminingActions;

		bool checkPressedAction(InputAction* action);
		bool checkReleasedAction(InputAction* action);
		bool checkDownAction(InputAction* action);
		bool checkUpAction(InputAction* action);
		bool checkRepeatAction(InputAction* action);
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
		int registerActionCallback(InputAction* action, std::function<void(InputAction*)> const& callback);

		int registerSelfDeterminingActionCallback(InputAction* category, SelfDeterminingInputAction* action, 
			std::function<bool(SelfDeterminingInputAction*)> const& checker,std::function<void(SelfDeterminingInputAction*)> const& callback);

		int registerSelfDeterminingActionCallback(InputAction* category, SelfDeterminingInputAction* action);
		

				
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
		void unregisterCallback(InputAction* action, int id);

		/// <summary>
		/// Checks all registered InputActions for callbacks.
		/// </summary>
		/// <remarks>
		/// @note Without this, the callbacks won't be called and the input system is as useful as gloves are to an armless person.
		/// </remarks>
		void checkAll();	

		std::unordered_map<InputAction*, std::unordered_map<int, std::function<void(InputAction*)>>>& getActions() { return mActions; }
	};


}