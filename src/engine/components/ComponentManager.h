#pragma once
#include "Component.h"
#include <map>
#include <memory>
#include <string>

namespace Atlas {
	class ComponentManager {
	private:
		std::map<std::string, std::shared_ptr<Component>> mComponents;
	public:
	
	};
}