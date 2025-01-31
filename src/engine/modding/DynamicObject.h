#pragma once
#include <iostream>
#include <unordered_map>
#include <string>
#include <type_traits>
#include <utility>

#ifdef ATLAS_ENABLE_MODDING
	#ifdef ATLAS_ENABLE_LUA

	#define SOL_ALL_SAFETIES_ON 1
		#include <sol/sol.hpp>
		#include <sol/forward.hpp>
		#include <sol/types.hpp>
	#endif

#endif




namespace Atlas {
	class DynamicObject {
	private:
		std::unordered_map<std::string, sol::object> mEntries;
	public:
		void setEntry(std::string key, sol::stack_object value) {
			auto it = mEntries.find(key);

			if (it == mEntries.cend()) {

				std::pair<const std::string, sol::object> kvp = { std::move(key), std::move(value) };
				mEntries.insert(it, kvp);
			}
			else {
				std::pair<const std::string, sol::object>& kvp = *it;
				sol::object& entry = kvp.second;
				entry = sol::object(std::move(value));
			}
		}

		sol::object getEntry(std::string key) {
			auto it = mEntries.find(key);
			if (it == mEntries.cend()) {
				return sol::lua_nil;
			}
			return it->second;
		}

	};
}