#pragma once
#include <cstdint>
#include <string>

namespace Atlas {
	struct Version {
		uint32_t major = -1;
		uint32_t minor = -1;
		uint32_t patch = -1;

		Version(uint32_t major, uint32_t minor, uint32_t patch) : major(major), minor(minor), patch(patch) {}

		Version(uint32_t major, uint32_t minor) : major(major), minor(minor), patch(0) {}

		Version() = default;

		~Version() = default;

		std::string toString() const {
			return std::to_string(major) + "." + std::to_string(minor) + "." + std::to_string(patch);
		}
	};
}
