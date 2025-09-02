/**************************************************************************************************
 * @file Temporary.h
 * 
 * @brief .
 * 
 * @date September 2025
 * 
 * @since v
 ***************************************************************************************************/
#pragma once
#include <memory>

namespace Atlas {
	template<typename T>
	class Temporary {
	private:
		std::unique_ptr<T> mPtr = nullptr;
	public:


	};
}
