#pragma once
#include "Component.h"
#include <map>
#include <memory>
#include <string>
#include <unordered_set>

namespace Atlas {

	template<typename T_COMPONENT_KEY, class T_BASE_COMPONENT_TYPE>
	class ComponentRegistry {
	private:
		std::map<T_COMPONENT_KEY, std::shared_ptr<T_BASE_COMPONENT_TYPE>> mComponents;
	public:
		
	};
}