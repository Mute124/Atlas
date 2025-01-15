#pragma once
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <string>
#include <any>
#include "Singleton.h"

namespace Atlas {
	class GlobalsContainer : Singleton<GlobalsContainer> {
	public:

		template<typename T>
		void set(const std::string& key, const T& value) {
			mData[key] = value;
		}

		void set(const std::string& key, const std::any& value) {
			mData[key] = value;
		}

		template<typename T>
		T get(const std::string& key) {
			value = std::any_cast<T>(mData[key]);
		}
	private:
		std::unordered_map<std::string, std::any> mData;
	};

	template<typename T>
	T& GetGlobal(const std::string& key) {
		std::any value;
		GlobalsContainer::Instance().get<T>(key, value);
		return std::any_cast<T>(value);
	}

	template<typename T>
	void SetGlobal(const std::string& key, const T& value) {
		GlobalsContainer::Instance().set(key, value);
	}
}
