#include "InputAction.h"

Atlas::InputAction::InputAction(const std::string& name, int key, const EInputType& type, const EInputTrigger& trigger)
	: name(name), key(key), type(type), trigger(trigger)
{
}
