#include "InputRegistry.h"

#include <stdexcept>
#include <raylib.h>
#include "EInputTrigger.h"
#include "EInputType.h"
#include "InputAction.h"
#include "SelfDeterminingInputAction.h"
#include <functional>
#include <unordered_map>

void Atlas::InputRegistry::addGroup(std::string const& name, std::shared_ptr<InputActionRegistry> group)
{
	mActionRegistries[name] = ThreadSafeVariable<std::shared_ptr<InputActionRegistry>>(group);
}

Atlas::InputActionCallbackID Atlas::InputRegistry::registerActionCallback(InputAction* action, std::function<void(InputAction*)> const& callback) {
	if (action == nullptr) {
		throw std::invalid_argument("InputAction cannot be null");
	}
	// Since callbacks are stored in an unordered_map, the next available id/slot is the map's size + 1.
	int id = mActions[action].size() + 1;

	//mActions[action].try_emplace(id, callback);

	mActionRegistries[action->inputRegistryName].get()->registerAction(action, callback);
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

void Atlas::InputRegistry::checkAll(std::string const& groupName) {
	// Iterate through all registered actions in mActions (and then through all of it's callbacks)
	//for (auto& [key, value] : mActionRegistries) {

		mActionRegistries[groupName].get()->checkAll();
		
	
	//}

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
