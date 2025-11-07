/**************************************************************************************************
 * @file FileData.h
 * 
 * @brief .
 * 
 * @date October 2025
 * 
 * @since v
 ***************************************************************************************************/
#pragma once
#include <iostream>
#include <vector>
#include <cstdint>

namespace Atlas {

	struct FileData {
		struct Byte {
			uint32_t byte;

			explicit(false) Byte(uint32_t b) : byte(b) {}
			explicit(false) Byte(uint8_t b) : byte(b) {}

			explicit(false) operator uint32_t() const { return byte; }
			explicit(false) operator uint8_t() const { return byte; }

			template<typename T_CAST_TO>
			T_CAST_TO to() const { return static_cast<T_CAST_TO>(byte); }

			char toChar() const { return static_cast<char>(byte); }
		};

		static inline std::vector<Byte> /*Atlas::FileData::*/getBytes(std::vector<uint8_t> bytes) {
			std::vector<Atlas::FileData::Byte> out;
			for (int i = 0; i < bytes.size(); i++) {
				out.push_back(Atlas::FileData::Byte(bytes[i]));
			}

			return out;
		}

		static inline std::vector<Byte> /*Atlas::FileData::*/getBytes(std::vector<uint32_t> bytes) {
			std::vector<Atlas::FileData::Byte> out;
			for (int i = 0; i < bytes.size(); i++) {
				out.push_back(Atlas::FileData::Byte(bytes[i]));
				std::cout << out[i].toChar();
			}

			return out;
		}

		std::vector<Byte> bytes;

		FileData(FileData&) = default;
		FileData(FileData&&) = default;

		explicit FileData(std::vector<Byte>&& b);

		explicit FileData(std::vector<uint32_t>&& b) : bytes(getBytes(b)) {}

		explicit FileData(std::vector<uint8_t>&& b) : FileData(getBytes(b)) {}

		FileData() = default;

		size_t size() const { return bytes.size(); }

		template<typename T_BYTE_CAST_TO = uint64_t>
		const T_BYTE_CAST_TO* data() const { 
			//std::vector<T_BYTE_CAST_TO> out(bytes.size());

			//for (size_t i = 0; i < bytes.size(); i++) {
			//	out[i] = bytes[i].to<T_BYTE_CAST_TO>();
			//}

			return bytes.data();
		}

		void print() {
			for (auto& byte : bytes) {
				std::cout << byte.toChar();
			}
		}

		Byte operator[](size_t i) const { return bytes[i]; }
	};
}
