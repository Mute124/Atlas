#include "InputAction.h"

Atlas::InputAction::InputAction(const std::string& name, const std::string& registryName, int key, const EInputType& type, const EInputTrigger& trigger)
	: name(name), inputRegistryName(registryName), key(key), type(type), trigger(trigger)
{
}
