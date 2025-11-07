/**************************************************************************************************
 * @file FileData.cpp
 * 
 * @brief .
 * 
 * @date October 2025
 * 
 * @since v
 ***************************************************************************************************/
#include <vector>
#include <cstdint>
#include <type_traits>
#include <iostream>

#include "FileData.h"



Atlas::FileData::FileData(std::vector<Byte>&& b)
	: bytes(std::move(b)) {
	for (auto& byte : bytes) {
		std::cout << byte.toChar();
	}
}

